# Generated by cpp11: do not edit by hand

create_fair_tree_2 <- function(points, bucket, aspect) {
  .Call(`_orion_create_fair_tree_2`, points, bucket, aspect)
}

create_fair_tree_3 <- function(points, bucket, aspect) {
  .Call(`_orion_create_fair_tree_3`, points, bucket, aspect)
}

create_median_of_max_spread_tree_2 <- function(points, bucket) {
  .Call(`_orion_create_median_of_max_spread_tree_2`, points, bucket)
}

create_median_of_max_spread_tree_3 <- function(points, bucket) {
  .Call(`_orion_create_median_of_max_spread_tree_3`, points, bucket)
}

create_median_of_rectangle_tree_2 <- function(points, bucket) {
  .Call(`_orion_create_median_of_rectangle_tree_2`, points, bucket)
}

create_median_of_rectangle_tree_3 <- function(points, bucket) {
  .Call(`_orion_create_median_of_rectangle_tree_3`, points, bucket)
}

create_midpoint_of_max_spread_tree_2 <- function(points, bucket) {
  .Call(`_orion_create_midpoint_of_max_spread_tree_2`, points, bucket)
}

create_midpoint_of_max_spread_tree_3 <- function(points, bucket) {
  .Call(`_orion_create_midpoint_of_max_spread_tree_3`, points, bucket)
}

create_midpoint_of_rectangle_tree_2 <- function(points, bucket) {
  .Call(`_orion_create_midpoint_of_rectangle_tree_2`, points, bucket)
}

create_midpoint_of_rectangle_tree_3 <- function(points, bucket) {
  .Call(`_orion_create_midpoint_of_rectangle_tree_3`, points, bucket)
}

create_sliding_fair_tree_2 <- function(points, bucket, aspect) {
  .Call(`_orion_create_sliding_fair_tree_2`, points, bucket, aspect)
}

create_sliding_fair_tree_3 <- function(points, bucket, aspect) {
  .Call(`_orion_create_sliding_fair_tree_3`, points, bucket, aspect)
}

create_sliding_midpoint_tree_2 <- function(points, bucket) {
  .Call(`_orion_create_sliding_midpoint_tree_2`, points, bucket)
}

create_sliding_midpoint_tree_3 <- function(points, bucket) {
  .Call(`_orion_create_sliding_midpoint_tree_3`, points, bucket)
}

tree_dimension <- function(tree) {
  .Call(`_orion_tree_dimension`, tree)
}

tree_split_type <- function(tree) {
  .Call(`_orion_tree_split_type`, tree)
}

tree_size <- function(tree) {
  .Call(`_orion_tree_size`, tree)
}

tree_bucket_size <- function(tree) {
  .Call(`_orion_tree_bucket_size`, tree)
}

tree_aspect_ratio <- function(tree) {
  .Call(`_orion_tree_aspect_ratio`, tree)
}

tree_points <- function(tree) {
  .Call(`_orion_tree_points`, tree)
}

tree_bbox <- function(tree) {
  .Call(`_orion_tree_bbox`, tree)
}

tree_point_search <- function(tree, points, n, eps, nearest, sort) {
  .Call(`_orion_tree_point_search`, tree, points, n, eps, nearest, sort)
}

tree_spheroid_search <- function(tree, spheroids, n, eps, nearest, sort) {
  .Call(`_orion_tree_spheroid_search`, tree, spheroids, n, eps, nearest, sort)
}

tree_box_search <- function(tree, boxes, n, eps, nearest, sort) {
  .Call(`_orion_tree_box_search`, tree, boxes, n, eps, nearest, sort)
}

tree_spheroid_range <- function(tree, spheroids, eps) {
  .Call(`_orion_tree_spheroid_range`, tree, spheroids, eps)
}

tree_box_range <- function(tree, boxes, eps) {
  .Call(`_orion_tree_box_range`, tree, boxes, eps)
}
