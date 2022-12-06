#ifndef H_COMMON
#define H_COMMON

#include <math.h>
#include <numeric>
#include <limits>

namespace dt {

template <typename T> struct Point2D {
  T x;
  T y;
};

struct Edge {
  /// Store indexes into points[]
  int fromPoint;
  int toPoint;
  bool isBad = false;
};

struct Triangle {
  /// Store indexes into points[]
  int aIdx;
  int bIdx;
  int cIdx;
  bool isBad = false;
};

inline bool almost_equal(const float x, const float y) {
  float ulpFloat = static_cast<float>(2);
  return fabsf(x - y) <= std::numeric_limits<float>::epsilon() * fabsf(x + y) * ulpFloat ||
         fabsf(x - y) < std::numeric_limits<float>::min();
}

template <typename T> inline bool almost_equal(const dt::Point2D<T> &v1, const dt::Point2D<T> &v2) {
  return almost_equal(v1.x, v2.x) && almost_equal(v1.y, v2.y);
}

inline bool almost_equal(const dt::Edge &e1, const dt::Edge &e2) {
  return (almost_equal(e1.fromPoint, e2.fromPoint) && almost_equal(e1.toPoint, e2.toPoint)) ||
         (almost_equal(e1.fromPoint, e2.toPoint) && almost_equal(e1.toPoint, e2.fromPoint));
}

template <typename T>
inline bool containsPoint(const dt::Point2D<T> *points, const dt::Triangle &triangle,
                          const dt::Point2D<T> &point) {
  return almost_equal(points[triangle.aIdx], point) || almost_equal(points[triangle.bIdx], point) ||
         almost_equal(points[triangle.cIdx], point);
}

template <typename T> T norm2(const dt::Point2D<T> &p) { return p.x * p.x + p.y * p.y; }

template <typename T> inline T dist2(const dt::Point2D<T> &p1, const dt::Point2D<T> &p2) {
  const T dx = p1.x - p2.x;
  const T dy = p1.y - p2.y;
  return dx * dx + dy * dy;
}

template <typename T>
inline bool circumCircleContains(const dt::Triangle &triangle, const dt::Point2D<T> &point,
                                 const dt::Point2D<T> *points) {
  const T ab = norm2(points[triangle.aIdx]);
  const T cd = norm2(points[triangle.bIdx]);
  const T ef = norm2(points[triangle.cIdx]);

  const T ax = points[triangle.aIdx].x;
  const T ay = points[triangle.aIdx].y;
  const T bx = points[triangle.bIdx].x;
  const T by = points[triangle.bIdx].y;
  const T cx = points[triangle.cIdx].x;
  const T cy = points[triangle.cIdx].y;

  const T circum_x = (ab * (cy - by) + cd * (ay - cy) + ef * (by - ay)) /
                     (ax * (cy - by) + bx * (ay - cy) + cx * (by - ay));
  const T circum_y = (ab * (cx - bx) + cd * (ax - cx) + ef * (bx - ax)) /
                     (ay * (cx - bx) + by * (ax - cx) + cy * (bx - ax));

  const dt::Point2D<T> circum = {circum_x / 2, circum_y / 2};
  const T circum_radius = dist2(points[triangle.aIdx], circum);
  const T dist = dist2(point, circum);
  return dist <= circum_radius;
}

} // namespace dt

#endif
