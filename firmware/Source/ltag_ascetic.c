#include "ltag_ascetic.h"

int main (void) {
configuring_ports();

init_timer2();
init_int0();
init_timer1();
init_timer0();
init_var();                		//����� �������� ����������

sei(); 

life_leds_status[0] = ON;
timer1 = 0;
while (timer1 < 10000);

life_leds_status[1] = ON;
timer1 = 0;
while (timer1 < 10000);

//LIFE_LED3_ON;
life_leds_status[2] = ON;
timer1 = 0;
while (timer1 < 10000);

//LIFE_LED4_ON;
life_leds_status[3] = ON;
timer1 = 0;
while (timer1 < 10000);

//FIRE_LED_ON;
fire_led_status = ON;
timer1 = 0;
while (timer1 < 10000);

BULLETS_OUT_LED_ON;
timer1 = 0;
while (timer1 < 10000);

WOUND_LED_ON;
timer1 = 0;
while (timer1 < 10000);


//LIFE_LED1_OFF;
life_leds_status[0] = OFF;
timer1 = 0;
while (timer1 < 10000);

//LIFE_LED2_OFF;
life_leds_status[1] = OFF;
timer1 = 0;
while (timer1 < 10000);

//LIFE_LED3_OFF;
life_leds_status[2] = OFF;
timer1 = 0;
while (timer1 < 10000);

//LIFE_LED4_OFF;
life_leds_status[3] = OFF;
timer1 = 0;
while (timer1 < 10000);

//FIRE_LED_OFF;
fire_led_status = OFF;
timer1 = 0;
while (timer1 < 10000);

BULLETS_OUT_LED_OFF;
timer1 = 0;
while (timer1 < 10000);

WOUND_LED_OFF;

//life_led1_status = FQR_1HZ;
//life_led2_status = FQR_2HZ;
//life_led3_status = FQR_4HZ;
//life_led4_status = ON;

display_life(life);

while(1){
switch(keyboard_event)
	{
	 	case no_key_pressing: break;
		case key_pressing:
		{
			if (bullets > 0)
			{
				bullets--;//��������� �� 1 ���������� ��������
				last_simple = 0;//������������� ���� ��������
				send_ir_package();		//���������� "�������"
				
	 		}
			keyboard_event=no_key_pressing; 
		} 
        break;
  

		default:keyboard_event=no_key_pressing;	
	}

//while(rx_event==NOT_EVENT){};

//	send_ir_package();		//���������� "�������"
			//_delay_ms(1000);    	// ��������� ��������
////	timer1=0;
////	while((rx_event==NOT_EVENT)&&(timer1 < 65000)){}; //����, ���� �� ������� ������� ��� ���������� �������
////	if(rx_event)// ���� ���� ������� ���������


//if (rx_event != NOT_EVENT) 


	{
		switch(rx_event)//�������, ����� ������ ������� ���������
					{
						case RX_COMPLETE: 	//������� �����
						{
						//	cli();
							WOUND_LED_ON; //�������� ��������������� ���������
							timer1=0;
							while(timer1 < 35000);
							WOUND_LED_OFF;	//��������� ��������������� ���������
							rx_event = NOT_EVENT;	
							if(!get_buffer_bit(0)) //���� ���� ��� ����� 0, �� ��� ����� � ������� (�������)
							{
						
								
	//							uint8_t player_id;
								rx_packet = get_packet_value();
								volatile int gg;
								gg++;

								if (rx_packet.team_id != team_id())//"����" ��������� �� ������ ������, �� �����, �������
								{
									uint8_t damage_tmp;
									switch(rx_packet.damage)
									{
										case 10:
										{
											damage_tmp = 1;
											break;
										}

										case 25:
										{
											damage_tmp = 2;
											break;
										}
																			
										case 50:
										{
											damage_tmp = 4;
											break;
										}
										case 100:
										{
											damage_tmp = 8;
											break;
										}
										default: damage_tmp = 1;
									}

									
								if (life > damage_tmp) life = life-damage_tmp;
									else 
									{	
										life = 0;
										WOUND_LED_ON;
										display_life(life);//��������� ������� ����� �� ������
										volatile uint8_t keypress_cntr; //������� ������, � ������� ������� ����� ��� �����
										keypress_cntr = 0;
										while(keypress_cntr < 20)
										{
											WOUND_LED_INVERT;
											timer1 = 0;
											while (timer1 < 10000);
											switch (FIRE_KEY_IN&FIRE_KEY_PIN) //���������, ����� �� �����
											{
												case 0:  keypress_cntr++ ; break;
												case FIRE_KEY_PIN: keypress_cntr = 0; break;
												default: keypress_cntr = 0;	
											}
						
										}

										init_var();//"�������" - �������� ����� ����
									}
								
								display_life(life);//��������� ������� ����� �� ������

								
								}

									
								



	//							rx_player_id = rx_buffer[0];
	//							rx_damage = get_damage_from_rx_paket();

							

							}
							
						
						
						//	sei();
							break;
						}
						
						case RX_ERROR:		//������ ������
						{
						//	cli();
							BULLETS_OUT_LED_ON;
							timer1=0;
							while(timer1 < 35000);
							BULLETS_OUT_LED_OFF;
							rx_event = NOT_EVENT;	
						//	sei();
							break;
						}
						
						case NOT_EVENT:		//������ ������
						{
						//	cli();
						//	rx_event = NOT_EVENT;	
						//	sei();
							break;
						}



					}



	}

//	timer1=0;				//������� �����
//	while(timer1 < 65000);	//��������, ���� �� ���������� 65000 ���������� ������� (���� ������ �������)



};
return 0;

}




