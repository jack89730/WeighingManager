//用于接入VZ的车牌识别设备（包括智能识别一体机和智能识别终端等）的应用程序接口
#ifndef _VZ_LPRC_SDK_H_
#define _VZ_LPRC_SDK_H_
#include <VzLPRClientSDKDefine.h>

#ifdef _DEV_API_
#define VZ_LPRC_API extern "C" __declspec(dllexport)
#else
#define VZ_LPRC_API extern "C" __declspec(dllimport)
#endif

#define VZ_DEPRECATED __declspec(deprecated)

/**
* @defgroup group_global 全局操作函数
* @defgroup group_device 单个设备操作函数
* @defgroup group_callback 回调函数
*/

/**
*  @brief 全局初始化
*  @return 0表示成功，-1表示失败
*  @note 在所有接口调用之前调用
*  @ingroup group_global
*/
VZ_LPRC_API int __stdcall VzLPRClient_Setup();

/**
*  @brief 全局释放
*  @note 在程序结束时调用，释放SDK的资源
*  @ingroup group_global
*/
VZ_LPRC_API void __stdcall VzLPRClient_Cleanup();

/**
*  @brief 通过该回调函数获得找到的设备基本信息
*  @param  [IN] pStrDevName 设备名称
*  @param  [IN] pStrIPAddr	设备IP地址
*  @param  [IN] usPort1		设备端口号
*  @param  [IN] usPort2		预留
*  @param  [IN] pUserData	回调函数上下文
*  @ingroup group_callback
*/
typedef void (__stdcall *VZLPRC_FIND_DEVICE_CALLBACK)(const char *pStrDevName, const char *pStrIPAddr, WORD usPort1, WORD usPort2, void *pUserData);

/**
*  @brief 开始查找设备
*  @param  [IN] func 找到的设备通过该回调函数返回
*  @param  [IN] pUserData 回调函数中的上下文
*  @return 0表示成功，-1表示失败
*  @ingroup group_global
*/
VZ_LPRC_API int __stdcall VZLPRClient_StartFindDevice(VZLPRC_FIND_DEVICE_CALLBACK func, void *pUserData);

/**
*  @brief 停止查找设备
*  @ingroup group_global
*/
VZ_LPRC_API void __stdcall VZLPRClient_StopFindDevice();

/**
*  @brief 通过该回调函数获得设备的一般状态信息
*  @param  [IN] handle		由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pUserData	回调函数上下文
*  @param  [IN] eNotify		通用信息反馈类型
*  @param  [IN] pStrDetail	详细描述字符串
*  @ingroup group_callback
*/
typedef void (__stdcall *VZLPRC_COMMON_NOTIFY_CALLBACK)(VzLPRClientHandle handle, void *pUserData,
													   VZ_LPRC_COMMON_NOTIFY eNotify, const char *pStrDetail);

/**
*  @brief 设置设备连接反馈结果相关的回调函数
*  @param  [IN] func 设备连接结果和状态，通过该回调函数返回
*  @param  [IN] pUserData 回调函数中的上下文
*  @return 0表示成功，-1表示失败
*  @ingroup group_global
*/
VZ_LPRC_API int __stdcall VZLPRClient_SetCommonNotifyCallBack(VZLPRC_COMMON_NOTIFY_CALLBACK func, void *pUserData);

/**
*  @brief 打开一个设备
*  @param  [IN] pStrIP 设备的IP地址
*  @param  [IN] wPort 设备的端口号
*  @param  [IN] pStrUserName 访问设备所需用户名
*  @param  [IN] pStrPassword 访问设备所需密码
*  @return 返回设备的操作句柄，当打开失败时，返回0
*  @ingroup group_device
*/
VZ_LPRC_API VzLPRClientHandle __stdcall VzLPRClient_Open(const char *pStrIP, WORD wPort, const char *pStrUserName, const char *pStrPassword);

