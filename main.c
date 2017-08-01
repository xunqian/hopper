//ICC-AVR application builder : 2017-7-05 10:19:15
// Target : M64
// Crystal: 16Mhz

#include <iom64v.h>
#include <macros.h>
#include "main.h"
#include "command.h"

uchar sensor_state_outcoin1;
uchar sensor_state_outcoin2;
uchar pre_sensor_state_outcoin1;
uchar pre_sensor_state_outcoin2;
uchar pre_sensor_state3;
uchar pre_sensor_state4;
uchar sensor_state3;
uchar sensor_state4;
uchar direction;
uchar number;

uchar time1=0;
uint out_time = 0;
uint sole_time;
uchar out_coin=0;

uint Uart_time;
uchar Uart_time_start=0;

uint CLR_Number = 0;
uchar m_RecSize = 0;

uchar Em,CEm;
uchar sensor_change = 0;
uchar sensor_change_CEm = 0;
uchar empty_change = 1;
uchar closeempty_change = 1;
uchar out_coin_error=0;
uchar clr_coin_error=0;

const char HOPPER_version[16] = "HOPPER_A1.0_V0.0" ;
uchar m_RecBuffer[20];
uchar buffer[20];
uchar state[10];
uchar state_1[10];
uchar check[10];
uchar state_check[10];
uchar table[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09,0x11,0xc1,0x63,0x85,0x61,0x71};
//uchar error[16];

uchar BUSY = 0;
uchar IO_init_OK;
uchar get_over;
//���⴫�����������������ƣ���߷����ǿ�ȣ������ͨ��ʱ�䲻����10us//
uint sensor_on_time = 0;
uint EMPTY_time = 0;

uchar message;   //��������λ����Ϣ����
uchar command = 0;
uchar undo = 0;   //0Ϊ����ģʽ��1Ϊά��ģʽ
uchar DIG_TOKEN = 0;//��Ʊ�ɹ�Ϊ0��ʧ��Ϊ1����ʼ��Ϊ�ɹ�
uchar DIG_TOKEN_num = 0;//������Ʊʧ�ܴ���
/////////////״̬����////////////////////////////////////////////////////

void delay(uint stime)
{
    uint i=0,j=0;
    for(i=0;i<stime;i++)
    {
	   delay_ms();
	}
}

/*1ms��ʱ����*/
void delay_ms(void) 
{  
   uint i;   
   for(i=1228; i>0; i--); 
}

/*1ms��ʱ����*/
/*void delay_ms(void) 
{  
   uint i;   
   for(i=1;i< (uint)(8*143-2); i++); 
}*/

void port_init(void)
{
 PORTA = 0xE0;  //��������0x1F  ��0x00
 DDRA  = 0x60;
 PORTB = 0x00;
 DDRB  = 0x03;
 PORTC = 0x00; //m103 output only
 DDRC  = 0xFF;
 PORTD = 0x00;
 DDRD  = 0x00;
 PORTE = 0x04;
 DDRE  = 0x04;
 PORTF = 0x00;
 DDRF  = 0x0F;
 PORTG = 0x00;
 DDRG  = 0x00;
}

//TIMER1 initialize - prescale:1
// WGM: 0) Normal, TOP=0xFFFF
// desired value: 2mSec
// actual value:  2.000mSec (0.0%)
void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0x83; //setup
 TCNT1L = 0x00;
 OCR1AH = 0x7D;
 OCR1AL = 0x00;
 OCR1BH = 0x7D;
 OCR1BL = 0x00;
 OCR1CH = 0x7D;
 OCR1CL = 0x00;
 ICR1H  = 0x7D;
 ICR1L  = 0x00;
 TCCR1A = 0x00;
 TCCR1B = 0x01; //start Timer
}

