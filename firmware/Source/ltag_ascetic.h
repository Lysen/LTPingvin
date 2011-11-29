#include <avr/io.h>        // ���������� ����� ��� ������ �����-�����
#include <util/delay.h>    // ���� ����������� ������������ ��������
#include <avr/interrupt.h> // ����� ������������ ����������
#include <avr/pgmspace.h>  //����� ������� ��������� � ������ ��������

#include "definition_of_ports_atmega16.h"
#include "hal.h"
#include "miles_protocol.h"
#include "types.h"







//����������� ���� �������

void configuring_ports(void);		//������������ ������
void init_timer2(void); 			//��������� ������� timer2, ����� �������������� ��� ����� � �������� ��-�������
void init_int0(void);				//����������� ������� ���������� ������ INT0
void set_buffer_bit(uint8_t, bool);	//������ �������� ���� � ������ ��-���������
void send_ir_package(void);			//���������� �������������� ����� (�������)
void set_player_id(uint8_t);		//������ ������ �������������
void set_team_color(tteam_color);	//������ ���� ����� �������
void set_gun_damage(tgun_damage);	//������ ��������� ������ ������ (����)
void init_var(void);				//����������� ���������� �������� ����������
bool get_buffer_bit(uint8_t);		//��������� �������� ���� � ������ ��-���������
inline trx_packet get_packet_value(void); //��������� ������ �� ����������� ������
tteam_color team_id(void);//���������� ���� ����� ������� 
tgun_damage gun_damage(void);//���������� ������� ����, ��������� ����� �����
void init_timer0(void); //����������� timer0 �� ����� �������� ���, ��� ������ 
void init_timer1(void); //����������� timer1 �� ������� ������� ����� -8 
void display_life(uint8_t life_value);//���������� ������� ����� �� ������������ 
inline  TKEYBOARD_STATUS get_keyboard_status(void); //���������, ����� �� �����
inline  TKEYBOARD_EVENT test_keyboard(void);//��������� ������� ����������
uint8_t bullets_limit(void);//���������� ����� ��������
TFIRE_MODE_STATUS fire_mode(void);//���������� ������� ����� ���� (���������/���������)




//inline trx_packet get_packet_value(void); //��������� ������ �� ����������� 



//���������� ����������

extern volatile uint16_t timer1; 

extern volatile bool start_bit_received;				//���� ���� ��������������, ���� ������ �����-���
extern volatile uint16_t high_level_counter; 			//������� ������������ ������� �������� ������ �� ������ ��-���������										//������� ����� �������� ���������
extern volatile uint16_t bit_in_rx_buff; 				//���������� ���, �������� ��-���������� 
extern volatile trx_event rx_event; 					//������� ��-���������
extern volatile uint16_t low_level_counter; 			//������� ������������ ������� ������� ������ �� ������ ��-���������										
extern volatile uint8_t rx_buffer[RX_BUFFER_SIZE]; 	//������ ��-���������
extern volatile bool ir_transmitter_on;				//����, ����������� (true) ��� ����������� �������� ������ ����� ��-���� 														//������� ����� �������� ���������
extern volatile int ir_pulse_counter; 					//�������� ������� "�������" ��-�����
extern volatile int ir_space_counter; 					//�������� ������� ������������ ������������ ��������� ��-����� (����� ����� ������) 
//extern volatile trx_packet rx_packet;

extern volatile union data_packet_union  data_packet; 	//� ���� ���������� ����� ����������� ����� ������ ��� �������� ����� IR
extern volatile uint8_t cursor_position;				//��� ���������� ����� ������� ����� �������� ������� ������ data_packet.data[x]
extern volatile union data_packet_union  data_packet; 	//� ���� ���������� ����� ����������� ����� ������ ��� �������� ����� IR
extern uint8_t damage_value [] PROGMEM;
extern volatile trx_packet rx_packet;
extern volatile uint8_t life; //������� ����� (��������)
extern volatile uint8_t life_leds_status[4]; //���� ������ ����� ������� ��������� ����������� ���������� ��������
extern volatile TKEYBOARD_EVENT  keyboard_event; //������� ���������� 
extern volatile struct pressing_duration key_pressing_duration;//��������� ������ �������� ������������ ������� ������
extern volatile uint8_t fire_led_status; //������ ���������� ������� �������� 
extern volatile uint16_t bullets;  //���������� �������� � ����
extern volatile uint16_t last_simple;					//���������� ����� ��������� ������� �����

