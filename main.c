#include "main.h"

#define GPIO_DIG P0
#define GPIO_KEY P1
#define S01		(0x77)
#define S02		(0x7b)
#define S03		(0x7d)
#define S04		(0x7e)
#define S05		(0xb7)
#define S06		(0xbb)
#define S07		(0xbd)
#define S08		(0xbe)
#define S09		(0xd7)
#define S10		(0xdb)
#define S11		(0xdd)
#define S12		(0xde)
#define S13		(0xe7)
#define S14		(0xeb)
#define S15		(0xed)
#define S16		(0xee)

#define NUM1     S01   //数字1
#define NUM2     S02   //数字2
#define NUM3     S03   //数字3
#define NUM4     S05   //数字4
#define NUM5     S06   //数字5
#define NUM6     S07   //数字6
#define NUM7     S09   //数字7
#define NUM8     S10   //数字8
#define NUM9     S11   //数字9
#define NUM0     S13   //数字0
#define POINT    S14   //小数点

#define ADD      S04    //加法
#define SUB      S08    //减法
#define MUL      S12   //乘法
#define DIV      S16   //除法
#define EQU      S15   //等于
  
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
*/                /*  0    1    2    3    4    5    6    7    8    9    .    a    b    c    d    e    F*/
u1 code smgduan[27]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x80,0x5f,0x7c,0x59,0x56,0x7b,0x71
                  /*  0.   1.   2.   3.   4.   5.   6.   7.   8.   9.    */   
                     0xbf,0x8b,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};   //0和0.差了17
                 /*   0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16
                      17   18   19   20   21   22   23   24   25   26   */
typedef void (*func)(void);

func fun_array[8] = {SetNCT_0,SetNCT_1,SetNCT_2,SetNCT_3,
                     SetNCT_4,SetNCT_5,SetNCT_6,SetNCT_7};
/* 
func fun_array[8] = {SetNCT_7,SetNCT_6,SetNCT_5,SetNCT_4,
                     SetNCT_3,SetNCT_2,SetNCT_1,SetNCT_0};
  */
enum OPERATER
{
    ADD_ = 1,
    SUB_    ,
    MUL_    ,
    DIV_    
};

  u1 KeyScan(void);
void KeyDown(void);
void delay(u2 i);
void DisPlay();
void FirstNumIsZeroFlagSet(u1* Num,u1 n);
void setNumArray(u1 NUM_X);
void setNowCount(u1 getCount);
void NumToTube(u4 NowCount);
void ClearnNumArray();
  u4 NowCount = 0;
  u4 PreCount = 0;  //当加减乘除任意一个被按下是，储存之前的值也就是NowCount的值
  u1 operater = 0;
  u1 NumIndexRemain = 7;
/**********下面的这个数组不能改**********/
u1 FirstNumIsZeor[8] = {1,0,0,0,0,0,0,0};
/**********上面的这个数组不能改**********/

