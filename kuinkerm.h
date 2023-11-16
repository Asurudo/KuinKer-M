/*
    KuinKer-M: My Algorithm Library based on STL
    Author: Asurudo Jyo
*/

#ifndef KUINKERMH
#define KUINKERMH

#include <memory>
#include <algorithm>

typedef long long ll;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define LLINF 0x3f3f3f3f3f3f3f3f
#define lowbit(x) ((x)&(-x))

namespace kuinkerm{
    class packDp
    {
	    private:
            // 最多有多少个物品
            int maxn;
            std::unique_ptr<int[]> dp; 
	        // 背包最大容量
            int capacity;
            // 背包内物品最大价值
            int maxvalue;
        public:
            int getMaxValue(){
                return maxvalue;
            }
            packDp(int n, int capacity): maxn(n), capacity(capacity), dp(std::make_unique<int[]>(n)) {}
            
            /*  01 背包
                给定一组物品，每个物品都有一个重量 weight 和一个价值 value。现在有一个最多能装 capacity 的背包，每种物品可以选择装或者不装进背包，求背包内物品的最大价值总和。 
               
                // 尝试将每个物品装入 01 背包
                for(every item)
                  kuinkerm::ZeroOnePack(item.weight, item.value);
                // 输出最大价值
                print(kuinkerm::getMaxValue());
            */
            void ZeroOnePack(int weight, int value){
                for(int i = capacity; i >= weight; i--)
                    dp[i] = std::max(dp[i], dp[i-weight] + value);
                maxvalue = dp[capacity];
            }
            
            /*  完全背包
                给定一组物品，每个物品都有一个重量 weight 和一个价值 value。现在有一个最多能装 capacity 的背包，每种物品可以选择装入任意个，求背包内物品的最大价值总和。 
               
                // 尝试将每个物品装入完全背包
                for(every item)
                  kuinkerm::CompletePack(item.weight, item.value);
                // 输出最大价值
                print(kuinkerm::getMaxValue());
            */
            void CompletePack(int weight, int value){
                for(int i = weight; i <= capacity; i++)
                    dp[i] = std::max(dp[i], dp[i-weight] + value);
                maxvalue = dp[capacity];
            }
            
            /*  多重背包
                给定一组物品，每个物品都有一个重量 weight 和一个价值 value。现在有一个最多能装 capacity 的背包，每种物品可以选择装入任意个，求背包内物品的最大价值总和。 
               
                // 尝试将每个物品装入多重背包
                for(every item)
                  kuinkerm::MultiplePack(item.weight, item.value, item.amount);
                // 输出最大价值
                print(kuinkerm::getMaxValue());
            */
            void MultiplePack(int weight, int value, int amount){
                if(weight*amount >= capacity) 
                    CompletePack(weight, value);
                else{
                    int k = 1;
                    while(k < amount){
                        ZeroOnePack(k*weight, k*value);
                        amount -= k;
                        k <<= 1;
                    }
                    ZeroOnePack(amount*weight, amount*value);
                }
            }
    };

    class TSP{

    };

}

#endif