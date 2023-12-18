#ifndef MSTKRUSKALH
#define MSTKRUSKALH

#include "kuinkerm.h"

/*  拓扑排序
 */
namespace kuinkerm {

MSTKruskal::MSTKruskal(const Graph& Gp) : Djs(Gp.Maxn + 1), MSTVal(0) {
  // 将所有边加入临时数组并按照边权从小到大排序
  for (int x = 1; x < Gp.Maxn; x++)
    for (int i = Gp.Head[x]; i; i = Gp.Next[i])
      e.push_back({x, Gp.Ver[i], Gp.Value[i]});
  sort(e.begin(), e.end());
  // 遍历所有边，若连边的两点未连通，则将该边加入最小生成树
  for (int i = 0; i < (int)e.size(); i++)
    if (!Djs.Same(e[i].u, e[i].v)) {
      Djs.Unite(e[i].u, e[i].v);
      std::cout << e[i].u << " " << e[i].v << " " << e[i].w << std::endl;
      MSTVal += e[i].w;
    }
}

ll MSTKruskal::getMSTVal() { return MSTVal; }
}  // namespace kuinkerm
#endif