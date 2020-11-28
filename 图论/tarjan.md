# Tarjan

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
                outg.m ++;
			}
	}
} dgt;
```



## 无向图

### 桥 && 边双连通分量

```c++
//记得初始化g.n和g.m，保证链式前向星的tot=1,++后从2开始！
struct udirGedgetarjan
{
	//时间戳和追溯值
	int dfn[maxn], low[maxn];
	//时间戳累加值
	int num, root;
	//是否是桥
	bool bridge[maxe];
	vector<pair<int,int>> edge;
	//c[x]是x所属边双编号，edcc是边双总数
	int c[maxn], edcc;
	//边双联通的点集
	vector<int> eDCC[maxn];
	vector<pair<int,int>> tarjanedge(const G& g)
	{
		_for(x,1,g.n+1)
		if(!dfn[x])
			tjeg(g,x,0);
		for(int i = 2; i < g.tot; i += 2)
		{
			if(bridge[i])
				edge.pb({g.ver[i],g.ver[i^1]});
		}
		_for(i,1,g.n+1)
		if(!c[i])
		{
			edcc ++;
			getedcc(g,i);
		}
		return edge;
	}
	void tjeg(const G& g, int x,int in_edge)
	{
		dfn[x] = low[x] = ++num;
		for(int i = g.head[x]; i; i = g.Next[i])
		{
			int y = g.ver[i];
			if(!dfn[y])
			{
				tjeg(g,y,i);
				low[x] = min(low[x],low[y]);

				if(low[y] > dfn[x])
					bridge[i] = bridge[i^1] = true;
			}
			else if(i != (in_edge^1))
				low[x] = min(low[x],dfn[y]);
		}
	}
	void getedcc(const G& g,int x)
	{
		c[x] = edcc;
		eDCC[edcc].pb(x);
		for(int i = g.head[x]; i; i = g.Next[i])
		{
			int y = g.ver[i];
			if(c[y] || bridge[i])
				continue;
			getedcc(g,y);
		}
	}
	//先跑tarjan和getedcc
	void eDCCsuo(const G &orig, G& outg)
	{
		outg.n = edcc;
		outg.tot = 1;
		_for(x,1,orig.n+1)
		for(int i = orig.head[x]; i; i = orig.Next[i])
		{
			int y = orig.ver[i];
			if(c[x] == c[y])
				continue;
			outg.add(c[x], c[y]);
			outg.m ++;
		}
	}
} uedgt;
```



### 割点 && 点双连通分量

```c++
//记得初始化g.n和g.m，保证链式前向星的tot=1,++后从2开始！
//割点去一下自环
struct udirGnodetarjan
{
	//时间戳和追溯值
	int dfn[maxn], low[maxn], stack[maxn];
	//时间戳累加值
	int num, root, top;
	//是否是割点
	bool cut[maxn];
	vector<int> point;
	//c[x]是x所属点双编号，vdcc是点双总数
	int c[maxn], vdcc;
	//点双联通的点集
	vector<int> vDCC[maxn];
	//缩点后,old_id[x]表示缩点后的图的该割点在原来的图中的编号 
	int old_id[maxn];
	//返回割点
	void clear(int x)
	{
		_for(i,0,x+2)
			vDCC[i].clear();
		num = root = top = vdcc = 0;
		memset(cut,0,sizeof(cut));
		memset(dfn,0,sizeof(dfn));
		memset(low,0,sizeof(low));
		memset(stack,0,sizeof(stack));
	}
	vector<int> tarjanpt(const G& g)
	{
		_for(x,1,g.n+1)
		if(!dfn[x])
			root = x, tjpt(g,x);
		_for(i,1,g.n+1)
		{
			if(cut[i])
				point.pb(i);
		}
		return point;
	}
	void tjpt(const G& g, int x)
	{
		dfn[x] = low[x] = ++num;
		stack[++top] = x;
		if(x == root && !g.head[x])
		{
			vDCC[++vdcc].pb(x);
			c[x] = vdcc;
			return ;
		}
		int flag = 0;
		for(int i = g.head[x]; i; i = g.Next[i])
		{
			int y = g.ver[i];
			if(!dfn[y])
			{
				tjpt(g,y);
				low[x] = min(low[x],low[y]);

				if(low[y] >= dfn[x])
				{
					flag ++;
					if(x != root || flag > 1)
						cut[x] = true;
					vdcc ++;
					int z;
					do
					{
						z = stack[top--];
						vDCC[vdcc].pb(z);
						c[z] = vdcc;
					}while(z != y);
						vDCC[vdcc].pb(x), c[x] = vdcc;
				}
			}
			else
				low[x] = min(low[x],dfn[y]);
		}
	}
	//先跑tarjan
	void vDCCsuo(const G &orig, G& outg)
	{
		int new_id[maxn];
		outg.n = vdcc;
		_for(i,1,orig.n+1)
		{
			if(cut[i])
			{
				new_id[i] = ++outg.n;
				old_id[outg.n] = i;
			}
		}
		outg.tot = 1;
		_for(i,1,vdcc+1)
		_for(j,0,vDCC[i].size())
		{
			int x = vDCC[i][j];
			if(cut[x])
			{
				outg.add(i, new_id[x]);
				outg.add(new_id[x], i);
				outg.m ++;
			}
		}
	}
} undgt;
```

