#define _NO_VERSION_
#ifndef __KERNEL__
#define __KERNEL__
#endif

#ifndef MODULE
#define MODULE
#endif

#include <linux/module.h>
//#include <linux/config.h>
#include <linux/version.h>
char kernel_version[]={9};//UTS_RELEASE;

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/random.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/wait.h>

//设定各个长度
//#include "../include/usbkeydrv.h"
#include "usbkeydr.h"

int debug; // for usb-serial.h 

#include <linux/usb.h>

#pragma pack(1)

#ifndef DECLARE_MUTEX
#define DECLARE_MUTEX(name) DEFINE_SEMAPHORE(name)
#endif

#ifndef init_MUTEX
#define init_MUTEX(sem) sema_init(sem,1)
#define init_MUTEX_LOCKED(sem)  sema_init(sem, 0)
#endif
//==============================
//==				函数定义					==
//==============================
//static void WD_irq(struct urb *urb);
static ssize_t WD_read (struct file *file,char *buf, size_t len, loff_t *ppos);
static ssize_t WD_write (struct file *file,	const char *buf, size_t len, loff_t *ppos);
static int WD_open (struct inode *inode, struct file *file);
static int WD_release (struct inode *inode, struct file *file);
static int WD_probe (struct usb_interface *udev, const struct usb_device_id *id);
static void WD_disconnect(struct usb_interface *udev);
static int __init WD_init(void);
static void __exit WD_cleanup(void);


#define VERSION 	"WatchKey Installation Disk V1.0"
#define RELEASE_DATE 	"(3/1/2002)"
#define MODULE_NAME	"WatchToken(2.6)"

/* Vendor and Product Information */
//CRW-VIII读卡器
#define USBREADER_VENDOR_CRWVIII 	0x163c
#define USBREADER_PRODUCT_CRWVIII	0x0803
//ST芯片usbkey
#define USBREADER_VENDOR_USBKEY_ST 	0x6513
#define USBREADER_PRODUCT_USBKEY_ST	0x0089
//zte芯片usbkey
#define USBREADER_VENDOR_USBKEY_ZTE 	0x163c
//#define USBREADER_PRODUCT_USBKEY_ZTE	0x0401
#define USBREADER_PRODUCT_USBKEY_ZTE	0x0407

//三种设备
#define DEVICE_TYPE_USBKEY_ST	1
#define DEVICE_TYPE_USBKEY_ZTE	2
#define DEVICE_TYPE_CRWVIII	3


#define DEVICE_CRWVIII_INTERFACE_CLASS_1		USB_CLASS_PER_INTERFACE
//这个于st的是不同的属性值
#define DEVICE_USBKEY_ZTE_INTERFACE_CLASS_1		USB_CLASS_CSCID
#define DEVICE_USBKEY_ST_INTERFACE_CLASS_1		USB_CLASS_PER_INTERFACE

#define DEVICE_CRWVIII_INTERFACE_CLASS_2		USB_CLASS_VENDOR_SPEC
#define DEVICE_USBKEY_ZTE_INTERFACE_CLASS_2		USB_CLASS_VENDOR_SPEC
#define DEVICE_USBKEY_ST_INTERFACE_CLASS_2		USB_CLASS_VENDOR_SPEC

//端点的数量
#define DEVICE_CRWVIII_ENDPOINTS 1
#define DEVICE_USBKEY_ZTE_ENDPOINTS 2
#define DEVICE_USBKEY_ST_ENDPOINTS 2

#define DEVICE_USBKEY_ST_INTERFACE_SUBCLASS 0
#define DEVICE_USBKEY_ZTE_INTERFACE_SUBCLASS 0
#define DEVICE_USBKEY_CRWVIII_INTERFACE_SUBCLASS 0

#define DEVICE_USBKEY_ST_PROTOCOL 0
#define DEVICE_USBKEY_ZTE_PROTOCOL 0
#define DEVICE_USBKEY_CRWVIII_PROTOCOL 0


