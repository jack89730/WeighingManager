#ifndef __CONTROLPANELPROTOCL_H__
#define __CONTROLPANELPROTOCL_H__

//FC2 = 0x40，DATA = 0x0e，显示：手工开闸（具有效卡功能）
//FC2 = 0x41，DATA = 0x0e，显示：手工落闸
int opendoor(unsigned char * data, int lenth);
int closedoor(unsigned char * data, int lenth);
//握手命令：FC1=0x6C，FC2=0x30，DATA：为两个字节填，
//0x30开头与接收指令的FC1为正确接收返回，
//两个0x31为接收错误返回。
//车到：FC1=0x63，FC2=0x30，DATA：0x0e
//车走：FC1=0x64，FC2=0x30，DATA：0x0e
int parseprotocol(unsigned char * data, int lenth);
#endif