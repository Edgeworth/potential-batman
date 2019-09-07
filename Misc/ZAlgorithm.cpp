/* START SOLUTION */
std::vector<int> zalg(const std::string& p, const std::string& s) {
  std::string v = p + s;
  std::vector<int> z(v.size(), 0);
  int l = 0, r = 0;
  for (int i = 1; i < v.size(); ++i) {
    if (i > r) {
      l = r = i;
      while (r < v.size() && v[r] == v[r - l]) r++;
      z[i] = r - l;
      r--;
    } else {
      if (i + z[i - l] - 1 < r) z[i] = z[i - l];
      else {
        l = i;
        while (r < v.size() && v[r] == v[r - l]) r++;
        z[i] = r - l;
        r--;
      }
    }
  }
  return z;
}
/* END SOLUTION */
