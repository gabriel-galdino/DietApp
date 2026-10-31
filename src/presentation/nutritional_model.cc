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
    double factor = food.quantity / 100.0;
    switch (col) {
      case 0:
        variant = wxString::FromUTF8(food.display_name);
        break;
      case 1:
        variant = wxString::Format("%.2lf", food.proteins_per_100g * factor);
        break;
      case 2:
        variant = wxString::Format("%.2lf", food.carbs_per_100g * factor);
        break;
      case 3:
        variant = wxString::Format("%.2lf", food.fats_per_100g * factor);
        break;
      case 4:
        variant = wxString::Format("%.2lf", CalculateAccumulatedKcal(row));
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
                      : wxString::Format("%.2lf", CalculateTotalProteins());
        break;
      case 2:
        variant = selected_foods_.empty()
                      ? wxString::Format("%.2lf", 0.00)
                      : wxString::Format("%.2lf", CalculateTotalCarbs());
        break;
      case 3:
        variant = selected_foods_.empty()
                      ? wxString::Format("%.2lf", 0.00)
                      : wxString::Format("%.2lf", CalculateTotalFats());
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

void NutritionalModel::ClearFoods() {
  selected_foods_.clear();
}

double NutritionalModel::CalculateAccumulatedKcal(unsigned int row) const {
  double accumulated_kcal = 0;
  const double protein_kcal = 4;
  const double carb_kcal = 4;
  const double fat_kcal = 9;
  for (int i = 0; i <= row; i++) {
    const auto& food = selected_foods_[i];
    double factor = food.quantity / 100.0;
    double partial_proteins_kcal = food.proteins_per_100g * protein_kcal;
    double partial_carbs_kcal = food.carbs_per_100g * carb_kcal;
    double partial_fats_kcal = food.fats_per_100g * fat_kcal;
    accumulated_kcal += (partial_proteins_kcal +  //
                         partial_carbs_kcal +     //
                         partial_fats_kcal) *
                        factor;
  }
  return accumulated_kcal;
}

double NutritionalModel::CalculateTotalKcal() const {

  double total_kcal = 0;
  if (selected_foods_.empty() == false) {
    total_kcal = CalculateAccumulatedKcal(selected_foods_.size() - 1);
  }
  return total_kcal;
}

double NutritionalModel::CalculateTotalProteins() const {
  double total_proteins_g = 0.0;
  for (const auto& food : selected_foods_) {
    double factor = food.quantity / 100.0;
    total_proteins_g += food.proteins_per_100g * factor;
  }
  return total_proteins_g;
}

double NutritionalModel::CalculateTotalCarbs() const {
  double total_carbs_g = 0.0;
  for (const auto& food : selected_foods_) {
    double factor = food.quantity / 100.0;
    total_carbs_g += food.carbs_per_100g * factor;
  }
  return total_carbs_g;
}

double NutritionalModel::CalculateTotalFats() const {
  double total_fats_g = 0.0;
  for (const auto& food : selected_foods_) {
    double factor = food.quantity / 100.0;
    total_fats_g += food.fats_per_100g * factor;
  }
  return total_fats_g;
}

unsigned int NutritionalModel::GetFoodsCount() const {
  return selected_foods_.size();
}

double NutritionalModel::GetFoodQuantity(unsigned int row) const {
  if (row < selected_foods_.size()) {
    return selected_foods_[row].quantity;
  }
  return 0.0;
}

void NutritionalModel::UpdateFoodQuantity(unsigned int row, double quantity) {
  if (row < selected_foods_.size()) {
    selected_foods_[row].quantity = quantity;
  }
}
