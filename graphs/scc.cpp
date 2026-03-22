#include <bits/stdc++.h>

using namespace std;
 
/*
  strongly connected components -> kosaraju's algorithm
*/
 
void dfs1(int u, vector<bool>& vis, vector<vector<int>>& e, vector<int>& out) {
  vis[u] = true;
  for (int& v : e[u]) {
    if (!vis[v]) {
      dfs1(v, vis, e, out);
    }
  }
  out.push_back(u);
}
 
void dfs2(int u, vector<bool>& vis, int group, vector<int>& ans, vector<vector<int>>& e) {
  vis[u] = true;
  ans[u] = group;
  for (int& v : e[u]) {
    if (!vis[v]) {
      dfs2(v, vis, group, ans, e);
    }
  }
}
 
int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int tt = 1;
  // cin >> tt;
  while (tt--) {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> e(n + 1);
    for (int i = 0; i < m; i++) {
      int u, v;
      cin >> u >> v;
      e[u].push_back(v);
    }
    vector<int> out;
    vector<bool> vis(n + 1, false);
    for (int v = 1; v <= n; v++) {
      if (!vis[v]) {
        dfs1(v, vis, e, out);
      }
    }
    vector<vector<int>> re(n + 1);
    for (int u = 1; u <= n; u++) {
      for (int& v : e[u]) {
        re[v].push_back(u);
      }
    }
    vector<int> ans(n + 1);
    reverse(out.begin(), out.end());
    vis.assign(n + 1, false);  
    int group = 1;
    for (int i = 0; i < int(out.size()); i++) {
      if (!vis[out[i]]) {
        dfs2(out[i], vis, group, ans, re);
        group += 1;
      }
    }
    cout << group - 1 << '\n';
    for (int v = 1; v <= n; v++) {
      cout << ans[v] << ' ';
    }
  }
  return 0;
}