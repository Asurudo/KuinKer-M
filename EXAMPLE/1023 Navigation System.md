# 1023 Navigation System

来源：https://codeforces.com/contest/1321/problem/D

$tag:$ `单源最短路` `反图`

## 题目大意

给定一个有 $n(1≤n≤2×10^5)$ 个点，$m(1≤m≤2×10^5)$ 条边的边权固定为 $1$ 的有向图和一条固定的路。路的起点和终点分别为 $s,t(1≤s,t≤n)$ 。$Jyo$ 按照该固定路线从 $s$ 到 $t$ ，每到一个点系统就会给他推荐一条最短路程。若 $Jyo$ 不按照推荐路程走，则 $Jyo$ 到达新地点时，导航系统将会重新推荐一条最短路。若有多条最短路，则会随机选择一条进行推荐。询问最少重新推荐次数和最大重新推荐次数。



## 题目样例

### 样例输入

```
6 9
1 5
5 4
1 2
2 3
3 4
4 1
2 6
6 4
4 2
4
1 2 3 4
```

### 样例输出

```
1 2
```

### 样例解释

样例图示如下：

![](https://tk.codeforces.com/a.png)



从 $1$ 开始，有且仅有一条最短路 $1-5-4$ ，所以系统肯定推荐这条路，而 $Jyo$ 走到了 $2$ ，系统自然必定要重新推荐一条路。

从 $2$ 开始，有两条最短路 $2-3-4$ 和  $2-6-4$ ，系统可能推荐 $2-3-4$ ，这样在下一步就不需要重新推荐，但如果选了 $2-6-4$ ，那到 $3$ 后就要重新推荐。

从 $3$ 开始，有且仅有一条最短路且 $Jyo$ 必走该条路，因此绝对不会重新推荐。

## 题目解法

观察题目信息，我们可以得知每到一个新点，有三种情况：

- 系统必定重新规划，最小最大次数都增加
- 系统可能重新规划，最大次数增加
- 系统必定不会重新规划，不做变动

考虑三种情况对应当前点信息，因为边权固定为 $1$ 因此每走一步若与终点距离减少，则说明走上了一条最短路，若距离没有减少甚至还增多了，说明没有走上一条最短路。因此上述三种情况对应如下：

- 与终点距离没有减少
- 与终点距离减少，但上个点到终点有多条最短路规划
- 与终点距离减少，上个点到终点的最短路有且仅有一条

建一张反图，从终点 $t$ 跑一次 $0-1$ 最短路 即可得到每个点到终点的最短路的最短距离，也就是与终点距离。

判断上个点到终点的最短路条数，可以遍历上个点的临点的与终点距离，值最小的一定是最短路，有多个这种最小值即有多条最短路，否则有且仅有一条最短路。

接下来根据上面三种情况，遍历固定路径中的每个点分类讨论即可。

时间复杂度 $O(n+m)$ ，因为还要遍历临点所以实际运行时间会较多。

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

#define maxn 200003
#define maxe 500003 
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
} g,rg;
struct ZOdequeSP 
{
	typedef pair<ll,ll> P; 
	#define psb push_back
    #define psf push_front
    #define ppb pop_back
    #define ppf pop_front
    #define bk back
    #define ft front
    ll d[maxn];
    int vis[maxn];
	deque<P> dq;
    int s;
    void ZOSP(int s,const G &g)
    {
		_for(i,1,g.n+1)
            d[i] = INF;
        memset(vis,0,sizeof(vis));
        d[s] = 0;
        dq.psb({0,s});
        while(!dq.empty())
        {
            P p = dq.ft();dq.ppf();
            int x = g.ver[p.first];
            if(!p.first)
            	x = s;
            if(vis[x])
                continue;
            if(x!=s)
				d[x] = d[p.second] + g.val[p.first];
            vis[x] = 1;
            for(int i = g.head[x]; i; i = g.Next[i])
            {
				int y = g.ver[i];
                int w = g.val[i];
                if(vis[y])
                	continue;
                if(w)
					dq.psb({i,x});
                else
                    dq.psf({i,x});
            }
        }
    }
} Z;
int k;
int path[maxn];

int main()
{
	ios::sync_with_stdio(false);
	cin >> g.n >> g.m;
	rg.n = g.n,rg.m = g.m;
	_for(i,0,g.m)
	{
		int x, y;
		cin >> x >> y;
		g.add(x,y,1);rg.add(y,x,1);
	}
	cin >> k;
	_for(i,1,k+1)
		cin >> path[i];
	Z.ZOSP(path[k],rg);
	
	int rnt1 = 0,rnt2 = 0;
	_for(j,2,k)
	{
		int minn = INT_MAX;
		int times = 0;
        //看看最短路条数
		for(int i = g.head[path[j-1]]; i; i = g.Next[i])
		{
			int y = g.ver[i];
			if(Z.d[y]<minn)
				minn = Z.d[y],times = 1;
			else if(Z.d[y]==minn)
				times ++;
		}
        //误入歧途，必定重新推荐
		if(Z.d[path[j]]>=Z.d[path[j-1]])
			rnt1 ++,rnt2 ++;
        //的确朝着一条最短路走，但系统可能推荐的不是这条
		else if(Z.d[path[j]]<Z.d[path[j-1]] && times>=2)
			rnt2 ++;
	}
	printf("%d %d",rnt1,rnt2);
	return 0;
}
```

