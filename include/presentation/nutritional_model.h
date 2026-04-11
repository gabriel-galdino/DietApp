#ifndef DIETAPP_INCLUDE_PRESENTATION_NUTRITIONAL_MODEL_H
#define DIETAPP_INCLUDE_PRESENTATION_NUTRITIONAL_MODEL_H

#include <vector>

#include <wx/dataview.h>
#include <wx/variant.h>

#include "core/dto/food_dto.h"

class NutritionalModel : public wxDataViewModel {
 public:
  NutritionalModel() = default;

  unsigned int GetColumnCount() const override;

  bool HasContainerColumns(const wxDataViewItem& item) const override {
    return false;
  }

  wxString GetColumnType(unsigned int col) const override;

  void GetValue(wxVariant& variant, const wxDataViewItem& item,
                unsigned int col) const override;

  bool SetValue(const wxVariant& variant, const wxDataViewItem& item,
                unsigned int col) override;

  wxDataViewItem GetParent(const wxDataViewItem& item) const override;

  bool IsContainer(const wxDataViewItem& item) const override;

  unsigned int GetChildren(const wxDataViewItem& parent,
                           wxDataViewItemArray& children) const override;

  FoodDTO GetFoodByRow(unsigned int row);

  void AddFoodToSelection(const FoodDTO& food);

  void DeleteFoodFromSelection(const FoodDTO& food);

  void UpdateSelectedFoods();

 private:
  double CalculatePartialKcal(unsigned int row) const;

  double CalculateTotalKcal() const;

  double CalculateTotalProteinsKcal() const;

  double CalculateTotalCarbsKcal() const;

  double CalculateTotalFatsKcal() const;

  std::vector<FoodDTO> selected_foods_;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_NUTRITIONAL_MODEL_H
