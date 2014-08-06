#ifndef OBSERVER_H
#define OBSERVER_H

#include <set>

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

#endif // OBSERVER_H
