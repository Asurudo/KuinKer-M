# 1045  Graph and Queries

**来源**：https://codeforces.com/contest/1417/problem/F

$tag$ : `Kruskal重构树` `线段树` 



## 题目大意

给出 $n(1≤n≤10^5)$ 个命题，每个命题包含两个数字 $x_i,x_j(1≤x_i,x_j≤10^9)$ ，以及一个整数 $e(e∈\{0,1\})$ ：

1. 若整数为 $0$，说明该命题为 $x_i≠x_j$ 
2. 若整数为 $1$ ，说明该命题为 $x_i=x_j$ 

若所有命题没有冲突，则输出 $YES$ ，否则输出 $NO$。

## 题目样例

### 样例输入

```
5 4 6
1 2 5 4 3
1 2
2 3
1 3
4 5
1 1
2 1
2 3
1 1
1 2
1 2
```

### 样例输出

```
5
1
2
0
```

### 样例解释

第一个样例有 $x_1=x_2$ 且 $x_1≠x_2$ 两个命题，显然矛盾，输出 $NO$。

第二个样例有 $x_1=x_2$ 且 $x_2=x_1$ 两个命题，显然可以成立，输出 $YES$。

## 题目解法

我们对删边这个操作的处理方法，其实是有限的，一般就是倒序并查集，所以我们从这个角度去思考。

但是我们发现，变 $0$ 操作很难处理，所以这题只能正序去解决。

虽然我们不能倒序操作了，但是我们正序处理所有询问和更改的时候，能不能使用倒序的数据结构来快速得到一些信息，比如联通情况？答案是可以的，只需要使用 $Kruskal$ 重构树，就能帮助正序的询问和更改在 $O(logn)$ 的时间复杂度内获得联通情况。

$Kruskal$ 重构树的叶子节点是真实存在的节点，但是对于非叶子节点，则是一个记录了时间的控制节点。对于 $Kruskal$ 重构树的某一个叶子节点，在某一时间前和该叶子节点同处一个连通块的叶子节点，必定在某一个记录了时间的控制节点的管理下。

所以倒序建树后，正序操作时将一棵线段树建在该树的叶子节点的 $dfs$ 序上，对于每个询问，通过倍增找到象征着某一时间的祖先节点，就能知道当前和该叶子节点联通的所有叶子节点，在线段树上查询和操作即可。

时间复杂度 $O((n+m+q)logn)$。



## 完整代码