#pragma interrupt_handler timer1_ovf_isr:iv_TIM1_OVF
void timer1_ovf_isr(void)
{
	uchar k;
	TCNT1H = 0x83; //reload counter high value
	TCNT1L = 0x00; //reload counter low value
	if(out_time!=0)
	{
		out_time--; 
	}
	if(sole_time!=0)
	{
		sole_time--;
	}
	if(Uart_time!=0)
	{
		Uart_time--;
	}
	if(motor_over)
	{
	 time1++;
	}
	else
	{
	 time1=0;
	}
	if(Uart_time_start==1)
	{
		Uart_time--;
	}
	if(Uart_time==0)
	{
		for(k=1;k<=m_RecSize;k++)
		{
			m_RecBuffer[k] = 0;
		}
		m_RecSize = 0;
		Uart_time_start=0;
	}
}

//UART0 initialize
// desired baud rate: 19200
// actual: baud rate:19231 (0.2%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
 //16M
 
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x33; //set baud rate lo
 UBRR0H = 0x00; //set baud rate hi
 //UBRR0L = 0x08; //set baud rate lo  ������115200
 //UBRR0H = 0x00; //set baud rate hi
 UCSR0B = 0x98;
 
 //8M
 /*
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x19; //set baud rate lo
 UBRR0H = 0x00; //set baud rate hi
 UCSR0B = 0x98;*/
}

///////////////////���ڷ���һ���ֽ�/////////////////////////////////////////////
void USART0_Transmit(uchar c)//���ݷ���
{
    while( !( UCSR0A & (1<<UDRE0)) ) ; //�ȴ����ͻ�����Ϊ��
    UDR0 = c;  // ��������    //character has been transmitte
}

#pragma interrupt_handler uart0_rx_isr:iv_USART0_RXC
void uart0_rx_isr(void)
{
 //uart has received a character in UDR0
    uchar i,j,k,z,y;
    uchar len;
	uchar number_7F = 0;
	//uchar temp = 0;
	i = UDR0;
	Uart_time_start=0;
	m_RecSize++;
	m_RecBuffer[m_RecSize] = i; 
    if(m_RecBuffer[1] == 0x7F)
	{
		Uart_time=25;
	    Uart_time_start=1;
		if(m_RecSize>=19)
		{
			for(k=1;k<=19;k++)
		    {
		       m_RecBuffer[k] = 0;
		    }
			m_RecSize=0;
		}
		if((m_RecSize>=2) && (m_RecBuffer[m_RecSize] == 0x80))
		{
			 for(z=m_RecSize-1;z>0;z--)
			 {
				if(m_RecBuffer[z]== 0x7F)
				{
				   number_7F++;
				}
				else
				{
				   break;
				}
			 } 
		     if((number_7F & 0x01)==0)//ż��
		     {
			    get_over = 1;
			    Uart_time_start=0;
			    for(j=2,y=2;j<=m_RecSize,y<=m_RecSize;j++,y++)
		        {
		           if(m_RecBuffer[y] == 0x7F)
				   {
				      y++;
				   }
				   buffer[j] = m_RecBuffer[y];
		        }
			    for(k=1;k<=19;k++)
			    {
			       m_RecBuffer[k] = 0;
			    }
                m_RecSize = 0;
				number_7F=0;
                return;
		     }
       }
	//m_RecSize = 0;
	}
	else
	{
		for(k=1;k<=19;k++)
	    {
	       m_RecBuffer[k] = 0;
	    }
		m_RecSize = 0;
	}
	if(m_RecSize>=19)
	{
		for(k=1;k<=19;k++)
	    {
	       m_RecBuffer[k] = 0;
	    }
		m_RecSize=0;
	}
}

//Watchdog initialize
// prescale: 2048K
void watchdog_init(void)
{
 WDR(); //this prevents a timout on enabling
 WDTCR = 0x1F; 
 WDTCR = 0x0F; //WATCHDOG ENABLED - dont forget to issue WDRs
}


//call this routine to initialize all peripherals
void init_devices(void)
{
 //stop errant interrupts until set up
 CLI(); //disable all interrupts
 XDIV  = 0x00; //xtal divider
 XMCRA = 0x00; //external memory
 port_init();
 //timer0_init();
 timer1_init();
 uart0_init();
 //watchdog_init();

 MCUCR = 0x00;
 EICRA = 0x00; //extended ext ints
 EICRB = 0x00; //extended ext ints
 EIMSK = 0x00;
 TIMSK = 0x04; //timer interrupt sources
 ETIMSK = 0x00; //extended timer interrupt sources
 SEI(); //re-enable interrupts
 //all peripherals are now initialized
}

