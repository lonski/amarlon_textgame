#ifndef INIFILE_H
#define INIFILE_H

#include <string>
#include <vector>
#include <map>

#include "func.h"

class INIFile
{
public:
  typedef std::string Filename;
  typedef std::string Key;
  typedef std::string Value;
  typedef std::string SectionName;

  struct Section
  {
    std::string name;
    std::map<Key, Value> values;
    Section()
    {
      clear();
    }
    void clear()
    {
      name = "";
      values.clear();
    }
  };

private:
  std::map<std::string, Section> _content;
  Filename filename;

public:
  INIFile(Filename file);
  INIFile();
  void open(Filename file);
  void load();
  void save();

  Value getValue(SectionName sectionName, Key key);
  bool setValue(SectionName sectionName, Key key, Value value);
  bool eraseSection(SectionName sectionName);
  std::vector<std::string> getSections();
};

#endif // INIFILE_H
