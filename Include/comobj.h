#ifndef COMOBJ_H
#define COMOBJ_H

#include "inc.h"

typedef double Weight;

//time
typedef unsigned int Minute;
typedef unsigned int Hour;
typedef unsigned int Day;
typedef unsigned int Month;
typedef unsigned int Year;

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
  void clear() { piercing = 0; slashing = 0; bashing = 0;}
};

class Subject;
class Observer
{
protected:
  Subject* _subject;
public:
  Observer(Subject* subject) : _subject(subject) {}
  virtual void update(Subject* subject) = 0;
  virtual ~Observer() {}
};

class Subject
{
private:
  std::set<Observer*> _observers;
public:
  virtual ~Subject() {}
  virtual void oNotify()
  {
    for (auto o = _observers.begin(); o != _observers.end(); ++o)
      (*o)->update(this);
  }
  virtual void oAdd(Observer* observer)
  {
    _observers.insert(observer);
  }
  virtual void oRemove(Observer* observer)
  {
    _observers.erase(observer);
  }
};

#endif // COMOBJ_H
