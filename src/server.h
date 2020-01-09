
#include <WinSock2.h>
#include <iostream>
#include <thread>
#include "zombie.h"
#pragma comment (lib , "ws2_32.lib")
using std::thread;

bool is_server = false;
char ip[16] = "127.0.0.1";

struct communication_data{
	//ship position
	vec2 p1_position=vec2(0,0);
	float angle=0.0f;
	float p1_position_z=-1.0f;
	vec2 crush_speed = vec2(0, 0);
	vec2 total_speed = vec2(0, 0);
	float crushed;

	bool plays_from_enemy[3];
	bool updates_from_enemy[3];
	bool no_fire;

	bool game_over=false;
	struct woodboard {
		vec3 position[WOODBOARD_NUM] = { 0 };
		float height[WOODBOARD_NUM] = { 0 };
		float rotate[WOODBOARD_NUM] = { 0 };
	}woodboard;
	struct barrel {
		vec3 position[BARREL_NUM] = { 0 };
		float height[BARREL_NUM] = { 0 };
		float rotate[BARREL_NUM] = { 0 };
	}barrels;
	int hp = 0, maxhp = 0, mp = 0, maxmp = 0, exper=0, maxexp=0, lv=1, death=0,round_number=3;

	//skills
	struct q_1 {
		bool turn_on = false;
		vec2 position = vec2(0, 0);
		int damage = 0;
		float sturn_time = 0;
	}q_1s;
	float chain_angle = 0.0f;
	float chain_angle2 = 0.0f;
	struct e_1 {
		bool turn_on = false;
		float alpha = 0.0f;
	}e_1s;
	struct r_1 {
		bool turn_on = false;
		float range = 20.0f;
		int damage;
	}r_1s;
	struct normal_left {
		bool render = false;
		int damage = 2;
	}normal_lefts;
	struct sphere {
		vec3 position[NUM_SPHERE] = { 0 };
		float radius = 1.0f;
	}spheres;
	struct sphere2 {
		bool is_hit[NUM_SPHERE] = { 0 };
	}spheres2;
	struct r_2 {
		bool complete = false;
	}r_2s;
	struct zombie {
		int cur = 0;
		mat4 model_matrix[ZOMBIE_NUM];
	}zombies;

	struct item {
		int woodboard = 0;
		int barrel = 0;
	}items;
}server_data;

struct communication_data2 {
	//ship position
	vec2 p2_position=vec2(0,0);
	float p2_position_z = -1.0f;
	float angle=0.0f;
	vec2 crush_speed = vec2(0, 0);
	float crushed;

	bool plays_from_enemy[3];
	bool updates_from_enemy[3];

	int hp = 0, maxhp = 0, mp = 0, maxmp = 0, exper=0, maxexp=0,lv=1, death=0,hp_shield=0;

	//skills
	struct q_1 {
		bool hit = false;
	}q_1s;
	struct e_2 {
		bool turn_on = false;
		vec3 position = { 0 };
		float radius = 14.0f;
	}e_2s;
	struct q_2 {
		vec3 position[5] = { 0 };
		float dir[5] = { 0 };
		vec2 size[5] = { 0 };
		bool available[5] = { 0 };
		int cur = 0;
		float trap_slow_down = 0.4f;
		int damage=0;
	}q_2s;
	struct r_2 {
		int damage = 0;
		int shot = 5;
		bool hit = false;
	}r_2s;
	struct sniper_bullet {
		vec3 pos = vec3(0, 0, 0);
		bool render = false;
	}sniper_bullets;
	struct zombie {
		int cur = 0;
		mat4 model_matrix[ZOMBIE_NUM];
	}zombies;
	struct normal_left {
		bool render = false;
		int damage = 2;
	}normal_lefts;
	struct sphere {
		vec3 position[NUM_SPHERE] = { 0 };
		float radius = 1.0f;
	}spheres;
	struct sphere2 {
		bool is_hit[NUM_SPHERE] = { 0 };
	}spheres2;
	bool game_start = false;
}client_data;

void showError(const char* msg)
{
	std::cout << "에러 : " << msg << std::endl;
	exit(-1);
}

int server_init() {
	//server
	WSADATA data;
	SOCKET server,client;
	sockaddr_in addr = { 0 };
	::WSAStartup(MAKEWORD(2, 2), &data);
	if (is_server == 1) {
		printf("SERVER\n");
		server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (server == INVALID_SOCKET)
			showError("Server Create Failure");

		addr.sin_family = AF_INET;
		//addr.sin_addr.s_addr = inet_addr("192.168.0.33");
		addr.sin_port = htons(23000);

		if (bind(server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
			showError("Binding Failure");

		if (listen(server, SOMAXCONN) == SOCKET_ERROR)
			showError("Listen Failure");


		std::cout << "Waiting Client.." << std::endl;

		SOCKET client = accept(server, NULL, NULL);
		
		while (!game_closed) {
			send(client, (char*)&server_data, sizeof(server_data), 0);
			int len = recv(client, (char*)&client_data, sizeof(client_data), 0);
		}
		
		closesocket(client);
		closesocket(server);
		::WSACleanup();
		printf("\nServer : Server Closed\n");
		return 0;
	}
	else { //client
		printf("CLIENT\n");
		client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (client == INVALID_SOCKET)
			showError("클라이언트 생성 실패");

		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(ip);
		addr.sin_port = htons(23000);

		if (connect(client, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
			showError("연결 실패");



		while (!game_closed) {
			int len = recv(client, (char*)&server_data, sizeof(server_data), 0);

			if (len == -1) {
				break;
			}
			printf("%d client : %f\r", len, server_data.p1_position.x);

			send(client, (char*)&client_data, sizeof(client_data), 0);
		}
		closesocket(client);
		::WSACleanup();
		printf("\nClient : Server Disconnected\n");
		return 0;
	}
}
