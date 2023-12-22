#ifndef TWOSATCPP
#define TWOSATCPP

#include "../kuinkerm.h"

/*  2-SAT
 */
namespace kuinkerm {

// std::shared_ptr<Graph> GPtr;
// std::shared_ptr<Tarjan> tarPtr;
// std::vector<int> Solution;
// bool Rnt;

twoSAT::twoSAT(int propNum, int ruleMaxNum)
    : runCalled(false), propNum(propNum), Rnt(true) {
  GPtr = std::make_shared<Graph>(propNum << 2, ruleMaxNum << 2, propNum);
}

void twoSAT::Or(int a, int b, bool aValue, bool bValue) {
  runCalled = false;
  if (aValue == true && bValue == true)
    GPtr->Add(a + propNum, b), GPtr->Add(b + propNum, a);
  else if (aValue == false && bValue == false)
    GPtr->Add(a, b + propNum), GPtr->Add(b, a + propNum);
  else if (aValue == true && bValue == false)
    GPtr->Add(a + propNum, b + propNum), GPtr->Add(b, a);
  else  // if(aValue == false && bValue == true)
    GPtr->Add(a, b), GPtr->Add(b + propNum, a + propNum);
}
void twoSAT::And(int a, int b, bool aValue, bool bValue) {
  runCalled = false;
  if (aValue == true && bValue == true)
    True(a), True(b);
  else if (aValue == false && bValue == false)
    False(a), False(b);
  else if (aValue == true && bValue == false)
    True(a), False(b);
  else  // if(aValue == false && bValue == true)
    False(a), True(b);
}
void twoSAT::Implies(int a, int b, bool aValue, bool bValue) {
  runCalled = false;
  Or(a, b, !aValue, bValue);
}
void twoSAT::True(int a) {
  runCalled = false;
  GPtr->Add(a + propNum, a);
}
void twoSAT::False(int a) {
  runCalled = false;
  GPtr->Add(a, a + propNum);
}
std::vector<int> twoSAT::getOneSolution() {
  Rnt = isPossible();
  std::vector<int> Dgt = tarPtr->getInSCC();
  for (int i = 1; i <= GPtr->getVertexNum(); i ++)
    if (Dgt[i] < Dgt[i + propNum]) 
        Solution.emplace_back(i);
  return Solution;
}
bool twoSAT::isPossible() {
  if (runCalled == true) return Rnt;
  runCalled = true;
  tarPtr = std::make_shared<Tarjan>(*GPtr, GPtr->getVertexNum(), "SCC");
  std::vector<int> Dgt = tarPtr->getInSCC();
  for (int i = 1; i <= GPtr->getVertexNum(); i ++)
    if (Dgt[i] == Dgt[i + propNum]) Rnt = false;
  return Rnt;
}

}  // namespace kuinkerm
#endif