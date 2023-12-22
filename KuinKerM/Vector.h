#ifndef VECTORH
#define VECTORH

#include "../kuinkerm.h"

/*  可变长数组
 */
namespace kuinkerm {

  template <typename T>
  Vector<T>::Vector() {}
  template <typename T>
  Vector<T>::Vector(__gnu_cxx::rope<T> Rp) : Rp(Rp) {}
  
  template <typename T>
  const T Vector<T>::operator[](std::size_t index) const { return Rp[index]; }

  template <typename T>
  void Vector<T>::pushBack(T x) { Rp.push_back(x); }
  template <typename T>
  void Vector<T>::popBack(T x) { Rp.pop_back(); }

  // 从 Pos 位置开始插入 x
  template <typename T>
  void Vector<T>::Insert(int Pos, Vector<T> x) { Rp.insert(Pos, x); }

  // 从 Pos 位置开始删掉长度为 Len 的元素
  template <typename T>
  void Vector<T>::Erase(int Pos, int Len) { Rp.erase(Pos, Len); }

  // 从 Pos 位置开始提取长度为 Len 的子串，返回一个 Vector
  template <typename T>
  Vector<T> Vector<T>::Substr(int Pos, int Len) { return Vector<T>(Rp.substr(Pos, Len)); }

  // 替换 Pos 处的元素为 x
  template <typename T>
  void Vector<T>::Replace(int Pos, T x) { Rp.replace(Pos, x); }

  // 从 Pos 位置开始替换为 x
  template <typename T>
  void Vector<T>::Replace(int L, Vector<T> x) { Rp.replace(L, x.Size(), x); }
  template <typename T>
  void Vector<T>::Clear() { Rp.clear(); }
  template <typename T>
  int Vector<T>::Size() { return Rp.size(); }
  template <typename T>
  bool Vector<T>::Empty() { return Rp.empty(); }
  
  template <typename T>
  Vector<T> Vector<T>::operator+(const Vector<T>& other) const {
    return Vector<T>(Rp + other.Rp);
  }
}  // namespace kuinkerm
#endif