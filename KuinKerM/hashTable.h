#ifndef HASHTABLEH
#define HASHTABLEH

#include "../kuinkerm.h"

/*  哈希
 */
namespace kuinkerm {

  // 插入一条哈希数据
  template <typename T1, typename T2>
  void hashTable<T1, T2>::Insert(T1 x, T2 y) const { hTable.insert(std::make_pair(x, y)); }

  // 用下标访问哈希数据，如 table[x] 就是键值的对应数据
  template <typename T1, typename T2>
  T2& hashTable<T1, T2>::operator[](T1 x) { return hTable[x]; }
  template <typename T1, typename T2>
  const T2& hashTable<T1, T2>::operator[](T1 x) const { return hTable[x]; }

  // 返回一个指针，first为键值，second为数据
  template <typename T1, typename T2>
  typename hashTable<T1, T2>::htIter hashTable<T1, T2>::Find(T1 x) const { return hTable.find(x); }
  template <typename T1, typename T2>
  typename hashTable<T1, T2>::htIter hashTable<T1, T2>::notFound() const { return hTable.end(); }
  // 删除键值x的哈希数据
  template <typename T1, typename T2>
  void hashTable<T1, T2>::Erase(T1 x) const { hTable.erase(x); }
  template <typename T1, typename T2>
  void hashTable<T1, T2>::Clear() const { hTable.clear(); }
  template <typename T1, typename T2>
  int hashTable<T1, T2>::Size() const { return hTable.size(); }
  template <typename T1, typename T2>
  bool hashTable<T1, T2>::Empty() const { return hTable.empty(); }

}  // namespace kuinkerm
#endif