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