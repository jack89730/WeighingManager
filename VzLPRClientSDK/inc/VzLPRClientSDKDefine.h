//VZ车牌识别设备SDK的结构体等定义
#ifndef _VZ_LPR_CLIENT_SDK_DEFINE_H_
#define _VZ_LPR_CLIENT_SDK_DEFINE_H_

#include <VzClientSDK_CommonDefine.h>
#include <VzClientSDK_LPDefine.h>

/**设备句柄，由VzLPRClient_Open获得，设备操作相关的函数都需要*/
typedef int VzLPRClientHandle;

/**设置回调函数时需要指定的类型*/
typedef enum
{
	VZ_LPRC_CALLBACK_COMMON_NOTIFY,	/**<SDK通用信息反馈（预留）*/
	VZ_LPRC_CALLBACK_PLATE_STR,		/**<车牌号码字符（预留）*/
	VZ_LRPC_CALLBACK_FULL_IMAGE,	/**<完整图像*/
	VZ_LPRC_CALLBACK_CLIP_IMAGE,	/**<截取图像*/
	VZ_LPRC_CALLBACK_PLATE_RESULT,	/**<实时识别结果*/
	VZ_LPRC_CALLBACK_PLATE_RESULT_STABLE,	/**<稳定识别结果*/
	VZ_LPRC_CALLBACK_PLATE_RESULT_TRIGGER,	/**<触发的识别结果，包括API（软件）和IO（硬件）方式的*/
	VZ_LPRC_CALLBACK_VIDEO,			/**<视频帧回调*/
}
VZ_LPRC_CALLBACK_TYPE;

/**通用信息反馈类型*/
typedef enum
{
	VZ_LPRC_NO_ERR = 0,		/**<无错误*/
	VZ_LPRC_ACCESS_DENIED,	/**<用户名密码错误*/
	VZ_LPRC_NETWORK_ERR,	/**<网络连接故障*/
	VZ_LPRC_IO_INPUT = 100, /**<IO口输入信号*/
}
VZ_LPRC_COMMON_NOTIFY;

/**识别结果的类型*/
typedef enum
{
	VZ_LPRC_RESULT_REALTIME,		/**<实时识别结果*/
	VZ_LPRC_RESULT_STABLE,			/**<稳定识别结果*/
	VZ_LPRC_RESULT_FORCE_TRIGGER,	/**<调用“VzLPRClient_ForceTrigger”触发的识别结果*/
	VZ_LPRC_RESULT_IO_TRIGGER,		/**<外部IO信号触发的识别结果*/
	VZ_LPRC_RESULT_VLOOP_TRIGGER,	/**<虚拟线圈触发的识别结果*/
	VZ_LPRC_RESULT_MULTI_TRIGGER,	/**<由_FORCE_\_IO_\_VLOOP_中的一种或多种同时触发，具体需要根据每个识别结果的TH_PlateResult::uBitsTrigType来判断*/
	VZ_LPRC_RESULT_TYPE_NUM			/**<结果种类个数*/
}
VZ_LPRC_RESULT_TYPE;

/**顶点定义
  *X_1000和Y_1000的取值范围为[0, 1000]；
  *即位置信息为实际图像位置在整体图像位置的相对尺寸；
  *例如X_1000 = x*1000/win_width，其中x为点在图像中的水平像素位置，win_width为图像宽度
*/
typedef struct
{
	unsigned X_1000;	/**<水平方向相对坐标*/
	unsigned Y_1000;	/**<竖直方向相对坐标*/
}
VZ_LPRC_VERTEX;

/**穿越方向枚举*/
typedef enum
{
	VZ_LPRC_BIDIR,		/**双向触发*/
	VZ_LPRC_UP2DOWN,	/**从上到下方向的车辆触发*/
	VZ_LPRC_DOWN2UP,	/**从下到上方向的车辆触发*/
}
VZ_LPRC_DIR;
/**虚拟线圈名称*/
#define VZ_LPRC_VIRTUAL_LOOP_NAME_LEN	32

/**虚拟线圈顶点个数*/
#define VZ_LPRC_VIRTUAL_LOOP_VERTEX_NUM	4

