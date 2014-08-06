#include "string_utils.h"
#include <algorithm>

using namespace std;

std::vector<string> fun::explode(string str, char ch)
{
  vector<string> result;
  string line;

  for (auto s = str.begin(); s != str.end(); ++s)
  {
    if (*s == ch)
    {
      if (!line.empty())
      {
        result.push_back(line);
        line.clear();
      }
    }
    else
    {
      line += *s;
    }
  }

  if (!line.empty())
  {
    result.push_back(line);
    line.clear();
  }

  return result;
}


std::string fun::toLower(string str)
{
  string r(str);
  std::transform(r.begin(), r.end(), r.begin(), ::tolower);
  return r;
}

QColor fun::Str2Color(string rgb_color)
{
  QColor kolor;
  vector<string> colors = fun::explode(rgb_color, ';');
  if (colors.size() == 3)
  {
    int r = fun::fromStr<int>(colors[0]);
    int g = fun::fromStr<int>(colors[1]);
    int b = fun::fromStr<int>(colors[2]);
    kolor = QColor(r,g,b);
  }

  return kolor;
}
