# 1004  Optimal Subsequences (Hard Version) 

来源： https://codeforces.com/contest/1262/problem/D2 

$tag:$ `treap` `离线询问`

## 题目大意

给定一个长度为 $n(1≤n≤200000)$ 的数组，再给出 $m(1≤m≤200000)$ 个询问，每次询问包含一个 $k(1≤k≤n)$，一个 $pos(1≤pos≤k)$，对于每次询问，你需要提取出原数组前 $k$ 大的**子序列**，即选出前 $k$ 大的数以后按照原数组的数的相对位置顺序将这 $k$ 个数排序。接着你需要输出在此子序列中从前往后数第 $pos$ 个数是多少。



## 题目样例

### 样例输入

```
3
10 20 10
6
1 1
2 1
2 2
3 1
3 2
3 3
```

### 样例输出

```
20
10
20
10
20
10
```

### 样例解释

当 $k=1$ 时，子序列为 $[20]$

当 $k=2$ 时，子序列为 $[10,20]$

当 $k=3$ 时，子序列为 $[10,20,10]$

因此对于询问 $(3,2)$，应该输出子序列中第 $2$ 个数 $20$。

对于询问 $(2,2)$，应该输出子序列中第 $2$ 个数 $20$。



## 题目解法：

$treap$ 是一种平衡二叉查找树，支持在 $O(logn)$ 的时间复杂度内完成 **增删改查** 四种操作，其中还可以完成给定 $x$ 查排名，和给定排名查 $x$，因此此题的做法如下

1. 准备原数组 $ori$ ，处理过的数组 $a$，处理过的询问数组 $v$。
2. 数组 $a$ 包含原数组的值 $val$ 和各个值在原数组中的下标 $pos$，按照 $val$ 从大到小排序，若 $val$ 相同则按 $pos$ 从小到大排序，因为这是加入 $treap$ 的顺序。
3. 数组 $v$ 包含询问的要求 $k$，询问内容 $pos$ 和询问是第几个 $id$，按照 $k$ 从小到大排序。
4. 从头到尾预处理询问，$k$ 表示要求当前 $treap$ 内需要的元素个数，注意， $treap$ 内存的是 $a.pos$ ，也就是原数组下标，$treap$ 维护下标后通过查找 第 $pos$ 个元素，也就找到了大小为 $k$ 的子序列的第 $pos$ 个数的下标，则 $ori[pos]$ 就是第 $id$ 个询问的答案。 



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
#define maxn 200003
#define pb push_back
#define debug() printf("Miku Check OK!\n")

