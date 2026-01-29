#include "database/sqlite_user_repository.h"

#include <memory>
#include <string>

#include "core/domain/user.h"
#include "database/database_adapter.h"

SQLiteUserRepository::SQLiteUserRepository(
    std::shared_ptr<DatabaseAdapter> db_adapter)
    : db_adapter_(db_adapter) {}

bool SQLiteUserRepository::Exists(const std::string& name) const {
  return FindByName(name) != users_.end();
}

const User& SQLiteUserRepository::GetUser(const std::string& name) const {
  auto it = FindByName(name);
  return *it;
}

bool SQLiteUserRepository::AddUser(User& user) {
  auto stmt = db_adapter_->Prepare(kInsertUserSql);
  stmt->Bind(":name", user.name());
  bool success = stmt->Execute();
  if (success == false) {
    return false;
  }
  user.set_id(db_adapter_->GetLastInsertId());
  users_.insert(user);
  return success;
}

bool SQLiteUserRepository::DelUser(const User& user) {
  return true;
}

bool SQLiteUserRepository::UpdateUser(const User& user) {
  return true;
}

bool SQLiteUserRepository::ValidateCredentials() {
  return false;
}

bool SQLiteUserRepository::LoadAllUsers() {
  auto stmt = db_adapter_->Prepare(kLoadAllUsers);
  auto result_set = stmt->ExecuteQuery();
  while (result_set->NextRow()) {
    User user(result_set->GetInt("id"), result_set->GetString("name"));
    users_.insert(user);
  }
  return true;
}