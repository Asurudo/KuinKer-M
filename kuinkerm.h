/*
    KuinKer-M: My Algorithm Library based on STL
    Author: Asurudo Jyo
    encode: utf-8
*/

#ifndef KUINKERMH
#define KUINKERMH

#include <algorithm>
#include <cassert>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/hash_policy.hpp>
#include <ext/pb_ds/priority_queue.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/rope>
#include <iomanip>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace kuinkerm {

using ll = long long;
constexpr int INF = 0x3f3f3f3f;
constexpr ll INFLL = 0x3f3f3f3f3f3f3f3f;
inline int lowbit(int x) { return ((x) & (-x)); }

class bigInt {
 private:
  using vll = std::vector<ll>;
  // value == 0 is represented by empty z
  std::vector<int> z;

  // 1 为非负数，-1为负数
  int Sign;
  void read(const std::string& s);

  // 取余的内部函数
  friend std::pair<bigInt, bigInt> divMod(const bigInt& a1, const bigInt& b1);

  // 去除前导0的内部函数
  void Trim();

  // 转换内部进制的内部函数
  static std::vector<int> convertBASE(const std::vector<int>& a, int oldDigits,
                                      int newDigits);

  // 实现高效乘法的内部函数
  static vll karatsubaMultiply(const vll& a, const vll& b);

 public:
  constexpr static int BASE = 1000000000;
  constexpr static int BASEDIGITS = 9;
  bigInt();
  bigInt(long long v);
  bigInt(const std::string& s);

  bigInt& operator=(long long v);

  static ll converseToLL(bigInt v);

  bigInt& operator+=(const bigInt& Other);

  friend bigInt operator+(bigInt a, const bigInt& b);

  bigInt& operator-=(const bigInt& Other);

  friend bigInt operator-(bigInt a, const bigInt& b);

  bigInt& operator*=(int v);

  bigInt operator*(int v) const;

  bigInt operator*(const bigInt& v) const;

  bigInt Sqrt(const bigInt& a1);

  bigInt operator/(const bigInt& v) const;

  bigInt operator%(const bigInt& v) const;

  bigInt& operator/=(int v);

  bigInt operator/(int v) const;

  bigInt& operator*=(const bigInt& v);

  bigInt& operator/=(const bigInt& v);

  bool operator<(const bigInt& v) const;

  bool operator>(const bigInt& v) const;

  bool operator<=(const bigInt& v) const;

  bool operator>=(const bigInt& v) const;

  bool operator==(const bigInt& v) const;

  bool operator!=(const bigInt& v) const;

  friend bigInt operator-(bigInt v);

  friend std::istream& operator>>(std::istream& stream, bigInt& v);

  friend std::ostream& operator<<(std::ostream& stream, const bigInt& v);

  // 判断是否为0
  bool isZero() const;
  // 绝对值
  bigInt Abs() const;
  // 将其转为long long类型
  long long longValue() const;
  // 计算最大公因数
  static bigInt Gcd(const bigInt& a, const bigInt& b);
  // 计算最小公倍数
  static bigInt Lcm(const bigInt& a, const bigInt& b);
  // 快速幂
  static bigInt Power(const bigInt& Base, const bigInt& Exponent);
};

class numTheoryBasic {
 public:
  static bigInt STANDARDMOD();
  static constexpr double STANDARDEPS() { return 1e-5; }

  // 快速幂
  static bigInt fastPow(bigInt a, bigInt n,
                        bigInt MOD = numTheoryBasic::STANDARDMOD());

  // 最大公因数
  template <typename T>
  static T Gcd(T a, T b) {
    if (b == 0) return a;
    return Gcd(b, a % b);
  }

  // 最小公倍数
  template <typename T>
  static T Lcm(T a, T b) {
    return a / Gcd(a, b) * b;
  }

  static bigInt bigIntLognX(bigInt n, bigInt X);
  static double doubleLognX(double n, double X);
};

template <typename T>
class binarySearch {
 private:
  static constexpr double EPS = numTheoryBasic::STANDARDEPS();
  static T leftIntBSearch(T l, T r, std::function<bool(T)> Judge);
  static T rightIntBSearch(T l, T r, std::function<bool(T)> Judge);
  static T leftFloBSearch(T l, T r, std::function<bool(T)> Judge);
  static T rightFloBSearch(T l, T r, std::function<bool(T)> Judge);
  binarySearch();

 public:
  ~binarySearch();
  binarySearch(const binarySearch&) = delete;
  binarySearch& operator=(const binarySearch&) = delete;

  static T biSearch(const std::string dirString, T l, T r,
                    std::function<bool(T)> Judge);
};

class packDp {
 private:
  // 最多有多少个物品
  int Maxn;
  std::vector<int> Dp;
  // 背包最大容量
  int Capacity;
  // 背包内物品最大价值
  int maxValue;

 public:
  /*  返回背包内物品最大价值
   */
  int getMaxValue();

  /*  物品的个数，背包的容量
   */
  packDp(int N, int Capacity);

