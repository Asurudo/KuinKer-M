#ifndef JUDGEPRIMEH
#define JUDGEPRIMEH

#include "kuinkerm.h"

/* 判定素数
    2的64次方以内基本没问题
*/
namespace kuinkerm {

  bool judgePrime::millerRabin(bigInt x, bigInt b) {
    bigInt k = x - 1;
    while (k != 0) {
      bigInt t = numTheoryBasic::fastPow(b, k, x);
      if (t != 1 && t != x - 1) return false;
      if ((k % 2) == 1 || t == x - 1) return true;
      k /= 2;
    }
    return true;
  }

  judgePrime::judgePrime(const std::string& isCertain) : isCertain(isCertain) {
    if (isCertain != "CERTAIN" && isCertain != "NOTCERTAIN")
      throw std::runtime_error("judgePrime, input parameter ERROR");

    Vis.resize(LISTLENGTH + 10);
    for (int i = 2; i <= LISTLENGTH; i++) {
      if (!Vis[i]) {
        Vis[i] = i;
        Prime.emplace_back(i);
      }
      int sz = Prime.size();
      for (int j = 0; j < sz; j++) {
        if (Prime[j] > Vis[i] || Prime[j] > LISTLENGTH / i) break;
        Vis[i * Prime[j]] = Prime[j];
      }
    }
  }
  bool judgePrime::isPrime(ll x) {
    if (x < 1e6) return (bool)(0 == Vis[x]);
    if (x < 2) return false;
    ll Lim = sqrt(x) + 1;
    for (int i = 2; i <= Lim; i++)
      if (!(x % i)) return false;
    return true;
  }
  bool judgePrime::isPossiblyPrime(bigInt x) {
    if (isCertain != "NOTCERTAIN")
      throw std::runtime_error(
          "judgePrime, be sure you call the right function.");
    if (x < 1e6) return (bool)(x == Vis[bigInt::converseToLL(x)]);
    return millerRabin(x, 2) && millerRabin(x, 325) && millerRabin(x, 9375) &&
           millerRabin(x, 28178) && millerRabin(x, 450775) &&
           millerRabin(x, 9780504) && millerRabin(x, 1795265022);
  }

}  // namespace kuinkerm
#endif