#define RX_BUFFER_SIZE   8 //������ ������� ���������
#define TX_BUFFER_SIZE   8 //������ ������� ���������

#ifndef bool

                        #define bool unsigned char

                        #define true 1

                        #define false 0

#endif



//---------------------------------------------------------------------//


//��������� ������������� ��� ��� ������� ��-���������
enum Rx_Event {	
				NOT_EVENT, 		//��� �������
				RX_COMPLETE, 	//������ �����
				RX_ERROR		//������ ������ ������
				}; 
typedef enum Rx_Event trx_event;


//---------------------------------------------------------------------//

//��������� ��������� ��� �������� �������������� ������
//� ��� ����� ������� ������������ ��������  � "�����" ������� (IR_ZERO ��� IR_ONE)
typedef struct PlayerID {
uint8_t bit_7;//��������� ��� (������ ������ ���� ����� IR_ZERO)
uint8_t bit_6; 
uint8_t bit_5; 
uint8_t bit_4;
uint8_t bit_3;
uint8_t bit_2;
uint8_t bit_1;
uint8_t bit_0;//������ ���
} tplayer_id;

/*
union player_id_union {
tplayer_id bits;
uint8_t data [8];
};
*/


//��������� ��������� ��� �������� �������������� ������
//� ��� ����� ������� ������������ �������� � "�����" ������� (IR_ZERO ��� IR_ONE) 
typedef struct TeamID{
uint8_t bit_1;
uint8_t bit_0;
} tteam_id;
 


typedef struct Damage{
uint8_t bit_3; 
uint8_t bit_2; 
uint8_t bit_1; 
uint8_t bit_0;

} tdamage;




//������ ������ ��������� ������ ������
typedef struct DataPacket {
uint8_t header; //���������, ������ ������ ���� ����� IR_START (1 ����)
tplayer_id player_id; //����� ��������� ���� ������������� ������ (8 ����)
tteam_id team_id;	  //����� ������������� ������� (2 �����)
tdamage damage;		  //�� � ��������� ����� "����" (4 �����)
uint8_t end_of_data;  //�����, ����������� �����������, ��� ������ ��� �������� ������ ��� (������ ������ ���� ����� 0) (1 ����)
} tdata_packet;

//---------------------------------------------------------------------//

union data_packet_union{
tdata_packet packet;
uint8_t data[16];
};





//������ ���������, � ������� ����� ������� ������, ��������� �� ����������� ������
typedef struct RX_Packet {
uint8_t player_id;	//������������� ��������� � ��� ������
uint8_t team_id;	//������������� (����) ��� �������
uint8_t damage;		//���������� ��� ����
} trx_packet;





enum typkeyboard_status {
			no_key_pressed,
			key_pressed
			};
typedef enum typkeyboard_status TKEYBOARD_STATUS;

enum typkeyboard_event  {
			no_key_pressing,
			key_pressing,
	        };
typedef enum typkeyboard_event TKEYBOARD_EVENT;


struct pressing_duration
		{ uint16_t key_1      ; //������������ ������������ 
					    //������� ������ 1
		  unsigned int key_1_inc:1; //���, ����������� 
					    //��� ����������� ������
					    //������������ ������� ������ 1  
		  unsigned int no_key   ;
		};



enum tfire_mode_status {
single,
queues
};
typedef enum tfire_mode_status TFIRE_MODE_STATUS;




