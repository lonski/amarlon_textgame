#ifndef COMOBJ_H
#define COMOBJ_H

struct Damage
{
  int piercing;
  int slashing;
  int bashing;

  Damage(int p = 0, int s = 0, int b = 0) : piercing(p), slashing(s), bashing(b) {}
};

#endif // COMOBJ_H
