# 1003  World of Darkraft: Battle for Azathoth

来源： https://codeforces.com/contest/1321/problem/E

$tag$ : `线段树`  `分块`   `离散化` 

## 题目大意

现在有 $n$ 个武器和 $m$ 个盾牌和 $p$ 个敌人，其中 $1≤n,m,p≤2×10^5$ 。武器有 攻击值 $a_i$ 和价格 $ca_i$ ，盾牌有防御值 $b_i$ 和价格 $cb_i$ ，敌人有 防御值 $y$ ，攻击值 $x$ 和击败后奖励 $z$ ，其中 $1≤a_i,b_i,x,y≤10^6$ ，$1≤z≤10^3$，$1≤ca_i,cb_i≤10^9$ 。你现在能且仅能选一个武器和一个盾牌去打这些敌人，规定当你持有的武器攻击值大于敌人防御值并且持有盾牌防御值大于敌人攻击值时，敌人能够被打败，你能够得到该敌人击败后奖励。你得到的击败奖励总和减去你的武器和盾牌开销为你的最终奖励，请问最终奖励最大为多少。



## 题目样例

### 样例输入

```
2 3 3
2 3
4 7
2 4
3 2
5 11
1 2 4
2 1 6
3 4 6
```

### 样例输出

```
1
```

### 样例解释

你有 $2$ 个武器，$3$ 个盾牌可供选择，可以选择第 $2$ 个武器，第 $2$ 个盾牌，总耗费为 $7+2=9$。

那么根据题意，第 $1,2$ 个敌人可以被你达到，你共得到击败奖励 $4+6=10$。

则你的最终所得为 $1$ 。可以证明这种选取方法可以将所得最大化。



## 题目解法

首先对武器按照攻击值从小到大排序，同时也对敌人按照防御值从小到大排序，这样我们线性遍历武器攻击值的时候，同样可以线性将敌人防御值不如当前武器攻击值的敌人加入到我们的集合里。在这个集合里再考虑敌人攻击值和我们盾牌防御值的问题。

考虑暴力算法，我们可以弄一个和盾牌数组等大的新数组，将对应的每个盾牌位置的初值设为 $-cb_i$ ，也就是该盾牌防御值的相反数，代表该盾牌的贡献。那么我们将盾牌按照防御值从小到大排序以后，就能通过二分查找找到一个盾牌的值，使得下标更大的盾牌都可以获得该敌人的击倒奖励，也就是将该位置以及以后位置的值都加上该敌人的贡献。接下来对新数组所有盾牌对应位置取最大值，代表使用该盾牌时最大所得，记得减去当前遍历到的武器的花费，不断更新结果即可。时间复杂度 $O((n+p)(m+logm))$，必定超时。

考虑优化，和 $1022$ 一样，考虑离散化+线段树，对盾牌按照防御值从小到大排序去重后离散化（防御值相同但是花费少的优先排在前面，$unique()$ 离散化会把后面的覆盖掉）。然后在离散化后的数组上建线段树，用线段树快速加上贡献，时间复杂度 $O((n+p)logm)$。

也可以不考虑离散化+线段树，用分块优雅暴力得更新每次的贡献以及统计最大值，时间复杂度 $O((n+p)(\sqrt{m}+logm))$ 。

## 完整代码（线段树做法）

```c++
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef double db;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define ZHUO 11100000007
#define MOD 1000000007
#define MIKUNUM 39
#define pb push_back
#define debug() printf("Miku Check OK!\n")
#define X first
#define Y second
#define maxn 200039

int n, m, p;
ll rnt = -INF;
struct good
{
	ll val;
	ll w;
	bool operator < (good b)
	{
		if(val!=b.val)
			return val < b.val;
		return w < b.w;
	}
	bool operator == (good b)
	{
		return val == b.val;
	}
}a[maxn], b[maxn];
struct mons
{
	ll x;
	ll y;
	ll w;
	bool operator < (mons b)
	{
		return x < b.x;
	}
}mo[maxn];

struct disp
{
	int sz; 
	good bcopy[maxn];
	ll bvalcopy[maxn];
	void init()
	{
        _for(i,1,m+1)
        {
        	bvalcopy[i] = b[i].val;
        	bcopy[i] = b[i];
        }
		sort(bcopy+1,bcopy+1+m);
		sort(bvalcopy+1,bvalcopy+1+m);
		unique(bvalcopy+1,bvalcopy+1+m);
		sz = unique(bcopy+1,bcopy+1+m)-(bcopy+1);
	}
	ll getbigw(int i)
	{
		return bcopy[i].w;
	}
}dsp;

struct SegTree
{
	struct SegNode
	{
		int l,r;
		ll add;
		ll max;
		#define l(x) tree[x].l
		#define r(x) tree[x].r
		#define add(x) tree[x].add
		#define MAX(x) tree[x].max
	}tree[maxn<<2];
	void build(int p,int l,int r)
	{
		l(p) = l,r(p) = r;
		if(l==r)
		{
			MAX(p) = -dsp.getbigw(r);
			return ;
		}
		int mid = (l+r)/2;
		build(p*2, l, mid);
		build(p*2+1, mid+1, r);
		MAX(p) = max(MAX(p*2),MAX(p*2+1));
	}
	void spread(int p)
	{
		if(add(p))
		{
			MAX(p*2) += add(p);
			MAX(p*2+1) += add(p);
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
	} 
	ll ask4max(int p,int l,int r)
	{
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
}T;

int main()
{
	ios::sync_with_stdio(false);
	cin >> n >> m >> p;
	_for(i,1,n+1)
		cin >> a[i].val >> a[i].w;
	_for(i,1,m+1)
		cin >> b[i].val >> b[i].w;
	_for(i,1,p+1)
		cin >> mo[i].x >> mo[i].y >> mo[i].w;
	
	sort(mo+1,mo+1+p);
	sort(a+1,a+1+n);
	dsp.init();
	T.build(1,1,dsp.sz);
	
	int in = 1;
	_for(i,1,n+1)
	{
		while(in <= p && mo[in].x < a[i].val)
		{
			int x = upper_bound(dsp.bvalcopy+1,dsp.bvalcopy+1+dsp.sz,mo[in].y)-dsp.bvalcopy;
			if(x <= dsp.sz)
				T.change(1,x,dsp.sz,mo[in].w);
			in ++;
		}
		rnt = max(rnt,T.ask4max(1,1,dsp.sz)-a[i].w);
	}
	printf("%lld",rnt);
	return 0;
}
```



