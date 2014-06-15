#ifndef COMOBJ_H
#define COMOBJ_H

typedef double Weight;

template<typename T>
struct CMValue
{
  T max;
  T cur;
  CMValue(T m = 0, T c = 0) : max(m), cur(c) {}
};

struct Damage
{
  int piercing;
  int slashing;
  int bashing;

  Damage(int p = 0, int s = 0, int b = 0) : piercing(p), slashing(s), bashing(b) {}
};

#endif // COMOBJ_H
