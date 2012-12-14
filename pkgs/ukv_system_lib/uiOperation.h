// file : #include "uiOperation.h"
#pragma once
#include "NativeCode.h"
#include <vector>
namespace uiOperation {
 ref class uiSave
  {
  public : int uiTbSave(array<System::Windows::Forms::Control^>^ tb,
               array<System::String^>^ Keys, /* ����� ��� ������ */
               int num,  /* ����� ��������� ��� ���������� */
               System::String^ xmlfname,
               int delta, /* ����� ������������ ������ ������� */
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
              array<System::Windows::Forms::Control^>^ cont,  /* ���� �������� */
              int num, /* ����� ����������� */
              std::vector<std::vector<int>> &xy_pos,  /* ������� */
              std::vector<std::vector<int>> &xy_size,  /* ������� */
              std::string &name,  /* ��� ���������� ��� ������ */
              int beginIndex,  /* ��������� ����� ���������� */
              bool picture
              );
 
 };
};
