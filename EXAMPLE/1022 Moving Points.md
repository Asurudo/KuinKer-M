# 1022  Moving Points

**来源**： https://codeforces.com/contest/1311/problem/F

$tag$ : `树状数组` `离散化` 



## 题目大意

给出长度为 $n(1≤n≤2×10^5)$ 的点数组，每个点都有两个属性 $x,v(1≤x≤10^8，-10^8≤v≤10^8)$ 。

现在考虑两点 $(i,j)$ ，若两点满足其中一点 $i$ 的两个属性均小于等于 $j$ 的两个属性，则令 $d(i,j)$ 为他们属性 $x$ 的差的绝对值。

顺带一提，每个点的属性 $x$ 值不会和另一点的属性 $x$ 值相同。 

求 $\sum\limits_{1 \le i < j \le n} d(i,j)$ 。 



## 题目样例

### 样例输入

```
3
1 3 2
-100 2 3
```

### 样例输出

```
3
```

### 样例解释

考虑属性 $x$ 为 $1$ 的点，该点的 属性 $v$ 劣于 属性 $x$ 为 $2,3$ 的点，故答案加 $|2-1|+|3-1|=3$。



## 题目解法

先把点按照 属性 $x$ 从小到大排序。然后我们从前往后遍历可以保证当前遍历到的点的属性 $x$ 一定最优，因此就从之前遍历过的点中选出 属性 $v$ 不如该点的点，计算一下 属性 $x$ 的差值即可。

暴力无疑整个算法时间复杂度是 $O(n^2)$ ，考虑快速选出 属性 $v$ 较低的已遍历过的点。我们可以每遍历一个点就将其加入一个集合，这样以后只需要在集合中考虑即可，也就是动态更新。

我们可以考虑首先离散化 点数组的属性 $v$ ，这样对于遍历到的每一个点我们就能在对应位置上对该集合进行修改。

实际上如果我们能一次性拿到两个属性都劣于当前点的点集合的属性 $x$ 的总和，当前点对答案做出的贡献就是 点集合中点个数*当前点位置-点集合的点的属性 $x$ 的总和。

既然我们能在某点 属性 $v$ 对应位置上作出改变了，那对于当前点我们也可以快速定位，因为 属性 $v$ 已经经过离散化，属性 $v$ 相对位置之前的点上的总和就是我们所求点集合个数和总和，用两个树状数组维护一下这两个值即可。

先排序再从前往后遍历动态更新保证了一个值的最优，离散化固定死了所有点的相对顺序且能快速定位，树状数组保证了快速统计相关值。

时间复杂度 $O(nlogn)$

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
#define maxn 200039

int n;
struct p
{
	int pos;
	ll v;
	bool operator < (p b)
	{
		return pos < b.pos;
	}
}a[maxn];

struct disp
{
	int n;
	int sz; 
	ll acopy[maxn];
	ll lsh[maxn]; 
	void init()
	{
		_for(i,1,n+1)
			acopy[i] = a[i].v;
		sort(acopy+1,acopy+1+n);
		sz = unique(acopy+1,acopy+1+n)-(acopy+1);
		_for(i,1,n+1)
			lsh[i] = lower_bound(acopy+1,acopy+1+sz,a[i].v)-acopy;
	}
	ll getsmall(int x)
	{
		return lower_bound(acopy+1,acopy+1+sz,x)-acopy;
	}
	ll getbig(int i)
	{
		return acopy[lsh[i]];
	}
}dsp;

struct bittree
{
	ll c[maxn];
	ll ask(int x)
	{
		ll ans = 0;
		for(; x; x -= x&-x)
			ans += c[x];
		return ans;
	}
	void add(int x,ll d)
	{
		for(; x <= maxn;x += x&-x)
			c[x] += d; 
	}
}btcnt,btsum;

int main()	
{
	ios::sync_with_stdio(false);
	cin >> n;
	dsp.n = n;
	_for(i,1,n+1)
		cin >> a[i].pos;
	_for(i,1,n+1)
		cin >> a[i].v;
	//按照属性x排序
	sort(a+1,a+1+n);
    //对属性v离散化
	dsp.init();

	ll rnt = 0;
	_for(i,1,n+1)
	{
		//当前遍历到的点的属性x一定更优，求他的属性v的离散值
        int x = dsp.getsmall(a[i].v);
        //答案累加
		rnt += btcnt.ask(x)*a[i].pos-btsum.ask(x);
        //当前点属性v离散值对应树状数组部分  （该点按照属性v从小到大排序的一个相对位置）加上属性x
		btsum.add(x,a[i].pos);
        //当前点属性v离散值对应树状数组部分 （同上） 加上1，代表该位置多了一个点
		btcnt.add(x,1);
	}
	printf("%lld\n",rnt);
	return 0;
}

```