  /*  01 背包
      给定一组物品，每个物品都有一个重量 weight 和一个价值
     value。现在有一个最多能装 capacity
     的背包，每种物品可以选择装或者不装进背包，求背包内物品的最大价值总和

      // 尝试将每个物品装入 01 背包
      for(every item)
      packDp.zeroOnePack(item.Weight, item.Value);
      // 输出最大价值
      print(packDp.getMaxValue());
  */
  void zeroOnePack(int Weight, int Value);

  /*  完全背包
      给定一组物品，每个物品都有一个重量 weight 和一个价值
     value。现在有一个最多能装 capacity
     的背包，每种物品可以选择装入任意个，求背包内物品的最大价值总和

      // 尝试将每个物品装入完全背包
      for(every item)
      packDp.completePack(item.Weight, item.Value);
      // 输出最大价值
      print(packDp.getMaxValue());
  */
  void completePack(int Weight, int Value);

  /*  多重背包
      给定一组物品，每个物品都有一个重量 weight 和一个价值
     value。现在有一个最多能装 capacity
     的背包，每种物品可以选择装入问题所给的有限个，求背包内物品的最大价值总和

      // 尝试将每个物品装入多重背包
      for(every item)
      packDp.multiplePack(item.Weight, item.Value, item.Amount);
      // 输出最大价值
      print(packDp.getMaxValue());
  */
  void multiplePack(int Weight, int Value, int Amount);
};

class Graph {
 public:
  friend class Dijkstra;
  friend class gameTheory;
  friend class expProceed;
  friend class TSP;
  friend class Tarjan;
  friend class topoSort;
  friend class connectingBlock;
  friend class MSTKruskal;
  friend class eulerTreePath;
  friend class LCADoubling;
  friend class netWorkFlow;

 protected:
  // 最大的点数和边数
  int Maxn, Maxe;
  // 实际的点数和边数
  int n, m;
  // 某一条边的后一条边
  std::vector<int> Next;
  // 边的起始信息，若有x ->(w) y，则Head[x]为第一条到达边的编号
  std::vector<int> Head;
  // 边的到达信息，即 x ->(w) y中的y
  std::vector<int> Ver;
  // 边的权值信息，即 x ->(w) y中的w
  std::vector<ll> Value;
  // 有向边的计数
  int Tot = 1;

 public:
  /*  最大点数，最大边数，实际点数（可不填），实际边数（可不填）
   */
  Graph(int Maxn, int Maxe, int n = 0, int m = 0);

  /*  向图中加有向边
      注意，加一条无向边等于加两条方向相反，权值相等的有向边

      // 向图中添加一条两端点编号分别为 39 和 42，且权值为 9 的无向边
      g.Add(39, 42, 9);
      g.Add(42, 39, 9);
  */
  // Add 返回 tot
  int Add(int x, int y, ll w = 0);
  int getVertexNum();
  int getEdgeNum();
  void setGpPointNumber(int n);
  void setGpEdgeNumber(int m);
};

class Dijkstra {
 private:
  // <最短距离，顶点编号>
  using Pair = std::pair<ll, int>;
  // 跑迪杰斯特拉最短路径算法的图
  const Graph& Gp;
  // 距离数组
  std::vector<ll> Distance;
  // 访问数组
  std::vector<bool> Visit;
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> Pq;
  // 源点
  int startPoint;

 public:
  /*  图，源点（默认为1）
      构造时直接开始跑最短路
  */
  Dijkstra(const Graph& Gp, int startPoint = 1);

  /*  返回整个Distance数组

      const vector<ll>& disRnt = dijkstra.getDistance();
  */
  const std::vector<ll>& getDistance() const;

  /*  返回源点到pointId的最短距离

      long long disRnt = dijkstra.getDistance(pointId);
  */
  ll getDistance(int pointId) const;
};

class TSP {
 private:
  // distance[xPoint][yPoint] 表示 xPoint 到 yPoint 的最小距离
  std::vector<std::vector<ll>> Distance;
  std::vector<std::vector<ll>> Dp;

  // 最小的边权总和
  ll minEdgeValueSum;

 public:
  /* 图构造时直接开始跑TSP
   */
  TSP(const Graph& Gp, int gN);
  /*  返回最小边权加和
   */
  ll getMinEdgeValueSum();
};

class LISLDS {
 private:
  // 是最长上升序列
  bool isLIS;
  // 是严格的
  bool isStrict;
  // 输入数组
  std::vector<ll> a;
  // dp数组
 public:
  std::vector<ll> Dp;
  // 最长的长度
  int rntLength;

  void strictLIS();

  void lenientLIS();

  void strictLDS();

  void lenientLDS();

 public:
  LISLDS(const std::vector<ll>& a, const std::string& isLIS,
         const std::string& isStrict);

  /*  返回最长长度
   */
  int getLength();
};

class disjointSet {
 private:
  // 最多有多少个集合
  int Maxn;
  // 每个集合的父节点
  std::vector<int> Parent;
  // 每个集合的高度
  std::vector<int> High;

 public:
  disjointSet(int Maxn);

  // 查询集合 x 的最终父亲
  int Find(int x);

  // 将集合 x 和集合 y 所属的两个集合连接起来
  void Unite(int x, int y);
  // 检查集合 x 和集合 y是否属于同一集合
  bool Same(int x, int y);
};

