#ifndef BIGINTH
#define BIGINTH

#include "kuinkerm.h"

/*  大数
 */
namespace kuinkerm {

  // 读入字符串的内部函数
  void bigInt::read(const std::string& s) {
    Sign = 1;
    z.clear();
    int pos = 0;
    while (pos < (int)s.size() && (s[pos] == '-' || s[pos] == '+')) {
      if (s[pos] == '-') Sign = -Sign;
      ++pos;
    }
    for (int i = (int)s.size() - 1; i >= pos; i -= BASEDIGITS) {
      int x = 0;
      for (int j = std::max(pos, i - BASEDIGITS + 1); j <= i; j++)
        x = x * 10 + s[j] - '0';
      z.push_back(x);
    }
    Trim();
  }

  // 取余的内部函数
    std::pair<bigInt, bigInt>  divMod(const bigInt& a1, const bigInt& b1) {
    int Norm = bigInt::BASE / (b1.z.back() + 1);
    bigInt a = a1.Abs() * Norm;
    bigInt b = b1.Abs() * Norm;
    bigInt q, r;
    q.z.resize(a.z.size());

    for (int i = (int)a.z.size() - 1; i >= 0; i--) {
      r *= bigInt::BASE;
      r += a.z[i];
      int s1 = b.z.size() < r.z.size() ? r.z[b.z.size()] : 0;
      int s2 = b.z.size() - 1 < r.z.size() ? r.z[b.z.size() - 1] : 0;
      int d = (int)(((long long)s1 *  bigInt::BASE + s2) / b.z.back());
      r -= b * d;
      while (r < 0) r += b, --d;
      q.z[i] = d;
    }

    q.Sign = a1.Sign * b1.Sign;
    r.Sign = a1.Sign;
    q.Trim();
    r.Trim();
    return {q, r / Norm};
  }

  // 去除前导0的内部函数
  void  bigInt::Trim() {
    while (!z.empty() && z.back() == 0) z.pop_back();
    if (z.empty()) Sign = 1;
  }

  // 转换内部进制的内部函数
  std::vector<int>  bigInt::convertBASE(const std::vector<int>& a, int oldDigits,
                                      int newDigits) {
    std::vector<ll> p(std::max(oldDigits, newDigits) + 1);
    p[0] = 1;
    for (int i = 1; i < (int)p.size(); i++) p[i] = p[i - 1] * 10;
    std::vector<int> res;
    long long cur = 0;
    int curDigits = 0;
    for (int v : a) {
      cur += v * p[curDigits];
      curDigits += oldDigits;
      while (curDigits >= newDigits) {
        res.push_back(int(cur % p[newDigits]));
        cur /= p[newDigits];
        curDigits -= newDigits;
      }
    }
    res.push_back((int)cur);
    while (!res.empty() && res.back() == 0) res.pop_back();
    return res;
  }

  // 实现高效乘法的内部函数
  bigInt::vll  bigInt::karatsubaMultiply(const vll& a, const vll& b) {
    int n = a.size();
    vll res(n + n);
    if (n <= 32) {
      for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) res[i + j] += a[i] * b[j];
      return res;
    }

    int k = n >> 1;
    vll a1(a.begin(), a.begin() + k);
    vll a2(a.begin() + k, a.end());
    vll b1(b.begin(), b.begin() + k);
    vll b2(b.begin() + k, b.end());

    vll a1b1 = karatsubaMultiply(a1, b1);
    vll a2b2 = karatsubaMultiply(a2, b2);

    for (int i = 0; i < k; i++) a2[i] += a1[i];
    for (int i = 0; i < k; i++) b2[i] += b1[i];

    vll r = karatsubaMultiply(a2, b2);
    for (int i = 0; i < (int)a1b1.size(); i++) r[i] -= a1b1[i];
    for (int i = 0; i < (int)a2b2.size(); i++) r[i] -= a2b2[i];