/**
*  @brief 关闭一个设备
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_Close(VzLPRClientHandle handle);

/**
*  @brief 通过IP地址关闭一个设备
*  @param  [IN] pStrIP 设备的IP地址
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_CloseByIP(const char *pStrIP);

/**
*  @brief 获取连接状态
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN/OUT] pStatus 输入获取状态的变量地址，输出内容为 1已连上，0未连上
*  @return 0表示成功，-1表示失败
*  @note   用户可以周期调用该函数来主动查询设备是否断线，以及断线后是否恢复连接
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_IsConnected(VzLPRClientHandle handle, BYTE *pStatus);

/**
*  @brief 播放实时视频
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] hWnd 窗口的句柄
*  @return 播放的句柄，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_StartRealPlay(VzLPRClientHandle handle, void *hWnd);

/**
*  @brief 停止播放指定的播放句柄
*  @param  [IN] nPlayHandle 播放的句柄
*  @return 0表示成功，-1表示失败
*  @note   可用来停止播放来自函数VzLPRClient_StartRealPlay和VzLPRClient_StartRealPlayFrameCallBack的播放句柄
*  @note   停止播放以后，该播放句柄就失效了
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_StopRealPlay(int nPlayHandle);

/**
*  @brief  通过该回调函数获得实时图像数据
*  @param  [IN] handle		由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pUserData	回调函数的上下文
*  @param  [IN] pFrame		图像帧信息，详见结构体定义VzYUV420P
*  @return 0表示成功，-1表示失败
*  @ingroup group_callback
*/
typedef void (__stdcall *VZLPRC_VIDEO_FRAME_CALLBACK)(VzLPRClientHandle handle, void *pUserData,
													  const VzYUV420P *pFrame);
/**
*  @brief 设置实时图像数据的回调函数
*  @param  [IN] handle		由VzLPRClient_Open函数获得的句柄
*  @param  [IN] func		实时图像数据函数
*  @param  [IN] pUserData	回调函数中的上下文
*  @return 0表示成功，-1表示失败
*  @note   DEPRECATED: 请转换为使用VzLPRClient_StartRealPlayFrameCallBack(handle, NULL, func, pUserData);
*  @ingroup group_device
*/
VZ_DEPRECATED VZ_LPRC_API int __stdcall VzLPRClient_SetVideoFrameCallBack(VzLPRClientHandle handle, VZLPRC_VIDEO_FRAME_CALLBACK func, void *pUserData);

/**
*  @brief  通过该回调函数获得实时图像数据
*  @param  [IN] handle		由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pUserData	回调函数的上下文
*  @param  [IN] pFrame		图像帧信息，详见结构体定义VZ_LPRC_IMAGE_INFO
*  @ingroup group_callback
*/
typedef void (__stdcall *VZLPRC_VIDEO_FRAME_CALLBACK_EX)(VzLPRClientHandle handle, void *pUserData,
														 const VZ_LPRC_IMAGE_INFO *pFrame);
/**
*  @brief 获取实时视频帧，图像数据通过回调函数到用户层，用户可改动图像内容，并且显示到窗口
*  @param  [IN] handle		由VzLPRClient_Open函数获得的句柄
*  @param  [IN] hWnd		窗口的句柄，如果为有效值，则视频图像会显示到该窗口上，如果为空，则不显示视频图像
*  @param  [IN] func		实时图像数据函数
*  @param  [IN] pUserData	回调函数中的上下文
*  @return 播放的句柄，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_StartRealPlayFrameCallBack(VzLPRClientHandle handle, void *hWnd,
												  VZLPRC_VIDEO_FRAME_CALLBACK_EX func, void *pUserData);

/**
*  @brief  通过该回调函数获得编码数据
*  @param  [IN] handle		由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pUserData	回调函数的上下文
*  @param  [IN] eDataType	数据类型，详见枚举VZ_LPRC_DATA_TYPE
*  @param  [IN] pData		数据帧信息，详见结构体定义VZ_LPRC_DATA_INFO
*  @ingroup group_callback
*/
typedef void (__stdcall *VZLPRC_VIDEO_DATA_CALLBACK)(VzLPRClientHandle handle, void *pUserData,
													 VZ_LPRC_DATA_TYPE eDataType, const VZ_LPRC_DATA_INFO *pData);

