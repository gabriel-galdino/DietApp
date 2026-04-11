#include "presentation/nutritional_model.h"

#include <algorithm>
#include <cstdint>
#include <vector>

#include <wx/dataview.h>
#include <wx/variant.h>

#include "core/dto/food_dto.h"

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
      type = wxString::FromUTF8("string");
      break;
    case 2:
      type = wxString::FromUTF8("string");
      break;
    case 3:
      type = wxString::FromUTF8("string");
      break;
    case 4:
      type = wxString::FromUTF8("string");
      break;
    default:
      void();
  }
  return type;
}

void NutritionalModel::GetValue(wxVariant& variant, const wxDataViewItem& item,
                                unsigned int col) const {
  unsigned int row = reinterpret_cast<uintptr_t>(item.GetID()) - 1;
  if (row < selected_foods_.size()) {
    const auto& food = selected_foods_[row];
    switch (col) {
      case 0:
        variant = wxString::FromUTF8(food.display_name);
        break;
      case 1:
        variant = wxString::Format("%.2lf", food.proteins_per_100g);
        break;
      case 2:
        variant = wxString::Format("%.2lf", food.carbs_per_100g);
        break;
      case 3:
        variant = wxString::Format("%.2lf", food.fats_per_100g);
        break;
      case 4:
        variant = wxString::Format("%.2lf", CalculatePartialKcal(row));
        break;
    }
  } else {
    switch (col) {
      case 0:
        variant = selected_foods_.empty() ? wxString("Sem alimentos")
                                          : wxString("Total");
        break;
      case 1:
        variant = selected_foods_.empty()
                      ? wxString::Format("%.2lf", 0.00)
                      : wxString::Format("%.2lf", CalculateTotalProteinsKcal());
        break;
      case 2:
        variant = selected_foods_.empty()
                      ? wxString::Format("%.2lf", 0.00)
                      : wxString::Format("%.2lf", CalculateTotalCarbsKcal());
        break;
      case 3:
        variant = selected_foods_.empty()
                      ? wxString::Format("%.2lf", 0.00)
                      : wxString::Format("%.2lf", CalculateTotalFatsKcal());
        break;
      case 4:
        variant = wxString::Format("%.2lf", CalculateTotalKcal());
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
    for (unsigned int i = 1; i <= selected_foods_.size() + 1; ++i) {
      children.Add(wxDataViewItem(reinterpret_cast<void*>(i)));
    }
    return children.GetCount();
  }
  return 0;
}

FoodDTO NutritionalModel::GetFoodByRow(unsigned int row) {
  return selected_foods_[row];
}

void NutritionalModel::AddFoodToSelection(const FoodDTO& food) {
  selected_foods_.push_back(food);
}

void NutritionalModel::DeleteFoodFromSelection(const FoodDTO& food) {
  auto new_logical_end =
      std::remove_if(selected_foods_.begin(), selected_foods_.end(),
                     [&](const FoodDTO& other) { return food.id == other.id; });
  selected_foods_.erase(new_logical_end, selected_foods_.end());
}

void NutritionalModel::UpdateSelectedFoods() {
  Cleared();
}

double NutritionalModel::CalculatePartialKcal(unsigned int row) const {
  double partial_kcal = 0;
  for (int i = 0; i <= row; i++) {
    const auto& food = selected_foods_[i];
    partial_kcal +=
        (food.proteins_per_100g + food.carbs_per_100g + food.fats_per_100g);
  }
  return partial_kcal;
}

double NutritionalModel::CalculateTotalKcal() const {
  double total_kcal = 0;
  for (const auto& food : selected_foods_) {
    total_kcal +=
        (food.proteins_per_100g + food.carbs_per_100g + food.fats_per_100g);
  }
  return total_kcal;
}

double NutritionalModel::CalculateTotalProteinsKcal() const {
  double total_proteins_kcal = 00;
  for (const auto& food : selected_foods_) {
    total_proteins_kcal += food.proteins_per_100g;
  }
  return total_proteins_kcal;
}

double NutritionalModel::CalculateTotalCarbsKcal() const {
  double total_carbs_kcal = 00;
  for (const auto& food : selected_foods_) {
    total_carbs_kcal += food.carbs_per_100g;
  }
  return total_carbs_kcal;
}

double NutritionalModel::CalculateTotalFatsKcal() const {
  double total_fats_kcal = 00;
  for (const auto& food : selected_foods_) {
    total_fats_kcal += food.fats_per_100g;
  }
  return total_fats_kcal;
}