    for (int i = 0; i < (int)r.size(); i++) res[i + k] += r[i];
    for (int i = 0; i < (int)a1b1.size(); i++) res[i] += a1b1[i];
    for (int i = 0; i < (int)a2b2.size(); i++) res[i + n] += a2b2[i];
    return res;
  }

   bigInt::bigInt() : Sign(1) {}
   bigInt::bigInt(long long v) { *this = v; }
   bigInt::bigInt(const std::string& s) { read(s); }

  bigInt&  bigInt::operator=(long long v) {
    Sign = v < 0 ? -1 : 1;
    v *= Sign;
    z.clear();
    for (; v > 0; v = v / BASE) z.push_back((int)(v % BASE));
    return *this;
  }

  ll  bigInt::converseToLL(bigInt v) {
    if (v >= INFLL)
      throw std::runtime_error("bigInt, invalid conversion, number is too big");
    ll Rnt = 1;
    if (v.Sign == -1) Rnt *= -1;

    Rnt *= (v.z.empty() ? 0 : v.z.back());
    for (int i = (int)v.z.size() - 2; i >= 0; --i) {
      Rnt *= 10;
      Rnt += v.z[i];
    }
    return Rnt;
  }

  bigInt&  bigInt::operator+=(const bigInt& Other) {
    if (Sign == Other.Sign) {
      for (int i = 0, Carry = 0; i < (int)Other.z.size() || Carry; ++i) {
        if (i == (int)z.size()) z.push_back(0);
        z[i] += Carry + (i < (int)Other.z.size() ? Other.z[i] : 0);
        Carry = z[i] >= BASE;
        if (Carry) z[i] -= BASE;
      }
    } else if (Other != 0 /* prevent infinite loop */) {
      *this -= -Other;
    }
    return *this;
  }

  bigInt  operator+(bigInt a, const bigInt& b) { return a += b; }

  bigInt&  bigInt::operator-=(const bigInt& Other) {
    if (Sign == Other.Sign) {
      if (Sign == 1 && *this >= Other || Sign == -1 && *this <= Other) {
        for (int i = 0, Carry = 0; i < (int)Other.z.size() || Carry; ++i) {
          z[i] -= Carry + (i < (int)Other.z.size() ? Other.z[i] : 0);
          Carry = z[i] < 0;
          if (Carry) z[i] += BASE;
        }
        Trim();
      } else {
        *this = Other - *this;
        this->Sign = -this->Sign;
      }
    } else {
      *this += -Other;
    }
    return *this;
  }

  bigInt  operator-(bigInt a, const bigInt& b) { return a -= b; }

  bigInt&  bigInt::operator*=(int v) {
    if (v < 0) Sign = -Sign, v = -v;
    for (int i = 0, Carry = 0; i < (int)z.size() || Carry; ++i) {
      if (i == (int)z.size()) z.push_back(0);
      long long cur = (long long)z[i] * v + Carry;
      Carry = (int)(cur / BASE);
      z[i] = (int)(cur % BASE);
    }
    Trim();
    return *this;
  }

  bigInt  bigInt::operator*(int v) const { return bigInt(*this) *= v; }

  bigInt  bigInt::operator*(const bigInt& v) const {
    std::vector<int> a6 = convertBASE(this->z, BASEDIGITS, 6);
    std::vector<int> b6 = convertBASE(v.z, BASEDIGITS, 6);
    vll a(a6.begin(), a6.end());
    vll b(b6.begin(), b6.end());
    while (a.size() < b.size()) a.push_back(0);
    while (b.size() < a.size()) b.push_back(0);
    while (a.size() & (a.size() - 1)) a.push_back(0), b.push_back(0);
    vll c = karatsubaMultiply(a, b);
    bigInt res;
    res.Sign = Sign * v.Sign;
    for (int i = 0, Carry = 0; i < (int)c.size(); i++) {
      long long cur = c[i] + Carry;
      res.z.push_back((int)(cur % 1000000));
      Carry = (int)(cur / 1000000);
    }
    res.z = convertBASE(res.z, 6, BASEDIGITS);
    res.Trim();
    return res;
  }

  bigInt  sqrt(const bigInt& a1) {
    bigInt a = a1;
    while (a.z.empty() || a.z.size() % 2 == 1) a.z.push_back(0);

    int n = a.z.size();

    int firstDigit = (int)::sqrt((double)a.z[n - 1] *  bigInt::BASE + a.z[n - 2]);
    int Norm =  bigInt::BASE / (firstDigit + 1);
    a *= Norm;
    a *= Norm;
    while (a.z.empty() || a.z.size() % 2 == 1) a.z.push_back(0);

    bigInt r = (long long)a.z[n - 1] *  bigInt::BASE + a.z[n - 2];
    firstDigit = (int)::sqrt((double)a.z[n - 1] *  bigInt::BASE + a.z[n - 2]);
    int q = firstDigit;
    bigInt res;

    for (int j = n / 2 - 1; j >= 0; j--) {
      for (;; --q) {
        bigInt r1 =
            (r - (res * 2 *  bigInt::BASE + q) * q) *  bigInt::BASE *  bigInt::BASE +
            (j > 0 ? (long long)a.z[2 * j - 1] *  bigInt::BASE + a.z[2 * j - 2] : 0);
        if (r1 >= 0) {
          r = r1;
          break;
        }
      }
      res *=  bigInt::BASE;
      res += q;

      if (j > 0) {
        int d1 = res.z.size() + 2 < r.z.size() ? r.z[res.z.size() + 2] : 0;
        int d2 = res.z.size() + 1 < r.z.size() ? r.z[res.z.size() + 1] : 0;
        int d3 = res.z.size() < r.z.size() ? r.z[res.z.size()] : 0;
        q = (int)(((long long)d1 *  bigInt::BASE *  bigInt::BASE + (long long)d2 *  bigInt::BASE + d3) /
                  (firstDigit * 2));
      }
    }

    res.Trim();
    return res / Norm;
  }

  bigInt  bigInt::operator/(const bigInt& v) const { return divMod(*this, v).first; }

  bigInt  bigInt::operator%(const bigInt& v) const { return divMod(*this, v).second; }

  bigInt&  bigInt::operator/=(int v) {
    if (v < 0) Sign = -Sign, v = -v;
    for (int i = (int)z.size() - 1, rem = 0; i >= 0; --i) {
      long long cur = z[i] + rem * (long long)BASE;
      z[i] = (int)(cur / v);
      rem = (int)(cur % v);
    }
    Trim();
    return *this;
  }

  bigInt  bigInt::operator/(int v) const { return bigInt(*this) /= v; }

