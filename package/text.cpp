#include "text.h"
client::client(int com, const char adress[])
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartupʧ��: " << WSAGetLastError() << std::endl;
	}
	// ����UDP Socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "����Socketʧ��: " << WSAGetLastError() << std::endl;
		WSACleanup();
	}
	// ����AGV��ַ���滻Ϊʵ��IP�Ͷ˿ڣ�
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(com); // AGV�˿�
	inet_pton(AF_INET, adress, &destAddr.sin_addr);
}
client::~client()//���������ر�sock;
{
	closesocket(sock);
	WSACleanup();
}
void client::subscribe(uint16_t& aa4, uint16_t method, uint16_t time_interval, uint32_t time)
{
	// ���챨ͷ
	Header header{};
	memset(&header, 0, sizeof(Header)); // ��ձ����ֶ�
	//const char* str = "HEADER_A1_DATA";
	uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
	memcpy(header.a1, Header_Array1, sizeof(header.a1)); // ���a1����
	header.a2 = 0x01;
	header.a3 = 0x00;//������
	header.a4 = aa4;//���ڷֱ�ÿ��ָ��
	aa4++;
	header.a5 = 0x10;
	header.a6 = 0xb1;//������������
	header.a7 = 0x00;//��������0
	header.a8 = 0x00;//Ԥ����0
	header.a9 = 0xc0;//����������
	uint8_t Header_Array2[] = { 0x00,0x00 };
	memcpy(header.a10, Header_Array2, sizeof(header.a10)); // ���a10����

	// ����������
	Data data{};
	memset(&data, 0, sizeof(Data)); // ��ձ����ֶ�
	data.data1 = method;
	data.data2 = time_interval;//�ϱ�ʱ����
	data.data3 = time;//�ϱ�ʱ��
	char buffer[sizeof(Header) + sizeof(Data)];
	memcpy(buffer, &header, sizeof(Header));
	memcpy(buffer + sizeof(Header), &data, sizeof(Data));

	// ��������
	int sentBytes = sendto
	(
		sock,
		buffer,
		sizeof(buffer),
		0,
		(sockaddr*)&destAddr,
		sizeof(destAddr)
	);
	if (sentBytes == SOCKET_ERROR)
	{
		std::cerr << "����ʧ��: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "�ѷ���" << sentBytes << "�ֽ�����" << std::endl;
	}
}
void client::pause_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint8_t stop)
{
	// ���챨ͷ
	Header header{};
	memset(&header, 0, sizeof(Header)); // ��ձ����ֶ�
	//const char* str = "HEADER_A1_DATA";
	uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
	memcpy(header.a1, Header_Array1, sizeof(header.a1)); // ���a1����
	header.a2 = 0x01;
	header.a3 = 0x00;//������
	header.a4 = aa4;//���ڷֱ�ÿ��ָ��
	aa4++;
	header.a5 = 0x10;//������
	header.a6 = 0xb2;//������������
	header.a7 = 0x00;//��������0
	header.a8 = 0x00;//Ԥ����0
	header.a9 = 0x10;//����������
	uint8_t Header_Array2[] = { 0x00,0x00 };
	memcpy(header.a10, Header_Array2, sizeof(header.a10)); // ���a10����

	//����������
	Action pause_action{};
	memset(&pause_action, 0, sizeof(Action)); // ��ձ����ֶ�
	pause_action.type = (uint16_t)0x01;
	pause_action.method = way;//0 1 2
	pause_action.preserve1 = (uint8_t)0x00;
	pause_action.dzID = dzid;
	pause_action.param_size = (uint8_t)0x04;
	pause_action.preserve2[0] = 0x00;
	pause_action.preserve2[1] = 0x00;
	pause_action.preserve2[2] = 0x00;
	pause_action.param1 = stop;

	// ��������
	char buffer[sizeof(Header) + sizeof(Action)];
	memcpy(buffer, &header, sizeof(Header));
	memcpy(buffer + sizeof(Header), &pause_action, sizeof(Action));
	int sentBytes = sendto
	(
		sock,
		buffer,
		sizeof(buffer),
		0,
		(sockaddr*)&destAddr,
		sizeof(destAddr)
	);

	if (sentBytes == SOCKET_ERROR)
	{
		std::cerr << "����ʧ��: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "�ѷ���" << sentBytes << "�ֽ�����" << std::endl;
	}
};
void client::resume_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint32_t ID, uint16_t KEY)
{
	// ���챨ͷ
	Header header{};
	memset(&header, 0, sizeof(Header)); // ��ձ����ֶ�
	//const char* str = "HEADER_A1_DATA";
	uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
	memcpy(header.a1, Header_Array1, sizeof(header.a1)); // ���a1����
	header.a2 = 0x01;
	header.a3 = 0x00;//������
	header.a4 = aa4;//���ڷֱ�ÿ��ָ��
	aa4++;
	header.a5 = 0x10;//������
	header.a6 = 0xb2;//������������
	header.a7 = 0x00;//��������0
	header.a8 = 0x00;//Ԥ����0
	header.a9 = 0x14;//����������
	uint8_t Header_Array2[] = { 0x00,0x00 };
	memcpy(header.a10, Header_Array2, sizeof(header.a10)); // ���a10����

	//����������
	Action resume_action{};
	memset(&resume_action, 0, sizeof(Action)); // ��ձ����ֶ�
	resume_action.type = (uint16_t)0x02;
	resume_action.method = way;
	resume_action.preserve1 = (uint8_t)0x00;
	resume_action.dzID = dzid;
	resume_action.param_size = (uint8_t)0x08;
	resume_action.preserve2[0] = (uint8_t)0x00;//Ԥ��3��00
	resume_action.preserve2[1] = (uint8_t)0x00;
	resume_action.preserve2[2] = (uint8_t)0x00;
	resume_action.param1 = ID;
	resume_action.param2 = KEY;

	// ��������
	char buffer[sizeof(Header) + sizeof(Action)];
	memcpy(buffer, &header, sizeof(Header));
	memcpy(buffer + sizeof(Header), &resume_action, sizeof(Action));
	int sentBytes = sendto
	(
		sock,
		buffer,
		sizeof(buffer),
		0,
		(sockaddr*)&destAddr,
		sizeof(destAddr)
	);

	if (sentBytes == SOCKET_ERROR)
	{
		std::cerr << "����ʧ��: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "�ѷ���" << sentBytes << "�ֽ�����" << std::endl;
	}
}
void client::cancel_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint16_t ID, uint8_t stop)
{
	// ���챨ͷ
	Header header{};
	memset(&header, 0, sizeof(Header)); // ��ձ����ֶ�
	//const char* str = "HEADER_A1_DATA";
	uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
	memcpy(header.a1, Header_Array1, sizeof(header.a1)); // ���a1����
	header.a2 = 0x01;
	header.a3 = 0x00;//������
	header.a4 = aa4;//���ڷֱ�ÿ��ָ��
	aa4++;
	header.a5 = 0x10;//������
	header.a6 = 0xb2;//������������
	header.a7 = 0x00;//��������0
	header.a8 = 0x00;//Ԥ����0
	header.a9 = 0x14;//����������
	uint8_t Header_Array2[] = { 0x00,0x00 };
	memcpy(header.a10, Header_Array2, sizeof(header.a10)); // ���a10����

	//����������
	Action cancel_action{};
	memset(&cancel_action, 0, sizeof(Action)); // ��ձ����ֶ�
	cancel_action.type = (uint16_t)0x03;
	cancel_action.method = way;//0 1 2����
	cancel_action.preserve1 = (uint8_t)0x00;
	cancel_action.dzID = dzid;
	cancel_action.param_size = (uint8_t)0x08;
	cancel_action.preserve2[0] = (uint8_t)0x00;//Ԥ��3��00
	cancel_action.preserve2[1] = (uint8_t)0x00;
	cancel_action.preserve2[2] = (uint8_t)0x00;
	cancel_action.param1 = ID;
	cancel_action.param2 = stop;//0Ϊ���� 1Ϊ����ֹͣ

	// ��������
	char buffer[sizeof(Header) + sizeof(Action)];
	memcpy(buffer, &header, sizeof(Header));
	memcpy(buffer + sizeof(Header), &cancel_action, sizeof(Action));
	int sentBytes = sendto
	(
		sock,
		buffer,
		sizeof(buffer),
		0,
		(sockaddr*)&destAddr,
		sizeof(destAddr)
	);

	if (sentBytes == SOCKET_ERROR)
	{
		std::cerr << "����ʧ��: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "�ѷ���" << sentBytes << "�ֽ�����" << std::endl;
	}
}
void client::pallet_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint32_t move)
{
	// ���챨ͷ
	Header header{};
	memset(&header, 0, sizeof(Header)); // ��ձ����ֶ�
	//const char* str = "HEADER_A1_DATA";
	uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
	memcpy(header.a1, Header_Array1, sizeof(header.a1)); // ���a1����
	header.a2 = 0x01;
	header.a3 = 0x00;//������
	header.a4 = aa4;//���ڷֱ�ÿ��ָ��
	aa4++;
	header.a5 = 0x10;//������
	header.a6 = 0xb2;//������������
	header.a7 = 0x00;//��������0
	header.a8 = 0x00;//Ԥ����0
	header.a9 = 0x10;//����������
	uint8_t Header_Array2[] = { 0x00,0x00 };
	memcpy(header.a10, Header_Array2, sizeof(header.a10)); // ���a10����

	//����������
	Action pallet_action{};
	memset(&pallet_action, 0, sizeof(Action)); // ��ձ����ֶ�
	pallet_action.type = (uint16_t)0x16;
	pallet_action.method = way;//0 1 2����
	pallet_action.preserve1 = (uint8_t)0x00;
	pallet_action.dzID = dzid;
	pallet_action.param_size = (uint8_t)0x04;
	pallet_action.preserve2[0] = (uint8_t)0x00;//Ԥ��3��00
	pallet_action.preserve2[1] = (uint8_t)0x00;
	pallet_action.preserve2[2] = (uint8_t)0x00;
	pallet_action.param1 = move;

	// ��������
	char buffer[sizeof(Header) + sizeof(Action)];
	memcpy(buffer, &header, sizeof(Header));
	memcpy(buffer + sizeof(Header), &pallet_action, sizeof(Action));
	int sentBytes = sendto
	(
		sock,
		buffer,
		sizeof(buffer),
		0,
		(sockaddr*)&destAddr,
		sizeof(destAddr)
	);

	if (sentBytes == SOCKET_ERROR)
	{
		std::cerr << "����ʧ��: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "�ѷ���" << sentBytes << "�ֽ�����" << std::endl;
	}
}
service::service(int com, const char adress[])//���캯��
{
	// ��ʼ��Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartupʧ��: " << WSAGetLastError() << std::endl;
	}

	// ����UDP Socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "����Socketʧ��: " << WSAGetLastError() << std::endl;
		WSACleanup();
	}

	// �󶨱��ض˿�
	serverAddr.sin_family = AF_INET;
	unsigned long ip = 0;
	inet_pton(AF_INET, adress, &ip);
	serverAddr.sin_addr.s_addr = ip;
	serverAddr.sin_port = htons(com);
	if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "�󶨶˿�ʧ��: " << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
	}
	std::cout << "�ȴ���������..." << std::endl;
}
service::~service()//���������ر�sock;
{
	closesocket(sock);
	WSACleanup();
}
void service::receive()
{
	// ��������
	char buffer[sizeof(RVData)]{};
	sockaddr_in clientAddr{};
	int clientAddrLen = sizeof(clientAddr);

	int recvBytes = recvfrom
	(
		sock,
		buffer,
		sizeof(buffer),
		0,
		(sockaddr*)&clientAddr,
		&clientAddrLen
	);

	if (recvBytes == SOCKET_ERROR)
	{
		std::cerr << "����ʧ��: " << WSAGetLastError() << std::endl;
	}
	else if (recvBytes != sizeof(buffer))
	{
		std::cerr << "���ݲ�����������" << sizeof(buffer) << "�ֽڣ�ʵ���յ�" << recvBytes << "�ֽ�" << std::endl;
	}
	else
	{
		RVData* data = reinterpret_cast<RVData*>(buffer);

		std::cout << "\n==== �յ�RVData���� ====" << std::endl;
		std::cout << "a1: 0x" << std::hex << (int)data->a1 << std::endl;
		std::cout << "a2: 0x" << std::hex << (int)data->a2 << std::endl;
		std::cout << "a3: 0x" << std::hex << (int)data->a3 << std::endl;
		std::cout << "a4: 0x" << std::hex << (int)data->a4 << std::endl;

		std::cout << "\n--- LocationStatusInfo ---" << std::endl;
		std::cout << "x: " << std::dec << data->locationStatusInfo.x << std::endl;
		std::cout << "y: " << data->locationStatusInfo.y << std::endl;
		std::cout << "r: " << data->locationStatusInfo.r << std::endl;
		std::cout << "ID1: " << data->locationStatusInfo.ID1 << std::endl;
		std::cout << "ID2: " << data->locationStatusInfo.ID2 << std::endl;
		std::cout << "fd: " << data->locationStatusInfo.fd << std::endl;
		std::cout << "Confidence: " << (int)data->locationStatusInfo.Confidence << std::endl;

		std::cout << "\n--- RunningStatusInfo ---" << std::endl;
		std::cout << "x: " << data->runningStatusInfo.x << std::endl;
		std::cout << "y: " << data->runningStatusInfo.y << std::endl;
		std::cout << "r: " << data->runningStatusInfo.r << std::endl;
		std::cout << "work: " << (int)data->runningStatusInfo.work << std::endl;
		std::cout << "state: " << (int)data->runningStatusInfo.state << std::endl;
		std::cout << "ability: " << (int)data->runningStatusInfo.ability << std::endl;

		std::cout << "\n--- TaskStatusInfo ---" << std::endl;
		std::cout << "ID: " << data->taskStatusInfo.ID << std::endl;
		std::cout << "KEY: " << data->taskStatusInfo.KEY << std::endl;
		std::cout << "point_size: " << (int)data->taskStatusInfo.point_size << std::endl;
		std::cout << "path_size: " << (int)data->taskStatusInfo.path_size << std::endl;

		std::cout << "\n--- PointStateSequence ---" << std::endl;
		std::cout << "point: " << data->taskStatusInfo.pointStateSequence.point << std::endl;
		std::cout << "ID: " << data->taskStatusInfo.pointStateSequence.ID << std::endl;

		std::cout << "\n--- PathStateSequence ---" << std::endl;
		std::cout << "path: " << data->taskStatusInfo.pathStateSequence.path << std::endl;
		std::cout << "ID: " << data->taskStatusInfo.pathStateSequence.ID << std::endl;

		std::cout << "\n--- BatteryStatusInfo ---" << std::endl;
		std::cout << "percentage: " << data->batteryStatusInfo.percentage << "%" << std::endl;
		std::cout << "V: " << data->batteryStatusInfo.V << "V" << std::endl;
		std::cout << "I: " << data->batteryStatusInfo.I << "A" << std::endl;
		std::cout << "state: " << (int)data->batteryStatusInfo.state << std::endl;

		std::cout << "\n--- AbnormalEventStatusInfo ---" << std::endl;
		std::cout << "event: " << data->abnormalEventStatusInfo.event << std::endl;
		std::cout << "anomaly: " << data->abnormalEventStatusInfo.anomaly << std::endl;

		std::cout << "\n--- ActionInfo ---" << std::endl;
		std::cout << "ID: " << data->actionInfo.ID << std::endl;
		std::cout << "state: " << (int)data->actionInfo.state << std::endl;

		std::cout << "==== ������� ====" << std::endl;
	}
}