// update root and its root on the go
struct seg_tree {
  int n;
  vector<long long> sum;
  vector<long long> mn;
  vector<long long> mx;
  vector<long long> lazy_add;
  vector<long long> lazy_set;

  seg_tree(const vector<long long>& a) {
    n = (int)a.size();
    sum.assign(4 * n, 0);
    mn.assign(4 * n, LLONG_MAX);
    mx.assign(4 * n, LLONG_MIN);
    lazy_add.assign(4 * n, 0);
    lazy_set.assign(4 * n, 0); 
    build(0, 0, n - 1, a);
  }

  void build(int node, int l, int r, const vector<long long>& a) {
    if (l == r) {
      long long v = a[l];
      sum[node] = v;
      mn[node] = v;
      mx[node] = v;
      return;
    }
    int mid = (l + r) / 2;
    build(2 * node + 1, l, mid, a);
    build(2 * node + 2, mid + 1, r, a);
    pull(node);
  }

  void push(int node, int l, int r) {
    if (lazy_set[node] != 0) {
      long long v = lazy_set[node];
      sum[node] = (long long)(r - l + 1) * v;
      mn[node] = v;
      mx[node] = v;
      if (l != r) {
        lazy_set[2 * node + 1] = v;
        lazy_add[2 * node + 1] = 0; 
        lazy_set[2 * node + 2] = v;
        lazy_add[2 * node + 2] = 0; 
      }
      lazy_set[node] = 0;
      lazy_add[node] = 0; 
    }
    if (lazy_add[node] != 0) {
      long long v = lazy_add[node];
      sum[node] += (long long)(r - l + 1) * v;
      mn[node] += v;
      mx[node] += v;
      if (l != r) {
        if (lazy_set[2 * node + 1] != 0) {
          lazy_set[2 * node + 1] += v;
        } else {
          lazy_add[2 * node + 1] += v;
        }
        if (lazy_set[2 * node + 2] != 0) {
          lazy_set[2 * node + 2] += v;
        } else {
          lazy_add[2 * node + 2] += v;
        }
      }
      lazy_add[node] = 0;
    }
  }

  void pull(int node) {
    sum[node] = sum[2 * node + 1] + sum[2 * node + 2];
    mn[node] = min(mn[2 * node + 1], mn[2 * node + 2]);
    mx[node] = max(mx[2 * node + 1], mx[2 * node + 2]);
  }
  
  long long range_sum(int node, int l, int r, int ql, int qr) {
    push(node, l, r); 
    if (r < ql || qr < l) return 0;
    if (ql <= l && r <= qr) return sum[node];
    int mid = (l + r) / 2;
    return range_sum(2 * node + 1, l, mid, ql, qr) + range_sum(2 * node + 2, mid + 1, r, ql, qr);
  }

  void range_add(int node, int l, int r, int ql, int qr, long long val) {
    push(node, l, r); 
    if (r < ql || qr < l) return;
    if (ql <= l && r <= qr) {
      if (lazy_set[node] != 0) {
        lazy_set[node] += val;
      } else {
        lazy_add[node] += val;
      }
      push(node, l, r);
      return;
    }
    int mid = (l + r) / 2;
    range_add(2 * node + 1, l, mid, ql, qr, val);
    range_add(2 * node + 2, mid + 1, r, ql, qr, val);
    pull(node);
  }

  long long range_min(int node, int l, int r, int ql, int qr) {
    push(node, l, r);
    if (r < ql || qr < l) return LLONG_MAX;
    if (ql <= l && r <= qr) return mn[node];
    int mid = (l + r) / 2;
    return min(range_min(2 * node + 1, l, mid, ql, qr), range_min(2 * node + 2, mid + 1, r, ql, qr));
  }
  
  long long range_max(int node, int l, int r, int ql, int qr) {
    push(node, l, r);
    if (r < ql || qr < l) return LLONG_MIN;
    if (ql <= l && r <= qr) return mx[node];
    int mid = (l + r) / 2;
    return max(range_max(2 * node + 1, l, mid, ql, qr), range_max(2 * node + 2, mid + 1, r, ql, qr));
  }

  void range_set(int node, int l, int r, int ql, int qr, long long val) {
    push(node, l, r);
    if (r < ql || qr < l) return;
    if (ql <= l && r <= qr) {
      lazy_set[node] = val;
      push(node, l, r);
      return;
    }
    int mid = (l + r) / 2;
    range_set(2 * node + 1, l, mid, ql, qr, val);
    range_set(2 * node + 2, mid + 1, r, ql, qr, val);
    pull(node);
  }
};



// first update root and later update its children
struct SegTree {
  int n;
  vector<long long> sum;
  vector<long long> mn;
  vector<long long> mx;
  vector<long long> lazy_add;
  vector<long long> lazy_set;
  vector<char> set_flag;

