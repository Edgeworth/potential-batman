// From Gozz
/* START SOLUTION */
enum event_type {add, inter, rem};
struct Event {
  Point p;
  coord_t d;  // Denominator for p
  event_type t;
  int i, j;
  Event(Point p, event_type t, int i) : p(p), d(1), t(t), i(i), j(-1) {}
  Event(Point p, coord_t d, int i, int j) : p(p), d(d), t(inter), i(i), j(j) {}
  bool operator>(const Event& rhs) const {
    if (p * rhs.d == rhs.p * d) return t > rhs.t;
    else return p * rhs.d > rhs.p * d;
  }
};

bool debug = false;

vector<pair<int, int>> many_segment_intersection(vector<Line> segs) {
  Point cmp_pt; // Scanline point to use for comparison
  coord_t cmp_den = 1;
  bool inclusive = true;

  for (auto& seg : segs) {  // Simplify maths below
    if (seg.e < seg.s) swap(seg.s, seg.e);
    if (debug) cerr << seg << endl;
    cmp_pt = min(cmp_pt, seg.s);
  }

  auto cmp = [&](const int& l, const int& r) {
    Line ls = segs[l], rs = segs[r];
    Point lv = segs[l].vel(), rv = segs[r].vel();
    coord_t lysl = lv.y * cmp_pt.x * rv.x + lv.cross(ls.s) * rv.x * cmp_den;  // l y-inter * lv.x * rv.x * cmp_den
    coord_t rysl = rv.y * cmp_pt.x * lv.x + rv.cross(rs.s) * lv.x * cmp_den;  // r y-inter * lv.x * rv.x * cmp_den
    if (eq(lysl, rysl, EPS)) {
      if (ls.s.x == ls.e.x && rs.s.x == rs.e.x) {
        return false;
      } else if (ls.s.x == ls.e.x) {
        rysl = rv.y * cmp_pt.x + rv.cross(rs.s) * cmp_den;  // r y-inter * rv.x * cmp_den
        if (eq(rysl, cmp_pt.y * rv.x, EPS)) return inclusive;
        else return cmp_pt.y * rv.x < rysl;
      } else if (rs.s.x == rs.e.x) {
        lysl = lv.y * cmp_pt.x + lv.cross(ls.s) * cmp_den;  // l y-inter * lv.x * cmp_den
        if (eq(lysl, cmp_pt.y * lv.x, EPS)) return !inclusive;
        else return lysl < cmp_pt.y * lv.x;
      } else {
        coord_t den = lv.cross(rv);
        Point intpt = (lv * rs.s.cross(rs.e) - rv * ls.s.cross(ls.e)) * cmp_den * sgn(den);
        Point refpt = cmp_pt * den * sgn(den);
        if (refpt < intpt || inclusive && refpt == intpt) { // Intersection in future
          return lv.y * rv.x > lv.x * rv.y;
        } else {  // Intersection in past
          return lv.y * rv.x < lv.x * rv.y;
        }
      }
    } else {
      return lysl < rysl;
    }
  };

  map<int, set<int>, decltype(cmp)> sl(cmp);

  priority_queue<Event, vector<Event>, greater<Event>> pq;
  for (int i = 0; i < segs.size(); ++i) {
    pq.push(Event(segs[i].s, add, i));
    pq.push(Event(segs[i].e, rem, i));
  }

  vector<pair<int, int>> inters;
  set<pair<int, int>> processed;
  auto push_inter = [&](int i, int j) {
    if (j < i) swap(i, j);
    if (processed.count(pair<int, int>(i, j))) return;
    processed.insert(pair<int, int>(i, j));
    Line is = segs[i], js = segs[j];
    if (is.s == is.e) ++is.e.y;
    if (js.s == js.e) ++js.e.y;
    Point iv = is.vel(), jv = js.vel();
    coord_t den = iv.cross(jv);
    Point num = (iv * js.s.cross(js.e) - jv * is.s.cross(is.e)) * sgn(den);
    den *= sgn(den);
    if (0 < den && (cmp_pt * den < num * cmp_den || inclusive && cmp_pt * den == num * cmp_den)) {
      pq.push(Event(num, den, i, j));
      if (debug) cerr << "\tpni " << i << ", " << j << " @ " << (num / den) << endl;
    }
  };

  while (!pq.empty()) {
    Event ev = pq.top();
    vector<pair<int, set<int>>> incident;
    if (debug) cerr << "at " << (ev.p / ev.d) << ":" << endl;
    if (ev.t == add) {
      if (debug) cerr << "\tadd " << ev.i << endl;
      pq.pop();
      auto it = sl.find(ev.i);
      if (it != sl.end()) { incident.push_back(*it); sl.erase(it); }  // Push old key first to avoid breaking processed
      incident.push_back(pair<int, set<int>>(ev.i, {ev.i}));
    } else if (ev.t == rem) {
      if (debug) cerr << "\trem " << ev.i << endl;
      pq.pop();
      auto it = sl.find(ev.i);
      if (it != sl.begin() && next(it) != sl.end()) push_inter(prev(it)->first, next(it)->first);
      if (it != sl.end()) it->second.erase(ev.i);
      if (it->second.empty()) sl.erase(it);
    } else if (ev.t == inter) {
      while (!pq.empty() && pq.top().p * ev.d == ev.p * pq.top().d && pq.top().t == inter) {
        ev = pq.top(); pq.pop();
        if (debug) cerr << "\tint " << ev.i << ", " << ev.j << endl;
        auto it = sl.find(ev.i), jt = sl.find(ev.j);
        if (it != sl.end()) { incident.push_back(*it); sl.erase(it); }
        if (jt != sl.end()) { incident.push_back(*jt); sl.erase(jt); }
      }
    }
    if (debug) {
      cerr << "\tsl:";
      for (auto row : sl) {
        cerr << "\t";
        for (auto el : row.second)
          cerr << el << " ";
      }
      cerr << endl;
    }
    if (debug) cerr << "\tincident.size() = " << incident.size() << endl;
    for (auto it = incident.begin(); it != incident.end(); ++it) {
      for (auto jt = next(it); jt != incident.end(); ++jt) {
        for (int i : it->second) {
          for (int j : jt->second) {
            inters.push_back(pair<int, int>(min(i, j), max(i, j)));
          }
        }
      }
    }
    cmp_pt = ev.p; cmp_den = ev.d;
    inclusive = ev.t == add;
    for (const auto& kv : incident) {
      sl[kv.first].insert(kv.second.begin(), kv.second.end());
      auto it = sl.find(kv.first);
      if (it != sl.begin()) push_inter(it->first, prev(it)->first);
      if (next(it) != sl.end()) push_inter(it->first, next(it)->first);
    }
    if (debug) {
      cerr << "\tsl:";
      for (auto row : sl) {
        cerr << "\t";
        for (auto el : row.second)
          cerr << el << " ";
      }
      cerr << endl;
    }
  }

  return inters;
}
/* END SOLUTION */
