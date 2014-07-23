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
            _content.push_back(section);
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
      _content.push_back(section);
      section.clear();
    }

    file.close();
  }
}

void INIFile::save()
{
  if (!filename.empty())
  {
    ofstream file(filename, ios::out | ios::trunc);

    for (auto s = _content.begin(); s != _content.end(); ++s)
    {
      Section& section = *s;
      file << "[" << section.name << "]" << endl;
      for (auto k = section.values.begin(); k != section.values.end(); ++k)
      {
        file << k->first << "=" << k->second << endl;
      }
    }

    file.close();
  }
}

std::vector<INIFile::Section>::iterator INIFile::findSection(SectionName sectionName)
{
  auto r = _content.end();

  for (auto s = _content.begin(); s!=_content.end(); ++s)
  {
    if (s->name == sectionName)
    {
      r = s;
      break;
    }
  }

  return r;
}

INIFile::Value INIFile::getValue(INIFile::SectionName sectionName, INIFile::Key key)
{
  Value r;
  auto sectionIter = findSection(sectionName);
  if (sectionIter != _content.end())
  {
    r = sectionIter->values[key];
  }

  return r;
}

bool INIFile::setValue(INIFile::SectionName sectionName, INIFile::Key key, INIFile::Value value)
{
  bool r = false;
  auto sectionIter = findSection(sectionName);
  if (sectionIter != _content.end())
  {
    sectionIter->values[key] = value;
    r = true;
  }
  else
  {
    Section s;
    s.name = sectionName;
    s.values[key] = value;
    _content.push_back(s);
  }

  return r;
}

bool INIFile::eraseSection(INIFile::SectionName sectionName)
{
  auto sectionIter = findSection(sectionName);
  if (sectionIter != _content.end())
  {
    _content.erase(sectionIter);
    return true;
  }

  return false;
}
