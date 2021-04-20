# 1010 Cleaning Shifts

来源：https://www.luogu.com.cn/problem/P4644

$tags:$ `权值线段树` `dp`  `排序`

------

## 题目大意

有 $N(1≤N≤10000)$ 头奶牛，现在需要让他们打扫屋子，要求从 $M$ 开始，到 $E(0≤M≤E≤86399)$ 结束。注意，这里的每个数代表一个时间段。现在每头奶牛可以在 $T_1$ 到 $T_2(M≤T_1≤T_2≤E)$ 时间段内工作，所要求的报酬是 $S(0≤S≤500000)$。每个时间段要求至少有一头奶牛在工作，求最小花费。



## 题目样例

### 样例输入

```
3 0 4
0 2 3
3 4 2
0 0 1
```

### 样例输出

```
5
```

### 样例解释

约翰有 $3$ 头牛，牛棚在第 $0$ 秒到第 $4$ 秒之间需要打扫。 约翰雇佣前两头牛清扫牛棚，可以只花 $5$ 美元就完成一整天的清扫。



## 题目解法

设 $dp[i]$ 为覆盖 $[M,i]$ 需要花费的最小代价。将所有奶牛的工作时间按照结束时间递增排序，按顺序扫描奶牛。设当前奶牛的工作时间为 $[T_1,T_2]$ ，价格为 $S$ ，则状态转移方程为：

$dp[T_2] = min(dp[T_2],\underset{T_1-1\leq x\leq T_2}{min}\{dp[x]\}+S)$

初值 $dp[M-1]:=0$，其余都为 $+∞$ 。目标为 $dp[E]$ 。时间复杂度 $O((E-M)^2)$ 。

初值赋 $dp[M-1]$ 为 $0$ 是因为转移是从 $dp[T_1-1]$ 开始搜索，也就是当前奶牛可以解决$[T_1,T_2]$ 这段区间，则 $T_1$ 这个时间段能被负责到，而 $T_1-1$ 无法负责到，而 $dp[i]$ 表示的又是闭区间 $[M,i]$ 的最小花费，因此可以从 $dp[T_1-1]$ 开始转移，因为这代表了 $[M,T_1-1]$ 的最小花费，即 $T_1-1$ 这个时间段已经被承包。至于排序的原因则是这样可以解决转移时的后效性问题，也就是之前的奶牛都不可能更新到 $T_2$ 这个时间段，只有当前的奶牛可以。

我们可以考虑进一步缩减时间复杂度，$dp$ 转移方程中需要查询 $\underset{T_1-1\leq x\leq T_2}{min}dp[x]$ ，这个操作可以用线段树来维护。我们在遍历奶牛的同时是对 $dp[]$ 进行单点更新，而需要区间查询最小值，因此我们使用 **单点更新·区间查询** 的权值线段树维护 $dp[]$ 。

最后要注意到的一点是，因为 开始时间 $M$ 最小从 $0$ 开始，而此时初始化 $dp[M-1]=dp[-1]$ ，越界了，因此我们将所有时间段标记 $+1$ 即可。



## 完整代码

```c++
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<ll,ll> P;
#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x3f3f3f3f
#define MOD 100000000
#define pb push_back
#define debug() printf("Miku Check OK!\n")

#define maxn 100003
int N, st, ed;
ll dp[maxn];
struct SegTree
{
	struct SegNode
	{
		//l,r代表的是该节点表示的原数组下标
		int l,r;
		//sum,max和min分别为原数组对应区间的和，最大值以及最小值
		ll min;
		//x为线段树内部下标
		#define l(x) tree[x].l
		#define r(x) tree[x].r
		#define MIN(x) tree[x].min
	} tree[maxn<<2];
	void build(int p,int l,int r)
	{
		//p为线段树内部下标
		l(p) = l,r(p) = r;
		//到达叶子节点
		if(l==r)
		{
			MIN(p) = dp[l];
			return ;
		}
		int mid = (l+r)/2;
		//递归构造左右节点
		build(p*2, l, mid);
		build(p*2+1, mid+1, r);
		//左右节点维护完后开始维护父节点
		MIN(p) = min(MIN(p*2),MIN(p*2+1));
	}
	void change(int p,int x, ll d)
	{
		//已经到了要修改的叶子节点
		if(l(p) == r(p))
		{
			MIN(p) = d;
			return ;
		}
		int mid = (l(p)+r(p))/2;
		/*如果要修改的端点比当前节点代表区间的mid小则说明要找的点在
		左区间*/
		if(x <= mid)
			change(p*2, x, d);
		//否则在右边
		else
			change(p*2+1, x, d);
		MIN(p) = min(MIN(p*2),MIN(p*2+1));
	}
	ll ask4min(int p,int l,int r)
	{
		//要修改区间完全包住当前节点区间
		if(l <= l(p) && r >= r(p))
			return MIN(p);
		int mid = (l(p)+r(p))/2;
		ll val = INF;
		//分别查询左右节点最大值
		if(l <= mid)
			val = min(val,ask4min(p*2, l, r));
		if(r > mid)
			val = min(val,ask4min(p*2+1, l, r));
		return val;
	}
} SegT;

struct cow
{
	int need;
	int l;
	int r;
	bool operator < (cow b)
	{
		return r < b.r;
	}	
} cow[maxn];

int main()
{
	scanf("%d%d%d",&N,&st,&ed);
	_for(i,1,N+1)
		scanf("%d%d%d",&cow[i].l,&cow[i].r,&cow[i].need);
	sort(cow+1,cow+1+N);
	
	memset(dp,0x3f,sizeof(dp));
	dp[st] = 0;
	SegT.build(1,0,ed+2);
	
	_for(i,1,N+1)
	{
		ll d = SegT.ask4min(1,cow[i].l,cow[i].r);
		dp[cow[i].r+1] = min(dp[cow[i].r+1],d+cow[i].need);
		SegT.change(1,cow[i].r+1,dp[cow[i].r+1]);
	}
	if(dp[ed+1] < INF)
		printf("%lld",dp[ed+1]);
	else
		printf("-1");
	return 0;
} 
```

