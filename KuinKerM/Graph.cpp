#ifndef GRAPHCPP
#define GRAPHCPP

#include "../kuinkerm.h"

/*  存图
 */
namespace kuinkerm {

  /*  最大点数，最大边数，实际点数（可不填），实际边数（可不填）
   */
  Graph::Graph(int Maxn, int Maxe, int n, int m)
      : Maxn(Maxn), Maxe(Maxe << 1), n(n), m(m) {
    Tot = 1;
    Next.resize(Maxe << 1);
    Head.resize(Maxn);
    Ver.resize(Maxe << 1);
    Value.resize(Maxe << 1);
  }

  /*  向图中加有向边
      注意，加一条无向边等于加两条方向相反，权值相等的有向边

      // 向图中添加一条两端点编号分别为 39 和 42，且权值为 9 的无向边
      g.Add(39, 42, 9);
      g.Add(42, 39, 9);
  */
  int Graph::Add(int x, int y, ll w) {
    Ver[++Tot] = y, Next[Tot] = Head[x], Head[x] = Tot, Value[Tot] = w;
    return Tot;
  }

  int Graph::getVertexNum(){
      return n;
  }
  int Graph::getEdgeNum(){
      return m;
  }

}

#endif