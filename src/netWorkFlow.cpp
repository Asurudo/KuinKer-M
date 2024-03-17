#ifndef NETWORKFLOWCPP
#define NETWORKFLOWCPP

#include "../kuinkerm.h"

/* 网络流
 */
namespace kuinkerm {

// 寻找增广路
bool netWorkFlow::edmondsKarpMaxFlow::getAugPaths() {
  std::fill(Vis.begin(), Vis.end(), 0);
  std::queue<ll> q;
  // 将起点放入队列
  q.push(s);
  Vis[s] = 1;
  // 从源点流出的流量自然是无穷
  Incf[s] = INFLL;

  while (!q.empty()) {
    int x = q.front();
    q.pop();
    // 遍历当前节点的所有边，包括反向边
    for (int i = GPtr->Head[x]; i; i = GPtr->Next[i])
      if (GPtr->Value[i]) {
        int y = GPtr->Ver[i];
        // 保证所有节点仅进队一次
        if (Vis[y]) continue;
        // 限流，不能大于父亲流过来的流量，也不能高于当前边限流
        Incf[y] = std::min(Incf[x], GPtr->Value[i]);
        // 存储父亲
        Pre[y] = i;
        // 入队并标记
        q.push(y);
        Vis[y] = 1;
        // std::cout << y << std::endl;
        // 到达汇点，说明找到了一条路，立即返回
        if (y == t) return true;
      }
  }
  return false;
}
void netWorkFlow::edmondsKarpMaxFlow::UpdateMaxflow() {
  int x = t;
  while (x != s) {
    int i = Pre[x];
    // 反向边下标为 (2,3) (4,5)...因此可以用 ^1 运算计算反向边
    GPtr->Value[i] -= Incf[t];
    GPtr->Value[i ^ 1] += Incf[t];
    x = GPtr->Ver[i ^ 1];
  }
  maxFlow += Incf[t];
}

netWorkFlow::edmondsKarpMaxFlow::edmondsKarpMaxFlow(int s, int t, int Maxn,
                                                    int Maxe)
    : s(s), t(t), maxFlow(0) {
  if (s == t) return;
  GPtr = std::make_shared<Graph>(Maxn + 2, (Maxe << 1) + 2);
  Vis.resize(GPtr->Maxn);
  Incf.resize(GPtr->Maxn);
  Pre.resize(GPtr->Maxn);
  // while (getAugPaths()) UpdateMaxflow();
}

ll netWorkFlow::edmondsKarpMaxFlow::getMaxFlow() {
  while (getAugPaths()) UpdateMaxflow();
  return maxFlow;
}
std::shared_ptr<Graph> netWorkFlow::edmondsKarpMaxFlow::getFlowPtr() {
  return GPtr;
}

// bfs求层次
bool netWorkFlow::dinicMaxFlow::getAugPaths() {
  std::fill(d.begin(), d.end(), 0);
  // 汇点开始
  std::queue<ll> q;
  q.push(s);
  Now[s] = GPtr->Head[s];
  d[s] = 1;
  while (!q.empty()) {
    int x = q.front();
    q.pop();
    for (int i = GPtr->Head[x]; i; i = GPtr->Next[i])
      // 如果有值且未访问过（反向边一开始值为0，不标层次）
      if (GPtr->Value[i] && !d[GPtr->Ver[i]]) {
        q.push(GPtr->Ver[i]);
        Now[GPtr->Ver[i]] = GPtr->Head[GPtr->Ver[i]];
        d[GPtr->Ver[i]] = d[x] + 1;
        if (GPtr->Ver[i] == t) return true;
      }
  }
  return false;
}
ll netWorkFlow::dinicMaxFlow::UpdateMaxflow(int x, ll Flow) {
  if (x == t) return Flow;
  // k为子节点增量，rest为当前点还能给出的最大增量
  ll Rest = Flow, k;
  // 如果没有子节点或者给不出增量自然不用考虑
  for (int i = Now[x]; i && Rest; i = GPtr->Next[i]) {
    Now[x] = i;
    // 如果层次是当前节点下一层且有值
    if (GPtr->Value[i] && d[GPtr->Ver[i]] == d[x] + 1) {
      // 限流后深搜
      k = UpdateMaxflow(GPtr->Ver[i], std::min(Rest, GPtr->Value[i]));
      // 这个子节点整不出增量了，就不再访问他了
      if (!k) d[GPtr->Ver[i]] = 0;
      // 对边进行更新操作，^1为其反向边
      GPtr->Value[i] -= k;
      GPtr->Value[i ^ 1] += k;
      // 当前节点还能整的增量变少
      Rest -= k;
    }
  }
  // 若flow==rest就是整不出活儿了，返回0等着被禁止访问吧
  // 若不为零说明还真的整了flow-rest的增量，返回增量值
  return Flow - Rest;
}

netWorkFlow::dinicMaxFlow::dinicMaxFlow(int s, int t, int Maxn, int Maxe)
    : s(s), t(t), maxFlow(0) {
  if (s == t) return;
  GPtr = std::make_shared<Graph>(Maxn + 2, (Maxe << 1) + 2);
  d.resize(GPtr->Maxn);
  Now.resize(GPtr->Maxn);

  // 每次的增量
  // ll Flow = 0;
  // maxFlow = 0;
  // bfs先预处理层次
  // while (getAugPaths())
  // dinic返回增量
  // while (Flow = UpdateMaxflow(s, INFLL)) maxFlow += Flow;
}
ll netWorkFlow::dinicMaxFlow::getMaxFlow() {  // 每次的增量
  ll Flow = 0;
  // bfs先预处理层次
  while (getAugPaths())
    // dinic返回增量
    while (Flow = UpdateMaxflow(s, INFLL)) maxFlow += Flow;
  return maxFlow;
}
std::shared_ptr<Graph> netWorkFlow::dinicMaxFlow::getFlowPtr() { return GPtr; }

bool netWorkFlow::costFlow::getAugPaths() {
  std::fill(Vis.begin(), Vis.end(), 0);
  for (int i = 0; i < (int)d.size(); i++) d[i] = INFLL;
  std::queue<int> q;
  // 将起点放入队列
  q.push(s);
  Vis[s] = 1;
  d[s] = 0;
  // 从源点流出的流量自然是无穷
  Incf[s] = INFLL;
  while (!q.empty()) {
    int x = q.front();
    q.pop();
    // 只要不在队中就还这个节点自由
    Vis[x] = 0;
    // 遍历当前节点的所有边，包括反向边
    for (int i = GPtr->Head[x]; i; i = GPtr->Next[i])
      if (GPtr->Value[i]) {
        int y = GPtr->Ver[i];
        ll c = Cost[i];
        if (d[y] > d[x] + c) {
          d[y] = d[x] + c;
          Incf[y] = std::min(Incf[x], GPtr->Value[i]);
          Pre[y] = i;
          if (!Vis[y]) Vis[y] = 1, q.push(y);
        }
      }
  }
  // 注意INF到底是哪个！
  if (d[t] == INFLL) return false;
  return true;
}
void netWorkFlow::costFlow::UpdateCostflow() {
  int x = t;
  while (x != s) {
    int i = Pre[x];
    // 反向边下标为 (2,3) (4,5)...因此可以用 ^1 运算计算反向边
    GPtr->Value[i] -= Incf[t];
    GPtr->Value[i ^ 1] += Incf[t];
    x = GPtr->Ver[i ^ 1];
  }
  maxFlow += Incf[t];
  minCost += d[t] * Incf[t];
}
netWorkFlow::costFlow::costFlow(int s, int t, int Maxn, int Maxe)
    : s(s), t(t), maxFlow(0) {
  if (s == t) return;
  GPtr = std::make_shared<Graph>(Maxn + 2, (Maxe << 1) + 2);
  Vis.resize(GPtr->Maxn);
  Incf.resize(GPtr->Maxn);
  Pre.resize(GPtr->Maxn);
  d.resize(GPtr->Maxn);
  Cost.resize((GPtr->Maxe << 1) + 2);
  // while (getAugPaths()) UpdateCostflow();
}
ll netWorkFlow::costFlow::getMaxFlow() {
  while (getAugPaths()) UpdateCostflow();
  return maxFlow;
}
ll netWorkFlow::costFlow::getMinCost() {
  while (getAugPaths()) UpdateCostflow();
  return minCost;
}
std::shared_ptr<Graph> netWorkFlow::costFlow::getFlowPtr() { return GPtr; }

netWorkFlow::netWorkFlow(int s, int t, int Maxn, int Maxe,
                         std::string algorithmType)
    : algorithmType(algorithmType),
      EKMF(algorithmType == "EK" ? edmondsKarpMaxFlow(s, t, Maxn, Maxe)
                                 : edmondsKarpMaxFlow(0, 0, 0, 0)),
      dinicMF(algorithmType == "DINIC" ? dinicMaxFlow(s, t, Maxn, Maxe)
                                       : dinicMaxFlow(0, 0, 0, 0)),
      CMF(algorithmType == "COST" ? costFlow(s, t, Maxn, Maxe)
                                  : costFlow(0, 0, 0, 0)) {}

void netWorkFlow::edmondsKarpMaxFlow::addEdge(int x, int y, ll Value) {
  GPtr->Add(x, y, Value);
  GPtr->Add(y, x, 0);
}
void netWorkFlow::dinicMaxFlow::addEdge(int x, int y, ll Value) {
  GPtr->Add(x, y, Value);
  GPtr->Add(y, x, 0);
}
void netWorkFlow::costFlow::addEdge(int x, int y, ll Value, ll edgeCost) {
  Cost[GPtr->Add(x, y, Value)] = edgeCost;
  Cost[GPtr->Add(y, x, 0)] = -edgeCost;
}

void netWorkFlow::addEdgeMaxFlow(int x, int y, ll Value) {
  if (algorithmType == "COST")
    throw std::runtime_error(
        "netWorkFlow, maybe you want to call function addEdgeCostFlow?");
  else if (algorithmType == "EK")
    EKMF.addEdge(x, y, Value);
  else
    dinicMF.addEdge(x, y, Value);
}
void netWorkFlow::addEdgeCostFlow(int x, int y, ll Value, ll Cost) {
  if (algorithmType != "COST")
    throw std::runtime_error(
        "netWorkFlow, maybe you want to call function addEdgeMaxFlow?");
  else
    CMF.addEdge(x, y, Value, Cost);
}
ll netWorkFlow::getMaxFlow() {
  if (algorithmType == "COST")
    return CMF.getMaxFlow();
  else if (algorithmType == "EK")
    return EKMF.getMaxFlow();
  else
    return dinicMF.getMaxFlow();
}
ll netWorkFlow::getMinCost() {
  if (algorithmType != "COST")
    throw std::runtime_error(
        "netWorkFlow, maybe you want to call function getMaxFlow?");
  else
    return CMF.getMinCost();
}
std::shared_ptr<Graph> netWorkFlow::getFlowPtr() {
  if (algorithmType == "EK")
    return EKMF.getFlowPtr();
  else if (algorithmType == "DINIC")
    return dinicMF.getFlowPtr();
  else
    return CMF.getFlowPtr();
}
}  // namespace kuinkerm

#endif