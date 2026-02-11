// non-recursive
auto be = [](int b, int e, int mod) {
  int ans = 1;
  while (e) {
    if (e & 1) {
      ans = (1LL * ans * b) % mod;
    }
    b = (1LL * b * b) % mod;
    e >>= 1;
  }
  return ans;
}

// recursive
int be(int b, int e, int mod) {
  if (e == 0) {
    return 1;
  }
  long long half = be(b, e / 2, mod);
  long long res = (half * half) % mod;
  if (e & 1) {
    res = (res * b) % mod;
  }
  return (int)res;
}
