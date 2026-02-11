/*
  Monotonic function:
  TTTT...FFFFFF or FFFF...TTTTTT
*/

bool ask(int x) {
  // return true if x is valid solution else false
}

// FFF...FTTTT, finds first T
int solve_min() {
  int l = 0, r = 1e9; 
  int ans = -1;       
  while (l <= r) {
    int mid = l + (r - l) / 2;
    if (ask(mid)) {
      ans = mid;
      r = mid - 1;
    } else {
      l = mid + 1; 
    }
  }
  return ans;
  // ans - 1 is last F
}

// TTT..TFFFF, finds last T
int solve_max() {
  int l = 0, r = 1e9; 
  int ans = -1;       
  while (l <= r) {
    int mid = l + (r - l) / 2;
    if (ask(mid)) {
      ans = mid;
      l = mid + 1; 
    } else {
      r = mid - 1;
    }
  }
  return ans;
  // ans + 1 is first F
}