/**
*  @brief 获取实时编码数据
*  @param  [IN] handle		由VzLPRClient_Open函数获得的句柄
*  @param  [IN] func		回调函数
*  @param  [IN] pUserData	回调函数中的上下文
*  @return 播放的句柄，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetRealDataCallBack(VzLPRClientHandle handle,
												  VZLPRC_VIDEO_DATA_CALLBACK func, void *pUserData);

/**
*  @brief 通过该回调函数获得车牌识别信息
*  @param  [IN] handle			由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pUserData		回调函数的上下文
*  @param  [IN] pResult			车牌信息数组首地址，详见结构体定义 TH_PlateResult
*  @param  [IN] uNumPlates		车牌数组中的车牌个数
*  @param  [IN] eResultType		车牌识别结果类型，详见枚举类型定义VZ_LPRC_RESULT_TYPE
*  @param  [IN] pImgFull		当前帧的图像内容，详见结构体定义VZ_LPRC_IMAGE_INFO
*  @param  [IN] pImgPlateClip	当前帧中车牌区域的图像内容数组，其中的元素与车牌信息数组中的对应
*  @return 0表示成功，-1表示失败
*  @note   如果需要该回调函数返回截图内容 pImgFull 和pImgPlateClip，需要在设置回调函数（VzLPRClient_SetPlateInfoCallBack）时允许截图内容，否则该回调函数返回的这两个指针为空；
*  @note   实时结果（VZ_LPRC_RESULT_REALTIME）的回调是不带截图内容的
*  @ingroup group_callback
*/
typedef int (__stdcall *VZLPRC_PLATE_INFO_CALLBACK)(VzLPRClientHandle handle, void *pUserData,
													const TH_PlateResult *pResult, unsigned uNumPlates,
													VZ_LPRC_RESULT_TYPE eResultType,
													const VZ_LPRC_IMAGE_INFO *pImgFull,
													const VZ_LPRC_IMAGE_INFO *pImgPlateClip);

/**
*  @brief 设置识别结果的回调函数
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] func 识别结果回调函数，如果为NULL，则表示关闭该回调函数的功能
*  @param  [IN] pUserData 回调函数中的上下文
*  @param  [IN] bEnableImage 指定识别结果的回调是否需要包含截图信息：1为需要，0为不需要
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetPlateInfoCallBack(VzLPRClientHandle handle, VZLPRC_PLATE_INFO_CALLBACK func, void *pUserData, int bEnableImage);


/**
*  @brief 设置脱机结果的回调函数
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] func 识别结果回调函数，如果为NULL，则表示关闭该回调函数的功能
*  @param  [IN] pUserData 回调函数中的上下文
*  @param  [IN] bEnableImage 指定识别结果的回调是否需要包含截图信息：1为需要，0为不需要
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetOfflinePlateInfoCallBack(VzLPRClientHandle handle, VZLPRC_PLATE_INFO_CALLBACK func, void *pUserData, int bEnableImage);


/**
*  @brief 发送软件触发信号，强制处理当前时刻的数据并输出结果
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @return 0表示成功，-1表示失败
*  @note   车牌识别结果通过回调函数的方式返回，如果当前视频画面中无车牌，则回调函数不会被调用
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_ForceTrigger(VzLPRClientHandle handle);

/**
*  @brief 设置虚拟线圈
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pVirtualLoops 虚拟线圈的结构体指针
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetVirtualLoop(VzLPRClientHandle handle, const VZ_LPRC_VIRTUAL_LOOPS *pVirtualLoops);

/**
*  @brief 获取已设置的虚拟线圈
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pVirtualLoops 虚拟线圈的结构体指针
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetVirtualLoop(VzLPRClientHandle handle,  VZ_LPRC_VIRTUAL_LOOPS *pVirtualLoops);

/**
*  @brief 获取已设置的预设省份
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pProvInfo 预设省份信息指针
*  @return 0表示成功，非0表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetSupportedProvinces(VzLPRClientHandle handle, VZ_LPRC_PROVINCE_INFO *pProvInfo);

/**
*  @brief 设置预设省份
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] nIndex 设置预设省份的序号，序号需要参考VZ_LPRC_PROVINCE_INFO::strProvinces中的顺序，从0开始，如果小于0，则表示不设置预设省份
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_PresetProvinceIndex(VzLPRClientHandle handle, int nIndex);

/**
*  @brief 将图像保存为JPEG到指定路径
*  @param  [IN] pImgInfo 图像结构体，目前只支持默认的格式，即ImageFormatRGB
*  @param  [IN] pFullPathName 设带绝对路径和JPG后缀名的文件名字符串
*  @param  [IN] nQuality JPEG压缩的质量，取值范围1~100；
*  @return 0表示成功，-1表示失败
*  @note   给定的文件名中的路径需要存在
*  @ingroup group_global
*/
VZ_LPRC_API int __stdcall VzLPRClient_ImageSaveToJpeg(const VZ_LPRC_IMAGE_INFO *pImgInfo, const char *pFullPathName, int nQuality);

