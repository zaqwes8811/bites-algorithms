/**
	file : #include "Protocols.h"

	��� �����������
	void updateInfo()
{
  ...
}

map <string, void (*)()> mFunctions;
mFunctions["updateInfo"] = updateInfo;
//� ��� ����� ��� ���� �������, ��� ����� ������������
...
string sFunctionName;
... 
//���-�� ��� ������� ��� �������

mFunctions[sFunctionName]();//��������� ������ �������
mFunctions["updateInfo"]();//�� �� �����

Updated 11.04.2007
[����� ��������� ������� � ������������ ��������, ��� 
���� ������ �����, ������������ ������������� � "����������� ��������������".]
*/

#ifndef _PROTOCOLS
#define _PROTOCOLS

// for debug
#include <uni_convertors/convertors.h>
#include "forDebug.h"

// ����������
#include "RS485.h"
#include "UdpExchangeClasses.h"
// stl
#include <memory>
#include <string>
typedef int ErrCode;
typedef CString Settings;
/// /// ///
///
///
namespace protocols {
/**

  TODO : 
    ������� ����������� � ����������� 
	  �������� ����� ������� �� ������������ 
*/
class IProtocol {
	// ��������� ��������� �� ���������� �������
public : 
	virtual ~IProtocol(void);
	explicit IProtocol();
	
	// ������� ��������
	// ���������� ������ � ������� � ������� �� ������ �� �������
	BOOL send( _FormatRS485Data* pDataRS485, void (*printTx)(UCHAR*, int));
	BOOL receive( FormatDataToSend* pDataRS485, void (*printRx)(UCHAR*, int) );

	// �������� ����������������� ������������� ������
	const BOOL testValidTransLayer() const;

	// ��� ��������� ������������� ������ - ����� ������ ����!!
	virtual ErrCode resetLayerParameters( Settings set );
	virtual ErrCode freeTransportLayer();

private : 
	// ������ ����������� ������
	mutable Ifaces::RS485 _rs485;

	// ���������� �������
	uchar ReceiveTempData[1024];
	int BadMess;
	uchar OldReceiveByte;

#ifdef CONTENT_UDP_CLIENT
	::CRITICAL_SECTION csUDPSender_;
	std::auto_ptr<transport_layers::IHardIfaceTop> hift_;
	// �������� ���������
	void sendMsg_( const FormatMsg& scTx );
	// �������� udp-�������
	
#endif
	void init_();
};

	// ������� ��������� 
void printTx(UCHAR*, int);
void printRx(UCHAR*, int);
void p0(UCHAR*, int);

}  // end namespace
#endif
