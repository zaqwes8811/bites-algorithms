/**
file : #include "UdpExchangeClasses.h"
	using namespace transport_layers;
*/
#ifndef UDP_EXCHANGE_CLASSES
#define UDP_EXCHANGE_CLASSES
#include <vector>
#include <string>
#include <windows.h>
#include <winsock.h>
#include "TransportStructurs.h"
#pragma comment(lib, "wsock32.lib")  // for Ethernet
extern OVERLAPPED olwr;  // ����� ������������ ��� �������� ������ 
extern OVERLAPPED olrd;  // ����� ������������ ��� �������� ������ 
namespace transport_layers
{
	/// <��������� ���������>
	/// �� ���� ���������. ��������� � ��������� ��� ����������� ����������
	/// ������� �� �� smart-�����������
	/// </��������� ���������>
	class IfaceSettings {
		  private : char cTypeConnect_;
		  private : int* pType_;  // �������� ���� �� �����, � ���� � �������
		  private : char** ppType_;  // ������ � ��������� �����
		  private : int iNumStr_;
		  
		  /// <Methods>
		  public : IfaceSettings();  // ����������� �� ��������� - �� ������ ������ 
			// ..����� ���� �����. ���. ������� ����� �� ��������
		  public : IfaceSettings(int iNumInt, int iNumStr, char TypeConnect);
		  public : ~IfaceSettings();
		  
		  /// ���������
		  public : char getTypeConnect(void);
		public : void setTypeConnect(char Type);
		public : void TypeSet(int iPoint, int iValue);
		public : int iTypeGet(int iPoint);
		public : void TypeSet( int iPos, std::string sValue );
		public : char* cTypeGet(int iPos);
	};

	class simpleCmd;
	// any ifaces
	class IHardIfaceTop {
		  public : virtual ~IHardIfaceTop() {}
		  /// iface ///
		  public : virtual int Create(IfaceSettings* pifs) = 0;  // �������� ����������, ������ ��������
		  public : virtual int Rx(simpleCmd* scRx) = 0;  // ������
		  public : virtual int Tx(const FormatMsg& scTx) = 0;  // ������
		  public : virtual void Remove(void) = 0;  // �������� ����������, ��������� ���. �������
	};

	// just udp
	const WORD WINSOCK_VER = 0x0101;  // init static
	
	class hardIfaceUdp : public IHardIfaceTop {
		  /// API Data
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
		  public : hardIfaceUdp();
		  public : int Create(IfaceSettings* pifs);  // �������� ����������, ������ ��������
		  public : int Rx(simpleCmd* scRx);  // ��������� �����
		  public : int Tx(const FormatMsg& scTx);  // ��������� �����
		  public : void Remove(void);  // �������� ����������, ��������� ���. ������� 
	};

	/// <���������� �������>
	/// ���� �������� ���������� ������, ��� ������ ������ �� ���������� ���������
	/// RS, TCP. � Udp ��� ������, ��� �� ��� � � ��������� �� ������ ���������
	/// ��� �� ���������� ������� ����������� ����� ������. ! ������ ���� ������� � ������ ������� �������
	/// </���������� �������>
	class simpleCmd {
		private : int NumCmd_;  // ����� ������ � ����� - ������
		private : int NumCmdDel_;
		private : int* pNumCharInCmd_;  // ����� ���� � ��������
		private : char* pCRC_;  // ������ � ������������ ������� �� ����
		private : char** ppCmds_;  // 2D ������ � ���������
		  
		/// <Methods>
		public : simpleCmd(int NumCmd, int maxLen);
		public : ~simpleCmd();
		  
		// ���������
		// ���������� �����
		public : void calkCRC(int Pos);
		public : char getCRC(int Pos);
		// ��������
		public : void setNumCmd(int Value);
		public : int getNumCmd(void);  // �������� ����� ������
		// ���������� � ������
		public : void setCmd(int Pos, char* inBuf); // �����(� ����); ��. ������; ��� �����;
		public : char* getCmdPtr(int Pos);  // �����. ����. �� ������ � ��������
		// ����� �������
		public : int getCmdLen(int Pos);  // 
		public : void setCmdLen(int Pos, int Value);  // 
		/// ��������� 
		// �� �������� 1. ����� �������. 2. ����� ������ 3� ��������
	};
};

#endif