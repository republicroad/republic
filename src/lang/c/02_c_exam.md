# 练习循环和分支

## 1.求1至100之和

```c
int main()
{
    int  x;
    for(int a=0; a<=100; a++)
    {
        x+=a;
    }

    return 0;
}
```

## 2.找出100以内的所有质数

![avatar](../attach/02_01.png)


## 3.使用筛子找出100以内的质数

```c
#include <stdio.h>
static int MAX_NUM = 101;

int main()
{
    int b=2; // 最小的质数, 遍历开始
    int nums[MAX_NUM];
    for(int j=0;j<MAX_NUM;j++)
    {
        nums[j] = 1;  //数组初始化为1,状态位
    }

    nums[0]=nums[1]=0;  // 0,1既不是质数也不是合数
    while(b * b < MAX_NUM)
    {
        if(nums[b] == 1){//查看当前数字(下标)是否是质数, 如果是质数,开始把它的倍数的状态位都设置为0, 表示合数.
            for(int i=2;i*b<MAX_NUM;i++)
            {
                nums[b*i]=0;
            }
        }
        b+=1;  //寻找下一个质数.
    }

    for(int j=0;j<MAX_NUM;j++)
    {
        if(nums[j] == 1){  // 剩下这些状态位为1的就是筛子留下的质数了.
            printf("%d ", j);
        }
    }
    return 0;

}

```


## 4.补充

问：最后不写return 0;有影响吗？

答：最好带上 return 语句. 一般c语言, linux内核返回0代表程序正常执行返回, 返回非0代表出问题了.


## 5. c types

2023-01-21
```c

#include <stdio.h>
#include <stdint.h>
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c\n"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

int main(void)
{
//    byte = 8 bit
//    bit: 计算机状态的最小存储单元.

 char c;
 printf("%zu %zu\n", sizeof(char), sizeof c);
 int i;
 printf("%zu %zu\n", sizeof(int), sizeof i);
 double d;
 printf("%zu %zu\n", sizeof(double), sizeof d);

 char ch = 'A';
 printf("char: %c\n", ch);
 printf("char: %d\n", ch);

  int  ci = 65;          // 64 + 1     2**6 + 2**0
 printf("char: %c\n", ci); //litral
 printf("char: %d\n", ci);

 int a = 63;
 printf("char: %c"" \n",a);
 printf("hex: %02x"" \n",a);// 十六进制  小写 a-f
 printf("oct: %o"" \n",a);  // 八进制
//
 printf("HEX: %02X"" \n",a);// 十六进制  大写 A-F
 printf("hex: 0x%02x"" \n",a);
 printf("HEX: 0x%02X"" \n",a);
//
 int b = 0x3f;  // 63 的十六进制
 printf("hex define int: %d \n", b);

 char byte='?';
 printf("byte to binary:"BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte));


 for(int i=-128;i<129;i++){
    char byte=i;
//    printf("%d %c to binary:"BYTE_TO_BINARY_PATTERN, i, i, BYTE_TO_BINARY(byte));
    printf("%03d to binary:"BYTE_TO_BINARY_PATTERN, i, BYTE_TO_BINARY(byte));
 }

 int8_t i8= -1;
 uint8_t uit=-1;
 printf("%03d\n", uit);
 printf("%03d to binary:"BYTE_TO_BINARY_PATTERN, i, BYTE_TO_BINARY(uit));

 printf("%03d\n", i8);
 printf("%03d to binary:"BYTE_TO_BINARY_PATTERN, i, BYTE_TO_BINARY(i8));

 uint8_t uit2 = 257;  // 1
 printf("%03d\n", uit2);

  for(uint8_t i=0;;i++){
    char byte=i;
//    printf("%d %c to binary:"BYTE_TO_BINARY_PATTERN, i, i, BYTE_TO_BINARY(byte));
    printf("%03d to binary:"BYTE_TO_BINARY_PATTERN, i, BYTE_TO_BINARY(byte));
    if(i==255){break;}
}

 return 0;
}

```











