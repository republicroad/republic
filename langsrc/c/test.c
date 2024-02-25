
// #include<stdio.h>
// int main(void)
// {
//     int a = 0;
//     int b = 0;
//     scanf("%d%d",&a,&b);
//     printf("%d %d\n",a,b);
//     return 0;
// }

#include<stdio.h>

int main()

{

int x=0,y=3,z;
int dsb = (x=y%2+1);
printf("%d\n", dsb);
if(dsb)

z=2;

printf("%d %d ",z,x);

return 0;

}