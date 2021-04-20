# deque

```c++
const int MAXN=1e5+5;
struct deQue
{
    int buffer[MAXN*2];
    int head=MAXN,tail=MAXN-1;
    bool rev;
    bool empty()
    {
        return tail<head;
    }
    int size()
    {
        return tail-head+1;
    }
    int front()
    {
        return rev?buffer[tail]:buffer[head];
    }
    int back()
    {
        return rev?buffer[head]:buffer[tail];
    }
    void push_front(int x)
    {
        rev?buffer[++tail]=x:buffer[--head]=x;
    }
    void push_back(int x)
    {
        rev?buffer[--head]=x:buffer[++tail]=x;
    }
    void pop_back()
    {
        rev?head++:tail--;
    }
    void pop_front()
    {
        rev?tail--:head++;
    }
    void reverse()
    {
        rev^=1;
    }
}q;
```

