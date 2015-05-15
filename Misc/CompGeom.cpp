#include "../Defines.h"

/* START SOLUTION */

struct v2d {
  dbl x, y;

  v2d operator+(const v2d& o) const {
    return {x + o.x, y + o.y};
  }
  v2d operator-(const v2d& o) const {
    return {x - o.x, y - o.y};
  }
  v2d operator*(const dbl mul) const {
    return {mul * x, mul * y};
  }
  v2d operator/(const dbl div) const {
    return {x / div, y / div};
  }
  bool operator==(const v2d& o) const {
    return equ(x, o.x) && equ(y, o.y);
  }
  bool operator!=(const v2d& o) const {
    return !(*this == o);
  }
  bool operator<(const v2d& o) const {
    if (x != o.x) return x < o.x;
    return y < o.y;
  }
};

struct line2d {
  v2d a, b;

  bool operator==(const line2d& o) const {
    return a == o.a && b == o.b;
  }
  bool operator!=(const line2d& o) const {
    return !(*this == o);
  }
  bool operator<(const line2d& o) const {
    if (a != o.a) return a < o.a;
    return b < o.b;
  }
};

typedef vector<v2d> poly2d;

dbl mag2d(const v2d& v) {
  return sqrt(v.x * v.x + v.y * v.y);
} 

dbl dist2d(const v2d& a, const v2d& b) {
  return mag2d(a - b);
}

v2d normalise2d(const v2d& v) {
  return v / mag2d(v);
}

dbl cross2d(const v2d& a, const v2d& b) {
  return a.x * b.y - a.y * b.x;
}

dbl cross_at2d(const v2d& o, const v2d& a, const v2d& b) {
  return cross2d(a - o, b - o);
}

// Strict.
bool is_strictly_left_of(const v2d& p, const line2d& l) {
  return gt(cross_at2d(l.a, l.b, p), 0.0);
}

// Strict.
bool is_strictly_right_of(const v2d& p, const line2d& l) {
  return lt(cross_at2d(l.a, l.b, p), 0.0);
}

dbl dot2d(const v2d& a, const v2d& b) {
  return a.x * b.x + a.y * b.y;
}

v2d perp2d(const v2d& v) {
  return {-v.y, v.x};
}

v2d reflect2d(const v2d& r, const v2d& n) {
  v2d normn = normalise2d(n);
  return normn * dot2d(normn, r) * 2.0 - r;
}

// REQUIRES NON-ZERO LENGTH LINE.
dbl prop_along_line2d(const v2d& p, const line2d& l) {
  v2d AP = p - l.a;
  v2d AB = l.b - l.a;
  if (equ(AB.x, 0.0)) {
    return AP.y / AB.y;
  } else {
    return AP.x / AB.x;
  }
}

bool point_on_line_inf2d(const v2d& p, const line2d& l) {
  return equ(cross_at2d(l.a, l.b, p), 0.0);
}

bool point_on_line_segment2d(
    const v2d& p, const line2d& l, bool include) {
  dbl l_minx = min(l.a.x, l.b.x);
  dbl l_maxx = max(l.a.x, l.b.x);
  dbl l_miny = min(l.a.y, l.b.y);
  dbl l_maxy = max(l.a.y, l.b.y);
  // To not include endpoints, remove eq's.
  if (!point_on_line_inf2d(p, l)) return false;
  if (p == l.a || p == l.b) return include;
  return gteq(p.x, l_minx) &&
         lteq(p.x, l_maxx) &&
         gteq(p.y, l_miny) &&
         lteq(p.y, l_maxy);
}

