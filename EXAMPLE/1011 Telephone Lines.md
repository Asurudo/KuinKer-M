# 1011 Telephone Lines

来源：https://www.luogu.com.cn/problem/P1948

$tag:$ `分层图` `最短路` `二分查找` 

## 题目大意

给定一个有 $n(1≤n≤1000)$ 个点，$m(1≤m≤10000)$条无向边的无向图。边权 $w_i(1≤w_i≤1000000)$ 。求出一条从点 $1$ 到点 $n$ 的路径，使得路径上第 $k+1$ 大的边尽量小。



## 题目样例

### 样例输入

第一行为 $n,m,k$，接下来有 $m$ 行无向边。

```
5 7 1
1 2 5
3 1 4
2 4 8
3 2 3
5 2 9
3 4 7
4 5 6
```

### 样例输出

```
4
```

### 样例解释

对于给定的无向图，可以证明对于路径 $1 -> 3 -> 2 -> 5$ ，经过的边权分别为 $4,3,9$，第 $2$ 大的边的边权为 $4$。



## 题目解法

本题有两种解法，第一种解法为分层图用 $Dijkstra$ 跑从 $1$ 到 $(k+1)*n$ 的最短路，第二种解法为二分枚举 边权排名为第 $k+1$ 条边的最小值，将原图转化为 $0-1$ 边权图，然后用 $Zero-One deque$ 验证值。

### 解法一

原问题可转化为：假设可以使原图的 $k$ 条边距离为 $0$ ，从 $1$  到 $n$ 的最大边的边权是多少。首先建立分层图。先建立 $1$ 个题目中输出的图，然后将其复制 $k$ 份。若原图中有一条 $x<->y$ 的无向边，对于第 $i(1≤i≤k)$ 层图，建立两条边权为 $0$ 的有向边，分别从第 $i$ 层的 $x$ 连到第 $i+1$ 层的 $y$ 和从第 $i$ 层的 $y$ 连到第 $i+1$ 层的 $x$。最后将每一层的 点 $n$ 用边权为 $0$ 的边两两相连。最后从第 $1$ 层的 点 $1$ 到第 $k$ 层的点 $n$ 跑一个 $Dijkstra $ 即可。

### 解法二

二分枚举 第 $k+1$ 条边的最小值，将原图中边权大于该最小值的边的边权都设为 $1$ ，小于等于的设为 $0$ 。然后从点 $1$ 到点 $n$ 跑一个 $Zero-One deque $ 最短路，若 $d[n]>k$ ，说明按照这个枚举的值，要删掉的边还是太多，枚举的值还是太小，边权排名第 $k+1$ 条边的最小值还要更大些才行。



## 完整代码

### 解法一

```c++
#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> P;
#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x3f3f3f3f
#define ZHUO 1000000007
#define pb push_back
#define debug() printf("Miku Check OK!\n")
//最大点数
#define maxn 1003*1003
//最大边数
#define maxe 20003*1003
int n, m, k;
struct G
{
	int n, m;
	//存储某一条边的后一条边
	int Next[maxe];
	//存储边的起始信息，若有x ->(w) y，则head[x]为第一条到达边的编号
	int head[maxn];
	//存储边的到达信息 ，即 x ->(w) y中的y
	int ver[maxe];
	//存储边的权值信息 ，即 x ->(w) y中的w
	int val[maxe];
	//有向边的总数
	int tot;
	void add(int x,int y,int w)
	{
		ver[++tot] = y,Next[tot] = head[x],head[x] = tot,val[tot] = w;
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
				if(d[y] > max(d[x],w))
				{
					d[y] = max(d[x],w);
					q.push(P {d[y],y});
                    //路径还原 pre[y] = x
				}
			}
		}
	}
} d1;

int main()
{
	scanf("%d%d%d",&n,&m,&k);
	g.n = (n+1)*(k+1),g.m = (m+1)*(k+1);
	_for(i,0,m)
	{
		int x, y, w;
		scanf("%d%d%d",&x,&y,&w);
		_for(j,0,k+1)
		{
			g.add(x+n*j,y+n*j,w);
			g.add(y+n*j,x+n*j,w);
			if(j!=k)
			{
				g.add(x+n*j,y+n*(j+1),0);
				g.add(y+n*j,x+n*(j+1),0);
			}
		}
	}
	_for(i,1,k+1)
		g.add(n*i,n*(i+1),0);
	
	d1.dij(1,g);
	if(d1.d[(k+1)*n]==INF)
		printf("%d\n",-1);
	else
		printf("%d\n",d1.d[(k+1)*n]);
	return 0;
}
```

