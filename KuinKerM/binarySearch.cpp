#ifndef BINARYSEARCHCPP
#define BINARYSEARCHCPP

#include "../kuinkerm.h"
#include <functional>

/* 二分判定
    纯虚函数
*/
namespace kuinkerm {

template <typename T>
T binarySearch<T>::leftIntBSearch(T l, T r, std::function<bool(T)> Judge) {
  r += 1;
  while (l < r) {
    T Mid = (l + r) / 2;
    if (Judge(Mid))
      r = Mid;
    else
      l = Mid + 1;
  }
  return l;
}

template <typename T>
T binarySearch<T>::rightIntBSearch(T l, T r, std::function<bool(T)> Judge) {
  l -= 1;
  while (l < r) {
    T Mid = (l + r + 1) / 2;
    if (Judge(Mid))
      l = Mid;
    else
      r = Mid - 1;
  }
  return l;
}

template <typename T>
T binarySearch<T>::leftFloBSearch(T l, T r, std::function<bool(T)> Judge) {
  // 若精度不容易确定或表示，可以采用循环固定次数的二分方法
  // for(int i = 0;i < 100;i ++)
  while (l + EPS < r) {
    T Mid = (l + r) / 2;
    if (Judge(Mid))
      r = Mid;
    else
      l = Mid;
  }
  return l;
}

template <typename T>
T binarySearch<T>::rightFloBSearch(T l, T r, std::function<bool(T)> Judge) {
  // 若精度不容易确定或表示，可以采用循环固定次数的二分方法
  // for(int i = 0;i < 100;i ++)
  while (l + EPS < r) {
    T Mid = (l + r) / 2;
    if (Judge(Mid))
      l = Mid;
    else
      r = Mid;
  }
  return l;
}

template <typename T>
binarySearch<T>::binarySearch() {}

template <typename T>
binarySearch<T>::~binarySearch(){};

template <typename T>
T binarySearch<T>::biSearch(const std::string dirString, T l, T r,
                            std::function<bool(T)> Judge) {
  if (dirString == "GOLEFT" &&
      (typeid(T) == typeid(bigInt) || std::is_integral<T>::value))
    return leftIntBSearch(l, r, Judge);
  else if (dirString == "GOLEFT" && std::is_floating_point<T>::value)
    return leftFloBSearch(l, r, Judge);
  else if (dirString == "GORIGHT" &&
           (typeid(T) == typeid(bigInt) || std::is_integral<T>::value))
    return rightIntBSearch(l, r, Judge);
  else if (dirString == "GORIGHT" && std::is_floating_point<T>::value)
    return rightFloBSearch(l, r, Judge);
  else
    throw std::runtime_error("binarySearch, input parameter ERROR");
  throw std::runtime_error("binarySearch, unnamed ERROR");
}
} 

#endif