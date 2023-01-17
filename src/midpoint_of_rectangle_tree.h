#pragma once

#include "tree.h"
#include <CGAL/Splitters.h>

class midpoint_of_rectangle_tree_2 : public tree<CGAL::Midpoint_of_rectangle<CGAL::Search_traits_2<Kernel> >, 2> {
  typedef CGAL::Midpoint_of_rectangle<CGAL::Search_traits_2<Kernel> > Splitter;
public:
  using tree::tree;
  ~midpoint_of_rectangle_tree_2() = default;

  std::string split_type() const { return "midpoint of rectangle"; }

protected:
  Splitter create_splitter(size_t bucket, double aspect) {
    return Splitter(bucket);
  }
};
typedef cpp11::external_pointer<midpoint_of_rectangle_tree_2> midpoint_of_rectangle_tree_2_p;

class midpoint_of_rectangle_tree_3 : public tree<CGAL::Midpoint_of_rectangle<CGAL::Search_traits_3<Kernel> >, 3> {
  typedef CGAL::Midpoint_of_rectangle<CGAL::Search_traits_3<Kernel> > Splitter;
public:
  using tree::tree;
  ~midpoint_of_rectangle_tree_3() = default;

  std::string split_type() const { return "midpoint of rectangle"; }

protected:
  Splitter create_splitter(size_t bucket, double aspect) {
    return Splitter(bucket);
  }
};
typedef cpp11::external_pointer<midpoint_of_rectangle_tree_3> midpoint_of_rectangle_tree_3_p;