/**
*  @brief 将图像编码为JPEG，保存到指定内存
*  @param  [IN] pImgInfo 图像结构体，目前只支持默认的格式，即ImageFormatRGB
*  @param  [IN/OUT] pDstBuf JPEG数据的目的存储首地址
*  @param  [IN] uSizeBuf JPEG数据地址的内存的最大尺寸；
*  @param  [IN] nQuality JPEG压缩的质量，取值范围1~100；
*  @return >0表示成功，即编码后的尺寸，-1表示失败，-2表示给定的压缩数据的内存尺寸不够大
*  @ingroup group_global
*/
VZ_LPRC_API int __stdcall VzLPRClient_ImageEncodeToJpeg(const VZ_LPRC_IMAGE_INFO *pImgInfo, void *pDstBuf, unsigned uSizeBuf, int nQuality);
/**
*  @brief 保存正在播放的视频的当前帧的截图到指定路径
*  @param  [IN] hWnd 正在播放视频的窗口的句柄
*  @param  [IN] pFullPathName 设带绝对路径和JPG后缀名的文件名字符串
*  @param  [IN] nQuality JPEG压缩的质量，取值范围1~100；
*  @return 0表示成功，-1表示失败
*  @note   前提条件是指定的hWnd的窗口已经调用了VzLPRClient_StartRealPlay播放实时视频
*  @note   使用的文件名中的路径需要存在
*  @note   DEPRECATED: 请转为使用VzLPRClient_GetSnapShootToJpeg2
*  @ingroup group_device
*/
VZ_DEPRECATED VZ_LPRC_API int __stdcall VzLPRClient_GetSnapShootToJpeg(void *hWnd, const char *pFullPathName, int nQuality);

