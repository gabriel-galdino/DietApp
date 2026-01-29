#ifndef DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_USER_REPOSITORY_H
#define DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_USER_REPOSITORY_H

#include <string>

#include "core/domain/user.h"

class UserRepository {
 public:
  virtual ~UserRepository() = default;

  virtual bool Exists(const std::string& name) const = 0;
  virtual const User& GetUser(const std::string& name) const = 0;
  virtual bool AddUser(User& user) = 0;
  virtual bool DelUser(const User& user) = 0;
  virtual bool UpdateUser(const User& user) = 0;
  virtual bool LoadAllUsers() = 0;

  virtual bool ValidateCredentials() = 0;
};

#endif  // DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_USER_REPOSITORY_H
