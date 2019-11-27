# 1005  Arson In Berland Forest 

来源： https://codeforces.com/contest/1262/problem/E 

$tag:$ `前缀和` `二分查找` 

## 题目大意

给定一个 $n*m(1≤n,m≤10^6,1≤n*m≤10^6)$ 的矩阵，里面有 "$.$" 和 "$X$" ,其中 $X$ 表示烧毁的树。当刚开始烧起来的时候，每过 $1s$，$X$ 会向周围八个方向传染，**矩阵外的树都没有被烧毁**。现在问你最大可能已经烧过多少秒了？并给出一开始就燃烧的树。



## 题目样例  

### 样例输入

```
10 10
.XXXXXX...
.XXXXXX...
.XXXXXX...
.XXXXXX...
.XXXXXXXX.
...XXXXXX.
...XXXXXX.
...XXXXXX.
...XXXXXX.
..........
```

### 样例输出

```
2
..........
..........
...XX.....
..........
..........
..........
.....XX...
..........
..........
..........
```

### 样例解释

图示中的情况经过 $2s$ 后可以变成输入时候的情况，并且构造不出一张图使得 $3s$ 或更久让构造图变为输入时候的情况。



## 题目解法

首先需要观察到，因为图外都没有起火，所有刚开始就燃烧的地方经过 $Ts$ 后必定会形成一个以它为中心的，边长为 $2T+1$ 的正方形。

而我们现在就是要求最大的 $T$，因此可以考虑二分答案，然后将固定的 $T$ 代入看看是不是所有烧灼形成的“死树区块”的长宽都大于等于 $2T+1$ ，如果是，那就可以考虑增大 $T$ 。

我们将原图处理一下，只要有一棵死树就在那一处置 $1$ ，否则置 $0$，然后求其前缀和。

之后二分答案，对于每一个给定的 $T$，遍历全图，若以当前节点为左上角，其向右向下扩展 $2T+1$ 的正方形里全都是死树，则该正方形的正中就是刚开始燃烧的树。将这些刚开始燃烧的树重新建一个数组存起来，并求其前缀和，之后再遍历一次全图，对于一棵死树，必须要有一棵刚开始燃烧的树覆盖它才行，能覆盖它的树一定处于它的左上角因为我们的正方形标记的是左上角，若不能覆盖则说明 $T$ 过大了，因为当 $T=0$ 时，所有树都刚开始燃烧，一定可以覆盖全图，因此缩小 $T$ 继续尝试，否则就扩大 $T$。

最后得到的 $T$ 就是最大可能的答案 $ans$ ，因为我们只存了正方形的左上角而不是中心，所以最后遍历全图的时候，若对于坐标 $(x,y)$ ，$(x-ans,y-ans)$被作为左上角存下来了，则 $(x,y)$ 就是原正方形的中心。

时间复杂度 $O(nm*log(min(n,m)))$



## 完整代码

```c++
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<ll,ll> P;

#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x7fffffff
#define MOD 100000000
#define maxn 1000003
#define pb push_back
#define debug() printf("Miku Check OK!\n")
char buf[maxn];
struct pres
{
	//矩阵行数为n，列宽为m
	ll n;
	ll m;
	vector<ll> A[maxn];
	vector<ll> S[maxn];
	void getsum()
	{
		_for(i,1,n+1)
		_for(j,1,m+1)
		S[i][j] = S[i-1][j]+S[i][j-1]-S[i-1][j-1]+A[i][j];
	}
	//_for(i,1,n+1)
	//	_for(j,1,m+1)
	//左上坐标为(x,y)算行高r1，列宽r2矩阵部分和
	ll lefttopasksum(ll x,ll y,ll r1,ll r2)
	{
		ll tot = S[min(n,x+r1-1)][min(m,y+r2-1)];
		tot -= S[x-1][min(m,y+r2-1)];
		tot -= S[min(n,x+r1-1)][y-1];
		tot += S[x-1][y-1];
	//	cout << tot << endl;
	//	printf("%lld %lld %lld %lld\n",x,y,r1,r2);
		return tot;
	}
	//右下坐标为(x,y)算行高r1，列宽r2矩阵部分和
	ll rightdownasksum(ll x,ll y,ll r1,ll r2)
	{
		ll tot = S[x][y];
		if(x > r1)
			tot -= S[x-r1][y];
		if(y > r2)
			tot -= S[x][y-r2];
		if(x>r1 && y>r1)
			tot += S[x-r1][y-r2];
		return tot;
	}
} P1,P2;

bool check(ll r)
{
	//2T+1 
	r = r*2+1;
	//不能用memset清零 
	_for(i,1,P1.n+1)
		_for(j,1,P1.m+1)
			P2.A[i][j] = P2.S[i][j] = 0;
	
	//若以(i,j)为左上角，边长为r的正方形里全是烧焦的树，就把(i,j)记下来 
	_for(i,1,P2.n+1)
		_for(j,1,P2.m+1)
			if(P1.lefttopasksum(i,j,r,r)==r*r)
				P2.A[i][j] = 1;
	//计算前缀和 
	P2.getsum();
	//若有烧焦的树没被覆盖到就返回false，T太大 
	_for(i,1,P1.n+1)
		_for(j,1,P1.m+1)
			if(P1.A[i][j])
				if(!P2.rightdownasksum(i,j,r,r))
					return false;
	return true;
}
ll Bsearch(ll l,ll r)
{
	while(l < r)
	{
		ll mid = (l + r + 1) >> 1;
		if(check(mid))
			l = mid;
		else
			r = mid - 1;
	}
	return l;
} 

int main()
{
	ll n,m;
	scanf("%lld%lld",&n,&m);
	P1.n = P2.n = n,P1.m = P2.m = m;
	//nm最大一百万，n和m最大也可能一百万，一百万*一百万开不下，只能动态开 
	//此时不能用memset 
	P1.A[0].resize(m+1),P1.S[0].resize(m+1),P2.A[0].resize(m+1),P2.S[0].resize(m+1);
	_for(i,1,n+1)
	{
		P1.A[i].resize(m+1);
		P1.S[i].resize(m+1);
		P2.A[i].resize(m+1);
		P2.S[i].resize(m+1);
		scanf("%s",buf+1);
		_for(j,1,m+1)
			P1.A[i][j] = (buf[j]=='X');
	}
	//求前缀和 
	P1.getsum();
	//二分答案 
	ll ans = Bsearch(0,(min(n,m)-1)>>1);
	printf("%lld\n",ans);
	check(ans);
	_for(i,1,n+1)
	{
		_for(j,1,m+1)
			//求正方形中心 
			if(i-ans>0 && j-ans>0 && P2.A[i-ans][j-ans])
				printf("X");
			else
				printf(".");
		printf("\n"); 
	}
	return 0;
}
```

