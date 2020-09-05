# 1006 Secret Passwords

来源：https://codeforces.com/contest/1263/problem/D

$tag:$ `并查集` `字符串哈希`  `隐式图`

## 题目大意

给定一个字符串列表，其中包含 $n (1≤n≤2×10^5)$ 个只包含小写字母的字符串，每个字符串的长度不超过 $50$ ，字符串总长度不超过 $10^6$。处于以下两种情况的字符串被认为是相等的：

1. 字符串 $a$ 和字符串  $b$ 中有至少一个字母相同。
2. 字符串 $a$ 和字符串列表中另一字符串 $c$ 相同，而 $c$ 又与字符串 $b$ 相同。

问至少长度为多少的字符串才能和列表中所有的字符串都相同？



## 题目样例

### 样例输入

```
4
a
b
ab
d
```

### 样例输出

```
2
```

### 样例解释

字符串 $a$ 与 字符串 $ab$ 通过规则 $1$ 相同，字符串 $b$ 与字符串 $ab$ 通过规则 $1$ 相同，字符串 $a$ 与 字符串  $b$ 通过规则 $2$ 相同，字符串 $d$ 与其余任何字符串都不相同，故最少需要长度为 $2$ 的字符串 $ad$ 或 $bd$ 才能和列表中所有字符串型相同。



## 题目解法

字符串之间的相同，本质上是字母之间的联系，若字符串 $s1$ 中的一个字母 $a$ 与 字符串 $s2$ 中的一个字母 $b$ 有联系，字符串 $s1$ 就和 字符串$s2$ 相同。因此字符串相当于边，而其中的字母是点，所以对于每一个字符串，都可以对其中的字母进行连线，其实用并查集维护即可，因为我们只关心字母 $a$ 与字母 $b$ 是否有联系。出现新字母就将答案 $+1$ ，而两个字母如果**首次建立联系**就将答案 $-1$。



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
#define maxn 103
#define pb push_back
#define debug() printf("Miku Check OK!\n")
int ans;
struct Djs
{
	int par[maxn];
	int high[maxn];
	Djs()
	{
		_for(i,1,maxn)
		{
			par[i] = i;
			high[i] = 0;
		}
	}
	int find(int x)
	{
		//路径压缩
		return par[x] == x ? x : par[x] = find(par[x]);
	}
	void unite(int x,int y)
	{
		x = find(x);
		y = find(y);
		if(x==y) return ;
		//按秩合并
		//若出现全新的连接，则将答案-1 
		ans --;
		if(high[x]<high[y])
			par[x] = y;
		else
		{
			par[y] = x;
			if(high[x]==high[y])
				high[x] ++;
		}
	}
	bool same(int x,int y)
	{
		return find(x) == find(y);
	}
} djs;

int vis[30];
int now[30];
int main()
{
	ans = 0;
	int n;
	scanf("%d",&n);
	memset(vis,0,sizeof(vis));
	_for(i,0,n)
	{
		string s;
		cin >> s;
		memset(now,0,sizeof(now));
		//当前字符串都有哪些字母 
		_for(i,0,s.size())
			now[s[i]-'a'] ++;
		//如果出现了之前没出现过的字母就将答案+1 
		_for(i,0,26)
			if(now[i] && !vis[i])
			{
				vis[i] = 1;
				ans ++;
			}
		//对于该字符串，将他看作边，将其中的字母连线 
		_for(i,0,26)
			if(now[i])
				_for(j,i+1,26)
					if(now[j])
						djs.unite(i,j);
	}
	printf("%d\n",ans);
	return 0;
}
```