## 完整代码（分块做法）

```c++
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef double db;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define ZHUO 11100000007
#define MOD 1000000007
#define MIKUNUM 39
#define pb push_back
#define debug() printf("Miku Check OK!\n")
#define X first
#define Y second
#define maxn 200039

int n, m, p;
ll rnt = -INF;
struct good
{
	ll val;
	ll w;
	bool operator < (good b)
	{
		if(val!=b.val)
			return val < b.val;
		return w < b.w;
	}
}a[maxn], b[maxn];
struct mons
{
	ll x;
	ll y;
	ll w;
	bool operator < (mons b)
	{
		return x < b.x;
	}
}mo[maxn];

int t;
struct piece
{
	ll bw[maxn];
	ll MAX[maxn], add[maxn];
	int L[maxn], R[maxn];
	int pos[maxn];

	void init()
	{
		t = sqrt(m);
		_for(i,1,t+3)
			MAX[i] = -INF;
		_for(i,1,t+1)
		{
			L[i] = (i-1) * sqrt(m) + 1;
			R[i] = i * sqrt(m);
		}

		if(R[t] < m)
		{
			t ++;
			L[t] = R[t-1] + 1;
			R[t] = m;
		}

		_for(i,1,t+1)
		_for(j,L[i],R[i]+1)
			pos[j] = i, MAX[i] = max(MAX[i],bw[j]);
	}
	void change(int l, int r, ll d)
	{
		int p = pos[l], q = pos[r];

		if(p==q)
		{
			_for(i,l,r+1)
				bw[i] += d;
			_for(i,L[p],R[p]+1)
				MAX[p] = max(MAX[p],bw[i]);
		}
		else
		{
			_for(i,p+1,q)
				add[i] += d;

			_for(i,l,R[p]+1)
				bw[i] += d;
			_for(i,L[p],R[p]+1)
				MAX[p] = max(MAX[p],bw[i]);

			_for(i,L[q],r+1)
				bw[i] += d;
			_for(i,L[q],R[q]+1)
				MAX[q] = max(MAX[q],bw[i]);
		}
	}

	ll ask(int l,int r)
	{
		int p = pos[l], q = pos[r];
		ll ans = -INF;

		if(p==q)
		{
			_for(i,l,r+1)
				ans = max(ans,bw[i]);
			ans += add[p];
		}
		else
		{
			_for(i,p+1,q)
				ans = max(ans,MAX[i]+add[i]);
			_for(i,l,R[p]+1)
				ans = max(ans,bw[i]);
			ans += add[p];
			
			_for(i,L[q],r+1)
				ans = max(ans,bw[i]);
			ans += add[q];
		}
		return ans;
	}
} P;

ll bval[maxn];
int main()
{
	ios::sync_with_stdio(false);
	cin >> n >> m >> p;
	_for(i,1,n+1)
		cin >> a[i].val >> a[i].w;
	_for(i,1,m+1)
		cin >> b[i].val >> b[i].w;
	_for(i,1,p+1)
		cin >> mo[i].x >> mo[i].y >> mo[i].w;
	
	sort(a+1,a+1+n);
	sort(b+1,b+1+m);
	sort(mo+1,mo+1+p);
	
	_for(i,1,m+1)
		bval[i] = b[i].val,P.bw[i] = -b[i].w;
	P.init();
	
	int in = 1;
	ll rnt = -INF;
	_for(i,1,n+1)
	{
		while(in <= p && mo[in].x < a[i].val)
		{
			int x = upper_bound(bval+1,bval+1+m,mo[in].y)-bval;
			if(x<=m)
				P.change(x, m, mo[in].w);
			in ++;
		}
		rnt = max(rnt,P.ask(1,m)-a[i].w);
	}
	printf("%lld",rnt);
	return 0;
}
```

