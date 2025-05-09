#include "text.h"
client::client(int com, const char adress[])
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartup失败: " << WSAGetLastError() << std::endl;
	}
	// 创建UDP Socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "创建Socket失败: " << WSAGetLastError() << std::endl;
		WSACleanup();
	}
	// 配置AGV地址（替换为实际IP和端口）
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(com); // AGV端口
	inet_pton(AF_INET, adress, &destAddr.sin_addr);
}
client::~client()//析构函数关闭sock;
{
	closesocket(sock);
	WSACleanup();
}
void client::subscribe(uint16_t& aa4, uint16_t method, uint16_t time_interval, uint32_t time)
{
	// 构造报头
	Header header{};
	memset(&header, 0, sizeof(Header)); // 清空保留字段
	//const char* str = "HEADER_A1_DATA";
	uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
	memcpy(header.a1, Header_Array1, sizeof(header.a1)); // 填充a1数组
	header.a2 = 0x01;
	header.a3 = 0x00;//请求报文
	header.a4 = aa4;//用于分别每条指令
	aa4++;
	header.a5 = 0x10;
	header.a6 = 0xb1;//区分命令类型
	header.a7 = 0x00;//请求报文置0
	header.a8 = 0x00;//预留置0
	header.a9 = 0xc0;//数据区长度
	uint8_t Header_Array2[] = { 0x00,0x00 };
	memcpy(header.a10, Header_Array2, sizeof(header.a10)); // 填充a10数组

	// 构造数据区
	Data data{};
	memset(&data, 0, sizeof(Data)); // 清空保留字段
	data.data1 = method;
	data.data2 = time_interval;//上报时间间隔
	data.data3 = time;//上报时间
	char buffer[sizeof(Header) + sizeof(Data)];
	memcpy(buffer, &header, sizeof(Header));
	memcpy(buffer + sizeof(Header), &data, sizeof(Data));

	// 发送数据
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
		std::cerr << "发送失败: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "已发送" << sentBytes << "字节数据" << std::endl;
	}
}
void client::pause_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint8_t stop)
{
	// 构造报头
	Header header{};
	memset(&header, 0, sizeof(Header)); // 清空保留字段
	//const char* str = "HEADER_A1_DATA";
	uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
	memcpy(header.a1, Header_Array1, sizeof(header.a1)); // 填充a1数组
	header.a2 = 0x01;
	header.a3 = 0x00;//请求报文
	header.a4 = aa4;//用于分别每条指令
	aa4++;
	header.a5 = 0x10;//服务码
	header.a6 = 0xb2;//区分命令类型
	header.a7 = 0x00;//请求报文置0
	header.a8 = 0x00;//预留置0
	header.a9 = 0x10;//数据区长度
	uint8_t Header_Array2[] = { 0x00,0x00 };
	memcpy(header.a10, Header_Array2, sizeof(header.a10)); // 填充a10数组

	//构造数据区
	Action pause_action{};
	memset(&pause_action, 0, sizeof(Action)); // 清空保留字段
	pause_action.type = (uint16_t)0x01;
	pause_action.method = way;//0 1 2
	pause_action.preserve1 = (uint8_t)0x00;
	pause_action.dzID = dzid;
	pause_action.param_size = (uint8_t)0x04;
	pause_action.preserve2[0] = 0x00;
	pause_action.preserve2[1] = 0x00;
	pause_action.preserve2[2] = 0x00;
	pause_action.param1 = stop;

	// 发送数据
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
		std::cerr << "发送失败: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "已发送" << sentBytes << "字节数据" << std::endl;
	}
};
void client::resume_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint32_t ID, uint16_t KEY)
{
	// 构造报头
	Header header{};
	memset(&header, 0, sizeof(Header)); // 清空保留字段
	//const char* str = "HEADER_A1_DATA";
	uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
	memcpy(header.a1, Header_Array1, sizeof(header.a1)); // 填充a1数组
	header.a2 = 0x01;
	header.a3 = 0x00;//请求报文
	header.a4 = aa4;//用于分别每条指令
	aa4++;
	header.a5 = 0x10;//服务码
	header.a6 = 0xb2;//区分命令类型
	header.a7 = 0x00;//请求报文置0
	header.a8 = 0x00;//预留置0
	header.a9 = 0x14;//数据区长度
	uint8_t Header_Array2[] = { 0x00,0x00 };
	memcpy(header.a10, Header_Array2, sizeof(header.a10)); // 填充a10数组

	//构造数据区
	Action resume_action{};
	memset(&resume_action, 0, sizeof(Action)); // 清空保留字段
	resume_action.type = (uint16_t)0x02;
	resume_action.method = way;
	resume_action.preserve1 = (uint8_t)0x00;
	resume_action.dzID = dzid;
	resume_action.param_size = (uint8_t)0x08;
	resume_action.preserve2[0] = (uint8_t)0x00;//预留3个00
	resume_action.preserve2[1] = (uint8_t)0x00;
	resume_action.preserve2[2] = (uint8_t)0x00;
	resume_action.param1 = ID;
	resume_action.param2 = KEY;

	// 发送数据
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
		std::cerr << "发送失败: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "已发送" << sentBytes << "字节数据" << std::endl;
	}
}
void client::cancel_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint16_t ID, uint8_t stop)
{
	// 构造报头
	Header header{};
	memset(&header, 0, sizeof(Header)); // 清空保留字段
	//const char* str = "HEADER_A1_DATA";
	uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
	memcpy(header.a1, Header_Array1, sizeof(header.a1)); // 填充a1数组
	header.a2 = 0x01;
	header.a3 = 0x00;//请求报文
	header.a4 = aa4;//用于分别每条指令
	aa4++;
	header.a5 = 0x10;//服务码
	header.a6 = 0xb2;//区分命令类型
	header.a7 = 0x00;//请求报文置0
	header.a8 = 0x00;//预留置0
	header.a9 = 0x14;//数据区长度
	uint8_t Header_Array2[] = { 0x00,0x00 };
	memcpy(header.a10, Header_Array2, sizeof(header.a10)); // 填充a10数组

	//构造数据区
	Action cancel_action{};
	memset(&cancel_action, 0, sizeof(Action)); // 清空保留字段
	cancel_action.type = (uint16_t)0x03;
	cancel_action.method = way;//0 1 2三种
	cancel_action.preserve1 = (uint8_t)0x00;
	cancel_action.dzID = dzid;
	cancel_action.param_size = (uint8_t)0x08;
	cancel_action.preserve2[0] = (uint8_t)0x00;//预留3个00
	cancel_action.preserve2[1] = (uint8_t)0x00;
	cancel_action.preserve2[2] = (uint8_t)0x00;
	cancel_action.param1 = ID;
	cancel_action.param2 = stop;//0为正常 1为立即停止

	// 发送数据
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
		std::cerr << "发送失败: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "已发送" << sentBytes << "字节数据" << std::endl;
	}
}
void client::pallet_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint32_t move)
{
	// 构造报头
	Header header{};
	memset(&header, 0, sizeof(Header)); // 清空保留字段
	//const char* str = "HEADER_A1_DATA";
	uint8_t Header_Array1[] = { 0xff,0x7f,0xa3, 0x23, 0x8b, 0x4e ,0x52, 0x4d, 0x93, 0xa0, 0x42, 0xe5, 0x10 ,0xf0 ,0x2d,0xb7 };
	memcpy(header.a1, Header_Array1, sizeof(header.a1)); // 填充a1数组
	header.a2 = 0x01;
	header.a3 = 0x00;//请求报文
	header.a4 = aa4;//用于分别每条指令
	aa4++;
	header.a5 = 0x10;//服务码
	header.a6 = 0xb2;//区分命令类型
	header.a7 = 0x00;//请求报文置0
	header.a8 = 0x00;//预留置0
	header.a9 = 0x10;//数据区长度
	uint8_t Header_Array2[] = { 0x00,0x00 };
	memcpy(header.a10, Header_Array2, sizeof(header.a10)); // 填充a10数组

	//构造数据区
	Action pallet_action{};
	memset(&pallet_action, 0, sizeof(Action)); // 清空保留字段
	pallet_action.type = (uint16_t)0x16;
	pallet_action.method = way;//0 1 2三种
	pallet_action.preserve1 = (uint8_t)0x00;
	pallet_action.dzID = dzid;
	pallet_action.param_size = (uint8_t)0x04;
	pallet_action.preserve2[0] = (uint8_t)0x00;//预留3个00
	pallet_action.preserve2[1] = (uint8_t)0x00;
	pallet_action.preserve2[2] = (uint8_t)0x00;
	pallet_action.param1 = move;

	// 发送数据
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
		std::cerr << "发送失败: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "已发送" << sentBytes << "字节数据" << std::endl;
	}
}
service::service(int com, const char adress[])//构造函数
{
	// 初始化Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartup失败: " << WSAGetLastError() << std::endl;
	}

	// 创建UDP Socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "创建Socket失败: " << WSAGetLastError() << std::endl;
		WSACleanup();
	}

	// 绑定本地端口
	serverAddr.sin_family = AF_INET;
	unsigned long ip = 0;
	inet_pton(AF_INET, adress, &ip);
	serverAddr.sin_addr.s_addr = ip;
	serverAddr.sin_port = htons(com);
	if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "绑定端口失败: " << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
	}
	std::cout << "等待接收数据..." << std::endl;
}
service::~service()//析构函数关闭sock;
{
	closesocket(sock);
	WSACleanup();
}
void service::receive()
{
	// 接收数据
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
		std::cerr << "接收失败: " << WSAGetLastError() << std::endl;
	}
	else if (recvBytes != sizeof(buffer))
	{
		std::cerr << "数据不完整，期望" << sizeof(buffer) << "字节，实际收到" << recvBytes << "字节" << std::endl;
	}
	else
	{
		RVData* data = reinterpret_cast<RVData*>(buffer);

		std::cout << "\n==== 收到RVData报文 ====" << std::endl;
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

		std::cout << "==== 解析完成 ====" << std::endl;
	}
}