void calculateLRC(uchar*buffer, int length,uchar lrc)
{
	uchar result = 0;
	int count = 0;
	for(count=2; count<(length+6); count++)
		{
			result += buffer[count];
		}
	lrc = result;
}

//����ָ���LRCУ��
uchar CheckLRC(void)
{
   uchar i;
   uchar result,length;
   result = 0;
   length = 0;
   length = buffer[5];
   
   
   for(i=2;i<length+6;i++)
   {
     result += buffer[i];
   }
   if(result == buffer[length+6])
   {
     return 1;  
   }
   else
   {
     return 0;
   }
   
}


/*��ת����*/
void motor_forword()
{
   motor_en1;
   motor_en2;
}

/*���ֹͣ*/
void motor_stop()
{
   motor_uen1;
   motor_en2;
}

/*���Ϻ���*/
void Sole_ON()
{  
   sole_en1;
   //sole_en2;
}

/*������ͷ�*/
void Sole_OFF()
{  
   OFFPF(2);
 }
/*��ת����*/
void motor_backword()
{   
   motor_uen1;
   motor_uen2;   
}

void INIT_IO(void)
{
	uchar i;	
	motor_en_ON;
	motor_backword();
	WDR();
	delay(1000);
	motor_stop();    
	WDR();
    delay(800);
	Sole_ON();
	WDR();
	delay(800);
	Sole_OFF();    
	WDR();
	delay(500);
}

void HD_SELF(void)
{    
	motor_en_ON;
	out_coin_sensors_check(BACKWARD,200,0);
	WDR();
	motor_stop();    
    delay(200);
	Sole_ON();
	delay(1000);
	WDR();
	Sole_OFF();   
	delay(200);	
	WDR();
	POWER_LED_ON;
	delay(1000);
	POWER_LED_OFF;
	delay(300);
	POWER_LED_ON;
	delay(1000);
}

void CHG(void)
{
   uchar i;   
   POWER_LED_ON;   
   out_coin_check();   
}

char sole_check(uint over_time)
{
   uchar sole_senror;
   uchar pre_sole_senror;
   sole_time=over_time;
   pre_sole_senror=GetPA(1);
   if(!pre_sole_senror)
   {
      return 0;
   }
   sole_en_ON;
   Sole_ON();
   while(sole_time)
   {
      WDR();
	  sole_senror = GetPA(1);
	  if(sole_senror == 0)
	  {
		 break;
	  }
	}
	if(sole_time==0)
	{
	     return 0;
	}
    delay(300);
	return 1;
}

void CLR(void)
{
   uchar i=0;
   uchar ret;
   POWER_LED_ON;
   clr_coin_error=0;   
   if(sole_check(1000))
   {   
	  for(i=0;i<5;i++)
      {
             if(GetPA(1)==1)
		     {
			    clr_coin_error=1;
				break;
			 }
			 WDR();
	         ret=out_coin_sensors_check(FOREWARD,400+i*100,0);
			 if(ret == 1)
	         {
	            i=0;		        
		        delay(1);
		  	    CLR_Number++;		        
	         }
	         else
	         {
	            ret=out_coin_sensors_check(BACKWARD,300+i*100,0);
				if(ret == 1)
	         	{
	            	i=0;
					delay(1);
		  	      	CLR_Number++;
	         	}
	         }
	         if(buffer[6] == 0x62)
             {
	            break;
             }
       }
   }
   Sole_OFF();
   delay(100);
}

void read_sensorstatus(void)
{
	pre_sensor_state_outcoin1=sensor_state_outcoin1;
    pre_sensor_state_outcoin2=sensor_state_outcoin2;
	pre_sensor_state3=sensor_state3 ;
	pre_sensor_state4=sensor_state4 ;
    sensor_state_outcoin1 = GetPA(4);
    sensor_state_outcoin2 = GetPA(3);
	sensor_state3 = GetPA(2);
    sensor_state4 = GetPA(0);   
}

