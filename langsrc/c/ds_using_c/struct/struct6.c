#include <stdio.h>
#include <stdlib.h>
// #include <conio.h>

#define clrscr() printf("\033[H\033[J")

struct student
{
    int r_no;
    char name[20];
    char course[20];
    int fees;
};
int main()
{
    struct student stud1, *ptr_stud1;
    clrscr();
    ptr_stud1 = &stud1;
    printf("\n Enter the details of the student :");
    printf("\n Enter the Roll Number =");
    scanf("%d", &ptr_stud1->r_no);
    printf("\n Enter the Name = ");
    gets(ptr_stud1->name);
    printf("\n Enter the Course = ");
    gets(ptr_stud1->course);
    printf("\n Enter the Fees = ");
    scanf("%d", &ptr_stud1->fees);
    printf("\n DETAILS OF THE STUDENT");
    printf("\n ROLL NUMBER = %d", ptr_stud1->r_no);
    printf("\n NAME = %s", ptr_stud1->name);
    printf("\n COURSE = %s", ptr_stud1->course);
    printf("\n FEES = %d", ptr_stud1->fees);
    return 0;
}