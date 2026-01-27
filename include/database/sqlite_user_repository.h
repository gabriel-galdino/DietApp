#ifndef DIETAPP_INCLUDE_DATABASE_SQLITE_USER_REPOSITORY_H
#define DIETAPP_INCLUDE_DATABASE_SQLITE_USER_REPOSITORY_H

#include <string>

#include "core/ports/repository/user_repository.h"

class SQLiteUserRepository : public UserRepository {
 public:
  void GetUser() override;

  bool ValidateCredentials() override;

 private:
  std::string name_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_USER_REPOSITORY_H