uchar out_coin_sensors_check(uchar direction,uint over_time,uchar empty_check)
{
   uchar ret = 0;
   out_time = over_time;
   read_sensorstatus();
   if(direction==FOREWARD)
   {
      WDR();
      motor_en_ON;
      motor_forword();
	  while(out_time)
      {
         WDR();
		 read_sensorstatus();
		 //���ҿڴ��������߼�
		 if((sensor_state_outcoin1 == 1) && (pre_sensor_state_outcoin1 == 0))		
		 {
		    WDR();
			ret = 1;
			motor_stop();			
			out_coin=1;
			break;
		 }
		 if(time1==M_SECOND)
		 {
		    ret = 0;
			motor_stop();
			break;
		 }		 
      }	
	  if(out_time==0)//�ڱ�ʧ��
	  {
	  	DIG_TOKEN = 1;
		DIG_TOKEN_num++;
	  }
	  else
	  {
		DIG_TOKEN = 0;
		DIG_TOKEN_num = 0;
	  }
   }		
   else if(direction == BACKWARD)
   {
	     WDR();
         motor_en_ON;
         motor_backword();
	     while(out_time)
         {
            WDR();
		    read_sensorstatus();
		    //���ҿڴ��������߼�
		    if((sensor_state_outcoin1 == 0) && (pre_sensor_state_outcoin1 == 1))	   
		    {
		       WDR();
			   ret=1;			   
			   out_coin=1;
		    }			
			if((sensor_state3 != pre_sensor_state3)||(GetPA(2) == 1))
            {
	           sensor_change = 1;
			   if(empty_check)
			   {
			      motor_stop();
				  break;
			   }
            }
	        if((sensor_state4 != pre_sensor_state4)||(GetPA(0) == 1))
            {
		       sensor_change_CEm = 1;
			   if(empty_check)
			   {
			      motor_stop();
				  break;
			   }
            }
		 }
		if(out_time == 0)
		{
			motor_stop();
			if(GetPA(2) == 0)
			{
				empty_change = 1;
			}
			else
			{
				empty_change = 0;
			}  
			if(GetPA(0) == 0)
			{
				closeempty_change = 1;
			}
			else
			{
				closeempty_change = 0;
			}		
		}
		if(sensor_change == 1)
        {
           sensor_change = 0;
           empty_change = 0;
        }
        if(sensor_change_CEm == 1)
        {
           sensor_change_CEm = 0;
           closeempty_change = 0; 
        } 
   }
   
   return ret; 
}

void out_coin_check(void)
{
   uchar ret;
   uchar n;
   out_coin_error=0;
   for(n=0;n<5;n++)
   {
	      if((GetPA(1)==1)&&(GetPA(4) == 0))
		  {
		     WDR();
			 if(n >= 2)
			 {
				WDR();
				motor_stop();
				delay(500+n*100);
			 }
		     ret=out_coin_sensors_check(FOREWARD,800+n*100,0);
             if(ret == 1)
             {
			    //out_coin_sensors_check(BACKWARD,50,0);
			    break;
             }
             else
             {
				WDR();
				motor_stop();
				delay(500+n*200);
				ret=out_coin_sensors_check(BACKWARD,500+n*100,0);
			    if(ret == 1)
			    {
			       break;
			    }
				else
				{
					out_coin_error=1;
					break;
				}
				
             }
		   }
		   else
		   {
		      out_coin_error=1;
		   }
    }
	if(((empty_change==1)&&(closeempty_change==1))&&(DIG_TOKEN = 1))
	{
	   out_coin_sensors_check(BACKWARD,500,1);
	}
	IOupDATA(0x04,message);
}

