#ifndef ENUMS_H
#define ENUMS_H

//===
enum class Directions{
  Null      = 0,
  North     = 1,
  South     = 2,
  West      = 3,
  East      = 4,
  Northeast = 5,
  Northwest = 6,
  Southeast = 7,
  Southwest = 8,
  Up        = 9,
  Down      = 10,
  End       = 11
};

Directions& operator++( Directions& d ) {
  d = static_cast<Directions>( static_cast<int>(d) + 1 );
  return d;
}
//~~~

#endif // ENUMS_H
