#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

int main()
{
    bool IsRunning = true;
    while(IsRunning)
    {
        // menu();
        printf("[7]Clear The window.\n");
        int Option;
        scanf(" %d", &Option);
        // scanf_s(" %d", &Option);
        if(Option==7){
           system("cls"); 
        }else{
            exit(0);
        }
        // char ch = getchar();
        // getchar();// getchar 接收标准输入的所有输入 当输入 7 enter 时, 标准输入的第二个是 enter, 所以第二个getchar就是把enter忽略掉.
        // printf("ch:%x", ch);
        // if(ch=='7'){
        //    system("cls"); 
        // }else{
        //     exit(0);
        // }
    }
    return 0;
} // end main function

