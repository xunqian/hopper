NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   008C   0037 =     55. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       008C  __idata_start
       00C3  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   00C4   125E =   4702. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0062  __start
       0062  __text_start
       0084  _exit
       0085  _delay
       0095  _delay_ms
       009E  _port_init
       00B8  _timer1_init
       00CC  _timer1_ovf_isr
       0156  _uart0_init
       0163  _USART0_Transmit
       0167  _uart0_rx_isr
       0240  _watchdog_init
       0246  _init_devices
       025A  _calculateLRC
       0270  _CheckLRC
       0291  _motor_forword
       029C  _motor_stop
       02A7  _Sole_ON
       02AD  _Sole_OFF
       02B3  _motor_backword
       02BE  _INIT_IO
       02D3  _HD_SELF
       02F8  _CHG
       02FA  _sole_check
       0334  _CLR
       0397  _read_sensorstatus
       03CC  _out_coin_sensors_check
       04A5  _out_coin_check
       0517  _Check_command
       05CE  _main
       05E8  _IOupACK
       0604  _IOupDATA
       0699  _States_Get
       0744  _States_Check
       08D8  _display
       08E3  _Version_Get
       0929  empy16s
       0939  pop_xgsetF0FC
       0944  push_xgsetF0FC
       0946  push_xgset30FC
       0948  push_xgset00FC
       094A  push_xgset003C
       094F  push_xgset300C
       0954  pop_xgset300C
       0959  push_xgsetF000
       095E  pop_xgsetF000
       0963  push_xgsetF00C
       096A  pop_xgsetF00C
       0971  push_xgset303C
       0978  pop_xgset303C
       097F  push_xgsetF03C
       0988  pop_xgsetF03C
       0991  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0100   0037 =     55. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0100  _time1
       0100  __data_start
       0101  _out_time
       0103  _out_coin
       0104  _Uart_time_start
       0105  _CLR_Number
       0107  _m_RecSize
       0108  _sensor_change
       0109  _sensor_change_CEm
       010A  _empty_change
       010B  _closeempty_change
       010C  _out_coin_error
       010D  _clr_coin_error
       010E  _HOPPER_version
       011E  _table
       012E  _BUSY
       012F  _sensor_on_time
       0131  _EMPTY_time
       0133  _command
       0134  _undo
       0135  _DIG_TOKEN
       0136  _DIG_TOKEN_num
       0137  __data_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             bss   0137   0063 =     99. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0137  __bss_start
       0137  _message
       0138  _get_over
       0139  _IO_init_OK
       013A  _state_check
       0144  _check
       014E  _state_1
       0158  _state
       0162  _buffer
       0176  _m_RecBuffer
       018A  _CEm
       018B  _Em
       018C  _Uart_time
       018E  _sole_time
       0190  _number
       0191  _direction
       0192  _sensor_state4
       0193  _sensor_state3
       0194  _pre_sensor_state4
       0195  _pre_sensor_state3
       0196  _pre_sensor_state_outcoin2
       0197  _pre_sensor_state_outcoin1
       0198  _sensor_state_outcoin2
       0199  _sensor_state_outcoin1
       019A  __bss_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   004C =     76. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\iccv7avr\lib\crtatmega.o	[ crtatmega.s ]
main.o	[ main.c ]
<library>	[ emul16s.s, gpop5.s, gpush.s, gpushx.s, gpushy.s, gpushz.s, gpusha.s, gpushb.s ]

User Global Definitions

ram_end = 0x10ff
hwstk_size = 0x1e

User Base Address Definitions

func_lit = 0x8c
data = 0x100
eeprom:0.2048

