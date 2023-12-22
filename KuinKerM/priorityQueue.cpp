#ifndef PRIORITYQUEUECPP
#define PRIORITYQUEUECPP

#include "../kuinkerm.h"

/*  优先队列
 */
namespace kuinkerm {

template <typename T, typename F>
priorityQueue<T, F>::priorityQueue() {
  isFundamentalType = std::is_fundamental<T>::value;
  wantUseTemplate = !std::is_void<F>::value;
  if (false == isFundamentalType && false == wantUseTemplate) {
    throw std::runtime_error(
        "priorityQueue, input type is not the fundamental type and the "
        "comparision function or struct is not provided.");
    return;
  } else if (true == wantUseTemplate)
    Pq = std::make_unique<pqType>();
  else
    Pq = std::make_unique<__gnu_pbds::priority_queue<T, Comparator, heapTag>>(
        [](const T& a, const T& b) -> bool { return a < b; });
}

template <typename T, typename F>
priorityQueue<T, F>::priorityQueue(Comparator f) {
  isFundamentalType = std::is_fundamental<T>::value;
  wantUseTemplate = !std::is_void<F>::value;
  if (true == wantUseTemplate) {
    throw std::runtime_error(
        "priorityQueue, both comparision function and struct are existed, "
        "delete one of them");
    return;
  }
  Pq = std::make_unique<__gnu_pbds::priority_queue<T, Comparator, heapTag>>(f);
}

template <typename T, typename F>
void priorityQueue<T, F>::Push(T x) {
  Pq->push(x);
}

template <typename T, typename F>
void priorityQueue<T, F>::Pop() {
  Pq->pop();
}

template <typename T, typename F>
T priorityQueue<T, F>::Top() {
  return Pq->top();
}

template <typename T, typename F>
int priorityQueue<T, F>::Size() {
  return Pq->size();
}

template <typename T, typename F>
bool priorityQueue<T, F>::Empty() {
  return Pq->empty();
}

template <typename T, typename F>
void priorityQueue<T, F>::Claer() {
  Pq->clear();
}

// 传入一个指针，将Iter指向的值改为x后，再重新维护堆
template <typename T, typename F>
void priorityQueue<T, F>::Modify(pqIter Iter, const T x) {
  Pq->modify(Iter, x);
}

// 将otherPq并入调用该方法的堆，并清空otherPq
template <typename T, typename F>
void priorityQueue<T, F>::Join(priorityQueue& otherPq) {
  Pq->join(*otherPq.Pq);
}

} 
#endif