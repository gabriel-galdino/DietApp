#ifndef DIETAPP_INCLUDE_DATABASE_DATABASE_STATEMENT_H
#define DIETAPP_INCLUDE_DATABASE_DATABASE_STATEMENT_H

#include <string>

class DatabaseStatement {
 public:
  virtual ~DatabaseStatement() = default;

  virtual void Bind(const std::string& param_name,
                    const std::string& value) = 0;
  virtual void Bind(const std::string& param_name, int value) = 0;
  // virtual void Bind(int index, double value) = 0;

  virtual bool Execute() = 0;
};

#endif  // DIETAPP_INCLUDE_DATABASE_DATABASE_STATEMENT_H
