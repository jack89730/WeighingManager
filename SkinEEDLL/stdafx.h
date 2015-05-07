// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

// 如果必须将位于下面指定平台之前的平台作为目标，请修改下列定义。
// 有关不同平台对应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以指定将 Windows Me 或更高版本作为目标。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。
#endif

//如果是WIN32 SDK编程即便您把头文件，库，命名空间都弄好了但是依然会一大串莫名其妙的错误
//只要把 stdafx.h 中的宏定义#define WIN32_LEAN_AND_MEAN把这一条给注释掉就可以了
//因为这一条的作用是：从 Windows 头中排除极少使用的资料
//而这极少使用的资料正是GDI＋需要的……

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
// Windows 头文件:
#include <windows.h>

//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的
//
//#include <atlbase.h>
//#include <atlstr.h>

// TODO: 在此处引用程序需要的其他头文件

#include <tchar.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <memory>
#include <sstream>
using namespace std;

#include <GdiPlus.h>
#pragma comment(lib,"GdiPlus.lib")
using namespace Gdiplus;

#include "import/tinyxml2.h"
using namespace tinyxml2;

