#ifndef DIETAPP_INCLUDE_DATABASE_SQLITE_FOOD_REPOSITORY_H
#define DIETAPP_INCLUDE_DATABASE_SQLITE_FOOD_REPOSITORY_H

#include <algorithm>
#include <memory>
#include <set>
#include <string>

#include "core/domain/food.h"
#include "core/ports/repository/food_repository.h"
#include "database_adapter.h"

class SQLiteFoodRepository : public FoodRepository {
 public:
  SQLiteFoodRepository(std::shared_ptr<DatabaseAdapter> db_adapter);

  const Food& GetFood(const std::string& name) const override;

  bool AddFood(Food& food) override;

  bool DelFood(const Food& food) override;

  bool UpdateFood(const Food& food) override;

  bool LoadAllFoods() override;

  bool FillFoodsTable(const std::vector<Food>& foods) override;

 private:
  auto FindByName(const std::string& name) const {
    return std::find_if(foods_.begin(), foods_.end(),
                        [&name](const Food& food) {    //
                          return food.name() == name;  //
                        });
  }

  std::set<Food> foods_;
  std::shared_ptr<DatabaseAdapter> db_adapter_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_FOOD_REPOSITORY_H
