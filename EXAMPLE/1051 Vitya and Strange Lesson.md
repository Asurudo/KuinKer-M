# 1051 Vitya and Strange Lesson

来源：https://codeforces.com/problemset/problem/842/D

$tag:$ `trie树` `位操作`  

## 题目大意

给你一个长度为 $n(1≤n≤3×10^5)$ 的非负整数数组 $a(0≤a_i≤3×10^5)$ ，再给你 $m(1≤m≤3×10^5)$ 个询问，每个询问包含一个整数 $x(0≤x≤3×10^5)$，每次你需要将整个数组异或上 $x$ ，然后输出整个数组的 $mex$ 值。



## 题目样例

### 样例输入

```
4 3
0 1 5 6
1
2
4
```

### 样例输出

```
2
0
0
```

### 样例解释

`0 1 5 6 -> 1 0 4 7 -> 3 2 6 5 -> 7 6 2 1`

因此，异或值分别是 $2,0,0$ 。  



## 题目解法

看到异或，就想到 $01$ $tire$ 或者线性基，先把原数组所有数字按照正常方式插入 $trie$ ，接着预处理出以某个结点为根的子树是否是完全二叉树，便于以后计算 $mex$。

对于异或操作，就是每一层设一个翻转标记，如果异或上的数字的该位为 $1$ ，则将标记异或上 $1$ 。

查询 $mex$ 时，若对于 $trie[rt][0]$ 为根的子树不为完全二叉树，则指针进入该子树，否则考虑 $trie[rt][1]$ 。

时间复杂度 $O(nlogn+mlogn)$ 。 



## 完整代码

```c++
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define pb push_back
#define lowbit(x) ((x)&(-x))
#define mod 1000000007
#define MIKU 39
#define Design ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define debug() printf("Miku Check OK!\n")
#define MIKUCHAR "M"

#define maxn 300039
#define maxe 450003
struct trie
{
	#define bitmaxd 20
	int tot;
	int fg[bitmaxd];
	int ex[maxn*bitmaxd];
	int trie[maxn*bitmaxd][2];
	void insert(int a)
	{
		int rt = 0;
		_rep(i,bitmaxd,-1)
		{
			int id = ((a >> i)&0x1);
			if(!trie[rt][id])
				trie[rt][id] = ++tot;
			rt = trie[rt][id];
			if(!i)
				ex[rt] = 1;
		}
	}
    //判定rt为根的子树是否为完全二叉树
	int getex(int rt,int d)
	{
		int ok = 1;
		if(d!=0)
		{
			if(trie[rt][0])
				ok &= getex(trie[rt][0], d-1);
			else
				ok = 0;
			if(trie[rt][1])
				ok &= getex(trie[rt][1], d-1);
			else
				ok = 0;
		}
		else
			return ex[rt];
		return ex[rt] = ok;
	}
	void totxorval(int val)
	{
		//对于每一层设一个标记
        _rep(i,bitmaxd,-1)
			if((val>>i) & (0x1))
				fg[i] ^= 1;
	} 
	int mex()
	{
		int rt = 0;
		int ans = 0;
		_rep(i,bitmaxd,-1)
		{
			//翻转
            int id = 0;
			if(fg[i])
				id = 1;
            //如果为完全二叉树
			if(ex[trie[rt][id]])
			{
				if(trie[rt][id^1])
					rt = trie[rt][id^1], ans += 1<<i;
				else
				{
					ans += 1<<i;
					return ans;
				}
			}
			else
			{
				if(trie[rt][id])
					rt = trie[rt][id];
				else
					return ans;
			}
		}
		return ans;
	}
}te;

int n, m;
int main()
{
	scanf("%d%d",&n,&m);
	_for(i,1,n+1)
	{
		int x;
		scanf("%d",&x);
        //插入trie
		te.insert(x);
	}
	te.getex(0,bitmaxd+1);
	_for(i,1,m+1)
	{
		int x;
		scanf("%d",&x);
		te.totxorval(x);
		printf("%d\n",te.mex());
	}
	return 0;
}
```

