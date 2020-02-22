# 1020 Animal Observation (hard version)

来源：https://codeforces.com/contest/1304/problem/F2

$tag:$ `前缀和` `线段树` `数据结构优化dp` `滑动区间`



## 题目大意

给定一个 $n×m(1≤n≤50,1≤m≤2×10^4)$ 的矩阵，在每一行你可以选择连续的且大小为$2× k(1≤k≤m)$ 的子矩阵并获得矩阵内所有数的加和。注意，重叠部分只算一次，且最后一行子矩阵大小缩减为 $1×k$。



## 题目样例

### 样例输入

```
4 5 2
0 2 1 1 0
0 0 3 1 2
1 0 4 3 1
3 3 0 0 4
```

### 样例输出

```
25
```

### 样例解释

![img](https://espresso.codeforces.com/49c4b7642b5058b4326289139e84c26b2d7211e4.png)



可以证明，按照图中所示的选取方法可以得到最大加和 $25$ 。



## 题目解法

想象一下，每一次选取你需要在意什么？首先就是上一行选了什么，这直接影响到了这一行的选取。现在我们考虑，当上一行确定且当前是第几行确定以后，是否已经得到的值越大越好？无后效性+最优子结构，动态规划走一波。

$dp[i][j]$ 表示当前在第 $i$ 行且这一行且这一行选 以 $(i,j)$ 为左上角的子矩阵，转移就从 $dp[i-1]$ 转移，根据上一行选取的不同，减去重叠部分以后取最大值即可。枚举一遍时间复杂度 $O(nm^2)$，不可接受。

考虑优化。考虑滑动区间，考虑每一行的情况。如果重叠也计算在内那自然可以从上一行的各个 $dp[i-1]$ 直接挑一个最大的转移就行，但既然不是这样的，因此我们就在上一层的基础上把对应的每个 $j$ 的重叠部分减去。

当我们想在 $(i,1)$ 放置为此左上角的子矩阵时，其实上面如果有个以 $(i-1,1)$为左上角的子矩阵 了，那就要在原先 $dp[i-1][1]$ 基础上减去第 $i$ 行的 $sum\{1...k\}$ ，那如果上面放在 了以 $(i-1,2)$为左上角的子矩阵，那就要在原先 $dp[i-1][2]$ 基础上减去第 $i$ 行的 $sum\{2...k\}$ ，这代表什么？这代表 $dp[i][1]$ 只能从上面获得减完以后的收益，然后累积上这一行以及下一行的结果，就是 $dp[i][1]$ 的最终结果，这本质就是一个去重的过程，也就是上面没有那么多下来，那么最后在上面这些减完的收益中取个最大值加上两行结果就可以了。

那如果想在 $(i,2)$ 放置呢？那就把刚刚对 $1$ 减的还回去，但是从 $2$ 开始，到 $2+k-1$ ，收益都要减去该行的 $2+k-1$ 位置的数值，因为上面如果有个以 $(i-1,2)$ 为左上角的子矩阵，那就是和现在想选的子矩阵重叠了，自然要将该子矩阵最后一个元素的收益也减掉。总之减多少就是看重叠部分。

往后亦如此，注意要把减的都还回去。用线段树维护一下就好。

时间复杂度 $O(nmlogm)$。

## 完整代码

```c++
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef double db;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f
#define ZHUO 11100000007
#define MOD 1000000007
#define MIKUNUM 39
#define pb push_back
#define debug() printf("Miku Check OK!\n")
#define X first
#define Y second

const int maxn = 69;
const int maxm = 20039;

int n, m, k;
int dp[maxn][maxm];
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
	}tree[maxm<<2];
	void memst()
	{
		memset(tree,0,sizeof(tree));
	}
	void build(int p,int l,int r,int i)
	{
		l(p) = l,r(p) = r;
		if(l==r)
		{
			MAX(p) = dp[i][l];
			return ;
		}
		int mid = (l+r)/2;
		build(p*2, l, mid,i);
		build(p*2+1, mid+1, r,i);
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

struct pres
{
	ll n;
	ll m;
	ll A[maxn][maxm];
	ll S[maxn][maxm];
	void getsum()
	{
		_for(i,1,n+1)
		_for(j,1,m+1)
		S[i][j] = S[i-1][j]+S[i][j-1]-S[i-1][j-1]+A[i][j];
	}
	ll lefttopasksum(ll x,ll y,ll r1,ll r2)
	{
		ll tot = S[min(n,x+r1-1)][min(m,y+r2-1)];
		tot -= S[x-1][min(m,y+r2-1)];
		tot -= S[min(n,x+r1-1)][y-1];
		tot += S[x-1][y-1];
		return tot;
	}
}P;

int main()
{
    ios::sync_with_stdio(false);
    cin >> n >> m >> k;
    P.n = n+1,P.m = m;
    
    memset(P.A,0,sizeof(P.A));
    _for(i,1,n+1)
		_for(j,1,m+1)
		 cin >> P.A[i][j];
    
    P.getsum();
    
    _for(i,1,n+1)
    {
    	if(i!=1)
    	{
			T.memst();
			T.build(1,1,m,i-1);
			_for(j,1,k+1)
				T.change(1,j,j,-P.lefttopasksum(i,j,1,k-j+1));
		}
    	
		_for(j,1,m-k+2)
    	{
			int val = P.lefttopasksum(i,j,2,k);
			if(i==1)
			{
				dp[i][j] = val;
				continue;
			}
			if(j!=1)
			{
				T.change(1,max(1,j-k),j-1,P.A[i][j-1]);
				T.change(1,j,j+k-1,-P.A[i][j+k-1]);
			}
			dp[i][j] = T.ask4max(1,1,m)+val;
		}
	}
	printf("%d",*max_element(dp[n]+1,dp[n]+m+1));
    return 0;
}
```

