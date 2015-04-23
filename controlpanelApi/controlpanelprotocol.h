#ifndef __CONTROLPANELPROTOCL_H__
#define __CONTROLPANELPROTOCL_H__

//FC2 = 0x40，DATA = 0x0e，显示：手工开闸（具有效卡功能）
//FC2 = 0x41，DATA = 0x0e，显示：手工落闸
int opendoor(unsigned char * data, int lenth);
int closedoor(unsigned char * data, int lenth);

#endif