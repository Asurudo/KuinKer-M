#ifndef DIJKSTRACPP
#define DIJKSTRACPP

#include "../kuinkerm.h"

/*  迪杰斯特拉单源最短路算法
    不支持负边
    1-index
*/
namespace kuinkerm {
  /*  图，源点（默认为1）
      构造时直接开始跑最短路
  */
  Dijkstra::Dijkstra(const Graph& Gp, int startPoint)
      : Gp(Gp), startPoint(startPoint) {
    Distance.resize(Gp.Maxn);
    Visit.resize(Gp.Maxn);
    for (int i = 1; i < Gp.Maxn; i++) Distance[i] = INFLL;
    Distance[startPoint] = 0;
    Pq.push(Pair{0, startPoint});
    while (!Pq.empty()) {
      Pair Pa = Pq.top();
      Pq.pop();
      int curPoint = Pa.second;
      if (Visit[curPoint]) continue;
      Visit[curPoint] = true;
      for (int i = Gp.Head[curPoint]; i; i = Gp.Next[i]) {
        int nxtPoint = Gp.Ver[i];
        int edgeValue = Gp.Value[i];
        if (Distance[nxtPoint] > Distance[curPoint] + edgeValue) {
          Distance[nxtPoint] = Distance[curPoint] + edgeValue;
          Pq.push(Pair{Distance[nxtPoint], nxtPoint});
          // 路径还原 pre[nxtPoint] = curPoint;
        }
      }
    }
  }

  /*  返回整个Distance数组

      const vector<ll>& disRnt = dijkstra.getDistance();
  */
  const std::vector<ll>& Dijkstra::getDistance() const { return Distance; }

  /*  返回源点到pointId的最短距离

      long long disRnt = dijkstra.getDistance(pointId);
  */
  ll Dijkstra::getDistance(int pointId) const { return Distance[pointId]; }

} 

#endif