// In the case of infinite intersections, aprop, bprop and
// inter_line are computed with respect to line |a|'s
// direction and the first point of intersection being the
// first point on |a|.
bool inter2d(
    const line2d& A, const line2d& B, bool include,
    dbl& aprop, dbl& bprop,
    line2d& inter_line, bool& infinite) {
  if ((A.a == B.a && A.b == B.b) || 
      (A.a == B.b && A.b == B.a)) {
    inter_line = A;
    aprop = 0.0;
    bprop = prop_along_line2d(A.a, B);
    infinite = !(A.a == A.b);
    return true;
  }
  dbl a = B.a.x - A.a.x;
  dbl b = B.b.x - B.a.x;
  dbl c = A.b.x - A.a.x;
  dbl d = B.a.y - A.a.y;
  dbl e = B.b.y - B.a.y;
  dbl f = A.b.y - A.a.y;
  dbl bfce = b * f - c * e;

  if (equ(bfce, 0.0)) {
    vector<pair<dbl, v2d>> points;
    if (point_on_line_segment2d(A.a, B, true)) {
      points.emplace_back(0.0, A.a);
    }
    if (point_on_line_segment2d(A.b, B, true)) {
      points.emplace_back(1.0, A.b);
    }
    if (point_on_line_segment2d(B.a, A, true)) {
      points.emplace_back(prop_along_line2d(B.a, A), B.a);
    }
    if (point_on_line_segment2d(B.b, A, true)) {
      points.emplace_back(prop_along_line2d(B.b, A), B.b);
    }
    sort(points.begin(), points.end());

    if (points.size() == 0) {
      return false;
    } else {
      inter_line = {points.front().second, points.back().second};
    if (inter_line.a == inter_line.b) {
      infinite = false;
      if (!include) {
        return false;
      }
    } else {
      infinite = true;
    }
      aprop = prop_along_line2d(inter_line.a, A);
      bprop = prop_along_line2d(inter_line.a, B);
      return true;
    }
  }

  infinite = false;
  aprop = (b * d - a * e) / bfce;
  bprop = (c * d - a * f) / bfce;
  v2d inter_point = A.a + (A.b - A.a) * aprop;
  inter_line = {inter_point, inter_point};
  return gti(aprop, 0.0, include) && lti(aprop, 1.0, include) &&
         gti(bprop, 0.0, include) && lti(bprop, 1.0, include);
}

bool colinear2d(const v2d& a, const v2d& b, const v2d& c) {
  return equ(cross_at2d(a, b, c), 0.0);
}


v2d c2dref;
bool convex2dcomp(const v2d& a, const v2d& b) {
  dbl det = cross_at2d(c2dref, a, b);
  if (equ(det, 0.0)) {
    return lt(dist2d(c2dref, a), dist2d(c2dref, b));
  } else {
    return gt(det, 0.0);
  }
}

// Produces CCW points not including the last point.
// CHECK RETURN VALUE FOR >= 3 POINTS
poly2d convex2d(vector<v2d> pts) {
  int min_i = 0;
  for (int i = 1; i < pts.size(); ++i) {
    if (lt(pts[i].x, pts[min_i].x) ||
        (equ(pts[i].x, pts[min_i].x) &&
         lt(pts[i].y, pts[min_i].y))) {
      min_i = i;
    }
  }
  swap(pts[min_i], pts.back());
  c2dref = pts.back();
  pts.pop_back();
  sort(pts.begin(), pts.end(), convex2dcomp);
  vector<v2d> convex = {c2dref, pts[0]};
  for (int i = 1; i < pts.size(); ++i) {
    convex.push_back(pts[i]);
    while (convex.size() >= 3) {
      v2d& top = convex[convex.size() - 1];
      v2d& mid = convex[convex.size() - 2];
      v2d& st = convex[convex.size() - 3];
      if (!is_strictly_left_of(top, {st, mid})) {
        swap(mid, top);
        convex.pop_back();
      } else {
        break;
      }
    }
  }
  return convex;
}

// DOESN'T HANDLE HOLES. DOESN'T INCLUDE BOUNDARY.
// SELF-INTERSECTING COUNTED AS INTERSECTION.
// REQUIRES CCW POLYGON.
bool point_in_poly2d(const v2d& v, const poly2d& poly) {
  int wn = 0;
  for (int i = 0; i < poly.size(); ++i) {
    const v2d& a = poly[i];
    const v2d& b = poly[(i + 1) % poly.size()];
    if (a.y <= v.y) {
		if (b.y > v.y && is_strictly_left_of(v, {a, b})) {
        wn++;
	  }
    } else if (b.y <= v.y && is_strictly_right_of(v, {a, b})) {
      wn--;
    }
  }
  
  return wn != 0;
}

