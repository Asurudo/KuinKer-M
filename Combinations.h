#ifndef COMBINATIONSH
#define COMBINATIONSH

#include "kuinkerm.h"

/* 组合数计算
 */
namespace kuinkerm {

Combinations::combinationsCalculation::combinationsCalculation(int Maxn)
    : Maxn(Maxn) {
  Fac.resize(Maxn + 1);
  Inv.resize(Maxn + 1);
  Fac[0] = Inv[0] = 1;

  for (int i = 1; i <= Maxn; i++) Fac[i] = Fac[i - 1] * i % MOD;

  Inv[Maxn] = numTheoryBasic::fastPow(Fac[Maxn], MOD - 2, MOD);
  for (int i = Maxn - 1; i >= 1; i--) Inv[i] = Inv[i + 1] * (i + 1LL) % MOD;
}

std::shared_ptr<Combinations::combinationsCalculation>
Combinations::combinationsCalculation::getInstance(int Maxn) {
  if (nullptr == comCalPtr)
    comCalPtr = std::shared_ptr<combinationsCalculation>(
        new combinationsCalculation(Maxn));
  return comCalPtr;
}
bigInt Combinations::combinationsCalculation::Fa(int i) { return Fac[i]; }
bigInt Combinations::combinationsCalculation::In(int i) { return Inv[i]; }

bigInt Combinations::Inv(bigInt x, bigInt MOD) {
  return numTheoryBasic::fastPow(x, MOD - 2, MOD);
}
// 卢卡斯
bigInt Combinations::privateLucasPre(bigInt n, bigInt m) {
  if (n < m) return 0;
  bigInt Up = 1, Down = 1;
  for (bigInt i = n - m + 1; i <= n; i = i + 1) Up = Up * i % MOD;
  for (bigInt i = 1; i <= m; i = i + 1) Down = Down * i % MOD;
  return Up * Inv(Down, MOD) % MOD;
}
bigInt Combinations::Lucas(bigInt n, bigInt m) {
  if (m == 0) return 1;
  return privateLucasPre((bigInt)n % MOD, (bigInt)m % MOD) *
         Lucas((bigInt)n / MOD, (bigInt)m / MOD) % MOD;
}

Combinations::Combinations(bigInt n, bigInt m) : n(n), m(m) {
  comCalPtr = combinationsCalculation::getInstance((int)(5e2 + 10));
}

// 比较大小
bool Combinations::operator<(Combinations s) const {
  bigInt TEN = 10ll;
  return numTheoryBasic::bigIntLognX(TEN, n) -
             numTheoryBasic::bigIntLognX(TEN, m) -
             numTheoryBasic::bigIntLognX(TEN, n - m) <
         numTheoryBasic::bigIntLognX(TEN, s.n) -
             numTheoryBasic::bigIntLognX(TEN, s.m) -
             numTheoryBasic::bigIntLognX(TEN, s.n - s.m);
}

// 求组合
bigInt Combinations::C() {
  if (n < m) return 0;
  if (Com != -1) return Com;
  if (n < 5e2)
    return Com = comCalPtr->Fa(bigInt::converseToLL(n)) *
                 comCalPtr->In(bigInt::converseToLL(m)) % MOD *
                 comCalPtr->In(bigInt::converseToLL(n - m)) % MOD;
  return Com = Lucas(n, m);
}
// 求排列
bigInt Combinations::P() {
  bigInt Factorial = 1, c = C() % MOD;
  while (m != 0) {
    Factorial = Factorial * m;
    Factorial = Factorial % MOD;
    m = m - 1;
  }
  return (c * Factorial) % MOD;
}
std::shared_ptr<Combinations::combinationsCalculation>
    Combinations::combinationsCalculation::comCalPtr = nullptr;

}  // namespace kuinkerm
#endif