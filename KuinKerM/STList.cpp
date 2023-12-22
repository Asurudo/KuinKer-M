#ifndef STLISTCPP
#define STLISTCPP

#include "../kuinkerm.h"

/* ST表
   注意，传入的vector是0-index，但是程序内为了方便处理使用的是1-index
*/
namespace kuinkerm {

  STList::STList(const std::vector<ll>& a) {
    n = a.size();
    Max.resize(n + 1), Min.resize(n + 1);
    Max[0].resize(log2(n) + 2), Min[0].resize(log2(n) + 2);
    for (int i = 1; i <= n; i++) {
      Max[i].resize(log2(n) + 2), Min[i].resize(log2(n) + 2);
      Max[i][0] = Min[i][0] = a[i - 1];
    }
    for (int j = 1; j < log2(n) + 2; j++)
      for (int i = 1; i + (1 << j) - 1 <= n; i++) {
        Max[i][j] = std::max(Max[i][j - 1], Max[i + (1 << (j - 1))][j - 1]);
        Min[i][j] = std::min(Min[i][j - 1], Min[i + (1 << (j - 1))][j - 1]);
      }
  }
  ll STList::askMax(int l, int r) {
    l++, r++;
    int k = log2(r - l + 1);
    return std::max(Max[l][k], Max[r - (1 << k) + 1][k]);
  }
  ll STList::askMin(int l, int r) {
    l++, r++;
    int k = log2(r - l + 1);
    return std::min(Min[l][k], Min[r - (1 << k) + 1][k]);
  }
} 

#endif