# 1014  Minimax Problem

**来源**： http://codeforces.com/contest/1288/problem/D

$tag$ : `位操作` `二分查找` 



## 题目大意

给出 $n(1≤n≤3×10^5)$ 个数组,每个数组的大小均为 $m(1≤m≤8)$ 。其中第 $x$ 个数组的第 $y$ 个数记为 $a_{x,y}$ 。你需要选择两个数组 $a_i$ 和 $a_j$ $(1≤i,j≤n),i==j$ $is$ $ok$。然后你就能得到一个新数组 $b$ 。其中 $b_k=max(a_{i,k},a_{j,k})(1≤k≤m)$ 。你需要选择一对 $i,j$ 使得 $min$ $b_k$ 尽可能大。   

## 题目样例

### 样例输入

```
6 5
5 0 3 1 2
1 8 9 1 3
1 2 3 4 5
9 1 0 3 7
2 3 0 6 3
6 4 1 7 0
```

### 样例输出

```
1 5
```

### 样例解释

选择 $a_1$ 和 $a_5$ 可以得到新数组 $b:$ $\{5，3，3，6，3\}$ ，则 $min$ $b_k=3$ ，可以证明选择 $1,5$ 可以得到最大的 $min$ $b_k$。

## 题目解法

观察到是最大最小问题且答案具有单调性，因此首先考虑用二分查找确定 $X=$ $min$ $b_k$ 。则由题意，对于每个数组，若其中的值大于等于确定的 $X$，则说明新数组的这一列取最大值后一定也大于等于 $X$。因此我们考虑将大于等于 $X$ 的值置 $1$，否则置 $0$ 。这样一列就可以看作是二进制而用一个整数表示。原问题中取最大值的操作等价转换为 $01$ 二进制数组的 $OR$ 操作，新数组中若为 $1$ 说明该列可满足 $X$ 的标准。若新数组中所有列都为 $1$ 说明所有列都可满足 $X$ 的标准，即新数组中的最小值也大于等于 $X$。所以更新答案以后返回 $true$。

## 完整代码

```c++
#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define ZHUO 11100000007
#define SHENZHUO 1000000007
#define pb push_back
#define debug() printf("Miku Check OK!\n")
#define maxn 300003
#define x first
#define y second

int n, m;
int a[maxn][10];
pair<ll,ll> ans, tmp;
bool check(ll x)
{
	//记录bitset及对应的行号
    map<unsigned long,int> mp;
	_for(i,0,n)
	{
		bitset<10> b;
        //若二维数组的当前值≥x，说明经过 max 操作后该位可以保证 “不拖后腿”，置1，否则置0
		_for(j,0,m)
			if(a[i][j]>=x)
				b[j] = 1;
			else
				b[j] = 0;
        //最多有8位，所以mp中最多有 2^8=256个元素
		mp[b.to_ulong()] = i+1;
	}
	//将mp里的内容都扔到vector里
	vector<pair<unsigned long,int>> v;
	for(auto p:mp)
		v.pb({p.x,p.y});
	
    //二维循环枚举 bitset 集
	int sz = v.size();
	_for(i,0,sz)
	{
		bitset<10> d(v[i].x);
		_for(j,0,sz)
		{
			bitset<10> e(v[j].x);
            //若两个bitset进行OR操作后全部m位都为1，说明原数组的两行经过max操作后每个数都≥x，因此记录
            //行号以后返回true
			if((d|e)==(1<<m)-1)
			{
				tmp.x = v[i].y,tmp.y = v[j].y;
				return true;
			}
		}
	}
    //有至少一位在拖后腿
	return false;
}

ll Bsearch(ll l,ll r)
{
	while(l < r)
	{
		ll mid = (l + r + 1) >> 1;
        //若当前mid可以满足，更新答案
		if(check(mid))
		{
			l = mid;
			ans.x = tmp.x,ans.y = tmp.y;
		}
		else
			r = mid - 1;
	}
	return l;
} 

int main()
{
	ios::sync_with_stdio(false);
    //预设答案为选取第一行和第一行
	ans.x = ans.y = 1;
	cin >> n >> m;
	_for(i,0,n)
		_for(j,0,m)
			cin >> a[i][j];
	
	Bsearch(0,INT_MAX);
	cout << ans.x << " " << ans.y;
	return 0;
}
```

