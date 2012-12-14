/** 
  file : #include "hardIface.h"

  �������� ������ :
  1. ������ ������ �������� ��������� �� ������� �����
  2. ��� ���������� �� ���������������� �����������, �������� 
    ���������� ���������(� ������������ ���� ���������� � ����������)
    - ���������� ��������, ��� ��������
    - ������ �������(������ �� �����������, ���� ����� ������, �� 
      �� ������������ ���� ��� ����� ����� �������� � Create())
  3. Create() - ��������������� ������� ����������(��� ���������� ������,
    ������� � ���������������)
  4. ���� ��� ������� �������, �� ����� �������� � ��������� ������
  5. ���������� �������� ����������

  ����������� ������������� : 
  � ������ ������ �� ����������� ��������� ������. ���� �� ������ �����
    �� ���� ������ �������� ������.

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
  public : virtual int Create(IfaceSettings* pifs) = 0;  // �������� ����������, ������ ��������
  public : virtual int Rx(simpleCmd* scRx) = 0;  // ������
  public : virtual int Tx(simpleCmd* scTx) = 0;  // ������
  public : virtual void Remove(void) = 0;  // �������� ����������, ��������� ���. �������
};


/// <RS-232>
/// </RS-232>
// ! ��������������� ��������� ����� �������� ��� ������ ����� �������� �� �����
class hardIfaceRS232 : public IHardIfaceTop {
  
  // API date
  private : OVERLAPPED* polwr_;  // ���� ����� ��������, ��� �� ��������
  private : OVERLAPPED* polrd_;
  private : CRITICAL_SECTION csTx_;  // 
  private : CRITICAL_SECTION csRx_;  
  private : HANDLE hPort_;  // ����� �����

  /// Filds
  private : int maxReadOnceRs_;  // ������� �������� ������ �� ���

  /// <Methods>
  public : hardIfaceRS232(OVERLAPPED* polwr, OVERLAPPED* polrd);
  public : ~hardIfaceRS232();
  public : int Create(IfaceSettings* pifs);  // �������� ����������
  public : int Rx(simpleCmd* scRx);  // ������ � ������ ������
  public : int Tx(simpleCmd* scTx);  // ��������� �����
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
  private : int iResult_;  // ��������� �������� ����������
  private : CRITICAL_SECTION csTx_;  // ����������� ������ ��� udp
  private : CRITICAL_SECTION csRx_;
  private : int iSockRx_;
  private : struct sockaddr_in addrRx_;
  private : int iSockTx_;
  private : struct sockaddr_in addrTx_;
  
  // data
  private : int maxReadOnceUdp_;  // ������ �������, ������� �������
  
  // ThreadMT
  private : bool bTxMT_ ;  // ���������� ������ ������ ��������
  private : bool bRxMT_ ;  // ������ ������
  private : void setTxState(bool TxFlag);  // ������� ���������
  private : void setRxState(bool RxFlag);
  
	
  /// <Methods>
  public : ~hardIfaceUdp();
  public : hardIfaceUdp(OVERLAPPED* polwr, OVERLAPPED* polrd);
  public : int Create(IfaceSettings* pifs);  // �������� ����������, ������ ��������
  public : int Rx(simpleCmd* scRx);  // ��������� �����
  public : int Tx(simpleCmd* scTx);  // ��������� �����
  public : void Remove(void);  // �������� ����������, ��������� ���. ������� 
};

#pragma managed
}

