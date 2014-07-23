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
  std::vector<Section> _content;
  Filename filename;
  std::vector<Section>::iterator findSection(INIFile::SectionName sectionName);

public:
  INIFile(Filename file);
  INIFile();
  void open(Filename file);
  void load();
  void save();

  Value getValue(SectionName sectionName, Key key);
  bool setValue(SectionName sectionName, Key key, Value value);
  bool eraseSection(SectionName sectionName);
};

#endif // INIFILE_H
