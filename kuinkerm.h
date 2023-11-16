/*
    KuinKer-M: My Algorithm Library based on STL
    Author: Asurudo Jyo
    encode: utf-8
*/

#ifndef KUINKERMH
#define KUINKERMH

#include <memory>
#include <algorithm>

namespace kuinkerm{
    using ll = long long;
    constexpr int INF = 0x3f3f3f3f;
    constexpr ll INFLL = 0x3f3f3f3f3f3f3f3f;
    inline int lowbit(int x) {
        return ((x)&(-x));
    }

    /*  背包dp：01背包，完全背包，多重背包
    */
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
            /*  返回背包内物品最大价值
            */
            int getMaxValue(){
                return maxvalue;
            }

            /*  物品的个数，背包的容量
            */
            packDp(int n, int capacity): maxn(n), capacity(capacity), dp(std::make_unique<int[]>(n)) {}
            
            /*  01 背包
                给定一组物品，每个物品都有一个重量 weight 和一个价值 value。现在有一个最多能装 capacity 的背包，每种物品可以选择装或者不装进背包，求背包内物品的最大价值总和
               
                // 尝试将每个物品装入 01 背包
                for(every item)
                  packdp.zeroOnePack(item.weight, item.value);
                // 输出最大价值
                print(packdp.getMaxValue());
            */
            void zeroOnePack(int weight, int value){
                for(int i = capacity; i >= weight; i--)
                    dp[i] = std::max(dp[i], dp[i-weight] + value);
                maxvalue = dp[capacity];
            }
            
            /*  完全背包
                给定一组物品，每个物品都有一个重量 weight 和一个价值 value。现在有一个最多能装 capacity 的背包，每种物品可以选择装入任意个，求背包内物品的最大价值总和
               
                // 尝试将每个物品装入完全背包
                for(every item)
                  packdp.completePack(item.weight, item.value);
                // 输出最大价值
                print(packdp.getMaxValue());
            */
            void completePack(int weight, int value){
                for(int i = weight; i <= capacity; i++)
                    dp[i] = std::max(dp[i], dp[i-weight] + value);
                maxvalue = dp[capacity];
            }
            
