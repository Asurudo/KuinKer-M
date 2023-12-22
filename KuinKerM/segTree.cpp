#ifndef SEGTREECPP
#define SEGTREECPP

#include "../kuinkerm.h"

/* 线段树
 */
namespace kuinkerm {

  void segTree::privateBuild(int p, int l, int r) {
    ADD(p) = 0;
    L(p) = l, R(p) = r;

    if (l == r) {
      SUM(p) = MAX(p) = MIN(p) = a[l];
      return;
    }
    int mid = (l + r) / 2;

    privateBuild(LChild(p), l, mid);
    privateBuild(RChild(p), mid + 1, r);

    SUM(p) = SUM(LChild(p)) + SUM(RChild(p));
    MAX(p) = std::max(MAX(LChild(p)), MAX(RChild(p)));
    MIN(p) = std::min(MIN(LChild(p)), MIN(RChild(p)));
  }

  void segTree::privateSpread(int p) {
    if (ADD(p)) {
      SUM(LChild(p)) += ADD(p) * (R(LChild(p)) - L(LChild(p)) + 1);
      SUM(RChild(p)) += ADD(p) * (R(RChild(p)) - L(RChild(p)) + 1);

      MAX(LChild(p)) += ADD(p);
      MAX(RChild(p)) += ADD(p);
      MIN(LChild(p)) += ADD(p);
      MIN(RChild(p)) += ADD(p);

      ADD(LChild(p)) += ADD(p);
      ADD(RChild(p)) += ADD(p);
      ADD(p) = 0;
    }
  }

  void segTree::privateAdd(int p, int l, int r, ll d) {
    if (l <= L(p) && r >= R(p)) {
      SUM(p) += d * (R(p) - L(p) + 1);
      MAX(p) += d;
      MIN(p) += d;
      ADD(p) += d;
      return;
    }
    privateSpread(p);
    int mid = (L(p) + R(p)) / 2;

    if (l <= mid) privateAdd(LChild(p), l, r, d);

    if (r > mid) privateAdd(RChild(p), l, r, d);

    SUM(p) = SUM(LChild(p)) + SUM(RChild(p));
    MAX(p) = std::max(MAX(LChild(p)), MAX(RChild(p)));
    MIN(p) = std::min(MIN(LChild(p)), MIN(RChild(p)));
  }

  ll segTree::privateAskSum(int p, int l, int r) {
    if (l <= L(p) && r >= R(p)) return SUM(p);
    privateSpread(p);
    int mid = (L(p) + R(p)) / 2;
    ll val = 0;
    if (l <= mid) val += privateAskSum(LChild(p), l, r);
    if (r > mid) val += privateAskSum(RChild(p), l, r);
    return val;
  }
  ll segTree::privateAskMax(int p, int l, int r) {
    if (l <= L(p) && r >= R(p)) return MAX(p);
    privateSpread(p);
    int mid = (L(p) + R(p)) / 2;
    ll val = -INFLL;

    if (l <= mid) val = std::max(val, privateAskMin(LChild(p), l, r));
    if (r > mid) val = std::max(val, privateAskMin(RChild(p), l, r));
    return val;
  }
  ll segTree::privateAskMin(int p, int l, int r) {
    if (l <= L(p) && r >= R(p)) return MIN(p);
    privateSpread(p);
    int mid = (L(p) + R(p)) / 2;
    ll val = INFLL;

    if (l <= mid) val = std::min(val, privateAskMin(LChild(p), l, r));
    if (r > mid) val = std::min(val, privateAskMin(RChild(p), l, r));
    return val;
  }
  ll segTree::privateAskFirstLess(int p, int l, int r, ll x) {
    if (L(p) == R(p)) {
      if (MIN(p) <= x)
        return L(p);
      else
        return -1;
    }
    privateSpread(p);

    if (l <= L(p) && r >= R(p) && MIN(p) > x) return -1;

    int mid = (L(p) + R(p)) / 2;
    int jd = -1;
    if (l <= mid) jd = privateAskFirstLess(LChild(p), l, r, x);
    if (jd != -1) return jd;
    if (r > mid) jd = privateAskFirstLess(RChild(p), l, r, x);
    return jd;
  }

  segTree::segTree(const std::vector<ll>& inputA) {
    Tree.resize((inputA.size() << 2) + 1);
    a.resize(inputA.size() + 1);
    for (int i = 1; i <= (int)inputA.size(); i++) a[i] = inputA[i - 1];
    privateBuild(1, 1, inputA.size());
  }

  void segTree::Add(int l, int r, ll d) { privateAdd(1, l + 1, r + 1, d); }
  ll segTree::askSum(int l, int r) { return privateAskSum(1, l + 1, r + 1); }
  ll segTree::askMax(int p, int l, int r) { return privateAskMax(1, l + 1, r + 1); }
  ll segTree::askMin(int p, int l, int r) { return privateAskMin(1, l + 1, r + 1); }
  // 返回第一个小于等于x的下标
  // 返回-2就是没找到
  ll segTree::askFirstLess(int l, int r, ll x) {
    return privateAskFirstLess(1, l + 1, r + 1, x) - 1;
  }
} 

#endif