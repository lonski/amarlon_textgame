#include "gameclock.h"

GameClock* GameClock::_clock = nullptr;

GameClock::GameClock()
: _minute(0), _hour(0), _day(0), _month(0), _year(0), _season(Season::Null)
{
}

GameClock &GameClock::Clock()
{
  if (nullptr == _clock) _clock = new GameClock;
  return *_clock;
}

void GameClock::tick_time(Minute tick)
{
  if ( (_minute+tick) < 60)
  //increase minutes
  {
    _minute += tick;
  }
  else
  //increase hours
  {
    Minute to_60 = 60 - _minute;
    tick -= to_60;

    Minute tick_hour_modulo = tick % 60;
    _minute = tick_hour_modulo;
    tick -= tick_hour_modulo;

    Hour hour_to_inc = 1 + tick/60;

    if (_hour + hour_to_inc < 24)
    {
      _hour += hour_to_inc;
    }
    else
    //increase days
    {
      Hour to_24 = 24 - _hour;
      hour_to_inc -= to_24;

      Hour days_modulo = hour_to_inc % 24;
      _hour = days_modulo;
      hour_to_inc -= days_modulo;

      Day days_to_inc = 1 + hour_to_inc / 24;

      if (_day + days_to_inc < 30)
      {
        _day += days_to_inc;
      }
      else
      //increase months
      {
        Day to_30 = 30 - _day;
        days_to_inc -= to_30;

        Day months_modulo = days_to_inc % 30;
        _day = months_modulo;
        days_to_inc -= months_modulo;

        Month months_to_inc = 1 + days_to_inc / 30;

        if (_month + months_to_inc < 12)
        {
          _month += months_to_inc;
        }
        else
        //increase years
        {
          Month to_12 = 12 - _month;
          months_to_inc -= to_12;

          Month years_modulo = months_to_inc % 12;
          _month = years_modulo;
          months_to_inc -= years_modulo;

          Year years_to_inc = 1 + months_to_inc / 12;

          _year += years_to_inc;
        }
      }
    }
  }

  _last_tick = tick;
  oNotify();
}



