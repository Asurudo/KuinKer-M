#ifndef TARJANH
#define TARJANH

#include "../kuinkerm.h"

/* Tarjan
 */
namespace kuinkerm {

void Tarjan::tarjanSCC::privateTarjanSCC(int x) {
  Dfn[x] = Low[x] = ++dfnNum;
  Stack[++stackTop] = x, inStack[x] = 1;
  for (int i = Gp.Head[x]; i; i = Gp.Next[i]) {
    int y = Gp.Ver[i];
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
  contractionGPtr = std::make_shared<Graph>(Gp.n << 1, Gp.Tot << 1);
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

Tarjan::tarjanSCC::tarjanSCC(const Graph& Gp)
    : Gp(Gp), dfnNum(0), stackTop(0), SCCCnt(0) {
  Dfn.resize(Gp.n + 10);
  Low.resize(Gp.n + 10);
  Stack.resize(Gp.n + 10);
  inStack.resize(Gp.n + 10);
  inSCC.resize(Gp.n + 10);
  for (int x = 1; x <= Gp.n; x++)
    if (!Dfn[x]) privateTarjanSCC(x);
  SCCContraction();
}

std::shared_ptr<Graph> Tarjan::tarjanSCC::gettarjanSCCPtr() {
  return contractionGPtr;
}
std::vector<int> Tarjan::tarjanSCC::getInSCC() { return inSCC; }
std::vector<std::vector<int>> Tarjan::tarjanSCC::getSCC() { return SCC; }

Tarjan::Tarjan(const Graph& Gp, int gN, const std::string& componentsType)
    : componentsType(componentsType),
      tSCC(componentsType == "SCC" ? Gp : Graph(0, 0, 0, 0)),
      tEDCC(componentsType == "EDCC" ? Gp : Graph(0, 0, 0, 0)),
      tVDCC(componentsType == "VDCC" ? Gp : Graph(0, 0, 0, 0)) {
  // std::cout << componentsType << std::endl;
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
  // std::cout << componentsType << std::endl;
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
std::shared_ptr<Graph> Tarjan::gettarjanEDCCPtr() {
  if (componentsType != "EDCC")
    throw std::runtime_error(
        "Tarjan, you call the wrong function, try VDCC or SCC");
  return tEDCC.gettarjanEDCCPtr();
}

std::vector<int> Tarjan::getInEDCC() {
  if (componentsType != "EDCC")
    throw std::runtime_error(
        "Tarjan, you call the wrong function, try VDCC or SCC");
  return tEDCC.getInEDCC();
}
std::vector<std::vector<int>> Tarjan::getEDCC() {
  if (componentsType != "EDCC")
    throw std::runtime_error(
        "Tarjan, you call the wrong function, try VDCC or SCC");
  return tEDCC.getEDCC();
}

std::shared_ptr<Graph> Tarjan::gettarjanVDCCPtr() {
  if (componentsType != "VDCC")
    throw std::runtime_error(
        "Tarjan, you call the wrong function, try SCC or EDCC");
  return tVDCC.gettarjanVDCCPtr();
}

std::vector<int> Tarjan::getInVDCC() {
  if (componentsType != "VDCC")
    throw std::runtime_error(
        "Tarjan, you call the wrong function, try SCC or EDCC");
  return tVDCC.getInVDCC();
}
std::vector<std::vector<int>> Tarjan::getVDCC() {
  if (componentsType != "VDCC")
    throw std::runtime_error(
        "Tarjan, you call the wrong function, try SCC or EDCC");
  return tVDCC.getVDCC();
}

void Tarjan::tarjanEDCC::privateTarjanEDCC(int x, int inEdge) {
  Dfn[x] = Low[x] = ++dfnNum;
  for (int i = Gp.Head[x]; i; i = Gp.Next[i]) {
    int y = Gp.Ver[i];
    if (!Dfn[y]) {
      privateTarjanEDCC(y, i);
      Low[x] = std::min(Low[x], Low[y]);

      if (Low[y] > Dfn[x]) isBridge[i] = isBridge[i ^ 1] = true;
    } else if (i != (inEdge ^ 1))
      Low[x] = std::min(Low[x], Dfn[y]);
  }
}

void Tarjan::tarjanEDCC::priavateGetEDCC(int x) {
  inEDCC[x] = EDCCCnt;
  EDCC[EDCCCnt - 1].push_back(x);
  for (int i = Gp.Head[x]; i; i = Gp.Next[i]) {
    int y = Gp.Ver[i];
    if (inEDCC[y] || isBridge[i]) continue;
    priavateGetEDCC(y);
  }
}

void Tarjan::tarjanEDCC::EDCCContraction() {
  contractionGPtr = std::make_shared<Graph>(Gp.n << 1, Gp.Tot << 1);
  contractionGPtr->n = EDCCCnt;
  contractionGPtr->m = 0;
  for (int x = 1; x <= Gp.n; x++)
    for (int i = Gp.Head[x]; i; i = Gp.Next[i]) {
      int y = Gp.Ver[i];
      if (inEDCC[x] == inEDCC[y]) continue;
      contractionGPtr->Add(inEDCC[x], inEDCC[y]);
      contractionGPtr->m++;
    }
}

Tarjan::tarjanEDCC::tarjanEDCC(const Graph& Gp)
    : Gp(Gp), dfnNum(0), EDCCCnt(0) {
  Dfn.resize(Gp.n + 10);
  Low.resize(Gp.n + 10);
  isBridge.resize(Gp.Maxe + 1);
  inEDCC.resize(Gp.n + 10);
  Bridge.resize(Gp.Maxe + 1);
  for (int i = 1; i <= Gp.n; i++)
    if (!Dfn[i]) privateTarjanEDCC(i, 0);
  for (int i = 2; i < Gp.Tot; i += 2) {
    if (isBridge[i]) Bridge.push_back({Gp.Ver[i], Gp.Ver[i ^ 1]});
  }

  for (int i = 1; i <= Gp.n; i++)
    if (!inEDCC[i]) {
      EDCCCnt++;
      EDCC.push_back({});
      priavateGetEDCC(i);
    }

  EDCCContraction();
}

std::shared_ptr<Graph> Tarjan::tarjanEDCC::gettarjanEDCCPtr() {
  return contractionGPtr;
}

std::vector<int> Tarjan::tarjanEDCC::getInEDCC() { return inEDCC; }

std::vector<std::vector<int>> Tarjan::tarjanEDCC::getEDCC() { return EDCC; }

void Tarjan::tarjanVDCC::privateTarjanVDCC(int x) {
  Dfn[x] = Low[x] = ++dfnNum;
  Stack[++stackTop] = x;
  if (x == Root && !Gp.Head[x]) {
    VDCC.push_back({});
    VDCC[VDCCCnt++].push_back(x);
    inVDCC[x] = VDCCCnt;
    return;
  }
  int flag = 0;
  for (int i = Gp.Head[x]; i; i = Gp.Next[i]) {
    int y = Gp.Ver[i];
    if (!Dfn[y]) {
      privateTarjanVDCC(y);
      Low[x] = std::min(Low[x], Low[y]);

      if (Low[y] >= Dfn[x]) {
        flag++;
        if (x != Root || flag > 1) isCutoffPoint[x] = true;
        VDCC.push_back({});
        int z;
        do {
          z = Stack[stackTop--];
          VDCC[VDCCCnt].push_back(z);
          inVDCC[z] = VDCCCnt+1;
        } while (z != y);
        VDCC[VDCCCnt++].push_back(x);
        inVDCC[x] = VDCCCnt;
      }
    } else
      Low[x] = std::min(Low[x], Dfn[y]);
  }
}
void Tarjan::tarjanVDCC::VDCCContraction() {
  contractionGPtr = std::make_shared<Graph>(Gp.n << 1, Gp.Tot << 1);
  contractionGPtr->n = VDCCCnt;
  contractionGPtr->m = 0;

  for (int x = 1; x <= Gp.n; x++)
    if (isCutoffPoint[x]) {
      newID[x] = ++contractionGPtr->n;
      oldID[contractionGPtr->n] = x;
    }

  for (int i = 0; i < VDCCCnt; i++)
    for (int j = 0; j < (int)VDCC[i].size(); j++) {
      int x = VDCC[i][j];
      if (isCutoffPoint[x]) {
        contractionGPtr->Add(i, newID[x]);
        contractionGPtr->Add(newID[x], i);
        contractionGPtr->m ++;
      }
    }
}

Tarjan::tarjanVDCC::tarjanVDCC(const Graph& Gp) 
  : Gp(Gp), dfnNum(0), VDCCCnt(0), stackTop(0), Root(0) {
    Dfn.resize(Gp.n + 10);
    Low.resize(Gp.n + 10);
    Stack.resize(Gp.n + 10);
    inStack.resize(Gp.n + 10);
    cutoffPoint.resize(Gp.n + 10);
    inVDCC.resize(Gp.n + 10);
    isCutoffPoint.resize(Gp.n + 10);
    oldID.resize(Gp.n + 10);
    newID.resize(Gp.n + 10);

    for (int i = 1; i <= Gp.n; i++)
      if (!Dfn[i]) Root = i, privateTarjanVDCC(i);

    for (int i = 1; i <= Gp.n; i++)
      if (isCutoffPoint[i]) cutoffPoint.emplace_back(i);

    VDCCContraction();
  }

  std::shared_ptr<Graph> Tarjan::tarjanVDCC::gettarjanVDCCPtr() {
    return contractionGPtr;
  }

  std::vector<int> Tarjan::tarjanVDCC::getInVDCC() { return inVDCC; }

  std::vector<std::vector<int>> Tarjan::tarjanVDCC::getVDCC() { return VDCC; }

}  // namespace kuinkerm
#endif