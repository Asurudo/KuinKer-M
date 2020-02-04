# 1016  Mind Control

**来源**：https://codeforces.com/contest/1291/problem/C

$tag$ : `线段树` `单调队列` `数学` `贪心` 



## 题目大意

给定一个长度为 $n(1≤n≤3500)$ 的正整数数组，现在已知有 $n$ 个人，你排在第 $m(1≤m≤n)$ 位，每次每个人都可以从数组头部或者尾部选取一个元素。现在你可以控制 $k(0≤k≤n-1)$ 个人指定他们分别选头或者尾部。已知你的选取会采用最优策略，而你没有控制的人的选取不定。现在记你可能选到的数字中最小为 $x$ ，询问 在你控制以后，$x$ 最大为多少。



## 题目样例

### 样例输入

```
4
6 4 2
2 9 2 3 8 5
4 4 1
2 13 60 4
4 1 3
1 2 2 1
2 2 0
1 2
```

### 样例输出

```
8
4
1
1
```

### 样例解释

对于第一个样例，你排在第 $4$ 位，现在你能控制 $2$ 个人，所以你可以随便让第一个人选什么，控制剩下两个人不要选到 $8$ 即可。这样你就能选到 $8$ ，可以证明这样控制，$x$ 为 $8$ 为最大。



## 题目解法

首先有一点可以确定：你控制你之后的人没有用，所以 $k$ 最大为 $m-1$。然后我们也不用考虑具体怎么控制这些人。观察一下，不管怎么取，最后一定是剩下一个长度为 $n-m+1$ 的原数组的子数组让你取，而这个子数组具体是哪个子数组，这要看两部分内容：

- 你控制了多少个人取头部，人数我们记为 $x$
- 你没法控制的人中，有多少个取了头部，人数我们记为 $y$

所以这个子数组就敲定下来了，因为你也只能取头部或者尾部，所以最后答案就是 $max(a_{1+x+y},a_{1+x+y+(n-m)})$ 

这样就有一个很显然的 $O(n^2)$ 算法，双重循环遍历 $x$ 和 $y$ 即可，我们记 $b_i = \max(a_{1+i}, a_{1+i+(n-m)})$

最终答案就是 $\displaystyle \max_{x \in [0 , k]} \bigg[ \min_{y \in [0 , m-1-k]} b_{x+y} \bigg]$ 

解释一下，最后的子数组你一定是取子数组两端的最大值，所以就是 $b_{x+y}$，但是因为你不确定你没法控制的人都取了什么，你只能按最坏情况考虑，他们都在拆你的台，因此就有在 $x$ 敲定后， $y \in[0,m-1-k]$ 中你所能取到的 $b_{x+y}$ 中的最小值，而在这些最小值中，因为 $x$ 是你可以控制的，所以取这些最小值中的最大值就是答案。

我们将这个最终答案式等价变形为 $\displaystyle \max_{x \in [0 , k]} \bigg[ \min_{y' \in [x , x+m-1-k]} b_{y'} \bigg]$ ，也就是令 $y'=x+y$ ,这样换式子，问题就可以被更好的解释成 ：对于数组 $b$，长度为 $m-k$ 的**滑动数组**中的最小值的最大值，滑动数组头部的取值为 $[0,k]$。

用线段树搞一搞就是 $O(nlogn)$ 的解法，用单调队列搞一搞就是 $O(n)$，代码是单调队列实现的。

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
#define maxn 3503
#define X first
#define Y second
int t;
int n, m, k;
int a[maxn];
 
int getb(int i)
{
	return max(a[1+i],a[1+i+(n-m)]);
}
 
int main()
{
	ios::sync_with_stdio(false);
	
	cin >> t;
	while(t--)
	{
		cin >> n >> m >> k;
		_for(i,1,n+1)
			cin >> a[i];
		
		k = min(k,m-1);
		
		deque<int> dq;
		int ans = -INF;
        
		for(int y = 0; y <= m-1;y ++)
		{
			//维护滑动数组长度
            while(dq.size() && dq.front() <= y-(m-k))
				dq.pop_front();
            //若当前不可能成为该滑动数组最小则出队
			while(dq.size() && getb(y) <= getb(dq.back()))
				dq.pop_back();
			dq.pb(y);
            //等滑动数组长度足够时再计算答案
			if(dq.size() && y >= m-k-1)
				ans = max(ans,getb(dq.front()));
		}
		printf("%d\n",ans);
	}
	return 0;
}
```

