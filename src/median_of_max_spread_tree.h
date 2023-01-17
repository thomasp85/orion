#pragma once

#include "tree.h"
#include <CGAL/Splitters.h>

class median_of_max_spread_tree_2 : public tree<CGAL::Median_of_max_spread<CGAL::Search_traits_2<Kernel> >, 2> {
  typedef CGAL::Median_of_max_spread<CGAL::Search_traits_2<Kernel> > Splitter;
public:
  using tree::tree;
  ~median_of_max_spread_tree_2() = default;

  std::string split_type() const { return "median of max spread"; }

protected:
  Splitter create_splitter(size_t bucket, double aspect) {
    return Splitter(bucket);
  }
};
typedef cpp11::external_pointer<median_of_max_spread_tree_2> median_of_max_spread_tree_2_p;

class median_of_max_spread_tree_3 : public tree<CGAL::Median_of_max_spread<CGAL::Search_traits_3<Kernel> >, 3> {
  typedef CGAL::Median_of_max_spread<CGAL::Search_traits_3<Kernel> > Splitter;
public:
  using tree::tree;
  ~median_of_max_spread_tree_3() = default;

  std::string split_type() const { return "median of max spread"; }

protected:
  Splitter create_splitter(size_t bucket, double aspect) {
    return Splitter(bucket);
  }
};
typedef cpp11::external_pointer<median_of_max_spread_tree_3> median_of_max_spread_tree_3_p;
