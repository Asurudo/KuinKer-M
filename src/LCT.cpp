#ifndef LCTCPP
#define LCTCPP

#include "../kuinkerm.h"

/* Link-Cut Tree
   进化版并查集
*/
namespace kuinkerm {

  // 获取节点 x 在其父节点中的左右关系
  inline int LCT::Get(int x) { return Nodes[Nodes[x].Fa].Ch[1] == x; }
  // 判断节点 x 是否是所在树的根
  inline bool LCT::isRoot(int x) {
    return Nodes[Nodes[x].Fa].Ch[0] != x && Nodes[Nodes[x].Fa].Ch[1] != x;
  }
  // 更新节点 x 的信息，根据左右子树的信息更新节点大小等
  void LCT::pushUp(int x) {
    Nodes[x].Sz =
        Nodes[Nodes[x].Ch[0]].Sz + Nodes[Nodes[x].Ch[1]].Sz + Nodes[x].Sz2 + 1;
  }
  // 反转节点 p 的左右子树
  void LCT::pushRotate(int x) {
    if (!x) return;
    std::swap(Nodes[x].Ch[0], Nodes[x].Ch[1]);
    Nodes[x].rotateTag ^= 1;
  }
  // 将节点 x 的标记向下传递
  void LCT::pushDown(int x) {
    if (Nodes[x].rotateTag) {
      pushRotate(Nodes[x].Ch[0]);
      pushRotate(Nodes[x].Ch[1]);
      Nodes[x].rotateTag = false;
    }
  }
  // 更新节点 x 的信息，递归更新其父节点
  void LCT::Update(int x) {
    if (!isRoot(x)) Update(Nodes[x].Fa);
    pushDown(x);
  }
  // 旋转节点 x
  void LCT::Rotate(int x) {
    int y = Nodes[x].Fa;
    int z = Nodes[y].Fa;
    int k = Get(x);
    if (!isRoot(y)) Nodes[z].Ch[Get(y)] = x;
    Nodes[x].Fa = z;
    Nodes[Nodes[x].Ch[!k]].Fa = y;
    Nodes[y].Ch[k] = Nodes[x].Ch[!k];
    Nodes[x].Ch[!k] = y;
    Nodes[y].Fa = x;
    pushUp(y);
    pushUp(x);
  }
  // 伸展节点x
  void LCT::Splay(int x) {
    Update(x);
    for (int p = Nodes[x].Fa; !isRoot(x); p = Nodes[x].Fa) {
      if (!isRoot(p)) Rotate(Get(p) == Get(x) ? p : x);
      Rotate(x);
    }
  }
  // 将节点 x 到其树根的路径转为 Splay Tree，并返回树根
  int LCT::Access(int x) {
    int p = 0;
    while (x) {
      Splay(x);
      Nodes[x].Sz2 += Nodes[Nodes[x].Ch[1]].Sz - Nodes[p].Sz;
      Nodes[x].Ch[1] = p;
      pushUp(x);
      p = x;
      x = Nodes[x].Fa;
    }
    return p;
  }
  // 将节点 x 变为所在树的根
  int LCT::makeRoot(int x) {
    x = Access(x);
    std::swap(Nodes[x].Ch[0], Nodes[x].Ch[1]);
    Nodes[x].rotateTag ^= 1;
    return x;
  }

  LCT::LCT(int Maxn) : Maxn(Maxn) {
    Nodes.resize(Maxn);
    Nodes[0].Fa = 0;
    Nodes[0].Ch[0] = Nodes[0].Ch[1] = 0;
    Nodes[0].rotateTag = false;
    n = 1;
    Nodes[0].Sz = 0;
    Nodes[0].Sz2 = 0;
  }

  int LCT::getSize(int x) { return Nodes[Find(x)].Sz; }

  // 判断是否存在边 x-y
  bool LCT::haveEdge(int x, int y) {
    if (Find(x) == Find(y)) {
      makeRoot(x);
      Access(y);
      Splay(x);
      if (Nodes[x].Ch[1] == y && Nodes[y].Ch[0] == 0) return true;
    }
    return false;
  }

  // 在两个不在同一树中的节点 x 和 y 之间添加边
  void LCT::Link(int x, int y) {
    if (haveEdge(x, y)) return;
    x = makeRoot(x);
    makeRoot(y);
    Nodes[x].Fa = y;
    Nodes[y].Sz2 += Nodes[x].Sz;
    while (y) {
      pushUp(y);
      y = Nodes[y].Fa;
    }
  }

  // 删除边 x-y
  void LCT::Cut(int x, int y) {
    // int res1, res2;
    makeRoot(x);
    Access(y);
    Splay(x);
    Nodes[y].Fa = 0;
    Nodes[x].Ch[1] = 0;
    pushUp(x);
    // res1 = Nodes[x].Sz;
    // res2 = Nodes[y].Sz;
  }

  // 返回节点 x 所在树的根
  int LCT::Find(int x) {
    x = Access(x);
    while (Nodes[x].Ch[0]) x = Nodes[x].Ch[0];
    Splay(x);
    return x;
  }

  // void LCT::clear(){
  //     for (int i = 1; i < n; i++)
  //     {
  //         Nodes[i].rotateTag = false;
  //         Nodes[i].Fa = 0;
  //         Nodes[i].Ch[0] = Nodes[i].Ch[1] = 0;
  //     }
  //     n = 1;
  // }
} 

#endif