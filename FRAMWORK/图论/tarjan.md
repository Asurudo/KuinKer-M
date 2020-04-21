# Tarjan

## 例题编号

**1026**

------





## 有向图

tarjan 算法在找有向图强连通分量的时候，其实是在找环。根据时间戳的先后到达一个点的时候，所关心的是这个点有没有边能使他到达时间戳靠前的他的祖先，或者到达一个能到达他祖先的点。所以我们就需要维护一个栈，所以当我们遍历到节点 $x$ 的时候，这个栈里包括

1. 该点的祖先节点
2. 已经访问过，且不属于其他强连通分量的能到达该点祖先节点的点

因为这些都是可能在节点 $x$ 的强连通分量内的点，接下来访问到的点只要能够到达这两种节点，就能形成环，从而形成强连通分量。

节点 $x$ 的追溯值是指点 $x$ 所能访问到的最小的祖先节点的时间戳。

tarjan算法按照以下步骤计算时间戳：

1. 当节点 $x$ 第一次被访问时，把 $x$ 入栈后，初始化 $low[x] = dfn[x]$。

2. 扫描从 $x$ 出发的 $(x,y)$:

   (1) 若 $y$ 没被访问过，则 $y$ 为 $x$ 的儿子，回溯后 $low[x]=min(low[x],low[y])$。

   (2) 若 $y$ 被访问过且不在栈中，说明 $y$ 在其他强连通分量里，不用管；若访问过且在栈里，说明通过 $y$ 可以到达 $x$ 的祖先，因此 $low[x] = min(low[x],dfn[y])$。

3. 从 $x$ 回溯前判断是否有 $dfn[x]=low[x]$，如果是，说明点 $x$ 是该强连通分量的最小祖先，不断出栈直到遇到 $x$ 为止，出栈的这些节点形成一个强连通分量。

以下程序中， $c[x]$ 表示了 $x$ 所在的强连通分量编号，$scc[i]$ 表示编号为 $i$ 的强连通分量具体都有哪些点，整张图共有 $cnt$ 个强连通分量。

若对于原图中的每条有向边 $(x,y)$ ，$c[x]≠c[y]$ ，则我们可以在新图上建边进行缩点。

时间复杂度 $O(n+m)$。

```c++
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
	void suo(const G &orig,const dirGtarjan& dgt, G& outg)
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
```

