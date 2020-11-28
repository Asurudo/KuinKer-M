# 2-SAT

对于某一个布尔表达式，如 $(¬a∨b)$ ，我们可以将其转化为 $a→b∧¬b→¬a$ ，代表 $a$ 若真，$b$ 则必须真，反之亦然，因为想要这个布尔表达式为真，则 $∨$ 左右两边至少要真一个。对于每个箭头(蕴含)，可以对应的连一条有向边，若 $x$ 与 $¬x$ 在同一强连通分量中，说明可以从 $x$ 推到 $¬x$ 或者 $¬x$ 推到 $x$ ，则 $2-SAT$ 不成立，否则对于形成的有向图，可以用 $tarjan$ 染色，若颜色的字典序较大，则说明可以被推到，而 $tarjan$ 染色已经是倒序了，因此若真的字典序小，该变量就为真，否则为假。

记得开两倍空间，时间复杂度 $O(n+m)$，详情见 P419。

```c++
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f3f3f3f3f
#define mod 1000000007
#define lowbit(x) ((x)&(-x))
#define pb push_back
#define MIKU 39
#define Design ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define debug() printf("Miku Check OK!\n")

//最大点数
#define maxn 2000039
//最大边数
#define maxe 2000039
struct G
{
	//点数和边数
	int n,m;
	//存储某一条边的后一条边
	int Next[maxe];
	//存储边的起始信息，若有x ->(w) y，则head[x]为第一条到达边的编号
	int head[maxn];
	//存储边的到达信息 ，即 x ->(w) y中的y
	int ver[maxe];
	//有向边的总数
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

struct dirGtarjan
{
	//时间戳和追溯值
	int dfn[maxn], low[maxn];
	//栈，是否在栈中，节点所在强连通分量编号
	int sk[maxn], ins[maxn], c[maxn];
	//时间戳累加值，栈顶值，强连通分量个数
	int num, top, cnt;
	//返回强连通分量个数 ,并准备好c和scc数组
	int tarjan(const G& g)
	{
		_for(x,1,g.n*2+1)
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
			cnt ++;
			int y;
			do
			{
				y = sk[top--], ins[y] = 0;
				c[y] = cnt;
			}
			while(x != y);
		}
	}
} dgt;

int main()
{
	scanf("%d%d",&g.n,&g.m);
	const int base = g.n;
	_for(x,1,g.m+1)
	{
		int i, a, j ,b;
		scanf("%d%d%d%d",&i,&a,&j,&b);
		if(!a && b)
		{
			g.add(i,j);
			g.add(j+base,i+base);
		}
		else if(a && !b)
		{
			g.add(i+base,j+base);
			g.add(j,i);
		}
		else if(a && b)
		{
			g.add(i+base,j);
			g.add(j+base,i);
		}
		else
		{
			g.add(i,j+base);
			g.add(j,i+base);
		}
	}
	dgt.tarjan(g);
	int flag = 1;
	_for(i,1,g.n+1)
		if(dgt.c[i]==dgt.c[i+base])
			flag = 0;
	if(!flag)
	{
		printf("IMPOSSIBLE");
		return 0;
	}
	else
		printf("POSSIBLE\n");

	_for(i,1,g.n+1)
		printf("%d ",dgt.c[i] < dgt.c[i+base]);
	return 0;
}
```

