//  C:\\Intel\\student.txt
#include <stdio.h>  /*stdio.h header file contains the prototypes of the library functions 
printf(), scanf(), gets(), fopen(), fprintf() and fclose(), hence needs to be included using #include preprocessor directive*/
#include <stdlib.h> /*stdlib.h header file contains the prototype of the library function exit(),
hence needs to be included using #include preprocessor directive*/

int main()
{
    /*C program starts from main() function which will return an integer and there is no argument for the function;
    the opening curly brace indicates the start of the body of the main() function and the program execution starts
    from the first statement just after this brace*/

    char name[50];                               /*char type array is declared which can take only characters. Size of the array is 50,
                                      means it can take up to 50 characters. Each array elements can be accessed using indexes starting from 0 to 49,
                                      that is- name[0], name[1], . . ., name[49]*/
    float mark;                                  /*float type variable is declared which can take and store any decimal value*/
    FILE *fptr;                                  /*FILE is a structured data type defined in stdio.h header file.
                                     Here a FILE type pointer is declared to refer any file*/
    fptr = fopen("student.txt", "w"); /*fopen() function opens a text file named student.txt
    in the address C:\\Intel\\ with write mode, indicated by “w”, and returns a FILE type pointer*/
    if (fptr == NULL)
    {                                                        /*after opening a file in C, we must always check whether the file opens successfully.
                                              To do that we check the pointer returned by fopen() function. If it is NULL then file could not open. Hence,
                                              here if the file failed to open the condition is true and following two statements will be executed*/
        printf("Error in opening the file, exiting . . .");  /*output function printf() will display the message
         inside the double quotation as it is on the screen*/
        exit(1);                                             /*exit(1) function causes abnormal termination of the program.
                                                    All buffers, temporary files, streams are deleted or cleared before the termination*/
    }                                                        /*this closing curly brace indicates the end of the if body*/
    printf("Enter name of the student:");                    /*output function printf() will display the message inside
                           the double quotation as it is on the screen*/
    gets(name);                                              /*gets() is an input function that can take any string or array of characters,
                                                     including space, tab, etc., end with NULL character \0 and store the string to its parameter name*/
    printf("Enter marks of the student:");                   /*output function printf() will display the message inside the double quotation as it is on the screen*/
    scanf("%f", &mark);                                      /*scanf() function takes a decimal value from the standard input terminal and store the float value to the variable mark*/
    fprintf(fptr, "\nName: %s\nMarks: %0.2f\n", name, mark); /*fprintf() function will write information inside the double quotation as it is in the file referred by the pointer fptr. Here enter is written in place of new line character, string name in place of %s format specifier and decimal value mark in place of %0.2f format specifier with 2 point precision*/
    fclose(fptr);                                            /*once a file is open in C, it is a good practice to close the file before end of the program to properly flush the output buffer. This is done here by the fclose() function which close the file referred by its argument fptr*/
    return 0;                                                /*0 is returned as it is the standard for the successful execution of the program*/
}