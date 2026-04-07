#include <wx/filename.h>

#include <OpenXLSX.hpp>

#include "core/domain/food.h"
#include "core/dto/food_dto.h"

class XlsxService {
 public:
  bool Initialize(wxFileName& workbook);

  bool Shutdown();

  bool LoadFoodDataFromSheet(const std::string worksheet,
                             std::vector<Food>& foods,
                             std::vector<FoodDTO>& data);

 private:
  double getDoubleSafe(int row, int col,
                       const OpenXLSX::XLWorksheet& worksheet);

  std::string GenerateDisplayName(const std::string& food_name);

  OpenXLSX::XLDocument doc_;
};