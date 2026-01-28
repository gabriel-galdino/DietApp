#ifndef DIETAPP_INCLUDE_DATABASE_SQLITE_USER_REPOSITORY_H
#define DIETAPP_INCLUDE_DATABASE_SQLITE_USER_REPOSITORY_H

#include <memory>
#include <set>
#include <string>

#include "core/domain/user.h"
#include "core/ports/repository/user_repository.h"
#include "database_adapter.h"

constexpr const char* kInsertUserSql = R"sql(
  INSERT INTO users(name)
  VALUES (:name)
)sql";

class SQLiteUserRepository : public UserRepository {
 public:
  SQLiteUserRepository(std::shared_ptr<DatabaseAdapter> db_adapter);

  User& GetUser(const std::string& name) override;

  bool AddUser(const User& user) override;

  bool DelUser(const User& user) override;

  bool UpdateUser(const User& user) override;

  bool ValidateCredentials() override;

 private:
  std::set<User> users_;
  std::shared_ptr<DatabaseAdapter> db_adapter_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_USER_REPOSITORY_H