void configuring_ports(){
IR_LED_DDR |= IR_LED_PIN; //�����, �� ������� ��������� �� ���� ��������� � ����� "�����"
LIFE_LEDS_DDR |= LIFE_LED1_PIN  //�����, �� �������
				| LIFE_LED2_PIN //���������� ����������,
				| LIFE_LED3_PIN //������������ ������� "�����"
				| LIFE_LED4_PIN;//���� ����������� ��� ������

SOUND_DDR |= SOUND_PIN; //����������� ����� ��� (���)
BULLETS_OUT_LED_DDR|=BULLETS_OUT_LED_PIN;
//DDRA |= (1 << 4)|(1<<5)|(1<<6)|(1<<7); // ������������� ���� PORTA.1 ��� �����                                                                                                                                                                    DDRB = 1<<DDB3;						//PB3 (OC0) set as output
//DDRD |= (1 << 7);//��������� �� �������
//DDRB |= (1 << 3);//����� ��� 
//PORTD &= ~(1 << 7);//��������� ��������� �� �������
WOUND_LED_DDR |= WOUND_LED_PIN;
TSOP_DDR &=~TSOP_PIN; //�����, � �������� ��������� ��-������ ����������� ��� "����"

}


/**************************************************************************************
* ������� ��������� ��������� ������� timer2
* ����� ������ ������ - ��� (����� ��� ����������)
* ������������ - ��� ��������, � �������� ������
* ������ ��������� ����� ����� ����� ��������, ����� ����������
* �������������� � ��������� �������� ������� ��-��������� 
***************************************************************************************/


void init_timer2(void){
OCR2 = F_CPU/IR_F0/2-1;       //���������� ����� �������������� � ��������, ����� �������, ��� ������� ������� 
TCCR2 = _BV(CS20)|_BV(WGM21); // ����� ������ ������ - ��� (����� ��� ����������)
                              // ������������ � �������� ���������� (7 372 800 ��)
TIMSK |= _BV(OCIE2);          // ��������� ���������� �� �������/���������
}




/**************************************************************************************
* ������� ��������� ��������� ������� ���������� ������ INT0
***************************************************************************************/
void init_int0(void){
DDRD &=~(1<<2); 				//����������� ����� INT0 ��� ����
MCUCR |=_BV(ISC01);				//���������� ����� �������������� 
MCUCR &=~_BV(ISC00);			//�� ����� ��������
GICR |=_BV(INT0); 				//��������� ������� ���������� �� INT0

}


void set_buffer_bit(uint8_t index, bool value){	//������ �������� ���� � ������ ��-���������
uint8_t byte_index;
uint8_t bit_index;
byte_index = index/8; //����������, � ����� ����� ��������� ������ ���
bit_index = index - (byte_index*8);//���������� ����� ���� � �����
if(value) 
		{
			rx_buffer[byte_index] |= (1<<(7-bit_index));
		}
else	{
			rx_buffer[byte_index] &= ~(1<<(7-bit_index));
		}
}






