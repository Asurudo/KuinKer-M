# 1026 Cantina of Babel

来源：https://nanti.jisuanke.com/t/44119

$tag:$ `tarjan` `字符串` `极大强连通分量` 

## 题目大意

有 $n(1≤n≤100)$ 个人，每个人都会说一种语言，还理解 $m(0≤m≤20)$ 种其他语言，当然了，每个人也理解他自己说的语言。如果 $a$ 理解 $b$ 说的语言，那就称 $a$ 理解 $b$ ，理解具有传递性，即 $a$ 理解 $b$ ，$b$ 也理解 $c$ ，则有 $a$ 理解 $c$ 。如果有 $x$ 个人，这些人中每个人都可以理解其余的人，则称他们相互理解。如果两个人之间不理解，他们有可能开战。现在这 $n$ 个人开会，请问至少剔除几个人，才能使得剩下的人都相互理解？



## 题目样例

### 样例输入

```
6
Fran French Italian
Enid English German
George German Italian
Ian Italian French Spanish
Spencer Spanish Portugese
Polly Portugese Spanish
```

### 样例输出

```
4
```

### 样例解释

Fran 和 Ian 相互理解，Polly 和 Spencer 相互理解，其他所有人都不能相互理解。

因此要么把除了Fran 和 Ian以外的人都剔除掉，或者把除了Polly 和 Spencer意外的人都剔除掉以外别无他法。

最后最多剩 $2$ 个人，也就是最少剔除 $4$ 个人。



## 题目解法

如果 $a$ 能理解 $b$，即 $a$ 的理解列表里有 $b$ 说的语言，就建一条有向边 $(a,b)$。

如果图中有一个集合相互可打，也就是对应元素的人相互理解，也就是极大联通分量。

现在的问题就转化成求最大的极大连通分量里的元素个数，然后用总人数减去最大的极大连通分量的元素个数就是答案。

时间复杂度$O(n^2logn)$。



## 完整代码

```c++
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define pb push_back

#define maxn 1003
#define maxe 50003
struct G
{
	int n,m;
	int Next[maxe];
	int head[maxn];
	int ver[maxe];
	int tot;
	void add(int x,int y)
	{
		ver[++tot] = y,Next[tot] = head[x],head[x] = tot;
	}
} g;

//记得初始化g.n和g.m 
struct dirGtarjan
{
	//时间戳和追溯值
	int dfn[maxn], low[maxn];
	//栈，是否在栈中，节点所在强连通分量编号
	int sk[maxn], ins[maxn], c[maxn];
	//强连通分量中的点集
	vector<int> scc[maxn];
	//时间戳累加值，栈顶值，强连通分量个数
	int num, top, cnt;
	//返回强连通分量个数 ,并准备好c和scc数组 
	int tarjan(const G& g)
	{
		_for(x,1,g.n+1)
			if(!dfn[x])
				tj(g,x);
		return cnt;
	}
	void tj(const G& g, int x)
	{
		dfn[x] = low[x] = ++num;
		sk[++top] = x, ins[x] = 1;
		for(int i = g.head[x]; i; i = g.Next[i])
		{
			int y = g.ver[i];
			if(!dfn[y])
			{
				tj(g,y);
				low[x] = min(low[x],low[y]);
			}
			else if(ins[y])
				low[x] = min(low[x],dfn[y]);
		}
		if(dfn[x]==low[x])
		{
			cnt ++;int y;
			do
			{
				y = sk[top--], ins[y] = 0;
				c[y] = cnt, scc[cnt].pb(y);
			}while(x != y);
		}
	} 
	void suo(const G& orig,const dirGtarjan& dgt, G& outg)
	{
		outg.n = dgt.cnt;
		_for(x,1,orig.n+1)
			for(int i = orig.head[x]; i; i = orig.Next[i])
			{
				int y = orig.ver[i];
				if(dgt.c[x] == dgt.c[y])
					continue;
				outg.add(dgt.c[x], dgt.c[y]);
			}
	}
} dgt;

struct strconint
{
	unordered_map<string,int> um;
	vector<string> v;
	string int2str(int x)
	{
		return v[x];
	}
	int str2int(string s)
	{
		if(um.find(s)==um.end())
		{
			v.pb(s);
			um[s] = (int)v.size();
			return (int)v.size();
		}
		else
			return um[s];
	}
} si;

vector<string> split(string str, char del)
{
	stringstream ss(str);
	string tmp;
	vector<string> rnt;
	while(getline(ss, tmp, del))
		rnt.pb(tmp);
	return rnt;
}
//speak[x]为第x个人说的语言
int speak[maxn];
//us[x]为第x个人理解的语言集合
set<int> us[maxn];
int main()
{
	stringstream sstream;
	cin >> g.n;
	getchar();
	_for(i,1,g.n+1)
	{
		string line;
		getline(cin,line);
		vector<string> in = split(line,' ');
		speak[i] = si.str2int(in[1]);
		_for(j,1,in.size())
			us[i].insert(si.str2int(in[j]));
	}
	//i能理解j，则建一条边
	_for(i,1,g.n+1)
		_for(j,1,g.n+1)
			if(i!=j && us[i].find(speak[j])!=us[i].end())
				g.add(i,j);
	//求极大连通分量
	dgt.tarjan(g);
	
	int rnt = 139;
	_for(i,1,dgt.cnt+1)
		rnt = min(rnt,g.n-(int)dgt.scc[i].size());
	cout << rnt;
	return 0;
}
```

