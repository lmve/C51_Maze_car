//红外接口定义
sfr P4 = 0xe8;

//红外地址接口定义
sbit A0 = P4^0;
sbit A1 = P2^0;
sbit A2 = P2^7;

//红外传感器接收信号口定义
sbit irR1 = P2^1;//前（左）
sbit irR2 = P2^2;//左前
sbit irR3 = P2^3;//左
sbit irR4 = P2^4;//右
sbit irR5 = P2^5;//右前


//红外传感器检测状态全局位变量，0表示无障碍
unsigned char irC=0, irL=0, irR=0, irLU=0, irRU=0;





