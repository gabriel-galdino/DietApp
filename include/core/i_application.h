#ifndef DIETAPP_INCLUDE_CORE_I_APPLICATION_H
#define DIETAPP_INCLUDE_CORE_I_APPLICATION_H

#include <string>

class IApplication {
 public:
  virtual ~IApplication() = default;
  virtual bool AddMealToUser(const std::string& name,
                             const std::string& meal) = 0;
};

#endif  // DIETAPP_INCLUDE_CORE_I_APPLICATION_H