class blockPartition {
 private:
  // 块的个数
  int blockNum;
  // 输入数组
  std::vector<ll> a;

  // sum[i]表示第 i 段的原始加和
  // add[i]表示第 i 段的每个元素待加值/增量标记
  std::vector<ll> Sum, Add;

  // L[i]和R[i]表示第 i 段在原数组中的左右端点下标
  std::vector<int> L, R;

  // pos[j]表示 原数组下标 j 在哪一段
  std::vector<int> Pos;

 public:
  blockPartition(const std::vector<ll>& a);

  // 将区间[l, r]加上 Value
  void change(int l, int r, ll Value);

  // 返回区间[l, r]的和
  ll ask(int l, int r);
};

template <typename T1, typename T2>
class hashTable {
 private:
  // T1为键值，T2为数据
  __gnu_pbds::gp_hash_table<T1, T2> hTable;
  using htIter =
      typename __gnu_pbds::gp_hash_table<T1, T2>::point_const_iterator;

 public:
  // 插入一条哈希数据
  void Insert(T1 x, T2 y) { hTable.insert(std::make_pair(x, y)); }

  // 用下标访问哈希数据，如 table[x] 就是键值的对应数据
  T2& operator[](T1 x) { return hTable[x]; }
  const T2& operator[](T1 x) const { return hTable[x]; }

  // 返回一个指针，first为键值，second为数据
  htIter Find(T1 x) const { return hTable.find(x); }
  htIter notFound() const { return hTable.end(); }
  // 删除键值x的哈希数据
  void Erase(T1 x) const { hTable.erase(x); }
  void Clear() const { hTable.clear(); }
  int Size() const { return hTable.size(); }
  bool Empty() const { return hTable.empty(); }
};

template <typename T, typename F = void>
class priorityQueue {
 private:
  using Comparator = std::function<bool(const T&, const T&)>;
  using heapTag = __gnu_pbds::pairing_heap_tag;
  using pqType = typename std::conditional<
      std::is_void<F>::value,
      __gnu_pbds::priority_queue<T, Comparator, heapTag>,
      __gnu_pbds::priority_queue<T, F, heapTag>>::type;

  // 对值的映射，方便以后根据值进行查询和更改
  // std::map<T, std::set<pqIter>> val2PointerMp;

  std::unique_ptr<pqType> Pq;
  template <typename U = T, typename V = F>
  typename std::enable_if<
      std::is_fundamental<U>::value && std::is_void<V>::value, void>::type
  initializePriorityQueue() {
    Pq = std::make_unique<pqType>(std::less<T>());
  }

  template <typename U = T, typename V = F>
  typename std::enable_if<
      !std::is_fundamental<U>::value || !std::is_void<V>::value, void>::type
  initializePriorityQueue() {
    Pq = std::make_unique<pqType>();
  }

 public:
  using pqIter = typename pqType::point_iterator;
  priorityQueue() {
    if (!std::is_fundamental<T>::value && std::is_void<F>::value) {
      throw std::runtime_error(
          "priorityQueue, input type is not the fundamental type and the "
          "comparision function or struct is not provided.");
      return;
    } else
      initializePriorityQueue();
  }

  priorityQueue(Comparator f) {
    if (!std::is_void<F>::value) {
      throw std::runtime_error(
          "priorityQueue, both comparision function and struct are existed, "
          "delete one of them");
      return;
    }
    Pq =
        std::make_unique<__gnu_pbds::priority_queue<T, Comparator, heapTag>>(f);
  }

  pqIter Push(T x) { return Pq->push(x); }

  void Pop() { Pq->pop(); }

  T Top() { return Pq->top(); }

  int Size() { return Pq->size(); }

  bool Empty() { return Pq->empty(); }

  void Claer() { Pq->clear(); }

  // 传入一个指针，将Iter指向的值改为x后，再重新维护堆
  void Modify(pqIter Iter, const T x) { Pq->modify(Iter, x); }

  // 将otherPq并入调用该方法的堆，并清空otherPq
  void Join(priorityQueue& otherPq) { Pq->join(*otherPq.Pq); }
};

template <typename T, typename F = void>
class multiSet {
 private:
  using PT = std::pair<T, int>;
  using mSTag = __gnu_pbds::rb_tree_tag;
  using mSType = typename std::conditional<
      std::is_void<F>::value,
      __gnu_pbds::tree<PT, __gnu_pbds::null_type,
                       std::function<bool(const PT&, const PT&)>, mSTag,
                       __gnu_pbds::tree_order_statistics_node_update>,
      __gnu_pbds::tree<PT, __gnu_pbds::null_type, std::less<std::pair<F, int>>,
                       mSTag,
                       __gnu_pbds::tree_order_statistics_node_update>>::type;

  std::function<bool(const T&, const T&)> userComparator;
  // 时间戳
  int Dfn = 0;

  // 对值的映射，方便以后根据值进行查询和更改
  // std::map<T, std::set<pqIter>> val2PointerMp;

