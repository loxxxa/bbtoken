

#ifndef __USBKEYDRIVER_HEAD
#define __USBKEYDRIVER_HEAD

//��󻺳���ֵ

//���ͷ�ĳ���,��CCID��ͷ�ĳ���
#define MAX_HEAD_SIZE (10)
//sw1��sw2��ռ����
#define SW1SW2_SIZE (2)
//ADPU�ĳ���
#define CMD_SIZE (5)

//���д���ݳ��� (data)
//Ϊ������ݰ�����-ADPU�ĳ���
#define MAX_WRITEDATA_BUF (MAX_CMD_BUF-CMD_SIZE)

//�������ݳ��� (data)
//Ϊ������ݰ��ĳ���-ADPU�ĳ���-��������Ҫ����SW1SW2
#define MAX_READDATA_BUF (MAX_CMD_BUF-CMD_SIZE-SW1SW2_SIZE)

//�������󳤶�,����������Ը�����data.
//���оƬI/O����-�������ͷ����
#define MAX_CMD_BUF MAX_DATA_BUF

//�ⲿ������ݰ�����
//������д�뵽driver����󳤶���.
//���оƬI/O����-�������ͷ����
#define MAX_DATA_BUF (MAX_PACKET_SIZE-MAX_HEAD_SIZE)

//�����ߴ�, ����driver�ڲ��Ŀ���ֵ,
//USBоƬһ��I/O���������,
//�ⲿӦ���ò���
//����ccidЭ��,���İ����ȿ���Ϊ0xFFFFFFFF
#define MAX_PACKET_SIZE (2048)

#endif

