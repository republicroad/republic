
# exam

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

![avatar](../../editor/attach/02_01.png)


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


## 5.c types

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




## function

函数定义语法:

```c
return_type function_name(argument_list){
//blocks of valid C statements }
```


```c
#include <stdio.h>
int addition(int a, int b){
    return a+b;
}

int addition_p(int a, int b, int *c){
    // return 1;
    *c =  a+b;
    return 0;
}


int incr(int a){
    printf("incr a arg address:%p\n", &a);
    printf("before incr arg:%d\n", a);
    a = a + 1;
    printf("after incr res:%d\n", a);
    return a;
}

int incr_by_point(int *a){
    printf("incr_by_point a arg address:%p\n", a);
    printf("before incr_by_point arg:%d\n", *a);
    *a = *a + 1;
    printf("after incr_by_point res:%d\n", *a);
    return a;
}


int main()
{
//    变量定义 声明和赋值(初始化)
    int a,b=6;  // 声明之后的值是不确定的.
    char c = '0';
    short d;
    int e;

    printf("Address of variable \"a\": %p\n", &a);
    printf("Address of variable \"b\": %p\n", &b);
    printf("Address of variable \"c\": %p\n", &c);
    printf("Address of variable \"d\": %p\n", &d);
    printf("Address of variable \"e\": %p\n", &e);
    printf("a: %d\n", a);
    printf("b: %d\n", b);
//    printf("c: %c\n", c);
//    printf("c: %d\n", c);

//    指针定义: 声明时使用 * 修饰
//    获取一个变量的指针, &varible
    int *p_a = &a;
    int *p_b = &b;
    printf("p_a: %p\n", p_a);//指针变量
    printf("p_b: %p\n", p_b);
    printf("p_a content: %d\n", *p_a);//使用 * 进行解引用 dereference
    printf("p_b content: %d\n", *p_b);
    printf("input a\n");
    scanf("%d", &a);
    printf("input b\n");
    scanf("%d", &b);

    printf("test function addition\n");
    int res = addition(a, b);
    printf("addition:%d \n", res);

    printf("test function arg poniter addition\n");
    int res1 = addition(*p_a, *p_b);
    printf("addition:%d \n", res1);
    int result;
    addition_p(*p_a, *p_b, &result);
    printf("addition_p:%d \n", result);

//    形参和实参的最大区别: 能否修改值.
    printf("\n\n------incr---------\n");
    incr(a);
    printf("outer incr a:%d\n", a);

    printf("\n\n------incr_by_point---------\n");
    incr_by_point(p_a);
    printf("outer incr_by_point a:%d\n", a);

////  函数指针
//    printf ("Address of variable \"main\": %p\n", &main);
//    printf ("Address of variable \"main\": %p\n", main);
//    printf ("Address of variable \"addition\": %p\n", &addition);
//    printf ("Address of variable \"addition\": %p\n", addition);
    return 0;
}
```


## 传参模式

~~因为c语言的函数传参本质就是 call by value, 把参数的值拷贝一份传入函数. 在需要进行原地修改的场景时, 那么需要把变量本身传递进去, 这样就有了保持变量本身的概念----指针.~~

c语言的函数传参会把每个实参都拷一份放在函数定义的形参空间中. 正因为如此, 为了对原有的值进行操作, 所以引入变量的引用(地址, 指针)的概念来进行原地修改.

### Call by Vaue 

In call by value method of parameter passing, the values of actual parameters are copied to the function’s formal parameters.

- There are two copies of parameters stored in different memory locations.
- One is the original copy and the other is the function copy.
- Any changes made inside functions are not reflected in the actual parameters of the caller.

```c
// C program to illustrate call by value
#include <stdio.h>

// Function Prototype
void swapx(int x, int y);

// Main function
int main()
{
	int a = 10, b = 20;

	// Pass by Values
	swapx(a, b); // Actual Parameters

	printf("In the Caller:\na = %d b = %d\n", a, b);

	return 0;
}

// Swap functions that swaps
// two values
void swapx(int x, int y) // Formal Parameters
{
	int t;

	t = x;
	x = y;
	y = t;

	printf("Inside Function:\nx = %d y = %d\n", x, y);
}

```

output:
```
Inside Function:
x = 20 y = 10
In the Caller:
a = 10 b = 20
```

### Call by Reference

In call by reference method of parameter passing, the address of the actual parameters is passed to the function as the formal parameters. In C, we use pointers to achieve call-by-reference.

- Both the actual and formal parameters refer to the same locations.
- Any changes made inside the function are actually reflected in the actual parameters of the caller.

```c
// C program to illustrate Call by Reference
#include <stdio.h>

// Function Prototype
void swapx(int*, int*);

// Main function
int main()
{
	int a = 10, b = 20;

	// Pass reference
	swapx(&a, &b); // Actual Parameters

	printf("Inside the Caller:\na = %d b = %d\n", a, b);

	return 0;
}

// Function to swap two variables
// by references
void swapx(int* x, int* y) // Formal Parameters
{
	int t;

	t = *x;
	*x = *y;
	*y = t;

	printf("Inside the Function:\nx = %d y = %d\n", *x, *y);
}

```

output:

```
Inside the Function:
x = 20 y = 10
Inside the Caller:
a = 20 b = 10
```

### 总结


> [!NOTE] Title
> In C, we use pointers to achieve call-by-reference. In C++, we can either use pointers or [references](https://www.geeksforgeeks.org/references-in-c/) for pass-by-reference. In Java,  [primitive types are passed as values and non-primitive types are always references](https://www.geeksforgeeks.org/g-fact-31-java-is-strictly-pass-by-value/)_._

|Call By Value|Call By Reference|
|---|---|
|While calling a function, we pass the values of variables to it. Such functions are known as “Call By Values”.|While calling a function, instead of passing the values of variables, we pass the address of variables(location of variables) to the function known as “Call By References.|
|In this method, the value of each variable in the calling function is copied into corresponding dummy variables of the called function.|In this method, **the address of actual variables** in the calling function** is copied into the dummy variables** of the called function. |
|With this method, the changes made to the dummy variables in the called function have no effect on the values of actual variables in the calling function.|With this method, using addresses we would have access to the actual variables and hence we would be able to manipulate them.|
|In call-by-values, we cannot alter the values of actual variables through function calls.|In call by reference, we can alter the values of variables through function calls.|
|Values of variables are passed by the Simple technique.|Pointer variables are necessary to define to store the address values of variables.|
|This method is preferred when we have to pass some small values that should not change.|This method is preferred when we have to pass a large amount of data to the function.|
|Call by value is considered safer as original data is preserved|Call by reference is risky as it allows direct modification in original data|

参考: [difference-between-call-by-value-and-call-by-reference](https://www.geeksforgeeks.org/difference-between-call-by-value-and-call-by-reference/)


##  学生成绩管理系统


[student-management-system-in-c](https://github.com/inside-the-div/student-management-system-in-c)

[SMS](https://github.com/ripgamer/SMS)

使用此程序进行演示:
![](../../../langsrc/c/mysms.c)