#include "ltag_ascetic.h"
#include "milshot_snd.h"
/**************************************************************************************
* ���������� ���������� �������
***************************************************************************************/

ISR(TIMER2_COMP_vect){
timer1++;
volatile uint8_t prt;


prt = TSOP_IN&TSOP_PIN; 
if (prt==0) //�� ����� ��-��������� ������ ������� ������� (������ ������ �������)
	{
	//	PORTA &= ~(1 << 0); //�������� ��������������� ���������
		low_level_counter++;//����������� ������� ������������ ������������� ������� �� ����� ��-���������
	}
else  //�� ����� ��-��������� ������� ������� ������� (������ ������ �������)
	{
	//	PORTA |=(1<<0);	//��������� ��������������� ���������
		high_level_counter++;///����������� ������� ������������ �������������� ������� �� ����� ��-���������
		if((start_bit_received)&&(high_level_counter > IR_ZERO*8))
		{//��������� ��������� ������ �� ��������
			start_bit_received	= false; 	//�������� �����
			rx_event = RX_COMPLETE;			//������� ������� "������ �����"		
		}
	
	
	}
if (ir_transmitter_on==true)
	{//���� �������� ���������

		if (ir_pulse_counter > 0)		//����������� ������������ ����� ��������� 
		{								//��� �� ����������, "������" ������
			IR_LED_INVERT;  			//����������� ������������ ����� 
			ir_pulse_counter--;
		}
		else							//����� ��������� ���� ����������, 
		{
			IR_LED_OFF;			//����� ��-���� 
			if ( ir_space_counter > 0)	//��������, �������� �� ���������� ����� ����������
			{							//���, ���������� �� ��������
			
					IR_LED_OFF;	//����� ��-����
					ir_space_counter--;	//��������� �������� ������� ����� �� ���� "���"		
			}
			else //����� ��������� � ���������� ����� ������ ��������
			{	 //����� ����������� ��������� ����� (������������ ���)
				
				
				if (data_packet.data[cursor_position]!=0) //���� ��������� ��������� �� �� ������ ������
				{
					ir_pulse_counter =data_packet.data[cursor_position++] ; //��������� ������� ��������� �������������
					ir_space_counter = IR_SPACE;      //� ��� ����� �� �������
				}
				else //��� ������ �������� (�������, �� ������� ��������� ���������, ����� 0)
				{
					ir_transmitter_on=false; //��������� ����������
				}	
				
				
				
				
						
			}				 
		}




	}
else 	{//���� �������� ���������

		}


}



/**************************************************************************************
* ���������� ������� ���������� ������ INT0
***************************************************************************************/


ISR(INT0_vect){
//TIMSK |= _BV(OCIE2);          // ��������� ���������� �� �������/���������

if(!(MCUCR&_BV(ISC00)))		 //���� ���������� ������� ������ 
	{
		MCUCR |=_BV(ISC00); //��������� ���������� ����� ������������� �������
		if (start_bit_received)//���� �����-��� ������, �� ���� ����� ������
			{
				if((high_level_counter < (IR_SPACE + ERROR_TOLERANCE))&&(high_level_counter > (IR_SPACE - ERROR_TOLERANCE)))//�������� ������������ ����� ����� ������
				{
					//������������ ����� ����� ���������� ���������
				}
				else //������������ ����� ����� ������� ����� �� ���������
				{//��������� ������ �����
						start_bit_received	= false; 	//�������� �����
						bit_in_rx_buff = 0;				//������� �����
						rx_event = RX_ERROR;			//���������� ������� - "������ �����"
//						TIMSK &= ~_BV(OCIE2);          // ��������� ���������� �� �������/���������
				}
			
			
			}		
		low_level_counter = 0;//�������� ������� ������������ ������������� ������� �� ����� ��-��������
		high_level_counter = 0;//�������� ������� ������������ �������������� ������� �� ����� ��-��������
	}
else 						//���������� ������� ������� 
	{
		MCUCR &=~_BV(ISC00); //��������� ���������� ����� ������������� ������
		
		if (start_bit_received)//���� �����-��� ������, �� ���� ����� ������
			{
				if((low_level_counter < (IR_ZERO + ERROR_TOLERANCE))&&(low_level_counter > (IR_ZERO - ERROR_TOLERANCE)))//��������, ������������� �� ������������ ������ �������� ����
				{
					set_buffer_bit(bit_in_rx_buff++, false);//������������ ����� ������������� ���� �� ��������� 0, ������� ���� � ����� ������
				
				}
				else //���, ��� �� ��� �� ��������� 0
				{
					if((low_level_counter < (IR_ONE + ERROR_TOLERANCE))&&(low_level_counter > (IR_ONE - ERROR_TOLERANCE)))//, ����� ��� ��� �� ��������� 1?
					{
							set_buffer_bit(bit_in_rx_buff++, true);//������������ ����� ������������� ���� �� ��������� 1, ������� ������� � ����� ������		
					}
					else //��� �� �������, �� ���� - ��� ������ 
					{
						start_bit_received	= false; 	//�������� �����
						bit_in_rx_buff = 0;				//������� �����
						rx_event = RX_ERROR;			//���������� ������� - "������ �����"
//						TIMSK &= ~_BV(OCIE2);          // ��������� ���������� �� �������/���������
					}
				}
			}
		else //�����-��� ��� �� ������
		{
			if ((low_level_counter < (IR_START + ERROR_TOLERANCE))&&(low_level_counter > (IR_START - ERROR_TOLERANCE))) //����� ��� �����-���?	
			{//��� �����-���
				bit_in_rx_buff = 0;				//������� �����
				start_bit_received	= true; 	//��������� ����� ������� (���)
				
			}
			else //��� �� �����-���, ��� ������ 
			{
				//����������
			}
		}
		
		
		low_level_counter = 0;//�������� ������� ������������ ������������� ������� �� ����� ��-��������
		high_level_counter = 0;//�������� ������� ������������ �������������� ������� �� ����� ��-��������
	
	


	}

}





