# 1027  Count Triangles

**来源**： http://codeforces.com/contest/1355/problem/C

$tag$ : `三角形` `前缀和` `差分`



## 题目大意

给出 $A,B,C,D(1≤A≤B≤C≤D≤5×10^5)$ 四个数字，要求找出符合条件的三角形个数。其中三角形三条边的长度分别为 $x,y,z$，需满足 $A \leq x \leq B \leq y \leq C \leq z       \leq D$ 。

## 题目样例

### 样例输入

```
1 2 2 5
```

### 样例输出

```
3
```

### 样例解释

一共有 $(1,2,2),(2,2,2),(2,2,3)$ 三种三角形。

## 题目解法

想要构成三角形，自然需要 $x+y>z$ 。那么我们可以枚举 $x$ ，对于该 $x$ ，有 $[x+b,x+c]$ 实数域上都 $+1$ 次数，代表若此时给出 $z≥y≥x$ ，则对总答案的贡献为 $[z+1,+∞]$ 上的次数。最后求一个次数的前缀和，对于给定的 $z$ 就能 $O(1)$ 计算出对总答案的贡献。

时间复杂度 $O(n)$。

## 完整代码

```c++
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define _for(i,a,b) for(ll i = (a);i < b;i ++)
#define _rep(i,a,b) for(ll i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define pb push_back
#define lowbit(x) ((x)&(-x))
#define ZHUO 10000000007
#define maxn 1000039

int n = maxn-1;
struct pres
{
	ll a[maxn];
	ll s[maxn];
	pres()
	{
		memset(a,0,sizeof(a));
		memset(s,0,sizeof(s));
	}
	void getsum()
	{
		_for(i,1,n+1)
			s[i] = s[i-1]+a[i];
	}
	ll asksum(ll l,ll r)
	{
		if(l>r)
			return 0; 
        return s[r] - s[l-1];
	}
}P ;

struct chas
{
	ll b[maxn];
	ll s[maxn];
	chas()
	{
		memset(b,0,sizeof(b));
		memset(s,0,sizeof(s));
	}
	void add(int l,int r,ll d)
	{
		b[l] += d;
		b[r+1] -= d;
	}
	void getfinal()
	{
		_for(i,1,n+1)
			s[i] = s[i-1]+b[i];
	}	
}C;

int main()
{
	ll a, b, c, d;
	scanf("%lld%lld%lld%lld",&a,&b,&c,&d);
    //对于[i+b,i+c] 实数域上的每一个数所对应个数+1
	_for(i,a,b+1)
		C.add(i+b,i+c,1);
    //求差分序列的前缀和
	C.getfinal();
	//求差分序列的前缀和的前缀和，便于计算
	memcpy(P.a,C.s,sizeof(C.s));
	P.getsum();
	
	ll sum = 0;
    //对于实数域[c,d]上的每一个数字 i ，计算 [i+1,+∞] 上的三角形个数加和，即为该位置对答案的贡献
	_for(i,c,d+1)
		sum += P.asksum(i+1,n-2);
	printf("%lld\n",sum);
	return 0;
}
```