/**
*  @brief 保存正在播放的视频的当前帧的截图到指定路径
*  @param  [IN] nPlayHandle 播放的句柄
*  @param  [IN] pFullPathName 设带绝对路径和JPG后缀名的文件名字符串
*  @param  [IN] nQuality JPEG压缩的质量，取值范围1~100；
*  @return 0表示成功，-1表示失败
*  @note   使用的文件名中的路径需要存在
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetSnapShootToJpeg2(int nPlayHandle, const char *pFullPathName, int nQuality);

/**
*  @brief 获取IO输出的状态
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] uChnId IO输出的通道号，从0开始
*  @param  [OUT] pOutput IO输出的状态，0表示继电器开路，1表示继电器闭路
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetIOOutput(VzLPRClientHandle handle, unsigned uChnId, int *pOutput);

/**
*  @brief 设置IO输出的状态
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] uChnId IO输出的通道号，从0开始
*  @param  [OUT] nOutput 将要设置的IO输出的状态，0表示继电器开路，1表示继电器闭路
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetIOOutput(VzLPRClientHandle handle, unsigned uChnId, int nOutput);

/**
*  @brief 设置串口参数
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] nSerialPort 指定使用设备的串口序号：0表示第一个串口，1表示第二个串口
*  @param  [IN] pParameter 将要设置的串口参数，详见定义 VZ_SERIAL_PARAMETER
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetSerialParameter(VzLPRClientHandle handle, int nSerialPort,
														 const VZ_SERIAL_PARAMETER *pParameter);

/**
*  @brief 获取串口参数
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] nSerialPort 指定使用设备的串口序号：0表示第一个串口，1表示第二个串口
*  @param  [OUT] pParameter 将要获取的串口参数，详见定义 VZ_SERIAL_PARAMETER
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetSerialParameter(VzLPRClientHandle handle, int nSerialPort,
														 VZ_SERIAL_PARAMETER *pParameter);
/**
*  @brief 通过该回调函数获得透明通道接收的数据
*  @param  [IN] nSerialHandle VzLPRClient_SerialStart返回的句柄
*  @param  [IN] pRecvData	接收的数据的首地址
*  @param  [IN] uRecvSize	接收的数据的尺寸
*  @param  [IN] pUserData	回调函数上下文
*  @ingroup group_callback
*/
typedef void (__stdcall *VZDEV_SERIAL_RECV_DATA_CALLBACK)(int nSerialHandle, const unsigned char *pRecvData, unsigned uRecvSize, void *pUserData);

