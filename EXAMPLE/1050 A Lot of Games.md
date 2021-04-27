# 1050  A Lot of Games

**来源**：https://codeforces.com/contest/455/problem/B

$tag$ : `博弈论` `trie树` 



## 题目大意

两人博弈，给你 $n(1≤n≤10^5)$ 个字符串。两个人共同构筑一个字符串，轮流往字符串尾部添加字母，但添加完字母后必须是给定的字符串中至少一个字符串的前缀，谁先不能添加谁输。

共进行 $k(1≤k≤10^9)$ 轮，前一轮输了的人下一轮为先手，双方都是最优策略，最后一轮谁赢就是游戏赢家，问谁会赢。

## 题目样例

### 样例输入

```
3 1
a
b
c
```

### 样例输出

```
First
```

### 样例解释

先手不管选哪一个字母，后手都无法选择一个字母使得字符串变为 $a$ 或 $b$ 或 $c$ 的前缀，而游戏只进行一轮，所以先手胜。

## 题目解法

我们将每个字符串看作路径构建 $trie$ 树，则双方的博弈变成在树上走路。根据博弈论基本原理（见模板），先预处理出两方追求必胜和追求必败的情况，接着对于每个字母，若先手可以控制每一局的胜负，或者能确保胜利且 $k$ 是奇数，则先手胜，否则后手胜。

时间复杂度 $O(n)$。

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

#define maxn 200003
#define maxe 450003 
struct G
{
    int n,m; 
	int Next[maxe]; 
	int head[maxn]; 
	int ver[maxe];
	int tot;
    void clear(int x)
	{
		tot = 0;
		memset(head,0,(x+3)*sizeof(int));
	}
	void add(int x,int y)
	{
		ver[++tot] = y,Next[tot] = head[x],head[x] = tot;
	}
} g;

struct trie
{
	int tot;
	int sum[maxn*26];
	int trie[maxn][26];
	void clear()
	{
		tot = 1;
		memset(trie,0,sizeof trie);
		memset(sum,0,sizeof sum);
	}
	void insert(const string& a)
	{
		int rt = 1;
		_for(i,0,a.size())
		{
			int id = a[i]-'a';
			if(!trie[rt][id])
			{
				trie[rt][id] = ++tot;
				g.add(rt,tot);
				g.add(tot,rt); 
			}
			rt = trie[rt][id];
			if(i==a.size()-1)
				sum[rt] ++;
		}
	}
}te;

int dp[maxn], dp2[maxn];
int dfs(int x,int fa)
{
	int rv = 0, fg = 0;
	for(int i = g.head[x]; i; i = g.Next[i])
	{
		int y = g.ver[i];
		if(y==fa)	
			continue;
		fg = 1;
		rv |= dfs(y,x);
	}
	if(!fg)
		return dp[x] = 1;
	return dp[x] = !rv;
}

int dfs2(int x,int fa)
{
	int rv = 1, fg = 0;
	for(int i = g.head[x]; i; i = g.Next[i])
	{
		int y = g.ver[i];
		if(y==fa)	
			continue;
		fg = 1;
		rv &= dfs2(y,x);
	}
	if(!fg)
		return dp2[x] = 1;
	return dp2[x] = !rv;
}

int n, k;
int main()
{
	Design;
	te.clear();
	cin >> n >> k;
	_for(i,1,n+1)
	{
		string a;
		cin >> a;
		te.insert(a);
	}
    //都追求胜利或者都追求失败
	dfs(1,-1);dfs2(1,-1);
	int ok1 = 0, ok2 = 0;
	for(int i = g.head[1]; i; i = g.Next[i])
	{
		int y = g.ver[i];
        //如果有一个字母，先手选中后必胜，则必胜可达
		if(dp[y])
			ok1 = 1;
        //如果有一个字母，先手选中后必败，则必败可达
		if(!dp2[y])
			ok2 = 1;
	}
    //先手可控制任意一局的胜负和先后
	if(ok1 && ok2)
		printf("First");
    //先手只能控制胜利，但想输输不了
	else if(ok1 && !ok2 && (k&0x1))
		printf("First");
    //先手无法确保胜利
	else
		printf("Second");
	return 0;
}
```

