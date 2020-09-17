# 1043  Three Occurrences

**来源**： https://codeforces.com/contest/1418/problem/G

$tag$ : `线段树` 



## 题目大意

给你一个长度为 $n(1≤n≤5×10^5)$ 的数组，我们将包含每个数字 $0$ 次或者 $3$ 次的子数组称为好数组。

询问数组中有多少个子数组是好数组。



## 题目样例

### 样例输入

```
12
1 2 3 4 3 4 2 1 3 4 2 1
```

### 样例输出

```
1
```

### 样例解释

$a[1...12]$ 是好数组，其他子数组都不是好数组。



## 题目解法

首先需要观察到对于每个数字，要么出现 $0$ 次，要么出现 $3$ 次，则包含这个数字的区间才合法。

那么我们感性理解一下：对于每一个区间，区间内的数字对该区间具有“一票否决权”，即若某个数字出现次数不是 $0/3$ 次，则一票否决该区间合法。

于是我们考虑枚举右端点，若能在 $O(logn)$ 时间复杂度内计算出合法的左端点个数，则此题可解。

还需要注意到以下事实：对于某个数字，他只会对一段区间“肯定”，而对其他所有区间否定。对于题目样例

`12` 
`1 2 3 4 3 4 2 1 3 4 2 1`

将 数字 $4$ 出现的下标位置进行记录，有 $pos=\{4,6,10\}$，则当右端点固定为下标 $6$ 的时候，可知数字 $4$ 显然不允许我们在区间 $[1,4]∪(4,6]$ 放置左端点；当右端点固定为下标 $10$ 的时候，有数字 $4$ 允许左端点放置在左端点 $[1,4]$ ，而不允许我们在区间 $(4,6]∪(6,10]$ 放置左端点。

实际上，抛开特殊情况不谈，更普遍的有：当右端点固定为下标 $i$ 时，设 $p_i$ 为 $i$ 在 $pos[a[i]]$ 中的下标，则 $a[i]$ 允许在下标 $(pos[p_i-3],pos[p_i-2]]$ 的区间内放置左端点，其他地方都不行。

如下图所示：

![](https://s1.ax1x.com/2020/09/16/wchbtS.md.jpg)

所以我们可以在区间上建一棵线段树，对于当前固定的右端点 $i$，找到 $a[i]$ 的 $pos$ 数组，用区间更新操作将这个数字不允许的区间 $+1$ ，而允许的区间 $-1$ ，动态更新合法左端点区间。若某个点不为 $0$，则说明有数字使用了“一票否决权”，否则说明所有数字都肯定此处可以放置左端点，因此该点就是一种合法情况，因此查询 $[1,i]$ 的 $0$ 的个数即可。

时间复杂度 $O(nlogn)$。

## 完整代码

```c++
/*Hatsune Miku 4ever!*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f
#define lowbit(x) ((x)&(-x))
#define pb push_back
#define MIKU 39
#define Design ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define debug() printf("Miku Check OK!\n")
#define maxn 500039

int n, m;
ll a[maxn];
struct SegTree
{
	struct SegNode
	{
		int l,r;
		ll add;
		ll cnt,min;
		#define l(x) tree[x].l
		#define r(x) tree[x].r
		#define cnt(x) tree[x].cnt
		#define add(x) tree[x].add
		#define MIN(x) tree[x].min
	}tree[maxn<<2];
	void build(int p,int l,int r)
	{
		add(p) = 0;
		l(p) = l,r(p) = r;
		if(l==r)
		{
			MIN(p) = 0;
			cnt(p) = 1;
			return ;
		}
		int mid = (l+r)/2;
		build(p*2, l, mid);
		build(p*2+1, mid+1, r);
		cnt(p) = cnt(p*2) + cnt(p*2+1); 
		MIN(p) = min(MIN(p*2),MIN(p*2+1));
	}
	void spread(int p)
	{
		if(add(p))
		{
			int mid = (l(p)+r(p))/2;
			MIN(p*2) += add(p);
			MIN(p*2+1) += add(p);
			add(p*2) += add(p);
			add(p*2+1) += add(p);
			add(p) = 0;
		} 
	}
	void change(int p,int l,int r,ll d)
	{
		if(l <= l(p) && r >= r(p))
		{
			MIN(p) += d;
			add(p) += d;
			return ;
		} 
		spread(p);
		int mid = (l(p)+r(p))/2;
		if(l <= mid)
			change(p*2, l, r, d);
		if(r > mid)
			change(p*2+1, l, r, d);
		MIN(p) = min(MIN(p*2),MIN(p*2+1));
		cnt(p) = 0;
		if(MIN(p*2)==MIN(p))
			cnt(p) += cnt(p*2);
		if(MIN(p*2+1)==MIN(p))
			cnt(p) += cnt(p*2+1);
	} 
	ll ask4mincnt(int p,int l,int r)
	{
		if(l <= l(p) && r >= r(p))
		{
			if(!MIN(p))
				return cnt(p);
			else
				return 0;
		}
		spread(p);
		int mid = (l(p)+r(p))/2;
		ll val = 0;
		if(l <= mid)
			val += ask4mincnt(p*2, l, r);
		if(r > mid)
			val += ask4mincnt(p*2+1,l,r);
		return val;
	}
}T;

vector<int> pos[maxn]; 
int ptr[maxn];
int main()
{
	scanf("%d",&n);
	_for(i,1,n+1)
	{
		scanf("%lld",&a[i]);
		pos[a[i]].pb(i);
	}
	T.build(1,1,n);
	ll ans = 0;
	_for(i,1,n+1)
	{
		if(!ptr[a[i]])
			T.change(1,1,pos[a[i]][ptr[a[i]]],1);
		else
			T.change(1,pos[a[i]][ptr[a[i]]-1]+1,pos[a[i]][ptr[a[i]]],1);
		if(ptr[a[i]]>=2)
		{
			if(ptr[a[i]]==2)
				T.change(1,1,pos[a[i]][0],-1);
			else if(ptr[a[i]]==3)
			{
				T.change(1,1,pos[a[i]][0],1);
				T.change(1,pos[a[i]][0]+1,pos[a[i]][1],-1);
			}
			else
			{
				T.change(1,pos[a[i]][ptr[a[i]]-4]+1,pos[a[i]][ptr[a[i]]-3],1);
				T.change(1,pos[a[i]][ptr[a[i]]-3]+1,pos[a[i]][ptr[a[i]]-2],-1);
			}
		}
		ptr[a[i]] ++;
		ans += T.ask4mincnt(1,1,i);
	}
	printf("%lld",ans);
	return 0;
}
```

