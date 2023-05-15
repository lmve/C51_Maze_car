#include <reg52.h>
#include "mouse.h"
#define  uchar unsigned char
#define  uint unsigned int
#define N 8
/* * * * * * * * * * * * * * * * * 
 *控制引脚申明
 */
sbit tube1 = P4^3;
sbit tube2 = P4^2;
sbit beep = P3^7;
/* * * * * * * * * * * * * * * * * *
 *数据单元
 */
//原地转圈
uchar code run1[] = {0x11,0x33,0x22,0x66,0x44,0xcc,0x88,0x99,0x11};
//直行
uchar code run2[] = {0x11,0x39,0x28,0x6c,0x44,0xc6,0x82,0x93,0x11};
//数码管
uchar code table[] = {0xc0,0xcf,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};
//地图
uchar map[8][8] = {0};
//位置坐标
uchar location[2] = {7,0};
//岔路口信息
uchar stack[17][2] = {0};
//char finall[17] = {0 ,0, 1, 1, 0 , 0 , 1 , 1 , 1 , 0 , 1 , 1 , 0 , 0 };//测试路径数据
char top = -1;
uchar direction = 0;
char head,rear,top1 = -1 ;
unsigned char  b[6][2] ;//队列b，和记录路的座标的结构体数组
char way[17] = {0};//路径数据 
void recall();//回溯 
void voice();
uchar pass(uchar way);
void init(){//初始化队列
    head = 0;
    rear = 0;
}
void pop(){//弹出队列成员
    char i;
    for(i = 1;i <= rear; i++)
    {
    	b[i-1][0] = b[i][0];
    	b[i-1][1] = b[i][1];
    }
    rear --;
}
void record(char x ,char y){//将走过的点信息加入队列，next为指向
//上一个节点数据的指针
    b[rear][0] = x;
    b[rear][1] = y;
    rear ++;
}

/*
 *延时1ms
 */
