/* START SOLUTION */
template <typename Key, typename Value>
class SplayMap {
public:
  SplayMap() : ns(2), root(DUMMY), size(0) {}
  template <typename ValueRef>
  bool Insert(Key key, ValueRef&& value) {
    if (Find(key)) return false;
    ++size;
    int oldroot = root;
    root = int(ns.size());
    if (key < ns[oldroot].k) {
      ns.push_back({key, ns[oldroot].l, oldroot, std::forward<ValueRef>(value)});
      ns[oldroot].l = DUMMY;
    } else {
      ns.push_back({key, oldroot, ns[oldroot].r, std::forward<ValueRef>(value)});
      ns[oldroot].r = DUMMY;
    }
    return true;
  }
  bool Find(Key key) {
    if (root == DUMMY) return false;
    int lnext = TMP, rnext = TMP;
    bool found = false;
    while (!found) {
      if (key < ns[root].k) {
        int l = ns[root].l;
        if (l == DUMMY) break;
        if (key < ns[l].k) {
          ns[root].l = ns[l].r;
          ns[l].r = root;
          if (ns[l].l == DUMMY) {
            root = l;
            break;
          } else {
            root = ns[l].l;
            ns[rnext].l = l;
            rnext = l;
          }
        } else if (ns[l].k < key) {
          ns[rnext].l = root;
          rnext = root;
          if (ns[l].r == DUMMY) {
            root = l;
            break;
          } else {
            root = ns[l].r;
            ns[lnext].r = l;
            lnext = l;
          }
        } else {
          ns[rnext].l = root;
          rnext = root;
          root = l;
          found = true;
        }
      } else if (ns[root].k < key) {
        int r = ns[root].r;
        if (r == DUMMY) break;
        if (ns[r].k < key) {
          ns[root].r = ns[r].l;
          ns[r].l = root;
          if (ns[r].r == DUMMY) {
            root = r;
            break;
          } else {
            root = ns[r].r;
            ns[lnext].r = r;
            lnext = r;
          }
        } else if (key < ns[r].k) {
          ns[lnext].r = root;
          lnext = root;
          if (ns[r].l == DUMMY) {
            root = r;
            break;
          } else {
            root = ns[r].l;
            ns[rnext].l = r;
            rnext = r;
          }
        } else {
          ns[lnext].r = root;
          lnext = root;
          root = r;
          found = true;
        }
      } else {
        found = true;
      }
    }
    ns[lnext].r = ns[root].l;
    ns[rnext].l = ns[root].r;
    ns[root].l = ns[TMP].r;
    ns[root].r = ns[TMP].l;
    return found;
  }
  bool Delete(Key key) {
    if (Find(key)) {
      if (ns[root].r == DUMMY) {
        root = ns[root].l;
      } else {
        int oldroot = root;
        root = ns[root].r;
        Find(key);
        ns[root].l = ns[oldroot].l;
      }
      --size;
      return true;
    }
    return false;
  }
  const Value& Get() {
    assert(Size() > 0);
    return ns[root].v;
  }
  Value& operator[](Key key) {
    if (Find(key)) return Get();
    Insert(key, Value());
    return Get();
  }
  std::size_t Size() { return size; }
  void Reserve(std::size_t s) { ns.reserve(s); }
private:
  constexpr static int DUMMY = 0, TMP = 1;
  struct node_t {
    Key k;
    int l, r;
    Value v;
  };
  std::vector<node_t> ns;
  int root;
  std::size_t size;
};
/* END SOLUTION */
