#ifndef TSPCPP
#define TSPCPP

#include "../kuinkerm.h"

/*  给出一个 n 个节点， m 条边的联通无向图，问从 1
 * 出发，游历所有点至少一次后返回，最少需经过的边权总和
 */
namespace kuinkerm {

/* 图构造时直接开始跑TSP
 */
TSP::TSP(const Graph& Gp) : Gp(Gp) {
  Dp.resize(Gp.Maxn);
  Distance.resize(Gp.Maxn);
  for (int i = 0; i < Gp.Maxn; i++) {
    Dp[i].resize(1 << Gp.Maxn);
    for (int j = 0; j < (1 << Gp.Maxn); j++) Dp[i][j] = INFLL;
    Distance[i].resize(Gp.Maxn);
  }
  for (int i = 1; i <= Gp.n; i++) {
    Dijkstra Dij(Gp, i);
    Distance[i] = Dij.getDistance();
  }
  Dp[0][1] = 0;
  for (int S = 0; S < (1 << Gp.n); S++)
    for (int St = 1; St <= Gp.n; St++)
      if (S & (1 << (St - 1)))
        for (int Ed = 1; Ed <= Gp.n; Ed++) {
          if (St == Ed || !(S & (1 << (Ed - 1)))) continue;
          Dp[Ed - 1][S] =
              std::min(Dp[Ed - 1][S],
                       Dp[St - 1][S ^ (1 << (Ed - 1))] + Distance[St][Ed]);
        }
  minEdgeValueSum = INFLL;
  for (int i = 1; i <= Gp.n; i++)
    minEdgeValueSum =
        std::min(minEdgeValueSum, Dp[i - 1][(1 << Gp.n) - 1] + Distance[i][1]);
}
/*  返回最小边权加和
 */
ll TSP::getMinEdgeValueSum() { return minEdgeValueSum; }

} 

#endif