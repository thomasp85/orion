#pragma once

#include "tree.h"
#include <CGAL/Splitters.h>

class sliding_fair_tree_2 : public tree<CGAL::Sliding_fair<CGAL::Search_traits_2<Kernel> >, 2> {
  typedef CGAL::Sliding_fair<CGAL::Search_traits_2<Kernel> > Splitter;
public:
  using tree::tree;
  ~sliding_fair_tree_2() = default;

  std::string split_type() const { return "sliding fair"; }

protected:
  Splitter create_splitter(size_t bucket, double aspect) {
    return Splitter(bucket, aspect);
  }
};
typedef cpp11::external_pointer<sliding_fair_tree_2> sliding_fair_tree_2_p;

class sliding_fair_tree_3 : public tree<CGAL::Sliding_fair<CGAL::Search_traits_3<Kernel> >, 3> {
  typedef CGAL::Sliding_fair<CGAL::Search_traits_3<Kernel> > Splitter;
public:
  using tree::tree;
  ~sliding_fair_tree_3() = default;

  std::string split_type() const { return "sliding fair"; }

protected:
  Splitter create_splitter(size_t bucket, double aspect) {
    return Splitter(bucket, aspect);
  }
};
typedef cpp11::external_pointer<sliding_fair_tree_3> sliding_fair_tree_3_p;
