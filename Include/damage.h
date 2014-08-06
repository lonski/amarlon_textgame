#ifndef DAMAGE_H
#define DAMAGE_H

struct Damage
{
  int piercing;
  int slashing;
  int bashing;

  Damage(int p = 0, int s = 0, int b = 0) : piercing(p), slashing(s), bashing(b) {}
  void clear() { piercing = 0; slashing = 0; bashing = 0;}
};

#endif // DAMAGE_H