void delayms(uint n)
{
	uchar i,j;
	for(j=n;j>0;j--)
	for(i=112;i>0;i--);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *移动单元
 * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *直行
 */
void go_straight()
{
//	uchar j,i;
//	for(j=0;j<106;j++)//一圈
//	{
//		for(i=8;i>0;i--)
//		{
//			P1 = run2[i];
//			delayms(2);
//		}
//	}
	uchar i,j,k,l = 21;
	while(l--){
		for(i=0;i<5;i++)
		{
			for(j=8;j>0;j--)
			{
				P1 = run2[j];
				delayms(3);
			}
		}
			if((irLU==1) && (irC == 0))
			{
				//voice();
				  for(k = 0 ;k < 3; k++)
				{
					for(j=0;j<8;j++)
					{
						P1 = run1[j];
						delayms(3);
					}
				}
				
			}
			if((irRU==1) && (irC == 0))
			{   
				//voice();
				  for(k = 0;k < 3; k++)
				{
					for(j=8;j>0;j--)
					{
						P1 = run1[j];
						delayms(3);
					}
				}
			}
		}
}

/*
 *掉头
 */
void back()
{
	uchar i,j;
	for(i=0;i<102;i++)
	{
		for(j=0;j<8;j++)
		{
			P1 = run1[j];
			delayms(2);
		}
	}
	direction = (direction + 2) % 4;
}

/*
 *右转
 */
void turn_right()
{
	uchar i,j;
	for(i=0;i<52;i++)
	{
		for(j=0;j<8;j++)
		{
			P1 = run1[j];
			delayms(2);
		}
	}
	direction = (direction + 1) % 4;
}

/*
 *左转
 */
void turn_left()
{
	uchar i,j;
	for(i=0;i<52;i++)
	{
		for(j=8;j>0;j--)
		{
			P1 = run1[j];
			delayms(2);
		}
	}
	direction = (direction + 3) % 4;
}
/*
 *冲刺
 */
void run_fast()
{
		uchar i,j,k,l = 20;
	while(l--){
		for(i=0;i<5;i++)
		{
			for(j=8;j>0;j--)
			{
				P1 = run2[j];
				delayms(2);
			}
		}
			if((irLU==1) && (irC == 0))
			{
					
				  for(k = 0 ;k < 3; k++)
				{
					for(j=0;j<8;j++)
					{
						P1 = run1[j];
						delayms(2);
					}
				}
				
			}
			if((irRU==1) && (irC == 0))
			{   
					
				  for(k = 0;k < 3; k++)
				{
					for(j=8;j>0;j--)
					{
						P1 = run1[j];
						delayms(2);
					}
				}
			}
		}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *外设单元
 * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *蜂鸣器
 */
void voice()
{
	  uchar j = 0;
		for(j = 10;j>0;j--){
			beep = ~beep;
			delayms(10);
		}
		for(j = 100;j>0;j--){
			beep = 1;
			delayms(10);
		}
}

/*
 *数码管
 */
void display(uchar num)
{
	uchar a,b;
	a = num / 10;
	b = num % 10;
	
	tube2 = 0;
	tube1 = 1;
	P0 = table[a];
	delayms(2);
	tube1 = 0;
	
	tube1 = 0;
	tube2 = 1;
	P0 = table[b];
	delayms(2);
	tube2 = 0;
}
////行，列 显示
void now_x(uchar num)
{
	tube2 = 0;
	tube1 = 1;
	P0 = table[num];
	delayms(2);
	tube1 = 0;
}
void now_y(uchar num)
{
	tube1 = 0;
	tube2 = 1;
	P0 = table[num];
	delayms(2);
	tube2 = 0;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *传感器单元
 * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *红外发射
 */
void mouse_ir_on(uchar group_no)
{
	A0 = (group_no) & 0x01;
	A1 = (group_no) & 0x02;
	A2 = (group_no) & 0x04;
}

/*
 *设置T2自动重装载寄存器和计数器
 */
void setTime2(uint us)
{
	TH2 = (65536 - us) % 256;
	RCAP2H = (65536 - us) / 256;
	TL2 = (65536 -us) % 256;
	RCAP2L = (65536 - us) % 256;
}
/*
 *定时计数器初始化
 */
void initTime2()
{
	EA = 1;
	ET2 = 1;
	setTime2(5000);//设置5ms中断一次
	TR2 = 1;
}
/*
 *避障测试
 */
void test()
{
	if(!irR)
	{
		turn_right();
		delayms(500);
		go_straight();
		delayms(1000);
	}
	else if(!irC)
	{
		go_straight();
		delayms(500);
		delayms(1000);
	}
	else if(!irL)
	{
		turn_left();
		delayms(500);
		go_straight();
		delayms(1000);
	}
	else
	{
		back();
		delayms(500);
		go_straight();
		delayms(500);
		voice();
		delayms(1000);
	}
	  
}
void ir_test(){ // 测试某个方向的红外
   
        if(irL||irR||irC){
            beep = 0;
        }else{
            beep = 1;
        }
   
}
/* * * * * * * * * * * * * * * 
 * 第一部分 遍历迷宫
 * 地图初始化
 */
void init_map()
{
	uchar i,j;
	//入栈
	top++;
	stack[top][0] = location[0];
	stack[top][1] = location[1];
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			map[i][j] = 0xff;
		}
	}
}
	
 /*
 *记录当前位置信息
 */
void save()
{ 	unsigned char temp = 0,count = 0,temp1 = 0;
		switch(direction)//地图信息的记录* * * * 初次进入此单元格
		{
			case 0:temp+=irC; temp<<=1 ; temp+=irR; temp<<=2; temp+=irL; map[location[0]][location[1]] &= (0xf0+temp); map[location[0]][location[1]] &= 0xdf; break;
			case 1:temp+=irL; temp<<=1 ; temp+=irC; temp<<=1; temp+=irR; temp<<=1; map[location[0]][location[1]] &= (0xf0+temp); map[location[0]][location[1]] &= 0xef; break;
			case 2:temp+=irL; temp<<=1 ; temp+=irC; temp<<=1; temp+=irR; map[location[0]][location[1]] &= (0xf0+temp); map[location[0]][location[1]] &= 0x7f; break;
			case 3:temp+=irR; temp<<=2 ; temp+=irL; temp<<=1; temp+=irC; map[location[0]][location[1]] &= (0xf0+temp); map[location[0]][location[1]] &= 0xbf; break;
		}
		if(location[0] == 7 && location[1] == 0)
		map[7][0] = 0xf7;
		//判断是否为岔路口
		if((!irC) && pass(0))
			count++;
		if((!irR) && pass(1))
			count++;
		if((!irL) && pass(3))
			count++;
		if(count>=2)//记录岔路口信息
		{
			top++;
			stack[top][0] = location[0];
			stack[top][1] = location[1];
		}
}	
/*
 *检查当前要走的方向是否走过
 *走过为0，否则为1
 */
uchar pass(uchar way)
{
		unsigned char x1,y1;
	  x1 = location[0];
	  y1 = location[1];
		switch(direction)
		{
			case 0: 
			{
				switch(way)
				{
					case 0: if(map[x1 - 1][y1] == 0xff) return 1;  return 0;//上 
					case 1: if(map[x1][y1 + 1] == 0xff) return 1;  return 0;//右 
					case 3: if(map[x1][y1 - 1] == 0xff) return 1;  return 0;//左 
				}
				
			}
			case 1: 
			{
				switch(way)
				{
					case 0: if(map[x1][y1+1] == 0xff) return 1;  return 0;//右
					case 1: if(map[x1+1][y1] == 0xff) return 1;  return 0;//下
					case 3: if(map[x1-1][y1] == 0xff) return 1;  return 0;//上 
				}
			}
			case 2: 
			{
				switch(way)
				{
					case 0: if(map[x1+1][y1] == 0xff) return 1;  return 0;//下 
					case 1: if(map[x1][y1-1] == 0xff) return 1;  return 0;//左
					case 3: if(map[x1][y1+1] == 0xff) return 1;  return 0;//右 
				}
			}
			case 3: 
			{
				switch(way)
				{
					case 0: if(map[x1][y1-1] == 0xff) return 1; return 0;//左 
					case 1: if(map[x1-1][y1] == 0xff) return 1; return 0;//上
					case 3: if(map[x1+1][y1] == 0xff) return 1; return 0;//下 
				}
			}
		}
}
/*
 * * 更新当前位置信息
 * 上 右 下 左 0 1 2 3
 */
void move()
{
	switch(direction)
	{
		case 0: {
			location[0] -= 1;
			break;
		}
		case 1:{
		  location[1] += 1;
		  break;
	  }
		case 2:{
			location[0] += 1;
			break;
		}
		case 3:{
			location[1] -= 1;
			break;
		}
	}
}	
/*
 * 回溯
 */
void recall()
{
	unsigned char temp = 0,count = 0,temp1 = 0,i = 0; 
	temp = map[location[0]][location[1]];
	while(1)
	{
		temp = map[location[0]][location[1]];
		if(!((temp >>= 4) & 0x01))//左
		{
			
			switch(direction)
			{
				case 0: turn_left(); delayms(100); go_straight();delayms(100); break;
				case 1: back();delayms(100);go_straight();delayms(100); break;
				case 2: turn_right(); delayms(100); go_straight();delayms(100); break;
				case 3: go_straight(); delayms(100); break;
			}
			move(); 
			temp = map[location[0]][location[1]];
		}
		//显示当前坐标
	  now_x(location[0]);
	  now_y(location[1]);
		if(location[0] == stack[top][0] && location[1] == stack[top][1])
			break;
	  	
		
		temp = map[location[0]][location[1]];
		if(!((temp >>= 5) & 0x01))//下
		{
			switch(direction)
			{
				case 0: back();delayms(100);go_straight();delayms(100); break;
				case 1: turn_right(); delayms(100);go_straight();delayms(100);break;
				case 2: go_straight(); delayms(100); break;
				case 3: turn_left();delayms(100);go_straight();delayms(100);break;
			}
			move();
			temp = map[location[0]][location[1]];
		}
		//显示当前坐标
	  now_x(location[0]);
	  now_y(location[1]);
		if(location[0] == stack[top][0] && location[1] == stack[top][1])
			break;
		
		temp = map[location[0]][location[1]];
		if(!((temp >>= 6) & 0x01))//右
		{
			switch(direction)
			{
				case 0: turn_right();delayms(100);go_straight();delayms(100);break;
				case 1: go_straight();delayms(100); break;
				case 2: turn_left();delayms(100);go_straight();delayms(100); break;
				case 3: back();delayms(100);go_straight();delayms(100); break;
			}
			 move();
			temp = map[location[0]][location[1]];
		}
		//显示当前坐标
	  now_x(location[0]);
	  now_y(location[1]);
		if(location[0] == stack[top][0] && location[1] == stack[top][1])
			break;
		
		temp = map[location[0]][location[1]];
		if(!((temp >>= 7) & 0x01))//上
		{
			switch(direction)
			{
				case 0: go_straight(); delayms(100); break;
				case 1: turn_left();delayms(100);go_straight();delayms(100);  break;
				case 2: back();delayms(100);go_straight();delayms(100);  break;
				case 3: turn_right();delayms(100); go_straight();delayms(100);break;
			}
			move(); 
			temp = map[location[0]][location[1]];
		}
		//显示当前坐标
	  now_x(location[0]);
	  now_y(location[1]);
		if(location[0] == stack[top][0] && location[1] == stack[top][1])
			break;
	}
	top--;//弹栈 
}
/* 
 *遍历迷宫
 */
void find()
{
	  //显示当前坐标
	  now_x(location[0]);
	  now_y(location[1]); 
	  //等待检测
//	  delayms(2);
	  //判断当前位置是否为第一次到达
	  if(map[location[0]][location[1]] == 0xff)
	  save();
	  
		  if(irR==0&&pass(1))//右边无挡板且未走过
			{
				turn_right();
				delayms(100);
				go_straight();
				delayms(500);
				move();
			}
			else if(irC==0&&pass(0))//右边有挡板，前边无且未走过
			{
				go_straight();
				delayms(500);
				move();
			}
			else if(irL==0&&pass(3))//右边有挡板前边有，左边无
			{
				turn_left();
				delayms(100);
				go_straight();
				delayms(500);
				move();
			}
			else//死胡同
			{
				recall();
			}

}
/* * * * * * * * * * * * * * 
 * 第二部分 寻找最优路径
 * 地图设置
 */
void set_map()
{
	unsigned char i,j,temp=0;
	for(i = 0; i < N ; i++)
	{
		for(j = 0;j < N; j++)
		{
			temp = map[i][j];
			temp &= 0x0f;
			temp += 0x10; 
			map[i][j] = temp;
		}
	}
} 
/*
 * 开始寻路
 */
void go_map(){//遍历地图
     char  count = 1, num = 1, loop = 1, first = 0, temp1 = 0, flag = 0;
    unsigned char temp[2] = {0} ;
    record(7,0);//起点坐标.............. 
    while(1){
        if(head >=rear )return ;//当队头和队尾相等时，退出
        temp[0] = b[0][0];
        temp[1] = b[0][1];
        pop();

        if( temp[0] == 0 && temp[1] == 7){//当走到终点时，用way 数组又保存返回到起点的路径
			map[temp[0]][temp[1]] = num++;
            return ;
        }
         temp1 = map[temp[0]][temp[1]];
         flag = 0;
        //左方 
        if((!(temp1 &= 0x01)) && map[temp[0]][temp[1]-1] >> 4)//遍历四个方向，只要满足条件，//就加入队列
        {
            record(temp[0] , temp[1] -1 ); 
            flag += 1;
        }
        //下方
		 temp1 = map[temp[0]][temp[1]]; 
         temp1 >>= 1;
        if((!(temp1 &= 0x01)) && map[temp[0]+1][temp[1]] >>4){
            record(temp[0] + 1 ,temp[1] ); 
            flag += 1;
        }
        //右方
        temp1 = map[temp[0]][temp[1]];
		temp1 >>= 2; 
        if((!(temp1 &= 0x01)) && map[temp[0]][temp[1]+1] >>4){
            record(temp[0] ,temp[1] + 1 );
            flag += 1;
        }
        temp1 = map[temp[0]][temp[1]];
		temp1 >>= 3; 
        //上方 
        if((!(temp1 &= 0x01)) && map[temp[0]-1][temp[1]] >>4){
            record(temp[0]- 1 ,temp[1]);
            flag += 1;
        }
        if(flag >=2 && first == 0)
        {
        	count += flag - 1;
        	first = 1;
        }
        else if(flag >= 2){
        	count += flag - 1;
        }
        if(flag >0){
        	map[temp[0]][temp[1]] = num;
        	loop --; 
        }
        else{
        	map[temp[0]][temp[1]] = num;
        	loop --;
        	count --;
        }
    
        if(loop == 0)
        {
        	loop = count;
        	num++;
        }
    }
}
/*
 * 准备冲刺
 */
void final_way()
{
	
	char x = 0, y = 7;//终点坐标 
	while(1)
	{
		    if(x == 7 && y == 0)//起点坐标 
		    break;
			if((map[x-1][y] == (map[x][y] - 1)) && (x-1 >=0))//上方 
			{
				top1++;
				way[top1] = 2;
				x -= 1; 
				continue;
			}
			if((map[x][y+1] == (map[x][y] - 1)) && (y+1 <= 7))//右方 
			{
				top1++;
				way[top1] = 3;
				y += 1; 
				continue;
			}
			if((map[x+1][y] == (map[x][y] - 1)) && (x+1 <= 7))//下方 
			{
				top1++;
				way[top1] = 0;
				x += 1; 
				continue;
			}
			if((map[x][y-1] == (map[x][y] - 1)) && (y-1 >= 0))//左方 
			{
				top1++;
				way[top1] = 1;
				y -= 1; 
				continue;
			}
	}
}  
/* * * * * * * * * * * * * 
 * 第三部分 冲刺
 * 冲刺
 */
void run()
{
  direction = 2;
	top1 = 13;
	back();
	while(top1 > -1)
	{
		switch(direction){
			case 0:
				{
		
			switch(finall[top1])
			{
				case 1: turn_right(); run_fast() ; break;
				case 0: run_fast() ; break; 
			}
			break;
		}
			case 1:{
				
			
			switch(finall[top1])
			{
				case 1: run_fast() ;break;
				case 0: turn_left(); run_fast() ; break; 
			}	
		    break;
		}
	   }
		top1--;
	}	

} 


void main()
{
//	initTime2();//初始化定时器
//	while(1){
//		//test();
//		ir_test();
//	}
	
	initTime2();//初始化定时器
	init_map();//初始化地图
	while(top >= 0)
	{
		delayms(500);
		find();//遍历地图
	}

	set_map();//处理地图
	
	init();//初始化队列
	
	go_map();//寻路
	
	final_way();//准备冲刺
	
	run();//冲刺
	voice();
	voice();
	voice();
	while(1);
}
/*
 *中断服务函数
 */
void time2() interrupt 5{ // TIM2中断服务函数
    static bit ir = 0;
    static unsigned char n=1;
    TF2 = 0;
    if(!ir)	{
        mouse_ir_on(n-1);
    }else{
        if(n==1){
            if(irR1)	irC=0;
            else			irC=1;
        }else if(n==2){
            if(irR2)	irLU=0;
            else			irLU=1;
        }else if(n==3){
            if(irR3)	irL=0;
            else			irL=1;
        }else if(n==4){
            if(irR4)	irR=0;
            else			irR=1;
        }else if(n==5){
            if(irR5)	irRU=0;
            else			irRU=1;
        }
    }
    if(ir)	n++;
    if(n>5)	n=1;
    ir=~ir;
}

