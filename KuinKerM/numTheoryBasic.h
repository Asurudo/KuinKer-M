#ifndef NUMTHEORYBASICH
#define NUMTHEORYBASICH

#include "../kuinkerm.h"

/* 数论基础算法
 */
namespace kuinkerm {
bigInt numTheoryBasic::STANDARDMOD() {
  return bigInt(
      "280656219389353782471416977020899982462872118843314491873197949878129927"
      "685031094185623779906768871499454115273165474238630081765635343668797211"
      "1");
}

constexpr double numTheoryBasic::STANDARDEPS() { return 1e-5; }

// 快速幂
bigInt numTheoryBasic::fastPow(bigInt a, bigInt n, bigInt MOD) {
  bigInt Res = 1;
  while (n != 0) {
    if ((n % 2) == 1) Res = Res % MOD * a % MOD;
    a = a % MOD * a % MOD;
    n /= 2;
  }
  return Res;
}

// 最大公因数
template <typename T>
T numTheoryBasic::Gcd(T a, T b) {
  if (b == 0) return a;
  return Gcd(b, a % b);
}

// 最小公倍数
template <typename T>
T numTheoryBasic::Lcm(T a, T b) {
  return a / Gcd(a, b) * b;
}

bigInt numTheoryBasic::bigIntLognX(bigInt n, bigInt X) {
  if (n <= 0 || X <= 0) return std::numeric_limits<bigInt>::quiet_NaN();

  bigInt r = 1.0;
  while (numTheoryBasic::fastPow(n, r) < X) r = r * 2;
  r = r + 1;

  return binarySearch<bigInt>::biSearch(
      X >= 1.0 ? "GOLEFT" : "GORIGHT", 0, r,
      [n, X](bigInt Mid) -> bool { return X <= fastPow(n, Mid); });
}

double numTheoryBasic::doubleLognX(double n, double X) {
  if (n <= 0 || X <= 0) return std::numeric_limits<double>::quiet_NaN();

  double r = 1.0;
  while (std::pow(n, r) < X) r = r * 2;
  r = r + 1;

  return binarySearch<double>::biSearch(
      X >= 1.0 ? "GOLEFT" : "GORIGHT", 0, r,
      [n, X](double Mid) -> bool { return X <= std::pow(n, Mid); });
}

}  // namespace kuinkerm
#endif