# 1007 Editor

来源：https://codeforces.com/contest/1263/problem/E

$tag:$ `字符串` `括号匹配` `前缀和` `线段树` `模拟` 

## 题目大意

给出一串长度为 $n(1≤n≤10^6)$ 的对于一行文本的操作指令，**一开始游标在最左端**，现在指令集合中包含如下指令:

1. $R$ 表示游标向右移动一格，$L$ 表示游标向左移动一格，若当前游标已在最左端则不用移动
2. 一些输入，可能包含小写字母和双括号，如果当前位置已有字符就进行覆盖

根据这些指令，你可以在一行文本中逐步敲出一串字符串，现在的问题是，对于每条指令，当前已有的字符串的括号是否匹配？若匹配，输出最大的匹配深度，若不匹配，输出 $-1$ 。



## 题目样例

### 样例输入

```
11
(RaRbR)L)L(
```

### 样例输出

```
-1 -1 -1 -1 -1 -1 1 1 -1 -1 2 
```

### 样例解释

对于每步指令，得到的字符串和光标情况如下：

![TIM截图20191130162953](https://s2.ax1x.com/2019/11/30/QVc2Hf.jpg)

## 题目解法

对于一串字符串是否匹配，将左括号视为 $+1$，右括号视为 $-1$ ，其余无关字符视为 $0$，得到一个整数数组，其匹配的充要条件是：

1. 该数组任意位置前缀和 $presum_i≥0(1≤i≤n)$。
2. 该数组的最后一位前缀和 $presum_n==0$ 。

满足该条件则匹配，不满足则不匹配，且最大的匹配深度为 $max(presum_i)(1≤i≤n)$ 。

则我们可以考虑使用 **区间更新·区间查询** 的 **线段树** 维护其前缀和 $presum$ ，对 $[i,n]$ 的修改即为修改其前缀和，例如若第 $i$ 位从右括号换成了左括号，则将 $[i,n]$ 这一区间加上 $2$ ，这样该前缀和数组就能得以维护。

对于每一个操作，分类讨论进行相应的更新以后，用上述的 $2$ 个充要条件检验该字符串整体是否合法（括号全都匹配），若不合法直接输出 $-1$ ，若合法则输出最大匹配深度，依然是用线段树 $ask4max(1,1,n)$ 即可。



## 完整代码

```c++
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<ll,ll> P;

#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x7fffffff
#define MOD 100000000
#define maxn 1000003
#define pb push_back
#define debug() printf("Miku Check OK!\n")

int n, m;
struct SegTree
{
	struct SegNode
	{
		//l,r代表的是该节点表示的原数组下标 
		int l,r;
		//add为该区间懒惰标记 
		ll add;
		//sum,max和min分别为原数组对应区间的和，最大值以及最小值 
		ll max,min;
		//x为线段树内部下标 
		#define l(x) tree[x].l
		#define r(x) tree[x].r
		#define add(x) tree[x].add
		#define MAX(x) tree[x].max
		#define MIN(x) tree[x].min
	}tree[maxn<<2];
	void build(int p,int l,int r)
	{
		//p为线段树内部下标 
		l(p) = l,r(p) = r;
		//到达叶子节点 
		if(l==r)
		{
			MAX(p) = MIN(p) = 0;
			return ;
		}
		int mid = (l+r)/2;
		//递归构造左右节点 
		build(p*2, l, mid);
		build(p*2+1, mid+1, r);
		//左右节点维护完后开始维护父节点 
		MAX(p) = max(MAX(p*2),MAX(p*2+1));
		MIN(p) = min(MIN(p*2),MIN(p*2+1));
	}
	//懒惰标记下放 
	void spread(int p)
	{
		if(add(p))
		{
			//管控节点个数乘以增值为当前节点增值 
			//最值加上增值即为当前最值 
			MAX(p*2) += add(p);
			MAX(p*2+1) += add(p);
			MIN(p*2) += add(p);
			MIN(p*2+1) += add(p); 
			//更新左右节点懒惰标记 
			add(p*2) += add(p);
			add(p*2+1) += add(p);
			//当前节点懒惰标记下放完成 
			add(p) = 0;
		} 
	}
	void change(int p,int l,int r,ll d)
	{
		//要修改区间完全包住当前节点区间 
		if(l <= l(p) && r >= r(p))
		{
			MAX(p) += d;
			MIN(p) += d;
			add(p) += d;
			return ;
		} 
		//否则下放当前节点懒惰标记 
		spread(p);
		int mid = (l(p)+r(p))/2;
		/*如果要修改的区间左端点比当前节点代表区间的mid小则说明涉及到了
		左区间*/ 
		if(l <= mid)
			change(p*2, l, r, d);
		//右端同理 
		if(r > mid)
			change(p*2+1, l, r, d);
		MAX(p) = max(MAX(p*2),MAX(p*2+1));
		MIN(p) = min(MIN(p*2),MIN(p*2+1));
	} 
	ll ask4max(int p,int l,int r)
	{
		//要修改区间完全包住当前节点区间 
		if(l <= l(p) && r >= r(p))
			return MAX(p);
		spread(p);
		int mid = (l(p)+r(p))/2;
		ll val = -INF;
		//分别查询左右节点最大值 
		if(l <= mid)
			val = max(val,ask4max(p*2, l, r));
		if(r > mid)
			val = max(val,ask4max(p*2+1, l, r));
		return val;
	}
	ll ask4min(int p,int l,int r)
	{
		//要修改区间完全包住当前节点区间 
		if(l <= l(p) && r >= r(p))
			return MIN(p);
		spread(p);
		int mid = (l(p)+r(p))/2;
		ll val = INF;
		//分别查询左右节点最大值 
		if(l <= mid)
			val = min(val,ask4min(p*2, l, r));
		if(r > mid)
			val = min(val,ask4min(p*2+1, l, r));
		return val;
	}
}T;

char in[maxn];
char now[maxn];
int main()
{
	scanf("%d",&n);
	T.build(1,1,n);
	scanf("%s",in); 
	int cur = 1;
	_for(i,0,n)
		now[i] = '?';
	_for(i,0,n)
	{
		//坑点，游标在最左端时不能再向左端移动 
		if(in[i]=='L'&&cur==1)
			;
		//游标左右移动 
		else if(in[i]=='L')
			cur --;
		else if(in[i]=='R')
			cur ++;
		//对当前位置的字符进行分类讨论 
		else if(now[cur]=='?')
		{
			now[cur] = in[i];
			if(in[i]=='(')
				T.change(1,cur,n,1);
			else if(in[i]==')')
				T.change(1,cur,n,-1);	
		}
		else if(now[cur]=='(')
		{
			now[cur] = in[i];
			if(in[i]==')')
				T.change(1,cur,n,-2);
			else if(in[i]!='(')
				T.change(1,cur,n,-1);
		}
		else if(now[cur]==')')
		{
			now[cur] = in[i];
			if(in[i]=='(')
				T.change(1,cur,n,2);
			else if(in[i]!=')')
				T.change(1,cur,n,1);
		}
		else
		{
			now[cur] = in[i];
			if(in[i]=='(')
				T.change(1,cur,n,1);
			else if(in[i]==')')
				T.change(1,cur,n,-1);
		}
		//若不满足字符串合法的2个充要条件 
		if(T.ask4min(1, n, n)!=0 || T.ask4min(1, 1, n)<0)
			printf("-1 ");
		//输出最大匹配深度 
		else
			printf("%lld ",T.ask4max(1,1,n));
	}
	return 0;
}
```

