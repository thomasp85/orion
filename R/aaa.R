get_ptr <- function(x) .subset2(x, 1L)

#' @importFrom euclid is_point is_weighted_point is_sphere is_circle is_iso_cube is_iso_rect is_bbox
is_valid_query <- function(x, search = TRUE) {
  if (search && (is_point(x) || is_weighted_point(x))) return(TRUE)
  if (dim(x) == 3 && (is_sphere(x) || is_iso_cube(x))) return(TRUE)
  if (dim(x) == 2 && (is_circle(x) || is_iso_rect(x))) return(TRUE)
  if (is_bbox(x)) return(TRUE)
  return(FALSE)
}
