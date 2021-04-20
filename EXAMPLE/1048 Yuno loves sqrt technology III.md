# 1048 Yuno loves sqrt technology III

**来源**： https://www.luogu.com.cn/problem/P5048

$tag$ : `分块` 



## 题目大意

给你一个长为 $n$ 的序列 $a$，$m(1≤n,m≤10^5)$ 次询问，每次查询一个区间的众数的出现次数，强制在线。



## 题目样例

### 样例输入

```
4 1
2 3 3 3
2 4
```

### 样例输出

```
3
```

### 样例解释

所查询的子数组为 $\{3,3,3\}$，其中众数为 $3$ ，众数出现的次数也为 $3$。



## 题目解法

区间求众数，主要处理三个数组：$zs[i][j]$ 代表第 $i$ 块到第 $j$ 块的众数，$poss[x]$ 代表离散化后的数字 $x$ 在原数组中的下标，$in[i]$ 代表 $a[i]$ 在 $poss[a[i]]$ 中的下标。

对于同一块内，用 $unordered\_map$ 暴力统计即可；不同块内先假设答案是 $zs$，然后对于两边的零散块，遍历其中每一个元素，$O(1)$ 快速找到对于当前答案，该数是不是比原有的众数出现的次数还要多，如果确实还要多，就看看它到底有多少，然后更新答案。

时间复杂度 $O(n\sqrt{n})$。

## 完整代码

```c++
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define _for(i,a,b) for(int i = (a);i < b;i ++)
#define _rep(i,a,b) for(int i = (a);i > b;i --)
#define INF 0x3f3f3f3f
#define pb push_back
#define lowbit(x) ((x)&(-x))
//#define mod 1000000007
#define MIKU 39
#define Design ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define debug() printf("Miku Check OK!\n")
#define MIKUCHAR "M"
#define maxn 500039

struct disp
{
	//a数组原大小
	int n;
	int a[maxn];
	//离散化后大小
	int sz;
	//acopy 为 a 排序去重以后
	int acopy[maxn];
	// lsh[i] 为 a[i] 离散化后的值
	int lsh[maxn];
	//kn为原数组大小
	void init(int kn)
	{
		n = kn;
		//可以把此处数组a换成别的数组
		memcpy(acopy,a,sizeof(a));
		sort(acopy+1,acopy+1+n);
		sz = unique(acopy+1,acopy+1+n)-(acopy+1);
		//可以把此处数组a换成别的数组
		_for(i,1,n+1)
		lsh[i] = lower_bound(acopy+1,acopy+1+sz,a[i])-acopy;
	}
	//O(logn)
	int getsmall(ll x)
	{
		return lower_bound(acopy+1,acopy+1+sz,x)-acopy;
	}
	//O(1)
	int getbig(int i)
	{
		return acopy[i];
	}
} dsp;

vector<int> poss[maxn];
int in[maxn];
//数组长度，查询个数，块的个数
int n, m, t;
struct piece
{
	int zs[710][710], cnt[maxn];
	//L[i]和R[i]表示第 i 段在原数组中的左右端点下标
	int L[maxn], R[maxn];
	//pos[j]表示 原数组下标 j 在哪一段
	int pos[maxn];

	void init()
	{
		t = sqrt(n);
		//块怎么分搞不懂看lyd p224
		_for(i,1,t+1)
		{
			L[i] = (i-1) * sqrt(n) + 1;
			R[i] = i * sqrt(n);
		}
		//尾巴还有一点，多分一个块
		if(R[t] < n)
		{
			t ++;
			L[t] = R[t-1] + 1;
			R[t] = n;
		}
		//O(nsqrt(n))时间复杂度处理众数zs[i][j]代表第i块到第j块的众数
		_for(i,1,t+1)
		{
			memset(cnt,0,sizeof(cnt));
			_for(j,i,t+1)
			{
				zs[i][j] = zs[i][j-1];
				_for(k,L[j],R[j]+1)
				{
					cnt[dsp.lsh[k]]++;
                    //求众数的话下面这行改成if就行，可以多开一个数组
					zs[i][j] = max(zs[i][j],cnt[dsp.lsh[k]]);
				}
			}
		}

		_for(i,1,t+1)
		_for(j,L[i],R[i]+1)
		pos[j] = i;
	}
	
	int ask(int l,int r)
	{
		//此次查询涉及[p,q]的分块
		int p = pos[l], q = pos[r];
		int ans = 0;
		//在同一分块内
		if(q<=p+1)
		{
			unordered_map<int,int> ump;
			_for(i,l,r+1)
			{
				ump[dsp.lsh[i]] ++;
				if(ump[dsp.lsh[i]] > ans)
					ans = ump[dsp.lsh[i]];
			}
		}
		else
		{
			//除一头一尾，其余中间每一块一块一块加
			ans = zs[p+1][q-1];
			//对头部的块朴素
			_for(i,l,R[p]+1)
			{
				int x = dsp.lsh[i];
				if(in[i]+ans-1>=poss[x].size())
					continue;
				int iid = in[i]+ans, tans = ans;
				while(iid < poss[x].size() && poss[x][iid] <= r)
				{
					iid ++;
					tans ++;
				}
				ans = tans;
			}

			//对尾部的块朴素
			_for(i,L[q],r+1)
			{
				int x = dsp.lsh[i];
				if(in[i]-ans+1<0)
					continue;
				int iid = in[i]-ans, tans = ans;
				while(iid >= 0 && poss[x][iid] >= l)
				{
					iid --;
					tans ++;
				}
				ans = tans;
			}
		}
		return ans;
	}
} P;

int main()
{
	scanf("%d%d",&n,&m);
	_for(i,1,n+1)
	scanf("%d",&dsp.a[++dsp.n]);
	dsp.init(dsp.n);
	_for(i,1,n+1)
	{
		int id = dsp.lsh[i];
		poss[id].pb(i);
		in[i] = poss[id].size()-1;
	}
	P.init();
	int lastans = 0;
	_for(i,1,m+1)
	{
		int l, r;
		scanf("%d%d",&l,&r);
		l ^= lastans, r ^= lastans;
		if(l>r)
			swap(l,r);
		printf("%d\n",lastans = P.ask(l,r));
	}
	return 0;
}
```