dbl area2d(const poly2d& poly) {
  if (poly.size() == 0) return 0.0;
  dbl area = 0.0;
  for (int i = 1; i < poly.size(); ++i) {
    const v2d& a = poly[i];
    const v2d& b = poly[(i + 1) % poly.size()];
    area += cross_at2d(poly[0], a, b);
  }
  return area / 2.0;
}

// Requires NON-ZERO POLY.
v2d centroid2d(const poly2d& poly) {
  dbl x = 0.0;
  dbl y = 0.0;
  dbl area = area2d(poly);
  for (int i = 0; i < poly.size(); ++i) {
    const v2d& a = poly[i];
    const v2d& b = poly[(i + 1) % poly.size()];
    dbl cross = cross2d(a, b);
    x += (a.x + b.x) * cross;
    y += (a.y + b.y) * cross;
  }

  return {x / (6.0 * area), y / (6.0 * area)};
}

// REQUIRES NON-ZERO LENGTH LINE.
v2d project_point_onto_line2d(const v2d& p, const line2d& l) {
  v2d AP = l.a - p;
  v2d AB = l.b - l.a;
  return l.a + AB * dot2d(AP, AB) / dot2d(AB, AB);
}

dbl dist_to_inf_line2d(const v2d& p, const line2d& l) {
  v2d projected = project_point_onto_line2d(p, l);
  return dist2d(projected, p);
}

dbl dist_to_line_segment_2d(const v2d& p, const line2d& l) {
  v2d projected = project_point_onto_line2d(p, l);
  if (point_on_line_segment2d(projected, l, true)) {
    return dist2d(projected, p);
  }

  return min(dist2d(p, l.a), dist2d(p, l.b));
}

// 3D
struct v3d {
  dbl x, y, z;

  v3d operator+(const v3d& o) const {
    return {x + o.x, y + o.y, z + o.z};
  }
  v3d operator-(const v3d& o) const {
    return {x - o.x, y - o.y, z - o.z};
  }
  v3d operator*(const dbl mul) const {
    return {x * mul, y * mul, z * mul};
  }
  v3d operator/(const dbl div) const {
    return {x * div, y * div, z * div};
  }
  bool operator==(const v3d& o) const {
    return equ(x, o.x) && equ(y, o.y) && equ(z, o.z);
  }
  bool operator!=(const v3d& o) const {
    return !(*this == o);
  }
  bool operator<(const v3d& o) const {
    if (x != o.x) return x < o.x;
    if (y != o.y) return y < o.y;
    return z < o.z;
  }
};

struct line3d {
  v3d a, b;

  bool operator==(const line3d& o) const {
    return a == o.a && b == o.b;
  }
  bool operator!=(const line3d& o) const {
    return !(*this == o);
  }
  bool operator<(const line3d& o) const {
    if (a != o.a) return a < o.a;
    return b < o.b;
  }
};

