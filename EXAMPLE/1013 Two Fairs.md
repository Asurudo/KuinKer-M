# 1013 Two Fairs

来源：https://codeforces.com/contest/1277/problem/E

$tag:$ `并查集` `图论`

## 题目大意

给定一张有 $n(4≤n≤2×10^5)$ 个点，$m(n-1≤m≤5×10^5)$ 条无向边的图，保证图一定联通。先给定两点 $a$ ，$b(1≤a,b≤n,a≠b)$ ，问图中存在多少点对$(x,y)(x≠a,x≠b,y≠a,y≠b)$ 使得从 $x$ 到 $y$ 必须经过两点 $a,b$ （经过顺序无关）？注意，$(x,y)$ 和 $(y,x)$ 只算一次。



## 题目样例

第一行为测试样例组数

接下来每个样例，第一行为 $n,m,a,b$ ，然后接下来 $m$ 行是无向边。

### 样例输入

```
3
7 7 3 5
1 2
2 3
3 4
4 5
5 6
6 7
7 5
4 5 2 3
1 2
2 3
3 4
4 1
4 2
4 3 2 1
1 2
2 3
4 1
```

### 样例输出

```
4
0
1
```

### 样例解释

对于第一组样例，存在 $(1,6),(1,7),(2,6),(2,7)$ 四组点对。

对于第二组样例，$(1,4)$ 可以不经过所有两点即可到达，因此答案为 $0$。

对于第三组样例，存在 $(4,3)$ 一组点对。



## 题目解法

因为图是保证联通的，所以若去掉 $a,b$ 点及其连边，无法到达的相互两点就是必须要经过 $a,b$ 才能互相到达的点对。我们在去掉 $a,b$ 点后，能得到一些连通块。连通块有三种类型，即块内本来可直接到达 $a$ 的连通块，本来可直接到达 $b$ 的连通块和本来 $a,b$ 都直接可达的连通块。我们的答案就是第一种连通块块内点总数乘以第二种连通块内块内点总数，第三种连通块不必考虑，因为块内的点不光在块内可不必经过两点就能到达，块外也只需要经过一点即可到达。而如果一个块本来只能直接到达 $a$ ，又因为图联通，所以肯定是只能通过 $a$ 到达 $b$，另一种同理，因此答案就是这两种类型连通块块内点数之积。注意开 $long$ $long$ ！

## 完整代码

```C++
#include <bits/stdc++.h>
 
using namespace std;
typedef long long ll;
typedef pair<ll,ll> P;
#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x3f3f3f3f
#define ZHUO 1000000007
#define pb push_back
#define debug() printf("Miku Check OK!\n")
 
#define maxn 510003
#define _for(i,a,b) for(register int i = (a);i < b;i ++)
struct Djs
{
	int par[maxn];
	int high[maxn];
	int sz[maxn];
	int toA[maxn],toB[maxn];
	void clear(int n)
	{
		memset(toA,0,(n+2)*sizeof(int));
		memset(toB,0,(n+2)*sizeof(int));
		_for(i,1,n+1)
		{
			par[i] = i;
			high[i] = 0;
			sz[i] = 1;
		}
	}
	Djs()
	{
		_for(i,1,maxn)
		{
			par[i] = i;
			high[i] = 0;
			sz[i] = 1;
		}
	}
	int find(int x)
	{
		//路径压缩 
		return par[x] == x ? x : par[x] = find(par[x]);
	}
	void unite(int x,int y)
	{
		x = find(x);
		y = find(y);
		if(x==y) return ;
        //若之前未记录以 x 为根的树与 y连接后的大小，则更新，y为根同理
		int szx = sz[x];
		int szy = sz[y];
		sz[x] += szy;
		sz[y] += szx;
		//按秩合并 
		if(high[x]<high[y])
			par[x] = y;
		else
		{
			par[y] = x;
			if(high[x]==high[y])
				high[x] ++;
		}
	}
	bool same(int x,int y)
	{
		return find(x) == find(y);
	}
}djs;
int n, m, a, b;
 
int main()
{
	int t;
	scanf("%d",&t);
	while(t--)
	{
		scanf("%d%d%d%d",&n,&m,&a,&b);
		_for(i,0,m)
		{
			int x, y;
			scanf("%d%d",&x,&y);
            //一个为a，一个不为b，说明只连a不连b，标记上
			if(x==a && y!=b)
				djs.toA[y] = 1;
			else if(y==a && x!=b)
				djs.toA[x] = 1;
            //同上
			else if(x==b && y!=a)
				djs.toB[y] = 1;
			else if(y==b && x!=a)
				djs.toB[x] = 1;
            //若两个都不是a,b，则将他们连起来，处于同一连通块
			else if(y!=b && x!=a && y!=a && x!=b)
				djs.unite(x,y);
		}
		
		_for(i,1,n+1)
		{
			//如果一个点和A或B连，则他的最终根也与A或B连，具有传递性
            if(djs.toA[i])
				djs.toA[djs.find(i)] = 1;
			if(djs.toB[i])
				djs.toB[djs.find(i)] = 1;
		}
		
		ll cnta = 0,cntb = 0;
		_for(i,1,n+1)
		{
			//如果i为根且只跟a或b连，则就是题解中只与一点相连的连通块（以i为代表）
            if(djs.par[i]==i && djs.toA[i] && !djs.toB[i])
				cnta += djs.sz[i];
			else if(djs.par[i]==i && djs.toB[i] && !djs.toA[i])
				cntb += djs.sz[i];
		}
		printf("%lld\n",cnta*cntb);
		djs.clear(n);
	}
	return 0;
}
```

