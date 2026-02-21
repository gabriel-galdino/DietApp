#ifndef DIETAPP_INCLUDE_DATABASE_SQLITE_USER_REPOSITORY_H
#define DIETAPP_INCLUDE_DATABASE_SQLITE_USER_REPOSITORY_H

#include <algorithm>
#include <memory>
#include <set>
#include <string>

#include "core/domain/user.h"
#include "core/ports/repository/user_repository.h"
#include "database_adapter.h"

constexpr const char* kInsertUserSql = R"sql(
  INSERT INTO users(username, display_name)
  VALUES (:username, :display_name)
)sql";

constexpr const char* kLoadAllUsers = R"sql(
  SELECT * FROM users
)sql";

class SQLiteUserRepository : public UserRepository {
 public:
  SQLiteUserRepository(std::shared_ptr<DatabaseAdapter> db_adapter);

  bool Exists(const std::string& username) const override;

  const User& GetUser(const std::string& name) const override;

  bool AddUser(User& user) override;

  bool DelUser(const User& user) override;

  bool UpdateUser(const User& user) override;

  bool ValidateCredentials() override;

  bool LoadAllUsers() override;

 private:
  auto FindByName(const std::string& name) const {
    return std::find_if(users_.begin(), users_.end(),
                        [&name](const User& user) {        //
                          return user.username() == name;  //
                        });
  }

  std::set<User> users_;
  std::shared_ptr<DatabaseAdapter> db_adapter_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_USER_REPOSITORY_H
