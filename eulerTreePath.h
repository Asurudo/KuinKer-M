#ifndef EULERTREEPATHH
#define EULERTREEPATHH

#include "kuinkerm.h"

/*  树的欧拉序
 */
namespace kuinkerm {

void eulerTreePath::eulerInit(const Graph& Gp, int x, int Fa) {
  L[x] = ++Pos;
  Ore[++oPos] = x;
  Oula[x] = oPos;
  Re[Pos] = x;
  for (int i = Gp.Head[x]; i; i = Gp.Next[i]) {
    int y = Gp.Ver[i];
    if (y == Fa) continue;
    eulerInit(Gp, y, x);
    Ore[++oPos] = x;
  }
  R[x] = Pos;
}

eulerTreePath::eulerTreePath(const Graph& Gp) : Pos(0), oPos(0) {
  L.resize(Gp.Maxn + 2);
  R.resize(Gp.Maxn + 2);
  Re.resize(Gp.Maxn + 2);
  Ore.resize(Gp.Maxn << 1 + 2);
  Oula.resize(Gp.Maxn << 1 + 2);
  eulerInit(Gp, 1, 0);
}

std::vector<int> eulerTreePath::getEulerPath(){
    return Ore;
}

}  // namespace kuinkerm
#endif