/**虚拟线圈信息定义*/
typedef struct
{
	BYTE			byID;		/**<序号*/
	BYTE			byEnable;	/**<是否有效*/
	BYTE			byDraw;		/**<是否绘制*/
	BYTE			byRes[1];	/**<预留*/
	char			strName[VZ_LPRC_VIRTUAL_LOOP_NAME_LEN];			/**<自定义名称*/
	VZ_LPRC_VERTEX	struVertex[VZ_LPRC_VIRTUAL_LOOP_VERTEX_NUM];	/**<顶点数组*/
	VZ_LPRC_DIR		eCrossDir;	/**<穿越方向限制*/
	unsigned		uTriggerTimeGap;	/**对相同车牌的触发时间间隔的限制，单位为秒*/
	unsigned		uMaxLPWidth;		/**最大车牌尺寸限制*/
	unsigned		uMinLPWidth;		/**最小车牌尺寸限制*/
}
VZ_LPRC_VIRTUAL_LOOP;

/**最大虚拟线圈个数*/
#define VZ_LPRC_VIRTUAL_LOOP_MAX_NUM	8

/**虚拟线圈序列*/
typedef struct
{
	unsigned				uNumVirtualLoop;	/**<实际个数*/
	VZ_LPRC_VIRTUAL_LOOP	struLoop[VZ_LPRC_VIRTUAL_LOOP_MAX_NUM];	/**<虚拟线圈数组*/
}
VZ_LPRC_VIRTUAL_LOOPS;

/**省份字符串的最大尺寸*/
#define VZ_LPRC_PROVINCE_STR_LEN	128

/**预设省份信息*/
typedef struct
{
	char strProvinces[VZ_LPRC_PROVINCE_STR_LEN];	/**<所有支持的省份简称构成的字符串*/
	int nCurrIndex;	/**<当前的预设省份的序号，在strProvinces中的，-1为未设置*/
}
VZ_LPRC_PROVINCE_INFO;

/**图像信息*/
typedef struct
{
	unsigned		uWidth;			/**<宽度*/
	unsigned		uHeight;		/**<高度*/
	unsigned		uPitch;			/**<图像宽度的一行像素所占内存字节数*/
	unsigned		uPixFmt;		/**<图像像素格式，参考枚举定义图像格式（ImageFormatXXX）*/
	unsigned char	*pBuffer;	/**<图像内存的首地址*/
}
VZ_LPRC_IMAGE_INFO;

/**数据帧类型*/
typedef enum
{
	VZ_LPRC_DATA_ENC_VIDEO,		/**<视频帧编码数据*/
	VZ_LPRC_DATA_ENC_AUDIO,		/**<音频帧编码数据*/
}
VZ_LPRC_DATA_TYPE;

/**数据帧信息*/
typedef struct
{
	unsigned char	*pBuffer;	/**<数据内存的首地址*/
	unsigned		uDataSize;	/**<数据帧尺寸，字节数*/
	unsigned		uPTS_Sec;	/**<帧时间戳_秒*/
	unsigned		uPTS_USec;	/**<帧时间戳_微妙*/
	unsigned char	uIsKeyFrame;/**<关键帧标志位：1为关键帧，0为非关键帧*/
	unsigned char	reserved[7];	/**<预留*/
}
VZ_LPRC_DATA_INFO;

/**镜头变倍变焦命令*/
typedef enum
{
	VZ_LENS_OPT_STOP,		/**<停止调节*/
	VZ_LENS_FOCUS_FAR,		/**<往远处聚焦*/
	VZ_LENS_FOCUS_NEAR,		/**<往近处聚焦*/
	VZ_LENS_ZOOM_TELE,		/**<往长焦变倍*/
	VZ_LENS_ZOOM_WIDE,		/**<往短焦变倍*/
}
VZ_LENS_OPT;

/**LED补光灯命令*/
typedef enum
{
	VZ_LED_AUTO,		/**<自动控制LED的开和关*/
	VZ_LED_MANUAL_ON,	/**<手动控制LED开启*/
	VZ_LED_MANUAL_OFF,	/**<手动控制LED关闭*/
}
VZ_LED_CTRL;

/**用户私有数据的最大字节数*/
#define VZ_LPRC_USER_DATA_MAX_LEN	128

/**设备唯一序列号*/
typedef struct
{
	unsigned uHi;	/**<高位*/
	unsigned uLo;	/**<低位*/
}
VZ_DEV_SERIAL_NUM;

