#ifndef DIETAPP_INCLUDE_DATABASE_USER_REPOSITORY_H
#define DIETAPP_INCLUDE_DATABASE_USER_REPOSITORY_H

class UserRepository {
 public:
  virtual ~UserRepository() = default;

  virtual void GetUser();

  virtual bool ValidateCredentials();
};

#endif  // DIETAPP_INCLUDE_DATABASE_USER_REPOSITORY_H