/**
*  @brief 开启透明通道
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] nSerialPort 指定使用设备的串口序号：0表示第一个串口，1表示第二个串口
*  @param  [IN] func 接收数据的回调函数
*  @param  [IN] pUserData 接收数据回调函数的上下文
*  @return 返回透明通道句柄，0表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SerialStart(VzLPRClientHandle handle, int nSerialPort,
												  VZDEV_SERIAL_RECV_DATA_CALLBACK func, void *pUserData);
/**
*  @brief 透明通道发送数据
*  @param [IN] nSerialHandle 由VzLPRClient_SerialStart函数获得的句柄
*  @param [IN] pData 将要传输的数据块的首地址
*  @param [IN] uSizeData 将要传输的数据块的字节数
*  @return 0表示成功，其他值表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SerialSend(int nSerialHandle, const unsigned char *pData, unsigned uSizeData);

/**
*  @brief 透明通道停止发送数据
*  @param [IN] nSerialHandle 由VzLPRClient_SerialStart函数获得的句柄
*  @return 0表示成功，其他值表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SerialStop(int nSerialHandle);

/**
*  @brief 调整设备镜头的变倍和聚焦
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [IN] eOPT 操作类型，详见定义VZ_LENS_OPT
*  @return 0表示成功，其他值表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_CtrlLens(VzLPRClientHandle handle, VZ_LENS_OPT eOPT);

/**
*  @brief 获取LED当前亮度等级和最大亮度等级
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [OUT] pLevelNow 用于输出当前亮度等级的地址
*  @param [OUT] pLevelMax 用于输出最高亮度等级的地址
*  @return 0表示成功，其他值表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetLEDLightStatus(VzLPRClientHandle handle, int *pLevelNow, int *pLevelMax);

/**
*  @brief 设置LED亮度等级
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [IN] nLevel，LED亮度等级
*  @return 0表示成功，其他值表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetLEDLightLevel(VzLPRClientHandle handle, int nLevel);

/**
*  @brief 获取LED当前控制模式
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [OUT] pCtrl 用于输出当前LED开关控制模式的地址，详见定义 VZ_LED_CTRL
*  @return 0表示成功，其他值表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetLEDLightControlMode(VzLPRClientHandle handle, VZ_LED_CTRL *pCtrl);

/**
*  @brief 设置LED控制模式
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [IN] eCtrl 控制LED开关模式，详见定义 VZ_LED_CTRL
*  @return 返回值为0表示成功，返回其他值表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetLEDLightControlMode(VzLPRClientHandle handle, VZ_LED_CTRL eCtrl);

/**
*  @brief 写入用户私有数据，可用于二次加密
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [IN] pUserData 用户数据
*  @param [IN] uSizeData 用户数据的长度，最大128字节
*  @return 返回值为0表示成功，返回其他值表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_WriteUserData(VzLPRClientHandle handle, const unsigned char *pUserData, unsigned uSizeData);

/**
*  @brief 读出用户私有数据，可用于二次加密
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [IN/OUT] pBuffer 用于存放读到的用户数据
*  @param [IN] uSizeBuf 用户数据缓冲区的最小尺寸，不小于128字节
*  @return 返回值为实际用户数据的字节数，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_ReadUserData(VzLPRClientHandle handle, unsigned char *pBuffer, unsigned uSizeBuf);

/**
*  @brief 读出设备序列号，可用于二次加密
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [IN/OUT] pSN 用于存放读到的设备序列号，详见定义 VZ_DEV_SERIAL_NUM
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetSerialNumber(VzLPRClientHandle handle, VZ_DEV_SERIAL_NUM *pSN);

/**
*  @brief 获取设备的日期时间
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [IN/OUT] pDTInfo 用于存放获取到的设备日期时间信息，详见定义 VZ_DATE_TIME_INFO
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetDateTime(VzLPRClientHandle handle, VZ_DATE_TIME_INFO *pDTInfo);

/**
*  @brief 设置设备的日期时间
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [IN] pDTInfo 将要设置的设备日期时间信息，详见定义 VZ_DATE_TIME_INFO
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetDateTime(VzLPRClientHandle handle, const VZ_DATE_TIME_INFO *pDTInfo);

/**
*  @brief 获取设备的存储信息
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [IN/OUT] pDTInfo 用于存放获取到的设备日期时间信息，详见定义 VZ_DATE_TIME_INFO
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetStorageDeviceInfo(VzLPRClientHandle handle, VZ_STORAGE_DEVICES_INFO *pSDInfo);

/**
*  @brief 开始录像功能
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param [IN] sFileName 录像文件的路径
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SaveRealData(VzLPRClientHandle handle,char *sFileName);

/**
*  @brief 停止录像
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_StopSaveRealData(VzLPRClientHandle handle);

/**
*  @brief 设置视频OSD参数；
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetOsdParam(VzLPRClientHandle handle, VZ_LPRC_OSD_Param *pParam);

/**
*  @brief 获取视频OSD参数；
*  @param [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetOsdParam(VzLPRClientHandle handle, VZ_LPRC_OSD_Param *pParam);

/**
*  @brief 开启脱机功能
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pUserData 接收数据回调函数的上下文
*   @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetOfflineCheck(VzLPRClientHandle handle);

/**
*  @brief 关闭脱机功能
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pUserData 接收数据回调函数的上下文
*   @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_CancelOfflineCheck(VzLPRClientHandle handle);

/**
*  @brief 根据ID查询记录
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] nID 车牌记录在数据中的ID
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_QueryPlateRecordByID(VzLPRClientHandle handle, int nID);

/**
*  @brief 根据起始时间和车牌关键字查询记录
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pStartTime 起始时间，格式如"2015-01-02 12:20:30"
*  @param  [IN] pEndTime   起始时间，格式如"2015-01-02 19:20:30"
*  @param  [IN] keyword    车牌号关键字, 如"川"
*  @return 返回值为0表示成功，返回-1表示失败
*  @说明   通过回调返回数据，最多返回100条数据，超过时请调用分页查询的接口
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_QueryRecordByTimeAndPlate(VzLPRClientHandle handle, const char *pStartTime, const char *pEndTime, const char *keyword);


/**
*  @brief 根据时间和车牌号查询记录条数
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pStartTime 起始时间，格式如"2015-01-02 12:20:30"
*  @param  [IN] pEndTime   起始时间，格式如"2015-01-02 19:20:30"
*  @param  [IN] keyword    车牌号关键字, 如"川"
*  @return 返回值为0表示失败，大于0表示记录条数
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_QueryCountByTimeAndPlate(VzLPRClientHandle handle, const char *pStartTime, const char *pEndTime, const char *keyword);

/**
*  @brief 根据时间和车牌号查询分页记录
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pStartTime 起始时间，格式如"2015-01-02 12:20:30"
*  @param  [IN] pEndTime   起始时间，格式如"2015-01-02 19:20:30"
*  @param  [IN] keyword    车牌号关键字, 如"川"
*  @param  [IN] start      起始位置大于0,小于结束位置
*  @param  [IN] end        结束位置大于0,大于起始位置，获取记录条数不能大于100
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_QueryPageRecordByTimeAndPlate(VzLPRClientHandle handle, const char *pStartTime, const char *pEndTime, const char *keyword, int start, int end);


/**
*  @brief 设置查询车牌记录的回调函数
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] func 识别结果回调函数，如果为NULL，则表示关闭该回调函数的功能
*  @param  [IN] pUserData 回调函数中的上下文
*  @param  [IN] bEnableImage 指定识别结果的回调是否需要包含截图信息：1为需要，0为不需要
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_SetQueryPlateCallBack(VzLPRClientHandle handle, VZLPRC_PLATE_INFO_CALLBACK func, void *pUserData);


/**
*  @brief 根据ID获取车牌图片
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] id     车牌记录的ID
*  @param  [IN] pdata  存储图片的内存
*  @param  [IN][OUT] size 为传入传出值，传入为图片内存的大小，返回的是获取到jpg图片内存的大小
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_LoadImageById(VzLPRClientHandle handle, int id, void *pdata, int* size);

/**
*  @brief 获取视频的宽度和高度
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [OUT] pWidth		视频图像宽度变量的地址
*  @param  [OUT] pHeight	视频图像高度变量的地址
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetVideoFrameSize(VzLPRClientHandle handle, int *pWidth, int *pHeight);

/**
*  @brief 获取视频的编码方式
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [OUT] pEncType	返回的编码方式, 0:H264  1:MPEG4  2:JPEG  其他:错误
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetVideoEncodeType(VzLPRClientHandle handle, int *pEncType);

/**
*  @brief 获取GPIO的状态
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] gpioIn 数据为0或1
*  @param  [OUT] value 0代表短路，1代表开路
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_GetGPIOValue(VzLPRClientHandle handle, int gpioIn, int *value);

/**
*  @brief 根据时间查询脱机记录条数
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pStartTime 起始时间，格式如"2015-01-02 12:20:30"
*  @param  [IN] pEndTime   起始时间，格式如"2015-01-02 19:20:30"
*  @return 返回值为0表示失败，大于0表示记录条数
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_QueryOfflineCountByTime(VzLPRClientHandle handle, const char *pStartTime, const char *pEndTime);

/**
*  @brief 根据时间和车牌号分页查询脱机记录
*  @param  [IN] handle 由VzLPRClient_Open函数获得的句柄
*  @param  [IN] pStartTime 起始时间，格式如"2015-01-02 12:20:30"
*  @param  [IN] pEndTime   起始时间，格式如"2015-01-02 19:20:30"
*  @param  [IN] start      起始位置大于0,小于结束位置
*  @param  [IN] end        结束位置大于0,大于起始位置，获取记录条数不能大于100
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_API int __stdcall VzLPRClient_QueryPageOfflineRecordByTime(VzLPRClientHandle handle, const char *pStartTime, const char *pEndTime, int start, int end);

#endif
