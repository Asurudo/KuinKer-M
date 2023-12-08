#ifndef LISLDSH
#define LISLDSH

#include "kuinkerm.h"

/*  LIS/LDS
 */
namespace kuinkerm {

void LISLDS::strictLIS() {
  if (a.empty()) return;
  Dp[0] = a[0];
  int dpLen = 0;
  for (int i = 1; i < (int)a.size(); i++)
    if (a[i] > Dp[dpLen])
      Dp[++dpLen] = a[i];
    else {
      int k = std::lower_bound(Dp.begin(), Dp.begin() + dpLen + 1, a[i]) -
              Dp.begin();
      Dp[k] = a[i];
      std::cout << k << " ";
    }
  rntLength = dpLen + 1;
}

void LISLDS::lenientLIS() {
  if (a.empty()) return;
  Dp[0] = a[0];
  int dpLen = 0;
  for (int i = 1; i < (int)a.size(); i++)
    if (a[i] >= Dp[dpLen])
      Dp[++dpLen] = a[i];
    else {
      int k = std::upper_bound(Dp.begin(), Dp.begin() + dpLen + 1, a[i]) -
              Dp.begin();
      Dp[k] = a[i];
    }
  rntLength = dpLen + 1;
}

void LISLDS::strictLDS() {
  if (a.empty()) return;
  Dp[0] = a[0];
  int dpLen = 0;
  for (int i = 1; i < (int)a.size(); i++)
    if (a[i] < Dp[dpLen])
      Dp[++dpLen] = a[i];
    else {
      int k = std::lower_bound(Dp.begin(), Dp.begin() + dpLen + 1, a[i],
                               std::greater<ll>()) -
              Dp.begin();
      Dp[k] = a[i];
    }
  rntLength = dpLen + 1;
}

void LISLDS::lenientLDS() {
  if (a.empty()) return;
  Dp[0] = a[0];
  int dpLen = 0;
  for (int i = 1; i < (int)a.size(); i++)
    if (a[i] <= Dp[dpLen])
      Dp[++dpLen] = a[i];
    else {
      int k = std::upper_bound(Dp.begin(), Dp.begin() + dpLen + 1, a[i],
                               std::greater<ll>()) -
              Dp.begin();
      Dp[k] = a[i];
    }
  rntLength = dpLen + 1;
}

LISLDS::LISLDS(const std::vector<ll>& a, const std::string& isLIS,
               const std::string& isStrict)
    : a(a) {
  Dp.resize(a.size());
  if (isLIS == "LIS" && isStrict == "STRICT")
    strictLIS(), this->isLIS = true, this->isStrict = true;
  else if (isLIS == "LIS" && isStrict == "LENIENT")
    lenientLIS(), this->isLIS = true, this->isStrict = false;
  else if (isLIS == "LDS" && isStrict == "STRICT")
    strictLDS(), this->isLIS = false, this->isStrict = true;
  else if (isLIS == "LDS" && isStrict == "LENIENT")
    lenientLDS(), this->isLIS = false, this->isStrict = false;
  else
    throw std::runtime_error("LISLDS, input parameter ERROR");
}

/*  返回最长长度
 */
int LISLDS::getLength() { return rntLength; }

}  // namespace kuinkerm
#endif