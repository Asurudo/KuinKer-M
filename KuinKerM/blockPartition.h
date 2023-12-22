#ifndef BLOCKPARTITIONH
#define BLOCKPARTITIONH

#include "../kuinkerm.h"

/*  分块
 */
namespace kuinkerm {
blockPartition::blockPartition(const std::vector<ll>& a, int Maxn) : a(a), Maxn(Maxn) {
  blockNum = std::sqrt(a.size());
  // 块怎么分搞不懂看lyd p224
  for (int i = 1; i <= blockNum; i++) {
    L[i] = (i - 1) * blockNum + 1;
    R[i] = i * blockNum;
  }
  // 尾巴还有一点，多分一个块
  if (R[blockNum] < (int)a.size()) {
    blockNum++;
    L[blockNum] = R[blockNum - 1] + 1;
    R[blockNum] = a.size();
  }
  // 预处理 pos和sum
  for (int i = 1; i <= blockNum; i++)
    for (int j = L[i]; j <= R[i]; j++) Pos[j] = i, Sum[i] += a[j];
}

// 将区间[l, r]加上 Value
void blockPartition::change(int l, int r, ll Value) {
  // 此次修改涉及[p,q]的分块
  int p = Pos[l], q = Pos[r];

  // 在同一分块内
  if (p == q) {
    // 局部朴素直接加到原数组
    for (int i = l; i <= r; i++) a[i] += Value;
    // 总和加增量*长度
    Sum[p] += Value * (r - l + 1);
  } else {
    // 除一头一尾，其余中间每一块的增量标记改变
    for (int i = p + 1; i <= q; i++) Add[i] += Value;
    // 对头部的块朴素
    for (int i = l; i <= R[p]; i++) a[i] += Value;
    Sum[p] += Value * (R[p] - l + 1);
    // 对尾部的块朴素
    for (int i = L[q]; i <= r; i++) a[i] += Value;
    Sum[q] += Value * (r - L[q] + 1);
  }
}

// 返回区间[l, r]的和
ll blockPartition::ask(int l, int r) {
  // 此次查询涉及[p,q]的分块
  int p = Pos[l], q = Pos[r];
  ll ans = 0;
  // 在同一分块内
  if (p == q) {
    for (int i = l; i <= r; i++) ans += a[i];
    ans += Add[p] * (r - l + 1);
  } else {
    // 除一头一尾，其余中间每一块一块一块加
    for (int i = p + 1; i <= q; i++) ans += Sum[i] + Add[i] * (R[i] - L[i] + 1);
    // 对头部的块朴素
    for (int i = l; i <= R[p]; i++) ans += a[i];
    ans += Add[p] * (R[p] - l + 1);

    // 对尾部的块朴素
    for (int i = L[q]; i <= r; i++) ans += a[i];
    ans += Add[q] * (r - L[q] + 1);
  }
  return ans;
}

}  // namespace kuinkerm
#endif