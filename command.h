//定义命令字

#define CMD_SW_RESET              0x87
#define CMD_STATE_QUERY           0x12
#define CMD_STATE_Check           0x13
#define CMD_READ_VERSION          0x16
#define CMD_HD_SELF               0x40
#define CMD_CHG                   0x60
#define CMD_CLR                   0x61
#define CMD_CLR_STOP              0x62



//定义故障字
#define ACK_ERR_START               
#define ACK_ERR_ADDR_SAME    
#define ACK_ERR_MSG_CHK
#define ACK_ERR_CMD_INVALID
#define ACK_ERR_CMD_HOPPER_BUSY
#define ACK_ERR_CMD_NOCMD
#define ACK_ERR_END

//定义回应字
#define ACK_START               0x80
#define ACK_CMD_ACK             0x90
#define ACK_CMD_INTIME          0xA0
#define ACK_CMD_UNTIME          0xA1
#define ACK_ERR                 0xE0
#define ACK_END                 0xFF

