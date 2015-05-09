#ifndef DEFINES_INCLUDED
#define DEFINES_INCLUDED

#include<iostream>
#include<iomanip>
#include<sstream>
#include<vector>
#include<queue>
#include<map>
#include<stack>
#include<set>
#include<climits>
#include<algorithm>
#include<tuple>
#include<cmath>
#include<sstream>
using namespace std;

typedef long long ll;
typedef long double ld;
// change to double if less accuracy needed 
typedef ld dbl; 

typedef tuple<dbl, dbl> vec2;
typedef tuple<int, int, int> tiii;
typedef pair<int, int> pii;
typedef pair<ll, int> pli;

typedef vector<ll> vl;
typedef vector<vl> vvl;

typedef vector<dbl> vd;
typedef vector<vd> vvd;

typedef vector<bool> vb;
typedef vector<vb> vvb;

typedef vector<int> vi;
typedef vector<vi> vvi;

typedef vector<string> vs;
typedef vector<vs> vvs;

typedef vector<pii> vpii;
typedef vector<vpii> vvpii;
typedef vector<tiii> vtiii;
typedef vector<pli> vpli;
typedef vector<vpli> vvpli;


typedef ostringstream oss;
typedef istringstream iss;

#define _ ignore

// used for comp geom and some maths
const dbl EP = 1e-7;

bool equ(dbl a, dbl b) {
  return abs(a - b) < EP;
}

bool lteq(dbl a, dbl b) {
  return a < b + EP;
}

bool lt(dbl a, dbl b) {
  return a < b - EP;
}

bool gteq(dbl a, dbl b) {
  return a > b - EP;
}

bool gt(dbl a, dbl b) {
  return a > b + EP;
}

bool lti(dbl a, dbl b, bool include) {
  if (include) {
    return lteq(a, b);
  } else {
    return lt(a, b);
  }
}

bool gti(dbl a, dbl b, bool include) {
  if (include) {
    return gteq(a, b);
  } else {
    return gt(a, b);
  }
}

// make c++ io faster
// put in main:
// ios::sync_with_stdio(false);
// cin.tie(NULL);
// and don't use endl

#endif
