#pragma once

#include <CGAL/Kd_tree.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/Search_traits_3.h>
#include <CGAL/Splitters.h>
#include <CGAL/Orthogonal_k_neighbor_search.h>
#include <CGAL/K_neighbor_search.h>
#include <CGAL/Weighted_Minkowski_distance.h>
#include <CGAL/Euclidean_distance.h>
#include <CGAL/Euclidean_distance_sphere_point.h>
#include <CGAL/Manhattan_distance_iso_box_point.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/Fuzzy_sphere.h>

#include <vector>
#include <string>
#include <typeinfo>

#include <cpp11/strings.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/integers.hpp>
#include <cpp11/list.hpp>
#include <cpp11/doubles.hpp>

#include <euclid.h>

using namespace cpp11::literals;

class tree_base {
public:
  tree_base() {}
  tree_base(SEXP points, size_t bucket, double aspect) {}
  virtual ~tree_base() = default;

  // Type
  virtual size_t dimension() const = 0;
  virtual std::string split_type() const = 0;
  virtual size_t bucket_size() const = 0;
  virtual double aspect_ratio() const = 0;

  // Content
  virtual size_t size() const = 0;
  virtual SEXP points() const = 0;
  virtual SEXP bbox() const = 0;

  // Search
  //virtual SEXP point_search(SEXP points, size_t n, Exact_number eps, bool nearest, bool sort, bool minkowski, double p, cpp11::doubles w) const = 0;
  virtual cpp11::writable::list point_search(SEXP points, cpp11::integers n, SEXP eps, bool nearest, bool sort) const = 0;
  virtual cpp11::writable::list spheroid_search(SEXP spheroids, cpp11::integers n, SEXP eps, bool nearest, bool sort) const = 0;
  virtual cpp11::writable::list box_search(SEXP boxes, cpp11::integers n, SEXP eps, bool nearest, bool sort) const = 0;

  virtual cpp11::writable::list spheroid_range(SEXP spheroids, SEXP eps) const = 0;
  virtual cpp11::writable::list box_range(SEXP boxes, SEXP eps) const = 0;
};
typedef cpp11::external_pointer<tree_base> tree_base_p;

template<typename T>
inline SEXP create_euclid_vec(std::vector<T>& geo);

template<>
inline SEXP create_euclid_vec(std::vector<Point_2>& geo) {
  return euclid::create_point_2_vec(geo);
}
template<>
inline SEXP create_euclid_vec(std::vector<Point_3>& geo) {
  return euclid::create_point_3_vec(geo);
}
template<>
inline SEXP create_euclid_vec(std::vector<Circle_2>& geo) {
  return euclid::create_circle_2_vec(geo);
}
template<>
inline SEXP create_euclid_vec(std::vector<Sphere>& geo) {
  return euclid::create_sphere_vec(geo);
}
template<>
inline SEXP create_euclid_vec(std::vector<Iso_rectangle>& geo) {
  return euclid::create_iso_rect_vec(geo);
}
template<>
inline SEXP create_euclid_vec(std::vector<Iso_cuboid>& geo) {
  return euclid::create_iso_cube_vec(geo);
}
template<typename T>
inline std::vector<T> get_euclid_vec(SEXP geo);

template<>
inline std::vector<Point_2> get_euclid_vec(SEXP geo) {
  return euclid::get_point_2_vec(geo);
}
template<>
inline std::vector<Point_3> get_euclid_vec(SEXP geo) {
  return euclid::get_point_3_vec(geo);
}
template<>
inline std::vector<Circle_2> get_euclid_vec(SEXP geo) {
  return euclid::get_circle_2_vec(geo);
}
template<>
inline std::vector<Sphere> get_euclid_vec(SEXP geo) {
  return euclid::get_sphere_vec(geo);
}
template<>
inline std::vector<Iso_rectangle> get_euclid_vec(SEXP geo) {
  return euclid::get_iso_rect_vec(geo);
}
template<>
inline std::vector<Iso_cuboid> get_euclid_vec(SEXP geo) {
  return euclid::get_iso_cube_vec(geo);
}

