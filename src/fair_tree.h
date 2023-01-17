#pragma once

#include "tree.h"
#include <CGAL/Splitters.h>

class fair_tree_2 : public tree<CGAL::Fair<CGAL::Search_traits_2<Kernel> >, 2> {
  typedef CGAL::Fair<CGAL::Search_traits_2<Kernel> > Splitter;
public:
  using tree::tree;
  ~fair_tree_2() = default;

  std::string split_type() const { return "fair"; }

protected:
  Splitter create_splitter(size_t bucket, double aspect) {
    return Splitter(bucket, aspect);
  }
};
typedef cpp11::external_pointer<fair_tree_2> fair_tree_2_p;

class fair_tree_3 : public tree<CGAL::Fair<CGAL::Search_traits_3<Kernel> >, 3> {
  typedef CGAL::Fair<CGAL::Search_traits_3<Kernel> > Splitter;
public:
  using tree::tree;
  ~fair_tree_3() = default;

  std::string split_type() const { return "fair"; }

protected:
  Splitter create_splitter(size_t bucket, double aspect) {
    return Splitter(bucket, aspect);
  }
};
typedef cpp11::external_pointer<fair_tree_3> fair_tree_3_p;
