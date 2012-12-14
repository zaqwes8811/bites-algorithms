#pragma once
#include "NativeCode.h"
#include "hardIface.h"
namespace IfaceEnvelop {
/// <Model>
  /// </Model>
  public ref class Model
  {
  private : OVERLAPPED* polwr_;  // ���� ����� ��������, ��� �� ��������
  private : OVERLAPPED* polrd_;
  private : HANDLE evReadEnd_;
  private : HANDLE evForWrite_;
  private : NativeCode::IHardIfaceTop* hift_;  // ��������� �� ������� �����


  // ! ����� ������� ! � ��� ����� ������, � ������ ������� ������ ���� ����
  public : NativeCode::simpleCmd* scTx_;  // ������ ��� ��������/������
  public : NativeCode::simpleCmd* scRx_;
  //


  private : char TypeStream_;  // ��������� ���� ������

  /// <Methods>
  public : ~Model(void);
  public : !Model(void);
  private : bool Main_;  // ���������� ��� ������ ������
  public : Model(OVERLAPPED* polwr, OVERLAPPED* polrd, int numWrCmd, int maxWrLen, int numRdCmd, int maxRdLen, bool Main);
  public : int Connect(NativeCode::IfaceSettings* pifs);  // ��������� �� �����, � ��� ���������);
  public : char getTypeStream() {return TypeStream_;}
  public : int DisConnect(void);  // 
  public : int Send(void);  // ������ ����������� ��������
           // ����� ��������� - ����� ������ ���� ���������� !
  public : void ReadComplete(void);
          
  /// Threads
  private : bool ConnectActiveMT_;
  private : System::Threading::Thread^ thr_;  // ����� ������
  private : System::Threading::Thread^ thwr_;  // ����� ������
  private : void fthRead_(void); 
  private : void fthWrite_(void); 
  public : void Run();
  public : bool getActive(void);

  /// Events/OnEvents
  public : event System::EventHandler<System::EventArgs^>^ evViewUbdate;  // ��� ������ � ���� ����� 
  };
};