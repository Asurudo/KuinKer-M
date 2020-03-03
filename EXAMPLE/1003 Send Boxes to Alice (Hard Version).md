# 1003  Send Boxes to Alice (Hard Version) 

来源： http://codeforces.com/contest/1255/problem/E2 

$tag$ : `数论`  `贪心`   `因数枚举` 

## 题目大意

给定一个大小为 $n(1≤n≤10^6)$ 的数组，里面有 $n$ 个元素 $a_i(0≤a_i≤10^6)$ 。可以花费 $1$ 个代价将 $a_i$ 加/减 $1$ ，并让 $a_{i-1}$ 或 $a_{i+1}$ 减/加 $1$ ，要求能让数组内的所有数被 $k(k>1)$ 整除的最小代价。



## 题目样例

### 样例输入

```
5
3 10 2 1 5
```

### 样例输出

```
2
```

### 样例解释

可以将 $a_2$ 移动 $1$ 到 $a_3$ ，$a_4$ 移动 $1$ 到 $a_5$ ，这样数组中每个数都能被 $3$ 整除，总花费为 $2$ 。



## 题目解法

因为移动操作不会改变该数组的和，所以我们计算数组的和 $S=\sum_{n}^{i=1}a_i$ ，移动后数组中每个数 $a_i$ 若都能被 $k$ 整除，则其和 $S$ 也一定能被 $k$ 整除 ，即 $S|k$ ，同样，若 $S|k$ ，则一定存在某种移动方式达成题目要求，因此我们可以分解 $S$ 的质因数，找到所有可能的 $k$ ，然后每次尝试用不同的 $k$ 更新花费。

但是这样有一个问题，对于题目所给的数据，数组长度最大为 $10^6$，若对于每个可能的 $S$ 的质因数都遍历一遍，那么要求质因数必须小于 $100$ 个，而$S$ 最大可达到 $10^{12}$ ，其因数个数最大一定可以超过 $100$ 个。

怎么办呢？其实我们可以缩小 $k$ 的范围，没必要拿 $S$ 的所有质因数进行枚举，若对于 $S$ 的任意两个质因数 $S_1$ 和 $S_2$ ，若有 $S_1|S_2$ ，则 $S_1$ 可以不用加入到要遍历的可能的 $k$ 中。  因为若有$a_i|S_1$ ,则必有 $a_i|S_2$ ，同时 $S_2<S_1$ ，选择$S_2$，则多出的需要移动的余数部分反而有可能更少。

此时可能的 $k$ 最多只有 $13$ 个，因为最差的情况就是 $S$ 为前 $13$ 个素数相乘得到的数。对于每一个可能的 $k$ ，我们贪心的移动数组的值。令 $b_i=a_i\%k$ ，则我们可以直接在 $b$ 数组上进行移动，因为对于任意 $a_i$ ，只差 $a_i\%k$ 或 $k-(a_i\%k)$ 就能被 $k$ 整除。因此我们只要把这些多出来的部分解决掉就行。至于 是移走 $a_i\%k $ 还是从别的地方移过来 $k-(a_i\%k)$ ，就要看是哪个更小了，这样我们就为每个 $a_i$ 找了一个需要变成的“基准数”。 

接下来就是经典问题，最小操作数移动数组使得数组值全都为“基准数”了。从前往后扫一遍，如果当前 $a_i$ 多出来，就把多出来的部分给 $a_{i+1}$ ，如果少了，就记录下来，以后从 $a_{i+1}$ “进账”。可以证明这样的操作数一定是最小的。

时间复杂度$O(n)$，不过常数略大。



## 完整代码

```c++
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define _for(i,a,b) for(register ll i = (a);i < b;i ++)
#define _rep(i,a,b) for(register ll i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define pb push_back
#define P pair<int,int>
#define maxn 1000003
ll n;
ll a[maxn];
ll b[maxn];
struct getFac
{
	vector<ll> fac;
	bool check(ll x)
	{
		_for(i,0,fac.size())
		if(!(x%fac[i]))
			return false;
		return true;
	}
	//约数枚举 
	void get(ll n)
	{
		for(ll i = 2; i * i <= n; i ++)
			if(n%i==0)
			{
				//判断是否已经为fac数组中某个可能的k的倍数 
				if(check(i))
					fac.pb(i);
				if(i!=n/i && check(n/i))
					fac.pb(n/i);
			}
	}
} gf1;

int main()
{
	ll sum = 0;
	scanf("%lld",&n);
	_for(i,1,n+1)
	{
		scanf("%lld",&a[i]);
		//求和 
		sum += a[i];
	}
	//获取所有可能的k 
	gf1.get(sum);
	//若sum为1，则不可能找到k满足题目要求 
	if(sum!=1)
		gf1.fac.pb(sum);
	ll ans = INF;
	//枚举所有可能的k 
	_for(i,0,gf1.fac.size())
	{
		ll hold = 0;
		ll tpans = 0;
		//预处理b数组b[i]=a[i]%k 
		_for(j,1,n+1)
			b[j] = a[j]%gf1.fac[i];
		_for(j,1,n+1)
		{
			//更新操作数 
			tpans += abs(hold);
			//多退少补 
			b[j] += hold;
			//看看现在的情况离基准数差多少 
			b[j] %= gf1.fac[i];
			//多了，加到下一个，少了，先记着，从下一个拿 
			if(b[j]<gf1.fac[i]-b[j])
				hold = b[j];
			else
				hold = b[j]-gf1.fac[i];
		}
		//对于当前k更新ans 
		ans = min(ans,tpans);
	}
	if(ans!=INF)
		printf("%lld",ans);
	else
		printf("-1");
	return 0;
}
```