  SegTree() : n(0) {}

  SegTree(const vector<long long>& a) {
    init(a);
  }

  void init(const vector<long long>& a) {
    n = (int)a.size();
    int sz = 4 * n + 5;
    sum.assign(sz, 0);
    mn.assign(sz, LLONG_MAX);
    mx.assign(sz, LLONG_MIN);
    lazy_add.assign(sz, 0);
    lazy_set.assign(sz, 0);
    set_flag.assign(sz, 0);
    if (n) build(1, 0, n - 1, a);
  }

  void build(int node, int l, int r, const vector<long long>& a) {
    if (l == r) {
      sum[node] = mn[node] = mx[node] = a[l];
      return;
    }
    int mid = (l + r) >> 1;
    build(node << 1, l, mid, a);
    build(node << 1 | 1, mid + 1, r, a);
    pull(node);
  }

  void apply_set(int node, int l, int r, long long val) {
    sum[node] = val * (r - l + 1LL);
    mn[node] = val;
    mx[node] = val;
    lazy_add[node] = 0;
    lazy_set[node] = val;
    set_flag[node] = 1;
  }

  void apply_add(int node, int l, int r, long long val) {
    sum[node] += val * (r - l + 1LL);
    mn[node] += val;
    mx[node] += val;
    if (set_flag[node]) {
      lazy_set[node] += val;
    } else {
      lazy_add[node] += val;
    }
  }

  void push(int node, int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1;
    int L = node << 1;
    int R = node << 1 | 1;

    if (set_flag[node]) {
      apply_set(L, l, mid, lazy_set[node]);
      apply_set(R, mid + 1, r, lazy_set[node]);
      set_flag[node] = 0;
      lazy_set[node] = 0;
    }

    if (lazy_add[node] != 0) {
      apply_add(L, l, mid, lazy_add[node]);
      apply_add(R, mid + 1, r, lazy_add[node]);
      lazy_add[node] = 0;
    }
  }

  void pull(int node) {
    int L = node << 1;
    int R = node << 1 | 1;
    sum[node] = sum[L] + sum[R];
    mn[node] = min(mn[L], mn[R]);
    mx[node] = max(mx[L], mx[R]);
  }

  void range_set(int ql, int qr, long long val) {
    if (ql > qr) return;
    range_set(1, 0, n - 1, ql, qr, val);
  }

  void range_add(int ql, int qr, long long val) {
    if (ql > qr) return;
    range_add(1, 0, n - 1, ql, qr, val);
  }

  long long query_sum(int ql, int qr) {
    if (ql > qr) return 0;
    return query_sum(1, 0, n - 1, ql, qr);
  }

  long long query_min(int ql, int qr) {
    if (ql > qr) return LLONG_MAX;
    return query_min(1, 0, n - 1, ql, qr);
  }

  long long query_max(int ql, int qr) {
    if (ql > qr) return LLONG_MIN;
    return query_max(1, 0, n - 1, ql, qr);
  }

  long long get(int idx) {
    return query_sum(idx, idx);
  }

private:
  void range_set(int node, int l, int r, int ql, int qr, long long val) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
      apply_set(node, l, r, val);
      return;
    }
    push(node, l, r);
    int mid = (l + r) >> 1;
    range_set(node << 1, l, mid, ql, qr, val);
    range_set(node << 1 | 1, mid + 1, r, ql, qr, val);
    pull(node);
  }

  void range_add(int node, int l, int r, int ql, int qr, long long val) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
      apply_add(node, l, r, val);
      return;
    }
    push(node, l, r);
    int mid = (l + r) >> 1;
    range_add(node << 1, l, mid, ql, qr, val);
    range_add(node << 1 | 1, mid + 1, r, ql, qr, val);
    pull(node);
  }

  long long query_sum(int node, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return 0;
    if (ql <= l && r <= qr) return sum[node];
    push(node, l, r);
    int mid = (l + r) >> 1;
    return query_sum(node << 1, l, mid, ql, qr) + query_sum(node << 1 | 1, mid + 1, r, ql, qr);
  }

  long long query_min(int node, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return LLONG_MAX;
    if (ql <= l && r <= qr) return mn[node];
    push(node, l, r);
    int mid = (l + r) >> 1;
    return min(query_min(node << 1, l, mid, ql, qr), query_min(node << 1 | 1, mid + 1, r, ql, qr));
  }

  long long query_max(int node, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return LLONG_MIN;
    if (ql <= l && r <= qr) return mx[node];
    push(node, l, r);
    int mid = (l + r) >> 1;
    return max(query_max(node << 1, l, mid, ql, qr), query_max(node << 1 | 1, mid + 1, r, ql, qr));
  }
};
