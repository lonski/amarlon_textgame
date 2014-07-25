#include "inifile.h"
#include <iostream>
#include <fstream>

using namespace std;

INIFile::INIFile(Filename file)
  : filename(file)
{
  load();
}

INIFile::INIFile()
{
}

void INIFile::open(INIFile::Filename file)
{
  filename = file;
  load();
}

void INIFile::load()
{
  if (!filename.empty())
  {
    ifstream file(filename, ifstream::in);

    string line;
    Section section;

    while(file.good())
    {
      getline(file, line);
      if (!line.empty())
      {
        if ( line.find('[') < line.size() ) //is new section
        {
          if (!section.name.empty())
          {
            _content[section.name] = section;
            section.clear();
          }

          size_t n = -1;
          while(++n < line.size())
            if ( line[n] != '[' && line[n] != ']' && line[n] != ' ' )
              section.name += line[n];

        }
        else if ( line.find('=') < line.size() ) //is value
        {
          Key key;
          Value val;

          size_t n = -1;
          while(++n < line.size() && line[n] != '=')
            key += line[n];

          while(++n < line.size())
            val += line[n];

          section.values[key] = val;
        }
      }
    }

    if (!section.name.empty())
    {
      _content[section.name] = section;
      section.clear();
    }
  }
}

void INIFile::save()
{
  if (!filename.empty())
  {
    ofstream ofs(filename, std::ios::trunc);

    for (auto s = _content.begin(); s != _content.end(); ++s)
    {
      Section& section = s->second;
      ofs << "[" << section.name << "]" << endl;
      for (auto k = section.values.begin(); k != section.values.end(); ++k)
      {
        ofs << k->first << "=" << k->second << endl;
      }
    }

    }
}

void INIFile::clear()
{
  _content.clear();
}

void INIFile::reload()
{
  clear();
  open(filename);
}

INIFile::Value INIFile::getValue(INIFile::SectionName sectionName, INIFile::Key key)
{
  Value r;
  auto sectionIter = _content.find(sectionName);
  if (sectionIter != _content.end())
  {
    r = sectionIter->second.values[key];
  }

  return r;
}

bool INIFile::setValue(INIFile::SectionName sectionName, INIFile::Key key, INIFile::Value value)
{
  bool r = false;
  auto sectionIter = _content.find(sectionName);
  if (sectionIter != _content.end())
  {
    sectionIter->second.values[key] = value;
    r = true;
  }
  else
  {
    Section s;
    s.name = sectionName;
    s.values[key] = value;
    _content[s.name] = s;
  }

  return r;
}

bool INIFile::eraseSection(INIFile::SectionName sectionName)
{
  auto sectionIter = _content.find(sectionName);
  if (sectionIter != _content.end())
  {
    _content.erase(sectionIter);
    return true;
  }

  return false;
}

std::vector<string> INIFile::getSections()
{
  vector<string> r;
  for (auto s = _content.begin(); s != _content.end(); ++s)
  {
    r.push_back(s->second.name);
  }

  return r;
}
