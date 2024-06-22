#include <stdio.h>  /*stdio.h header file contains the prototypes of the library functions printf(), fgetc(), fopen() and fclose(), hence needs to be included using #include preprocessor directive*/
#include <stdlib.h> /*stdlib.h header file contains the prototype of the library function exit(), hence needs to be included using #include preprocessor directive*/
int main()
{                                                /*C program starts from main() function which will return an integer and there is no argument for the function; the opening curly brace indicates the start of the body of the main() function and the program execution starts from the first statement just after this brace*/
    char ch;                                     /*char type variable is declared which can take and store only character*/
    FILE *fptr;                                  /*FILE is a structured data type defined in stdio.h header file. Here a FILE type pointer is declared to refer any file*/
    fptr = fopen("student.txt", "r"); /*fopen() function opens a text file named student.txt in the address C:\\Intel\\ with read mode, indicated by “r”, and returns a FILE type pointer*/
    if (fptr == NULL)
    {                                                       /*after opening a file in C, we must always check whether the file opens successfully. To do that we check the pointer returned by fopen() function. If it is NULL then file could not open. Hence, here if the file failed to open the condition is true and following two statements will be executed*/
        printf("Error in opening the file, exiting . . ."); /*output function printf() will display the message inside the double quotation as it is on the screen*/
        exit(1);                                            /*exit(1) function causes abnormal termination of the program. All buffers, temporary files, streams are deleted or cleared before the termination*/
    }                                                       /*this closing curly brace indicates the end of the if body*/
    printf("Contents of the file are:\n");                  /*output function printf() will display the message inside the double quotation as it is on the screen*/
    ch = fgetc(fptr);                                       /*fgetc() function reads a character from the file referred by the pointer fptr. At start, it reads the 1st character and the pointer automatically points to the next character. Here ch=1st character, pointer points to the 2nd character*/
    while (ch != EOF)
    {                     /*ch is compared with EOF, means whether we reach the end-of- file or not. If not, then statements inside the body of the while loop will execute*/
        printf("%c", ch); /*output function printf() will display the character corresponding to ch in place of format specifier %c on screen*/
        ch = fgetc(fptr); /*now fgetc() function reads 2nd character in the file in the 1st iteration and pointer points to the 3rd character. This ch=2nd character is compared with EOF and if we don’t reach at the end-of-the file, printf() function prints ch=2nd character and fgetc() function reads the 3rd character. This continues till we reach the EOF in which case the condition becomes false and the program steps out of the while loop*/
    }                     /*this closing curly brace indicates the end of the while loop*/
    fclose(fptr);         /*once a file is open in C, it is a good practice to close the file before end of the program to properly flush the output buffer. This is done here by the fclose() function which close the file referred by its argument fptr*/
    return 0;             /*0 is returned as it is the standard for the successful execution of the program*/
} /*the closing curly brace indicates the end of the body of main() function and the end of the program. No statement will execute after that curly brace*/