#pragma once
#include "NativeCode.h"
#include "hardIface.h"
namespace IfaceEnvelop {
/// <Model>
  /// </Model>
  public ref class Model
  {
  private : OVERLAPPED* polwr_;  // явно нужно передать, так не работают
  private : OVERLAPPED* polrd_;
  private : HANDLE evReadEnd_;
  private : HANDLE evForWrite_;
  private : NativeCode::IHardIfaceTop* hift_;  // указатель на базовый класс


  // ! пусть открыты ! к ним нужен доступ, а методы доступа писать пока лень
  public : NativeCode::simpleCmd* scTx_;  // бланки для отправки/приема
  public : NativeCode::simpleCmd* scRx_;
  //


  private : char TypeStream_;  // индикация типа потока

  /// <Methods>
  public : ~Model(void);
  public : !Model(void);
  private : bool Main_;  // специально для потока чтения
  public : Model(OVERLAPPED* polwr, OVERLAPPED* polrd, int numWrCmd, int maxWrLen, int numRdCmd, int maxRdLen, bool Main);
  public : int Connect(NativeCode::IfaceSettings* pifs);  // указатель на настр, и две структуры);
  public : char getTypeStream() {return TypeStream_;}
  public : int DisConnect(void);  // 
  public : int Send(void);  // просто записывавем комманду
           // место непростое - адрес должен быть актуальным !
  public : void ReadComplete(void);
          
  /// Threads
  private : bool ConnectActiveMT_;
  private : System::Threading::Thread^ thr_;  // поток чтения
  private : System::Threading::Thread^ thwr_;  // поток записи
  private : void fthRead_(void); 
  private : void fthWrite_(void); 
  public : void Run();
  public : bool getActive(void);

  /// Events/OnEvents
  public : event System::EventHandler<System::EventArgs^>^ evViewUbdate;  // вот кстати к нему может 
  };
};