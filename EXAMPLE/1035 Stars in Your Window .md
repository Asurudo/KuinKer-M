# 1035 **Stars in Your Window**

来源：http://poj.org/problem?id=2482

$tags:$ `权值线段树` `扫描线`  `离散化`

------

## 题目大意

有 $n(1≤n≤10^4)$ 个星星，每个星星都有一个坐标 $x,y$ ，和一个亮度 $l(0≤x,y,l＜2^{31})$ 。你现在有一个长为 $W$ ,宽为 $H(1≤W,H≤10^6)$ 的矩形，现在矩形可以任意摆放，问矩形内的星星的权值之和最大为多少。（矩形边框上的星星不计入矩形内）



## 题目样例

### 样例输入

```
2
3 5 4
1 2 3
2 3 2
6 3 1
3 5 4
1 2 3
2 3 2
5 3 1
```

### 样例输出

```
5
6
```

### 样例解释

对于第一个样例，显然可以将矩形放置在合理位置使得第一个和第二个星星在矩形内，可以证明这样放置有权值之和的最大值 $5$ 。



## 题目解法

见 $LYD$ $P.220$ 以及代码注释。

时间复杂度 $O(nlogn)$ 。



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
#define MIKU 39
#define maxn 20039

ll n, W, H;
struct line
{
	ll x1;
	ll y1;
	ll y2;
	ll d;
	bool operator < (line b)
	{
		//先处理出边
        if(x1!=b.x1)
			return x1 < b.x1;
		return d < b.d;
	}
} LE[maxn];

struct disp
{ 
	int n;
	ll a[maxn];
	int sz; 
	ll acopy[maxn];
	int lsh[maxn]; 
	void init(int kn)
	{
		n = kn;
        memcpy(acopy,a,sizeof(a));
		sort(acopy+1,acopy+1+n);
		sz = unique(acopy+1,acopy+1+n)-(acopy+1);
		_for(i,1,n+1)
			lsh[i] = lower_bound(acopy+1,acopy+1+sz,a[i])-acopy;
	}
	int getsmall(ll x)
	{
		return lower_bound(acopy+1,acopy+1+sz,x)-acopy;
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
	}tree[maxn<<3];
	void build(int p,int l,int r)
	{
		add(p) = 0;
		l(p) = l,r(p) = r;
		if(l==r)
		{
			MAX(p) = 0;
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
	int t;
	scanf("%d",&t);
	while(t--)
	{
		scanf("%lld%lld%lld",&n,&W,&H);
		int j = 1;
		_for(i,0,n)
		{
			ll x, y, l;
			scanf("%lld%lld%lld",&x,&y,&l);
            //整理出一条条扫描线，长度为H-1
			LE[i*2+1] = {x,y,y+H-1,l};
			LE[i*2+2] = {x+W,y,y+H-1,-l};
            //离散化纵坐标
			dsp.a[j++] = y;
			dsp.a[j++] = y+H-1;
		}
		sort(LE+1,LE+1+n*2);
		dsp.init(j-1);
        //对纵坐标建立线段树
		T.build(1,1,dsp.sz);
		
		ll ans = 0;
		_for(i,1,n*2+1)
		{
	        //更改扫描线纵坐标对应的权值
            T.change(1,dsp.getsmall(LE[i].y1),dsp.getsmall(LE[i].y2),LE[i].d);
            //求所有纵坐标上的最大值
			if(i!=2*n)
				ans = max(ans,T.MAX(1));
		}
		printf("%lld\n",ans);
	}
	return 0;
}
```

