# Link Cut Tree

这个数据结构可以建立点与点的间接关系与直接关系——可以查询一个点的权值，以及两点是否在同一棵树中，同时对已经连边的两点，切断本来已经有的连边。

$init():$ 用之前先初始化一下，同时注意 $MAXN$ 到底是多少。

$clear():$ 将所有的节点清空。

$haveEdge(x,y):$  若之前 $x$ 和 $y$  $link()$ 过，则返回 $true$，否则返回 $false$。

$find(x):$ 找到 $x$ 所属树的编号。

$link(x,y):$ 将 $x$ 和 $y$ 连边，前提是 $find(x)!=find(y)$ 。

$cut(x,y):$ 将 $x$ 和 $y$ 之间的边隔断，前提是 $haveEdge(x,y)$ 为 $true$。

一开始赋值的时候给 $sz2[]$ 赋值为初值。

每个操作一次的时间复杂度大概是 $O(logn)$。



```c++
struct lctNode
{
    int fa;
    int ch[2];
    //add your info
    int sz,sz2;
    //add your tag
    bool rotatetag;
};
const int MAXN=200000+10;
struct LCT
{
    lctNode nodes[MAXN];
    //the node you can use in the lct
    int n;
 
    //get a node's kind
    inline int Get(int x)
    {
        return nodes[nodes[x].fa].ch[1] == x;
    }
    //judge a node is the root in the splay tree
    inline bool isRoot(int x)
    {
        return nodes[nodes[x].fa].ch[0] != x && nodes[nodes[x].fa].ch[1] != x;
    }
    //if a node's child tree'struct change,use this to update info
    void pushUp(int x)
    {
        nodes[x].sz = nodes[nodes[x].ch[0]].sz + nodes[nodes[x].ch[1]].sz + nodes[x].sz2 + 1;
    }
    //pushDonw'son function
 
    void pushRotate(int p)
    {
        if(!p)return;
        swap(nodes[p].ch[0],nodes[p].ch[1]);
        nodes[p].rotatetag^=1;
    }
    //if you need visit a nodes'son,use this to make it real
    void pushDown(int x)
    {
        if(nodes[x].rotatetag)
        {
            pushRotate(nodes[x].ch[0]);
            pushRotate(nodes[x].ch[1]);
            nodes[x].rotatetag = false;
        }
    }
    //if you want ope on node which is unreal,please use it to make it real
    void update(int x)
    {
        if (!isRoot(x))update(nodes[x].fa);
        pushDown(x);
    }
    //rotate x
    void rotate(int x)
    {
        int y=nodes[x].fa;
        int z=nodes[y].fa;
        int k=Get(x);
        if(!isRoot(y)) nodes[z].ch[Get(y)]=x;
        nodes[x].fa=z;
        nodes[nodes[x].ch[!k]].fa=y;
        nodes[y].ch[k]=nodes[x].ch[!k];
        nodes[x].ch[!k]=y;
        nodes[y].fa=x;
        pushUp(y);
        pushUp(x);
        //we don't need to pushUP z,because it's son'stuct change don't produce a change to z
    }
    //splay a node to the splay root
    void splay(int x)
    {
        //before we ope on x,we need make sure it's true
        update(x);
        //rotate twice every route,the first rotate isn't necessary
        for (int p = nodes[x].fa; !isRoot(x); p = nodes[x].fa)
        {
            if (!isRoot(p))rotate(Get(p) == Get(x) ? p : x);
            rotate(x);
        }
    }
    //this function is used to produce a splay from the root in the origin tree to the node x
    //and it will return the splay root
    int Access(int x)
    {
        int p = 0;
        while (x)
        {
            //this cut is on the splay tree so don't cut the edge from son to father
            splay(x);
            //use this to modify the sz2
            nodes[x].sz2+=nodes[nodes[x].ch[1]].sz-nodes[p].sz;
            nodes[x].ch[1] = p;
            //we delete the on son tree of x,so it's necessary to pushUp x
            //use this to modify the sz
            pushUp(x);
            p = x;
            x = nodes[x].fa;
        }
        return p;
    }
    //this function is used to make the node x to the root in the origin tree
    //this function will return a splay tree's root which have node x
    int makeRoot(int x)
    {
        x = Access(x);
        //add a rotate tag to x
        swap(nodes[x].ch[0], nodes[x].ch[1]);
        nodes[x].rotatetag ^= 1;
        return x;
    }
    //this function is used to add a edge between x and y in the origin tree
    //but you need make sure x and y isn't at the same tree
    void link(int x,int y)
    {
        //we must make x to the root else this edge mean the preroot between and y
        x = makeRoot(x);
        makeRoot(y);
        //try optimization it's
        //splay(x);
        nodes[x].fa = y;
        //use this to modify the sz2 of the y
        nodes[y].sz2 += nodes[x].sz;
        while (y)
        {
            pushUp(y);
            y = nodes[y].fa;
        }
    }
    //this function will produce a splay tree from x to y in the origin tree path
    //it will return the splay root,but this root can be not x or y
    int split(int x,int y)
    {
        makeRoot(x);
        return Access(y);
    }
    //this function is used to cut edge x-y
    //but you need make sure there is a edge between x and y
    int res1,res2;
    void cut (int x,int y)
    {
        makeRoot(x);
        Access(y);
        splay(x);
        nodes[y].fa = 0;
        nodes[x].ch[1] = 0;
        pushUp(x);
        res1 = nodes[x].sz;
        res2 = nodes[y].sz;
    }
    //this function is used to find the root in the origin tree
    int find(int x)
    {
        x = Access(x);
        while (nodes[x].ch[0])x = nodes[x].ch[0];
        //if we ope a node in the splay tree,after that make it to the root of the splay
        splay(x);
        return x;
    }
    //this function is used to test if there a edge between x and y
    bool haveEdge(int x,int y)
    {
        if(find(x)==find(y))
        {
            makeRoot(x);
            Access(y);
            splay(x);
            if(nodes[x].ch[1]==y&&nodes[y].ch[0]==0)return true;
        }
        return false;
    }
    //this ope is used to init the LCT
    //please write a empty tree in this place
    //every change on empty tree will useless
    //every use of empty tree will true
    void init()
    {
        nodes[0].fa = 0;
        nodes[0].ch[0] = nodes[0].ch[1] = 0;
        nodes[0].rotatetag = false;
        n = 1;
        //other init ope
        nodes[0].sz=0;
        nodes[0].sz2=0;
    }
    //this function let everynodes be empty tree
    void clear()
    {
        for (int i = 1; i < n; i++)
        {
            nodes[i].rotatetag = false;
            nodes[i].fa = 0;
            nodes[i].ch[0] = nodes[i].ch[1] = 0;
            //other clear operation
 
        }
        n = 1;
    }
    //this function is used to produce a newTree according your info
    int newTree(int _weight)
    {
        //use index: n
        return n++;
    }
    //your special function
 
} tree;
```


