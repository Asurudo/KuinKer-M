#ifndef TARJANH
#define TARJANH

#include "kuinkerm.h"

/* Tarjan
 */
namespace kuinkerm {

void Tarjan::tarjanSCC::privateTarjanSCC(int x) {
  Dfn[x] = Low[x] = ++dfnNum;
  Stack[++stackTop] = x, inStack[x] = 1;
  tmpEdgeRecord = 0;
  for (int i = Gp.Head[x]; i; i = Gp.Next[i]) {
    int y = Gp.Ver[i];
    tmpEdgeRecord++;
    if (!Dfn[y]) {
      privateTarjanSCC(y);
      Low[x] = std::min(Low[x], Low[y]);
    } else if (inStack[y])
      Low[x] = std::min(Low[x], Dfn[y]);
  }
  if (Dfn[x] == Low[x]) {
    SCC.push_back({});
    int y;
    do {
      y = Stack[stackTop--], inStack[y] = 0;
      inSCC[y] = SCCCnt, SCC[SCCCnt].emplace_back(y);
    } while (x != y);
    SCCCnt++;
  }
}
void Tarjan::tarjanSCC::SCCContraction() {
  contractionGPtr = std::make_shared<Graph>(Gp.n << 1, tmpEdgeRecord << 1);
  contractionGPtr->n = SCCCnt;
  contractionGPtr->m = 0;
  for (int x = 1; x <= Gp.n; x++)
    for (int i = Gp.Head[x]; i; i = Gp.Next[i]) {
      int y = Gp.Ver[i];
      if (inSCC[x] == inSCC[y]) continue;
      contractionGPtr->Add(inSCC[x], inSCC[y]);
      contractionGPtr->m++;
    }
}

Tarjan::tarjanSCC::tarjanSCC(const Graph& Gp) : Gp(Gp){
  Dfn.resize(Gp.n + 10);
  Low.resize(Gp.n + 10);
  Stack.resize(Gp.n + 10);
  inStack.resize(Gp.n + 10);
  inSCC.resize(Gp.n + 10);
  for (int x = 1; x <= Gp.n; x++)
    if (!Dfn[x]) privateTarjanSCC(x);
}

std::shared_ptr<Graph> Tarjan::tarjanSCC::gettarjanSCCPtr() {
  return contractionGPtr;
}

std::vector<int> Tarjan::tarjanSCC::getInSCC() { return inSCC; }
std::vector<std::vector<int>> Tarjan::tarjanSCC::getSCC() { return SCC; }

Tarjan::Tarjan(const Graph& Gp, int gN, const std::string& componentsType)
    : componentsType(componentsType), tSCC(componentsType == "SCC" ? Gp : Graph(0,0)) {
  if (gN != Gp.n)
    throw std::runtime_error(
        "Tarjan, the second parameter differs from the point number in G, "
        "check it");

  if (componentsType == "SCC")
    ;
  else if (componentsType == "VDCC")
    ;
  else if (componentsType == "EDCC")
    ;
  else
    throw std::runtime_error("Tarjan, input parameter ERROR");
}

std::shared_ptr<Graph> Tarjan::gettarjanSCCPtr() {
  if (componentsType != "SCC")
    throw std::runtime_error(
        "Tarjan, you call the wrong function, try VDCC or EDCC");
  return tSCC.gettarjanSCCPtr();
}

std::vector<int> Tarjan::getInSCC() {
  if (componentsType != "SCC")
    throw std::runtime_error(
        "Tarjan, you call the wrong function, try VDCC or EDCC");
  return tSCC.getInSCC();
}
std::vector<std::vector<int>> Tarjan::getSCC() {
  if (componentsType != "SCC")
    throw std::runtime_error(
        "Tarjan, you call the wrong function, try VDCC or EDCC");
  return tSCC.getSCC();
}

}  // namespace kuinkerm
#endif