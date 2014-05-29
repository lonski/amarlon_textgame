#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "inc.h"

class creation_error : public std::logic_error {
public:
  explicit creation_error (const std::string& what_arg): std::logic_error(what_arg) {}
};

#endif // EXCEPTIONS_H