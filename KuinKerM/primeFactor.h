#ifndef PRIMEFACTORH
#define PRIMEFACTORH

#include "../kuinkerm.h"

/* 提取素因子
*/
namespace kuinkerm {

  ll primeFactor::pollardRho(ll x) {
    ll s = 0, t = 0, Val = 1, c = 1ll * rand() % (x - 1) + 1;
    int St = 0, Tar = 1;
    while (true) {
      for (St = 1; St <= Tar; St++) {
        t = ((ll)t * t + c) % x;
        Val = (ll)Val * llabs(t - s) % x;
        if ((St % 127) == 0) {
          ll d = numTheoryBasic::Gcd(Val, x);
          if (d > 1) return d;
        }
      }
      ll d = numTheoryBasic::Gcd(Val, x);
      if (d > 1) return d;
      Tar <<= 1;
      s = t;
      Val = 1;
    }
  }

  void primeFactor::getFactors(ll x) {
    if (x < 2) return;
    if (jPrime.isPossiblyPrime(x)) {
      Pi.emplace_back(std::make_pair(x, 0));
      return;
    }
    ll t = x;
    while (t >= x) t = pollardRho(x);
    while ((x % t) == 0) x /= t;
    getFactors(x);
    getFactors(t);
  }

  primeFactor::primeFactor(const std::string& isCertain, ll x)
      : isCertain(isCertain), jPrime("NOTCERTAIN") {
    if (isCertain != "CERTAIN" && isCertain != "NOTCERTAIN")
      throw std::runtime_error("primeFactor, input parameter ERROR");

    if (isCertain == "CERTAIN") {
      ll Lim = sqrt(x) + 1;
      for (int i = 2; i < Lim; i++) {
        if (!(x % i)) {
          Pi.emplace_back(std::make_pair(i, 0));
          while (!(x % i)) x /= i, Pi[(int)(Pi.size() - 1)].second++;
        }
      }
      if (x > 1) Pi.emplace_back(std::make_pair(x, 1));
    } else {
      getFactors(x);
      std::set<ll> tmpFacSet;
      for (auto Pa : Pi) tmpFacSet.insert(Pa.first);
      Pi.clear();
      for (auto Pa : tmpFacSet) Pi.emplace_back(std::make_pair(Pa, 0));
      for (auto& Pa : Pi) {
        while (!(x % Pa.first)) x /= Pa.first, Pa.second++;
      }
    }
  }

  std::vector<std::pair<ll, ll>> primeFactor::getFactors() { return Pi; }

}  // namespace kuinkerm
#endif