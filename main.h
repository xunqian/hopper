#include <iom64v.h>
#include <macros.h>

#include <AVRdef.h> 

#define uchar unsigned char 
#define uint unsigned int 
#define ulong unsigned long

#define GetPA(x)  ((PINA&(1<<x)) ? 1 : 0)
#define GetPB(x)  ((PINB&(1<<x)) ? 1 : 0)
#define GetPC(x)  ((PINC&(1<<x)) ? 1 : 0)
#define GetPD(x)  ((PIND&(1<<x)) ? 1 : 0)
#define GetPE(x)  ((PINE&(1<<x)) ? 1 : 0)
#define GetPF(x)  ((PINF&(1<<x)) ? 1 : 0)
#define GetPG(x)  ((PING&(1<<x)) ? 1 : 0)

#define ONPA(y)   (PORTA|=(BIT(y)))
#define OFFPA(y)  (PORTA&=~(BIT(y)))
#define ONPB(y)   (PORTB|=(BIT(y)))
#define OFFPB(y)  (PORTB&=~(BIT(y)))
#define ONPC(y)   (PORTC|=(BIT(y)))
#define OFFPC(y)  (PORTC&=~(BIT(y)))
#define ONPD(y)   (PORTD|=(BIT(y)))
#define OFFPD(y)  (PORTD&=~(BIT(y)))
#define ONPE(y)   (PORTE|=(BIT(y)))
#define OFFPE(y)  (PORTE&=~(BIT(y)))
#define ONPF(y)   (PORTF|=(BIT(y)))
#define OFFPF(y)  (PORTF&=~(BIT(y)))
#define ONPG(y)   (PORTG|=(BIT(y)))
#define OFFPG(y)  (PORTG&=~(BIT(y)))

#define L_LEVEL    0
#define H_LEVEL    1
#define FOREWARD 1
#define BACKWARD 0
//1S时间
#define SECOND  500 
//200MS时间
#define M_SECOND  100 
//出币口传感器 ON是遮挡，OFF是未遮挡
#define Token_out_SENSOR1_OFF      (H_LEVEL == GetPA(PA4))
#define Token_out_SENSOR1_ON      (L_LEVEL == GetPA(PA4))

#define Token_out_SENSOR2_OFF      (H_LEVEL == GetPA(PA3))
#define Token_out_SENSOR2_ON      (L_LEVEL == GetPA(PA3))
//空传感器
#define UNEMPTY      (L_LEVEL == GetPA(PA2))
#define EMPTY      (H_LEVEL == GetPA(PA2))
//清币传感器
#define SOLE_ON       (H_LEVEL == GetPA(PA1))
#define SOLE_OFF       (L_LEVEL == GetPA(PA1))
//将空传感器
#define close_EMPTY_OFF       (L_LEVEL == GetPA(PA0))
#define close_EMPTY_ON      (H_LEVEL == GetPA(PA0))

//电机堵转 新版本
#define motor_over       (H_LEVEL == GetPA(PA7))
//旧版本
//#define motor_over       (H_LEVEL == GetPB(PB6))

//电机使能，电磁铁使能，传感器电源
#define motor_en_ON          ONPB(1)
#define sole_en_ON           ONPB(0)
#define POWER_LED_ON         OFFPE(2)

#define motor_en_OFF         OFFPB(1)
#define sole_en_OFF          OFFPB(0)
#define POWER_LED_OFF        ONPE(2)



//电机正转
#define motor_en1        ONPF(0)
#define motor_en2        OFFPF(1)

//电机反转
#define motor_uen1        OFFPF(0) 
#define motor_uen2        ONPF(1)

//电磁铁吸合
#define sole_en1        ONPF(2)
#define sole_en2        OFFPF(3)

#define STX           0x7F
#define RSEQ          0x00
#define DESA          0x00
#define SRCA          0x00
#define ETX           0x80

void delay(uint time);
void delay_ms(void);
void USART0_Transmit(uchar c);
void Check_command(void);
//void IOupACK(uchar DLEN,uchar DATA);
void IOupACK(uchar DLEN,uchar DATE);
void States_Get(uchar DLEN);
void check_sensors(void);
void out_coin_check(void);

void calculateLRC(uchar*buffer, int length,uchar lrc);
uchar CheckLRC(void);
uchar out_coin_sensors_check(uchar direction,uint over_time,uchar empty_check);
void read_sensorstatus(void);
void display(uchar number);

void HD_SELF(void);
void CHG(void);
void CLR(void);

void States_Get(uchar DLEN);
//void Version_Get(void);