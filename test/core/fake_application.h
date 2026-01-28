#ifndef DIETAPP_INCLUDE_TEST_APPLICATION_H
#define DIETAPP_INCLUDE_TEST_APPLICATION_H

#include <string>

#include "core/i_application.h"

class FakeApplication : public IApplication {
 public:
  FakeApplication() = default;

  ~FakeApplication() = default;

  bool IsTestsMode() const override { return true; }

  bool AddMealToUser(const std::string& name, const std::string& meal) override;

  void WillReturn(bool success) { success_ = success; }

 private:
  bool success_ = true;
};

#endif  // DIETAPP_INCLUDE_TEST_APPLICATION_H
