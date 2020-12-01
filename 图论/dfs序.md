# dfs序

```c++
struct dfstree
{
	int pos = 0;
	int L[maxn], R[maxn], re[maxn];
	void init(const G& g,int x)
	{
		_init(x,0);
	}
	void _init(int x,int fa)
	{
		L[x] = ++pos;
		re[pos] = x;
		for(int i = g.head[x]; i; i = g.Next[i])
		{
			int y = g.ver[i];
			if(y==fa)
				continue;
			_init(y,x);
		}
		R[x] = pos;
	}
} dte;
```

