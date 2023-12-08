#ifndef DISJOINTSETH
#define DISJOINTSETH

#include "kuinkerm.h"

/*  并查集
 */
namespace kuinkerm {

  disjointSet::disjointSet(int Maxn) : Maxn(Maxn) {
    Parent.resize(Maxn + 1);
    High.resize(Maxn + 1);

    for (int i = 1; i < Maxn; i++) Parent[i] = i, High[i] = 0;
  }

  // 查询集合 x 的最终父亲
  int disjointSet::Find(int x) { return Parent[x] == x ? x : Parent[x] = Find(Parent[x]); }

  // 将集合 x 和集合 y 所属的两个集合连接起来
  void disjointSet::Unite(int x, int y) {
    x = Find(x);
    y = Find(y);
    if (x == y) return;
    if (High[x] < High[y])
      Parent[x] = y;
    else {
      Parent[y] = x;
      if (High[x] == High[y]) High[x]++;
    }
  }
  // 检查集合 x 和集合 y是否属于同一集合
  bool disjointSet::Same(int x, int y) { return Find(x) == Find(y); }

}  // namespace kuinkerm
#endif