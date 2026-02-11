class FenwickTree {
  vector<long long> fenw;
  int n;

  FenwickTree(int n_) : n(n_) {
    fenw.resize(n);
  }
  
  void modify(int i, int v) {
    while (i <= n) {
      fenw[i] += v;
      i += i & -i;
    }
  }

  long long query(int i) {
    long long sum = 0;
    while (i > 0) {
      sum += fenw[i];
      i -= i & -i;
    }
    return sum;
  }
};
