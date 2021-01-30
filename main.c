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

#define NUM1     S01   //����1
#define NUM2     S02   //����2
#define NUM3     S03   //����3
#define NUM4     S05   //����4
#define NUM5     S06   //����5
#define NUM6     S07   //����6
#define NUM7     S09   //����7
#define NUM8     S10   //����8
#define NUM9     S11   //����9
#define NUM0     S13   //����0
#define POINT    S14   //С����

#define ADD      S04    //�ӷ�
#define SUB      S08    //����
#define MUL      S12   //�˷�
#define DIV      S16   //����
#define EQU      S15   //����
  
//�����������ڿ����ĸ��������
sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;


//�����Ӣ�numerical code tube
//��������ĸ�����ֱ�ʾ��ʾ�������NCT_num
//����ߵ���0��ţ����ұߵ���7���
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
* 0x20      0x02	������ϵ���ʾ����		*
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
���������������������ʾ�����ֵ�CODE
�����Ǵ�0��ʼ��9��С����.
*/                /*  0    1    2    3    4    5    6    7    8    9    .    a    b    c    d    e    F*/
u1 code smgduan[27]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x80,0x5f,0x7c,0x59,0x56,0x7b,0x71
                  /*  0.   1.   2.   3.   4.   5.   6.   7.   8.   9.    */   
                     0xbf,0x8b,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};   //0��0.����17
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
  u4 PreCount = 0;  //���Ӽ��˳�����һ���������ǣ�����֮ǰ��ֵҲ����NowCount��ֵ
  u1 operater = 0;
  u1 NumIndexRemain = 7;
/**********�����������鲻�ܸ�**********/
u1 FirstNumIsZeor[8] = {1,0,0,0,0,0,0,0};
/**********�����������鲻�ܸ�**********/

u1 Num[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
u1 NumPre[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; //���Ӽ��˳�����һ���������ǣ�����֮ǰ��ֵҲ����Num��ֵ
/*�������Ϊ1ʱ����0�����£�Ϊ0ʱ��0������*/
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
* ���ģ����ʱʱ�� *
* 5000�൱��450ms  *
/******************/
void delay(u2 i)
{
    while(i--);
}

/*******************
* ��̬�������ʾ   *
*  ������        *
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
****�˺��������������������ֵ**********
***************************************/
void setNowCount(u1 getCount)
{
     NowCount = NowCount*10+ getCount;
}

/*********************************
****�˺��������������ʾ������****
*********************************/
void DisPlay()
{
    u1 temp[8];
	u1 getNum;
	u1 j = 7;  //���ں���ָ��
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
			delay(200);	  //250ms�������۲��ֱ治����������������������̫�ٵĻ����Ȳ���
			P0 = 0;	 //����������Ӱ
		}
		else
		{
		    //P0 = 0;	 //������û�а�������ǰ������ܲ���ʾ�κ�����
		}
	}  
}
/********************************************
*������������ڲ���ʾ��һ����������֮ǰ���� *
*����001234500��1֮ǰ������0Ӧ�����������  *
*����ʾ���������Ӧ��ֻ��ʾ1234500          *
********************************************/
void FirstNumIsZeroFlagSet(u1* Num,u1 n)
{
    u1 i;
	if(n>0)	  //�������Ҫ����0����Ȼ�ᷢ�����߰��������
	{
		i = n; 
		//ע�⣬�������Ҫ��ʼ������Ȼ�Ļ�i��ֵ�ܿ�����0�����u1 FirstNumIsZeor[8] = {1,0,0,0,0,0,0,0};��1�����ĵ�
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
****�˺��������жϾ����ĸ�����������****
***************************************/
u1 KeyScan()
{
	u1 cord_l,cord_h;//�������ߺ����ߵ�ֵ�Ĵ������
	GPIO_KEY = 0xf0;//1111 0000
	if( (GPIO_KEY & 0xf0) != 0xf0)//�ж��Ƿ��а�������
	{
		delay(5);//�������
		if( (GPIO_KEY & 0xf0) != 0xf0)//�ж��Ƿ��а�������
		{
			  cord_l = GPIO_KEY & 0xf0;// ��������ֵ
			  GPIO_KEY = cord_l | 0x0f;
			  cord_h = GPIO_KEY & 0x0f;// ��������ֵ
			  while( (GPIO_KEY & 0x0f) != 0x0f );//���ּ��
			  return (cord_l + cord_h);//���ؼ�ֵ��
		}	
	}
		
}
/***************************************
****��ǰ����ʲôֵ���������ʾʲôֵ****
***************************************/
void setNumArray(u1 NUM_X)
{
    if(NumIndexRemain>=0)
        Num[NumIndexRemain--] =  NUM_X;
}

void ClearnNumArray()
{
    u1 i = 0;
    NumIndexRemain = 7;  //��������������±���ֵ
   
    for(i=0;i<=7;i++)
    {
        NumPre[i] = Num[i];
        Num[i] = 0xff;
    } 
}

/***************************************
****�˺�������ֵת�����������ʾ********
***************************************/
void NumToTube(u4 NowCount)
{
    s1 i = 0;
    u1 figure = 1;
    u1 Num_t[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
    u4 NowCount_t = NowCount;
    u1 Remainder;
    NumIndexRemain = 7;  //��������������±���ֵ
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
       /*setNumArray(Remainder);*/   /*��һ���������������Num���������ֵ�����������������ʱ�����õ�*/
       Num[7-i] = Remainder;       /*�ڶ�����Ҫ��ת*/
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