void Check_command(void)
{
   switch(command)
   {
      case CMD_SW_RESET://87
	           message = CMD_SW_RESET; //�����λ
	           IOupACK(0x02,message);
			   IOupDATA(0x02,message);
		       command = 0;
			   delay(4000);
	   break;
	  case CMD_STATE_QUERY://12
	           message = CMD_STATE_QUERY; //״̬��ȡ
	           IOupACK(0x02,message);
			   States_Get(0x0A);
		       command = 0;
	   break;
	   case CMD_STATE_Check://13
	           message = CMD_STATE_Check; //״̬���
	           IOupACK(0x02,message);
			   States_Check(0x0A);
		       command = 0;
	   break;
	  case CMD_HD_SELF://40
	           message = CMD_HD_SELF; //Ӳ���Լ�
	           IOupACK(0x02,message);
			   HD_SELF();
			   IOupDATA(0x02,message);
		       command = 0;
	   break;
	  case CMD_CHG://60
	           message = CMD_CHG; //����
	           IOupACK(0x02,message);			   
			   CHG();			   
		       command = 0;
	   break;
	  case CMD_CLR://61
	           message = CMD_CLR; //���
	           IOupACK(0x02,message);
			   CLR();
			   IOupDATA(0x04,message);
			   CLR_Number= 0;
			   //CLR_Number_H = 0;
		       command = 0;
	   break;
	   case CMD_CLR_STOP://62
	           message = CMD_CLR_STOP; //ֹͣ���
			   IOupACK(0x02,message);			   
			   motor_stop();
	           Sole_OFF();
			   IOupDATA(0x04,message);
		       command = 0;
	   break;
	   case CMD_READ_VERSION: //16              //��ȡģ��汾��Ϣ
	           message = CMD_READ_VERSION;
			   IOupACK(0x02,message);
	           Version_Get(0x12);
		       command = 0;
	   break;
   }
}

void main(void)
{
  init_devices();
  delay(1000);
  WDR();
  INIT_IO();
  POWER_LED_ON;  
  while(1)
  {
	  if(1 == get_over)
	  {
		if(CheckLRC() == 1)
		{
		   command=buffer[6];
		}
		get_over=0;
	  }
	  Check_command();
	  WDR();
   }   
}

/*****************************************************************************/
/**********  IOupACK()                                                       /
/**********  ��ӦPC                                                           /

/*****************************************************************************/
void IOupACK(uchar DLEN,uchar DATA)
{
   uchar temp;
   USART0_Transmit(STX);
   USART0_Transmit(RSEQ);
   temp = RSEQ;
   USART0_Transmit(DESA);
   temp += DESA;
   USART0_Transmit(SRCA);
   temp += SRCA;
   USART0_Transmit(DLEN);
   temp += DLEN;
   USART0_Transmit(ACK_CMD_ACK);
   temp += ACK_CMD_ACK;
   USART0_Transmit(DATA);
   temp += DATA;
   USART0_Transmit(temp);
   USART0_Transmit(ETX);
}
/*****************************************************************************/
/**********  IOupDATA()                                                      /
/**********  ���ݷ���                                                         /

/*****************************************************************************/
void IOupDATA(uchar DLEN,uchar DATE)
{
   uchar temp;
   uchar i;
   uchar ack;
   uchar DATA[20];
   USART0_Transmit(STX);
   USART0_Transmit(RSEQ);
   temp = RSEQ;
   USART0_Transmit(DESA);
   temp += DESA;
   USART0_Transmit(SRCA);
   temp += SRCA;
   USART0_Transmit(DLEN);
   temp += DLEN;
   
   switch(DATE)
   {
      case 0x87:
	          ack=ACK_CMD_INTIME;
	          DATA[1]=CMD_SW_RESET;
	   break;
	  case 0x40:
	          ack=ACK_CMD_INTIME;
	          DATA[1]=CMD_HD_SELF;
	   break;
	  case 0x60:
	          ack=ACK_CMD_UNTIME;
	          DATA[1]=CMD_CHG;
			  if(closeempty_change == 0)
              {
                 empty_change = 0;
              }
			  DATA[2]=(empty_change * 16)+(closeempty_change * 32);
			  if(out_coin)
			  {
			      DATA[3]=0x01;
				  out_coin=0;
			  }
			  else
			  {
			      DATA[3]=0x00;
			  }
	   break;
	  case 0x61:
	          ack=ACK_CMD_UNTIME;
	          DATA[1]=CMD_CLR;
			  DATA[2]=(CLR_Number>>8) & 0xff;
			  DATA[3]=CLR_Number & 0xff;
	   break;
	   case 0x62:
	          ack=ACK_CMD_INTIME;
	          DATA[1]=CMD_CLR_STOP;
	   break;
   }
   USART0_Transmit(ack);
   temp += ack;
   for(i=1;i<DLEN;i++)
   {
	 USART0_Transmit(DATA[i]);
	 if((DATA[2]==0x80)||(DATA[3]==0x80))
	 {
		USART0_Transmit(STX);
	 }
     temp += DATA[i];
   }
   USART0_Transmit(temp);
   USART0_Transmit(ETX);  
}


