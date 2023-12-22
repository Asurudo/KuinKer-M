#ifndef LCADOUBLINGH
#define LCADOUBLINGH

#include "../kuinkerm.h"

/*  LCA
 */
namespace kuinkerm {

LCADoubling::LCADoubling(const Graph& Gp)
    : bitMaxD((int)(std::log(Gp.Maxn) / std::log(2)) + 1) {
  d.resize(Gp.Maxn+2);
  Dist.resize(Gp.Maxn+2);
  f.resize(bitMaxD+2);
  for(int i = 0; i <= bitMaxD; i ++)
    f[i].resize(Gp.Maxn+2);
  
  // 需要用到的临时队列
  std::queue<int> q;
  // 根节点深度为1，bitmaxd = log2底n n为节点数量
  q.push(1);
  d[1] = 1;
  while (!q.empty()) {
    int x = q.front();
    q.pop();
    for (int i = Gp.Head[x]; i; i = Gp.Next[i]) {
      int y = Gp.Ver[i];
      // d[y]被更新过说明y是x的爸爸，略过
      if (d[y]) continue;
      // 深度+1
      d[y] = d[x] + 1;
      // 距离+边权
      // Dist[y] = Dist[x] + Gp.Val[i];
      // y的爸爸是x
      f[0][y] = x;
      // 更新y的列祖列宗
      // dist[x][k]表示从该节点想要到达距离自己2^k的祖先，需要经过的最大边
      // dist[y][0] = g.val[i];
      for (int j = 1; j <= bitMaxD; j++) f[j][y] = f[j - 1][f[j - 1][y]];
      // dp[y,y+2^j] = min(dp[y,y+2^(j-1)],dp[y+2^(j-1),y+2^j])
      // dist[y][j] = min(dist[y][j-1],dist[f[j-1][y]][j-1]);
      q.push(y);
    }
  }
}
int LCADoubling::LCA(int x, int y) {
  // 确保 d[x]≥d[y]
  if (d[x] < d[y]) std::swap(x, y);
  // 让x的深度与y相同
  for (int i = bitMaxD; i >= 0; i--)
    if (d[f[i][x]] >= d[y])
      // ans = min(ans,dist[x][i]),下面一行也是需要的
      x = f[i][x];
  // 若x==y则x和y在一条链上，说明LCA(x,y)=y
  // return ans;
  if (x == y) return y;
  // 否则x和y一起跳到LCA(x,y)的儿子节点
  for (int i = bitMaxD; i >= 0; i--)
    if (f[i][x] != f[i][y])
      // ans = min(ans,dist[x][i]);
      // ans = min(ans,dist[y][i]);
      x = f[i][x], y = f[i][y];
  // return min(ans,min(dist[x][0],dist[y][0]));
  return f[0][x];
}
}  // namespace kuinkerm
#endif