/**************************************************************************************
* ���������� ������� ���������� timer1A
***************************************************************************************/

ISR(TIMER1_COMPA_vect){
TIMSK &= ~_BV(OCIE1A);  //��������� ���������� timer1, ����� �� ���� ��������
sei(); 

if (bullets >0)//���� ������� �� ���������
{
	if (last_simple == 0) 
	{
		fire_led_status=ON; //�������� �������
	//	if (fire_mode()==queues) fire_led_status=FQR_4HZ;
	//	else fire_led_status=ON; //�������� �������
	}
	else;
	if (last_simple < sizeof(pSnd))//3913
			{
				if (last_simple==(sizeof(pSnd)/100)*CUT_OFF_SOUNT)
				{
					if (fire_mode()==queues)
					{
						if ((get_keyboard_status()==key_pressed)&&(life>0)) //����� �����, �� �������� ����
						{
							bullets--;//��������� �� 1 ���������� ��������
							send_ir_package();	//���������� "�������"
							last_simple=0;		//������������� ���� �������
							fire_led_status=FQR_4HZ;
						}
						else 	fire_led_status=ON;						
					}
					else;
				}
				else;
				
				OCR0 = pgm_read_byte(&(pSnd[last_simple++]));
			}
	if (last_simple >= sizeof(pSnd)&&(last_simple)!=0xFFFF)//3913
			{
			//	last_simple = 0;
			//	PORTA &= ~(1 << 2);
				OCR0 = 128; //���������� = 0,5
				fire_led_status=OFF;
				//FIRE_LED_OFF;
			};


}


if (bullets <= 0) //������� ���������
	{
		BULLETS_OUT_LED_ON; // �������� ��������� "������� ���������"
		if (last_simple < sizeof(pSnd)) {OCR0 = pgm_read_byte(&(pSnd[last_simple++]));}//����� �������� ���������� �� �����
		else fire_led_status = OFF;
	};






static volatile uint16_t tmp_cntr=0;

	if ((tmp_cntr - (tmp_cntr/100)*100)==0)
	{
		switch(keyboard_event)
			{
		  	case no_key_pressing: 
		  		{
					keyboard_event=test_keyboard(); 
					break;
				}
		  	default:;         
			}	

	}



//cli();

if (++tmp_cntr > 1000) //���� �������� ���������
	{
		  
		
		
		
		//test_keyboard();
		//LIFE_LED1_INVERT;
		tmp_cntr = 0;
		static volatile uint8_t bit_mask = 0b00000001;
	
		if ((life_leds_status[0]&bit_mask)==0) 
			{
				LIFE_LED1_OFF;
			}
		else 
			{
				LIFE_LED1_ON;
			};
		if ((life_leds_status[1]&bit_mask)==0) 
			{
				LIFE_LED2_OFF;
			}
		else 
			{
				LIFE_LED2_ON;
			};
		if ((life_leds_status[2]&bit_mask)==0) 
			{
				LIFE_LED3_OFF;
			}
		else 
			{
				LIFE_LED3_ON;
			};

		if ((life_leds_status[3]&bit_mask)==0) 
			{
				LIFE_LED4_OFF;
			}
		else 
			{
				LIFE_LED4_ON;
			};
		if ((fire_led_status&bit_mask)==0)
			{
				FIRE_LED_OFF;
			}
		else
			{
				FIRE_LED_ON;
			};


		bit_mask = (bit_mask<<1);
		if (bit_mask == 0)  bit_mask = 0b00000001;

	
	}


cli();

TIMSK |= _BV(OCIE1A);  //������ ����� ��������� ���������� timer1


}





inline  TKEYBOARD_STATUS get_keyboard_status(void) {

volatile	TKEYBOARD_STATUS s_ret;
	
	switch (FIRE_KEY_IN&FIRE_KEY_PIN) //���������, ����� �� �����
		{
			case FIRE_KEY_PIN: s_ret=no_key_pressed  ; break;
			default: s_ret=key_pressed ;	
		}



		return s_ret;
}









inline  TKEYBOARD_EVENT test_keyboard(void){
	TKEYBOARD_STATUS key_status;
	TKEYBOARD_EVENT ret_ev;
	key_status=get_keyboard_status();
	switch(key_status)  //���������, ��� ������
	{
		case no_key_pressed: 
		{
			if (key_pressing_duration.key_1>= SHORT_DURATION)
			{
				ret_ev=key_pressing;
                key_pressing_duration.key_1    =0;
                key_pressing_duration.key_1_inc=0;
				return ret_ev;
			
			}

			else 
			{
				ret_ev=no_key_pressing;
                key_pressing_duration.key_1    =0;
                key_pressing_duration.key_1_inc=1;
			
			}
		
		} 
		break;
	 	case key_pressed  : //������ ������ 1
		{ 
			if(key_pressing_duration.key_1>= SHORT_DURATION)
			{
				ret_ev=key_pressing;
             	key_pressing_duration.key_1    =0;
                key_pressing_duration.key_1_inc=0;
			} 
			else 
			{
				key_pressing_duration.key_1 += key_pressing_duration.key_1_inc; 
                ret_ev=keyboard_event;

			}
		}
		break;
		default: ret_ev=keyboard_event;
	
	}

return   ret_ev;
}