```c++
/*Hatsune Miku 4ever!*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define lowbit(x) ((x)&(-x))
#define pb push_back
#define MIKU 39
#define Design ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define debug() printf("Miku Check OK!\n")
#define bigmiku 3939393939
#define LOG 18

#define maxn 1000039
#define maxe 1000039

int n, m, q;
int tim[maxn], node_tim[maxn];
int rev[maxn], min_dfn[maxn], max_dfn[maxn], anc[maxn][LOG+1];
int egs_in, dfn_in;
struct edge
{
	int x;
	int y;
}e[maxe], egs[maxe];

struct query
{
	int op;
	int x;
}qu[maxn];

struct G
{
	int n,m;
	int Next[maxe];
	int head[maxn];
	int ver[maxe];
	ll val[maxn];
	int tot = 1;
	void add(int x,int y)
	{
		ver[++tot] = y,Next[tot] = head[x],head[x] = tot;
	}
} g;

struct Djs
{
	int par[maxn];
	int ptr;
	void renew()
	{
		ptr = n+1;
		_for(i,1,maxn)
			par[i] = i;
	}
	int find(int x)
	{
		return par[x] == x ? x : par[x] = find(par[x]);
	}
	bool same(int x,int y)
	{
		return find(x) == find(y);
	}
}djs;

struct ANS
{
	int pos;
	ll val;
	bool operator < (ANS b)
	{
		return val < b.val;
	}
	bool operator == (ANS b)
	{
		return val == b.val;
	}
};

struct SegTree
{
	struct SegNode
	{
		int l,r;
		ANS max;
		#define l(x) tree[x].l
		#define r(x) tree[x].r
		#define MAX(x) tree[x].max
	} tree[maxn<<2];
	void build(int p,int l,int r)
	{
		l(p) = l,r(p) = r;
		if(l==r)
		{
			MAX(p) = {l,g.val[rev[l]]};
			return ;
		}
		int mid = (l+r)/2;
		build(p*2, l, mid);
		build(p*2+1, mid+1, r);
		
        if(MAX(p*2+1) < MAX(p*2))
			MAX(p) = MAX(p*2);
		else
			MAX(p) = MAX(p*2+1);
	}
	void change(int p,int pos)
	{
		if(l(p) == r(p))
		{
			MAX(p).val = 0;
			return ;
		}
		int mid = (l(p)+r(p))/2;
		if(pos <= mid)
			change(p*2, pos);
		else
			change(p*2+1, pos);
		
		if(MAX(p*2+1) < MAX(p*2))
			MAX(p) = MAX(p*2);
		else
			MAX(p) = MAX(p*2+1);
	}
	ANS ask4max(int p,int l,int r)
	{
		if(l <= l(p) && r >= r(p))
			return MAX(p);
		int mid = (l(p)+r(p))/2;
		ANS re = {0,-INF};
		if(l <= mid)
		{
			ANS tp = ask4max(p*2, l, r);
			if(re < tp)
				re = tp;
		}
		if(r > mid)
		{
			ANS tp = ask4max(p*2+1, l, r);
			if(re < tp)
				re = tp;
		}
		return re;
	}
} T;

void dfs(int x)
{
	_for(i,1,LOG+1)
		anc[x][i] = anc[anc[x][i-1]][i-1];
	bool is_leaf = 1;
	min_dfn[x] = n*2;
	for(int i = g.head[x]; i; i = g.Next[i])
	{
		int y = g.ver[i];
		is_leaf = 0;
		anc[y][0] = x;
		dfs(y);
        //子树的叶子节点中，最小的时间序和最大的时间序
		min_dfn[x] = min(min_dfn[x],min_dfn[y]);
		max_dfn[x] = max(max_dfn[x],max_dfn[y]);
	}
	if(is_leaf)
	{
		min_dfn[x] = max_dfn[x] = ++dfn_in;
		rev[dfn_in] = x; 
	}
}

//寻找x的祖先的时间控制节点，祖先的节点时间必须小于等于t
int get_anc(int x,int t)
{
	int y = x;
	_rep(i,LOG,-1)
		if(anc[y][i] && node_tim[anc[y][i]] <= t)
			y = anc[y][i];
	return y;
}

int vis[maxn];
int main()
{
	scanf("%d%d%d",&n,&m,&q);
	g.n = n;
	g.m = m;
	_for(i,1,n+1)
		scanf("%d",&g.val[i]);
	_for(i,1,m+1)
	{
		scanf("%d%d",&e[i].x,&e[i].y);
		vis[i] = 1;
	}
	_for(i,1,q+1)
	{
		scanf("%d%d",&qu[i].op,&qu[i].x);
		if(qu[i].op==2)
			vis[qu[i].x] = 0;
	}
	
	_for(i,1,m+1)
		if(vis[i])
			egs[++egs_in] = e[i];
	
	_rep(i,q,0)
	{
		if(qu[i].op==2)
			egs[++egs_in] = e[qu[i].x];
		else
			tim[i] = egs_in;
	}
    //倒序建树，[1,n]是原来的节点，也就是叶子节点，[n+1,?]是时间控制节点
	djs.renew();
	_for(i,1,m+1)
	{
		int u = djs.find(egs[i].x);
		int v = djs.find(egs[i].y);
		if(u != v)
		{
			int par = djs.ptr++;
			djs.par[u] = djs.par[v] = par;
			node_tim[par] = i;
			g.add(par,u);
			g.add(par,v);
		}
	}
	
	_for(i,1,djs.ptr)
		if(djs.find(i)==i)
			dfs(i);
	//在叶子节点上建线段树进行查询
	T.build(1,1,n);
	_for(i,1,q+1)
	{
		if(qu[i].op==2)
			continue;
		int u = qu[i].x;
        //找时间达标的祖先，则子树的子节点就都是联通的
		int v = get_anc(u,tim[i]);
		ANS p = T.ask4max(1,min_dfn[v],max_dfn[v]);
		printf("%lld\n",p.val);
		if(p.val)
			T.change(1,p.pos);
	}
	return 0;
}
```

