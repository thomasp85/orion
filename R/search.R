#' Locate nearest or farthest points in a tree
#'
#' A kd tree is excellent for locating the points closest or farthest from a
#' given object. orion supports queries from points, circle/spheres, and
#' iso_rect/iso_cubes. If a point lies inside the geometry it's distance is 0.
#' If more points than requested lies inside the geometry a subset of these will
#' be returned. orion supports approximate queries through the `eps` argument.
#' Using it will speed up searches but may return wrong results (but within the
#' bounds of the given `eps`).
#'
#' @param geometries A vector of geometries to use for queries. Either a
#' `euclid_point`, `euclid_circle2`, `euclid_sphere`, `euclid_iso_rect`, or
#' `euclid_iso_cube` vector. `euclid_point_w` will get coerced to `euclid_point`
#' and `euclid_bbox` will get coerced to `euclid_iso_rect`/`euclid_iso_cube`
#' @param tree a `orion_kd_tree`
#' @param n An integer vector giving the number of points to find per query.
#' Will recycle to the length of `geometries`
#' @param eps Approximation factor for the search. For nearest neighbor the
#' returned points are no more than `1 + eps` times farther away than the true
#' nearest neighbor, whereas for furthest neighbor the returned points are no
#' less than `1/(1 + eps)` nearer than the distance to the true match. Will
#' recycle to the length of `geometries`
#' @param nearest Should nearest neighbor be search (setting it to `FALSE`
#' results in a furthest neighbor search)
#' @param sort Should the returned points be search by their distance to the
#' query
#' @param ... Arguments passed on
#'
#' @return A list with elements `points` holding a `euclid_point` vector, `id`
#' matching the `points` to the index of `geometries`, and `distance` providing
#' the distance to the query
#'
#' @family kd tree queries
#' @export
#'
#' @examples
#' # Create a kd tree with points
#' pts <- euclid::point(runif(100), runif(100))
#' tree <- kd_tree(pts)
#'
#' # Find furthest neighbor to point
#' pt <- euclid::point(0.4, 0.7)
#' neighbors <- kd_tree_search(pt, tree, 10, nearest = FALSE)
#' plot(pts, cex = 1)
#' euclid_plot(neighbors$points, cex = 0.6, pch = 16, col = 'red')
#' euclid_plot(pt, cex = 1, pch = 16, col = 'green')
#'
#' # Find nearest neighbor to circle
#' circ <- euclid::circle(pt, 0.01)
#' neighbors <- kd_tree_search(circ, tree, 10)
#' plot(pts, cex = 1)
#' euclid_plot(neighbors$points, cex = 0.6, pch = 16, col = 'red')
#' euclid_plot(circ, fg = 'green')
#'
kd_tree_search <- function(geometries, tree, n, eps = 0, nearest = TRUE, sort = TRUE, ...) {
  if (!is_kd_tree(tree) || dim(tree) != dim(geometries)) {
    cli_abort("{.arg tree} must be a {.cls orion_kd_tree} matching the dimensionality of {.arg geometries}")
  }
  if (!is_valid_query(geometries)) {
    cli_abort(c(
      "{.arg geometries} must be a valid geometry for a query",
      i = "Provide either a {.or {c('euclid_point', 'euclid_circle2', 'euclid_sphere', 'euclid_iso_rect', 'euclid_iso_cube')}} vector"
    ))
  }
  if (!is_logical(nearest, 1L) || !is_logical(sort, 1L)) {
    cli_abort("{.arg nearest} and {.arg sort} must be scalar logicals")
  }
  UseMethod("kd_tree_search")
}
#' @export
kd_tree_search.default <- function(geometries, tree, n, eps = 0, nearest = TRUE, sort = TRUE, ...) {
  cli_abort("No way to search the kd tree for proximity to {.cls {class(geometries)}} objects")
}
#' @importFrom euclid exact_numeric
#' @export
kd_tree_search.euclid_point <- function(geometries, tree, n, eps = 0, nearest = TRUE, sort = TRUE, ...) {
  n <- as.integer(n)
  if (any(is.na(n) || n < 1L)) {
    cli_abort("{.arg k} must be positive integers")
  }
  eps <- exact_numeric(eps)
  if (any(is.na(eps) || eps < 0)) {
    cli_abort("{.arg eps} must be finite and greater than or equal to 0.0")
  }
  tree_point_search(get_ptr(tree), geometries, n, eps, nearest, sort)
}
#' @importFrom euclid as_point
#' @export
kd_tree_search.euclid_point_w <- function(geometries, tree, n, eps = 0, nearest = TRUE, sort = TRUE, ...) {
  geometries <- as_point(geometries)
  kd_tree_search(geometries, tree, n, eps, nearest, sort)
}
#' @importFrom euclid exact_numeric
#' @export
kd_tree_search.euclid_circle2 <- function(geometries, tree, n, eps = 0, nearest = TRUE, sort = TRUE, ...) {
  n <- as.integer(n)
  if (any(is.na(n) || n < 1L)) {
    cli_abort("{.arg k} must be positive integers")
  }
  eps <- exact_numeric(eps)
  if (any(is.na(eps) || eps < 0)) {
    cli_abort("{.arg eps} must be finite and greater than or equal to 0.0")
  }
  tree_spheroid_search(get_ptr(tree), geometries, n, eps, nearest, sort)
}
#' @export
kd_tree_search.euclid_sphere <- kd_tree_search.euclid_circle2
#' @importFrom euclid exact_numeric
#' @export
kd_tree_search.euclid_iso_rect <- function(geometries, tree, n, eps = 0, nearest = TRUE, sort = TRUE, ...) {
  n <- as.integer(n)
  if (any(is.na(n) || n < 1L)) {
    cli_abort("{.arg k} must be positive integers")
  }
  eps <- exact_numeric(eps)
  if (any(is.na(eps) || eps < 0)) {
    cli_abort("{.arg eps} must be finite and greater than or equal to 0.0")
  }
  tree_box_search(get_ptr(tree), geometries, n, eps, nearest, sort)
}
#' @export
kd_tree_search.euclid_iso_cube <- kd_tree_search.euclid_iso_rect
#' @importFrom euclid as_iso_rect as_iso_cube
#' @export
kd_tree_search.euclid_bbox <- function(geometries, tree, n, eps = 0, nearest = TRUE, sort = TRUE, ...) {
  if (dim(geometries) == 2) {
    geometries <- as_iso_rect(geometries)
  } else {
    geometries <- as_iso_cube(geometries)
  }
  kd_tree_search(geometries, tree, n, eps, nearest, sort)
}
