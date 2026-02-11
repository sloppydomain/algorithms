class dsu {
public:
  vector<int> p, sz;
  int n;
  
  dsu(int _n) : n(_n) {
    p.resize(n, -1);
    sz.resize(n, 1);
    for (int i = 0; i < n; ++i) {
      p[i] = i;
    }
  }
 
  inline int get(int v) {
    if (v == p[v]) {
      return v;
    }
    return p[v] = get(p[v]);
  }
 
  inline void unite(int u, int v) {
    int rootu = get(u);
    int rootv = get(v);
    if (rootu != rootv) {
      if (sz[rootu] < sz[rootv]) {
        swap(rootu, rootv);
      }
      p[rootv] = rootu;
      sz[rootu] += sz[rootv];
    }
  }
};