  std::unique_ptr<mSType> Ms;
  // template <typename U = T, typename V = F>
  // typename std::enable_if<
  //     std::is_fundamental<U>::value && std::is_void<V>::value, void>::type
  // initializePriorityQueue() {
  //   Pq = std::make_unique<mSType>(std::less<T>());
  // }

  // template <typename U = T, typename V = F>
  // typename std::enable_if<
  //     !std::is_fundamental<U>::value || !std::is_void<V>::value, void>::type
  // initializePriorityQueue() {
  //   Pq = std::make_unique<mSType>();
  // }

 public:
  using mSIter = typename mSType::point_iterator;
  multiSet() {
    if (!std::is_fundamental<T>::value && std::is_void<F>::value) {
      throw std::runtime_error(
          "multiSet, input type is not the fundamental type and the "
          "comparision function or struct is not provided.");
      return;
    } else
      // initializePriorityQueue();
      std::make_unique<mSType>();
  }

  multiSet(std::function<bool(const T&, const T&)> f) : userComparator(f) {
    if (!std::is_void<F>::value) {
      throw std::runtime_error(
          "multiSet, both comparision function and struct are existed, delete "
          "one of them");
      return;
    }
    Ms = std::make_unique<__gnu_pbds::tree<
        PT, __gnu_pbds::null_type, std::function<bool(const PT&, const PT&)>,
        mSTag, __gnu_pbds::tree_order_statistics_node_update>>(
        [f](const PT& x, const PT& y) -> bool {
          if (f(x.first, y.first) ^ f(y.first, x.first))
            return f(x.first, y.first);
          else
            return (bool)(x.second < y.second);
        });
  }

  int Size() { return Ms->size(); }

  bool Empty() { return Ms->empty(); }

  void Clear() { Ms->clear(); }

  mSIter Insert(T x) { return (Ms->insert({x, ++Dfn})).first; }

  void Erase(T x) { Ms->erase(Ms->lower_bound({x, 0})); }

  int isRank(T x) {
    if (rankIs(Ms->order_of_key({x, 0}) + 1) != x) return -1;
    return Ms->order_of_key({x, 0}) + 1;
  }

  bool Exist(T x) { return Ms->lower_bound({x, 0}) != Ms->end(); }

  T rankIs(int a) { return Ms->find_by_order(a - 1)->first; }

  // T Prev(T x) { return Ms->lower_bound({x-1, 0})->first; }
  T Next(T x) {
    if (false == this->Exist(x))
      throw std::runtime_error(
          "multiSet, x is not exist. make sure the x exists.");

    T Value = Ms->lower_bound({x, INF})->first;
    if (false == this->Exist(Value))
      throw std::runtime_error(
          "multiSet, Next is not exist. make sure the Next exists.");
    return Value;
  }

  // 返回第一个大于等于x的元素
  T lowerBound(T x) {
    T Value = Ms->lower_bound({x, 0})->first;
    // std::cout << (int)(Ms->lower_bound({x, 0}) == Ms->end()) << std::endl;
    if (false == this->Exist(Value))
      throw std::runtime_error(
          "multiSet, lowerBound is not exist. make sure the lowerBound "
          "exists.");
    return Value;
  }

  // 返回第一个大于x的元素
  T upperBound(T x) {
    T Value = Ms->lower_bound({x, INF})->first;
    if (false == this->Exist(Value))
      throw std::runtime_error(
          "multiSet, upperBound is not exist. make sure the upperBound "
          "exists.");
    return Value;
  }

  // 确保被合并set的所有元素大于合并set的所有元素，合并后清空
  void Join(multiSet& otherMs) {
    if (userComparator(otherMs.rankIs(1), this->rankIs(this->Size())))
      throw std::runtime_error(
          "multiSet, Join failed. The necessary condition for merging two sets "
          "is that all elements in the merging set must be smaller than all "
          "elements in the target set.");
    Ms->join(*otherMs.Ms);
  }

  // 分裂，小于x的元素留住，大于等于x的元素分裂出去
  void Split(T x, multiSet& otherMs) {
    otherMs.Clear();
    Ms->split({x, 0}, *otherMs.Ms);
  }
};

template <typename T>
class Vector {
 private:
  using rpIter = typename __gnu_cxx::rope<T>::iterator;
  __gnu_cxx::rope<T> Rp;
  Vector(__gnu_cxx::rope<T> Rp) : Rp(Rp) {}

 public:
  Vector() {}
  Vector(const Vector<T>& v) : Rp(v.Rp) {}

  const T operator[](std::size_t index) const { return Rp[index]; }

  void pushBack(T x) { Rp.push_back(x); }
  void popBack(T x) { Rp.pop_back(); }

  // 从 Pos 位置开始插入 x
  void Insert(int Pos, Vector<T> x) { Rp.insert(Pos, x); }

  // 从 Pos 位置开始删掉长度为 Len 的元素
  void Erase(int Pos, int Len) { Rp.erase(Pos, Len); }

  // 从 Pos 位置开始提取长度为 Len 的子串，返回一个 Vector
  Vector<T> Substr(int Pos, int Len) { return Vector<T>(Rp.substr(Pos, Len)); }

  // 替换 Pos 处的元素为 x
  void Replace(int Pos, T x) { Rp.replace(Pos, x); }

