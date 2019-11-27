# Treap

## 例题编号：

**1004**

------

$Treap$ 是一种满足二叉查找树性质且较为平衡的数据结构，使用随机生成的值让整棵树不断左旋右旋以达到平衡，所以关于随机值的部分**不要进行改动**，随机值只管平衡的问题，和二叉树的功能没有任何关系。

关于旋转的图示如下，理解旋转后就不难理解删除操作中的一部分代码了。

![](C:\Users\Asurudo Jyo\Desktop\TIM截图20191127090846.jpg)

其它插入删除等操作和二叉查找树相同，详见注释。

刚开始插入了两个节点，一个 $INF$，一个 $-INF$，数组下标编号分别为 $2$ 和 $1$，所以 $size(root)$ 一开始就是 $2$。

查询排名时是 $1,2,3 ... ...$ ，记得 $+1$ 或者 $-1$ 详情见模板代码。

支持的功能如下：

1. 插入数值 $x$
2. 删除数值 $x$（若有多个相同的数，应只删除一个）
3. 查询数值 $x$ 的排名（若有多个相同的数，应输出最小的排名）
4. 查询排名为 $x$ 的数值
5. 求数值 $x$ 的前驱（前驱定义为小于 $x$ 的最大的数，有可能是 $-INF$）
6. 求数值 $x$ 的后继（后继定义为大于 $x$ 的最小的数，有可能是 $INF$ ）

```c++
#include <bits/stdc++.h>

using namespace std;

typedef pair<int,int> P;
typedef long long ll;
#define _for(i,a,b) for(register int i = (a);i < b;i ++)
#define _rep(i,a,b) for(register int i = (a);i > b;i --)
#define INF 0x7fffffff
#define MOD 100000000
#define maxn 100003
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

int main()
{
	ll n;
	scanf("%lld",&n);
	while(n--)
	{
		ll op, x;
		scanf("%lld%lld",&op,&x);
		if(op==1)
			T1.insert(T1.root,x);
		else if(op==2)
			T1.remove(T1.root,x);
		else if(op==3)
			printf("%lld\n",T1.getrankbyval(T1.root,x)-1);
		else if(op==4)
			printf("%lld\n",T1.getvalbyrank(T1.root,x+1));
		else if(op==5)
			printf("%lld\n",T1.getpre(x));
		else
			printf("%lld\n",T1.getnext(x));
	}
	return 0;
}
```

