#ifndef DIETAPP_INCLUDE_CORE_DOMAIN_USER_H
#define DIETAPP_INCLUDE_CORE_DOMAIN_USER_H

class User {
 public:
  virtual ~User() = default;

 private:
  int id_;
};

#endif  // DIETAPP_INCLUDE_CORE_DOMAIN_USER_H
