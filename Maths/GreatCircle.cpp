// From Gozz
/* START SOLUTION */
typedef long double coord_t;

const double PI = 3.141592653589793238L;
const coord_t R_Earth = 6378000; // in metres

// z = north, x = greenwich
coord_t great_circle_distance(coord_t A_lat, coord_t A_lon, coord_t B_lat, coord_t B_lon) {
  coord_t A_x, A_y, A_z, B_x, B_y, B_z;
  A_x = cos(A_lat) * cos(A_lon); A_y = cos(A_lat) * sin(A_lon); A_z = sin(A_lat);
  B_x = cos(B_lat) * cos(B_lon); B_y = cos(B_lat) * sin(B_lon); B_z = sin(B_lat);
  coord_t AcB_x, AcB_y, AcB_z;
  AcB_x = A_y * B_z - A_z * B_y; AcB_y = A_z * B_x - A_x * B_z; AcB_z = A_x * B_y - A_y * B_x;
  coord_t AcB = sqrt(AcB_x * AcB_x + AcB_y * AcB_y + AcB_z * AcB_z);
  coord_t AdB = A_x * B_x + A_y * B_y + A_z * B_z;
  return R_Earth * atan2(AcB, AdB);
}


bool circle3points( const xy& a, const xy& b, const xy& c, double& eps, circle& res)
{
  if (std::abs(left(a,b,c))<=eps) return false;
  line l1(a, b, false);
  line l2(b, c, false);
  line pl1; pl1.upper = a + (b-a)/2; pl1.lower = pl1.s + l1.normal();
  line pl2; pl2.upper = b + (c-b)/2; pl2.e = pl2.s + l2.normal();
  bool overlap;
  intersect_line(pl2,pl1, eps, res.c, overlap);
  res.r = dist(res.c, a);
  return true;
}
/* END SOLUTION */