#define IRQ_STATE_LINK 1
#define IRQ_STATE_UNLINK 0
/* current USB framework handles max of 16 USB devices per driver */
#define	MAX_USBREADERS		16
/* USB char devs use USB_MAJOR and from USB_USBREADER_MINOR_BASE up */
#define	USB_USBREADER_MINOR_BASE	224


/* XXX remove packet size limit, now that bulk transfers seem fixed */

#define	MAX_READ_RETRY		500		/* times to retry reads */
#define	MAX_WRITE_RETRY		500		/* times to retry writes */
#define	RETRY_TIMEOUT		10	/* sleep between retries  1000ms or 1s*/

#define SHARE_MODE 1 //0 独占模式 1 共享模式

#define dw2i(a, x) ((((((a[x+3] << 8) + a[x+2]) << 8) + a[x+1]) << 8) + a[x])

//返回标志
#define RETURN_ERROR 1
#define RETURN_SUCCESS 0

//T1
#define NAD 0x12

//CCID Command Pipe Bulk-OUT Messages
#define PC_to_RDR_IccPowerOn 0x62
#define PC_to_RDR_IccPowerOff 0x63
#define PC_to_RDR_GetSlotStatus 0x65
#define PC_to_RDR_XfrBlock 0x6F
#define PC_to_RDR_GetParameters 0x6C
#define PC_to_RDR_ResetParameters 0x6D
#define PC_to_RDR_SetParameters 0x61
#define PC_to_RDR_Escape 0x6B
#define PC_to_RDR_IccClock 0x6E
#define PC_to_RDR_T0APDU 0x6A
#define PC_to_RDR_Secure 0x69
#define PC_to_RDR_Mechanical 0x71
#define PC_to_RDR_Abort 0x72
#define PC_to_RDR_SetDataRateAndClockFrequency 0x73

//CCID Command Pipe Bulk-IN Messages
#define RDR_to_PC_DataBlock 0x80
#define RDR_to_PC_SlotStatus 0x81
#define RDR_to_PC_Parameters 0x82
#define RDR_to_PC_Escape 0x83
#define RDR_to_PC_DataRateAndClockFrequency 0x84

#define CCID_BULKIN_STATUS_ICC_OK	0
#define CCID_BULKIN_STATUS_COMMAND_OK	0

#define STATUS_OFFSET 7

//bmCommandStatus =bit(1)
#define CCID_COMMAND_FAILED 0x40
#define CCID_ICC_NOPRESENT  0x42

#define CCID_TIME_EXTENSION 0x80

//特别注意要.
#ifndef BYTE
	typedef unsigned char BYTE;
#endif

#ifndef DWORD
#    if defined(__alpha) || defined(__amd64__) || defined(__x86_64__)
typedef unsigned int DWORD;
#else
typedef unsigned long DWORD;
#    endif
#endif

//#################################################################################################
//============================
//==			数据结构					==
//============================
//两种协议的头
struct __ccid_head{
	BYTE bMessageType;
	DWORD dwLength;
	BYTE bSlot;
	BYTE bSequ;
	union{
		BYTE UNUSED1;
		BYTE eTimeOut;
	};
	union{
		BYTE UNUSED2;
		BYTE eHLen;
	};
	union{
		BYTE UNUSED3;
		BYTE eLLen;
	};
};

struct __T1_head{
	BYTE nad;
	BYTE CLA;
	BYTE len;
};
enum{COMMAND_RESET,COMMAND_SENDDATA,COMMAND_NULL};

struct usbreader_state {
	struct usb_device	*dev;			/* USB device handle */
	int			DeviceType;
	int			inEP;					/* read endpoint */
	int			outEP;					/* write endpoint */
	int			irqEP;					//中断控制点
	const struct usb_device_id	*info;	/* DC-240, etc */
	int			subminor;				/* which minor dev #? */
	struct semaphore	sem;			/* locks this struct */
	char			*buf;				/* buffer for I/O */
	/* always valid */
	wait_queue_head_t	queue;			/* for timed waits */
	int open_count;							/* if open 使得多个应用可以同时打开Key*/
	int present;						/* Device is present on the bus */
	int share_mode;
	struct urb reader_irq;
	int card_state;						//卡状态标志
	unsigned char button[8];
	int irqState;						//irq标志
	int iLastCommand;					//记录最后一次的命令,用于ccid的复位命令加上正确返回值


};

