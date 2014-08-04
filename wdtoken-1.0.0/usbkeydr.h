

#ifndef __USBKEYDRIVER_HEAD
#define __USBKEYDRIVER_HEAD

//最大缓冲区值

//最大头的长度,是CCID的头的长度
#define MAX_HEAD_SIZE (10)
//sw1和sw2所占长度
#define SW1SW2_SIZE (2)
//ADPU的长度
#define CMD_SIZE (5)

//最大写数据长度 (data)
//为最大数据包长度-ADPU的长度
#define MAX_WRITEDATA_BUF (MAX_CMD_BUF-CMD_SIZE)

//最大读数据长度 (data)
//为最大数据包的长度-ADPU的长度-返回数据要带的SW1SW2
#define MAX_READDATA_BUF (MAX_CMD_BUF-CMD_SIZE-SW1SW2_SIZE)

//命令的最大长度,包括命令可以附带的data.
//最大芯片I/O容量-最大命令头长度
#define MAX_CMD_BUF MAX_DATA_BUF

//外部最大数据包长度
//这是能写入到driver的最大长度了.
//最大芯片I/O容量-最大命令头长度
#define MAX_DATA_BUF (MAX_PACKET_SIZE-MAX_HEAD_SIZE)

//最大包尺寸, 这是driver内部的控制值,
//USB芯片一次I/O的最大容量,
//外部应该用不到
//根据ccid协议,最大的包长度可以为0xFFFFFFFF
#define MAX_PACKET_SIZE (2048)

#endif

