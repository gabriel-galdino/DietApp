#ifndef DIETAPP_INCLUDE_DATABASE_DATABASE_RESULT_SET_H
#define DIETAPP_INCLUDE_DATABASE_DATABASE_RESULT_SET_H

#include <string>

class DatabaseResultSet {
 public:
  virtual ~DatabaseResultSet() = default;
  virtual bool NextRow() = 0;
  virtual int GetInt(const std::string& column_name) = 0;
  virtual std::string GetString(const std::string& column_name) = 0;
  virtual double GetDouble(const std::string& column_name) = 0;
};

#endif  // DIETAPP_INCLUDE_DATABASE_DATABASE_RESULT_SET_H
