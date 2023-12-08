#ifndef GAMETHEORYH
#define GAMETHEORYH

#include "kuinkerm.h"

/* 博弈
   输入一棵树，每次对手可以从自己所在的位置往子树移动一个位置。
   假设发展树的叶子节点为必胜状态。
   如果想把发展树的叶子节点都改成必败状态，则在每个节点后面再接一个节点即可。
   1-index
*/
namespace kuinkerm {

  // 双方都是最优策略
  bool gameTheory::bothSmartDfs(int x, int Fa) {
    bool boundToWin = false, endOfBranch = true;
    for (int i = boardTree.Head[x]; i; i = boardTree.Next[i]) {
      int y = boardTree.Ver[i];
      if (y == Fa) continue;
      endOfBranch = false;
      boundToWin |= bothSmartDfs(y, x);
    }
    if (endOfBranch) return bothSmartDP[x] = true;
    return bothSmartDP[x] = !boundToWin;
  }

  // 双方都是最差策略
  bool gameTheory::bothSillyDfs(int x, int Fa) {
    bool boundToWin = true, endOfBranch = true;
    for (int i = boardTree.Head[x]; i; i = boardTree.Next[i]) {
      int y = boardTree.Ver[i];
      if (y == Fa) continue;
      endOfBranch = false;
      boundToWin &= bothSillyDfs(y, x);
    }
    if (endOfBranch) return bothSillyDp[x] = true;
    return bothSillyDp[x] = !boundToWin;
  }

  // 先手最优策略，后手最差策略
  bool gameTheory::smartSillyDfs(int x, int Fa, bool isSmart) {
    bool boundToWin = false, endOfBranch = true;
    for (int i = boardTree.Head[x]; i; i = boardTree.Next[i]) {
      int y = boardTree.Ver[i];
      if (y == Fa) continue;
      endOfBranch = false;
      if (isSmart == true)
        boundToWin |= smartSillyDfs(y, x, !isSmart);
      else
        boundToWin &= smartSillyDfs(y, x, !isSmart);
    }
    if (endOfBranch) return bothSmartDP[x] = true;
    return bothSmartDP[x] = !boundToWin;
  }

  gameTheory::gameTheory(const Graph& boardTree, const std::string& firstMove,
             const std::string& secondMove)
      : boardTree(boardTree) {
    if (firstMove == "SMART" && secondMove == "SMART") {
      bothSmartDP.resize(boardTree.Maxn);
      bothSmartDfs(1, -1);
      // 子节点存在必胜 = 先手必胜
      firstMoveWin = !bothSmartDP[1];
    } else if (firstMove == "SILLY" && secondMove == "SILLY") {
      bothSillyDp.resize(boardTree.Maxn);
      bothSillyDfs(1, -1);
      // 子节点都为必胜 = 先手必胜
      firstMoveWin = !bothSillyDp[1];
    } else if (firstMove == "SMART" && secondMove == "SILLY") {
      smartSillyDp.resize(boardTree.Maxn);
      smartSillyDfs(1, -1, true);
      // 子节点存在必胜 = 先手必胜
      firstMoveWin = smartSillyDp[1];
    } else if (firstMove == "SILLY" && secondMove == "SMART") {
      sillySmartDp.resize(boardTree.Maxn);
      smartSillyDfs(1, -1, false);
      // 子节点都为必胜 = 先手必胜
      firstMoveWin = smartSillyDp[1];
    } else
      throw std::runtime_error("gameTheory, input parameter ERROR");
  }

  bool gameTheory::getFirstMove() { return firstMoveWin; }

}  // namespace kuinkerm
#endif