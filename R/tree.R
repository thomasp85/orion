#' Create a kd tree of points to search on
#'
#' An efficient spatial search is often done in two steps: 1) Prepare the points
#' to search in a potential costly operation, 2) Perform one or more searches on
#' the prepared data. The orion package supports efficient searching based on a
#' kd tree representation of the search space. The kd tree can be created with a
#' range of different setups, using different splitters and bucket sizes for
#' leaf nodes.
#'
#' @param x An `orion_kd_tree` object
#' @param points A `euclid_point` vector holding the points to search
#' @param split_strategy One of `"fair"`, `"sliding_fair"`, `"sliding_midpoint"`,
#' `"median_of_max_spread"`, `"median_of_rectangle"`, `"midpoint_of_max_spread"`,
#' or `"midpoint_of_rectangle"`, defining the splitting strategy to use when
#' creating new nodes in the kd tree
#' @param bucket_size The maximum number of points in the terminal nodes of the
#' kd tree
#' @param aspect For `"fair"` and `"sliding_fair` splitting strategies, defines
#' the maximum aspect ratio between the largest and smallest side of the split.
#'
#' @return An `orion_kd_tree` object
#'
#' @importFrom euclid is_point
#' @export
kd_tree <- function(points, split_strategy = "sliding_midpoint", bucket_size = 10, aspect = 3) {
  if (!is_point(points)) {
    cli_abort("{.arg points} must be a vector of {.cls euclid_point}")
  }
  bucket_size <- as.integer(bucket_size)
  if (length(bucket_size) > 1 || bucket_size < 1 || is.na(bucket_size)) {
    cli_abort("{.arg bucket_size} must be a scala integer greater or equal to 1")
  }
  aspect <- as.numeric(aspect)
  if (length(aspect) > 1 || aspect < 1 || !is.finite(aspect)) {
    cli_abort("{.arg aspect} must be a scala finite numeric greater than 1")
  }
  arg_match(
    split_strategy,
    c(
      "fair",
      "sliding_fair",
      "sliding_midpoint",
      "median_of_max_spread",
      "median_of_rectangle",
      "midpoint_of_max_spread",
      "midpoint_of_rectangle"
    )
  )
  if (dim(points) == 2) {
    new_search_tree(create_2d_tree(points, split_strategy, bucket_size, aspect))
  } else {
    new_search_tree(create_3d_tree(points, split_strategy, bucket_size, aspect))
  }
}

#' @rdname kd_tree
#' @export
is_kd_tree <- function(x) inherits(x, "orion_kd_tree")

#' @export
summary.orion_kd_tree <- function(object, ...) {
  res <- list(
    size = tree_size(get_ptr(object)),
    splitter = tree_split_type(get_ptr(object)),
    bucket_size = tree_bucket_size(get_ptr(object))
  )
  asp <- tree_aspect_ratio(get_ptr(object))
  if (asp != 0) res$aspect_ratio <- asp
  res
}

#' @export
length.orion_kd_tree <- function(x) 1L

#' @export
dim.orion_kd_tree <- function(x) {
  tree_dimension(get_ptr(x))
}

#' @export
print.orion_kd_tree <- function(x, ...) {
  info <- summary(x)
  cat("<", dim(x), "D kd tree [", info$size, "]>\n", sep = "")
  cat("Tree constructed using the ", info$splitter, " strategy\n", sep = "")
  cat(" - bucket size: ", info$bucket_size, "\n", sep = "")
  if (!is.null(info$aspect_ratio)) {
    cat(" - aspect ratio: ", info$aspect_ratio, "\n", sep = "")
  }
}

#' @importFrom euclid as_point
#' @export
as_point.orion_kd_tree <- function(x) {
  tree_points(get_ptr(x))
}
#' @importFrom euclid as_bbox
#' @export
as_bbox.orion_kd_tree <- function(x) {
  tree_bbox(get_ptr(x))
}

new_search_tree <- function(x) {
  d <- tree_dimension(x)
  x <- list(x)
  class(x) <- c(paste0("orion_kd_tree", d), "orion_kd_tree")
  x
}
create_2d_tree <- function(points, split_strategy, bucket_size, aspect) {
  switch(
    split_strategy,
    fair = create_fair_tree_2(points, bucket_size, aspect),
    sliding_fair = create_sliding_fair_tree_2(points, bucket_size, aspect),
    sliding_midpoint = create_sliding_midpoint_tree_2(points, bucket_size),
    median_of_max_spread = create_median_of_max_spread_tree_2(points, bucket_size),
    median_of_rectangle = create_median_of_rectangle_tree_2(points, bucket_size),
    midpoint_of_max_spread = create_midpoint_of_max_spread_tree_2(points, bucket_size),
    midpoint_of_rectangle = create_midpoint_of_rectangle_tree_2(points, bucket_size),
    cli_abort("Unkown {.arg split_strategy}")
  )
}
create_3d_tree <- function(points, split_strategy, bucket_size, aspect) {
  switch(
    split_strategy,
    fair = create_fair_tree_3(points, bucket_size, aspect),
    sliding_fair = create_sliding_fair_tree_3(points, bucket_size, aspect),
    sliding_midpoint = create_sliding_midpoint_tree_3(points, bucket_size),
    median_of_max_spread = create_median_of_max_spread_tree_3(points, bucket_size),
    median_of_rectangle = create_median_of_rectangle_tree_3(points, bucket_size),
    midpoint_of_max_spread = create_midpoint_of_max_spread_tree_3(points, bucket_size),
    midpoint_of_rectangle = create_midpoint_of_rectangle_tree_3(points, bucket_size),
    cli_abort("Unkown {.arg split_strategy}")
  )
}
