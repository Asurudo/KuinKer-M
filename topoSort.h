#ifndef TOPOSORTH
#define TOPOSORTH

#include "kuinkerm.h"

/*  拓扑排序
 */
namespace kuinkerm {

topoSort::topoSort(const Graph& Gp, int gN) : gN(gN) {
  if (gN != Gp.n)
    throw std::runtime_error(
        "topoSort, the second parameter differs from the point number in G, "
        "check it");

  Deg.resize(Gp.Maxn + 2);

  // 统计入度
  for (auto x : Gp.Ver) Deg[x]++;

  // 度数为 0 的节点入队
  for (int i = 1; i < Gp.n + 1; i++)
    if (!Deg[i]) Q.push(i);

  while (!Q.empty()) {
    int x = Q.front();
    Q.pop();
    topoSequence.emplace_back(x);
    for (int i = Gp.Head[x]; i; i = Gp.Next[i]) {
      int y = Gp.Ver[i];
      if (--Deg[y] == 0) Q.push(y);
    }
  }
}

bool topoSort::haveCircle() { return ((int)topoSequence.size() != gN); }
std::vector<int> topoSort::getTopoSequence() { return topoSequence; }

}  // namespace kuinkerm
#endif