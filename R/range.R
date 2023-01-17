#' Locate points contained within a geometry
#'
#' While a kd tree is often used to locate nearest neighbors, it works equally
#' well for quickly looking up points contained within a geometry. This type of
#' query obviously doesn't work for points but only circles/spheres and
#' rectangles/cubes. As with [kd_tree_search()] it is possible to do a fuzzy
#' search by providing an `eps`, but it works differently. For `kd_tree_range()`
#' the `eps` argument provides a fuzzy zone around the query geometry. For
#' circle/spheres, this means that all points inside `radius - eps` will get
#' reported, points inside `radius + eps`, but outside `radius - eps` _may_ get
#' reported, and points outside `radius + eps` will not get reported. For
#' rectangles/cubes it works the same but instead it dilates and expands the box
#' by the `eps` arguments to create the fuzzy zones.
#'
#' @param geometries A vector of geometries to use for queries. Either a
#' `euclid_circle2`, `euclid_sphere`, `euclid_iso_rect`, or `euclid_iso_cube`
#' vector. `euclid_bbox` will get coerced to `euclid_iso_rect`/`euclid_iso_cube`
#' @param tree a `orion_kd_tree`
#' @param eps Fuzzyness factor for the query. See the description. Will recycle
#' to the length of `geometries`
#' @param ... Arguments passed on
#'
#' @return A list with elements `points` holding a `euclid_point` vector and `id`
#' matching the `points` to the index of `geometries`
#'
#' @family kd tree queries
#' @export
#'
#' @examples
#' # Create a kd tree with points
#' pts <- euclid::point(runif(100), runif(100))
#' tree <- kd_tree(pts)
#'
#' # Do an exact range query (eps = 0)
#' pt <- euclid::point(0.4, 0.7)
#' circ <- euclid::circle(pt, 0.02)
#' inside <- kd_tree_range(circ, tree)
#' plot(pts, cex = 1)
#' euclid_plot(inside$points, cex = 0.6, pch = 16, col = 'red')
#' euclid_plot(circ, fg = 'green')
#'
#' # Do a fuzzy range query
#' circ <- euclid::circle(pt, 0.04)
#' inside <- kd_tree_range(circ, tree, 0.1)
#' plot(pts, cex = 1)
#' euclid_plot(inside$points, cex = 0.6, pch = 16, col = 'red')
#' euclid_plot(circ, fg = 'green')
#' euclid_plot(circle(pt, 0.1^2), fg = 'green', lty = 2)
#' euclid_plot(circle(pt, 0.3^2), fg = 'green', lty = 2)
#'
kd_tree_range <- function(geometries, tree, eps = 0, ...) {
  if (!is_kd_tree(tree) || dim(tree) != dim(geometries)) {
    cli_abort("{.arg tree} must be a {.cls orion_kd_tree} matching the dimensionality of {.arg geometries}")
  }
  if (!is_valid_query(geometries, search = FALSE)) {
    cli_abort(c(
      "{.arg geometries} must be a valid geometry for a query",
      i = "Provide either a {.or {c('euclid_circle2', 'euclid_sphere', 'euclid_iso_rect', 'euclid_iso_cube')}} vector"
    ))
  }
  UseMethod("kd_tree_range")
}
#' @importFrom euclid exact_numeric
#' @export
kd_tree_range.euclid_circle2 <- function(geometries, tree, eps = 0, ...) {
  eps <- exact_numeric(eps)
  if (any(is.na(eps) || eps < 0)) {
    cli_abort("{.arg eps} must be finite and greater than or equal to 0.0")
  }
  tree_spheroid_range(get_ptr(tree), geometries, eps)
}
#' @export
kd_tree_range.euclid_sphere <- kd_tree_range.euclid_circle2
#' @importFrom euclid exact_numeric
#' @export
kd_tree_range.euclid_iso_rect <- function(geometries, tree, eps = 0, ...) {
  eps <- exact_numeric(eps)
  if (any(is.na(eps) || eps < 0)) {
    cli_abort("{.arg eps} must be finite and greater than or equal to 0.0")
  }
  tree_box_range(get_ptr(tree), geometries, eps)
}
#' @export
kd_tree_range.euclid_iso_cube <- kd_tree_range.euclid_iso_rect
#' @importFrom euclid as_iso_rect as_iso_cube
#' @export
kd_tree_range.euclid_bbox <- function(geometries, tree, eps = 0, ...) {
  if (dim(geometries) == 2) {
    geometries <- as_iso_rect(geometries)
  } else {
    geometries <- as_iso_cube(geometries)
  }
  kd_tree_range(geometries, tree, eps)
}