  // 从 Pos 位置开始替换为 x
  void Replace(int L, Vector<T> x) { Rp.replace(L, x.Size(), x); }
  void Clear() { Rp.clear(); }
  int Size() { return Rp.size(); }
  bool Empty() { return Rp.empty(); }
  Vector<T>& operator=(const Vector<T>& other) {
    if (this != &other) {
      Rp = other.Rp;
    }
    return *this;
  }
  Vector<T> operator+(const Vector<T>& other) const {
    return Vector<T>(Rp + other.Rp);
  }
};

class bitTree {
 private:
  // 矩阵的长和宽
  int n, m;
  std::vector<ll> a;
  std::vector<ll> c;
  std::vector<std::vector<ll>> a2;
  std::vector<std::vector<ll>> c2;
  ll Ask(int x);
  ll Ask(int x, int y);

 public:
  bitTree(const std::vector<ll>& a);
  bitTree(const std::vector<std::vector<ll>>& a2);

  void Add(int x, ll d);
  void Add(int x, int y, ll d);
  ll getSum(int l, int r);
  ll getSum(int x1, int y1, int x2, int y2);
};

class segTree {
 private:
  std::vector<ll> a;
  struct SegNode {
    int l, r;
    ll Add;
    ll Sum, Max, Min;
  };
  std::vector<struct SegNode> Tree;

  inline int& L(int x) { return Tree[x].l; }
  inline int& R(int x) { return Tree[x].r; }
  inline ll& ADD(int x) { return Tree[x].Add; }
  inline ll& SUM(int x) { return Tree[x].Sum; }
  inline ll& MAX(int x) { return Tree[x].Max; }
  inline ll& MIN(int x) { return Tree[x].Min; }
  inline int LChild(int p) { return p * 2; }
  inline int RChild(int p) { return p * 2 + 1; }

  void privateBuild(int p, int l, int r);

  void privateSpread(int p);

  void privateAdd(int p, int l, int r, ll d);

  ll privateAskSum(int p, int l, int r);
  ll privateAskMax(int p, int l, int r);
  ll privateAskMin(int p, int l, int r);
  ll privateAskFirstLess(int p, int l, int r, ll x);

 public:
  segTree(const std::vector<ll>& inputA);

  void Add(int l, int r, ll d);
  ll askSum(int l, int r);
  ll askMax(int l, int r);
  ll askMin(int l, int r);
  // 返回第一个小于等于x的下标
  // 返回-1就是没找到
  ll askFirstLess(int l, int r, ll x);
};

class LCT {
 private:
  int Maxn;
  // 实际的节点数
  int n;
  struct lctNode {
    int Fa;
    int Ch[2];
    int Sz, Sz2;
    bool rotateTag;
  };
  std::vector<struct lctNode> Nodes;
  // 获取节点 x 在其父节点中的左右关系
  inline int Get(int x);
  // 判断节点 x 是否是所在树的根
  inline bool isRoot(int x);
  // 更新节点 x 的信息，根据左右子树的信息更新节点大小等
  void pushUp(int x);
  // 反转节点 p 的左右子树
  void pushRotate(int x);
  // 将节点 x 的标记向下传递
  void pushDown(int x);
  // 更新节点 x 的信息，递归更新其父节点
  void Update(int x);
  // 旋转节点 x
  void Rotate(int x);
  // 伸展节点x
  void Splay(int x);
  // 将节点 x 到其树根的路径转为 Splay Tree，并返回树根
  int Access(int x);
  // 将节点 x 变为所在树的根
  int makeRoot(int x);

 public:
  LCT(int Maxn);

  int getSize(int x);

  // 判断是否存在边 x-y
  bool haveEdge(int x, int y);

  // 在两个不在同一树中的节点 x 和 y 之间添加边
  void Link(int x, int y);

  // 删除边 x-y
  void Cut(int x, int y);

  // 返回节点 x 所在树的根
  int Find(int x);

  // void clear();
};

class STList {
 private:
  int n;
  std::vector<std::vector<ll>> Max;
  std::vector<std::vector<ll>> Min;

 public:
  STList(const std::vector<ll>& a);
  ll askMax(int l, int r);
  ll askMin(int l, int r);
};

class stringDel {
 public:
  static std::vector<std::string> splitByDel(std::string s, char Del);
  // 1-index
  class stringHash {
   private:
    hashTable<std::string, int> HT;
    std::vector<std::string> V;

   public:
    std::string int2Str(int x);
    int str2Int(const std::string& s);
  };
};

class Trie {
 private:
  // 最长字符串的长度, 字符的总数
  int maxStrLen, maxCharNum;
  stringDel::stringHash strHash;
  // 存在于trie树中的字符串总数量
  int n = 0;

  std::vector<int> Sum;
  std::vector<int> preSum;
  std::vector<std::vector<int>> Try;

 public:
  Trie(int maxStrLen, int maxCharNum);
  void Insert(const std::string& s);

  // 返回true为删除成功，否则为删除失败
  bool Delete(const std::string& s);
  // 字符串的存在次数
  int existTimes(const std::string& s);

