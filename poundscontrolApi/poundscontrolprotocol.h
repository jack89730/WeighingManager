#ifndef __POUNDSCONTROLPROTOCOL_H__
#define __POUNDSCONTROLPROTOCOL_H__

// 读取仪表状态信息:寄存器起始地址=0x21,寄存器数量=1
// 读取仪表显示重量值:寄存器起始地址=0x06,寄存器数量=4
int readstatus(unsigned char * data, int len);
int readweight(unsigned char * data, int len);

#endif