/*

inline trx_packet get_packet_value(){ //��������� ������ �� ����������� ������
trx_packet result;
uint8_t byte_tmp;

result.player_id = rx_buffer[0];
byte_tmp = rx_buffer[1];
byte_tmp = byte_tmp << 2; //����������� �� ��� ����� �������
byte_tmp = byte_tmp >> 4;
result.damage = pgm_read_byte(&(damage_value[byte_tmp]));
result.team_id = rx_buffer[1]>>6;

return result;
}
*/




/**************************************************************************************
* ������ ���������� "�������"
* ������������ ������ �� ������� ������ ����� ������ data_packet.data[0]
* � ��������� �������� ������
* ������� ���������� ����������  ������ ����� �������� ���� ������ 
***************************************************************************************/


void send_ir_package(void){ //���������� ����� ("��������")
cursor_position = 0; 		//������ - �� ������ ����� ������
ir_transmitter_on = true;	//��������� ��������
//while (ir_transmitter_on);	//����, ���� ����� �����������
}

/**************************************************************************************
* ��������� �������������� ������
* � �������� ��������� ������� ����������� ����������������� ����� ������ (�� 1 �� 127)
* � ���������� ���������� ������� � ���������� ���������� data_packet.player_id
* ����� ��������������� ������� ������������  data_packet.player_id.(bit_0 ... bit_7) 
***************************************************************************************/
void set_player_id(uint8_t ID){

uint8_t *p_id_bit; 							//��������� �� ���� ��������� player_id
p_id_bit = &data_packet.packet.player_id.bit_6; 	//��������� �� 6 "���" ��������� 
for (int i=0; i < 7; i++) { 				//���� ������ �������� 7 ������� ��� ID
ID = ID << 1; 								//�������� ����� �� ���� ���
if (ID&(1<<7)) 								//���� ������� ��� = 1
	{
		*p_id_bit++ = IR_ONE; 				//����������� ��������������� ��������  data_packet.player_id.bit_x
	}
else 
	{
		*p_id_bit++ = IR_ZERO; 
	}

}

data_packet.packet.player_id.bit_7 = IR_ZERO; //�������� ���������, ���� "���" ������ ���� ����� 0 

}



/**************************************************************************************
* ��������� �������������� (�����) �������
* � �������� ��������� ������� ����������� ����������������� ����� (����) ������� (�� 0 �� 3)
* � ���������� ���������� ������� � ���������� ���������� data_packet.team_id
* ����� ��������������� ������� ������������  data_packet.team_id.(bit_0 � bit_1) 
***************************************************************************************/



void set_team_color(tteam_color  color){
switch(color){

		case Red : { //�� ��������� 00 = Red
						data_packet.packet.team_id.bit_0 = IR_ZERO;
						data_packet.packet.team_id.bit_1 = IR_ZERO;
						break;	
					}
		case Blue: { //�� ��������� 01 = Blue
						data_packet.packet.team_id.bit_0 = IR_ONE;
						data_packet.packet.team_id.bit_1 = IR_ZERO;
						break;	
					}
		case Yellow: { //�� ��������� 10 = Yellow
						data_packet.packet.team_id.bit_0 = IR_ZERO;
						data_packet.packet.team_id.bit_1 = IR_ONE;
						break;	
					}
		case Green: { //�� ��������� 11 = Green
						data_packet.packet.team_id.bit_0 = IR_ONE;
						data_packet.packet.team_id.bit_1 = IR_ONE;
						break;	
					}


			}




}




/**************************************************************************************
* ��������� ��������� ��������� ������ ������ (��������� ����)
* � �������� ��������� ������� ����������� ��������� ����
* � ���������� ���������� ������� � ���������� ���������� data_packet.damage
* ����� ��������������� ������� ������������  data_packet.damage.(bit_0 � bit_3) 
***************************************************************************************/


