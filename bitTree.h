#ifndef BITTREEH
#define BITTREEH

#include "kuinkerm.h"

/*  树状数组
    注意，传入的vector是0-index，但是程序内为了方便处理使用的是1-index
*/
namespace kuinkerm {

  bitTree::bitTree(const std::vector<ll>& a) : a(a) {
    n = a.size();
    c.resize(n << 2);
    for (int i = 1; i <= n; i++) Add(i, a[i - 1]);
  }
  bitTree::bitTree(const std::vector<std::vector<ll>> a2) : a2(a2) {
    n = a2.size();
    if (!n) return;
    m = a2[0].size();
    c2.resize(n << 2);

    for (int i = 0; i <= (int)(n << 1); i++) {
      c2[i].resize(m << 2);
      if (i < n && (int)a2[i].size() != m)
        throw std::runtime_error("bitTree, you need to offer a matrix");
    }

    for (int i = 1; i <= n; i++)
      for (int j = 1; j <= m; j++) Add(i, j, a2[i - 1][j - 1]);
  }

  void bitTree::Add(int x, ll d) {
    x++;
    for (; x <= (int)(n << 1); x += lowbit(x)) c[x] += d;
  }
  void bitTree::Add(int x, int y, ll d) {
    x++, y++;
    for (int i = x; i <= (int)(n << 1); i += lowbit(i))
      for (int j = y; j <= (int)(m << 1); j += lowbit(j)) c2[i][j] += d;
  }
  ll bitTree::Ask(int x) {
    x++;
    ll Ans = 0;
    for (; x; x -= lowbit(x)) Ans += c[x];
    return Ans;
  }
  ll bitTree::Ask(int x, int y) {
    x++, y++;
    ll ans = 0;
    for (int i = x; i; i -= lowbit(i))
      for (int j = y; j; j -= lowbit(j)) ans += c2[i][j];
    return ans;
  }
  ll bitTree::getSum(int l, int r) { return Ask(r) - Ask(l - 1); }
  ll bitTree::getSum(int x1, int y1, int x2, int y2) {
    return Ask(x2, y2) - Ask(x1 - 1, y2) - Ask(x2, y1 - 1) +
           Ask(x1 - 1, y1 - 1);
  }
}  // namespace kuinkerm
#endif