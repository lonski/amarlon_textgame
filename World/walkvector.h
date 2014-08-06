#ifndef WALKVECTOR_H
#define WALKVECTOR_H

#include "Include/enums/e_direction.h"


class WalkVector{
private:
  short _vals[4];
  static short indexof(Direction dir);

public:
  WalkVector(short range = 0);
  WalkVector(short north, short south, short east, short west );
  void inc(Direction dir, short val = 1);
  void dec(Direction dir, short val = 1);
  short operator[](Direction dir) const;

};

#endif // WALKVECTOR_H
