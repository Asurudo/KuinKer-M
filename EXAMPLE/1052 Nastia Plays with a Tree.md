# 1052  Nastia Plays with a Tree

**来源**： http://codeforces.com/contest/1521/problem/D

$tag$ : `贪心` `最小路径覆盖` 



## 题目大意

给你一个 $n(1≤n≤10^5)$ 个结点的树，你每次操作需要：

1.  删除一条边
2.  加上一条边

请输出经过最少多少次操作后，这棵树会变成一条链。



## 题目样例

### 样例输入

```
2
7
1 2
1 3
2 4
2 5
3 6
3 7
4
1 2
1 3
3 4
```

### 样例输出

```
2
2 5 6 7
3 6 4 5
0
```

### 样例解释

![img](https://espresso.codeforces.com/b7e8970210ecaf417c347c8cc56a296aa22a5c76.png)

红色边为要删除的边，绿色边为要增加的边。

## 题目解法

将该树上的最小路径覆盖计算出，并且统计出每条路径的两端，将这些路径首尾相连就是答案。

难点在于统计答案，主要有以下两种情况：

1. 当前结点 $x$ 已经成为拐点，那么就需要删掉 $x$ 与其父亲相连的边，两个儿子返回上来的点作为该链的两端。
2. 当前结点 $x$ 没有成为拐点，那就交给父亲处理。

因为交给父亲处理，所以如果：

1. 当前结点已经成为拐点，则其所有儿子（除了一条链上的两个），那么就需要删掉 $x$ 与儿子相连的边，将儿子返回上来的点和儿子作为该链的两端。
2. 当前节点没有成为拐点，则只有一个儿子，因此继续向上交由父亲处理。

时间复杂度 $O(n)$。

## 完整代码

```c++
/*Hatsune Miku 4ever!*/
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

#define maxn 100003
#define maxe 250003
struct G
{
	int n,m;
	int Next[maxe];
	int head[maxn];
	int ver[maxe];
	int val[maxe];
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

vector<pair<int,int>> del;
//链的两端
vector<pair<int,int>> line;
struct mincocline
{
	//size[x]表示以x为根的子树的最小链数量
	int size[maxn];
	//rot[x]=1代表x为拐点
	int rot[maxn];
	void clear(int x)
	{
		memset(size,0,(x+3)*sizeof(int));
		memset(rot,0,(x+3)*sizeof(int));
	}
	int getline(int x,int fa)
	{
		size[x] = 1;
		int cnt = 0;
        //分别装非旋转点的儿子的一端和儿子
		vector<int> re, er;
		for(int i = g.head[x]; i; i = g.Next[i])
		{
			int y = g.ver[i];
			if(y==fa)
				continue;
			int tp = getline(y,x);
			size[x] += size[y];
			if(!rot[y])
			{
				assert(tp!=-1);
				cnt ++;
                //确定该点已经成为旋转点
				if(cnt==2)
				{
					del.pb({x,fa});
					line.pb({re[0],tp});
					er.clear();
					re.clear();
				}
				else
					re.pb(tp), er.pb(y);
			}
		}
		assert(re.size()==er.size());
		//x为拐点
		if(cnt>=2)
		{
			size[x] -= 2;
			rot[x] = true;
			_for(i,0,re.size())
			{
				del.pb({x,er[i]});
				line.pb({er[i],re[i]});
			}
			return -1;
		}
		//x不为拐点，一起向上
		else if(cnt==1)
		{
			size[x] --;
			return re[0];
		}
		return x;
	}
} mcl;

int main()
{
	int _;
	scanf("%d",&_);
	while(_--)
	{
		scanf("%d",&g.n);
		g.m = g.n-1;
		del.clear();
		line.clear();
		_for(i,1,g.m+1)
		{
			int x, y;
			scanf("%d%d",&x,&y);
			g.add(x,y);
			g.add(y,x);
		}
		mcl.getline(1,-1);
		printf("%d\n",mcl.size[1]-1);
		int pos = -1, lian;
		_for(i,0,del.size())
			if(del[i].first==-1 || del[i].second==-1)
			{
				pos = i;
				lian = line[i].first;
				break;
			}
		if(pos==-1)
			lian = 1;
		_for(i,0,del.size())
		{
			if(i==pos)
				continue;
			printf("%d %d %d %d\n",del[i].first,del[i].second,lian,line[i].first);
			lian = line[i].second;
		}
		g.clear(g.n);
		mcl.clear(g.n);
	}
	return 0;
}
```