  // 计算拥有某前缀的字符串的数量
  int preExistTimes(const std::string& s);
};

class gameTheory {
 private:
  std::vector<bool> bothSmartDP, bothSillyDp, smartSillyDp, sillySmartDp;
  // 游戏的发展树
  const Graph& boardTree;
  // 为true则先手必胜/后手必败，否则先手必败/后手必胜
  bool firstMoveWin = false;

  // 双方都是最优策略
  bool bothSmartDfs(int x, int Fa);

  // 双方都是最差策略
  bool bothSillyDfs(int x, int Fa);

  // 先手最优策略，后手最差策略
  bool smartSillyDfs(int x, int Fa, bool isSmart);

 public:
  gameTheory(const Graph& boardTree, const std::string& firstMove,
             const std::string& secondMove);

  bool getFirstMove();
};

class judgePrime {
 private:
  const int LISTLENGTH = 1e6;
  std::vector<int> Vis;
  std::vector<int> Prime;
  std::string isCertain;
  bool millerRabin(bigInt x, bigInt b);

 public:
  judgePrime(const std::string& isCertain);
  bool isPrime(ll x);
  bool isPossiblyPrime(bigInt x);
};

class primeFactor {
 private:
  judgePrime jPrime;
  std::string isCertain;
  // 数字可以被分解为pi.first的pi.second次方的乘积
  std::vector<std::pair<ll, ll>> Pi;
  ll pollardRho(ll x);
  void getFactors(ll x);

 public:
  primeFactor(const std::string& isCertain, ll x);
  std::vector<std::pair<ll, ll>> getFactors();
};

class Combinations {
 private:
  // 内部的计算函数
  class combinationsCalculation {
   private:
    int Maxn;
    bigInt MOD = numTheoryBasic::STANDARDMOD();
    std::vector<bigInt> Fac;
    std::vector<bigInt> Inv;
    static std::shared_ptr<combinationsCalculation> comCalPtr;
    combinationsCalculation(int Maxn);

   public:
    ~combinationsCalculation(){};
    combinationsCalculation(const combinationsCalculation&) = delete;
    combinationsCalculation& operator=(const combinationsCalculation&) = delete;
    static std::shared_ptr<combinationsCalculation> getInstance(int Maxn);
    bigInt Fa(int i);
    bigInt In(int i);
  };
  // n >= m
  bigInt n, m;
  bigInt MOD = numTheoryBasic::STANDARDMOD();
  bigInt Com = -1;

  std::shared_ptr<combinationsCalculation> comCalPtr;
  // 求逆元
  bigInt Inv(bigInt x, bigInt MOD);
  // 卢卡斯
  bigInt privateLucasPre(bigInt n, bigInt m);
  bigInt Lucas(bigInt n, bigInt m);

 public:
  Combinations(bigInt n, bigInt m);

  // 比较大小
  bool operator<(Combinations s) const;

  // 求组合
  bigInt C();
  // 求排列
  bigInt P();
};

/* Tarjan算法
只有有向图有强连通分量，强连通分量内的点任意两点存在双向路径
无向图中，随便删去一点，u到v可达，则称u和v点双连通
随便删去一边，u到v可达，则称u和v边双连通
*/
class Tarjan {
 private:
  class tarjanSCC {
   private:
    const Graph& Gp;
    std::shared_ptr<Graph> contractionGPtr;
    // 时间戳和追溯值
    std::vector<int> Dfn, Low;
    // 栈，是否在栈中，节点所在强连通分量编号
    std::vector<int> Stack, inStack, inSCC;
    // 强连通分量中的点集
    std::vector<std::vector<int>> SCC;
    // 时间戳累加值，栈顶值，强连通分量个数
    int dfnNum, stackTop, SCCCnt;

    void privateTarjanSCC(int x);
    void SCCContraction();

   public:
    tarjanSCC(const Graph& Gp);
    // 获得一个指向将强连通分量缩成点以后的图的指针
    std::shared_ptr<Graph> gettarjanSCCPtr();
    // 获得节点所在强连通分量编号
    std::vector<int> getInSCC();
    // 获得每个强连通分量
    std::vector<std::vector<int>> getSCC();
  };
  class tarjanEDCC {
   private:
    const Graph& Gp;
    std::shared_ptr<Graph> contractionGPtr;
    // 时间戳和追溯值
    std::vector<int> Dfn, Low;
    // 某条边是否是桥，节点所在边双连通分量编号
    std::vector<int> isBridge, inEDCC;
    // 所有的桥
    std::vector<std::pair<int, int>> Bridge;
    // 边双连通分量中的点集
    std::vector<std::vector<int>> EDCC;
    // 时间戳累加值，强连通分量个数
    int dfnNum, EDCCCnt;

    void privateTarjanEDCC(int x, int inEdge);
    void priavateGetEDCC(int x);
    void EDCCContraction();

   public:
    tarjanEDCC(const Graph& Gp);
    // 获得一个指向将边双连通分量缩成点以后的图的指针
    std::shared_ptr<Graph> gettarjanEDCCPtr();
    // 获得节点所在边双连通分量编号
    std::vector<int> getInEDCC();
    // 获得每个边双连通分量
    std::vector<std::vector<int>> getEDCC();
  };
  class tarjanVDCC {
   private:
    const Graph& Gp;
    std::shared_ptr<Graph> contractionGPtr;
    // 时间戳和追溯值
    std::vector<int> Dfn, Low;

