#ifndef CONNECTINGBLOCKH
#define CONNECTINGBLOCKH

#include "kuinkerm.h"

/*  图的联通块划分
 */
namespace kuinkerm {

connectingBlock::connectingBlock(const Graph& Gp, int gN) : blockCnt(0) {
  if (gN != Gp.n)
    throw std::runtime_error(
        "connectingBlock, the second parameter differs from the point number "
        "in G, "
        "check it");

  belongToBlock.resize(Gp.Maxn);
  std::fill(belongToBlock.begin(), belongToBlock.end(), -1);

  for (int i = 1; i <= Gp.n; i++)
    if (belongToBlock[i] == -1) {
      Blocks.push_back({});
      privateTraverseSubg(i, Gp);
      blockCnt++;
    }
}
void connectingBlock::privateTraverseSubg(int x, const Graph& Gp) {
  belongToBlock[x] = blockCnt;
  Blocks[blockCnt].emplace_back(x);
  for (int i = Gp.Head[x]; i; i = Gp.Next[i]) {
    int y = Gp.Ver[i];
    if (belongToBlock[y] != -1) continue;
    privateTraverseSubg(y, Gp);
  }
}

std::vector<std::vector<int>> connectingBlock::getBlocks() {
    return Blocks;
}
std::vector<int> connectingBlock::getBelongToBlock(){
    return belongToBlock;
}
int connectingBlock::getBlockCnt(){
    return blockCnt;
}
}  // namespace kuinkerm
#endif