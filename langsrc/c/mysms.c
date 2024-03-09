#include <windows.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sms.h"





// some global variables
// extern int i,j;
// extern int TotalStudents = 0;
// extern int TotalCourse = 0;
// extern char StudentID[10];

extern int i,j;
extern int TotalStudents;
extern int TotalCourse ;
extern char StudentID[10];

// FILE *AllStudents;
// FILE *AllCourses;
// FILE *ExistingAllStudents;
// FILE *TempAllStudents;
// FILE *ExistingAllCourses;
// FILE *TempAllCourses;
// end

bool IsRunning = true;

int main()
{
    DataSeed(); // you can comment this line if not want dummy data

    while(IsRunning)
    {
        Menu();
        int Option;
        scanf("%d",&Option);
        switch(Option)
        {
        case 0:
            IsRunning = false;
            ExitProject();
            break;
        case 1:
            system("cls");
            printf("\n **** Add A New Student ****\n\n");
            AddNewStudent();
            GoBackOrExit();
            break;
        case 2:
            system("cls");
            printf("\n **** All Students ****\n\n");
            ShowAllStudents();
            GoBackOrExit();
            break;
        case 3:
        {
            system("cls");
            printf("\n **** Search Students ****\n\n");
            printf(" Enter The Student ID: ");
            scanf("%s",StudentID);
            int IsFound = SearchStudent(StudentID);
            if(IsFound<0)
            {
                printf(" No Student Found\n\n");
            }
            printf("\n");
            GoBackOrExit();
            break;
        }
        case 4:
            system("cls");
            printf("\n **** Edit a Student ****\n\n");
            printf(" Enter The Student ID: ");
            scanf("%s",StudentID);
            int StudentFoundIndex = SearchStudent(StudentID);

            if(StudentFoundIndex>=0)
            {
                EditStudent(StudentFoundIndex);
            }
            else
            {
                printf(" No Student Found\n\n");
            }
            GoBackOrExit();
            break;
        case 5:
            system("cls");
            printf("\n **** Delete a Student ****\n\n");
            printf(" Enter The Student ID: ");
            scanf("%s",StudentID);

            int DeleteStudentFoundIndex = SearchStudent(StudentID);

            if(DeleteStudentFoundIndex>=0)
            {
                char Sure = 'N';
                getchar();
                printf("\n\n");
                printf(" Are you sure want to delete this student? (Y/N): ");
                scanf("%c",&Sure);

                if(Sure == 'Y' || Sure == 'y')
                {
                    DeleteStudent(DeleteStudentFoundIndex);
                }
                else
                {
                    printf(" Your Data is Safe.\n\n");
                    GoBackOrExit();
                }

            }
            else
            {
                printf(" No Student Found\n\n");
                GoBackOrExit();
            }

            break;
        case 6:
        {
            char Sure = 'N';
            getchar();
            system("cls");
            printf("\n **** Delete ALL Students ****\n\n");

            printf(" Are you sure want to delete all the students? (Y/N): ");
            scanf("%c",&Sure);
            if(Sure == 'Y' || Sure == 'y')
            {
                DeleteAllStudents();
            }
            else
            {
                printf(" Your Data is Safe.\n\n");
                GoBackOrExit();
            }
            break;
        }

        case 7:
            system("cls");
            break;
        default:
            ExitProject();
            break;
        }
    }

    return 0;
} // end main function

