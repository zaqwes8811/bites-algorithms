/** 
  file : #include "hardIface.h"

  јлгоритм работы :
  1. ћодель хранит нативный указатель на базовый класс
  2. ѕри соединении он инициализируетс€ производным, которому 
    передаютс€ настройки(в конструкторе идет подготовка к соединению)
    - заполнение структур, без загрузки
    - захват событий(ошибки не учитываютс€, если нужно учесть, то 
      из конструктора этот код нужно будет перенсти в Create())
  3. Create() - непосредственно создает соединение(она возвращает ошибки,
    которые и контроллируютс€)
  4. ≈сли все успешно создано, то можно посылать и принимать данные
  5. инициируем закрытие соединени€

  ќсобенности использовани€ : 
  ¬ данной версии не допускаетс€ наложение команд. ѕока не пришел ответ
    на одну другую посылать нельз€.

*/
#pragma once
#include "NativeCode.h"
namespace NativeCode {
#pragma unmanaged

/// <Any iface>
/// </Any iface>
class IHardIfaceTop {
  public : virtual ~IHardIfaceTop() {}
	/// iface ///
  public : virtual int Create(IfaceSettings* pifs) = 0;  // создание соединени€, захват ресурсов
  public : virtual int Rx(simpleCmd* scRx) = 0;  // читать
  public : virtual int Tx(simpleCmd* scTx) = 0;  // писать
  public : virtual void Remove(void) = 0;  // закрытие соединени€, отпускает нек. ресурсы
};


/// <RS-232>
/// </RS-232>
// ! перекрывающиес€ структура нужно объ€вить вне модул€ иначе работать не будет
class hardIfaceRS232 : public IHardIfaceTop {
  
  // API date
  private : OVERLAPPED* polwr_;  // €вно нужно передать, так не работают
  private : OVERLAPPED* polrd_;
  private : CRITICAL_SECTION csTx_;  // 
  private : CRITICAL_SECTION csRx_;  
  private : HANDLE hPort_;  // хэндл порта

  /// Filds
  private : int maxReadOnceRs_;  // сколько максимум читаме за раз

  /// <Methods>
  public : hardIfaceRS232(OVERLAPPED* polwr, OVERLAPPED* polrd);
  public : ~hardIfaceRS232();
  public : int Create(IfaceSettings* pifs);  // создание соединени€
  public : int Rx(simpleCmd* scRx);  // запуск в потоке чтени€
  public : int Tx(simpleCmd* scTx);  // отправить бланк
  public : void Remove(void);
  
  // ThreadMT
  private : bool bTxMT_; 
  private : bool bRxMT_;  
  private : void setTxState(bool TxFlag);
  private : void setRxState(bool RxFlag);

  
};

/// <UDP>
/// </UDP>
class hardIfaceUdp : public IHardIfaceTop {
    
  /// API Data
  protected : static int WINSOCK_VERSION;
  private : WSADATA wsaData_;
  private : int iResult_;  // результат открыти€ библиотеки
  private : CRITICAL_SECTION csTx_;  // критическа€ секци€ дл€ udp
  private : CRITICAL_SECTION csRx_;
  private : int iSockRx_;
  private : struct sockaddr_in addrRx_;
  private : int iSockTx_;
  private : struct sockaddr_in addrTx_;
  
  // data
  private : int maxReadOnceUdp_;  // размер буффера, которым черпаем
  
  // ThreadMT
  private : bool bTxMT_ ;  // разрешение работы потока отправки
  private : bool bRxMT_ ;  // потока чтени€
  private : void setTxState(bool TxFlag);  // функции установки
  private : void setRxState(bool RxFlag);
  
	
  /// <Methods>
  public : ~hardIfaceUdp();
  public : hardIfaceUdp(OVERLAPPED* polwr, OVERLAPPED* polrd);
  public : int Create(IfaceSettings* pifs);  // создание соединени€, захват ресурсов
  public : int Rx(simpleCmd* scRx);  // заполнить бланк
  public : int Tx(simpleCmd* scTx);  // отправить бланк
  public : void Remove(void);  // закрытие соединени€, отпускает нек. ресурсы 
};

#pragma managed
}