            /*  多重背包
                给定一组物品，每个物品都有一个重量 weight 和一个价值 value。现在有一个最多能装 capacity 的背包，每种物品可以选择装入问题所给的有限个，求背包内物品的最大价值总和
               
                // 尝试将每个物品装入多重背包
                for(every item)
                  packdp.multiplePack(item.weight, item.value, item.amount);
                // 输出最大价值
                print(packdp.getMaxValue());
            */
            void multiplePack(int weight, int value, int amount){
                if(weight*amount >= capacity) 
                    completePack(weight, value);
                else{
                    int k = 1;
                    while(k < amount){
                        zeroOnePack(k*weight, k*value);
                        amount -= k;
                        k <<= 1;
                    }
                    zeroOnePack(amount*weight, amount*value);
                }
            }
    };

    /*  存图
    */
    class Graph
    {
        public:
            friend class Dijkstra;
            friend class TSP;
        
        protected:
            // 最大的点数和边数
            int maxn, maxe;
            // 实际的点数和边数
            int n, m;
            // 某一条边的后一条边 
            std::vector<int> next;
            // 边的起始信息，若有x ->(w) y，则head[x]为第一条到达边的编号 
            std::vector<int> head;
            // 边的到达信息，即 x ->(w) y中的y 
            std::vector<int> ver;
            // 边的权值信息，即 x ->(w) y中的w
            std::vector<int> value;
            // 有向边的总数
            int tot;

            /*  最大点数，最大边数，实际点数，实际边数（可不填）
            */
            Graph(int maxn, int maxe, int n, int m = 0): maxn(maxn), maxe(maxe<<1), n(n), m(m){
                tot = 0;
                next.reserve(maxe<<1);
                head.reserve(maxn);
                ver.reserve(maxe<<1);
                value.reserve(maxe<<1);
            }

            /*  向图中加有向边
                注意，加一条无向边等于加两条方向相反，权值相等的有向边

                // 向图中添加一条两端点编号分别为 39 和 42，且权值为 9 的无向边
                g.Add(39, 42, 9);
                g.Add(42, 39, 9);
            */
            void Add(int x, int y, int w = 0){
                ver[++tot] = y,next[tot] = head[x],head[x] = tot,value[tot] = w;
            }
    };

    /*  迪杰斯特拉单源最短路算法
        不支持负边
    */
    struct Dijkstra
    {
        private:
            // <最短距离，顶点编号>
            using Pair = std::pair<ll, int>;
            // 跑迪杰斯特拉最短路径算法的图
            const Graph& graph;
            // 距离数组
            std::vector<ll> distance;
            // 访问数组
            std::vector<bool> visit;
            std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
            // 源点
            int startpoint;

        public:
            /*  图，源点（默认为1）
                构造时直接开始跑最短路
            */

            Dijkstra(const Graph& graph, int startpoint = 1): graph(graph), startpoint(startpoint){
                distance.reserve(graph.maxn);
                visit.reserve(graph.maxn);
                for(int i = 1; i <= graph.n; i ++)
                    distance[i] = INFLL;
                distance[startpoint] = 0;
                pq.push(Pair {0, startpoint});
                while(!pq.empty()){
                    Pair pair = pq.top();
                    pq.pop();
                    int curPoint = pair.second;
                    if(visit[curPoint]) continue;
                    visit[curPoint] = true;
                    for(int i = graph.head[curPoint]; i; i = graph.next[i])
                    {
                        int nxtPoint = graph.ver[i];
                        int edgeValue = graph.value[i];
                        if(distance[nxtPoint] > distance[curPoint] + edgeValue)
                        {
                            distance[nxtPoint] = distance[curPoint] + edgeValue;
                            pq.push(Pair {distance[nxtPoint], nxtPoint});
                            //路径还原 pre[nxtPoint] = curPoint;
                        }
                    }
                }
            }

            /*  返回整个distance数组
                
                const vector<ll>& disRnt = dijkstra.getDistance();
            */
            const std::vector<ll>& getDistance() const{
                return distance;
            }

            /*  返回源点到pointId的最短距离
                
                long long disRnt = dijkstra.getdistance(pointId);
            */
            ll getDistance(int pointId) const{
                return distance[pointId];
            }
    };


    
    /*  给出一个 n 个节点， m 条边的联通无向图，问从 1 出发，游历所有点至少一次后返回，最少需经过的边权总和
    */
    class TSP{
        private://std::shared_ptr<ll[]>
            // 跑TSP算法的图
            const Graph& graph;
            // distance[xPoint][yPoint] 表示 xPoint 到 yPoint 的最小距离
            std::vector<std::vector<ll>> distance;
            std::vector<std::vector<ll>> dp;

            // 最小的边权总和
            ll minedgevaluesum;

            /*  图
                构造时直接开始跑TSP
            */
            TSP(const Graph& graph): graph(graph){
                dp.reserve(graph.maxn);
                distance.reserve(graph.maxn);
                for(int i = 0; i < graph.maxn; i ++)
                {
                    dp[i].reserve(1<<graph.maxn);
                    for(int j = 0; j < (1<<graph.maxn); j ++)
                        dp[i][j] = INFLL;
                    distance[i].reserve(graph.maxn);
                }
                for(int i = 1; i <= graph.n; i++){
                    Dijkstra dij(graph, i);
                    distance[i] = dij.getDistance();
                }
                dp[0][1] = 0;
                for(int S = 0; S < (1<<graph.n); S ++)
                    for(int st = 1; st <= graph.n; st ++)
                        if(S & (1<<(st-1)))
                            for(int ed = 1; ed <= graph.n; ed ++)
                            {
                                if(st==ed || !(S & (1<<(ed-1))))
                                    continue;
                                dp[ed-1][S] = std::min(dp[ed-1][S],dp[st-1][S^(1<<(ed-1))]+distance[st][ed]);
                            }
                minedgevaluesum = INFLL;
                for(int i = 1; i <= graph.n; i ++)
                    minedgevaluesum = std::min(minedgevaluesum, dp[i-1][(1<<graph.n)-1]+distance[i][1]);
            }

            public:
                /*  返回最小边权加和
                */
                ll getMinEdgeValueSum(){
                    return minedgevaluesum;
                }
    };

}

#endif