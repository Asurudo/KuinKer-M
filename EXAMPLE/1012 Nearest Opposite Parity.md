# 1012 Nearest Opposite Parity

来源：http://codeforces.com/contest/1272/problem/E

$tag:$ `多源最短路` `反向图` `DFS` 

## 题目大意

给定一个长度为 $n(1≤n≤2×10^5)$ 的 正整数数组，下标为 $1-n$，其中元素 $a_i(1≤a_i≤n)$ 。当你在位置 $i(1≤i≤n)$ 的时候，若 $1≤i-a_i$  ，则你可以转移到 $i-a_i$ 的位置，若 $i+a_i≤n$ ，则你可以转移到 $i+a_i$ 的位置。若对于位置 $i,j(1≤i,j≤n)$ ，$a_i$ 与 $a_j$ 的奇偶性不同，且 $i-a_i==j$ 或 $i+a_i==j$ ，则 $a_i$ 可以通过一次转移到达 $a_j$。请你求出每个点最少需要转移几次，才可以到和自己奇偶性不同的点，若不能做到，则该点输出 $-1$ 。



## 题目样例

### 样例输入

```
10
4 5 7 6 7 5 4 4 6 4
```

### 样例输出

```
1 1 1 2 -1 1 1 3 1 1 
```

### 样例解释

位置 $1$ 可以通过一次转移到达位置 $5$ ，位置 $1$ 和位置 $5$ 的奇偶性不同，因此位置 $1$ 转移一次即可。

但是对于位置 $8$ ，需要先转移到位置 $4$ ，再转移到位置 $10$ ，最后转移到位置 $6$ 才能到达和自己奇偶性不同的点，因此需要转移 $3$ 次，输出 $3$ 。

至于位置 $5$ ，哪里都不能到达，因此输出 $-1$ 。



## 题目解法

若位置 $i$ 可以通过一次转移就到达和自己奇偶性不同的点，则对于位置 $i$ 来说，答案必定为 $1$ ，我们将这些点称为 “特权点”，然后可以把转移看作连边，边权为 $1$ ，通过数组构建一个图，所以题目所求即为各个点到最近的特权点的距离。这就是一个多源最短路的模型。要注意的一点是，如果 $i$ 可以到达 $j$ ，则应该连一条 $j->i$ 的反边，比如一个点可以到达特权点，则从特权点向这个点连边，到时候就可以求特权点到这个点的最短距离，即为答案。



## 完整代码

```c++
#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
//typedef pair<ll,ll> P;
#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x3f3f3f3f
#define ZHUO 1000000007
#define pb push_back
#define maxn 200003
#define debug() printf("Miku Check OK!\n")

//最大点数 
#define maxn 200003
//最大边数
#define maxe 1000003 
struct G
{
	//点数和边数
    int n,m;
    //存储某一条边的后一条边 
	int Next[maxe];
	//存储边的起始信息，若有x ->(w) y，则head[x]为第一条到达边的编号 
	int head[maxn];
	//存储边的到达信息 ，即 x ->(w) y中的y 
	int ver[maxe];
	int val[maxe];
	//有向边的总数 
	int tot;
	void add(int x,int y,ll w)
	{
		ver[++tot] = y,Next[tot] = head[x],head[x] = tot,val[tot] = w;
	}
	void clear()
	{
		tot = 0;
		memset(head,0,sizeof(head));
	}
} g;

struct Dij
{
	//first 是最短距离，second 是顶点编号
	typedef pair<int,int> P;
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
        memset(vis,0,sizeof(vis));
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
				int w = g.val[i];
				if(d[y] > d[x] + w)
				{
					d[y] = d[x] + w;
					q.push(P {d[y],y});
                    //路径还原 pre[y] = x
				}
			}
		}
	}
} d1;

int vis[maxn];
int a[maxn];
int n;
void build(int i)
{
	vis[i] = 1;
	//可以转移 
	if(i+a[i]<=n)
	{
		//特权点连边，多源最短路 
		if((a[i]^a[i+a[i]])&0x1)
			g.add(0,i,1);
		//否则连反边等待转移 
		g.add(i+a[i],i,1);
		if(!vis[i+a[i]])
			build(i+a[i]);
	}
	if(i-a[i]>=1)
	{
		if((a[i]^a[i-a[i]])&0x1)
			g.add(0,i,1);
		g.add(i-a[i],i,1);
		if(!vis[i-a[i]])
			build(i-a[i]);
	}
}
int main()
{
	scanf("%d",&n);
	g.n = n;
	_for(i,1,n+1)
		scanf("%d",&a[i]);
	
	_for(i,1,n+1)
		if(!vis[i])
			build(i);

	d1.dij(0,g);
	_for(i,1,n+1)
		if(d1.d[i]==INF)
			printf("-1 ");
		else
			printf("%d ",d1.d[i]);
	return 0;
}
```

