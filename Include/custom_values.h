#ifndef COMOBJ_H
#define COMOBJ_H

#include <set>

template<typename T>
struct CMValue
{
  T max;
  T cur;
  CMValue(T m = 0, T c = 0) : max(m), cur(c) {}
};

template<typename T>
struct VLValue
{
  T val;
  T left;
  VLValue(T v = 0, T l = 0) : val(v), left(l) {}
};

template<typename T>
struct MCMValue
{
  T max;
  T cur;
  T min;
  MCMValue(T mn = 0, T c = 0, T mx = 0) : max(mx), cur(c), min(mn) {}
};

#endif // COMOBJ_H
