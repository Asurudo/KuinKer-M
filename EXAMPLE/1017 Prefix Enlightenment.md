# 1017  Prefix Enlightenment

来源： https://codeforces.com/contest/1291/problem/E

$tag:$ `并查集` 

## 题目大意

给定一个长度为 $n(1≤n≤300000)$ 的主数组，再给出 $k(1≤m≤300000)$ 个子数组，每个数组长度为 $c(1≤c≤n)$。其中主数组仅有 $0$ 和 $1$ 构成，子数组中都是主数组的下标，当你选取某个子数组时，子数组中元素对应主数组的元素会进行一次翻转操作。请问要求 主数组前 $i(1≤i≤n)$ 位全部为 $1$ 的最小选取个数。注意，主数组的同一下标最多只会在子数组中出现两次，且一定存在一种选取方案使得主数组每一元素都翻转为 $1$ 。

## 题目样例

### 样例输入

```
7 3
0011100
3
1 4 6
3
3 4 7
2
2 3
```

### 样例输出

```
1
2
3
3
3
3
3
```

### 样例解释

对于 $i==1$ ，我们选取子数组 $1$，主数组变为 $1010110$

对于 $i==2$ ，我们选取子数组 $1,3$，主数组变为 $1100110$

对于 $i≥3$ ，我们选取子数组 $1,2,3$，主数组变为 $1111111$

可以证明这样选取，既满足条件且子数组选取个数最少。

## 题目解法：

我们可以将子数组看成 点 ，主数组的下标看成线，若两个不同的子数组中存在至少一个相同的主数组下标，那我们就可以认为这两个子数组是有联系的。具体联系体现在：

- 当主数组某一位为 $0$ 时，有联系的两个子数组必定只选取一个
- 当主数组某一位为 $1$ 时，有联系的两个子数组必定同时选取或不选取

有没有一点 种类并查集 的感觉，那我们拆点，把点拆成两个状态：选取与不选取，并且给选取赋权值为 $1$，不选取赋权值为 $0$。我们将两个子数组各自的选取与不选取状态与对方的选取与不选取状态连起来，说明他们有联系。具体的联系方式为：

- 当主数组某一位为 $0$ 时，一个子数组的选取与另一个子数组的不选取连接
- 当主数组某一位为 $1$ 时，一个子数组的选取与另一个子数组的选取，不选取与不选取连接

这样就算是把主数组分为两个不同的 命题 ，这两个命题都可以为真，就看你选哪个了，那自然是选权值和最小的一个。为什么一定不会矛盾？因为题目保证一定存在一种选取方案使得主数组每一元素都翻转为 $1$ 。

这样感觉就大功告成了，不过我们再考虑一下，题目说最多两个，那一个和没有的情况呢？

因为题目保证一定存在一种选取方案使得主数组每一元素都翻转为 $1$ ，所以如果某一个主数组下标在子数组中没有出现，那原数组一定为 $1$ ，我们什么都不用做即可。

如果是出现一次，那就有：

- 当主数组某一位为 $0$ 时，该子数组必定选取
- 当主数组某一位为 $1$ 时，该子数组必定不选取

这怎么连接命题？我们可以设一个点，将他赋权值为 $+∞$ ，若必定选取，则将该点不选取状态与他相连即可，必定不选取时同理。

问题又来了，你说好两个命题呢，那这种必定选取或者不选取我们怎么统计啊？

我们就可以使用动态更新 $ans$ 的思想，当要更新一个或者两个点的时候，先消除他们在之前对答案的贡献，将该点更新过后，再将其加入。

时间复杂度 $O((n+k)α(n))$ 。

## 完整代码

```c++
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef double db;
#define _for(i,a,b) for(ll i = (a);i < b;i ++)
#define _rep(i,a,b) for(ll i = (a);i > b;i --)
#define INF 0x3f3f3f3f
#define ZHUO 11100000007
#define MOD 1000000007
#define MIKUNUM 39
#define pb push_back
#define debug() prllf("Miku Check OK!\n")
#define maxn 300039*2
#define X first
#define Y second

ll n, k;
string lamp;
vector<ll> have[maxn];

struct Djs
{
	ll par[maxn];
	ll high[maxn];
	ll d[maxn];
	void renew()
	{
		_for(i,1,maxn)
		{
			par[i] = i;
			high[i] = 0;
			if(i<=k)
				d[i] = 1;
			else
				d[i] = 0;
		}
	}
	ll find(ll x)
	{
		return par[x] == x ? x : par[x] = find(par[x]);
	}
	void unite(ll x,ll y)
	{
		x = find(x);
		y = find(y);
		if(x==y) return ;

		ll dx = d[x], dy = d[y];
		d[x] += dy;
		d[y] += dx;

		if(high[x]<high[y])
			par[x] = y;
		else
		{
			par[y] = x;
			if(high[x]==high[y])
				high[x] ++;
		}
	}
	bool same(ll x,ll y)
	{
		return find(x) == find(y);
	}
} djs;

int main()
{
	ios::sync_with_stdio(false);
	cin >> n >> k;
	string tmp;
	cin >> tmp;
    //补位
	lamp += '6';
	lamp += tmp;
	_for(i,1,k+1)
	{
		ll tn, tmp;
		cin >> tn;
		_for(j,0,tn)
		{
			cin >> tmp;
            //have[i]表示主数组下标 i 时有哪些子数组里包含 i
			have[tmp].pb(i);
		}
	}

	ll ans = 0;
	djs.renew();
    //st就是无穷节点
	int st = 600039;
	djs.d[st] = INF;

	_for(i,1,n+1)
	{
		//主数组下标i只存在一个子数组包含i
        if(have[i].size()==1)
		{
			int x = have[i][0];
            //消除该子数组之前对答案的影响
			ans -= min(djs.d[djs.find(x)],djs.d[djs.find(x+k)]);
			if(lamp[i]=='1')
				djs.unite(x,st);
			else
				djs.unite(x+k,st);
            //只有两种可能：选这个节点与不选这个节点
			ans += min(djs.d[djs.find(x)],djs.d[djs.find(x+k)]);
		}
		else if(have[i].size()==2)
		{
			int x = have[i][0], y = have[i][1];
            //如果他们已经联系过了，则没有更新发生
			if(!djs.same(x,y) && !djs.same(x,y+k))
			{
				//消除子数组 x 和 y的影响，因为他们马上要合起来考虑
                ans -= min(djs.d[djs.find(x)],djs.d[djs.find(x+k)])
				+ min(djs.d[djs.find(y)],djs.d[djs.find(y+k)]);
				if(lamp[i]=='1')
					djs.unite(x,y),djs.unite(x+k,y+k);
				else
					djs.unite(x+k,y),djs.unite(x,y+k);
				ans += min(djs.d[djs.find(x)],djs.d[djs.find(x+k)]);
			}
		}
		printf("%d\n",ans);
	}
	return 0;
}
```

