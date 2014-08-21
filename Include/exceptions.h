#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

namespace error
{

class creation_error : public std::logic_error {
public:
  explicit creation_error (const std::string& what_arg): std::logic_error(what_arg) {}
};

class container_insertion_error : public std::logic_error {
public:
  explicit container_insertion_error (const std::string& what_arg): std::logic_error(what_arg) {}
};

class no_ref : public std::logic_error {
public:
  explicit no_ref (const std::string& what_arg): std::logic_error(what_arg) {}
};

class equip_no_bodyparts : public std::logic_error {
public:
  explicit equip_no_bodyparts (const std::string& what_arg): std::logic_error(what_arg) {}
};

class execution_error : public std::logic_error {
public:
  explicit execution_error (const std::string& what_arg): std::logic_error(what_arg) {}
};

class cloning_error : public std::logic_error {
public:
  explicit cloning_error (const std::string& what_arg = ""): std::logic_error(what_arg) {}
};


}

#endif // EXCEPTIONS_H
