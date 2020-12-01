# dfs序

```c++
struct dfstree
{
	int pos = 0, opos = 0;
	int L[maxn], R[maxn], re[maxn];
    //oula[x]:每个结点的起始（第一个）欧拉序列 ore[i]:第i个欧拉序列对应的结点
    int ore[maxn*2], oula[maxn*2];
	void init(const G& g,int x)
	{
		_init(x,0);
	}
	void _init(int x,int fa)
	{
		L[x] = ++pos;
        ore[++opos] = x;
        oula[x] = opos;
		re[pos] = x;
		for(int i = g.head[x]; i; i = g.Next[i])
		{
			int y = g.ver[i];
			if(y==fa)
				continue;
			_init(y,x);
            ore[++opos] = x;
		}
		R[x] = pos;
	}
} dte;
```

