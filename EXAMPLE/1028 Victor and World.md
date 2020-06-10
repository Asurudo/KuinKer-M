# 1028  Victor and World

**来源**： http://acm.hdu.edu.cn/showproblem.php?pid=5418

$tag$ : `单源最短路径` `状压dp`



## 题目大意

给出一个 $n$ 个节点， $m$ 条边的联通无向图，问 从 $1$ 出发，游历所有点至少一次后返回，最少需经过的边权总和是多少？



## 题目样例

### 样例输入

```
1
3 2
1 2 2
1 3 3
```

### 样例输出

```
10
```

### 样例解释

按照 $1->2->1->3->1$ 的路线走，在达到题目要求的基础上花费最少，为 $10$。

## 题目解法

若此时所在城市与已访问过的城市敲定后，则花费越少越好，因此有 $dp[i][S]$ 表示当前在第 $i$ 个城市，已访问过的城市集合，用二进制表示为 $S_2$ ，用十进制表示为 $S_{10}$ 。

转移方程为 $dp[ed][S]=min\{dp[st][S去掉ed]+dis[st][ed]\}$ ，其中 $S$ 必须包含 $st$ 和 $ed$，代表转移后到过 $st$ 和 $ed$ ，$dis[i][j]$ 表示从第 $i$ 个城市到第 $j$ 个城市的最小花费。

时间复杂度 $O(n^2×2^n)$  

## 完整代码

```c++
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define pb push_back
#define lowbit(x) ((x)&(-x))
#define ZHUO 10000000007

#define maxn 18
#define maxe 200039
struct G
{
	int n,m;
	int Next[maxe];
	int head[maxn];
	int ver[maxe];
	int val[maxe];
	int tot;
	void clear(int x)
	{
		tot = 0;
		memset(head,0,(x+1)*sizeof(int));
	}
	void add(int x,int y,int w)
	{
		ver[++tot] = y,Next[tot] = head[x],head[x] = tot,val[tot] = w;
	}
} g;

struct Dij
{
	typedef pair<int,int> P;
	ll d[maxn];
	int vis[maxn];
	priority_queue<P,vector<P>,greater<P>> q;
	int s;
    void clear(int x)
	{
		memset(vis,0,(x+1)*sizeof(int));
        while(!q.empty())
        	q.pop();
	}
	void dij(int s,const G &g)
	{
		_for(i,1,g.n+1)
			d[i] = INF;
        memset(vis,0,sizeof(vis));
		d[s] = 0;
		q.push(P {0,s});
		while(!q.empty())
		{
			P p = q.top();
			q.pop();
			int x = p.second;
			if(vis[x]) continue;
			vis[x] = 1;
			for(int i = g.head[x]; i; i = g.Next[i])
			{
				int y = g.ver[i];
				int w = g.val[i];
				if(d[y] > d[x] + w)
				{
					d[y] = d[x] + w;
					q.push(P {d[y],y});
				}
			}
		}
	}
} d;

ll dp[maxn][1<<maxn];

int main()
{
	int t;
	scanf("%d",&t);
	
	while(t--)
	{
		//dis[i][j] 表示 i到j的最小距离
        ll dis[maxn][maxn];
		//初始化 && 建图
		g.clear(16);
		scanf("%d%d",&g.n,&g.m);
		_for(i,1,g.m+1)
		{
			ll x, y, d;
			scanf("%lld%lld%lld",&x,&y,&d);
			g.add(x,y,d);
			g.add(y,x,d);
		}
		//求dis[][]数组
		_for(i,1,g.n+1)
		{
			d.clear(16);
			d.dij(i,g);
			memcpy(dis[i],d.d,sizeof(d.d));
		}
		
		memset(dp,0x3f,sizeof(dp));
        //从第1个城市出发，S==1，证明访问过第1个城市，其他状态都不合法
		dp[0][1] = 0;
		//枚举起点和终点，以及当前状态S，注意下标
		_for(S,0,1<<g.n)
			_for(st,1,g.n+1)
				if(S & (1<<(st-1)))//S必须包含st
					_for(ed,1,g.n+1)
					{
					//st ≠ ed且 S必须包含ed	
                    if(st==ed || !(S & (1<<(ed-1))))
							continue;
						dp[ed-1][S] = min(dp[ed-1][S],dp[st-1][S^(1<<(ed-1))]+dis[st][ed]);
					}
		//从任意一个城市返回第一个城市的最小值即为答案
		ll rnt = INF;
		_for(i,1,g.n+1)
			rnt = min(rnt,dp[i-1][(1<<g.n)-1]+dis[i][1]);
		
		printf("%lld\n",rnt);
	}
	return 0;
}
```

