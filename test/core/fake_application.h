#ifndef DIETAPP_INCLUDE_TEST_APPLICATION_H
#define DIETAPP_INCLUDE_TEST_APPLICATION_H

#include <string>

#include "core/i_application.h"

class FakeApplication : public IApplication {
 public:
  FakeApplication() = default;

  ~FakeApplication() = default;

  bool AddMealToUser(const std::string& name, const std::string& meal) override;
};

#endif  // DIETAPP_INCLUDE_TEST_APPLICATION_H
