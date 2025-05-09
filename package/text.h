#ifndef TEXT_H
#define TEXT_H
#include <Winsock2.h>//这个必须在前
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
			uint32_t fd;         // 最后通过点在任务中的序列号
			uint8_t Confidence;
			uint8_t preserve;
		} locationStatusInfo;    // <<<<<< 注意加上变量名

		struct RunningStatusInfo
		{
			float x;
			float y;
			float r;
			uint8_t work;         // 工作模式
			uint8_t state;        // 机器人状态
			uint8_t ability;      // 机器人能力集设置状态
			uint8_t preserve;     // 预留
		} runningStatusInfo;

		struct TaskStatusInfo
		{
			uint32_t ID;          // 订单ID，0为无任务
			uint32_t KEY;         // 任务
			uint8_t point_size;   // 点状态序列数量
			uint8_t path_size;    // 段状态序列数量
			uint8_t preserve;     // 预留

			// 点状态序列
			struct PointStateSequence
			{
				uint32_t point;
				uint32_t ID;
			} pointStateSequence;   // 单独一个点，如果需要多个要用数组

			// 段状态序列
			struct PathStateSequence
			{
				uint32_t path;
				uint32_t ID;
			} pathStateSequence;    // 同上，单独一个段
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
	service(int com, const char adress[]);//构造函数创建UDP Socket 配置AGV地址（替换为实际IP和端口）
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
		uint16_t type;//动作类型
		uint8_t method;//并行方式 0并行+移动 1并行 2动作
		uint8_t preserve1;
		uint32_t dzID;//动作ID
		uint8_t param_size;
		uint8_t preserve2[3];
		uint32_t param1;
		uint32_t param2;
	};

public:
	client(int com, const char adress[]);//构造函数创建UDP Socket 配置AGV地址（替换为实际IP和端口）
	~client();
	void subscribe(uint16_t& aa4, uint16_t method, uint16_t time_interval, uint32_t time);//订阅函数；
	void pause_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint8_t stop);//任务暂停
	void resume_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint32_t ID, uint16_t KEY);//任务恢复
	void cancel_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint16_t ID, uint8_t stop);//任务取消
	void pallet_action(uint16_t& aa4, uint8_t way, uint32_t dzid, uint32_t move);//托盘升降
};

#endif