//   int  bigInt::operator%(int v) const {
//     if (v < 0) v = -v;
//     int m = 0;
//     for (int i = (int)z.size() - 1; i >= 0; --i)
//       m = (int)((z[i] + m * (long long)BASE) % v);
//     return m * Sign;
//   }

  bigInt&  bigInt::operator*=(const bigInt& v) {
    *this = *this * v;
    return *this;
  }

  bigInt&  bigInt::operator/=(const bigInt& v) {
    *this = *this / v;
    return *this;
  }

  bool  bigInt::operator<(const bigInt& v) const {
    if (Sign != v.Sign) return Sign < v.Sign;
    if (z.size() != v.z.size()) return z.size() * Sign < v.z.size() * v.Sign;
    for (int i = (int)z.size() - 1; i >= 0; i--)
      if (z[i] != v.z[i]) return z[i] * Sign < v.z[i] * Sign;
    return false;
  }

  bool  bigInt::operator>(const bigInt& v) const { return v < *this; }

  bool  bigInt::operator<=(const bigInt& v) const { return !(v < *this); }

  bool  bigInt::operator>=(const bigInt& v) const { return !(*this < v); }

  bool  bigInt::operator==(const bigInt& v) const {
    return !(*this < v) && !(v < *this);
  }

  bool  bigInt::operator!=(const bigInt& v) const { return *this < v || v < *this; }

   bigInt operator-(bigInt v) {
    if (!v.z.empty()) v.Sign = -v.Sign;
    return v;
  }

  std::istream&  operator>>(std::istream& stream, bigInt& v) {
    std::string s;
    stream >> s;
    v.read(s);
    return stream;
  }

  std::ostream&  operator<<(std::ostream& stream, const bigInt& v) {
    if (v.Sign == -1) stream << '-';
    stream << (v.z.empty() ? 0 : v.z.back());
    for (int i = (int)v.z.size() - 2; i >= 0; --i)
      stream << std::setw(bigInt::BASEDIGITS) << std::setfill('0') << v.z[i];
    return stream;
  }

  // 判断是否为0
  bool  bigInt::isZero() const { return z.empty(); }
  // 绝对值
  bigInt  bigInt::Abs() const { return Sign == 1 ? *this : -*this; }
  // 将其转为long long类型
  long long  bigInt::longValue() const {
    long long res = 0;
    for (int i = (int)z.size() - 1; i >= 0; i--) res = res * BASE + z[i];
    return res * Sign;
  }
  // 计算最大公因数
  bigInt  bigInt::Gcd(const bigInt& a, const bigInt& b) {
    return b.isZero() ? a : Gcd(b, a % b);
  }
  // 计算最小公倍数
  bigInt  bigInt::Lcm(const bigInt& a, const bigInt& b) {
    return a / Gcd(a, b) * b;
  }
  // 快速幂
  bigInt  bigInt::Power(const bigInt& Base, const bigInt& Exponent) {
    bigInt a = Base;
    bigInt b = Exponent;

    if (b == 0) {
      return 1;
    }
    bigInt Result = 1;
    while (b > 0) {
      if (b % 2 == 1) {
        Result *= a;
      }
      a *= a;
      b /= 2;
    }
    return Result;
  }
};

  // namespace kuinkerm
#endif