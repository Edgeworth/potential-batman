// From Max
/* START SOLUTION */
typedef int ind_t; typedef int data_t;
struct SegTree {
  // Base value is the "initial" value of any index.
  const data_t base_value = 0;
  // The identity value for f (0 for +, 1 for *, INF for min, etc).
  const data_t identity = 0;
  // left, right, node value, lazy delta, and left/right children.
  ind_t l, r;
  data_t val, delta;
  unique_ptr<SegTree> children[2];
  SegTree(ind_t l, ind_t r) : l(l), r(r), val(base_value), delta(0) {}
  // Binary function. Max is given, min is the same. Commented additon shown.
  data_t f(data_t a, data_t b) { return max(a,b); /*return a + b;*/ }
  // How to modify f for the delta over this range. Change for + is commented.
  data_t f_delta() { return delta; /*return delta*(r-l+1);*/ }
  data_t value() { return val + f_delta(); }
  data_t middle() { return l == r-1 ? l : l + (r-l+1)/2; }
  void lazy_push() {
    ind_t mid = middle();
    if (delta != 0) {
      // Push delta into children.
      if (children[0] == nullptr)
        children[0] = unique_ptr<SegTree>(new SegTree(l, mid));
      children[0]->delta += delta;
      if (children[1] == nullptr)
        children[1] = unique_ptr<SegTree>(new SegTree(mid+1, r));
      children[1]->delta += delta;
      val += f_delta();
      delta = 0;
    }
  }
  data_t update(ind_t ql, ind_t qr, data_t v) {
    if (l == ql && r == qr) {
      delta += v;
      return val + f_delta();
    }
    lazy_push();
    // init left and right.
    data_t left = base_value, right = base_value;
    ind_t mid = middle();
    if (children[0] != nullptr) left = children[0]->value();
    if (children[1] != nullptr) right = children[1]->value();
    // update
    if (ql <= mid) {
      if (children[0] == nullptr)
        children[0] = unique_ptr<SegTree>(new SegTree(l, mid));
      left = children[0]->update(ql, qr < mid ? qr : mid, v);
    }
    if (qr > mid) {
      if (children[1] == nullptr)
        children[1] = unique_ptr<SegTree>(new SegTree(mid+1, r));
      right = children[1]->update(ql < mid+1 ? mid+1 : ql, qr, v);
    }
    return val = f(left, right);
  }
  data_t query(ind_t ql, ind_t qr) {
    if (l == ql && r == qr)
      return val + f_delta();
    lazy_push();
    data_t left = identity, right = identity;
    ind_t mid = middle();
    if (ql <= mid)
      if (children[0] == nullptr) left = base_value;
      else left = children[0]->query(ql, qr < mid ? qr : mid);
    if (qr > mid)
      if (children[1] == nullptr) right = base_value;
      else right = children[1]->query(ql < mid+1 ? mid+1 : ql, qr);

    return f(left, right);
  }
  void print_vals() {
    for (int i = l; i <= r; ++i)
      cout << query(i, i) << " ";
    cout << endl;
  }

  void print(int indent) {
    for (int i = 0 ; i < indent; ++i)
      cout << " ";
    cout << l << " " << r << " " << val << " " << delta << endl;
    if (children[0] != nullptr)
      children[0]->print(indent+2);
    if (children[1] != nullptr)
      children[1]->print(indent+2);
  }
};
/* END SOLUTION */
