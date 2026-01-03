#ifndef DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_USER_REPOSITORY_H
#define DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_USER_REPOSITORY_H

class UserRepository {
 public:
  virtual ~UserRepository() = default;

  virtual void GetUser() = 0;

  virtual bool ValidateCredentials() = 0;
};

#endif  // DIETAPP_INCLUDE_CORE_PORTS_REPOSITORY_USER_REPOSITORY_H
