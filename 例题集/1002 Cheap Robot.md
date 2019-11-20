#  1002 Cheap Robot 

**来源**： https://codeforces.com/contest/1253/problem/F 

$tag$ ：`最短路`  `图论`  `倍增`  `BFS`  `最小生成树`  `树状dp`



## 题目大意

给定一个有 $n(2≤n≤100000)$ 个点和 $m(1≤m≤300000)$ 条边的**连通带权无向图** 。

有 $k(2≤k≤n)$ 个充电点，你现在有一个最大电量为 $c(1≤c≤10^9)$ 的机器人，这个机器人在图中穿梭，每经过一条 $(u,v,w)$ 的边后，当前电量都会减去 $w$ 。机器人的电量 $x$ 需要始终满足 $0≤x≤c$ ，到了充电点的时候可以一次性充满电。

题目将给出 $q(1≤q≤300000)$ 个独立的任务，每个任务包含两个**充电点** $a_{i},b_{i}$ 分别作为起点和终点，对于每个任务你都需要回答最少的 $c$ 是多少，能够使得机器人成功从起点到终点。

## 题目样例

### 样例输入

```
9 11 3 2
1 3 99
1 4 5
4 5 3
5 6 3
6 4 11
6 7 21
7 2 6
7 8 4
8 9 3
9 2 57
9 3 2
3 1
2 3
```

### 样例输出

```
38
15
```

### 样例解释

