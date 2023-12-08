#ifndef MULTISETH
#define MULTISETH

#include "kuinkerm.h"

/*  旋转树
 */
namespace kuinkerm {

  template <typename T, typename F>
  multiSet<T, F>::multiSet() {
    isFundamentalType = std::is_fundamental<T>::value;
    wantUseTemplate = !std::is_void<F>::value;
    if (false == isFundamentalType && false == wantUseTemplate) {
      throw std::runtime_error(
          "multiSet, input type is not the fundamental type and the "
          "comparision function or struct is not provided.");
      return;
    } else if (true == wantUseTemplate)
      Ms = std::make_unique<mSType>();
    else
      Ms = std::make_unique<mSType>();
  }

  template <typename T, typename F>
  multiSet<T, F>::multiSet(std::function<bool(const T&, const T&)> f) : userComparator(f) {
    isFundamentalType = std::is_fundamental<T>::value;
    wantUseTemplate = !std::is_void<F>::value;
    if (false == isFundamentalType && true == wantUseTemplate) {
      throw std::runtime_error(
          "multiSet, both comparision function and struct are existed, delete "
          "one of them");
      return;
    }
    Ms = std::make_unique<__gnu_pbds::tree<
        PT, __gnu_pbds::null_type, std::function<bool(const PT&, const PT&)>,
        mSTag, __gnu_pbds::tree_order_statistics_node_update>>(
        [f](const PT& x, const PT& y) -> bool {
          if (f(x.first, y.first) ^ f(y.first, x.first))
            return f(x.first, y.first);
          else
            return (bool)(x.second < y.second);
        });
  }

  template <typename T, typename F>
  int multiSet<T, F>::Size() { return Ms->size(); }

  template <typename T, typename F>
  bool multiSet<T, F>::Empty() { return Ms->empty(); }

  template <typename T, typename F>
  void multiSet<T, F>::Clear() { Ms->clear(); }

  template <typename T, typename F>
  void multiSet<T, F>::Insert(T x) { Ms->insert({x, ++Dfn}); }

  template <typename T, typename F>
  void multiSet<T, F>::Erase(T x) { Ms->erase(Ms->lower_bound({x, 0})); }

  template <typename T, typename F>
  int multiSet<T, F>::isRank(T x) { return Ms->order_of_key({x, 0}) + 1; }

  template <typename T, typename F>
  T multiSet<T, F>::rankIs(int a) { return Ms->find_by_order(a - 1)->first; }

  template <typename T, typename F>
  T multiSet<T, F>::Prev(T x) { return Ms->prev(Ms->lower_bound({x, 0}))->first; }

  // 返回第一个大于等于x的元素
  template <typename T, typename F>
  T multiSet<T, F>::lowerBound(T x) { return Ms->lower_bound({x, 0})->first; }

  // 返回第一个大于x的元素
  template <typename T, typename F>
  T multiSet<T, F>::upperBound(T x) { return Ms->upper_bound({x, INF})->first; }

  // 确保被合并set的所有元素大于合并set的所有元素，合并后清空
  template <typename T, typename F>
  void multiSet<T, F>::Join(multiSet& otherMs) {
    if (userComparator(otherMs.rankIs(1), this->rankIs(this->Size())))
      throw std::runtime_error(
          "multiSet, Join failed. The necessary condition for merging two sets "
          "is that all elements in the merging set must be smaller than all "
          "elements in the target set.");
    Ms->join(*otherMs.Ms);
  }

  // 分裂，小于x的元素留住，大于等于x的元素分裂出去
  template <typename T, typename F>
  void multiSet<T, F>::Split(T x, multiSet& otherMs) {
    otherMs.Clear();
    Ms->split({x, 0}, *otherMs.Ms);
  }

}  // namespace kuinkerm
#endif