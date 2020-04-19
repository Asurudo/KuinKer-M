# 1025  Linova and Kingdom

**来源**：http://codeforces.com/contest/1337/problem/C

$tag$ : `树状dp` `贪心` 



## 题目大意

给出一个有 $n(2≤n≤2×10^5)$ 个节点的每条边权值都为 $1$  的树，要求恰好选出 $k(1≤k≤n)$ 个节点。其中 $1$ 为根节点，选出的每个节点到根节点的距离减去途中经过的选取点个数即作为贡献累加到答案中，在一定选取规则下，问答案最大值。

## 题目样例

### 样例输入

```
7 4
1 2
1 3
1 4
3 5
3 6
4 7
```

### 样例输出

```
7
```

### 样例解释

![img](https://espresso.codeforces.com/d811810ac8e2c43cb201a3388ae50f1b3e17a687.png)

得到的答案为 $1+2+2+1=7$。若在该选取情况下， $3$ 也被选取，则节点 $5$ 和节点 $6$ 的总贡献由原本的 $2+2=4$ 变为 $1+1=2$ 。

## 题目解法

考虑一下，以 $1$ 为根节点时，对于某个节点，是否可能在该节点的儿子有未选取的情况下选该节点？答案是不可能，因为儿子到根节点的距离更长，而可能遮盖的点却更少。

因此对于每个节点，我们可以总结出他的贡献，这是一个固定值：到根节点的距离-儿子节点的个数，因为如果选取一个点，说明我们肯定把他的所有儿子都选取到了，所以需要减去儿子节点的个数。对于同一支上的节点，父节点的固定贡献一定比儿子更少。

那就变成了我们有这么多固定值，从中选 $k$ 个值使得总值最大，妥妥选前 $k$ 大，贪心即可。

获取到三个值：到根节点距离，儿子节点个数，以及他们两者之差后，对最后一个值从大到小排序取前 $k$ 大，累加即可。

时间复杂度 $O(nlogn)$。

## 完整代码

```c++
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x3f3f3f3f
#define pb push_back

#define maxn 500003
#define maxe 500003 
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
int k;
int childnum[maxn];
int depth[maxn];
vector<int> rnt;
//获取儿子节点总数
int getchild(int x,int fa)
{
	int cdnm = 0;
	for(int i = g.head[x]; i; i = g.Next[i])
	{
		int y = g.ver[i];
		if(y==fa)
			continue;
		cdnm += getchild(y,x)+1;
	}
	return childnum[x] = cdnm;
}
//获取到节点1距离
void getdepth(int x,int fa,int vl)
{
	depth[x] = vl;
	for(int i = g.head[x]; i; i = g.Next[i])
	{
		int y = g.ver[i];
		if(y==fa)
			continue;
		getdepth(y,x,vl+1);
	}
}
//两者之差
void getrntvec(int x,int fa)
{
	rnt.pb(depth[x]-childnum[x]);
	for(int i = g.head[x]; i; i = g.Next[i])
	{
		int y = g.ver[i];
		if(y==fa)
			continue;
		getrntvec(y,x);
	}
}

int main()
{
	scanf("%d%d",&g.n,&k);
	g.m = g.n-1;
	_for(i,0,g.n-1)
	{
		int a, b;
		scanf("%d%d",&a,&b);
		g.add(a,b);g.add(b,a);
	}
	getchild(1,0);
	getdepth(1,0,0);
	getrntvec(1,0);
	sort(rnt.begin(),rnt.end(),greater<int>());
	ll rt = 0;
	_for(i,0,k)
		rt += rnt[i];
	printf("%lld",rt);
	return 0;
}
```