题目给出的图如下（红色的点为充电点）：

 ![img](https://codeforces.com/predownloaded/e8/bc/e8bcaba09c4d0c97518355f878ea2092668e7cdb.png) 

对于第一个任务，机器人可以走 $3→9→8→7→2→7→6→5→4→13→9→8→7→2→7→6→5→4→1$

对于第二个任务，机器人可以走

$2→7→8→9→3$ 



## 题目解法：

需要观察到两点：

1. 因为我们总是在充电点结束，因此任何时刻机器人都要能够到达最近的充电点。
2. 因为我们总是在充电点开始，所以不管在哪一点，都可以去到最近的充电点后再回来，这样电量不会减少。

我们首先使用 $ Dijkstra $ 算法计算任意一点 $u$ 到最近的充电点的距离 $d_{u}$。可以设一个虚拟点 $s$ ，该虚拟点向所有充电点连接边权为 $0$ 的边，以 $s$ 点为起点跑单源最短路，则 $s$ 点到 $u$ 点的最短距离即所求 $d_{u}$ 。

假设机器人最大电量为 $c$ ，此时在 $u$ 点且剩余电量为 $x$ ，注意到 $x≤c-d_{u}$ 。为什么呢？因为机器人总可以原路返回上一个充电点，所以 $x$ 不会大于 $c-d_{u}$ 。但其实也不会小于，因为机器人可以到最接近的距离为 $d_{u}$的充电站充电后再回来，因此 $x$ 恒等于 $c-d_{u}$ 。

因此我们可以用二分查找可能的 $c$ 值后进行简单的 $dfs$ 看看能否从起点到终点，时间复杂度 $O(mlogm+qmlogn)$ ，很明显需要优化。

要保证机器人始终能够到达最近的充电点且需要经过边 $(u,v,w)$ 的充分必要条件是当前电量减去边权消耗电量需要大于等于边终点到最近充电点的距离，写成数学公式也就是 $x-w≥d_{v}$（ $x$ 为当前电量） ，即$(c-d_{u})-w≥d_{v}$ ，进行移项原式等价为 $d_{u}+d_{v}+w≤c$ 。

因此我们要求的 $c$ 只要一路满足 上述不等式即可。我们可以将原图换边，将原图每条边的边权 $w$ 用 $w'=w+d_{u}+d_{v}$ 替换，则找出一条最大的任意两点间距离值最小的路就成了我们的目标，什么意思？即我们将一条路中两点距离的最大值作为这条路的权，要求权最小的路。而求出的该权即为所求的 $c$ 。

$Kruskal$ 算法生成最小生成树每次都尝试用最小的边将一个点纳入自己的点集合中，因此要求一个点到另一个点每段路程的最大值的最小值，则该问题可转化为求在该图的最小生成树上两点路径的最大值（树上两点路径确定）。我们用换完边的图求一下最小生成树，则将问题进一步转化为最小生成树上两点路径最大值问题。

两点之间的路径必然经过两点最近公共祖先，则该问题可分解为求两条链（也可能是两点在一条链上）上的最大值问题。我们考虑用倍增算法跳到其最近公共祖先，在这之中我们要求出该链上的最大值。怎么求？我们设数组 $dist[x][k]$表示节点 $x$ 顺着所在链向上跳 $2^k$ 次，经过的边权的最大值。如果能求出这个数组，则在向上跳找最近公共祖先的时候，每跳一次，就能得到从 $x$ 到 其上 $2^k$ 位置之间最大的边权值，转移方程为 $ans = max(ans,dist[x][i])$ $(x往上2^i后合法)$ 因为最后一定跳到了最近公共祖先的下方一格位置，所以这条链上的最大的边权值也就不难得到，综合一下两条链的最大边权值和最近公共祖先到其下一格的两条边的边权值，取一个最大值，就是答案，即 $max(ans,max(dist[x][0],dist[y][0]))$ 。

怎么求 $dist$ 数组？我们可以在倍增算法预处理的时候把它求出来。因为预处理时候是用 $bfs$ 逐层处理，所以深度越小的节点一定先处理完，那么我们就有 $dist[x][i] = max(dist[x][i-1],dist[f[x][i-1]][i-1])$ 。怎么来的？因为是在一条链上求解，我们可以将其视为一个线性问题。则有

$[x,x+2^i]区间边权最大值=max([x,x+2^{i-1}]区间边权最大值,[x+2^{i-1},x+2^i]区间边权最大值)$

正好对应上面的转移方程。  

时间复杂度为$O(mlogm+qlogn)$。



## 完整代码

```c++
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define pb push_back
//最大点数
#define maxn 110003
//最大边数
#define maxe 1000003
int k, q;
struct G
{
	int n,m;
	//存储某一条边的后一条边
	int Next[maxe];
	//存储边的起始信息，若有x ->(w) y，则head[x]为第一条到达边的编号
	int head[maxn];
	//存储边的到达信息 ，即 x ->(w) y中的y
	int ver[maxe];
	//存储边的权值信息 ，即 x ->(w) y中的w
	ll val[maxe];
	//有向边的总数
	int tot;
	void add(int x,int y,ll w)
	{
		ver[++tot] = y,Next[tot] = head[x],head[x] = tot,val[tot] = w;
	}
} g,MSTg;

struct Djs
{
	int par[maxn];
	int high[maxn];
	Djs()
	{
		_for(i,1,maxn)
		{
			par[i] = i;
			high[i] = 0;
		}
	}
	int find(int x)
	{
		//路径压缩
		return par[x] == x ? x : par[x] = find(par[x]);
	}
	void unite(int x,int y)
	{
		x = find(x);
		y = find(y);
		if(x==y) return ;
		//按秩合并
		if(high[x]<high[y])
			par[x] = y;
		else
		{
			par[y] = x;
			if(high[x]==high[y])
				high[x] ++;
		}
	}
	bool same(int x,int y)
	{
		return find(x) == find(y);
	}
} MSTd1;

struct Kruskal
{
	struct edge
	{
		int u;
		int v;
		ll w;
		bool operator < (edge b)
		{
			return w < b.w;
		}
	};
	//需要用到的边数组
	vector<edge> e;
	//最小生成树权值和
	ll MSTval;
	void kruskal(const G &g)
	{
		//将所有边加入临时数组并按照边权从小到大排序
		_for(x,1,g.n+1)
		for(int i = g.head[x]; i; i = g.Next[i])
			e.pb({x,g.ver[i],g.val[i]});
		sort(e.begin(),e.end());
		//遍历所有边，若连边的两点未连通，则将该边加入最小生成树
		_for(i,0,e.size())
		if(!MSTd1.same(e[i].u,e[i].v))
		{
			//消除虚拟节点s对结果的影响 
			if(!e[i].u || !e[i].v)
				continue;
			//构造最小生成树 
			MSTg.add(e[i].u,e[i].v,e[i].w);
			MSTg.add(e[i].v,e[i].u,e[i].w);
			MSTd1.unite(e[i].u,e[i].v);
			MSTval += e[i].w;
		} 
		MSTg.n = g.n;
		MSTg.m = g.n-1;
	}
} k1;

struct Dij
{
	//first 是最短距离，second 是顶点编号
	typedef pair<ll,int> P;
	//距离数组
	ll d[maxn];
	//是否使用过？
	int vis[maxn];
	priority_queue<P,vector<P>,greater<P>> q;
	// s -> other v
	int s;
	void dij(int s,const G &g)
	{
		//d初始化
		_for(i,1,g.n+1)
			d[i] = INF;
		d[s] = 0;
		//起点入队
		q.push(P {0,s});
		while(!q.empty())
		{
			//获取d最小的点
			P p = q.top();
			q.pop();
			int x = p.second;
			if(vis[x]) continue;
			vis[x] = 1;
			//使用x更新到其他点距离
			for(int i = g.head[x]; i; i = g.Next[i])
			{
				int y = g.ver[i];
				ll w = g.val[i];
				if(d[y] > d[x] + w)
				{
					d[y] = d[x] + w;
					q.push(P {d[y],y});
				}
			}
		}
	}
} d1;
struct BitIn
{
	//f[x][k]表示x向上走2^k
	int f[maxn][30];
	//深度数组，根节点深度为1
	int d[maxn];
	//dist[x]表示根节点到节点x的距离
	ll dist[maxn][30];
	//树的最大深度为2^bitmaxd
	int bitmaxd;
	//预处理
	void bfsInit(const G &g,int st)
	{
		//需要用到的临时队列
		memset(dist,0,sizeof(dist));
		queue<int> q;
		//根节点深度为1，bitmaxd = log2底n n为节点数量
		q.push(st);
		d[st] = 1;
		bitmaxd = (int)(log(g.n)/log(2)) + 1;
		while(!q.empty())
		{
			int x = q.front();
			q.pop();
			for(int i = g.head[x]; i; i = g.Next[i])
			{
				int y = g.ver[i];
				//d[y]被更新过说明y是x的爸爸，略过
				if(d[y])
					continue;
				//深度+1
				d[y] = d[x] + 1;
				//y的爸爸是x
				f[y][0] = x;
				//dist[x][k]表示从该节点想要到达距离自己2^k的祖先，需要经过的最大边 
				dist[y][0] = g.val[i];
				//更新y的列祖列宗和dist 
				_for(j,1,bitmaxd+1)
				{
					f[y][j] = f[f[y][j-1]][j-1];
					// dp[y,y+2^j] = max(dp[y,y+2^(j-1)],dp[y+2^(j-1),y+2^j]) 
					dist[y][j] = max(dist[y][j-1],dist[f[y][j-1]][j-1]);
				}
				q.push(y);
			}
		}
	}
	ll query(int x,int y)
	{
		ll ans = 0;
		//确保 d[x]≥d[y]
		if(d[x]<d[y])
			swap(x,y);
		//让x的深度与y相同,同时更新ans
		_rep(i,bitmaxd,-1)
		if(d[f[x][i]] >= d[y])
			ans = max(ans,dist[x][i]),x = f[x][i];
		//若x==y则x和y在一条链上，说明LCA(x,y)=y
		if(x==y) return ans;
		//否则x和y一起跳到LCA(x,y)的儿子节点
		_rep(i,bitmaxd,-1)
		if(f[x][i] != f[y][i])
		{
			ans = max(ans,dist[x][i]);
			ans = max(ans,dist[y][i]);
			x = f[x][i],y = f[y][i];
		}
		
		return max(ans,max(dist[x][0],dist[y][0]));
	}
} b1;
int main()
{
	scanf("%d%d%d%d",&g.n,&g.m,&k,&q);
	_for(i,0,g.m)
	{
		int x,y;
		ll z;
		scanf("%d%d%lld",&x,&y,&z);
		g.add(x,y,z);
		g.add(y,x,z);
	}
	_for(i,1,k+1)
	g.add(0,i,0),g.add(i,0,0);

	d1.dij(0,g);
    //更新权值
	_for(x,1,g.n+1)
	for(int i = g.head[x]; i; i = g.Next[i])
		g.val[i] += d1.d[x]+d1.d[g.ver[i]];

	k1.kruskal(g);
	b1.bfsInit(MSTg,1);
	while(q--)
	{
		int x,y;
		scanf("%d%d",&x,&y);
		printf("%lld\n",b1.query(x,y));
	}
	return 0;
}
```