typedef vector<v3d> poly3d;
dbl mag3d(const v3d& v) {
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

dbl dist3d(const v3d& a, const v3d& b) {
  return mag3d(a - b);
}

v3d normalise3d(const v3d& v) {
  return v / mag3d(v);
}

v3d cross3d(const v3d& a, const v3d& b) {
  dbl x = a.y * b.z - a.z * b.y;
  dbl y = a.z * b.x - a.x * b.z;
  dbl z = a.x * b.y - a.y * b.x;
  return {x, y, z};
}

v3d cross_at3d(const v3d& o, const v3d& a, const v3d& b) {
  return cross3d(a - o, b - o);
}

dbl dot3d(const v3d& a, const v3d& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

v3d perp3d(const v3d& a, const v3d& b) {
  return normalise3d(cross3d(a, b));
}

v3d reflect3d(const v3d& r, const v3d& n) {
  v3d normn = normalise3d(n);
  return normn * dot3d(r, normn) * 2.0 - r;
}

/* END SOLUTION */
#include <cstdio>

void print_v2d(const v2d& v) {
  printf("(%.3Lf, %.3Lf)", v.x, v.y);
}

void print_line2d(const line2d& v) {
  printf("{");
  print_v2d(v.a);
  printf(", ");
  print_v2d(v.b);
  printf("}");
}

void assert_equal_int(const dbl& expected, const dbl& actual) {
  if (!equ(expected, actual)) {
    printf("ASSERTION:\nExpected: %.3Lf\nGot: %.3Lf\n", expected, actual);
    exit(1);
  }
}

void assert_equal_int(const v2d& expected, const v2d& actual) {
  if (expected != actual) {
    printf("ASSERTION:\nExpected: ");
    print_v2d(expected);
    printf("\nGot: ");
    print_v2d(actual);
    printf("\n");
    exit(1);
  }
}

void assert_equal_int(const line2d& expected, const line2d& actual) {
  if (expected != actual) {
    printf("ASSERTION:\nExpected: ");
    print_line2d(expected);
    printf("\nGot: ");
    print_line2d(actual);
    printf("\n");
    exit(1);
  }
}

void assert_equal_int(const poly2d& expected, const poly2d& actual) {
  if (expected.size() != actual.size()) goto fail;
  for (int i = 0; i < expected.size(); ++i) {
    if (expected[i] != actual[i]) goto fail;
  }
  return;

fail:
  printf("ASSERTION\nExpected: ");
  for (auto i : expected) {
    print_v2d(i);
    printf(" ");
  }
  printf("\nActual: ");
  for (auto i : actual) {
    print_v2d(i);
    printf(" ");
  }
  printf("\n");
  exit(1);
}

#define assert ASSERT
#define assert_equal(A, B) printf("LINE: %d -- assert_equal(%s, %s)\n", __LINE__, #A, #B); assert_equal_int(A, B);
#include <cassert>

void test() {
  // dbl mag2d(const v2d& v);
  assert_equal(sqrt(2), mag2d({1, 1}));
  assert_equal(2, mag2d({0, 2}));
  assert_equal(0, mag2d({0, 0}));
  
  // dbl dist2d(const v2d& a, const v2d& b);
  assert_equal(sqrt(2), dist2d({0, 0}, {1, 1}));
  assert_equal(sqrt(2), dist2d({2, 2}, {1, 1}));

  // v2d normalise2d(const v2d& v);
  assert_equal((v2d{1 / sqrt(2), 1 / sqrt(2)}), normalise2d({100, 100}));
  assert_equal((v2d{1, 0}), normalise2d({100, 0}));
  
  // v2d cross2d(const v2d& a, const v2d& b);
  assert_equal(-25.548, cross2d({1.32, 5.32}, {5.1, 1.2}));
  assert_equal(25.548, cross2d({5.1, 1.2}, {1.32, 5.32}));
  
  // v2d cross_at2d(const v2d& o, const v2d& a, const v2d& b);
  assert_equal(25.548, cross_at2d({0, 0}, {5.1, 1.2}, {1.32, 5.32}));
  assert_equal(8.824 * 2.0, cross_at2d({1, 1}, {5.1, 1.2}, {1.32, 5.32}));
  
  // bool is_strictly_left_of(const v2d& p, const line2d& l);
  // bool is_strictly_right_of(const v2d& p, const line2d& l);
  assert(is_strictly_left_of({3, 3}, {{1, 1}, {4, 3}}));
  assert(!is_strictly_right_of({3, 3}, {{1, 1}, {4, 3}}));
  assert(!is_strictly_left_of({2.5, 2}, {{1, 1}, {4, 3}}));
  assert(!is_strictly_right_of({2.5, 2}, {{1, 1}, {4, 3}}));
  
  // v2d dot2d(const v2d& a, const v2d& b);
  assert_equal(13.116, dot2d({5.1, 1.2}, {1.32, 5.32}));

  // v2d normal_of2d(const v2d& v);
  assert_equal((v2d{-5.32, 1.32}), perp2d({1.32, 5.32}));

  // v2d reflect2d(const v2d& r, const v2d& n);
  assert_equal((v2d{0, 3}), reflect2d({3, 0}, {1, 1}));
  assert_equal((v2d{3, 0}), reflect2d({0, 3}, {1, 1}));
  assert_equal((v2d{1, 1}), reflect2d({-1, 1}, {0, 1}));

  // dbl prop_along_line2d(const v2d& p, const line2d& l);
  assert_equal(0.0, prop_along_line2d({1, 1}, {{1, 1}, {3, 44}}));
  assert_equal(1.0, prop_along_line2d({3, 44}, {{1, 1}, {3, 44}}));
  assert_equal(0.5, prop_along_line2d({2, 22.5}, {{1, 1}, {3, 44}}));
  assert_equal(-0.5, prop_along_line2d({0, 0}, {{1, 1}, {3, 3}}));

  // bool point_on_line_inf2d(const v2d& p, const line2d& l);
  assert(point_on_line_inf2d({0, 0}, {{0, 0}, {2, 5}}));
  assert(point_on_line_inf2d({1, 2.5}, {{0, 0}, {2, 5}}));
  assert(point_on_line_inf2d({-1, -2.5}, {{0, 0}, {2, 5}}));
  assert(!point_on_line_inf2d({-0, -2.5}, {{0, 0}, {2, 5}}));
  assert(!point_on_line_inf2d({1, 2}, {{0, 0}, {2, 5}}));

  // bool point_on_line_segment2d(const v2d& p, const line2d& l, bool include);
  assert(point_on_line_segment2d({0, 0}, {{0, 0}, {2, 5}}, true));
  assert(!point_on_line_segment2d({0, 0}, {{0, 0}, {2, 5}}, false));
  assert(point_on_line_segment2d({1, 2.5}, {{0, 0}, {2, 5}}, true));
  assert(point_on_line_segment2d({1, 2.5}, {{0, 0}, {2, 5}}, false));
  assert(!point_on_line_segment2d({-1, -2.5}, {{0, 0}, {2, 5}}, true));
  assert(!point_on_line_segment2d({-1, -2.5}, {{0, 0}, {2, 5}}, false));
  assert(!point_on_line_segment2d({-0, -2.5}, {{0, 0}, {2, 5}}, true));
  assert(!point_on_line_segment2d({-0, -2.5}, {{0, 0}, {2, 5}}, false));
  assert(!point_on_line_segment2d({1, 2}, {{0, 0}, {2, 5}}, true));
  assert(!point_on_line_segment2d({1, 2}, {{0, 0}, {2, 5}}, false));

  assert(!point_on_line_segment2d({0, 3}, {{0, 0}, {0, 2}}, true));
  assert(!point_on_line_segment2d({0, 3}, {{0, 0}, {0, 2}}, false));

  assert(point_on_line_segment2d({0, 2}, {{0, 0}, {0, 2}}, true));
  assert(!point_on_line_segment2d({0, 2}, {{0, 0}, {0, 2}}, false));

  assert(point_on_line_segment2d({0, 1}, {{0, 0}, {0, 2}}, true));
  assert(point_on_line_segment2d({0, 1}, {{0, 0}, {0, 2}}, false));

  assert(!point_on_line_segment2d({-1, 0}, {{0, 0}, {2, 0}}, true));
  assert(!point_on_line_segment2d({-1, 0}, {{0, 0}, {2, 0}}, false));

  assert(point_on_line_segment2d({0, 0}, {{0, 0}, {2, 0}}, true));
  assert(!point_on_line_segment2d({0, 0}, {{0, 0}, {2, 0}}, false));

  assert(point_on_line_segment2d({1, 0}, {{0, 0}, {2, 0}}, true));
  assert(point_on_line_segment2d({1, 0}, {{0, 0}, {2, 0}}, false));

  // bool inter2d(const line2d& a, const line2d& b, bool include,
  //              dbl& aprop, dbl& bprop, line2d& inter_line, bool& infinite);
  dbl aprop;
  dbl bprop;
  line2d inter_line;
  bool infinite;

  line2d A = {{1, 3}, {3, 2}};
  line2d B = {{1, 1}, {2, 3}};
  line2d C = {{1, 1}, {3, 1}};
  line2d C2 = {{1, 1}, {1, 3}};
  line2d D = {{3, 3}, {1, 3}};
  line2d D2 = {{3, 3}, {3, 1}};
  line2d E = {{3, 1}, {5, 1}};
  line2d E2 = {{1, 3}, {1, 5}};
  line2d F = {{2, 1}, {4, 1}};
  line2d F2 = {{1, 2}, {1, 4}};
  line2d F3 = {{4, 1}, {2, 1}};
  line2d G = {{1, 2}, {3, 1}};
  line2d H = {{2, 1}, {3, 1}};
  line2d H2 = {{1, 2}, {1, 3}};
  line2d I = {{4, 1}, {5, 1}};
  line2d I2 = {{1, 4}, {1, 5}};
  line2d J = {{2, 1}, {2, 4}};
  line2d J2 = {{2, 2.5}, {2, 4}};
  line2d K = {{3, 1}, {3, 4}};

  assert(inter2d(A, B, true, aprop, bprop, inter_line, infinite));
  assert(!infinite);
  assert_equal(aprop, 0.4);
  assert_equal(bprop, 0.8);
  assert_equal(inter_line.a, (v2d{1.8, 2.6}));
  assert_equal(inter_line.b, (v2d{1.8, 2.6}));

  assert(inter2d(A, B, false, aprop, bprop, inter_line, infinite));
  assert(!infinite);
  assert_equal(aprop, 0.4);
  assert_equal(bprop, 0.8);
  assert_equal(inter_line.a, (v2d{1.8, 2.6}));
  assert_equal(inter_line.b, (v2d{1.8, 2.6}));

  assert(!inter2d(A, G, true, aprop, bprop, inter_line, infinite));
  assert(!inter2d(A, G, false, aprop, bprop, inter_line, infinite));

  assert(inter2d(C, E, true, aprop, bprop, inter_line, infinite));
  assert(!infinite);
  assert_equal(aprop, 1);
  assert_equal(bprop, 0);
  assert_equal(inter_line.a, C.b);
  assert_equal(inter_line.b, C.b);
  assert(!inter2d(C, E, false, aprop, bprop, inter_line, infinite));

  assert(inter2d(C, H, true, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.5);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, H.a);
  assert_equal(inter_line.b, H.b);

  assert(inter2d(C, H, false, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.5);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, H.a);
  assert_equal(inter_line.b, H.b);

  assert(inter2d(C, F, true, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.5);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, F.a);
  assert_equal(inter_line.b, C.b);

  assert(inter2d(C, F, false, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.5);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, F.a);
  assert_equal(inter_line.b, C.b);

  assert(inter2d(C, F3, true, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.5);
  assert_equal(bprop, 1.0);
  assert_equal(inter_line.a, F3.b);
  assert_equal(inter_line.b, C.b);

  assert(inter2d(C, F3, false, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.5);
  assert_equal(bprop, 1.0);
  assert_equal(inter_line.a, F3.b);
  assert_equal(inter_line.b, C.b);

  assert(!inter2d(C, I, true, aprop, bprop, inter_line, infinite));
  assert(!inter2d(C, I, false, aprop, bprop, inter_line, infinite));

  assert(!inter2d(C, D, true, aprop, bprop, inter_line, infinite));
  assert(!inter2d(C, D, false, aprop, bprop, inter_line, infinite));

  assert(inter2d(C2, E2, true, aprop, bprop, inter_line, infinite));
  assert(!infinite);
  assert_equal(aprop, 1);
  assert_equal(bprop, 0);
  assert_equal(inter_line.a, C2.b);
  assert_equal(inter_line.b, C2.b);
  assert(!inter2d(C2, E2, false, aprop, bprop, inter_line, infinite));

  assert(inter2d(C2, H2, true, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.5);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, H2.a);
  assert_equal(inter_line.b, H2.b);

  assert(inter2d(C2, H2, false, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.5);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, H2.a);
  assert_equal(inter_line.b, H2.b);

  assert(inter2d(C2, F2, true, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.5);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, F2.a);
  assert_equal(inter_line.b, C2.b);

  assert(inter2d(C2, F2, false, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.5);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, F2.a);
  assert_equal(inter_line.b, C2.b);

  assert(!inter2d(C2, I2, true, aprop, bprop, inter_line, infinite));
  assert(!inter2d(C2, I2, false, aprop, bprop, inter_line, infinite));

  assert(!inter2d(C2, D2, true, aprop, bprop, inter_line, infinite));
  assert(!inter2d(C2, D2, false, aprop, bprop, inter_line, infinite));

  assert(inter2d(A, A, true, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.0);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, A.a);
  assert_equal(inter_line.b, A.b);

  assert(inter2d(A, A, false, aprop, bprop, inter_line, infinite));
  assert(infinite);
  assert_equal(aprop, 0.0);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, A.a);
  assert_equal(inter_line.b, A.b);

  assert(inter2d(C, J, true, aprop, bprop, inter_line, infinite));
  assert(!infinite);
  assert_equal(aprop, 0.5);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, J.a);
  assert_equal(inter_line.b, J.a);

  assert(!inter2d(C, J, false, aprop, bprop, inter_line, infinite));

  assert(inter2d(C, K, true, aprop, bprop, inter_line, infinite));
  assert(!infinite);
  assert_equal(aprop, 1.0);
  assert_equal(bprop, 0.0);
  assert_equal(inter_line.a, K.a);
  assert_equal(inter_line.b, K.a);

  assert(!inter2d(C, K, false, aprop, bprop, inter_line, infinite));

  assert(!inter2d(C, J2, true, aprop, bprop, inter_line, infinite));
  assert(!inter2d(C, J2, false, aprop, bprop, inter_line, infinite));

  // bool colinear2d(const v2d& a, const v2d& b, const v2d& c);
  assert(colinear2d({0, 0}, {1, 1}, {2, 2}));
  assert(colinear2d({0, 0}, {2, 2}, {1, 1}));
  assert(!colinear2d({0, 0}, {2, 3}, {1, 1}));

  // poly2d convex2d(const vector<v2d>& pts);
  vector<v2d> p1 = {{0, 0}, {1, 0}, {2, 0}, {-1, 0}, {-2, 0}, 
    {0, 1}, {0, 2}, {0, -1}, {0, -2}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1}};
  vector<v2d> p2 = {{0, 0}, {1, 0}, {1, 1}};
  vector<v2d> p3 = {{0, 0}, {1, 1}};
  vector<v2d> p4 = {{0, 0}, {1, 1}, {2, 2}};
  vector<v2d> p5 = {{0, 0}, {1, 1}, {0, 2}, {2, 2}};
  vector<v2d> p6 = {{-2, 2}, {-1, 1}, {0, 0}, {1, 1}, {2, 2}};
  assert_equal((poly2d{{-2, 0}, {0, -2}, {2, 0}, {0, 2}}), convex2d(p1));
  assert_equal(p2, convex2d(p2));
  assert_equal(p3, convex2d(p3));
  assert_equal((poly2d{{0, 0}, {2, 2}}), convex2d(p4));
  assert_equal((poly2d{{0, 0}, {2, 2}, {0, 2}}), convex2d(p5));
  assert_equal((poly2d{{-2, 2}, {0, 0}, {2, 2}}), convex2d(p6));


  // bool point_in_poly2d(const v2d& v, const poly2d& poly);
  poly2d star = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  poly2d square = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
  poly2d triangle = {{-0.5, 0}, {0, -1}, {0.5, 0}};
  poly2d ushape = {{0, 0}, {3, 0}, {3, 2}, {2, 2}, {2, 1}, {1, 1}, {1, 2}, {0, 2}};
  poly2d breaking = {{8, 4}, {8, 8}, {7, 8}, {7, 6}, {6, 6}, {6, 8}, {4, 8}, {4, 4}};
  assert(point_in_poly2d({0, 0}, star));
  assert(!point_in_poly2d({1, 0}, star));
  assert(!point_in_poly2d({2, 0}, star));
  assert(point_in_poly2d({0.5, 0.5}, square));
  assert(!point_in_poly2d({1.0, 0.5}, square));
  assert(!point_in_poly2d({1.5, 0.5}, square));
  assert(point_in_poly2d({0.1, -0.2}, triangle));
  assert(point_in_poly2d({0.5, 1.5}, ushape));
  assert(!point_in_poly2d({1.5, 1.5}, ushape));
  assert(point_in_poly2d({2.5, 1.5}, ushape));
  assert(!point_in_poly2d({2.5, 6}, breaking));

  // dbl area2d(const poly2d& poly);
  assert_equal(1.0, area2d({{0, 0}, {1, 0}, {1, 1}, {0, 1}}));
  assert_equal(0.5, area2d({{0, 0}, {1, 0}, {1, 1}}));

  // v2d centroid2d(const poly2d& poly);

  // v2d project_point_onto_line2d(const v2d& p, const line2d& l);

  // dbl dist_to_inf_line2d(const v2d& p, const line2d& l);

  // dbl dist_to_line_segment_2d(const v2d& p, const line2d& l);

  // dbl mag3d(const v3d& v);
   
  // dbl dist3d(const v3d& a, const v3d& b);
   
  // v3d normalise3d(const v3d& v);
   
  // v3d cross3d(const v3d& a, const v3d& b);
   
  // v3d cross_at3d(const v3d& o, const v3d& a, const v3d& b);
   
  // dbl dot3d(const v3d& a, const v3d& b);
   
  // v3d perp3d(const v3d& a, const v3d& b);
   
  // v3d reflect3d(const v3d& r, const v3d& n);
}


// http://uva.onlinejudge.org/external/6/681.html
bool solve_convex_hull_finding() {
  int K;
  cin >> K;
  cout << K << endl;
  for (int k = 0; k < K; ++k) {
    int N;
    cin >> N;
    vector<v2d> pts;
    for (int i = 0; i < N; ++i) {
      int x, y;
      cin >> x >> y;
      if (i != N-1) pts.push_back({y, x});
    }

    vector<v2d> hull = convex2d(pts);

    cout << (hull.size() + 1) << endl;
    cout << hull[0].y << " " << hull[0].x << endl;
    for (int i = hull.size()-1; i >= 0; --i) {
      cout << hull[i].y << " " << hull[i].x << endl;
    }

    if (k != K-1) {
      cin >> N; // -1
      cout << N << endl;
    }
  }
}


// http://uva.onlinejudge.org/external/110/11030.html
void solve_predator_ii() {
  int T;
  cin >> T;
  for (int t = 0; t < T; t++) {
    cout << "Case " << (t+1) << ":" << endl;

    int N;
    cin >> N;
    vector<vector<v2d>> polys(N);
    for (int n = 0; n < N; ++n) {
      int S;
      cin >> S;
      for (int i = 0; i < S; ++i) {
        int x, y;
        cin >> x >> y;
        polys[n].push_back({x, y});
      }
      int i = 0;
      while (i < S - 2 && 
          colinear2d(polys[n][i], polys[n][i+1], polys[n][i+2])) {
        ++i;
      }
      if (is_strictly_right_of(polys[n][i+2], {polys[n][i], polys[n][i+1]})) {
        reverse(polys[n].begin(), polys[n].end());
        cerr << "rev" << endl;
      }
    }

    int Q;
    cin >> Q;
    for (int q = 0; q < Q; ++q) {
      v2d start, end;
      cin >> start.x >> start.y >> end.x >> end.y;
      int cost = 0;
      for (int i = 0; i < N; i++) {
        bool is = point_in_poly2d(start, polys[i]);
        bool in = point_in_poly2d(end,   polys[i]);
        //DBG(start.x);
        //DBG(start.y);
        //DBG(is);
        //DBG(end.x);
        //DBG(end.y);
        //DBG(in);
        cost += is != in;
      }
      cout << cost << endl;
    }
  }
}

int main() {
  solve_predator_ii();
  //solve_convex_hull_finding();
  //test();
}
