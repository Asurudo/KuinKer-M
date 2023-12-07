#ifndef PACKDPH
#define PACKDPH

#include "kuinkerm.h"

/*  背包dp：01背包，完全背包，多重背包
 */
namespace kuinkerm {

  /*  返回背包内物品最大价值
   */
  int packDp::getMaxValue() { return maxValue; }

  /*  物品的个数，背包的容量
   */
   packDp::packDp(int N, int Capacity) : Maxn(N), Capacity(Capacity) { Dp.resize(Maxn); }

  /*  01 背包
      给定一组物品，每个物品都有一个重量 weight 和一个价值
     value。现在有一个最多能装 capacity
     的背包，每种物品可以选择装或者不装进背包，求背包内物品的最大价值总和

      // 尝试将每个物品装入 01 背包
      for(every item)
      packDp.zeroOnePack(item.Weight, item.Value);
      // 输出最大价值
      print(packDp.getMaxValue());
  */
  void packDp::zeroOnePack(int Weight, int Value) {
    for (int i = Capacity; i >= Weight; i--)
      Dp[i] = std::max(Dp[i], Dp[i - Weight] + Value);
    maxValue = Dp[Capacity];
  }

  /*  完全背包
      给定一组物品，每个物品都有一个重量 weight 和一个价值
     value。现在有一个最多能装 capacity
     的背包，每种物品可以选择装入任意个，求背包内物品的最大价值总和

      // 尝试将每个物品装入完全背包
      for(every item)
      packDp.completePack(item.Weight, item.Value);
      // 输出最大价值
      print(packDp.getMaxValue());
  */
  void packDp::completePack(int Weight, int Value) {
    for (int i = Weight; i <= Capacity; i++)
      Dp[i] = std::max(Dp[i], Dp[i - Weight] + Value);
    maxValue = Dp[Capacity];
  }

  /*  多重背包
      给定一组物品，每个物品都有一个重量 weight 和一个价值
     value。现在有一个最多能装 capacity
     的背包，每种物品可以选择装入问题所给的有限个，求背包内物品的最大价值总和

      // 尝试将每个物品装入多重背包
      for(every item)
      packDp.multiplePack(item.Weight, item.Value, item.Amount);
      // 输出最大价值
      print(packDp.getMaxValue());
  */
  void packDp::multiplePack(int Weight, int Value, int Amount) {
    if (Weight * Amount >= Capacity)
      completePack(Weight, Value);
    else {
      int k = 1;
      while (k < Amount) {
        zeroOnePack(k * Weight, k * Value);
        Amount -= k;
        k <<= 1;
      }
      zeroOnePack(Amount * Weight, Amount * Value);
    }
  }

}  // namespace kuinkerm
#endif