#ifndef DIETAPP_INCLUDE_PRESENTATION_NUTRITIONAL_MODEL_H
#define DIETAPP_INCLUDE_PRESENTATION_NUTRITIONAL_MODEL_H

#include <vector>

#include <wx/dataview.h>
#include <wx/variant.h>

#include "core/dto/food_dto.h"

class NutritionalModel : public wxDataViewModel {
 public:
  NutritionalModel(const std::vector<FoodDTO> foods);

  unsigned int GetColumnCount() const override;

  bool HasContainerColumns(const wxDataViewItem& item) const override {
    printf("item: %p\n", item.GetID());
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

 private:
  double CalculatePartialKcal(unsigned int row) const;

  double CalculateTotalKcal() const;

  const std::vector<FoodDTO> foods_;
};

#endif  // DIETAPP_INCLUDE_PRESENTATION_NUTRITIONAL_MODEL_H