struct Tre
{
	struct TreNode
	{
		//左右子节点在数组中的下标
		ll l,r;
		//节点关键码，随机数
		ll val,rd;
		//节点相同树个数，子树大小（含当前根节点）
		ll cnt,size;
#define l(x) tree[x].l
#define r(x) tree[x].r
#define val(x) tree[x].val
#define rd(x) tree[x].rd
#define cnt(x) tree[x].cnt
#define size(x) tree[x].size
	} tree[maxn];
	//节点个数，根节点下标
	ll tot, root;
	Tre()
	{
		New(-INF), New(INF);
		//存入两节点，+oo是-oo的右节点
		root = 1,r(1) = 2;
		//更新根节点大小
		upsize(root);
		srand(time(NULL));
	}
	//整个新节点
	int New(ll val)
	{
		val(++tot) = val;
		rd(tot) = rand();
		//副本数和子树大小都是1
		cnt(tot) = size(tot) = 1;
		//返回 当前节点在数组中的下标
		return tot;
	}
	//更新子树大小
	void upsize(ll p)
	{
		size(p) = size(l(p)) + size(r(p)) + cnt(p);
	}
	//根据val定排名
	ll getrankbyval(ll p,ll val)
	{
		//空节点
		if(!p) return 0;
		//找到了，返回左子树大小+1
		if(val == val(p))
			return size(l(p)) + 1;
		//要找的在左子树
		if(val < val(p))
			return getrankbyval(l(p),val);
		//在右子树,则左子树节点和当前节点的排名都不如该值
		return getrankbyval(r(p),val) + size(l(p)) + cnt(p);
	}
	//根据排名找val
	ll getvalbyrank(ll p,ll rank)
	{
		//空节点
		if(!p) return INF;
		//没找到 ，要找的排名在左子树
		if(size(l(p)) >= rank)
			return getvalbyrank(l(p),rank);
		//找到了
		if(size(l(p)) + cnt(p) >= rank)
			return val(p);
		//要找的排名在右子树
		return getvalbyrank(r(p),rank - size(l(p)) - cnt(p));
	}
	//右旋
	void zig(ll &p)
	{
		ll q = l(p);
		l(p) = r(q), r(q) = p, p = q;
		upsize(r(p)), upsize(p);
	}
	//左旋
	void zag(ll &p)
	{
		ll q = r(p);
		r(p) = l(q), l(q) = p, p = q;
		upsize(l(p)), upsize(p);
	}
	void insert(ll &p, ll val)
	{
		//空树
		if(!p)
		{
			p = New(val);
			return ;
		}
		//之前已经有这个数了
		if(val == val(p))
		{
			cnt(p) ++, upsize(p);
			return ;
		}
		//该放入左子树
		if(val < val(p))
		{
			insert(l(p),val);
			//玄学右旋
			if(rd(p) < rd(l(p)))
				zig(p);

		}
		//放入右子树
		else
		{
			insert(r(p),val);
			//玄学左旋
			if(rd(p) < rd(r(p)))
				zag(p);
		}
		//更新子树大小
		upsize(p);
	}
	//找关键码小于 val 的前提下，val最大的节点大小
	ll getpre(ll val)
	{
		//val(1) == -INF,ans 为要找的节点的下标
		ll ans = 1;
		ll p = root;
		while(p)
		{
			if(val == val(p))
			{
				if(l(p))
				{
					p = l(p);
					//找左子树上最右节点
					while(r(p))
						p = r(p);
					ans = p;
				}
				break;
			}
			//更新节点
			if(val(p) < val && val(p) > val(ans))
				ans = p;
			p = val < val(p) ? l(p) : r(p);
		}
		//可能返回-INF
		return val(ans);
	}
	//找关键码大于 val 的前提下，val最小的节点大小
	ll getnext(ll val)
	{
		//val(2) == INF,ans 为要找的节点的下标
		ll ans = 2;
		ll p = root;
		while(p)
		{
			if(val == val(p))
			{
				if(r(p))
				{
					p = r(p);
					//找右子树上最左节点
					while(l(p))
						p = l(p);
					ans = p;
				}
				break;
			}
			//更新节点
			if(val(p) > val && val(p) < val(ans))
				ans = p;
			p = val < val(p) ? l(p) : r(p);
		}
		//可能返回-INF
		return val(ans);
	}
	//删除节点
	void remove(ll &p,ll val)
	{
		if(!p)
			return ;
		//逮住你了，栽种
		if(val == val(p))
		{
			//小样你还整替身哈
			if(cnt(p) > 1)
			{
				//死一个替身
				cnt(p) --,upsize(p);
				return ;
			}
			//非叶子节点
			if(l(p) || r(p))
			{
				//只有左子树 或者该转了 ,转完再找到节点删
				if(!r(p) || rd(l(p)) > rd(r(p)))
					zig(p),remove(r(p),val);
				else
					zag(p),remove(l(p),val);
				upsize(p);
			}
			//叶子节点直接删
			else
				p = 0;
			return ;
		}
		//没找到，继续找
		val < val(p) ? remove(l(p),val) : remove(r(p),val);
		upsize(p);
	}
} T1;

ll n, m;
//离线询问 
struct ask
{
	ll k;
	ll pos;
	ll id;
	bool operator < (ask b)
	{
		return k < b.k;
	}
};
struct d
{
	//原来的值和在原数组中的位置 
	ll val;
	ll pos;
	bool operator < (d b)
	{
		if(val != b.val)
			return val > b.val;
		return pos < b.pos;
	}
};
//需要依次进树的经过处理的数组 
vector<d> a;
//询问数组 
vector<ask> v;
//最后的答案 
ll ans[200003];
//原数组 
vector<ll> ori;
int main()
{
	scanf("%lld",&n);
	_for(i,0,n)
	{
		ll t;
		scanf("%lld",&t);
		ori.pb(t);
		a.pb({t,i});
	}
	//根据数值大小从大到小排序 
	sort(a.begin(),a.end());
	
	scanf("%lld",&m);
	_for(i,0,m)
	{
		ll k, pos, id;
		id = i;
		scanf("%lld%lld",&k,&pos);
		v.pb({k,pos,id});
	}
	//根据k值从小到大对询问排序 
	sort(v.begin(),v.end());
	int index = 0;
	_for(i,0,m)
	{
		while(T1.size(T1.root)-2 < v[i].k)
			T1.insert(T1.root,a[index++].pos);
		ans[v[i].id] = T1.getvalbyrank(T1.root,v[i].pos+1);
	}
	_for(i,0,m)
		printf("%lld\n",ori[ans[i]]);
	return 0;
}
```

