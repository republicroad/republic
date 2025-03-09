// Enter the data for the student
// ROLL NO.: 01
// NAME: Rahul
// COURSE: BCA
// FEES: 45000
// DETAILS OF STUDENT
// ROLL NO. = 01
// NAME = Rahul
// COURSE = BCA
// FEES = 45000

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
void display(struct student *);
int main()
{
    struct student *ptr;
    ptr = (struct student *)malloc(sizeof(struct student));
    printf("\n Enter the data for the student ");
    printf("\n ROLL NO.: ");
    scanf("%d", &ptr->r_no);
    printf("\n NAME: ");
    gets(ptr->name);
    printf("\n COURSE: ");
    gets(ptr->course);
    printf("\n FEES: ");
    scanf("%d", &ptr->fees);
    display(ptr);
    getch();
    return 0;
}
void display(struct student *ptr)
{
    printf("\n DETAILS OF STUDENT");
    printf("\n ROLL NO. = %d", ptr->r_no);
    printf("\n NAME = %s", ptr->name);
    printf("\n COURSE = %s ", ptr->course);
    printf("\n FEES = %d", ptr->fees);
}