void set_gun_damage(tgun_damage damage){

switch(damage){
		case Damage_1:{  //�� ��������� 0000 = 1
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_2:{  //�� ��������� 0001 = 2
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}

		case Damage_4:{  //�� ��������� 0010 = 4
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}

		case Damage_5:{  //�� ��������� 0011 = 5
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_7:{  //�� ��������� 0100 = 7
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_10:{  //�� ��������� 0101 = 10
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_15:{  //�� ��������� 0110 = 15
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_17:{  //�� ��������� 0111 = 17
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ZERO;
						break;
						}
		case Damage_20:{  //�� ��������� 1000 = 20
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}

		case Damage_25:{  //�� ��������� 1001 = 25
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}

		case Damage_30:{  //�� ��������� 1010 = 30
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}
		case Damage_35:{  //�� ��������� 1011 = 35
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ZERO;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}

		case Damage_40:{  //�� ��������� 1100 = 40
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}

		case Damage_50:{  //�� ��������� 1101 = 50
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ZERO;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}
		case Damage_75:{  //�� ��������� 1110 = 75
						data_packet.packet.damage.bit_0 = IR_ZERO;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}

		case Damage_100:{  //�� ��������� 1111 = 100
						data_packet.packet.damage.bit_0 = IR_ONE;
						data_packet.packet.damage.bit_1 = IR_ONE;
						data_packet.packet.damage.bit_2 = IR_ONE;
						data_packet.packet.damage.bit_3 = IR_ONE;
						break;
						}



			}



}




void init_var(void){            //����� �������� ����������
last_simple = 0xFFFF; //����� ����� ���� �������� ��� ������
ir_transmitter_on=false; 	//�������� ���� �������� ������ (��������� ������ ��� �� �������������)
set_player_id(1);	//������������� ������������� ������
set_team_color(team_id());	//������������� ������������� (����) �������
set_gun_damage(gun_damage());		//������������� ��������� ������ (����)
data_packet.packet.header = IR_START;		//�������������  ��������� (����� ����) ����������� ������������
data_packet.packet.end_of_data = 0;		//0 - ��� �������� �����������, ��� ������ ��� �������� ������ ���
cursor_position = 0; //������ - �� ������ ����� ������
start_bit_received = false;//����� ��� ��� �� ������
bit_in_rx_buff = 0;//����� ��������� ����
rx_event = NOT_EVENT;//���������� ������� ���������
//reset_clock(); //�������� ����
life = 8;//�������� -100% 
key_pressing_duration.key_1    =0;//�������� �������� 
						  //������������
						  //������������ ������� ������
key_pressing_duration.key_1_inc=1;//��������� ������ ������������
bullets = bullets_limit();

}




bool get_buffer_bit(uint8_t index){		//��������� �������� ���� � ������ ��-���������
uint8_t byte_index;
uint8_t bit_index;
byte_index = index/8; //����������, � ����� ����� ��������� ������ ���
bit_index = index - (byte_index*8);//���������� ����� ���� � �����
if(rx_buffer[byte_index]&(1<<(7-bit_index))) return true;
else return false;


}


inline trx_packet get_packet_value(){ //��������� ������ �� ����������� ������
trx_packet result;
uint8_t byte_tmp;

result.player_id = rx_buffer[0];
byte_tmp = rx_buffer[1];
byte_tmp = byte_tmp << 2; //����������� �� ��� ����� �������
byte_tmp = byte_tmp >> 4;
result.damage = pgm_read_byte(&(damage_value[byte_tmp]));
result.team_id = rx_buffer[1]>>6;

return result;
}


tteam_color team_id()//���������� ���� ����� ������� 
{


tteam_color result;
	switch (SW_TEAM_IN&SW_TEAM_MASK) //�������� ��������� ������������� "DAMAGE"
	{
		case SW_TEAM_KEY1_PIN: //1-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = Blue;
			//return result;
			break;
		}
		case SW_TEAM_KEY2_PIN://2-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = Yellow;
			//return result;
			break;
		}
		
		case SW_TEAM_KEY1_PIN|SW_TEAM_KEY2_PIN: //��� ����� � ��������� OFF
		{
			result = Red;
			//return result;
			break;
		}

		case 0: //��� ����� � ��������� ON
		{
			result = Green;
			//return result;
			break;
		}
		default: result = Red;

	}

return result;
}


/**************************************************************************************
* ������� ��������� ��������� ������� timer0
* ����� ������ ������ - ��� (����� ��� ����������)
* ������������ - � ������������� �� 1024
* ������ ��������� ����� ����� ����� ��������, ����� ����������
* �������������� 100 ���������� � ������� 
***************************************************************************************/

void init_timer0(void){

//OCR0 = F_CPU/1024/100-1;		// ���������� ������ �������������� � �������� 100 ��

OCR0 = 128; //���������� = 0,5
TCCR0 = _BV(WGM01)| _BV(WGM00);	// ����� ������ ������ - fast PWM (������� ���)
TCCR0 |=  _BV(CS00);            // ������������ � �������� ���������� 8 ���
TCCR0 |=  _BV(COM01);    		//����������������� ����� ���






//TIMSK |= _BV(OCIE0);          // ��������� ���������� �� �������/���������
		                      // ��������� ���������� ���������

}


tgun_damage gun_damage()//���������� ������� ����, ��������� ����� �����
{
tgun_damage result;
	switch (SW_DAMAGE_IN&SW_DAMAGE_MASK) //�������� ��������� ������������� "DAMAGE"
	{
		case SW_DAMAGE_KEY1_PIN: //1-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = Damage_50;
			//return result;
			break;
		}
		case SW_DAMAGE_KEY2_PIN://2-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = Damage_25;
			//return result;
			break;
		}
		
		case SW_DAMAGE_KEY1_PIN|SW_DAMAGE_KEY2_PIN: //��� ����� � ��������� OFF
		{
			result = Damage_10;
			//return result;
			break;
		}

		case 0: //��� ����� � ��������� ON
		{
			result = Damage_100;
			//return result;
			break;
		}
		default: result = Damage_25;

	}

return result;

}



void init_timer1(void){ //����������� timer1 �� ������� ������� ����� -8 
TCCR1A &=~_BV(WGM10); //����� ������ ������� - CTC (����� ��� ����������)
TCCR1A &=~_BV(WGM11);
TCCR1B |=_BV(WGM12); 
TCCR1B &=~_BV(WGM13); 
TCCR1A &=~_BV(COM1A0);//��������� ������ �� ������ OC1A
TCCR1A &=~_BV(COM1A1);
TCCR1B &=~_BV(COM1B0);//��������� ������ �� ������ OC1B
TCCR1B &=~_BV(COM1B1);

TCCR1B &=~_BV(CS10); //�������� = 8
TCCR1B |=_BV(CS11);
TCCR1B &=~_BV(CS12); 
//OCR1AL=60;
//OCR1AL=124;
OCR1AL=(F_CPU/8000)/8-1; // ����������� �� ������� ������� ����� - 8 ���
//OCR1AL=248; 
//OCR1AH=0x27;
//OCR1AL=0x0F;


TIMSK |= _BV(OCIE1A);  
//TIMSK |= _BV(OCIE1B);  

}



void display_life(uint8_t life_value) //���������� ������� ����� �� ������������ �������
{

uint8_t integer_part;
	for (int i=0; i<4; i++)
	{life_leds_status[i] = OFF;}

integer_part = life_value/2;
for (int i=0; i<integer_part; i++)
{
	life_leds_status[i] = ON;
}


if ((life_value-integer_part*2)>0) 
 life_leds_status[integer_part] = FQR_2HZ;
 
}


uint8_t bullets_limit(void)//���������� ����� ��������
{

uint16_t result;
	switch (SW_BULLETS_LIMIT_IN&SW_BULLETS_LIMIT_MASK) //�������� ��������� ������������� "BULLETS_LIMIT"
	{
		case SW_BULLETS_LIMIT_KEY1_PIN: //1-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = 64;
			//return result;
			break;
		}
		case SW_BULLETS_LIMIT_KEY2_PIN://2-� ���� � ��������� OFF (���������), � ������ ������� (ON)
		{
			result = 32;
			//return result;
			break;
		}
		
		case SW_BULLETS_LIMIT_KEY1_PIN|SW_BULLETS_LIMIT_KEY2_PIN: //��� ����� � ��������� OFF
		{
			result = 16;
			//return result;
			break;
		}

		case 0: //��� ����� � ��������� ON
		{
			result = 128;
			//return result;
			break;
		}
		default: result = 16;

	}

return result;


}



TFIRE_MODE_STATUS fire_mode()//���������� ������� ����� ���� (���������/���������)
{
TFIRE_MODE_STATUS result;
if (FIRE_MODE_KEY_IN&FIRE_MODE_KEY_PIN) result = single;
else  result = queues;
return result;

}

