// Enter the number of students : 1
// Enter the data for student 1
// ROLL NO.: 01
// NAME: Rahul
// COURSE: BCA
// FEES: 45000
// DETAILS OF STUDENTS
// ROLL NO. = 01
// NAME = Rahul
// COURSE = BCA
// FEES = 45000

#include <stdio.h>
#include <stdlib.h>
// #include <conio.h>
// #include <alloc.h>
#define clrscr() printf("\033[H\033[J")

struct student
{
    int r_no;
    char name[20];
    char course[20];
    int fees;
};
struct student *ptr_stud[10];
int main()
{
    int i, n;
    printf("\n Enter the number of students : ");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        ptr_stud[i] = (struct student *)malloc(sizeof(struct student));
        printf("\n Enter the data for student %d ", i + 1);
        printf("\n ROLL NO.: ");
        scanf("%d", &ptr_stud[i]->r_no);
        printf("\n NAME: ");
        gets(ptr_stud[i]->name);
        printf("\n COURSE: ");
        gets(ptr_stud[i]->course);
        printf("\n FEES: ");
        scanf("%d", &ptr_stud[i]->fees);
    }
    printf("\n DETAILS OF STUDENTS");
    for (i = 0; i < n; i++)
    {
        printf("\n ROLL NO. = %d", ptr_stud[i]->r_no);
        printf("\n NAME = %s", ptr_stud[i]->name);
        printf("\n COURSE = %s", ptr_stud[i]->course);
        printf("\n FEES = %d", ptr_stud[i]->fees);
    }
    return 0;
}