u1 Num[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
u1 NumPre[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; //当加减乘除任意一个被按下是，储存之前的值也就是Num的值
/*下面变量为1时：非0被按下；为0时：0被按下*/
u1 NumDown=0;
u1 NumDownPre=0;
u1 OperatDown = 0;
void main()
{	
    while(1)
	{
	     KeyDown();
		 DisPlay();
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
	switch( KeyScan() )
	{	
		case NUM1:
            NumDown=1;
            OperatDown=0;
            setNowCount(1);
            setNumArray(1);
            break;

		case NUM2:
            NumDown=1;
            OperatDown=0;
            setNowCount(2);
            setNumArray(2);
            break;

		case NUM3:
            NumDown=1;
            OperatDown=0;
            setNowCount(3);
            setNumArray(3);
            break;

		case NUM4:
            NumDown=1;
            OperatDown=0;
            setNowCount(4);
            setNumArray(4);
            break;

		case NUM5:
            NumDown=1;
            OperatDown=0;
            setNowCount(5);
            setNumArray(5);
            break;

		case NUM6:
            NumDown=1;
            OperatDown=0;
            setNowCount(6);
            setNumArray(6);
            break;

		case NUM7:
            NumDown=1;
            OperatDown=0;
            setNowCount(7);
            setNumArray(7);
            break;

		case NUM8:
            NumDown=1;
            OperatDown=0;
            setNowCount(8);
            setNumArray(8);
            break;

		case NUM9:
            NumDown=1;
            OperatDown=0;
            setNowCount(9);
            setNumArray(9);
            break;

		case NUM0:
            OperatDown=0;
            if(NumDown!=0)
            {
                setNowCount(0);
                setNumArray(0);
            }
            break;

		case ADD:
            PreCount = NowCount;
            NowCount = 0;
            NumDown=0;
            OperatDown=1;
            operater = ADD_;
            //setNumArray(11);
            ClearnNumArray();
            break;

        case SUB:
            PreCount = NowCount;
            NowCount = 0;
            NumDown=0;
            OperatDown=1;
            operater = SUB_;
            ClearnNumArray();
            break;

        case MUL:
            PreCount = NowCount;
            NowCount = 0;
            NumDown=0;
            OperatDown=1;
            operater = MUL_;
            ClearnNumArray();
            break;

        case DIV:
            PreCount = NowCount;
            NowCount = 0;
            NumDown=0;
            OperatDown=1;
            operater = DIV_;
            ClearnNumArray();
            break;

        case EQU:
        //    NowCount = 0;
            NumDown=0;
        //    OperatDown=1;
            switch(operater)
            {
                case ADD_:
                    NowCount = PreCount + NowCount;
                    break;
                case SUB_:
                    NowCount = PreCount - NowCount;
                    break;
                case MUL_:
                    NowCount = PreCount * NowCount;
                    break;
                case DIV_:
                    NowCount = PreCount / NowCount;
                    break;
            }
            NumToTube(NowCount);
            break;
	}
}
/***************************************
****此函数用于设置运算的输入值**********
***************************************/
void setNowCount(u1 getCount)
{
     NowCount = NowCount*10+ getCount;
}

/*********************************
****此函数用于数码管显示数字用****
*********************************/
void DisPlay()
{
    u1 temp[8];
	u1 getNum;
	u1 j = 7;  //用于函数指针
	u1 i = 0;
    //NumToTube(12345678);
	FirstNumIsZeroFlagSet(Num,7);
	for(;i<=7;i++)
	{
        if(OperatDown==1)
        {
            temp[i] = NumPre[i];  
        }
        else
        {
		    temp[i] = Num[i];
        }
		if(temp[i]<=9&&FirstNumIsZeor[i]!=0)
		{
			getNum = temp[i];
			fun_array[j--]();
			GPIO_DIG = smgduan[getNum];
			delay(200);	  //250ms左右肉眼差不多分辨不出来，超过会有闪现现象，太少的话亮度不够
			P0 = 0;	 //用于消除鬼影
		}
		else
		{
		    //P0 = 0;	 //用于在没有按下数字前，数码管不显示任何数字
		}
	}  
}
/********************************************
*这个方法是用于不显示第一个非零数字之前的零 *
*比如001234500，1之前的两个0应该在数码管上  *
*不显示，数码管上应该只显示1234500          *
********************************************/
void FirstNumIsZeroFlagSet(u1* Num,u1 n)
{
    u1 i;
	if(n>0)	  //这里必须要大于0，不然会发生乱七八糟的事情
	{
		i = n; 
		//注意，这里必须要初始化，不然的话i的值很可能是0，会把u1 FirstNumIsZeor[8] = {1,0,0,0,0,0,0,0};的1给串改掉
		if(Num[n] != 0)
		{
			 for(i=n;i>0;i--)
			{
				 FirstNumIsZeor[i] = 1;
			}
		}
		else
		{
			FirstNumIsZeor[i] = 0;
			FirstNumIsZeroFlagSet(Num,(n-1));
		}
	}
}
/***************************************
****此函数用于判断矩阵哪个按键被按下****
***************************************/
u1 KeyScan()
{
	u1 cord_l,cord_h;//声明列线和行线的值的储存变量
	GPIO_KEY = 0xf0;//1111 0000
	if( (GPIO_KEY & 0xf0) != 0xf0)//判断是否有按键按下
	{
		delay(5);//软件消抖
		if( (GPIO_KEY & 0xf0) != 0xf0)//判断是否有按键按下
		{
			  cord_l = GPIO_KEY & 0xf0;// 储存列线值
			  GPIO_KEY = cord_l | 0x0f;
			  cord_h = GPIO_KEY & 0x0f;// 储存行线值
			  while( (GPIO_KEY & 0x0f) != 0x0f );//松手检测
			  return (cord_l + cord_h);//返回键值码
		}	
	}
		
}
/***************************************
****当前按下什么值数码管上显示什么值****
***************************************/
void setNumArray(u1 NUM_X)
{
    if(NumIndexRemain>=0)
        Num[NumIndexRemain--] =  NUM_X;
}

void ClearnNumArray()
{
    u1 i = 0;
    NumIndexRemain = 7;  //这个变量必须重新被赋值
   
    for(i=0;i<=7;i++)
    {
        NumPre[i] = Num[i];
        Num[i] = 0xff;
    } 
}

/***************************************
****此函数将数值转换成数码管显示********
***************************************/
void NumToTube(u4 NowCount)
{
    s1 i = 0;
    u1 figure = 1;
    u1 Num_t[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
    u4 NowCount_t = NowCount;
    u1 Remainder;
    NumIndexRemain = 7;  //这个变量必须重新被赋值
    while((NowCount_t=NowCount_t/10)!=0)
        figure++;
    for(i=0;i<=7;i++)
    {
        Num[i] = 0xff;
    }
    NowCount_t = NowCount;
  //  for(i=(7-figure+1);i<=7;i++)
    for(i=7;i>(7-figure);i--)
    {
       Remainder = (u1)(NowCount_t % 10);
       NowCount_t = NowCount_t /10;
       /*setNumArray(Remainder);*/   /*第一不能用这个来设置Num数组里面的值，这个是用来按键的时候设置的*/
       Num[7-i] = Remainder;       /*第二，需要反转*/
    }
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