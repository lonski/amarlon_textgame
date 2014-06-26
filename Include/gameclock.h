#ifndef GAMECLOCK_H
#define GAMECLOCK_H

#include "inc.h"
#include "comobj.h"
#include "enums.h"

class GameClock : public Subject
{
private:
  GameClock();
  GameClock(const GameClock&);
  GameClock& operator=(const GameClock&);

  static GameClock* _clock;

  Minute _minute;
  Hour _hour;
  Day _day;
  Month _month;
  Year _year;
  Season _season;

  Minute _last_tick;
public:
  static GameClock& Clock();
  void tick_time(Minute tick = 1);

  Minute minute() const { return _minute; }
  Hour hour() const { return _hour; }
  Day day() const { return _day; }
  Month month() const { return _month; }
  Year year() const { return _year; }
  Season season() const { return _season; }
  Minute last_tick() const { return _last_tick; }
};

class TimeObserver : Observer
{
public:
  TimeObserver() : Observer(&GameClock::Clock())
  {
    GameClock::Clock().oAdd(this);
  }
  virtual void update(Subject *subject)
  {
    if (subject == _subject)
    {
      tick_time( GameClock::Clock().last_tick() );
    }
  }
  virtual void tick_time(Minute tick) = 0;
};

#endif // GAMECLOCK_H
