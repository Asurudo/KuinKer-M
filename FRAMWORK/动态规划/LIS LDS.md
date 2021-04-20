# LIS LDS相关dp

------

该问题分为  $O(n^2)$ 的可记录路径解法和 $O(nlogn)$ 的二分不可记录路径解法，两种方法都是 $dp$ 。

对于 $O(n^2)$ 解法，令 $dp[i]$ 为以 $a[i]$ 结尾的 $LIS$ 长度，例如对于以下数组，则有 $dp$ 数组如下：

| $a$  | 68   | 69   | 54   | 64   | 68   | 64   | 70   | 67   | 78   | 62   |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| $dp$ | 1    | 2    | 1    | 2    | 3    | 2    | 4    | 3    | 5    | 2    |

初始状态 $dp[0]=0$ ，转移为 $dp[i]=max\{dp[j]+1\}(0≤j＜i,a[j]＜a[i])$

转移路径即再开一个数组 $pre$，当每次进行转移时记录上一个的下标。



对于 $O(nlogn)$ 解法，令 $dp[i]$ 为长度为 $i$ 的 $LIS$ 的结尾的最小值，对于上面的 $a$ 数组，$dp$ 数组的情况如下：

| $a$  | 68   | 69     | 54     | 64     | 68         | 64         | 70             | 67             | 78                 | 62                 |
| ---- | ---- | ------ | ------ | ------ | ---------- | ---------- | -------------- | -------------- | ------------------ | ------------------ |
| $dp$ | 68   | 68，69 | 54，69 | 54，64 | 54，64，68 | 54，64，68 | 54，64，68，70 | 54，64，67，70 | 54，64，67，70，78 | 54，62，67，70，78 |

遍历数组 $a$ ，若当前 数字比 $dp$ 数组的最后一位还要大，则将其放在 $dp$ 数组最后一位，拓展 $dp$ 数组；否则二分 $dp$ 数组 找到最小的比当前数字大的数，将他替换。

下面是某个需要求出非严格下降序列长度和严格上升序列长度的代码模板。

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
#define maxn 1000003
#define debug() printf("Miku Check OK!\n")

//严格上升
struct LIS1
{
	//数组长度
	int n;
	ll a[maxn];
	//dp数组长度
	int dplen;
	ll dp[maxn];
	int getLIS()
	{
		dplen = 0;
		dp[1] = a[1];
		_for(i,2,n+1)
		if(a[i]>dp[dplen])
			dp[++dplen] = a[i];
		else
		{
			int k = lower_bound(dp+1,dp+dplen+1,a[i])-dp;
			dp[k] = a[i];
		}
		return dplen;
	}
} LIS;

//非严格上升
struct LIS2
{
	//数组长度
	int n;
	ll a[maxn];
	//dp数组长度
	int dplen;
	ll dp[maxn];
	int getLIS()
	{
		dplen = 0;
		dp[1] = a[1];
		_for(i,2,n+1)
		if(a[i]>=dp[dplen])
			dp[++dplen] = a[i];
		else
		{
			int k = upper_bound(dp+1,dp+dplen+1,a[i])-dp;
			dp[k] = a[i];
		}
		return dplen;
	}
};

//严格下降
struct LDS1
{
	//数组长度
	int n;
	ll a[maxn];
	//dp数组长度
	int dplen;
	ll dp[maxn];
	int getLDS()
	{
		dplen = 1;
		dp[1] = a[1];
		_for(i,2,n+1)
		if(a[i]<dp[dplen])
			dp[++dplen] = a[i];
		else
		{
			int k = lower_bound(dp+1,dp+dplen+1,a[i],greater<ll>())-dp;
			dp[k] = a[i];
		}
		return dplen;
	}
};

//非严格下降
struct LDS2
{
	//数组长度
	int n;
	ll a[maxn];
	//dp数组长度
	int dplen;
	ll dp[maxn];
	int getLDS()
	{
		dplen = 1;
		dp[1] = a[1];
		_for(i,2,n+1)
		{
			if(a[i] <= dp[dplen])
			{
				dp[++dplen] = a[i];
			}
			else
			{
				int k = upper_bound(dp+1,dp+dplen+1,a[i],greater<ll>())-dp;
				dp[k] = a[i];
			}
		}
		return dplen;
	}
} LDS;

inline bool read(ll &x)
{
	char c=getchar();
	if(c==EOF)return false;
	while(c>'9'||c<'0')c=getchar();
	while(c>='0'&&c<='9')
	{
		x=(x<<1)+(x<<3)+(c^48);
		c=getchar();
	}
	return true;
}
int n = 1;
int main()
{
	while(read(LDS.a[n++]))
		LIS.a[n-1] = LDS.a[n-1];
	n-=2;
	LIS.n = LDS.n = n;
	printf("%d\n%d",LDS.getLDS(),LIS.getLIS());
	return 0;
}
```