template<typename Splitter, size_t dim>
class tree : public tree_base {
  typedef typename std::conditional<dim == 2, Point_2, Point_3>::type Point;
  typedef typename std::conditional<dim == 2, Circle_2, Sphere>::type Spheroid;
  typedef typename std::conditional<dim == 2, Iso_rectangle, Iso_cuboid>::type Box;
  typedef typename std::conditional< dim == 2, CGAL::Search_traits_2<Kernel>, CGAL::Search_traits_3<Kernel> >::type Traits;
  typedef CGAL::Kd_tree<Traits, Splitter, CGAL::Tag_true> Tree;

protected:
  Tree _tree;
  size_t _bucket;
  double _aspect;
  virtual Splitter create_splitter(size_t bucket, double aspect) {
    return Splitter(bucket);
  }

public:
  tree(SEXP points, size_t bucket, double aspect) : _tree(create_splitter(bucket, aspect)), _bucket(bucket), _aspect(aspect) {
    std::vector<Point> p = get_euclid_vec<Point>(points);
    _tree.insert(p.begin(), p.end());
    _tree.build();
  }
  ~tree() = default;

  size_t dimension() const { return dim; }

  size_t size() const { return _tree.size(); }
  size_t bucket_size() const { return _bucket; }
  double aspect_ratio() const { return _aspect; }
  SEXP points() const {
    std::vector<Point> res(_tree.begin(), _tree.end());
    return create_euclid_vec(res);
  }
  SEXP bbox() const {
    if (dim == 2) {
      std::vector<Iso_rectangle> ir;
      ir.emplace_back(Point_2(_tree.bounding_box().min_coord(0), _tree.bounding_box().min_coord(1)),
                      Point_2(_tree.bounding_box().max_coord(0), _tree.bounding_box().max_coord(1)));
      return create_euclid_vec(ir);
    } else {
      std::vector<Iso_cuboid> ic;
      ic.emplace_back(Point_3(_tree.bounding_box().min_coord(0), _tree.bounding_box().min_coord(1), _tree.bounding_box().min_coord(2)),
                      Point_3(_tree.bounding_box().max_coord(0), _tree.bounding_box().max_coord(1), _tree.bounding_box().max_coord(2)));
      return create_euclid_vec(ic);
    }
  }

  //SEXP point_search(SEXP points, size_t n, Exact_number eps, bool nearest, bool sort, bool minkowski, double p, cpp11::doubles w) const {
  //  std::vector<Point> pts = get_euclid_vec<Point>(points);
  //  if (minkowski) {
  //    if (dim != w.size()) {
  //      cpp11::stop("A weight for each dimension must be given");
  //    }
  //    typedef CGAL::Weighted_Minkowski_distance<Traits> Dist;
  //    typedef CGAL::Orthogonal_k_neighbor_search<Traits, Dist, Splitter, Tree> Search;
  //    Dist dist(p, dim, w.begin(), w.end());
  //    return search_impl<Point, Dist, Search>(pts, n, dist, eps, nearest, sort);
  //  } else {
  //    typedef CGAL::Euclidean_distance<Traits> Dist;
  //    typedef CGAL::Orthogonal_k_neighbor_search<Traits, Dist, Splitter, Tree> Search;
  //    Dist dist;
  //    return search_impl<Point, Dist, Search>(pts, n, dist, eps, nearest, sort);
  //  }
  //}

  cpp11::writable::list point_search(SEXP points, cpp11::integers n, SEXP eps, bool nearest, bool sort) const {
    typedef CGAL::Euclidean_distance<Traits> Dist;
    typedef CGAL::Orthogonal_k_neighbor_search<Traits, Dist, Splitter, Tree> Search;
    std::vector<Point> pts = get_euclid_vec<Point>(points);
    Dist dist;
    return search_impl<Point, Dist, Search>(pts, n, dist, eps, nearest, sort);
  }

