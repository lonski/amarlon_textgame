#include "common_utils.h"

using namespace std;

Profession fun::skill_group(Skill skill)
{
  const int s = static_cast<int>(skill);
  const int group_cnt = 7;

  for (int i = 1; i < group_cnt; ++i)
  {
    int low_bound = i*10;
    int high_bound = low_bound + 10;

    if (s > low_bound && s < high_bound ) return static_cast<Profession>(i);
  }

  return Profession::Null;
}
