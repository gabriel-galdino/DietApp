#include "presentation/nutritional_model.h"

#include <cstdint>
#include <vector>

#include <wx/dataview.h>
#include <wx/variant.h>

#include "core/dto/food_dto.h"

NutritionalModel::NutritionalModel(const std::vector<FoodDTO> foods)
    : foods_(foods) {}

unsigned int NutritionalModel::GetColumnCount() const {
  return 5;
}

wxString NutritionalModel::GetColumnType(unsigned int col) const {
  wxString type;
  switch (col) {
    case 0:
      type = wxString::FromUTF8("string");
      break;
    case 1:
      type = wxString::FromUTF8("double");
      break;
    case 2:
      type = wxString::FromUTF8("double");
      break;
    case 3:
      type = wxString::FromUTF8("double");
      break;
    case 4:
      type = wxString::FromUTF8("double");
      break;
    default:
      void();
  }
  return type;
}

void NutritionalModel::GetValue(wxVariant& variant, const wxDataViewItem& item,
                                unsigned int col) const {
  unsigned int row = reinterpret_cast<uintptr_t>(item.GetID()) - 1;
  if (row < foods_.size()) {
    const auto& food = foods_[row];
    switch (col) {
      case 0:
        variant = food.name;
        break;
      case 1:
        variant = food.protein_per_100g;
        break;
      case 2:
        variant = food.fat_per_100g;
        break;
      case 3:
        variant = food.carb_per_100g;
        break;
      case 4:
        variant = CalculatePartialKcal(row);
        break;
    }
  } else {
    switch (col) {
      case 0:
        variant =
            foods_.empty() ? wxString("Sem alimentos") : wxString("Total");
        break;
      case 1:
      case 2:
      case 3:
        variant = 0.0;
        break;
      case 4:
        variant = CalculateTotalKcal();
        break;
    }
  }
}

bool NutritionalModel::SetValue(const wxVariant& variant,
                                const wxDataViewItem& item, unsigned int col) {
  return false;
}

wxDataViewItem NutritionalModel::GetParent(const wxDataViewItem& item) const {
  return wxDataViewItem();
}

bool NutritionalModel::IsContainer(const wxDataViewItem& item) const {
  return !item.IsOk();
}

unsigned int NutritionalModel::GetChildren(
    const wxDataViewItem& parent, wxDataViewItemArray& children) const {
  if (!parent.IsOk()) {
    for (unsigned int i = 1; i <= foods_.size() + 1; ++i) {
      children.Add(wxDataViewItem(reinterpret_cast<void*>(i)));
    }
    return children.GetCount();
  }
  return 0;
}

double NutritionalModel::CalculatePartialKcal(unsigned int row) const {
  return 0;
}

double NutritionalModel::CalculateTotalKcal() const {
  return 0;
}