### 解法二

```c++
#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> P;
#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x3f3f3f3f
#define ZHUO 1000000007
#define pb push_back
#define debug() printf("Miku Check OK!\n")
//最大点数
#define maxn 3000
//最大边数
#define maxe 100000

struct G
{
    int n, m;
    //存储某一条边的后一条边
    int Next[maxe];
    //存储边的起始信息，若有x ->(w) y，则head[x]为第一条到达边的编号
    int head[maxn];
    //存储边的到达信息 ，即 x ->(w) y中的y
    int ver[maxe];
    //存储边的权值信息 ，即 x ->(w) y中的w
    int val[maxe];
    //有向边的总数
    int tot;
    void add(int x,int y,int w)
    {
        ver[++tot] = y,Next[tot] = head[x],head[x] = tot,val[tot] = w;
    }
} g;

//ZERO-ONE deque Shortest Path
struct ZOdequeSP 
{
	typedef pair<ll,ll> P; 
	#define psb push_back
    #define psf push_front
    #define ppb pop_back
    #define ppf pop_front
    #define bk back
    #define ft front
    //距离数组
    ll d[maxn];
    //是否访问过 
    int vis[maxn];
    /*双端队列 
    	first 存边的编号，从中可以知道边的除起点外的边权信息
		second 存该边的起点编号 
	*/
	deque<P> dq;
    // s -> other v
    int s;
    void ZOSP(int s,const G &g)
    {
		//距离数组和vis数组初始化 
		_for(i,1,g.n+1)
            d[i] = INF;
        memset(vis,0,sizeof(vis));
        //此处的 {0,s} 中的0没有任何含义 
        d[s] = 0;
        dq.psb({0,s});
        while(!dq.empty())
        {
            P p = dq.ft();dq.ppf();
            //通过该边的到达信息到达其他点 
            int x = g.ver[p.first];
            //若不是起始点 
            if(!p.first)
            	x = s;
            if(vis[x])
                continue;
            //若不是起始点，敲定单源最短距离 
            if(x!=s)
				d[x] = d[p.second] + g.val[p.first];
            vis[x] = 1;
            for(int i = g.head[x]; i; i = g.Next[i])
            {
				int y = g.ver[i];
                int w = g.val[i];
                if(vis[y])
                	continue;
                //权值为1 
                if(w)
					dq.psb({i,x});
                else
                    dq.psf({i,x});
            }
        }
    }
} Z1;

int k;
bool check(int x)
{
	G gt = g;
	_for(i,1,gt.tot+1)
	{
		if(gt.val[i] > x)
			gt.val[i] = 1;
		else
			gt.val[i] = 0;
	}
	Z1.ZOSP(1,gt);
	return Z1.d[gt.n] <= k;
}

ll Bsearch()
{
	int l = 0,r = 1000002;
	while(l < r)
	{
		int mid = (l + r) >> 1;
	//	cout << mid << endl;
		if(check(mid))
			r = mid;
		else
			l = mid+1;
	}
	return l;
} 
int main()
{
    scanf("%d%d%d",&g.n,&g.m,&k);
    _for(i,0,g.m)
    {
		int x, y, w;
		scanf("%d%d%d",&x,&y,&w);
		g.add(x,y,w);
		g.add(y,x,w);
	}
	ll rnt = Bsearch();
	if(rnt>1000000)
		printf("-1\n");
	else
		printf("%lld\n",rnt);
    return 0;
}
```