/*****************************************************************************/
/**********  States_Get()                                                         /
/**********  ״̬��ȡ                                                 /
/*****************************************************************************/
void States_Get(uchar DLEN)
{
   uchar temp;
   uchar outcoin1;
   uchar outcoin2;
   uchar i;
   if(GetPA(4)==1)
   {
      outcoin1=0;
   }
   else
   {
      outcoin1=1;
   }
   
  /* if(GetPA(3)==1)
   {
      outcoin2=0;
   }
   else
   {
      outcoin2=1;
   }*/
   
   state[1] = outcoin1 + outcoin2 * 2 + GetPA(1)*4;;//���ҿڴ�����1�����ҿڴ�����2����մ�����
   state[2] = 0x00;
   state[3] = 0x00;
   state[4] = 0x00;   
   if(closeempty_change == 0)
   {
       empty_change = 0;
   }
   
   state[5] = (BUSY * 2)+(empty_change * 16)+(closeempty_change * 32)+(out_coin_error *64)+(clr_coin_error *128);//æµ���ѿգ����գ����ҹ��ϣ���ҹ���
   state[6] = 0x00;
   state[7] = 0x00;
   state[8] = IO_init_OK*128;
   USART0_Transmit(STX);
   USART0_Transmit(RSEQ);
   temp = RSEQ;
   USART0_Transmit(DESA);
   temp += DESA;
   USART0_Transmit(SRCA);
   temp += SRCA;
   USART0_Transmit(DLEN);
   temp += DLEN;
   USART0_Transmit(ACK_CMD_INTIME);
   temp += ACK_CMD_INTIME;
   USART0_Transmit(CMD_STATE_QUERY);
   temp += CMD_STATE_QUERY;
   for(i=1;i<9;i++)
   {
     if((state[i]==0x80)||(state[i]==0x7F))
	 {
	    USART0_Transmit(STX);
	 }
	 USART0_Transmit(state[i]);
     temp += state[i];
   }
   if((temp==0x80)||(temp==0x7F))
   {
	    USART0_Transmit(STX);
   }
   USART0_Transmit(temp);
   USART0_Transmit(ETX);
}

