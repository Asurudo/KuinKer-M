# ST表

## 例题编号：

无

------

ST表是一种利用倍增思想处理区间最值的数据结构，预处理 $O(nlogn)$ ，查询 $O(1)$ 。

$max[i][j]$ 表示从 $i$ 开始的 $2^j$ 个数中的最大值，也就是 $[i,2^j-1]$ 中的最大值，例如 $max[i][1]$ 表示的是 $i$ 位置和 $i+1$ 位置的最值。

![img](https://images2018.cnblogs.com/blog/1101696/201803/1101696-20180317093105594-446018365.png)

需要查询 $[l,r]$ 的时候，就要计算出 $k$ 使得：

![img](https://images2018.cnblogs.com/blog/1101696/201803/1101696-20180317094520949-961365919.png)

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
#define MIKU 39
#define maxn 1000039
int n, m;
ll a[maxn];
struct STlist 
{
	//如果区间长度变动，则21也需要变动 
	ll MAX[maxn][21];
	ll MIN[maxn][21];	
	void build()
	{
		_for(i,1,n+1)
			MAX[i][0] = MIN[i][0] = a[i];
		_for(j,1,22)
			for(int i = 1;i+(1<<j)-1 <= n; i ++)
			{
				MAX[i][j] = max(MAX[i][j-1],MAX[i+(1<<(j-1))][j-1]);
				MIN[i][j] = min(MIN[i][j-1],MIN[i+(1<<(j-1))][j-1]);
			}
	}
	ll ask4max(int l,int r)
	{
		int k = log2(r-l+1); 
    	return max(MAX[l][k],MAX[r-(1<<k)+1][k]);
	}
	ll ask4min(int l,int r)
	{
		int k = log2(r-l+1); 
    	return min(MIN[l][k],MIN[r-(1<<k)+1][k]);
	}
}ST;

inline int read()
{
	int x=0,f=1;char ch=getchar();
	while (!isdigit(ch)){if (ch=='-') f=-1;ch=getchar();}
	while (isdigit(ch)){x=x*10+ch-48;ch=getchar();}
	return x*f;
}

int main()
{
	scanf("%d%d",&n,&m);
	_for(i,1,n+1)
		scanf("%lld",&a[i]);
	ST.build();
	_for(i,1,m+1)
	{
		int l, r;
		l = read(), r = read();
		printf("%lld\n",ST.ask4max(l,r));
	}
	return 0;
}
```



