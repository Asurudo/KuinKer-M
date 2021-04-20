# trie树

## 例题编号

**NULL**

------

```c++
struct trie
{
	int tot;
    //maxn是字符串最大个数*字符串最大长度
	int sum[maxn*26];
	int trie[maxn][26];
	void clear()
	{
		tot = 0;
		memset(trie,0,sizeof trie);
		memset(sum,0,sizeof sum);
	}
	void insert(string a)
	{
		int rt = 0;
		_for(i,0,a.size())
		{
			int id = a[i]-'a';
			if(!trie[rt][id])
				trie[rt][id] = ++tot;
			rt = trie[rt][id];
			if(i==a.size()-1)
				sum[rt] ++;
		}
	}
	int exittimes(string a)
	{
		int rt = 0;
		_for(i,0,a.size())
		{
			int id = a[i]-'a';
			if(!trie[rt][id])
				return 0;
			rt = trie[rt][id];
			if(i==a.size()-1)
				return sum[rt];
		}
		return 0;
	}
}te;
```

