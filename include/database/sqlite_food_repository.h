#ifndef DIETAPP_INCLUDE_DATABASE_SQLITE_FOOD_REPOSITORY_H
#define DIETAPP_INCLUDE_DATABASE_SQLITE_FOOD_REPOSITORY_H

#include <memory>
#include <string>

#include "core/domain/food.h"
#include "core/ports/repository/food_repository.h"
#include "database_adapter.h"

constexpr const char* kInsertFoodSql = R"sql(
  INSERT INTO foods(name,
                    display_name,
                    proteins_per_100g,
                    carbs_per_100g,
                    fats_per_100g)
  VALUES (:name,
          :display_name,
          :proteins_per_100g,
          :carbs_per_100g,
          :fats_per_100g)
)sql";

constexpr const char* kSelectFoodSql = R"sql(
  SELECT id, name, proteins_per_100g, carbs_per_100g, fats_per_100g
  FROM foods
  WHERE display_name = :display_name
)sql";

constexpr const char* kCheckTableSql = R"sql(
  SELECT 1
  FROM foods
  LIMIT 1;
)sql";

class SQLiteFoodRepository : public FoodRepository {
 public:
  SQLiteFoodRepository(std::shared_ptr<DatabaseAdapter> db_adapter);

  const Food GetFood(const std::string& name) const override;

  bool AddFood(Food& food) override;

  bool DelFood(const Food& food) override;

  bool UpdateFood(const Food& food) override;

  bool LoadAllFoods() override;

  bool FillFoodsTable(const std::vector<Food>& foods) override;

 private:
  std::shared_ptr<DatabaseAdapter> db_adapter_;
};

#endif  // DIETAPP_INCLUDE_DATABASE_SQLITE_FOOD_REPOSITORY_H
