#include "StdAfx.h"
#include "uiOperation.h"
using namespace uiOperation;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Xml;
using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Xml::XPath;
using namespace std;

/// ���������� tboxes
int uiSave::uiTbSave(array<Control^>^ tb, array<String^>^ Keys, int num, String^ xmlfname, int delta, bool Save) {
  int itog = 0;
  XmlDocument^ xmlDoc;  // ���������
  xmlDoc = gcnew XmlDocument();  // ��� �������������
  try {
    xmlDoc->Load(xmlfname);
    XmlNodeList^ xmlNodes;
    XmlNode^ xmlElement;
	  String^ elementValue;  // �������� ��������
	  for (int j = 0+delta; j < num+delta; j++) {  // �� TextBox'��
      xmlNodes = xmlDoc->SelectNodes(Keys[j-delta]);
      // ���������� ����������� ������� ��������� ���������,
      for (int i = 0; i < xmlNodes->Count; i++) {
        xmlElement = xmlNodes[i];
        if (xmlElement->HasChildNodes) {
          // ������
          elementValue = xmlElement->FirstChild->Value->Trim();
          if(tb[j] != nullptr) {
		        if (!Save) tb[j]->Text = elementValue;
            else xmlElement->InnerText = tb[j]->Text;
          }
        }  // if (xmlEle..
	    }  // for (int i
    }  // for (int j =..
    if (Save) xmlDoc->Save(xmlfname);
  }  // try {..
  catch (XPathException ^ex0) {
    //listBoxMsg->Items->Add("������� �� ������.");
    itog = 3;
  }
  catch (XmlException ^ex1) {
    //listBoxMsg->Items->Add("���������� ��� ������ � xml.");
    itog = 2;
  }
  catch (FileNotFoundException ^ex2) {
    //listBoxMsg->Items->Add("������: ���� prd_ukv.cfg.xml �� ������ � ������ � �����������");
    itog = 1;  // ����� ��� ������
	  //createXmlCfg();  // ������� ����
  }
  ///
  return itog;
}
// �������� �������������� �����
int uiSave::XmlTemplCreate(System::String^ xmlName, array<String^>^ Layers, int num) { 
//
  XmlTextWriter^ tW = gcnew XmlTextWriter(xmlName, nullptr);
  tW->WriteStartDocument();
  /// ��� ������

  tW->WriteStartElement(Layers[0]);
    for(int i = 1; i < num; i++){
      tW->WriteStartElement(Layers[i]);
      tW->WriteString("?");
      tW->WriteEndElement();  // ��������� ���������
    }
  tW->WriteEndElement();  // ��������� ���������
  
  /// ��������� ��� ���������
  tW->WriteEndDocument();
  tW->Close();  

// 
    return 0; 
}
// tester
uiSaveTest::uiSaveTest() {
  uis_ = gcnew uiSave();
  ///
}
int uiSaveTest::testCreate() {
  int num = 1+3;
  array<String^>^ Layers = gcnew array<String^>(4);
  Layers[0] = "Title";
  for(int i = 1; i < num; i++)
    Layers[i] = "L"+i.ToString();
  // call
  uis_->XmlTemplCreate("test.xml", Layers, num);
  //
  return 0;
}
int uiSaveTest::testSave(bool Save, array<Control^>^ tb) {
    array<String^>^ pathSet = gcnew array<String^>(3);
    String^ Sum = gcnew String("");
    for(int i = 1; i < 4; i++)
        pathSet[i-1] = "/Title/L" + i.ToString();
    // ��������
    uis_->uiTbSave(tb, pathSet, 3, "test.xml", 0, true);
    // 
    return 0;
}

/////
void uiDraw::uiDrawControl(
  array<Control^>^ cont,  /* ���� �������� */
  int num, /* ����� ����������� */
  vector<vector<int>> &xy_pos,  /* ������� */
  vector<vector<int>> &xy_size,  /* ������� */
  string &name,  /* ��� ���������� ��� ������ */
  int beginIndex,  /* ��������� ����� ���������� */
  bool picture  /* ���� �������, �� ���������� � ��������� */
) {
  String^ sname = gcnew String(name.c_str());
  for(std::size_t i = 0; i < xy_pos.size(); i++) {
    if(picture) 
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(cont[i]))->BeginInit();
    cont[i]->Location = System::Drawing::Point(xy_pos[i][0], xy_pos[i][1]);
    cont[i]->Size = System::Drawing::Size(xy_size[i][0], xy_size[i][1]);
    cont[i]->Name = sname+(i+beginIndex).ToString();
    cont[i]->Text = "?";
    cont[i]->Visible = true;
    if(picture) 
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(cont[i]))->EndInit();
  }   
}
