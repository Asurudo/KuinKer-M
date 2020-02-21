# 1019 1-Trees and Queries

来源：https://codeforces.com/contest/1304/problem/E

$tag:$ `图论` `LCA`

## 题目大意

给出一棵有 $n(3≤n≤10^5)$ 个节点的边权恒为 $1$ 的树。现在给出 $q(1≤q≤10^5)$ 个询问，每个询问包含 $5$ 个数$x,y,a,b,k(1 \le x,y,a,b \le n,x \ne y,1 \le k \le 10^9)$ 代表将在 节点 $x,y$  连一条边权为 $1$ 的无向边，问在 节点 $a,b$ 间是否存在一条长度正好为 $k$ 的路。注意，所有的点和边都可以重复走，但每一条询问结束后，加入的边也会被取消。

## 题目样例

### 样例输入

```
5
1 2
2 3
3 4
4 5
5
1 3 1 2 2
1 4 1 3 2
1 4 1 3 3
4 2 3 3 9
5 2 3 3 9
```

### 样例输出

```
YES
YES
NO
YES
NO
```

### 样例解释

![img](https://espresso.codeforces.com/1c2872e93b3768065463225f0bd43e6bd1f54def.png)

回答 $YES$ 的询问的 $a,b$ 两点间路径如下：

- 第 $1$ 条询问: $1 – 3 – 2$
- 第 $2$ 条询问: $1 – 2 – 3$
- 第 $4$ 条询问: $3 – 4 – 2 – 3 – 4 – 2 – 3 – 4 – 2 – 3$



## 题目解法

首先我们对于路径长度要有一个想法：如果两点之间存在一条 $K$ 长度的路，因为可以重复走边和点，那么通过反复横跳，$K+2i(i≥0)$ 长度的路也将存在。

考虑不加边的情况，此时两点 $a,b$ 之间路径固定，如果加一条边的话，此时 两点 $a,b$ 之间的路径无非就是多出了以下两种情况：

- $a-x-y-b$

- $a-y-x-b$

走其他路都可以看做是这三条路的 “补充”，也就是 $+2i$ 的部分。因此我们得知，如果想要满足题目询问条件，就是在这三条路中要至少找到一条，使得该条路 $K$ 与题目要求 $k$ 的奇偶性相同，且满足 $K≤k$。

询问距离的部分用 $O(nlogn)$ 的 $bfs$ 预处理一下，通过 $LCA$ 计算两点距离即可。

时间复杂度 $O((n+q)logn)$

## 完整代码

```c++
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef double db;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f
#define ZHUO 11100000007
#define MOD 1000000007
#define MIKUNUM 39
#define pb push_back
#define debug() printf("Miku Check OK!\n")
#define X first
#define Y second

#define maxn 500003
#define maxe 1000003
struct G
{
    int n,m;
    int Next[maxe];
    int head[maxn];
    int ver[maxe];
    int tot;
    void add(int x,int y)
    {
        ver[++tot] = y,Next[tot] = head[x],head[x] = tot;
    }
} g;

struct BitIn
{
	int f[maxn][30];
	int d[maxn];
	int bitmaxd;
	void bfsInit(const G &g,int st)
	{
		queue<int> q;
		q.push(st);d[st] = 1;bitmaxd = (int)(log(g.n)/log(2)) + 1;
		while(!q.empty())
		{
			int x = q.front();q.pop();
			for(int i = g.head[x]; i; i = g.Next[i])
			{
				int y = g.ver[i];
				if(d[y])
					continue;
				d[y] = d[x] + 1;
				f[y][0] = x;
				_for(j,1,bitmaxd+1)
					f[y][j] = f[f[y][j-1]][j-1];
				q.push(y);
			}
		}
	}
	int lca(int x,int y)
	{
		if(d[x]<d[y])
			swap(x,y);
		_rep(i,bitmaxd,-1)
			if(d[f[x][i]] >= d[y])
				x = f[x][i];
		if(x==y) return y;
		_rep(i,bitmaxd,-1)
			if(f[x][i] != f[y][i])
				x = f[x][i],y = f[y][i];
		return f[x][0];
	}
}b;

int getdis(int x,int y)
{
	int lca = b.lca(x,y);
	return b.d[x]+b.d[y]-2*b.d[lca];
}

int main()
{
    ios::sync_with_stdio(false);
    cin >> g.n;
    g.m = g.n-1;
    _for(i,0,g.n-1)
    {
		int x, y;
		cin >> x >> y;
		g.add(x,y);g.add(y,x);
	}
	b.bfsInit(g,1);
	
	int q;
	cin >> q;
	while(q--)
	{
		int x, y, a, b, k;
		cin >> x >> y >> a >> b >> k;
		int ans1 = getdis(a,b);
		int ans2 = getdis(a,x)+getdis(b,y)+1;
		int ans3 = getdis(a,y)+getdis(b,x)+1;
		if( !((ans1^k)&0x1) && ans1<=k
		||  !((ans2^k)&0x1) && ans2<=k
		||  !((ans3^k)&0x1) && ans3<=k)
			printf("YES\n");
		else
			printf("NO\n");
	}
    return 0;
}
```

