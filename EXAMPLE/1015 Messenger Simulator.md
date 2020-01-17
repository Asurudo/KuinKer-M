# 1015  Messenger Simulator

**来源**： http://codeforces.com/contest/1288/problem/E

$tag$ : `树状数组` `序列问题` 



## 题目大意

给出一个长度为 $n(1≤n≤3×10^5)$ 的数组 $a$，初始时 $a_i=i(1≤i≤n)$。再给出一个长度为 $m(1≤m≤3×10^5)$ 的数组 $b$，其中 $1≤b_j≤n(1≤j≤m)$ 。 遍历数组 $b$ ，对于每一个 $b_j$ 都将 $a_i=b_j$ 移动到 $a$ 数组最前面，其他 $a$ 数组中的数字保持相对位置不变。问数字 $1—n$ 在 $a$ 数组中的最前和最后位置分别是多少。 

## 题目样例

### 样例输入

```
5 4
3 5 1 4
```

### 样例输出

```
1 3
2 5
1 4
1 5
1 5
```

### 样例解释

样例中，$a$ 数组的变动如下：

- $[1,2,3,4,5]$
- $[3,1,2,4,5]$
- $[5,3,1,2,4]$
- $[1,5,3,2,4]$
- $[4,1,5,3,2]$

因此，数字 $1$ 的位置分别是 $1,2,3,1,2$，其中最前位置是 $1$ ，最后位置是 $3$。

## 题目解法

首先考虑最前位置，很显然若数组 $b$ 中存在 $a_i$ ，则 $a_i$ 的最前位置为 $1$ ，否则一定为 $i$。

思考最后位置什么时候会发生变动：只需要考虑数组 $b$ 中出现 $a_i$ 和遍历完所有数组 $b$ 中所有元素时考虑即可，其他时候考虑都是多余的。

接下来考虑模拟这个移动过程。我们开一个大小为 $n+m$ 的数组 $c$，前 $n$ 位置 $1$，表示$n,n-1,n-2 ......3,2,1$（数组 $a$ 的初始状态的倒叙），则对于数组 $a$，初始时每个数字在新数组中的位置 $pos_{a_i}=n-i+1$。对于每个数组 $b$ 中的数字，我们先统计新数组 $c$ 中 $1—(pos_{a_i}-1)$ 有多少个 $1$ ，则 $n-sum(1)$ 就是 $a_i$ 当前所在位置，用它更新最后位置后，置 $c[pos_{a_i}]$ 为 $0$ ，从新数组的最后一个没用过的位置搞一个新位置过来，让 $pos_{a_i}$ 等于新位置的坐标，接着将新位置置 $1$ ,这就算把 $a_i$ 移动到了最前面。

那么这个 **单点更新·区间查询** 的操作很明显用树状数组是最好的。

时间复杂度 $O((n+m)log(n+m))$  

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
#define maxn 300003*2 
#define x first
#define y second

int n, m;
ll send[maxn];
ll ans1[maxn], ans2[maxn]; 
ll a[maxn];
int pos[maxn];
struct bittree
{
	ll c[maxn];
	//初始化树状数组 
	void build()
	{
		_for(i,1,n+1)
			add(i,a[i]);
	} 
	//返回 a数组[1,x]的区间加和 
	ll ask(int x)
	{
		ll ans = 0;
		for(; x; x -= x&-x)
			ans += c[x];
		return ans;
	} 
	//n为数组大小，将a数组下标为 x 的地方加上d,然后一层一层向上更新 
	void add(int x,ll d)
	{
		for(; x <= maxn;x += x&-x)
			c[x] += d; 
	}
}BT1;

int main()
{
	ios::sync_with_stdio(false);
	cin >> n >> m;
	_for(i,1,n+1)
	{
		//最先位置默认是i
        ans1[i] = i;
        //前i为置1
		a[i] = 1;
        //初始时各个数字在新数组中的位置
		pos[i] = n-i+1;
	}
	_for(i,1,m+1)
	{
		cin >> send[i];
        //若移动到过最前面，则最前位置为1
		ans1[send[i]] = 1;
	}
	BT1.build();
	_for(i,1,m+1)
	{
		//看看当前数字在新数组c里的前面有多少个1，然后用n减一下后更新ans2
        ans2[send[i]] = max(ans2[send[i]],n-BT1.ask(pos[send[i]]-1));
        //移动当前数字，先在旧位置上置0，再找新位置，再在新位置上置1
		BT1.add(pos[send[i]],-1);
		pos[send[i]] = n+i;
		BT1.add(pos[send[i]],1);
	}
    //全部结束后还要更新一波最后位置，因为可能有数字根本没动过
	_for(i,1,n+1)
	{
		ans2[i] = max(ans2[i],n-BT1.ask(pos[i]-1));
		printf("%lld %lld\n",ans1[i],ans2[i]);
	}
	return 0;
}
```

