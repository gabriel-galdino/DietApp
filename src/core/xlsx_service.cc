#include "core/xlsx_service.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include <OpenXLSX.hpp>

#include "core/domain/food.h"
#include "core/dto/food_dto.h"

namespace {
/* Merge given strings removing leading and trailing spaces */
std::string MergeStrings(std::vector<std::string>& parts) {
  auto trim = [](std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),          //
                                    [](unsigned char ch) {       //
                                      return !std::isspace(ch);  //
                                    }));

    s.erase(std::find_if(s.rbegin(), s.rend(),        //
                         [](unsigned char ch) {       //
                           return !std::isspace(ch);  //
                         })
                .base(),
            s.end());
  };

  std::string pretty_name = "";
  for (auto& part : parts) {
    trim(part);
    pretty_name += part;
    pretty_name.push_back(' ');
  }
  pretty_name.pop_back();

  return pretty_name;
}
}  // namespace

bool XlsxService::Initialize(wxFileName& workbook) {
  bool success = true;

  if (workbook.IsOk() == false) {
    wxStandardPaths& paths = wxStandardPaths::Get();
    workbook = wxFileName(paths.GetDataDir(), "");
    workbook.AppendDir("spreadsheet");
    workbook.SetFullName("Taco-4a-Edicao.xlsx");
  }

  try {
    doc_.open(workbook.GetFullPath().utf8_string());
  } catch (const OpenXLSX::XLException& e) {
    std::cerr << e.what() << std::endl;
    success = false;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    success = false;
  }

  return success;
}

bool XlsxService::Shutdown() {
  doc_.close();
  return true;
}

bool XlsxService::LoadFoodDataFromSheet(const std::string sheet_name,
                                        std::vector<Food>& foods,
                                        std::vector<FoodDTO>& data) {
  auto worksheet = doc_.workbook().worksheet(sheet_name);
  for (auto row = 3; row <= worksheet.rowCount(); ++row) {

    auto cellType = worksheet.cell(row, 1).value().type();

    if (cellType != OpenXLSX::XLValueType::Integer &&
        cellType != OpenXLSX::XLValueType::Float) {
      continue;
    }

    auto cellId = worksheet.cell(row, 1).value().get<int>();

    auto name = worksheet.cell(row, 2).value().get<std::string>();

    auto parseValue = [&](int col) {
      auto type = worksheet.cell(row, col).value().type();
      if (type == OpenXLSX::XLValueType::Float ||
          type == OpenXLSX::XLValueType::Integer) {
        return getDoubleSafe(row, col, worksheet);
      }
      return 0.0;
    };
    const std::string display_name = GenerateDisplayName(name);
    const double protein_per_100g = parseValue(6);  // Column F
    const double fat_per_100g = parseValue(7);      // Column G
    const double carb_per_100g = parseValue(9);     // Column I

    Food food;
    food.set_id(cellId);
    food.set_name(name);
    food.set_display_name(display_name);
    food.set_protein_per_100g(protein_per_100g);  // Column F
    food.set_fat_per_100g(fat_per_100g);          // Column G
    food.set_carb_per_100g(carb_per_100g);        // Column I

    FoodDTO food_data;
    food_data.id = cellId;
    food_data.name = name;
    food_data.name = display_name;
    food_data.protein_per_100g = protein_per_100g;
    food_data.fat_per_100g = fat_per_100g;
    food_data.carb_per_100g = carb_per_100g;
    data.push_back(food_data);

    if (food.protein_per_100g() > 0 || food.fat_per_100g() > 0 ||
        food.carb_per_100g() > 0) {
      foods.push_back(food);
    }
  }

  return true;
}

double XlsxService::getDoubleSafe(int row, int col,
                                  const OpenXLSX::XLWorksheet& worksheet) {
  const auto& ref = worksheet.cell(row, col).cellReference();
  auto cell = worksheet.cell(ref);

  if (cell.value().type() == OpenXLSX::XLValueType::Float) {
    return cell.value().get<double>();
  }

  if (cell.value().type() == OpenXLSX::XLValueType::Integer) {
    return static_cast<double>(cell.value().get<int>());
  }

  return 0.0;
}

std::string XlsxService::GenerateDisplayName(const std::string& food_name) {
  std::vector<std::string> parts;

  size_t start = 0;
  size_t comma_pos = food_name.find(',');

  while (comma_pos != std::string::npos) {
    parts.emplace_back(food_name.substr(start, comma_pos - start));
    start = comma_pos + 1;
    comma_pos = food_name.find(',', start);
  }

  parts.emplace_back(food_name.substr(start));

  return MergeStrings(parts);
}