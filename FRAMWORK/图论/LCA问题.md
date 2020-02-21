# LCA问题

## 倍增

## 例题编号

**1002**，**1019**

------

设 $F[x,k]$ 表示 $x$ 的 $2^k$ 辈祖先，即从 $x$ 向根节点走 $2^k$ 步到达的节点。特别地，若 $x$ 节点不存在，则有 $F[x,k] = 0$ 。$F[x,0]$ 就是节点 $x$ 的父节点。除此之外，对于 $\forall k\in [1,logn]$ ，$F[x,k]=F[F[x,k-1],k-1]$。也就是对于节点 $x$ 向上的 $2^{k-1}$ 个节点，其再往上走 $2^{k-1}$ 步就是 $x$ 再往上走 $2^k$ 步。

预处理部分时间复杂度为 $O(nlogn)$，对于给定的两点，每次询问的时间复杂度为$O(logn)$ 。

算法流程如下：

1. 设 $d[x]$ 表示 $x$ 的深度。不妨设 $d[x]≥d[y]$ （否则可交换 $x,y$）。
2. 依次尝试从 $x$ 向上走 $k=2^{logn},...,2^1,2^0$ 步，直到 $x$ 的深度与 $y$ 的深度相等。若到达的节点的深度小于 $y$ 节点的深度，则说明跳到了 $y$ 节点“上面”，则不行，而跳完后深度大于等于 $y$ 节点深度则可以跳。
3. 此时 $x,y$ 等深，若 $x=y$ 则说明 $x,y$ 在同一条链上，即 $LCA(x,y)=y$ 。
4. 若不在同一条链上，则将 $x,y$ 同时向上调整，要保证深度一致且二者不相会。具体来说就是将 $x,y$ 同时向上走 $k=2^{logn},...,2^1,2^0$ 步，每次尝试中若 $F[x,k]≠F[y,k]$ （若相等则说明跳到了祖先上，至于是哪个祖先并不知道）。
5. 此时$x,y$ 的父节点必定相同且就是 $LCA(x,y)$ 。

```c++
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define pb push_back
//最大点数
#define maxn 500003
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
    //存储边的权值信息 ，即 x ->(w) y中的w
    ll val[maxe];
    //有向边的总数 
    int tot;
    void add(int x,int y,ll w)
    {
        ver[++tot] = y,Next[tot] = head[x],head[x] = tot,val[tot] = w;
    }
} g1;

struct BitIn
{
	//f[x][k]表示x向上走2^k 
	int f[maxn][30];
	//深度数组，根节点深度为1 
	int d[maxn];
	//dist[x]表示根节点到节点x的距离 
	ll dist[maxn];
	//树的最大深度为2^bitmaxd 
	int bitmaxd;
	//预处理 
	void bfsInit(const G &g,int st)
	{
		//需要用到的临时队列 
		queue<int> q;
		//根节点深度为1，bitmaxd = log2底n n为节点数量 
		q.push(st);d[st] = 1;bitmaxd = (int)(log(g.n)/log(2)) + 1;
		while(!q.empty())
		{
			int x = q.front();q.pop();
			for(int i = g.head[x]; i; i = g.Next[i])
			{
				int y = g.ver[i];
				//d[y]被更新过说明y是x的爸爸，略过 
				if(d[y])
					continue;
				//深度+1 
				d[y] = d[x] + 1;
				//距离+边权 
				dist[y] = dist[x] + g.val[i];
				//y的爸爸是x 
				f[y][0] = x;
				//更新y的列祖列宗 
				_for(j,1,bitmaxd+1)
					f[y][j] = f[f[y][j-1]][j-1];
				q.push(y);
			}
		}
	}
	int lca(int x,int y)
	{
		//确保 d[x]≥d[y] 
		if(d[x]<d[y])
			swap(x,y);
		//让x的深度与y相同 
		_rep(i,bitmaxd,-1)
			if(d[f[x][i]] >= d[y])
				x = f[x][i];
		//若x==y则x和y在一条链上，说明LCA(x,y)=y 
		if(x==y) return y;
		//否则x和y一起跳到LCA(x,y)的儿子节点 
		_rep(i,bitmaxd,-1)
			if(f[x][i] != f[y][i])
				x = f[x][i],y = f[y][i];
		return f[x][0];
	}
}b1;

int main()
{
	int q ,s;
	scanf("%d%d%d",&g1.n,&q,&s);
	g1.m = g1.n-1;
	_for(i,1,g1.m+1)
	{
		int x,y,z;
		scanf("%d%d%d",&x,&y,&z);
		g1.add(x,y,z);g1.add(y,x,z);
	}
	b1.bfsInit(g1,s);
	while(q--)
	{
		int x, y;
		scanf("%d%d",&x,&y);
		printf("%d\n",b1.lca(x,y));
	}
	return 0;
}
```