/**设备日期时间参数*/
typedef struct
{
	unsigned uYear;		/**<年*/
	unsigned uMonth;	/**<月 [1, 12]*/
	unsigned uMDay;		/**<月中的天数 [1, 31]*/
	unsigned uHour;		/**<时*/
	unsigned uMin;		/**<分*/
	unsigned uSec;		/**<秒*/
}
VZ_DATE_TIME_INFO;

/**串口参数*/
typedef struct
{
	unsigned uBaudRate;		/**<波特率*/
	unsigned uParity;		/**<校验位 其值为0-4=no,odd,even,mark,space*/
	unsigned uDataBits;		/**<数据位 其值为4-8=4,5,6,7,8 位数据位*/
	unsigned uStopBit;		/**<停止位 其值为0,1,2 = 1, 1.5, 2 位停止位*/
}
VZ_SERIAL_PARAMETER;

/**存储设备类型*/
typedef enum
{
	VZ_STORAGE_DEV_SD = 0,	/**<SD卡*/
	VZ_STORAGE_DEV_HD		/**<硬盘*/
}
VZ_STORAGE_DEV_TYPE;

/**存储设备状态*/
typedef enum
{
	VZ_STORAGE_DEV_NO_PART = 0, /**<未分区*/
	VZ_STORAGE_DEV_PART_OK,		/**<已分区*/
}
VZ_STORAGE_DEV_STATUS;

/**存储设备分区的工作状态*/
typedef enum
{
	VZ_STORAGE_DEV_PART_ERROR = 0,	/**<异常*/
	VZ_STORAGE_DEV_PART_NOT_FORMAT,	/**<未格式化*/
	VZ_STORAGE_DEV_PART_NOT_MOUNT,	/**<未挂载*/
	VZ_STORAGE_DEV_PART_FORMATING,	/**<正在格式化*/
	VZ_STORAGE_DEV_PART_DELETING,	/**<正在删除文件*/
	VZ_STORAGE_DEV_PART_NORMAL,		/**<正常工作状态*/
}
VZ_STORAGE_DEV_PART_STATUS;

/**存储设备分区信息*/
typedef struct
{
	VZ_STORAGE_DEV_PART_STATUS	eStatus;	/**<工作状态*/
	unsigned	uUsed;			/**<已用空间（MB)*/
	unsigned	uLeft;			/**<可用空间（MB)*/
	unsigned	uTotal;			/**<总容量（MB）*/
	int			nFormatPercent;	/**<格式化百分比*/
}
VZ_STORAGE_DEV_PART_INFO;

#define VZ_MAX_NUM_STORAGE_DEV_PART_NUM	4
/**单个存储设备信息*/
typedef struct
{
	char reserved[32];
	VZ_STORAGE_DEV_TYPE		eType;					/**<存储设备类型*/
	VZ_STORAGE_DEV_STATUS	eStatus;				/**<存储设备状态*/
	unsigned				uPartNum;				/**<分区个数*/
	VZ_STORAGE_DEV_PART_INFO	struPartInfo[VZ_MAX_NUM_STORAGE_DEV_PART_NUM];		/**<各分区信息*/
}
VZ_STORAGE_DEVICE_INFO;

#define VZ_MAX_NUM_STORAGE_DEVICE	4
/**存储设备信息*/
typedef struct
{
	int nDevCount;		/**<存储设备个数*/
	VZ_STORAGE_DEVICE_INFO	struSDI[VZ_MAX_NUM_STORAGE_DEVICE];	/**<各存储设备信息*/
}
VZ_STORAGE_DEVICES_INFO;

#define LPRC_OSDTEXTLEN				16

typedef struct
{
	unsigned char	dstampenable;					// 0 off 1 on
	int				dateFormat;						// 0:YYYY/MM/DD;1:MM/DD/YYYY;2:DD/MM/YYYY
	int				datePosX;
	int				datePosY;
	unsigned char	tstampenable;   				// 0 off 1 on
	int				timeFormat;						// 0:12Hrs;1:24Hrs
	int				timePosX;
	int				timePosY;
	unsigned char	nLogoEnable;					// 0 off 1 on
	int				nLogoPositionX;   				///<  logo position
	int				nLogoPositionY;   				///<  logo position
	unsigned char	nTextEnable;					///0 off 1 on
	int				nTextPositionX;   				///<  text position
	int				nTextPositionY;   				///<  text position
	char			overlaytext[LPRC_OSDTEXTLEN];	///user define text
}VZ_LPRC_OSD_Param;

#endif