/*****************************************************************************/
/**********  States_Check()                                                         /
/**********  ״̬���                                                /

/*****************************************************************************/
void States_Check(uchar DLEN)
{
   uchar temp;
   uchar i;
   uchar ret;
   uchar error_number=0;
   for(i=1;i<9;i++)
   {
      state_check[i]=0;
   }
   state[1] = GetPA(4) + GetPA(3)*2 + GetPA(1)*4;//���ҿڴ�����1�����ҿڴ�����2����մ�����  0x07
   POWER_LED_OFF;
   delay(5);
   check[1] = GetPA(4) + GetPA(3)*2 + GetPA(1)*4; //���ҿڴ�����1�����ҿڴ�����2����մ�����0x00
   POWER_LED_ON;
   //sole_en_ON;
   motor_en_ON;
   Sole_ON();
   out_coin_sensors_check(BACKWARD,1000,1);
   WDR();   
   if(((check[1] ^ 0x07) & 0x07) == (state[1] & 0x07))
   {
      state_check[1]=state_check[1] | 0x00;      //�ޱ���   0x00
	  display(0);
   }
   else
   {
      if((check[1] & 0x01) == (state[1] & 0x01))
	  {
	     state_check[1]=state_check[1] | 0x01;  //������1����   0x01
		 error_number++;
		 display(1);
	  }
	  if((check[1] & 0x02) == (state[1] & 0x02))
	  {
	     state_check[1]=state_check[1] | 0x02;  //������2����   0x02
		 error_number++;
		 display(2);
	  }
	  if((check[1] & 0x04) == (state[1] & 0x04))
	  {
	     state_check[1]=state_check[1] | 0x04;  //��մ���������  0x04
		 error_number++;
		 display(3);
	  }
   }
   Sole_OFF();
   motor_stop();
   state[2] = 0x00;
   state_check[2]=0x00;
   state[3] = 0x00;
   state_check[3]=0x00;
   state[4] = 0x00;
   state_check[4]=0x00;   
   state[5] = BUSY*2+GetPA(2)*16 +GetPA(0)*32;//æµ���ѿգ�����
   POWER_LED_OFF;
   delay(5);
   check[5] = BUSY*2+GetPA(2)*16 +GetPA(0)*32;//æµ���ѿգ�����
   POWER_LED_ON;
   if(((check[5] ^ 0x30) & 0x30) == (state[5] & 0x30))
   {
      state_check[5]=state_check[5] | 0x00;      //�ޱ���   0x00
	  if(error_number==0)
	  {
	     display(0);
	  }
   }
   else
   {
      if((check[5] & 0x10) == (state[5] & 0x10))
	  {
	     state_check[5]=state_check[5] | 0x10;  //�ѿմ���������   0x10
		 display(4);
	  }
	  if((check[5] & 0x20) == (state[5] & 0x20))
	  {
	     state_check[5]=state_check[5] | 0x20;  //���մ���������   0x20
		 display(5);
	  }
   }
   state[6] = 0x00;
   state_check[6]=0x00;
   state[7] = 0x00;
   state_check[7]=0x00;
   state[8] = IO_init_OK*128;
   state_check[8]=0x00;
   USART0_Transmit(STX);
   USART0_Transmit(RSEQ);
   temp = RSEQ;
   USART0_Transmit(DESA);
   temp += DESA;
   USART0_Transmit(SRCA);
   temp += SRCA;
   USART0_Transmit(DLEN);
   temp += DLEN;
   USART0_Transmit(ACK_CMD_INTIME);
   temp += ACK_CMD_INTIME;
   USART0_Transmit(CMD_STATE_Check);
   temp += CMD_STATE_Check;
   for(i=1;i<9;i++)
   {
     if((state_check[i]==0x80)||(state_check[i]==0x7F))
	 {
	    USART0_Transmit(STX);
	 }
	 USART0_Transmit(state_check[i]);
     temp += state_check[i];
   }
   if((temp==0x80)||(temp==0x7F))
   {
	    USART0_Transmit(STX);
   }
   USART0_Transmit(temp);
   USART0_Transmit(ETX);
}

void display(uchar number)
{
   OFFPA(5);
   OFFPA(6);
   PORTC=table[number];
}

void Version_Get(uchar DLEN)
{
   uint i;
   uchar temp;
   USART0_Transmit(STX);
   USART0_Transmit(RSEQ);
   temp = RSEQ;
   USART0_Transmit(DESA);
   temp += DESA;
   USART0_Transmit(SRCA);
   temp += SRCA;
   USART0_Transmit(DLEN);
   temp += DLEN;
   USART0_Transmit(ACK_CMD_INTIME);
   temp += ACK_CMD_INTIME;
   USART0_Transmit(CMD_READ_VERSION);
   temp += CMD_READ_VERSION;
   for(i=0;i<16;i++)
   {
     if((HOPPER_version[i]==0x80)||(HOPPER_version[i]==0x7F))
	 {
	    USART0_Transmit(STX);
	 }
	 USART0_Transmit(HOPPER_version[i]);
     temp += HOPPER_version[i];
   }
   if((temp==0x80)||(temp==0x7F))
   {
	    USART0_Transmit(STX);
   }
   USART0_Transmit(temp);
   USART0_Transmit(ETX);
}