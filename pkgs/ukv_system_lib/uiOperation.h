// file : #include "uiOperation.h"
#pragma once
#include "NativeCode.h"
#include <vector>
namespace uiOperation {
 ref class uiSave
  {
  public : int uiTbSave(array<System::Windows::Forms::Control^>^ tb,
               array<System::String^>^ Keys, /* ключи для поиска */
               int num,  /* число элементов для сохранения */
               System::String^ xmlfname,
               int delta, /* сдвиг относительно начала массива */
               bool Save);
  public : int XmlTemplCreate(System::String^ xmlName,
               array<System::String^>^ Layers,
               int num
               );
  };
 // uiSaveTest
 ref class uiSaveTest {
 public : uiSaveTest();
 private : uiSave^ uis_;
 // create layer
 public : int testCreate();
 public : int testSave(bool Save, array<System::Windows::Forms::Control^>^ tb);
 
 };
 // uiDrow
 ref class uiDraw {
 public : void uiDrawControl(
              array<System::Windows::Forms::Control^>^ cont,  /* сами контролы */
              int num, /* число комананетов */
              std::vector<std::vector<int>> &xy_pos,  /* позиции */
              std::vector<std::vector<int>> &xy_size,  /* размеры */
              std::string &name,  /* имя компанента без номера */
              int beginIndex,  /* начальный номер компанента */
              bool picture
              );
 
 };
};
