#include "tree.h"
#include "fair_tree.h"
#include "median_of_max_spread_tree.h"
#include "median_of_rectangle_tree.h"
#include "midpoint_of_max_spread_tree.h"
#include "midpoint_of_rectangle_tree.h"
#include "sliding_midpoint_tree.h"
#include "sliding_fair_tree.h"

// Constructors

[[cpp11::register]]
fair_tree_2_p create_fair_tree_2(SEXP points, int bucket, double aspect) {
  fair_tree_2 *tree(new fair_tree_2(points, bucket, aspect));
  return {tree};
}
[[cpp11::register]]
fair_tree_3_p create_fair_tree_3(SEXP points, int bucket, double aspect) {
  fair_tree_3 *tree(new fair_tree_3(points, bucket, aspect));
  return {tree};
}

[[cpp11::register]]
median_of_max_spread_tree_2_p create_median_of_max_spread_tree_2(SEXP points, int bucket) {
  median_of_max_spread_tree_2 *tree(new median_of_max_spread_tree_2(points, bucket, 0.0));
  return {tree};
}
[[cpp11::register]]
median_of_max_spread_tree_3_p create_median_of_max_spread_tree_3(SEXP points, int bucket) {
  median_of_max_spread_tree_3 *tree(new median_of_max_spread_tree_3(points, bucket, 0.0));
  return {tree};
}

[[cpp11::register]]
median_of_rectangle_tree_2_p create_median_of_rectangle_tree_2(SEXP points, int bucket) {
  median_of_rectangle_tree_2 *tree(new median_of_rectangle_tree_2(points, bucket, 0.0));
  return {tree};
}
[[cpp11::register]]
median_of_rectangle_tree_3_p create_median_of_rectangle_tree_3(SEXP points, int bucket) {
  median_of_rectangle_tree_3 *tree(new median_of_rectangle_tree_3(points, bucket, 0.0));
  return {tree};
}

[[cpp11::register]]
midpoint_of_max_spread_tree_2_p create_midpoint_of_max_spread_tree_2(SEXP points, int bucket) {
  midpoint_of_max_spread_tree_2 *tree(new midpoint_of_max_spread_tree_2(points, bucket, 0.0));
  return {tree};
}
[[cpp11::register]]
midpoint_of_max_spread_tree_3_p create_midpoint_of_max_spread_tree_3(SEXP points, int bucket) {
  midpoint_of_max_spread_tree_3 *tree(new midpoint_of_max_spread_tree_3(points, bucket, 0.0));
  return {tree};
}

[[cpp11::register]]
midpoint_of_rectangle_tree_2_p create_midpoint_of_rectangle_tree_2(SEXP points, int bucket) {
  midpoint_of_rectangle_tree_2 *tree(new midpoint_of_rectangle_tree_2(points, bucket, 0.0));
  return {tree};
}
[[cpp11::register]]
midpoint_of_rectangle_tree_3_p create_midpoint_of_rectangle_tree_3(SEXP points, int bucket) {
  midpoint_of_rectangle_tree_3 *tree(new midpoint_of_rectangle_tree_3(points, bucket, 0.0));
  return {tree};
}

[[cpp11::register]]
sliding_fair_tree_2_p create_sliding_fair_tree_2(SEXP points, int bucket, double aspect) {
  sliding_fair_tree_2 *tree(new sliding_fair_tree_2(points, bucket, aspect));
  return {tree};
}
[[cpp11::register]]
sliding_fair_tree_3_p create_sliding_fair_tree_3(SEXP points, int bucket, double aspect) {
  sliding_fair_tree_3 *tree(new sliding_fair_tree_3(points, bucket, aspect));
  return {tree};
}

[[cpp11::register]]
sliding_midpoint_tree_2_p create_sliding_midpoint_tree_2(SEXP points, int bucket) {
  sliding_midpoint_tree_2 *tree(new sliding_midpoint_tree_2(points, bucket, 0.0));
  return {tree};
}
[[cpp11::register]]
sliding_midpoint_tree_3_p create_sliding_midpoint_tree_3(SEXP points, int bucket) {
  sliding_midpoint_tree_3 *tree(new sliding_midpoint_tree_3(points, bucket, 0.0));
  return {tree};
}


// Basic info

[[cpp11::register]]
cpp11::writable::integers tree_dimension(tree_base_p tree) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return {int(tree->dimension())};
}

[[cpp11::register]]
cpp11::writable::strings tree_split_type(tree_base_p tree) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return {tree->split_type()};
}

[[cpp11::register]]
cpp11::writable::integers tree_size(tree_base_p tree) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return {int(tree->size())};
}

[[cpp11::register]]
cpp11::writable::integers tree_bucket_size(tree_base_p tree) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return {int(tree->bucket_size())};
}

[[cpp11::register]]
cpp11::writable::doubles tree_aspect_ratio(tree_base_p tree) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return {double(tree->aspect_ratio())};
}

[[cpp11::register]]
SEXP tree_points(tree_base_p tree) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return tree->points();
}

[[cpp11::register]]
SEXP tree_bbox(tree_base_p tree) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return tree->bbox();
}

// Searches

[[cpp11::register]]
SEXP tree_point_search(tree_base_p tree, SEXP points, cpp11::integers n, SEXP eps, bool nearest, bool sort) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return tree->point_search(points, n, eps, nearest, sort);
}

[[cpp11::register]]
SEXP tree_spheroid_search(tree_base_p tree, SEXP spheroids, cpp11::integers n, SEXP eps, bool nearest, bool sort) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return tree->spheroid_search(spheroids, n, eps, nearest, sort);
}

[[cpp11::register]]
SEXP tree_box_search(tree_base_p tree, SEXP boxes, cpp11::integers n, SEXP eps, bool nearest, bool sort) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return tree->box_search(boxes, n, eps, nearest, sort);
}

[[cpp11::register]]
SEXP tree_spheroid_range(tree_base_p tree, SEXP spheroids, SEXP eps) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return tree->spheroid_range(spheroids, eps);
}

[[cpp11::register]]
SEXP tree_box_range(tree_base_p tree, SEXP boxes, SEXP eps) {
  if (tree.get() == nullptr) {
    cpp11::stop("Data structure pointer cleared from memory");
  }
  return tree->box_range(boxes, eps);
}
