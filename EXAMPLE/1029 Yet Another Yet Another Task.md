# 1029  Yet Another Yet Another Task 

来源： https://codeforces.com/contest/1359/problem/D

$tag:$ `线段树` `前缀和` `优先队列` 

## 题目大意

给定一个长度为 $n(1≤n≤100000)$ 的数组 $a$，其中 $a_i∈[-30,30]$ 你需要给定 $[l,r](1≤l≤r≤n)$ ，使得 $\sum_{i=l}^{r}a[i]-max(a[i])(i\epsilon [l,r])$ 最大，只需要给出最大值即可。



## 题目样例  

### 样例输入

```
5
5 -2 10 -1 4
```

### 样例输出

```
6
```

### 样例解释

选择 $l=1,r=5$ ，可以获得最大值 $5+(-2)+10+(-1)+4-10=6$ 。



## 题目解法

考虑到 $a_i$ 并不大，我们可以假定最大值为 $j$，遍历数组，用最大子段和的思想，若当前的数字大于枚举的最大值，则置 $0$ ，否则加到和中，最后减去 $j$ 更新答案即可，时间复杂度 $O(max(a_i)*n)$。

或者用优先队列储存 $a$ ，开三个线段树分别记录 $a$ 的前缀和和对于当前最大值的左右端点，每次优先队列弹出一个当前最大值，在左右端点范围内寻找最大值，更新答案，同时别忘了更新左右限制，时间复杂度 $O(nlogn)$。

核心思想就是确定下最大值以后，转为求范围内最大子段和。



## 完整代码

```c++
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define pb push_back
#define lowbit(x) ((x)&(-x))
#define mod 1000000007
#define maxn 100039

int n;
ll a[maxn];
ll sum[maxn];
int L[maxn];
int R[maxn];
priority_queue<pair<ll,int>> pq;

struct SegTree
{
	struct SegNode
	{ 
		int l,r;
		ll add; 
		ll max,min;
		#define l(x) tree[x].l
		#define r(x) tree[x].r
		#define sum(x) tree[x].sum
		#define add(x) tree[x].add
		#define MAX(x) tree[x].max
		#define MIN(x) tree[x].min
	}tree[maxn<<2];
	void build_sum(int p,int l,int r)
	{
		add(p) = 0;
		l(p) = l,r(p) = r;
		if(l==r)
		{
			MAX(p) = MIN(p) = sum[l];
			return ;
		}
		int mid = (l+r)/2; 
		build_sum(p*2, l, mid);
		build_sum(p*2+1, mid+1, r); 
		MAX(p) = max(MAX(p*2),MAX(p*2+1));
		MIN(p) = min(MIN(p*2),MIN(p*2+1));
	}
	void build_L(int p,int l,int r)
	{
		add(p) = 0; 
		l(p) = l,r(p) = r; 
		if(l==r)
		{
			MAX(p) = MIN(p) = L[l];
			return ;
		}
		int mid = (l+r)/2; 
		build_L(p*2, l, mid);
		build_L(p*2+1, mid+1, r); 
		MAX(p) = max(MAX(p*2),MAX(p*2+1));
		MIN(p) = min(MIN(p*2),MIN(p*2+1));
	}
	void build_R(int p,int l,int r)
	{
		add(p) = 0;
		l(p) = l,r(p) = r;
		if(l==r)
		{
			MAX(p) = MIN(p) = R[l];
			return ;
		}
		int mid = (l+r)/2; 
		build_R(p*2, l, mid);
		build_R(p*2+1, mid+1, r);
		MAX(p) = max(MAX(p*2),MAX(p*2+1));
		MIN(p) = min(MIN(p*2),MIN(p*2+1));
	}
	void spread(int p)
	{
		if(add(p))
		{
			MAX(p*2) += add(p);
			MAX(p*2+1) += add(p);
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
			MAX(p) += d;
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
		MAX(p) = max(MAX(p*2),MAX(p*2+1));
		MIN(p) = min(MIN(p*2),MIN(p*2+1));
	} 
	ll ask4max(int p,int l,int r)
	{
		if(l>r)
			return 0;
		if(l <= l(p) && r >= r(p))
			return MAX(p);
		spread(p);
		int mid = (l(p)+r(p))/2;
		ll val = -INF; 
		if(l <= mid)
			val = max(val,ask4max(p*2, l, r));
		if(r > mid)
			val = max(val,ask4max(p*2+1, l, r));
		return val;
	}
	ll ask4min(int p,int l,int r)
	{
		if(l>r)
			return 0;
		if(l <= l(p) && r >= r(p))
			return MIN(p);
		spread(p);
		int mid = (l(p)+r(p))/2;
		ll val = INF;
		if(l <= mid)
			val = min(val,ask4min(p*2, l, r));
		if(r > mid)
			val = min(val,ask4min(p*2+1, l, r));
		return val;
	}
}Tsum, TL, TR;

int main()
{
	scanf("%d",&n);
	_for(i,2,n+2)
	{
		scanf("%lld",&a[i]);
		pq.push({a[i],i});
		sum[i] = sum[i-1]+a[i];
		L[i] = 1, R[i] = n+2;
	}
	Tsum.build_sum(1,1,n+1);
	TL.build_L(1,1,n+1);
	TR.build_R(1,1,n+2);

	ll rnt = 0;
	while(!pq.empty())
	{
		int pos = pq.top().second;
		pq.pop();
		
		//更新左边和右边的点的左右端点
        int LL = TL.ask4max(1,pos,pos);
		int RR = TR.ask4max(1,pos,pos);
		TR.change(1,LL+1,pos-1,pos-RR);
		TL.change(1,pos+1,RR-1,pos-LL);
        //最大值就是a[pos]，所以求最大子段和就行，对于该值一定最优
		rnt = max(rnt,Tsum.ask4max(1,pos,RR-1)-Tsum.ask4min(1,LL,pos-1)-a[pos]);
		TR.change(1,pos,pos,-1);
		TL.change(1,pos,pos,39393939);
	}
	printf("%lld\n",rnt);
	return 0;
}
```

