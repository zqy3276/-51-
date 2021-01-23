#include "main.h"

#define GPIO_DIG P0
#define GPIO_KEY P1
//下面三个用于控制哪个数码管亮
sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;


//数码管英语：numerical code tube
//用三个字母加数字表示显示的数码管NCT_num
//最左边的是0编号，最右边的是7编号
void SetNCT_0(void);
void SetNCT_1(void);
void SetNCT_2(void);
void SetNCT_3(void);
void SetNCT_4(void);
void SetNCT_5(void);
void SetNCT_6(void);
void SetNCT_7(void);
#define NCT_(x)    (SetNCT_##x)
/*
#define NCT_0()    (SetNCT_0())  
#define NCT_1()    (SetNCT_1())
#define NCT_2()    (SetNCT_2())
#define NCT_3()    (SetNCT_3())
#define NCT_4()    (SetNCT_4())
#define NCT_5()    (SetNCT_5())
#define NCT_6()    (SetNCT_6())
#define NCT_7()    (SetNCT_7())
*/
/********************************************
*  +---0x01---+                             *
*  |		  |							    *
*  |	      |								*
* 0x20      0x02	数码管上的显示数字		*
*  |          |								*
*  |	      |								*
*  +---0x40---+								*
*  |		  |								*
*  |		  |								*
* 0x10      0x04							*
*  |		  |								*
*  |	      |								*
*  +---0x08---+ 0x80  						*
********************************************/
/*
下面是用于在数码管上显示的数字的CODE
依次是从0开始到9再小数点.
*/
u1 code smgduan[11]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x80};
typedef void (*func)(void);

func fun_array[8] = {SetNCT_0,SetNCT_1,SetNCT_2,SetNCT_3,
                     SetNCT_4,SetNCT_5,SetNCT_6,SetNCT_7};
/* 
func fun_array[8] = {SetNCT_7,SetNCT_6,SetNCT_5,SetNCT_4,
                     SetNCT_3,SetNCT_2,SetNCT_1,SetNCT_0};
  */
void KeyDown(void);
void delay(u2 i);
void DisPlay(u1* Num);
void FirstNumIsZeroFlagSet(u1* Num);
u1 KeyValue;
u1 FirstNumIsZeor[8] = {1,0,0,0,0,0,0,0};
void main()
{
    u1 Num[8] = {0,0,0,0,0,0,0,5};
	
    while(1)
	{
	     //KeyDown();
		 DisPlay(Num);
	}
}

/*******************
* 软件模拟延时时间 *
* 5000相当于450ms  *
/******************/
void delay(u2 i)
{
    while(i--);
}

/*******************
* 动态晶体管显示   *
*  矩阵按下        *
/******************/
void KeyDown(void)
{
    u1 out = 0;//当按键一直被按着不放时，用于强制退出
	GPIO_KEY = 0x0f;
	if(GPIO_KEY!=0x0f)
	{
	    delay(1000);//用于消抖
		if(GPIO_KEY!=0x0f)
		{
		    switch(GPIO_KEY)
			{
			    case 0x07: KeyValue=0x01;break;
				case 0x0b: KeyValue=0x02;break;
				case 0x0d: KeyValue=0x03;break;
				//case 0x0e: KeyValue=0x03;break;
			}
			GPIO_KEY = 0xf0;
		   	switch(GPIO_KEY)
			{
			    case 0x70: KeyValue += 0x00;break;
				case 0xb0: KeyValue += 0x03;break;
				case 0xd0: KeyValue += 0x06;break;
				case 0xe0: KeyValue = 0x00;break;
			}
		}
	}
	if((out<50)&&(GPIO_KEY!=0xf0)) //松手检测
	{
	    out++;
		delay(100);
	}
	//NCT_7();
}

void SetNCT_0(void)
{
    LSA = 1;LSB = 1;LSC = 1;
}
void SetNCT_1(void)
{
    LSA = 0;LSB = 1;LSC = 1;
}
void SetNCT_2(void)
{
    LSA = 1;LSB = 0;LSC = 1;
}
void SetNCT_3(void)
{
    LSA = 0;LSB = 0;LSC = 1;
}
void SetNCT_4(void)
{
    LSA = 1;LSB = 1;LSC = 0;
}
void SetNCT_5(void)
{
    LSA = 0;LSB = 1;LSC = 0;
}
void SetNCT_6(void)
{
    LSA = 1;LSB = 0;LSC = 0;
}
void SetNCT_7(void)
{
    LSA = 0;LSB = 0;LSC = 0;
}
void DisPlay(u1* Num)
{
    u1 temp[8];
	u1 getNum;
	u1 j = 7;  //用于函数指针
	u1 i = 0;
	FirstNumIsZeroFlagSet(Num);
	for(;i<=7;i++)
	{
	    getNum = 0xf;
		temp[i] = Num[i];
		//P0 = 0;	 //用于消除鬼影		u1 Num[8] = {9,2,11,4,5,6,0,0};
		if(temp[i]<=9&&FirstNumIsZeor[i]!=0)
		{
			getNum = temp[i];
			fun_array[j--]();
			GPIO_DIG = smgduan[getNum];
			delay(250);	  //250ms左右肉眼差不多分辨不出来，超过会有闪现现象，太少的话亮度不够
			P0 = 0;	 //用于消除鬼影
		}
	}  
}

void FirstNumIsZeroFlagSet(u1* Num)
{
    u1 i;
    if(Num[7] != 0)
	{
	    for(i=7;i>0;i--)
		{
	        FirstNumIsZeor[i] = 1;
		}
	}
	else
	{
	    FirstNumIsZeor[7] = 0;
	   	if(Num[6] != 0)
		{
		    for(i=6;i>0;i--)
		    {
	            FirstNumIsZeor[i] = 1;
		    }   
		
		}
		else
		{
		    FirstNumIsZeor[6] = 0;
			if(Num[5] != 0)
			{
			    for(i=5;i>0;i--)
				{
				    FirstNumIsZeor[i] = 1;
				}
			}
			else
			{
			    FirstNumIsZeor[5] = 0;
				if(Num[4] != 0)
				{
				    for(i=4;i>0;i--)
					{
					    FirstNumIsZeor[i] = 1;
					}
				}
				else
				{
				    FirstNumIsZeor[4] = 0;
					if(Num[3] != 0)
					{
					    for(i=3;i>0;i--)
						{
						    FirstNumIsZeor[i] = 1;
						}
					}
					else
					{
					    FirstNumIsZeor[3] = 0;
						if(Num[2] !=0)
						{
						   for(i=2;i>0;i--)
						   {
						       FirstNumIsZeor[i] = 1;
						   }
						}
						else
						{
						    FirstNumIsZeor[2] = 0;
						    if(Num[1] !=0)
							{
							   for(i=1;i>0;i--)
							   {
							       FirstNumIsZeor[i] = 1;
							   }
							}
							else
							{
							     FirstNumIsZeor[1] = 0;
							}
						}
					}
				}
			}
		}
	}
}