# 1005  Arson In Berland Forest 

来源： https://codeforces.com/contest/1313/problem/C2

$tag:$ `线段树` `分治` 

## 题目大意

给定一个长度为 $n(1≤n≤500000)$ 的数组 $m$，对于任意的 $i(1≤i≤n)$ ，你可以令 $a_i≤m_i$ 且对于 $j＜i＜k$ ，一定不能有 $a_j＞a_i＜a_k$，也就是说 $a_i$ 要么是左侧要么是右侧，至少有一侧的所有数字都不能大于 $a_i$。请构造出这样一个数组 $a$ ，使得 $a$ 数组所有元素之和在满足条件情况下最大。



## 题目样例  

### 样例输入

```
3
10 6 8
```

### 样例输出

```
10 6 6 
```

### 样例解释

很显然输出满足题目的条件且总和为 $22$，实际上 $[6,6,8]$ 也满足题目条件，但总和为 $20<22$，因此不是题目要的最优 $a$ 数组。



## 题目解法

既然不能让一个值被“夹在中间”，那首先应该考虑该数组中上限最低的地方 $i$，让 $a[i]=m[i]$，因为这个位置取上限值必定不会影响到左右侧的值，不如说即使他取上限值，左侧或者右侧还是得有至少一侧不能比这个所谓的上限值大。

那么我们就可以先找出最小位置和其最小值，然后下面的抉择无非就是两个：

- 给左侧“剃头”，放纵右侧
- 给右侧“剃头”，放纵左侧

那到底选哪个，要试试看才知道。经过这样一分，我们可以知道，剃头的一方最后获得的总值就是 一侧长度*该最小值没跑了，但是另一侧我们并不知道他最优情况如何抉择才能得到，并且也不知道是多少。那么这其实就是一个子问题，我们调用原来的解决函数去运算即可。

所以我们编写的解决函数既要完成赋值任务，还要完成计算区间内最优总和的任务。线段树改一改就能实现在 $O(nlogn)$ 的预处理时间复杂度下，以 $O(logn)$ 的时间复杂度完成每次区间最小值和其位置询问，需要注意的是最小值可能不止只有一个，选最靠边的即可。加上分治以后时间复杂度还是 $O(nlogn)$ 。

## 完整代码

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
#define maxn 500039

int n;
ll m[maxn];
ll mxht = 0;
ll ans[maxn];

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
		ANS min;
		#define l(x) tree[x].l
		#define r(x) tree[x].r
		#define MIN(x) tree[x].min
	} tree[maxn<<2];
	void build(int p,int l,int r)
	{
		l(p) = l,r(p) = r;
		if(l==r)
		{
			MIN(p) = {l,m[l]};
			return ;
		}
		int mid = (l+r)/2;
		build(p*2, l, mid);
		build(p*2+1, mid+1, r);
		
        //若最小的数不止一个，要最靠近边上的一个，一定更优，因为可以使得得以冲击上限的数字更多
        if(MIN(p*2) < MIN(p*2+1)
		|| MIN(p*2) == MIN(p*2+1)
		&& min(MIN(p*2).pos-l(p),r(p)-MIN(p*2).pos)
		<= min(MIN(p*2+1).pos-l(p),r(p)-MIN(p*2+1).pos))
			MIN(p) = MIN(p*2);
		else
			MIN(p) = MIN(p*2+1);
	}
	ANS ask4min(int p,int l,int r)
	{
		if(l <= l(p) && r >= r(p))
			return MIN(p);
		int mid = (l(p)+r(p))/2;
		ANS re = {0,INF};
		if(l <= mid)
		{
			ANS tp = ask4min(p*2, l, r);
			if(tp < re)
				re = tp;
		}
		if(r > mid)
		{
			ANS tp = ask4min(p*2+1, l, r);
			if(tp < re)
				re = tp;
		}
		return re;
	}
} T;

//[l,r]最小值为x.val，在x.pos位置，此刻在计算[l,r]的最优情况
ll div(ANS x,int l,int r)
{
	if(l>r)
		return 0;
	ans[x.pos] = x.val;
	
    //分治，看看放纵左侧更好还是放纵右侧更好
	ll sum1 = (x.pos-l)*x.val + div(T.ask4min(1,x.pos+1,r),x.pos+1,r);
	ll sum2 = div(T.ask4min(1,l,x.pos-1),l,x.pos-1) + (r-x.pos)*x.val;
	
    //返回的是[l,r]能带来的最大总和
	if(sum1>sum2)
	{
		_for(i,l,x.pos)
			ans[i] = x.val;
		return sum1+x.val;
	}
	else
	{
		_for(i,x.pos+1,r+1)
			ans[i] = x.val;
		return sum2+x.val;
	}
}

int main()
{
	ios::sync_with_stdio(false);
	cin >> n;
	_for(i,1,n+1)
		cin >> m[i];
	T.build(1,1,n);
	
	div(T.ask4min(1,1,n),1,n);
	_for(i,1,n+1)
		printf("%d ",ans[i]);
	return 0;
}
```