  cpp11::writable::list spheroid_search(SEXP spheroids, cpp11::integers n, SEXP eps, bool nearest, bool sort) const {
    typedef CGAL::Euclidean_distance_sphere_point<Traits> Dist;
    typedef CGAL::K_neighbor_search<Traits, Dist, Splitter, Tree> Search;
    std::vector<Spheroid> sph = get_euclid_vec<Spheroid>(spheroids);
    Dist dist;
    return search_impl<Spheroid, Dist, Search>(sph, n, dist, eps, nearest, sort);
  }

  cpp11::writable::list box_search(SEXP boxes, cpp11::integers n, SEXP eps, bool nearest, bool sort) const {
    typedef CGAL::Manhattan_distance_iso_box_point<Traits> Dist;
    typedef CGAL::K_neighbor_search<Traits, Dist, Splitter, Tree> Search;
    std::vector<Box> box = get_euclid_vec<Box>(boxes);
    Dist dist;
    return search_impl<Box, Dist, Search>(box, n, dist, eps, nearest, sort);
  }

  cpp11::writable::list spheroid_range(SEXP spheroids, SEXP eps) const {
    std::vector<Spheroid> sph = get_euclid_vec<Spheroid>(spheroids);
    std::vector<Exact_number> eps_vec = euclid::get_exact_numeric_vec(eps);
    std::vector<Point> res;
    cpp11::writable::integers ids;
    for (size_t i = 0; i < sph.size(); ++i) {
      std::vector<Point> temp_res;
      CGAL::Fuzzy_sphere<Traits> fs(sph[i].center(), Exact_number(CGAL::sqrt(CGAL::to_double(sph[i].squared_radius().exact()))), eps_vec[i % eps_vec.size()]);
      _tree.search(std::back_inserter(temp_res), fs);
      for (auto iter = temp_res.begin(); iter != temp_res.end(); iter++) {
        res.push_back(*iter);
        ids.push_back(i + 1);
      }
    }
    return cpp11::writable::list({
      "points"_nm = create_euclid_vec(res),
      "id"_nm = ids
    });
  }

  cpp11::writable::list box_range(SEXP boxes, SEXP eps) const {
    std::vector<Box> box = get_euclid_vec<Box>(boxes);
    std::vector<Exact_number> eps_vec = euclid::get_exact_numeric_vec(eps);
    std::vector<Point> res;
    cpp11::writable::integers ids;
    for (size_t i = 0; i < box.size(); ++i) {
      std::vector<Point> temp_res;
      CGAL::Fuzzy_iso_box<Traits> fb(box[i].min(), box[i].max(), eps_vec[i % eps_vec.size()]);
      _tree.search(std::back_inserter(temp_res), fb);
      for (auto iter = temp_res.begin(); iter != temp_res.end(); iter++) {
        res.push_back(*iter);
        ids.push_back(i + 1);
      }
    }
    return cpp11::writable::list({
      "points"_nm = create_euclid_vec(res),
      "id"_nm = ids
    });
  }

private:
  template<typename Q, typename D, typename S>
  cpp11::writable::list search_impl(std::vector<Q>& queries, cpp11::integers n, D dist, SEXP eps, bool nearest, bool sort) const {
    std::vector<Exact_number> eps_vec = euclid::get_exact_numeric_vec(eps);
    std::vector<Point> res;
    cpp11::writable::integers ids;
    cpp11::writable::doubles distances;
    size_t i = 0;
    for (auto iter = queries.begin(); iter != queries.end(); iter++) {
      S s(_tree, *iter, n[i % n.size()], eps_vec[i % eps_vec.size()], nearest, dist, sort);
      i++;
      for (auto iter_p = s.begin(); iter_p != s.end(); iter_p++) {
        res.push_back(iter_p->first);
        ids.push_back(i);
        distances.push_back(CGAL::to_double(iter_p->second));
      }
    }
    return cpp11::writable::list({
      "points"_nm = create_euclid_vec(res),
      "id"_nm = ids,
      "distance"_nm = distances
    });
  }
};