    // 栈，是否在栈中，节点所在点双连通分量编号
    std::vector<int> Stack, inStack, inVDCC;
    // 某个点是否是割点，所有割点
    std::vector<int> isCutoffPoint, cutoffPoint;
    // 点双连通分量中的点集
    std::vector<std::vector<int>> VDCC;
    // newID[i] = x 表示原图i点变成了缩图x点
    // oldID[x] = i 表示缩图x点中含有原图i点
    std::vector<int> oldID, newID;
    // 时间戳累加值，栈顶值，当前根节点, 点双联通分量个数
    int dfnNum, stackTop, Root, VDCCCnt;

    void privateTarjanVDCC(int x);
    void VDCCContraction();

   public:
    tarjanVDCC(const Graph& Gp);
    // 获得一个指向将点双连通分量缩成点以后的图的指针
    std::shared_ptr<Graph> gettarjanVDCCPtr();
    // 获得节点所在点双连通分量编号
    std::vector<int> getInVDCC();
    // 获得每个点双连通分量
    std::vector<std::vector<int>> getVDCC();
  };
  const std::string componentsType = "";
  tarjanSCC tSCC;
  tarjanEDCC tEDCC;
  tarjanVDCC tVDCC;

 public:
  Tarjan(const Graph& Gp, int gN, const std::string& componentsType);
  std::shared_ptr<Graph> gettarjanSCCPtr();
  std::vector<int> getInSCC();
  std::vector<std::vector<int>> getSCC();
  std::shared_ptr<Graph> gettarjanEDCCPtr();
  std::vector<int> getInEDCC();
  std::vector<std::vector<int>> getEDCC();
  std::shared_ptr<Graph> gettarjanVDCCPtr();
  std::vector<int> getInVDCC();
  std::vector<std::vector<int>> getVDCC();
};

/* 拓扑排序
 */
class topoSort {
 private:
  // 辅助队列
  std::queue<int> Q;
  // deg[i]为点 i 的入度
  std::vector<int> Deg;
  // topoSequence为最终拓扑序
  std::vector<int> topoSequence;
  // 记录图的结点数
  int gN;

 public:
  topoSort(const Graph& Gp, int gN);
  bool haveCircle();
  std::vector<int> getTopoSequence();
};

/* 图的联通块划分
   联通块从0开始标号
 */
class connectingBlock {
 private:
  // 联通块标号
  int blockCnt;
  // belongToBlock[i]为i属于哪个连通块 ,同时也为标记数组
  std::vector<int> belongToBlock;
  // Blocks[i]为连通块i里的点数组
  std::vector<std::vector<int>> Blocks;
  void privateTraverseSubg(int x, const Graph& Gp);

 public:
  connectingBlock(const Graph& Gp, int gN);
  std::vector<std::vector<int>> getBlocks();
  std::vector<int> getBelongToBlock();
  int getBlockCnt();
};

/* 2-SAT
   1-index
   有一些命题，a,b,c,d....
   给出一些规则，例如 a且b，非c，非c蕴含a...
   询问是否有一种方案，使得规则全部进行合取后，成立。
 */
class twoSAT {
 private:
  int propNum;
  std::shared_ptr<Graph> GPtr;
  std::shared_ptr<Tarjan> tarPtr;
  std::vector<int> Solution;
  bool Rnt;
  bool runCalled;

 public:
  twoSAT(int propNum, int ruleMaxNum);
  void Or(int a, int b, bool aValue = true, bool bValue = true);
  void And(int a, int b, bool aValue = true, bool bValue = true);
  void Implies(int a, int b, bool aValue = true, bool bValue = true);
  void True(int a);
  void False(int a);
  std::vector<int> getOneSolution();
  bool isPossible();
};

/* MST
   1-index
 */
class MSTKruskal {
 private:
  struct Edge {
    int u;
    int v;
    ll w;
    bool operator<(Edge b) { return w < b.w; }
  };
  // 需要用到的边数组
  std::vector<Edge> e;
  // 最小生成树权值和
  ll MSTVal;
  disjointSet Djs;

 public:
  MSTKruskal(const Graph& Gp);
  ll getMSTVal();
};

/* 树的欧拉序
   默认1为树根
 */
class eulerTreePath {
 private:
  int Pos = 0, oPos = 0;
  std::vector<int> L, R, Re;
  // oula[x]:每个结点的起始（第一个）欧拉序列 ore[i]:第i个欧拉序列对应的结点
  std::vector<int> Ore, Oula;
  void eulerInit(const Graph& Gp, int x, int Fa);

 public:
  eulerTreePath(const Graph& Gp);
  std::vector<int> getEulerPath();
};

/* LCA
   倍增实现
   默认1为树根
 */
class LCADoubling {
 private:
  // f[x][k]表示x向上走2^k
  std::vector<std::vector<int>> f;
  // 深度数组，根节点深度为1
  std::vector<int> d;
  // dist[x]表示根节点到节点x的距离
  std::vector<ll> Dist;
  // 树的最大深度为2^bitmaxd
  int bitMaxD;

