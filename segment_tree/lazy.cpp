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
    sum[node] = sum[2 * node + 1] + sum[2 * node + 2];
    mn[node] = min(mn[2 * node + 1], mn[2 * node + 2]);
    mx[node] = max(mx[2 * node + 1], mx[2 * node + 2]);
  }

  void push(int node, int l, int r) {
    if (lazy_set[node] != 0) {
      sum[node] = (r - l + 1) * lazy_set[node];
      mn[node] = lazy_set[node];
      mx[node] = lazy_set[node];
      if (l != r) {
        lazy_set[2 * node + 1] = lazy_set[node];
        lazy_set[2 * node + 2] = lazy_set[node];
      }
      lazy_set[node] = 0;
      lazy_add[node] = 0;
    }
    if (lazy_add[node] != 0) {
      sum[node] += (r - l + 1) * lazy_add[node];
      mn[node] += lazy_add[node];
      mx[node] += lazy_add[node];
      if (l != r) {
        lazy_add[2 * node + 1] += lazy_add[node];
        lazy_add[2 * node + 2] += lazy_add[node];
      }
      lazy_add[node] = 0;
    }
    return;
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
    long long left_sum = range_sum(2 * node + 1, l, mid, ql, qr);
    long long right_sum = range_sum(2 * node + 2, mid + 1, r, ql, qr);
    return left_sum + right_sum;
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

  void range_add(int node, int l, int r, int ql, int qr, long long val) {
    push(node, l, r);
    if (r < ql || qr < l) return;
    if (ql <= l && r <= qr) {
      lazy_add[node] += val;
      push(node, l, r);
      return;
    }
    int mid = (l + r) / 2;
    range_add(2 * node + 1, l, mid, ql, qr, val);
    range_add(2 * node + 2, mid + 1, r, ql, qr, val);
    pull(node);
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