//#################################################################################################
//============================
//==			宏								==
//============================
#define outp(format,arg...)						\
	do{											\
	printk("%s %s ",__FILE__,__FUNCTION__);		\
	printk(format,##arg);						\
	printk("\n");								\
	}while(0)

/*
*	type	type
*	rc		receive count
*	rv		return value
*	b		target buffer
*	sb		source buffer
*	rlen	target len
*/
#define copybuf(type,rc,rv,b,sb,rlen) {							\
	int realLen;												\
	if(rc < sizeof(struct __##type##_head)){					\
	rv=-EFAULT;												\
	}															\
	if(rlen < (rc-sizeof(struct __##type##_head))){				\
	realLen = rlen;											\
	}else{														\
	realLen=rc-sizeof(struct __##type##_head);				\
	}															\
	if (copy_to_user(b, (sb+sizeof(struct __##type##_head)),	\
	realLen))												\
	rv = -EFAULT;										\
	else{														\
	rv = realLen;											\
	}															\
}

#define isZTE(des)	\
	(checkDEV(des,USBREADER_VENDOR_USBKEY_ZTE,USBREADER_PRODUCT_USBKEY_ZTE))

#define isST(des)	\
	(checkDEV(des,USBREADER_VENDOR_USBKEY_ST,USBREADER_PRODUCT_USBKEY_ST))

#define isVIII(des)	\
	(checkDEV(des,USBREADER_VENDOR_CRWVIII,USBREADER_PRODUCT_CRWVIII))

#define checkDEV(des,VID,PID)	\
	((des.idVendor == VID) && (des.idProduct == PID))
//===========================

//#################################################################################################

//=============================
//==			全局变量					===
//=============================
static DECLARE_MUTEX (state_table_mutex);
/* Support multiple usbreaders, possibly of different types.  */
static struct usbreader_state *minor_data [MAX_USBREADERS];
/* table of usbreaders that work through this driver */
static struct usb_device_id usbreader_table [] = {
	{ USB_DEVICE(USBREADER_VENDOR_CRWVIII, USBREADER_PRODUCT_CRWVIII) },
	{ USB_DEVICE(USBREADER_VENDOR_USBKEY_ST, USBREADER_PRODUCT_USBKEY_ST) },
	{ USB_DEVICE(USBREADER_VENDOR_USBKEY_ZTE, USBREADER_PRODUCT_USBKEY_ZTE) },
	{ }						/* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, usbreader_table);

static struct file_operations usb_usbreader_fops = {
owner:		THIS_MODULE,
read:		WD_read,
write:		WD_write,
open:		WD_open,
release:	WD_release,
};

static struct usb_class_driver usbreader_class= {
	.name=		"usb/WDUsbKey%d",
	.fops=		&usb_usbreader_fops,
	//.mode=	S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH,
	.minor_base=	USB_USBREADER_MINOR_BASE,
};

static struct usb_driver usbreader_driver = {
	.name=		MODULE_NAME,
	//.owner=	THIS_MODULE,
	.id_table=	usbreader_table,
	.probe=		WD_probe,
	.disconnect=	WD_disconnect,
};

//#################################################################################################
//=============================
//==			基本函数					===
//=============================
static inline void usbreader_delete(struct usbreader_state *usbreader)
{
	if(usbreader != NULL){
		if(usbreader->buf != NULL ){
			kfree(usbreader->buf);
			usbreader->buf=NULL;
		}
		kfree(usbreader);
		usbreader=NULL;
	}
	return;
}

static void i2dw(int value, unsigned char buffer[])
{
	buffer[0] = value & 0xFF;
	buffer[1] = (value >> 8) & 0xFF;
	buffer[2] = (value >> 16) & 0xFF;
	buffer[3] = (value >> 24) & 0xFF;
} /* i2dw */

//=============================

static ssize_t WD_read (struct file *file,char *buf, size_t len, loff_t *ppos){
	struct usbreader_state	*usbreader;
	short			retries;
	short			retval = 0;
	char		sw12_ok[]={0x90,0x00};
	char		sw12_NOICC[]={0x62,0x00};
	char		sw12_UNKNOW[]={0x62,0xff};
	short			need_len = 0;
	usbreader = (struct usbreader_state *) file->private_data;

	DEFINE_WAIT(wait);


	switch(usbreader->DeviceType ){
		case DEVICE_TYPE_CRWVIII:
		case DEVICE_TYPE_USBKEY_ST:
			//这是T=1协议
			if((need_len=len+sizeof(struct __T1_head)) > MAX_PACKET_SIZE) return  -EINVAL;
			//这里,向设备发送的命令中,要求返回的长度以外,还有加上头 和 sw1 sw2两个字节
			break;
		case DEVICE_TYPE_USBKEY_ZTE:
			//这是CCID协议
			if((need_len=len+sizeof(struct __ccid_head)) > MAX_PACKET_SIZE) return  -EINVAL;
			//这里,向设备发送的命令中,要求返回的长度以外,还有加上头 和 sw1 sw2两个字节
			break;
		default:
			return -ENODEV;
	}
	down (&usbreader->sem);
	if (!usbreader->dev) {
		up (&usbreader->sem);
		return -ENODEV;    /* -ENODEV : device not existing or removed */
	}

	for (retries = 0; retries < MAX_READ_RETRY; retries++) {
		int			count;

		if (signal_pending (current)) {
			retval = -EINTR;
			break;
		}

		//这里,根据2048bit密钥的时间问题,修改大一些时间
		//10分钟*3
		//现在设计的是,一次读写若返回的数据长度超过了MAX_PACKET_SIZE,则失败
		//根据ccid的协议,这个SIZE最大可以到0xFFFFFFFF这么长.
		//现在是0x800
		retval = usb_bulk_msg (usbreader->dev,
			usb_rcvbulkpipe (usbreader->dev, usbreader->inEP),
			usbreader->buf, need_len, &count, HZ*600000);
#ifdef DEBUG
		outp("retval=%d,need_len=%d",retval,need_len);
#endif
		if (!retval) {
			//现在不能直接拷回去了.
			switch(usbreader->DeviceType ){
				case DEVICE_TYPE_CRWVIII:
				case DEVICE_TYPE_USBKEY_ST:
					//这是T=1
					copybuf(T1,count,retval,buf,(usbreader->buf),len)
					goto done;
					break;
				case DEVICE_TYPE_USBKEY_ZTE:
					//这是CCID
					//首先判断是否有错误发生
					if((usbreader->buf[STATUS_OFFSET]) & CCID_COMMAND_FAILED){
						//这是错误了
						if((usbreader->buf[STATUS_OFFSET]) & CCID_ICC_NOPRESENT){
							//无卡
							//在返回的数据的后面加上结果的值,
							if(count < (MAX_PACKET_SIZE-2)){
								memcpy((usbreader->buf+count),sw12_NOICC,2);
							}else{
								retval=-EFAULT;
							}
						}else{
							//其他错误
							if(count < (MAX_PACKET_SIZE-2)){
								memcpy((usbreader->buf+count),sw12_UNKNOW,2);
							}else{
								retval=-EFAULT;
							}
						}
						goto done;
					}else if (usbreader->buf[STATUS_OFFSET] & CCID_TIME_EXTENSION){
						retval=-EFAULT;
						goto need_sleep;
					}
					if(usbreader->iLastCommand == COMMAND_RESET){
						//这里是对上一个命令是复位命令的处理,
						//这和普通命令是有区别的
						//要给一个正确sw0 sw1的值在后面
						if(count >= sizeof(struct __ccid_head)){
							if(count < (MAX_PACKET_SIZE-2)){
								memcpy((usbreader->buf+count),sw12_ok,2);
								count=dw2i((usbreader->buf),1) + sizeof(struct __ccid_head);
								count+=2;
							}else{
								retval=-EFAULT;
								goto done;
							}
						}
					}
					copybuf(ccid,count,retval,buf,(usbreader->buf),len)
						goto done;
					break;
				default:
					return -ENODEV;
			}
			break;
		}
		if (retval != -ETIMEDOUT){
			retval=-EFAULT;
			break;
		}
need_sleep:

                prepare_to_wait(&usbreader->queue, &wait, TASK_INTERRUPTIBLE);
                schedule_timeout(RETRY_TIMEOUT);
                finish_wait(&usbreader->queue, &wait);

		//dbg ("read (%d) - retry", len);
	}//for 
done:
	up (&usbreader->sem);
#ifdef DEBUG
	outp("%s retval=%d","WD read",retval);
#endif
	return retval;
}

static ssize_t WD_write (struct file *file,	const char *buf, size_t len, loff_t *ppos)
{
	struct usbreader_state	*usbreader;
	ssize_t			bytes_written = 0;
	struct __ccid_head ccid_head;
	struct __T1_head T1_head;

	BYTE		*obuf;
	short		maxretry = MAX_WRITE_RETRY;
	DWORD	copy_size, thistime;
	BYTE tmpbuf[MAX_PACKET_SIZE];

	char ResetICCCommand[]={0x00,0x12,0x00,0x00,0x00};
	char ResetDeviceCommand[]={0x00,0x19,0x00,0x00,0x00};
	char T1_ResetCommand[]={0x00,0x12,0x00,0x00,0x00};
	char ccid_ResetCommand[]={PC_to_RDR_IccPowerOn,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

	DEFINE_WAIT(wait);

	usbreader = (struct usbreader_state *) file->private_data;

	obuf = usbreader->buf;
	//这里判断要写入的len是否合法
	//因为原来的driver中,这个头是在外部已经添加好的,所以len< Max_PACKET_SIZE就可以了
	//但是现在需要在driver中加,所以外部能写入的最大值不能超过这个值减去最大的头的长度
	//
	switch(usbreader->DeviceType ){
		case DEVICE_TYPE_CRWVIII:
		case DEVICE_TYPE_USBKEY_ST:
			//T=1
			if((len+sizeof(struct __T1_head)) > MAX_PACKET_SIZE) return  -EINVAL;
			break;
		case DEVICE_TYPE_USBKEY_ZTE:
			//CCID
			if((len+sizeof(struct __ccid_head)) > MAX_PACKET_SIZE) return  -EINVAL;
			break;
		default:
			return -ENODEV;
	}


	down (&usbreader->sem);

	if (!usbreader->dev) {
		up (&usbreader->sem);
		return -ENODEV;
	}


	copy_size = len;
	thistime=0;
	if(copy_from_user (tmpbuf, buf, copy_size)){
		bytes_written = -EFAULT;
		goto done;
	}

	//从这里就要开始整理这个obuf了
	switch(usbreader->DeviceType ){
		case DEVICE_TYPE_CRWVIII:
		case DEVICE_TYPE_USBKEY_ST:
			//
			memset(&T1_head,0,sizeof(struct __T1_head));

			T1_head.nad = NAD;
			T1_head.len = len;
			memcpy(obuf,&T1_head,sizeof(struct __T1_head));

			if( (memcmp(tmpbuf,ResetICCCommand,5) == 0 ) && (len == 5) ){
				memcpy((obuf+sizeof(struct __T1_head)),T1_ResetCommand,5);
			}else{
				memcpy((obuf+sizeof(struct __T1_head)), tmpbuf, copy_size);
			}
			thistime = len+sizeof(struct __T1_head);
			usbreader->iLastCommand =COMMAND_NULL;
			break;
		case DEVICE_TYPE_USBKEY_ZTE:
			//这是CCID
			/*
			*	若是设备复位命令,
			*	按照解决方案,要屏蔽这条命令,
			*	但是屏蔽以后,上层程序要read一次,而这次read必然返回错误了.
			*	所以这里将设备复位命令也解释成ccid的icc复位命令.保证有返回
			*/
			if( ((memcmp(tmpbuf,ResetICCCommand,5) == 0) || (memcmp(tmpbuf,ResetDeviceCommand,5) == 0)) && (len == 5) ){
				memcpy(obuf,ccid_ResetCommand,10);
				usbreader->iLastCommand=COMMAND_RESET;
				thistime = sizeof(struct __ccid_head);
			}else{
				memset(&ccid_head,0,sizeof(struct __ccid_head));
				//命令类型
				ccid_head.bMessageType = PC_to_RDR_XfrBlock;
				//这是长度,命令的长度
				i2dw(len,(unsigned char *)&ccid_head.dwLength);
				//ccid_head.dwLength = len;
				//这个slot没有设置
				//ccid_head.bSlot=0;
				//这个顺序号也没有设置的必要应该.
				//ccid_head.bSequ=0;
				// extend block waiting timeout
				ccid_head.eTimeOut=0;
				// Expected length
				//ccid_head.eHLen=MAX_PACKET_SIZE & 0xFF;
				//ccid_head.eLLen=(MAX_PACKET_SIZE>>8) & 0xFF;
				memcpy(obuf,&ccid_head,sizeof(struct __ccid_head));
				memcpy((obuf+sizeof(struct __ccid_head)), tmpbuf, copy_size);
				usbreader->iLastCommand =COMMAND_NULL;
				thistime = len+sizeof(struct __ccid_head);
			}
			break;
		default:
			break;
	}

	//若是需要分段传送,要另行处理,
	while (thistime) {
		int		result;
		int		count;

		if (signal_pending (current)) {
			if (!bytes_written)
				bytes_written = -EINTR;
			goto done;
		}
		count=0;
#ifdef DEBUG
		printk("%s %s ",__FILE__,__FUNCTION__);
		int __i;
		for(__i=0;__i<thistime;__i++){
			printk("%.2X ",obuf[__i]);
		}
		printk("\n");
#endif
		result = usb_bulk_msg (usbreader->dev,
			usb_sndbulkpipe (usbreader->dev, usbreader->outEP),
			obuf, thistime, &count, HZ*10);

		if(result){
			if (result == -ETIMEDOUT) {
				//只对超时作处理工作,其他直接返回
				if (!maxretry--) {
					bytes_written = -ETIME;
					goto done;
				}

				prepare_to_wait(&usbreader->queue, &wait, TASK_INTERRUPTIBLE);
				schedule_timeout(RETRY_TIMEOUT);
				finish_wait(&usbreader->queue, &wait);

				continue;
			}else{
				bytes_written = -EFAULT;
				goto done;
			}
		}
		if (count) {

			obuf += count;
			thistime -= count;
			maxretry = MAX_WRITE_RETRY;
			bytes_written +=count;
			continue;
		}else{
			//若result==0,但count也==0,则
			bytes_written=0;
			goto done;
		}
	}
	//因为前面的thistime是加上头的长度,这里要设置成实际值
	bytes_written=len;
done:
	up (&usbreader->sem);
	//dbg ("wrote %d", bytes_written);
#ifdef DEBUG
	outp("%s bytes_written=%d","WD write",bytes_written);
#endif
	return bytes_written;
}

static int WD_open (struct inode *inode, struct file *file)
{
	struct usbreader_state	*usbreader = NULL;
	struct usb_interface *interface;
	int			subminor;
	int			value = 0;

	down (&state_table_mutex);

	subminor = iminor(inode);
	interface = usb_find_interface (&usbreader_driver, subminor);

	if (!interface){
#ifdef DEBUG
		outp("%s","no find device");
#endif
			up (&state_table_mutex);
			return -ENODEV;
	}

	usbreader = usb_get_intfdata(interface);
	if (!usbreader) {
#ifdef DEBUG
		outp("%s","not find control bulk");
#endif
		up (&state_table_mutex);
		return -ENODEV;
	}
	down (&usbreader->sem);
	//这个锁没有用了?
	up (&state_table_mutex);

	if(usbreader->open_count){
		//已经打开了.
		value = -EBUSY;
		if(SHARE_MODE ==1){
			goto ok_out;
		}
		goto done;
	}


	if (usbreader->DeviceType==DEVICE_TYPE_CRWVIII ) {
		//这里应该是对CRW-VIII读卡器的设置工作了
	}

ok_out:
	usbreader->open_count++;
	file->private_data = usbreader;
	value=0;
#ifdef DEBUG
	outp("%s","WD open");
#endif
done:	
		up (&usbreader->sem);
		return value;
}

//释放工作
static int WD_release (struct inode *inode, struct file *file)
{

	struct usbreader_state	*usbreader;

	usbreader = (struct usbreader_state *) file->private_data;

	down (&state_table_mutex);
	down (&usbreader->sem);

	//计数先减1
	usbreader->open_count--;
	if(usbreader->open_count){
		if( SHARE_MODE == 1){
			//没有完全释放
			goto done;
		}else{
			//到了这里说明有了错误了.
		}
	}
	//到这里,open_count必须等于0了
	//也就是没有打开这个文件的调用了.
	//则对应的于open中的操作要进行反向操作了.
	if(usbreader->DeviceType == DEVICE_TYPE_CRWVIII){
		//这是对CRW-VIII的设置
	}

	//如果这里的dev已经不存在了,则说明前面已经进行的probe的反向操作(disconnect)
	//所以这里要替disconnect擦屁股了.
	if (!usbreader->dev) {
		up (&usbreader->sem);
		usbreader_delete (usbreader);
		goto ok_out;
	}

done:
	up (&usbreader->sem);
ok_out:
	up (&state_table_mutex);
#ifdef	DEBUG
	outp("%s ","WD release!");
#endif
	return 0;
}

/*
static void WD_irq(struct urb *urb){
//	struct usbreader_state	*usbreader;

	if(urb->status < 0 ) return;

	return;
}
*/

//添加设备
static int WD_probe (struct usb_interface *udev, const struct usb_device_id *id)
{
	//获得usb_device信息
	struct usb_device *dev = interface_to_usbdev(udev);
	struct usbreader_state *usbreader=NULL;
	struct usb_host_interface	*interface;
	struct usb_endpoint_descriptor *endpoint;
	int retval = -ENOMEM;
	int i;
	if(dev==NULL){
#ifdef DEBUG
		outp("%s","dev is Null!");
#endif
		goto Err_free;
	}
	usbreader = kmalloc (sizeof(struct usbreader_state), GFP_KERNEL);
	if( !usbreader){
#ifdef DEBUG
		outp("%s","kmalloc mem err!");
#endif
		retval = -ENOMEM;
		goto Err_free;
	}
	memset(usbreader,0,sizeof(struct usbreader_state));
	//分配空间
	usbreader->buf = (char *) kmalloc (MAX_PACKET_SIZE, GFP_KERNEL);
	if( !(usbreader->buf)){
#ifdef DEBUG
			outp("%s","kmalloc mem err!");
#endif
			retval = -ENOMEM;
			goto Err_free;
	}
	memset(usbreader->buf,0,MAX_PACKET_SIZE);

	//判断是否是其中的某个要求的设备
	if(isZTE(dev->descriptor)){
		//这种设置方法在应该进一步的优化.
		usbreader->DeviceType=DEVICE_TYPE_USBKEY_ZTE;
	}else if(isST(dev->descriptor)){
		usbreader->DeviceType=DEVICE_TYPE_USBKEY_ST;
	}else if(isVIII(dev->descriptor)){
		usbreader->DeviceType=DEVICE_TYPE_CRWVIII;
	}else{
		retval=-ENODEV;
		goto Err_free;
	}
	usbreader->inEP=usbreader->outEP=-1;
	interface = udev->cur_altsetting;
	//对于不同种类的设备,有不同的描述.
	switch(usbreader->DeviceType){
		case DEVICE_TYPE_USBKEY_ZTE:
		case DEVICE_TYPE_USBKEY_ST:
			for(i=0;i<interface->desc.bNumEndpoints;++i){
				endpoint = &interface->endpoint[i].desc;
				//这里都没有判断设备pipe的类型
				if ((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN) {
					usbreader->inEP = endpoint->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
				}
				if ((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT){
					usbreader->outEP = endpoint->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
				}
			}
			break;
		case DEVICE_TYPE_CRWVIII:
			//这里设置I/O都是0.
			for(i=0;i<interface->desc.bNumEndpoints;++i){
				endpoint = &interface->endpoint[i].desc;
				if (((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN) &&
					((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)) {
						//中断点的设置
						usbreader->irqEP = endpoint->bEndpointAddress;
					}
			}
			//i/o用同一个端点.
			usbreader->inEP = usbreader->outEP = 0;
			break;
		default:
			retval=-ENODEV;
			goto Err_free;
			break;
	}
#ifdef DEBUG
	outp("%s in point=%d out point=%d","POINT:", usbreader->inEP,usbreader->outEP);
#endif
	//设置该设备的一些属性
	init_MUTEX(&(usbreader->sem));
//	init_waitqueue_head (&usbreader->wait);
//	preepare_to_wait(q, &usbreader->wait, TASK_INTERRUPTIBLE);
	usbreader->dev = dev;

	usb_set_intfdata (udev,usbreader);

        if(usbreader->inEP == 1)
	  retval = usb_register_dev (udev, &usbreader_class);

	if (retval) {
#ifdef DEBUG
		outp("%s retval=%d","usb_register_dev err!",retval);
#endif
			/* something prevented us from registering this driver */
		usb_set_intfdata (udev, NULL);
		retval=-ENODEV;
		goto Err_free;
	}
#ifdef DEBUG
	switch(usbreader->DeviceType) {
	case DEVICE_TYPE_USBKEY_ZTE:
		outp("%s ","We have a device: ZTE");
		break;
	case DEVICE_TYPE_USBKEY_ST:
		outp("%s ","We have a device: ST");
		break;
	case DEVICE_TYPE_CRWVIII:
		outp("%s ","We have a device: CRW-VIII");
		break;
	default:
		outp("%s ","could not go here.");
		break;
	}
#endif
	return 0;
Err_free:
	usbreader_delete(usbreader);
	return retval;

}

//设备移除
static void WD_disconnect(struct usb_interface *udev)
{
	struct usbreader_state *usbreader=NULL;

	usbreader=usb_get_intfdata (udev);
	usb_set_intfdata (udev, NULL);
        
        if(usbreader->inEP == 1)
	  usb_deregister_dev (udev, &usbreader_class);

	down (&usbreader->sem);
	//设备移除时有两种情况:
	//没有打开的设备:	则删除对应的reader_state
	//有打开的设备:		usbreader_state->dev=NULL
	if (!usbreader->open_count) {
		//全部关闭了.
		up (&usbreader->sem);
		usbreader_delete (usbreader);
	} else {
		//还没有全部关闭时.
		usbreader->dev = NULL;
		up (&usbreader->sem);
	}
#ifdef DEBUG
	outp("%s","WD disconnect!");
#endif
	return;
}

//初始化,注册
static int __init WD_init(void)
{
	int i;
	int result;
	if ((result=usb_register(&usbreader_driver)) < 0){
#ifdef	DEBUG
		outp("%s result=%d","Can't register WD driver!",result);
#endif
		return -1;
	}
	for (i = 0; i < MAX_USBREADERS; i++) {
		minor_data[i]=NULL;
	}
#ifdef	DEBUG
	outp("%s ","WD init!");
#endif
	return 0;
}
//注销
static void __exit WD_cleanup(void)
{
	usb_deregister (&usbreader_driver);
#ifdef	DEBUG
	outp("%s ","WD cleanup!");
#endif
}

//===========================================================================
module_init (WD_init);
module_exit (WD_cleanup);
MODULE_AUTHOR("WatchData Technologies Pte. Ltd.");
MODULE_DESCRIPTION("USB Driver for WatchData Key");
MODULE_LICENSE("GPL");
