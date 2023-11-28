/*
    KuinKer-M: My Algorithm Library based on STL
    Author: Asurudo Jyo
    encode: utf-8
*/

#ifndef KUINKERMH
#define KUINKERMH

#include <memory>
#include <algorithm>
#include <type_traits>
#include <ext/rope>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/hash_policy.hpp>
#include <ext/pb_ds/priority_queue.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/assoc_container.hpp>

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
            int Maxn;
            std::vector<int> Dp; 
	        // 背包最大容量
            int Capacity;
            // 背包内物品最大价值
            int maxValue;
        public:
            /*  返回背包内物品最大价值
            */
            int getMaxValue(){
                return maxValue;
            }

            /*  物品的个数，背包的容量
            */
            packDp(int N, int Capacity): Maxn(N), Capacity(Capacity) {
                Dp.resize(Maxn);
            }
            
            /*  01 背包
                给定一组物品，每个物品都有一个重量 weight 和一个价值 value。现在有一个最多能装 capacity 的背包，每种物品可以选择装或者不装进背包，求背包内物品的最大价值总和
               
                // 尝试将每个物品装入 01 背包
                for(every item)
                  packDp.zeroOnePack(item.Weight, item.Value);
                // 输出最大价值
                print(packDp.getMaxValue());
            */
            void zeroOnePack(int Weight, int Value){
                for(int i = Capacity; i >= Weight; i--)
                    Dp[i] = std::max(Dp[i], Dp[i-Weight] + Value);
                maxValue = Dp[Capacity];
            }
            
            /*  完全背包
                给定一组物品，每个物品都有一个重量 weight 和一个价值 value。现在有一个最多能装 capacity 的背包，每种物品可以选择装入任意个，求背包内物品的最大价值总和
               
                // 尝试将每个物品装入完全背包
                for(every item)
                  packDp.completePack(item.Weight, item.Value);
                // 输出最大价值
                print(packDp.getMaxValue());
            */
            void completePack(int Weight, int Value){
                for(int i = Weight; i <= Capacity; i++)
                    Dp[i] = std::max(Dp[i], Dp[i-Weight] + Value);
                maxValue = Dp[Capacity];
            }
            
            /*  多重背包
                给定一组物品，每个物品都有一个重量 weight 和一个价值 value。现在有一个最多能装 capacity 的背包，每种物品可以选择装入问题所给的有限个，求背包内物品的最大价值总和
               
                // 尝试将每个物品装入多重背包
                for(every item)
                  packDp.multiplePack(item.Weight, item.Value, item.Amount);
                // 输出最大价值
                print(packDp.getMaxValue());
            */
            void multiplePack(int Weight, int Value, int Amount){
                if(Weight*Amount >= Capacity) 
                    completePack(Weight, Value);
                else{
                    int k = 1;
                    while(k < Amount){
                        zeroOnePack(k*Weight, k*Value);
                        Amount -= k;
                        k <<= 1;
                    }
                    zeroOnePack(Amount*Weight, Amount*Value);
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
            int Maxn, Maxe;
            // 实际的点数和边数
            int n, m;
            // 某一条边的后一条边 
            std::vector<int> Next;
            // 边的起始信息，若有x ->(w) y，则Head[x]为第一条到达边的编号 
            std::vector<int> Head;
            // 边的到达信息，即 x ->(w) y中的y 
            std::vector<int> Ver;
            // 边的权值信息，即 x ->(w) y中的w
            std::vector<int> Value;
            // 有向边的总数
            int Tot;

            /*  最大点数，最大边数，实际点数，实际边数（可不填）
            */
            Graph(int Maxn, int Maxe, int n, int m = 0): Maxn(Maxn), Maxe(Maxe<<1), n(n), m(m){
                Tot = 0;
                Next.resize(Maxe<<1);
                Head.resize(Maxn);
                Ver.resize(Maxe<<1);
                Value.resize(Maxe<<1);
            }

            /*  向图中加有向边
                注意，加一条无向边等于加两条方向相反，权值相等的有向边

                // 向图中添加一条两端点编号分别为 39 和 42，且权值为 9 的无向边
                g.Add(39, 42, 9);
                g.Add(42, 39, 9);
            */
            void Add(int x, int y, int w = 0){
                Ver[++Tot] = y, Next[Tot] = Head[x], Head[x] = Tot, Value[Tot] = w;
            }
    };

    /*  迪杰斯特拉单源最短路算法
        不支持负边
    */
    class Dijkstra
    {
        private:
            // <最短距离，顶点编号>
            using Pair = std::pair<ll, int>;
            // 跑迪杰斯特拉最短路径算法的图
            const Graph& Gp;
            // 距离数组
            std::vector<ll> Distance;
            // 访问数组
            std::vector<bool> Visit;
            std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> Pq;
            // 源点
            int startPoint;

        public:
            /*  图，源点（默认为1）
                构造时直接开始跑最短路
            */

            Dijkstra(const Graph& Gp, int startPoint = 1): Gp(Gp), startPoint(startPoint){
                Distance.resize(Gp.Maxn);
                Visit.resize(Gp.Maxn);
                for(int i = 1; i <= Gp.n; i ++)
                    Distance[i] = INFLL;
                Distance[startPoint] = 0;
                Pq.push(Pair {0, startPoint});
                while(!Pq.empty()){
                    Pair Pa = Pq.top();
                    Pq.pop();
                    int curPoint = Pa.second;
                    if(Visit[curPoint]) continue;
                    Visit[curPoint] = true;
                    for(int i = Gp.Head[curPoint]; i; i = Gp.Next[i])
                    {
                        int nxtPoint = Gp.Ver[i];
                        int edgeValue = Gp.Value[i];
                        if(Distance[nxtPoint] > Distance[curPoint] + edgeValue)
                        {
                            Distance[nxtPoint] = Distance[curPoint] + edgeValue;
                            Pq.push(Pair {Distance[nxtPoint], nxtPoint});
                            //路径还原 pre[nxtPoint] = curPoint;
                        }
                    }
                }
            }

            /*  返回整个Distance数组
                
                const vector<ll>& disRnt = dijkstra.getDistance();
            */
            const std::vector<ll>& getDistance() const{
                return Distance;
            }

            /*  返回源点到pointId的最短距离
                
                long long disRnt = dijkstra.getDistance(pointId);
            */
            ll getDistance(int pointId) const{
                return Distance[pointId];
            }
    };
    
    /*  给出一个 n 个节点， m 条边的联通无向图，问从 1 出发，游历所有点至少一次后返回，最少需经过的边权总和
    */
    class TSP{
        private:
            // 跑TSP算法的图
            const Graph& Gp;
            // distance[xPoint][yPoint] 表示 xPoint 到 yPoint 的最小距离
            std::vector<std::vector<ll>> Distance;
            std::vector<std::vector<ll>> Dp;

            // 最小的边权总和
            ll minEdgeValueSum;

            public:
                /* 图构造时直接开始跑TSP
                */
                TSP(const Graph& Gp): Gp(Gp){
                    Dp.resize(Gp.Maxn);
                    Distance.resize(Gp.Maxn);
                    for(int i = 0; i < Gp.Maxn; i ++)
                    {
                        Dp[i].resize(1<<Gp.Maxn);
                        for(int j = 0; j < (1<<Gp.Maxn); j ++)
                            Dp[i][j] = INFLL;
                        Distance[i].resize(Gp.Maxn);
                    }
                    for(int i = 1; i <= Gp.n; i++){
                        Dijkstra Dij(Gp, i);
                        Distance[i] = Dij.getDistance();
                    }
                    Dp[0][1] = 0;
                    for(int S = 0; S < (1<<Gp.n); S ++)
                        for(int St = 1; St <= Gp.n; St ++)
                            if(S & (1<<(St-1)))
                                for(int Ed = 1; Ed <= Gp.n; Ed ++)
                                {
                                    if(St==Ed || !(S & (1<<(Ed-1))))
                                        continue;
                                    Dp[Ed-1][S] = std::min(Dp[Ed-1][S],Dp[St-1][S^(1<<(Ed-1))]+Distance[St][Ed]);
                                }
                    minEdgeValueSum = INFLL;
                    for(int i = 1; i <= Gp.n; i ++)
                        minEdgeValueSum = std::min(minEdgeValueSum, Dp[i-1][(1<<Gp.n)-1]+Distance[i][1]);
                }
                /*  返回最小边权加和
                */
                ll getMinEdgeValueSum(){
                    return minEdgeValueSum;
                }
    };

    /*  LIS/LDS
    */
    class LISLDS{
        private:
            // 是最长上升序列
            bool isLIS;
            // 是严格的
            bool isStrict;
            // 输入数组
            std::vector<ll> a;
            // dp数组
        public:
            std::vector<ll> Dp;
            // 最长的长度
            int rntLength;

            void strictLIS()
            {
                if(a.empty())
                    return ;
                Dp[0] = a[0];
                int dpLen = 0;
                for(int i = 1; i < (int)a.size();i ++)
                    if(a[i] > Dp[dpLen])
                        Dp[++dpLen] = a[i];
                    else{
                        int k = std::lower_bound(Dp.begin(), Dp.begin()+dpLen+1, a[i]) - Dp.begin();
                        Dp[k] = a[i];
                        std::cout << k << " ";
                    }
                rntLength = dpLen+1;
            }

            void lenientLIS()
            {
                if(a.empty())
                    return ;
                Dp[0] = a[0];
                int dpLen = 0;
                for(int i = 1; i < (int)a.size();i ++)
                    if(a[i] >= Dp[dpLen])
                        Dp[++dpLen] = a[i];
                    else
                    {
                        int k = std::upper_bound(Dp.begin(), Dp.begin()+dpLen+1, a[i]) - Dp.begin();
                        Dp[k] = a[i];
                    }
                rntLength = dpLen+1;
            }

            void strictLDS()
            {
                if(a.empty())
                    return ;
                Dp[0] = a[0];
                int dpLen = 0;
                for(int i = 1; i < (int)a.size();i ++)
                    if(a[i] < Dp[dpLen])
                        Dp[++dpLen] = a[i];
                    else
                    {
                        int k = std::lower_bound(Dp.begin(), Dp.begin()+dpLen+1, a[i], std::greater<ll>()) - Dp.begin();
                        Dp[k] = a[i];
                    }
                rntLength = dpLen+1;
            }

            void lenientLDS()
            {
                if(a.empty())
                    return ;
                Dp[0] = a[0];
                int dpLen = 0;
                for(int i = 1; i < (int)a.size();i ++)
                    if(a[i] <= Dp[dpLen])
                        Dp[++dpLen] = a[i];
                    else
                    {
                        int k = std::upper_bound(Dp.begin(), Dp.begin()+dpLen+1, a[i], std::greater<ll>()) - Dp.begin();
                        Dp[k] = a[i];
                    }
                rntLength = dpLen+1;
            }

            public:
                LISLDS(const std::vector<ll>& a, const std::string& isLIS, const std::string& isStrict): a(a){
                    Dp.resize(a.size());
                    if(isLIS == "LIS" && isStrict == "STRICT")
                        strictLIS(), this->isLIS = true, this->isStrict = true;
                    else if(isLIS == "LIS" && isStrict == "LENIENT")
                        lenientLIS(), this->isLIS = true, this->isStrict = false;
                    else if(isLIS == "LDS" && isStrict == "STRICT")
                        strictLDS(), this->isLIS = false, this->isStrict = true;
                    else if(isLIS == "LDS" && isStrict == "LENIENT")
                        lenientLDS(), this->isLIS = false, this->isStrict = false;
                    else 
                        throw std::runtime_error("LISLDS, input parameter ERROR");
                }

                /*  返回最小边权加和
                */
                int getLength(){
                    return rntLength;
                }
    };

    /*  大数
    */
    class bigInt
    {
        private:
            constexpr static int BASE = 1000000000;
            constexpr static int BASEDIGITS = 9;
            using vll = std::vector<ll>;
            // value == 0 is represented by empty z
            std::vector<int> z;

            // 1 为非负数，-1为负数
            int Sign;

            // 读入字符串的内部函数
            void read(const std::string& s)
            {
                Sign = 1;
                z.clear();
                int pos = 0;
                while (pos < (int)s.size() && (s[pos] == '-' || s[pos] == '+'))
                {
                    if (s[pos] == '-')
                        Sign = -Sign;
                    ++pos;
                }
                for (int i = (int)s.size() - 1; i >= pos; i -= BASEDIGITS)
                {
                    int x = 0;
                    for (int j = std::max(pos, i - BASEDIGITS + 1); j <= i; j++)
                        x = x * 10 + s[j] - '0';
                    z.push_back(x);
                }
                Trim();
            }

            // 取余的内部函数
            friend std::pair<bigInt, bigInt> divMod(const bigInt& a1, const bigInt& b1)
            {
                int Norm = BASE / (b1.z.back() + 1);
                bigInt a = a1.Abs() * Norm;
                bigInt b = b1.Abs() * Norm;
                bigInt q, r;
                q.z.resize(a.z.size());

                for (int i = (int)a.z.size() - 1; i >= 0; i--)
                {
                    r *= BASE;
                    r += a.z[i];
                    int s1 = b.z.size() < r.z.size() ? r.z[b.z.size()] : 0;
                    int s2 = b.z.size() - 1 < r.z.size() ? r.z[b.z.size() - 1] : 0;
                    int d = (int)(((long long)s1 * BASE + s2) / b.z.back());
                    r -= b * d;
                    while (r < 0)
                        r += b, --d;
                    q.z[i] = d;
                }

                q.Sign = a1.Sign * b1.Sign;
                r.Sign = a1.Sign;
                q.Trim();
                r.Trim();
                return {q, r / Norm};
            }

            // 去除前导0的内部函数
            void Trim()
            {
                while (!z.empty() && z.back() == 0)
                    z.pop_back();
                if (z.empty())
                    Sign = 1;
            }

            // 转换内部进制的内部函数
            static std::vector<int> convertBASE(const std::vector<int>& a, int oldDigits, int newDigits)
            {
                std::vector<ll> p(std::max(oldDigits, newDigits) + 1);
                p[0] = 1;
                for (int i = 1; i < (int)p.size(); i++)
                    p[i] = p[i - 1] * 10;
                std::vector<int> res;
                long long cur = 0;
                int curDigits = 0;
                for (int v : a)
                {
                    cur += v * p[curDigits];
                    curDigits += oldDigits;
                    while (curDigits >= newDigits)
                    {
                        res.push_back(int(cur % p[newDigits]));
                        cur /= p[newDigits];
                        curDigits -= newDigits;
                    }
                }
                res.push_back((int)cur);
                while (!res.empty() && res.back() == 0)
                    res.pop_back();
                return res;
            }

            // 实现高效乘法的内部函数
            static vll karatsubaMultiply(const vll& a, const vll& b)
            {
                int n = a.size();
                vll res(n + n);
                if (n <= 32)
                {
                    for (int i = 0; i < n; i++)
                        for (int j = 0; j < n; j++)
                            res[i + j] += a[i] * b[j];
                    return res;
                }

                int k = n >> 1;
                vll a1(a.begin(), a.begin() + k);
                vll a2(a.begin() + k, a.end());
                vll b1(b.begin(), b.begin() + k);
                vll b2(b.begin() + k, b.end());

                vll a1b1 = karatsubaMultiply(a1, b1);
                vll a2b2 = karatsubaMultiply(a2, b2);

                for (int i = 0; i < k; i++)
                    a2[i] += a1[i];
                for (int i = 0; i < k; i++)
                    b2[i] += b1[i];

                vll r = karatsubaMultiply(a2, b2);
                for (int i = 0; i < (int)a1b1.size(); i++)
                    r[i] -= a1b1[i];
                for (int i = 0; i < (int)a2b2.size(); i++)
                    r[i] -= a2b2[i];

                for (int i = 0; i < (int)r.size(); i++)
                    res[i + k] += r[i];
                for (int i = 0; i < (int)a1b1.size(); i++)
                    res[i] += a1b1[i];
                for (int i = 0; i < (int)a2b2.size(); i++)
                    res[i + n] += a2b2[i];
                return res;
            }
        
        public:
            bigInt() : Sign(1) {}
            bigInt(long long v) { *this = v; }
            bigInt(const std::string& s) { read(s); }

            bigInt& operator=(long long v)
            {
                Sign = v < 0 ? -1 : 1;
                v *= Sign;
                z.clear();
                for (; v > 0; v = v / BASE)
                    z.push_back((int)(v % BASE));
                return *this;
            }

            bigInt& operator+=(const bigInt& Other)
            {
                if (Sign == Other.Sign)
                {
                    for (int i = 0, Carry = 0; i < (int)Other.z.size() || Carry; ++i)
                    {
                        if (i == (int)z.size())
                            z.push_back(0);
                        z[i] += Carry + (i < (int)Other.z.size() ? Other.z[i] : 0);
                        Carry = z[i] >= BASE;
                        if (Carry)
                            z[i] -= BASE;
                    }
                }
                else if (Other != 0 /* prevent infinite loop */)
                {
                    *this -= -Other;
                }
                return *this;
            }

            friend bigInt operator+(bigInt a, const bigInt& b)
            {
                return a += b;
            }

            bigInt& operator-=(const bigInt& Other)
            {
                if (Sign == Other.Sign)
                {
                    if (Sign == 1 && *this >= Other || Sign == -1 && *this <= Other)
                    {
                        for (int i = 0, Carry = 0; i < (int)Other.z.size() || Carry; ++i)
                        {
                            z[i] -= Carry + (i < (int)Other.z.size() ? Other.z[i] : 0);
                            Carry = z[i] < 0;
                            if (Carry)
                                z[i] += BASE;
                        }
                        Trim();
                    }
                    else
                    {
                        *this = Other - *this;
                        this->Sign = -this->Sign;
                    }
                }
                else
                {
                    *this += -Other;
                }
                return *this;
            }

            friend bigInt operator-(bigInt a, const bigInt& b)
            {
                return a -= b;
            }

            bigInt& operator*=(int v)
            {
                if (v < 0)
                    Sign = -Sign, v = -v;
                for (int i = 0, Carry = 0; i < (int)z.size() || Carry; ++i)
                {
                    if (i == (int)z.size())
                        z.push_back(0);
                    long long cur = (long long)z[i] * v + Carry;
                    Carry = (int)(cur / BASE);
                    z[i] = (int)(cur % BASE);
                }
                Trim();
                return *this;
            }

            bigInt operator*(int v) const
            {
                return bigInt(*this) *= v;
            }

            bigInt operator*(const bigInt& v) const
            {
                std::vector<int> a6 = convertBASE(this->z, BASEDIGITS, 6);
                std::vector<int> b6 = convertBASE(v.z, BASEDIGITS, 6);
                vll a(a6.begin(), a6.end());
                vll b(b6.begin(), b6.end());
                while (a.size() < b.size())
                    a.push_back(0);
                while (b.size() < a.size())
                    b.push_back(0);
                while (a.size() & (a.size() - 1))
                    a.push_back(0), b.push_back(0);
                vll c = karatsubaMultiply(a, b);
                bigInt res;
                res.Sign = Sign * v.Sign;
                for (int i = 0, Carry = 0; i < (int)c.size(); i++)
                {
                    long long cur = c[i] + Carry;
                    res.z.push_back((int)(cur % 1000000));
                    Carry = (int)(cur / 1000000);
                }
                res.z = convertBASE(res.z, 6, BASEDIGITS);
                res.Trim();
                return res;
            }

            friend bigInt sqrt(const bigInt& a1)
            {
                bigInt a = a1;
                while (a.z.empty() || a.z.size() % 2 == 1)
                    a.z.push_back(0);

                int n = a.z.size();

                int firstDigit = (int)::sqrt((double)a.z[n - 1] * BASE + a.z[n - 2]);
                int Norm = BASE / (firstDigit + 1);
                a *= Norm;
                a *= Norm;
                while (a.z.empty() || a.z.size() % 2 == 1)
                    a.z.push_back(0);

                bigInt r = (long long)a.z[n - 1] * BASE + a.z[n - 2];
                firstDigit = (int)::sqrt((double)a.z[n - 1] * BASE + a.z[n - 2]);
                int q = firstDigit;
                bigInt res;

                for (int j = n / 2 - 1; j >= 0; j--)
                {
                    for (;; --q)
                    {
                        bigInt r1 = (r - (res * 2 * BASE + q) * q) * BASE * BASE + (j > 0 ? (long long)a.z[2 * j - 1] * BASE + a.z[2 * j - 2] : 0);
                        if (r1 >= 0)
                        {
                            r = r1;
                            break;
                        }
                    }
                    res *= BASE;
                    res += q;

                    if (j > 0)
                    {
                        int d1 = res.z.size() + 2 < r.z.size() ? r.z[res.z.size() + 2] : 0;
                        int d2 = res.z.size() + 1 < r.z.size() ? r.z[res.z.size() + 1] : 0;
                        int d3 = res.z.size() < r.z.size() ? r.z[res.z.size()] : 0;
                        q = (int)(((long long)d1 * BASE * BASE + (long long)d2 * BASE + d3) / (firstDigit * 2));
                    }
                }

                res.Trim();
                return res / Norm;
            }

            bigInt operator/(const bigInt& v) const
            {
                return divMod(*this, v).first;
            }

            bigInt operator%(const bigInt& v) const
            {
                return divMod(*this, v).second;
            }

            bigInt& operator/=(int v)
            {
                if (v < 0)
                    Sign = -Sign, v = -v;
                for (int i = (int)z.size() - 1, rem = 0; i >= 0; --i)
                {
                    long long cur = z[i] + rem * (long long)BASE;
                    z[i] = (int)(cur / v);
                    rem = (int)(cur % v);
                }
                Trim();
                return *this;
            }

            bigInt operator/(int v) const
            {
                return bigInt(*this) /= v;
            }

            int operator%(int v) const
            {
                if (v < 0)
                    v = -v;
                int m = 0;
                for (int i = (int)z.size() - 1; i >= 0; --i)
                    m = (int)((z[i] + m * (long long)BASE) % v);
                return m * Sign;
            }

            bigInt& operator*=(const bigInt& v)
            {
                *this = *this * v;
                return *this;
            }

            bigInt& operator/=(const bigInt& v)
            {
                *this = *this / v;
                return *this;
            }

            bool operator<(const bigInt& v) const
            {
                if (Sign != v.Sign)
                    return Sign < v.Sign;
                if (z.size() != v.z.size())
                    return z.size() * Sign < v.z.size() * v.Sign;
                for (int i = (int)z.size() - 1; i >= 0; i--)
                    if (z[i] != v.z[i])
                        return z[i] * Sign < v.z[i] * Sign;
                return false;
            }

            bool operator>(const bigInt& v) const
            {
                return v < *this;
            }

            bool operator<=(const bigInt& v) const
            {
                return !(v < *this);
            }

            bool operator>=(const bigInt& v) const
            {
                return !(*this < v);
            }

            bool operator==(const bigInt& v) const
            {
                return !(*this < v) && !(v < *this);
            }

            bool operator!=(const bigInt& v) const
            {
                return *this < v || v < *this;
            }

            friend bigInt operator-(bigInt v)
            {
                if (!v.z.empty())
                    v.Sign = -v.Sign;
                return v;
            }
            
            friend std::istream& operator>>(std::istream& stream, bigInt& v)
            {
                std::string s;
                stream >> s;
                v.read(s);
                return stream;
            }

            friend std::ostream& operator<<(std::ostream& stream, const bigInt& v)
            {
                if (v.Sign == -1)
                    stream << '-';
                stream << (v.z.empty() ? 0 : v.z.back());
                for (int i = (int)v.z.size() - 2; i >= 0; --i)
                    stream << std::setw(BASEDIGITS) << std::setfill('0') << v.z[i];
                return stream;
            }
        
            // 判断是否为0
            bool isZero() const
            {
                return z.empty();
            }
            // 绝对值
            bigInt Abs() const
            {
                return Sign == 1 ? *this : -*this;
            }
            // 将其转为long long类型
            long long longValue() const
            {
                long long res = 0;
                for (int i = (int)z.size() - 1; i >= 0; i--)
                    res = res * BASE + z[i];
                return res * Sign;
            }
            // 计算最大公因数
            static bigInt Gcd(const bigInt& a, const bigInt& b)
            {
                return b.isZero() ? a : Gcd(b, a % b);
            }
            // 计算最小公倍数
            static bigInt Lcm(const bigInt& a, const bigInt& b)
            {
                return a / Gcd(a, b) * b;
            }
            // 快速幂
            static bigInt Power(const bigInt& Base, const bigInt& Exponent) {
                bigInt a = Base;
                bigInt b = Exponent;
                
                if (b == 0) {
                    return 1;
                }
                bigInt Result = 1;
                while (b > 0) {
                    if (b % 2 == 1) {
                        Result *= a;
                    }
                    a *= a;
                    b /= 2;
                }
                return Result;
            }
    };

    /*  并查集
    */
    class disjointSet{
        private:
            // 最多有多少个集合
            int Maxn;
            // 每个集合的父节点
            std::vector<int> Parent;
            // 每个集合的高度
            std::vector<int> High;
        public:
            disjointSet(int Maxn): Maxn(Maxn){
                Parent.resize(Maxn+1);
                High.resize(Maxn+1);
                
                for(int i = 1; i < Maxn; i ++)
                    Parent[i] = i, High[i] = 0;
            }

            // 查询集合 x 的最终父亲
            int find(int x){
                return Parent[x] == x ? x : Parent[x] = find(Parent[x]);
            }

            // 将集合 x 和集合 y 所属的两个集合连接起来
            void unite(int x, int y){
                x = find(x);
                y = find(y);
                if(x == y) return ;
                if(High[x] < High[y])
                    Parent[x] = y;
                else{
                    Parent[y] = x;
                    if(High[x] == High[y])
                        High[x] ++;
                }
            }
            // 检查集合 x 和集合 y是否属于同一集合
            bool same(int x, int y){
                return find(x) == find(y);
            }
    };

    /*  分块
    */
    class blockPartition{
        // 数组最大长度，块的个数
        int Maxn, blockNum;
        // 输入数组
        std::vector<ll> a;

        // sum[i]表示第 i 段的原始加和
        // add[i]表示第 i 段的每个元素待加值/增量标记
        std::vector<ll> Sum, Add;
        
        // L[i]和R[i]表示第 i 段在原数组中的左右端点下标
        std::vector<int> L, R;
        
        //pos[j]表示 原数组下标 j 在哪一段
        std::vector<int> Pos;

        blockPartition(const std::vector<ll>& a, int Maxn): a(a), Maxn(Maxn){
            blockNum = sqrt(a.size());
            //块怎么分搞不懂看lyd p224
            for(int i = 1; i <= blockNum; i ++){
                L[i] = (i-1) * blockNum + 1;
                R[i] = i * blockNum;
            }
            //尾巴还有一点，多分一个块
            if(R[blockNum] < (int)a.size()){
                blockNum ++;
                L[blockNum] = R[blockNum-1] + 1;
                R[blockNum] = a.size();
            }
            //预处理 pos和sum
            for(int i = 1; i <= blockNum; i ++)
                for(int j = L[i]; j <= R[i]; j ++)
                    Pos[j] = i, Sum[i] += a[j];
        }

        // 将区间[l, r]加上 Value
        void change(int l, int r, ll Value){
            //此次修改涉及[p,q]的分块
            int p = Pos[l], q = Pos[r];

            //在同一分块内
            if(p==q)
            {
                //局部朴素直接加到原数组
                for(int i = l; i <= r; i ++)
                    a[i] += Value;
                //总和加增量*长度
                Sum[p] += Value*(r-l+1);
            }
            else
            {
                //除一头一尾，其余中间每一块的增量标记改变
                for(int i = p+1; i <= q; i ++)
                    Add[i] += Value;
                //对头部的块朴素
                for(int i = l; i <= R[p]; i ++)
                    a[i] += Value;
                Sum[p] += Value*(R[p] - l + 1);
                //对尾部的块朴素
                for(int i = L[q]; i <= r; i ++)
                    a[i] += Value;
                Sum[q] += Value*(r - L[q] + 1);
            }
        }

        // 返回区间[l, r]的和
        ll ask(int l, int r){
            //此次查询涉及[p,q]的分块
            int p = Pos[l], q = Pos[r];
            ll ans = 0;
            //在同一分块内
            if(p==q){
                for(int i = l; i <= r; i ++)
                    ans += a[i];
                ans += Add[p] * (r-l+1);
            }
            else{
                //除一头一尾，其余中间每一块一块一块加
                for(int i = p+1; i <= q; i ++)
                    ans += Sum[i] + Add[i]*(R[i]-L[i]+1);
                //对头部的块朴素
                for(int i = l; i <= R[p]; i ++)
                    ans += a[i];
                ans += Add[p]*(R[p]-l+1);
                
                //对尾部的块朴素
                for(int i = L[q]; i <= r; i ++)
                    ans += a[i];
                ans += Add[q]*(r-L[q]+1);
            }
            return ans;
        }
    };

    /*  哈希
    */
    template<typename T1, typename T2>
    class HashTable{
        private:
            // T1为键值，T2为数据
            __gnu_pbds::gp_hash_table<T1, T2> hTable;

        public:
            // 插入一条哈希数据
            void Insert(T1 x, T2 y){
                hTable.insert(std::make_pair(x, y));
            }

            // 用下标访问哈希数据，如 table[x] 就是键值的对应数据
            T2& operator[](T1 x){
                return hTable[x];
            }
            const T2& operator[](T1 x) const{
                return hTable[x];
            }

            // 返回一个指针，first为键值，second为数据
            auto Find(T1 x){
                return hTable.find(x);
            }

            // 删除键值x的哈希数据
            void Erase(T1 x){
                hTable.erase(x);
            }
            void Clear(){
                hTable.clear();
            }
            int Size(){
                return hTable.size();
            }
            bool Empty(){
                return hTable.empty();
            }
    };

    /*  优先队列
    */
    template<typename T, typename F = void>
    class priorityQueue{
        private:
            using Comparator = std::function<bool(const T&, const T&)>;
            using heapTag = __gnu_pbds::pairing_heap_tag;
            using pqType = typename std::conditional<std::is_void<F>::value,
                                    __gnu_pbds::priority_queue<T, Comparator, heapTag>,
                                    __gnu_pbds::priority_queue<T, F, heapTag>>::type;
            using pqIter = typename pqType::point_iterator;

            // 传入的是否是基本类型，是否重载了调用运算符（F是否为空）
            bool isFundamentalType, wantUseTemplate;
            
            // 对值的映射，方便以后根据值进行查询和更改
            // std::map<T, std::set<pqIter>> val2PointerMp;

            std::unique_ptr<pqType> Pq;

        public:
            priorityQueue(){
                isFundamentalType = std::is_fundamental<T>::value;
                wantUseTemplate = !std::is_void<F>::value;
                if (false == isFundamentalType && false == wantUseTemplate){
                    throw std::runtime_error("priorityQueue, input type is not the fundamental type and the comparision function or struct is not provided.");
                    return ;
                }
                else if(true == wantUseTemplate)
                    Pq = std::make_unique<pqType>();
                else
                    Pq = std::make_unique<pqType>();
            }

            priorityQueue(Comparator f){
                isFundamentalType = std::is_fundamental<T>::value;
                wantUseTemplate = !std::is_void<F>::value;
                if (false == isFundamentalType && true == wantUseTemplate){
                    throw std::runtime_error("priorityQueue, both comparision function and struct are existed, delete one of them");
                    return ;
                }
                Pq = std::make_unique<__gnu_pbds::priority_queue<T, Comparator, heapTag>>(f);
            }

            auto Push(T x){
                return Pq->push(x);
            }

            void Pop(){
                Pq->pop();
            }

            T Top(){
                return Pq->top();
            }

            int Size(){
                return Pq->size();
            }

            bool Empty(){
                return Pq->empty();
            }

            void Claer(){
                Pq->clear();
            }

            // 传入一个指针，将Iter指向的值改为x后，再重新维护堆
            void Modify(pqIter Iter, const T x){
                Pq->modify(Iter, x);
            }

            // 将otherPq并入调用该方法的堆，并清空otherPq
            void Join(priorityQueue& otherPq){                
                Pq->join(*otherPq.Pq);
            }
    };

    /*  旋转树
    */
   template<typename T, typename F = void>
    class multiSet{
        private:
            using PT = std::pair<T, int>;
            using mSTag = __gnu_pbds::rb_tree_tag;
            using mSType = typename std::conditional<std::is_void<F>::value,
                                    __gnu_pbds::tree<PT, __gnu_pbds::null_type, std::function<bool(const PT&, const PT&)>, mSTag, __gnu_pbds::tree_order_statistics_node_update>,
                                    __gnu_pbds::tree<PT, __gnu_pbds::null_type, std::less<std::pair<F, int>>, mSTag, __gnu_pbds::tree_order_statistics_node_update>>::type;
            using mSIter = typename mSType::point_iterator;
            
            // 传入的是否是基本类型，是否重载了调用运算符（F是否为空）
            bool isFundamentalType, wantUseTemplate;
            std::function<bool(const T&, const T&)> userComparator;
            // 时间戳
            int Dfn = 0;
            
            // 对值的映射，方便以后根据值进行查询和更改
            // std::map<T, std::set<pqIter>> val2PointerMp;

            std::unique_ptr<mSType> Ms;

        public:
            multiSet(){
                isFundamentalType = std::is_fundamental<T>::value;
                wantUseTemplate = !std::is_void<F>::value;
                if (false == isFundamentalType && false == wantUseTemplate){
                    throw std::runtime_error("multiSet, input type is not the fundamental type and the comparision function or struct is not provided.");
                    return ;
                }
                else if(true == wantUseTemplate)
                    Ms = std::make_unique<mSType>();
                else
                    Ms = std::make_unique<mSType>();
            }

            multiSet(std::function<bool(const T&, const T&)> f): userComparator(f){
                isFundamentalType = std::is_fundamental<T>::value;
                wantUseTemplate = !std::is_void<F>::value;
                if (false == isFundamentalType && true == wantUseTemplate){
                    throw std::runtime_error("multiSet, both comparision function and struct are existed, delete one of them");
                    return ;
                }
                Ms = std::make_unique<__gnu_pbds::tree<PT, __gnu_pbds::null_type, std::function<bool(const PT&, const PT&)>, mSTag, __gnu_pbds::tree_order_statistics_node_update>>(
                    [f](const PT &x,const PT &y) -> bool {
                        if(f(x.first, y.first) ^ f(y.first, x.first))
                            return f(x.first, y.first);
                        else 
                            return (bool) (x.second < y.second);
                    }
                );
            }

            int Size(){
                return Ms->size();
            }

            bool Empty(){
                return Ms->empty();
            }

            void Clear(){
                Ms->clear();
            }

            void Insert(T x){
                Ms->insert({x, ++Dfn});
            }

            void Erase(T x){
                Ms->erase(Ms->lower_bound({x, 0}));
            }

            int isRank(T x){
                return Ms->order_of_key({x, 0}) + 1;
            }

            T rankIs(int a){
                return Ms->find_by_order(a-1)->first;
            }

            T Prev(T x){
                return Ms->prev(Ms->lower_bound({x, 0}))->first;
            }
            
            // 返回第一个大于等于x的元素
            T lowerBound(T x){
                return Ms->lower_bound({x, 0})->first;
            }

            // 返回第一个大于x的元素
            T upperBound(T x){
                return Ms->upper_bound({x, INF})->first;
            }

            // 确保被合并set的所有元素大于合并set的所有元素，合并后清空
            void Join(multiSet& otherMs){                
                if(userComparator(otherMs.rankIs(1), this->rankIs(this->Size())))
                    throw std::runtime_error("multiSet, Join failed. The necessary condition for merging two sets is that all elements in the merging set must be smaller than all elements in the target set.");
                Ms->join(*otherMs.Ms);
            }

            // 分裂，小于x的元素留住，大于等于x的元素分裂出去
            void Split(T x, multiSet& otherMs){
                otherMs.Clear();
                Ms->split({x, 0}, *otherMs.Ms);
            }
    };

        /*  可变长数组
        */
        template<typename T>
        class Vector{
            private:
                using rpIter = typename __gnu_cxx::rope<T>::iterator;
                __gnu_cxx::rope<T> Rp;
                
            public:
                Vector(){}
                Vector(__gnu_cxx::rope<T> Rp): Rp(Rp) {}

                const T operator[](std::size_t index) const {
                    return Rp[index];
                }

                void pushBack(T x){
                    Rp.push_back(x);
                }
                void popBack(T x){
                    Rp.pop_back();
                }

                // 从 Pos 位置开始插入 x
                void Insert(int Pos, Vector<T> x){
                    Rp.insert(Pos, x);
                }

                // 从 Pos 位置开始删掉长度为 Len 的元素
                void Erase(int Pos, int Len){
                    Rp.erase(Pos, Len);
                }

                // 从 Pos 位置开始提取长度为 Len 的子串，返回一个 Vector
                Vector<T> Substr(int Pos, int Len){
                    return Vector<T>(Rp.substr(Pos, Len));
                }

                // 替换 Pos 处的元素为 x
                void Replace(int Pos, T x){
                    Rp.replace(Pos, x);
                }

                // 从 Pos 位置开始替换为 x
                void Replace(int L, Vector<T> x){
                    Rp.replace(L, x.Size(), x);
                }
                void Clear(){
                    Rp.clear();
                }
                int Size(){
                    return Rp.size();
                }
                bool Empty(){
                    return Rp.empty();
                }

                Vector<T> operator+(const Vector<T>& other) const {
                    return Vector<T>(Rp + other.Rp);
                }
        };

}

#endif