# 1018 Cow and Fields

来源：http://codeforces.com/contest/1307/problem/D

$tag:$ `单源最短路` `排序` 

## 题目大意

给定一个有 $n(2≤n≤2×10^5)$ 个点，$m(n-1≤m≤2×10^5)$ 条边的无向图，边的权值都为 $1$ 。还有 $k(2≤k≤n)$ 个特殊点。你必须且仅能挑选任意两个特殊点，连一条无向边。现在有一个人要从点 $1$ 走到 点 $n$ 且他一定走最短路。请问你加完边后他最多要走的边的权值总和为多少？



## 题目样例

### 样例输入

```
5 5 3
1 3 5
1 2
2 3
3 4
3 5
2 4
```

### 样例输出

```
3
```

### 样例解释

<img src="https://espresso.codeforces.com/598f8aecf9e44b0bdbcedfa73a9bb5489dc476a1.png" alt="img" style="zoom:67%;" />

可以证明，在 特殊点 $3$ 和特殊点 $5$ 之间加一条边后，最短路为 $3$，这样加边，最短路是最大的。 



## 题目解法

加完边后他走最短路会有 $2$ 种情况：

1. 他不走新加入的边。
2. 他走新加入的边。

他不走新加入的边，说明新加入的边对最短路没有影响，因此这种情况下就是原图的最短路。

他走新加入的边，也就是走到了某一个特殊点，再走新加入的边以后走到另一个特殊点，最后到达点 $n$。

我们对原图跑两次单源最短路，第一次从点 $1$ 开始跑，距离数组我们记作 $d0$ ；第二次从点 $n$ 开始跑，距离数组我们记作 $d1$ 。则上述第一种情况的答案就是 $d0[n]$ ，第二种情况的答案就是 

$$
max\{min\{d0[特殊点1]+1+d1[特殊点2],d0[特殊点2]+1+d1[特殊点1]\}\}
$$
而特殊点 $1,2$ 就是新加入的边的两个端点，因此我们双重循环遍历即可，最后两种情况取最小值即为答案。

时间复杂度 $O(n^2)$ ，过不了。



考虑优化，公式里的 取 $min$ 到底是怎么回事。若：
$$
d0[特殊点1]+1+d1[特殊点2] < d0[特殊点2]+1+d1[特殊点1]
$$
这时候其实那个人铁定先走特殊点 $1$，因为先走 特殊点 $2$ 显然更大。我们将这个式子等价变换一下，得到下式：
$$
d0[特殊点1]-d1[特殊点1] < d0[特殊点2]-d1[特殊点2]
$$
也就是如果上式成立，我们就可以把特殊点 $2$ 排在 特殊点 $1$ 后面，接下来遍历一遍特殊点，对于特殊点 $j(1≤j≤k)$，根据公式，我们只要找到一个特殊点 $i(1≤i＜j)$ 使得 $d0[i]$ 最大，然后带入第二种情况的计算公式即可。

时间复杂度的上限在于排序，$O(nlogn)$



## 完整代码

```c++
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
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

#define maxn 400003
#define maxe 1000003
int k;
struct sp
{
	int num;
	int d0;
	int d1;
	int d;
	bool operator < (sp b)
	{
		return d < b.d;
	}
}sp[maxn];
struct G
{
	int n,m;
	int Next[maxe];
	int head[maxn];
	int ver[maxe];
	int val[maxe];
	int tot;
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
} d0,d1;

int main()
{
	ios::sync_with_stdio(false);
	cin >> g.n >> g.m >> k;
	_for(i,1,k+1)
		cin >> sp[i].num;
	_for(i,0,g.m)
	{
		int x, y;
		cin >> x >> y;
		g.add(x,y,1),g.add(y,x,1);
	}
	
	d0.dij(1,g),d1.dij(g.n,g);
	_for(i,1,k+1)
	{
		sp[i].d0 = d0.d[sp[i].num],
		sp[i].d1 = d1.d[sp[i].num],
		sp[i].d = sp[i].d0 - sp[i].d1;
	}
	sort(sp+1,sp+1+k);
	
	int dmax = -INF;
	int dnum = 0;
	int rnt = 0;
	sp[0].d0 = -INF;
	_for(j,1,k+1)
	{
		rnt = max(rnt,sp[dnum].d0+sp[j].d1+1);
		if(sp[j].d0 > dmax)
			dmax = sp[j].d0,dnum = j;
	}
	rnt = min(rnt,(int)d0.d[g.n]);
	printf("%d",rnt);
	return 0;
}
```