 public:
  LCADoubling(const Graph& Gp);
  int LCA(int x, int y);
};

/* 网络流
   网络流就是在不超过流量限制的前提下，流从源点源源不断地产生，流经整个网络，最终全部归于汇点的模型。
   使得整个网络的流量总和最大的问题称为最大流问题，此时的流量称为网络的最大流量。
 */
class netWorkFlow {
 private:
  class edmondsKarpMaxFlow {
   private:
    std::shared_ptr<Graph> GPtr;
    // Incf[i]为i在此趟BFS流过的流量
    // Pre[i]为当前流量路径中的节点i的父亲是谁，也就是谁把流给他的
    std::vector<ll> Vis, Incf, Pre;
    // s起点，t终点，maxflow最大流流量
    ll s, t, maxFlow = 0;
    // 寻找增广路
    bool getAugPaths();
    void UpdateMaxflow();

   public:
    edmondsKarpMaxFlow(int s, int t, int Maxn, int Maxe);
    ll getMaxFlow();
    std::shared_ptr<Graph> getFlowPtr();
    void addEdge(int x, int y, ll Value);
  };
  class dinicMaxFlow {
   private:
    std::shared_ptr<Graph> GPtr;
    // d[s] = 1,未访问过的点i的d[i]=0,d值越大说明越深
    std::vector<ll> d;
    // s起点，t终点，maxflow最大流流量
    ll s, t, maxFlow = 0;
    // 当前弧优化
    std::vector<int> Now;
    // bfs求层次
    bool getAugPaths();
    ll UpdateMaxflow(int x, ll Flow);

   public:
    dinicMaxFlow(int s, int t, int Maxn, int Maxe);
    ll getMaxFlow();
    std::shared_ptr<Graph> getFlowPtr();
    void addEdge(int x, int y, ll Value);
  };
  class costFlow {
   private:
    std::shared_ptr<Graph> GPtr;
    // incf[i]为i在此趟BFS流过的流量
    // pre[i]为当前流量路径中的节点i的父亲是谁，也就是谁把流给他的
    std::vector<ll> Vis, Incf, Pre;
    std::vector<ll> Cost;
    // spfa的单源距离
    std::vector<ll> d;
    // s起点，t终点，maxflow最大流流量
    ll s, t, maxFlow = 0, minCost = 0;
    std::shared_ptr<Graph> Gp;
    // 寻找增广路
    bool getAugPaths();
    void UpdateCostflow();

   public:
    costFlow(int s, int t, int Maxn, int Maxe);
    ll getMaxFlow();
    ll getMinCost();
    std::shared_ptr<Graph> getFlowPtr();
    void addEdge(int x, int y, ll Value, ll edgeCost);
  };
  edmondsKarpMaxFlow EKMF;
  dinicMaxFlow dinicMF;
  costFlow CMF;
  std::string algorithmType = "";

 public:
  void addEdgeMaxFlow(int x, int y, ll Value);
  void addEdgeCostFlow(int x, int y, ll Value, ll Cost);
  ll getMaxFlow();
  ll getMinCost();
  std::shared_ptr<Graph> getFlowPtr();
  netWorkFlow(int s, int t, int Maxn, int Maxe,
              std::string algorithmType = "EK");
};

}  // namespace kuinkerm

#ifdef KKMDEBUGMODE
#include "KuinKerM/Combinations.cpp"
#include "KuinKerM/Dijkstra.cpp"
#include "KuinKerM/Graph.cpp"
#include "KuinKerM/LCADoubling.cpp"
#include "KuinKerM/LCT.cpp"
#include "KuinKerM/LISLDS.cpp"
#include "KuinKerM/MSTKruskal.cpp"
#include "KuinKerM/STList.cpp"
#include "KuinKerM/TSP.cpp"
#include "KuinKerM/Tarjan.cpp"
#include "KuinKerM/Trie.cpp"
#include "KuinKerM/Vector.cpp"
#include "KuinKerM/bigInt.cpp"
#include "KuinKerM/binarySearch.cpp"
#include "KuinKerM/bitTree.cpp"
#include "KuinKerM/blockPartition.cpp"
#include "KuinKerM/connectingBlock.cpp"
#include "KuinKerM/disjointSet.cpp"
#include "KuinKerM/eulerTreePath.cpp"
#include "KuinKerM/gameTheory.cpp"
#include "KuinKerM/hashTable.cpp"
#include "KuinKerM/judgePrime.cpp"
#include "KuinKerM/multiSet.cpp"
#include "KuinKerM/netWorkFlow.cpp"
#include "KuinKerM/numTheoryBasic.cpp"
#include "KuinKerM/packDp.cpp"
#include "KuinKerM/primeFactor.cpp"
#include "KuinKerM/priorityQueue.cpp"
#include "KuinKerM/segTree.cpp"
#include "KuinKerM/stringDel.cpp"
#include "KuinKerM/topoSort.cpp"
#include "KuinKerM/twoSAT.cpp"
#endif

#endif
