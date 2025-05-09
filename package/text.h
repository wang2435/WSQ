#ifndef TEXT_H
#define TEXT_H
#include <Winsock2.h>//���������ǰ
#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <cstring>
#include <ws2tcpip.h>
#include <vector>
#pragma comment(lib, "ws2_32.lib")
#pragma once
#pragma pack(pop)
#pragma pack(push, 1)
class service
{
public:
	WSADATA wsaData;
	SOCKET sock;
	sockaddr_in serverAddr;
	struct RVData
	{
		uint8_t a1;
		uint8_t a2;
		uint8_t a3;
		uint8_t a4;

		struct LocationStatusInfo
		{
			float x;
			float y;
			float r;
			uint32_t ID1;
			uint32_t ID2;
			uint32_t fd;         // ���ͨ�����������е����к�
			uint8_t Confidence;
			uint8_t preserve;
		} locationStatusInfo;    // <<<<<< ע����ϱ�����

		struct RunningStatusInfo
		{
			float x;
			float y;
			float r;
			uint8_t work;         // ����ģʽ
			uint8_t state;        // ������״̬
			uint8_t ability;      // ����������������״̬
			uint8_t preserve;     // Ԥ��
		} runningStatusInfo;

		struct TaskStatusInfo
		{
			uint32_t ID;          // ����ID��0Ϊ������
			uint32_t KEY;         // ����
			uint8_t point_size;   // ��״̬��������
			uint8_t path_size;    // ��״̬��������
			uint8_t preserve;     // Ԥ��

			// ��״̬����
			struct PointStateSequence
			{
				uint32_t point;
				uint32_t ID;
			} pointStateSequence;   // ����һ���㣬�����Ҫ���Ҫ������

			// ��״̬����
			struct PathStateSequence
			{
				uint32_t path;
				uint32_t ID;
			} pathStateSequence;    // ͬ�ϣ�����һ����
		} taskStatusInfo;

		struct BatteryStatusInfo
		{
			float percentage;
			float V;
			float I;
			uint8_t state;
			uint8_t preserve;
		} batteryStatusInfo;

		struct AbnormalEventStatusInfo
		{
			uint16_t event;
			uint16_t anomaly;
			uint8_t preserve;
		} abnormalEventStatusInfo;

		struct ActionInfo
		{
			uint32_t ID;
			uint8_t state;
			uint8_t preserve;
		} actionInfo;
	};

	struct Data
	{
		uint16_t  data1;
		uint16_t  data2;
		uint32_t  data3;
		uint8_t  data4[];
	};
public:
	service(int com, const char adress[]);//���캯������UDP Socket ����AGV��ַ���滻Ϊʵ��IP�Ͷ˿ڣ�
	~service();
	void receive();
};
class client
{
public:
	WSADATA wsaData;
	SOCKET sock;
	sockaddr_in destAddr{};
	struct Header
	{
		uint8_t a1[16];
		uint8_t a2;
		uint8_t a3;
		uint16_t a4;
		uint8_t a5;
		uint8_t a6;
		uint8_t a7;
		uint8_t a8;
		uint16_t a9;
		uint8_t  a10[2];
	};
	struct Data
	{
		uint16_t  data1;
		uint16_t  data2;
		uint32_t  data3;
		uint8_t  data4[184];
	};
	struct Action
	{
		uint16_t type;//��������
		uint8_t method;//���з�ʽ 0����+�ƶ� 1���� 2����
		uint8_t preserve1;
		uint32_t dzID;//����ID
		uint8_t param_size;
		uint8_t preserve2[3];
		uint32_t param1;
		uint32_t param2;
	};

public:
	client(int com, const char adress[]);//���캯������UDP Socket ����AGV��ַ���滻Ϊʵ��IP�Ͷ˿ڣ�
	~client();
	void subscribe(uint16_t& aa4, uint16_t method, uint16_t time_interval, uint32_t time);//���ĺ�����
	void pause_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint8_t stop);//������ͣ
	void resume_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint32_t ID, uint16_t KEY);//����ָ�
	void cancel_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint16_t ID, uint8_t stop);//����ȡ��
	void pallet_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint32_t move);//��������
};

#endif
