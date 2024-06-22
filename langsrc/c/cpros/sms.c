#include <stdio.h>
/*header file stdio.h contains prototypes of the library functions
like printf(), scanf(), fopen(), fclose(), fprint() etc., hence
needs to be included using preprocessor directive #include before
they can be used in the program*/

#include <stdlib.h>
/*header file stdlib.h contains prototypes of the library function
exit(), hence needs to be included using preprocessor directive
#include before they can be used in the program*/
struct subject
{
    /*structured data type subject is defined here using keyword struct;
    its members are declared within the curly braces*/
    int mark;
    /*int type variable mark is declared as member of the structured
    data type subject, that will be used to store the obtained mark
    in a particular subject*/
    float point;
    /*float type variable point is declared as member of the
    structured data type subject, that will be used to store the
    obtained grade-point in a particular subject*/
    char *letter;
    /*char type pointer letter is declared as member of the
    structured data type subject, that will be used to store the
    obtained letter-grade in a particular subject*/
};
/*definition of structured data type must end with a semicolon; no
variable of the defined data type marks is declared here*/
struct exam
{
    /*structured data type exam is defined here using keyword struct;
    its members are declared within the curly braces*/
    int roll, total, pos;
    /*int type variables roll, total and pos are declared as members
    of the structured data type exam, that will be used to store
    roll no, obtained total marks and merit position, respectively
    of a particular student*/
    struct subject sub[10];
    /*structured subject type array sub[] with size 10 is declared
    as member of the structured data type exam, that will be used to
    store information of maximum 10 subjects of a particular student*/
    float gpa;
    /*float type variable gpa is declared as member of the structured
    data type exam, that will be used to store gpa calculated for
    a particular student*/
};
/*definition of structured data type must end with a semicolon; no
variable of the defined data type marks is declared here*/
int main()
{
    /*here main() function returns an integer and parameters/arguments
    of the main() function also remain void; execution of the program
    starts with main() function; no statement before opening curly
    brace of the main() function will be executed by the compiler*/

    FILE *fp;
    /*FILE is a structured data type defined in stdio.h header file.
    Here a FILE type pointer fp is declared to refer any file*/
    struct exam stud[100], temp;
    /*structured exam type variable temp and an array stud[] with
    size 100 are declared here; members of each of the array element
    can be accessed through dot (.) operator*/
    int numstd, numsub, i, j;
    /*integer type variables numstd, numsub, i and j are declared;
    required memory spaces have been allocated for each of the
    variables*/
    float totgrd;
    /*float type variable totgrd is declared that can store any
    decimal value*/
    char ch;
    /*char type variable is declared which can take and store only
    character*/
    printf("Enter the number of students: ");
    /*output function printf() displays the message in the quotation
    as it is on screen*/
    scanf("%d", &numstd);
    /*number of student will be input using this input function
    scanf() which will take decimal value from input terminal and
    keep the value in the memory spaces allocated for the variable
    numstd*/
    printf("How many subjects: ");
    /*output function printf() displays the message in the quotation
    as it is on screen*/
    scanf("%d", &numsub);
    /*number of subject will be input using this input function
    scanf() which will take decimal value from input terminal and
    keep the value in the memory spaces allocated for the variable
    numsub*/
    for (i = 0; i < numstd; i++)
    {
        /*here i=0 is initialization, i<numstd is condition and i++ is
        increment; this for loop works as follows- initialization is
        done only once at the start; then condition is checked, if it is
        true statements in the body, enclosed by curly braces, execute
        and value of i is incremented by 1; now the condition is checked
        again and if it is true the body executes again; these steps
        continue till the condition becomes false; this for loop is
        used here to input necessary information of all numstd students
        and calculate marks, gpa and letter-grade; in each iteration
        individual student is considered*/
        printf("Enter the roll no. of student #%d: ", i + 1);
        /*output function printf() displays the message in the
        quotation as it is on screen except format specifier %d is
        replaced by the value of i+1*/

        scanf("%d", &stud[i].roll);
        /*this scanf() function takes a decimal value from input
        terminal and save the value in roll which is a member of
        struct subject type array stud[]*/
        stud[i].total = 0;
        /*member total of structured subject type array stud[] is
        initialized to 0 that will be used to store the total marks
        of a particular student; it is initialized to 0, otherwise
        the summation operation may give wrong answer because, when
        we declare a variable a memory space is allocated for that
        variable and the memory space may contain some garbage
        value; when we sequentially add the data with the total
        and result will be accumulated in the total variable, the
        garbage value may add up in the first summation*/
        totgrd = 0;
        /*variable totgrd is initialized to 0 that will be used to store
        the total grade of a particular student needed to calculate
        gpa; it is initialized to 0 to avoid add up any garbage value
        in the first summation and get any wrong answer*/
        for (j = 0; j < numsub; j++)
        {
            /*here j=0 is initialization, j<numsub is condition
            and j++ is increment; this for loop works as followsinitialization is done only once at the start; then
            condition is checked, if it is true statements in the
            body, enclosed by curly braces, execute and value of j
            is incremented by 1; now the condition is checked again
            and if it is true the body executes again; these steps
            continue till the condition becomes false; this for loop
            is used here to input necessary information of all numsub
            subjects and calculate marks, gpa and letter-grade; in
            each iteration individual subject is considered*/
            printf("Mark obtained in EEE-110%d: ", j + 1);
            /*output function printf() displays the message in the
            quotation as it is on screen except format specifier %d
            is replaced by the value of j+1*/
            scanf("%d", &stud[i].sub[j].mark);
            /*this scanf() function takes a decimal value from
            input terminal and save the value in mark which is a
            member of struct subject type array sub[] which is in
            turn is a member of structured exam type array stud[],
            hence variable mark is accessed through double dot (.)
            operators*/
            if (stud[i].sub[j].mark >= 90)
            {
                /*if the condition is true, means marks of a particular
                subject of a particular student is greater than or
                equal to 90, then following two statements execute;
                multiple statements in the body of 'if' is enclosed
                with curly braces*/

                stud[i].sub[j].point = 4.00;
                /*point is a member of struct subject type array
                sub[] which is in turn is a member of structured
                exam type array stud[], hence variable point is
                accessed through chain of dot (.) operators and
                assigned a value 4.00*/
                stud[i].sub[j].letter = "A+";
                /*character type pointer letter is a member of
                struct subject type array sub[] which is in turn
                is a member of structured exam type array stud[],
                hence variable point is accessed through chain of
                dot (.) operators and assigned a string A+*/
            }
            /*this closing curly brace indicates end of if (stud[i].
            sub[j].mark>=90)*/
            else if (stud[i].sub[j].mark >= 80)
            {
                /*if the condition is true, means marks of a particular
                subject of a particular student is greater than or equal to
                80 but less than 90, then following two statements execute;
                condition of 90 has already been checked in the previous
                'if' condition and condition of 80 is only checked if the
                above 'if' condition of 90 is false; multiple statements
                in the body of 'if' is enclosed with curly braces*/
                stud[i].sub[j].point = 3.50;
                /*point is a member of struct subject type array sub[]
                which is in turn is a member of structured exam type
                array stud[], hence variable point is accessed through
                chain of dot (.) operators and assigned a value 3.50*/
                stud[i].sub[j].letter = "A";
                /*character type pointer letter is a member of
                struct subject type array sub[] which is in turn
                is a member of structured exam type array stud[],
                hence variable point is accessed through chain of
                dot (.) operators and assigned a string A*/
            }
            /*this closing curly brace indicates end of else if
            (stud[i].sub[j].mark>=80)*/
            else if (stud[i].sub[j].mark >= 70)
            {
                /*if the condition is true, means marks of a particular
                subject of a particular student is greater than or equal
                to 70 but less than 80, then following two statements
                execute; condition of 90 and 80 have already been
                checked in the previous 'if' and 'else if' conditions
                and condition of 70 is only checked if the both of
                the above 'if' and 'else if' conditions of 90 and 80
                are false; multiple statements in the body of 'if' is
                enclosed with curly braces*/

                stud[i].sub[j].point = 3.00;
                /*point is a member of struct subject type array
                sub[] which is in turn is a member of structured
                exam type array stud[], hence variable point is
                accessed through chain of dot (.) operators and
                assigned a value 3.00*/
                stud[i].sub[j].letter = "B";
                /*character type pointer letter is a member of
                struct subject type array sub[] which is in turn
                is a member of structured exam type array stud[],
                hence variable point is accessed through chain of
                dot (.) operators and assigned a string B*/
            }
            /*this closing curly brace indicates end of else if
            (stud[i].sub[j].mark>=70)*/
            else if (stud[i].sub[j].mark >= 60)
            {
                /*if the condition is true, means marks of a particular
                subject of a particular student is greater than or equal
                to 60 but less than 70, then following two statements
                execute; this condition of 60 is checked only if all the
                above 'if' and 'else if' conditions are false*/
                stud[i].sub[j].point = 2.50;
                /*point is a member of struct subject type array
                sub[] which is in turn is a member of structured
                exam type array stud[], hence variable point is
                accessed through chain of dot (.) operators and
                assigned a value 2.50*/
                stud[i].sub[j].letter = "C";
                /*character type pointer letter is a member of
                struct subject type array sub[] which is in turn
                is a member of structured exam type array stud[],
                hence variable point is accessed through chain of
                dot (.) operators and assigned a string C*/
            }
            /*this closing curly brace indicates end of else if
            (stud[i].sub[j].mark>=60)*/
            else if (stud[i].sub[j].mark >= 50)
            {
                /*if the condition is true, means marks of a particular
                subject of a particular student is greater than or equal
                to 50 but less than 60, then following two statements
                execute; this condition of 50 is checked only if all the
                above 'if' and 'else if' conditions are false*/
                stud[i].sub[j].point = 2.00;
                /*point is a member of struct subject type array
                sub[] which is in turn is a member of structured
                exam type array stud[], hence variable point is
                accessed through chain of dot (.) operators and

                assigned a value 2.00*/
                stud[i].sub[j].letter = "D";
                /*character type pointer letter is a member of
                struct subject type array sub[] which is in turn
                is a member of structured exam type array stud[],
                hence variable point is accessed through chain of
                dot (.) operators and assigned a string D*/
            }
            /*this closing curly brace indicates end of else if
            (stud[i].sub[j].mark>=50)*/
            else
            {
                /*if all the above conditions of 'if' and 'else if'
                are false, means marks of a particular subject of a
                particular student is less than 50, then following two
                statements execute*/
                stud[i].sub[j].point = 0.00;
                /*point is a member of struct subject type array
                sub[] which is in turn is a member of structured
                exam type array stud[], hence variable point is
                accessed through chain of dot (.) operators and
                assigned a value 0.00*/
                stud[i].sub[j].letter = "F";
                /*character type pointer letter is a member of
                struct subject type array sub[] which is in turn
                is a member of structured exam type array stud[],
                hence variable point is accessed through chain of
                dot (.) operators and assigned a string F*/
            }
            /*this closing curly brace indicates end of above
            'else'*/
            stud[i].total += stud[i].sub[j].mark;
            /*this arithmetic operation calculates total marks
            of a particular student by adding obtained marks of
            all the subjects; in each iteration of first 'for'
            loop one particular student is considered and in each
            iteration of second 'for' loop individual subject of
            that particular student is considered*/
            totgrd += stud[i].sub[j].point;
            /*this arithmetic operation calculates total grades of
            a particular student by adding all grade-points of
            individual subject; in each iteration of first 'for'
            loop one particular student is considered and in each
            iteration of second 'for' loop individual subject of
            that particular student is considered*/
        }
        /*this closing curly brace indicates end of for (j=0;
        j<numsub; j++)*/

        stud[i].gpa = totgrd / numsub;
        /*gpa of each student is calculated from total grade-point
        totgrd of that particular student; in each iteration of
        first 'for' loop individual student is considered*/
    }
    /*this closing curly brace indicates end of for (i=0; i<numstd;
    i++)*/
    for (i = 1; i <= numstd - 1; i++)
        /* this for loop is used to repeat the following 'for' loop as
        long as the condition satisfies; here i=1 is initialization,
        i<=numstd-1 is condition and i++ is increment; this for loop
        works as follows- initialization is done only once at the start;
        then condition is checked, if it is true statement in the body
        executes and value of i is incremented by 1; now the condition
        is checked again and if it is true the body executes again;
        these steps continue till the condition becomes false; this for
        loop together with the following for loop are used here to sort
        students based on their earned gpa in ascending order*/
        for (j = 1; j <= numstd - i; j++)
            /*this for loop is used to compare gpa of each student
            and swap if necessary to place one student in its right
            position in each iteration; hence after each iteration,
            number of gpa of students to be compared is decreased
            by 1, as the student which is already placed in right
            position in previous iteration does not need to be compared
            again in the subsequent iteration; here j=1 is initialization,
            j<numstd-i is condition and j++ is increment; this for loop
            works as follows- initialization is done only once at the
            start; then condition is checked, if it is true statement in
            the body executes and value of j is incremented by 1; now the
            condition is checked again and if it is true the body executes
            again; these steps continue till the condition becomes false*/
            if (stud[j - 1].gpa < stud[j].gpa)
            {
                /*body of above 'for' loop contains only this 'if'
                condition; if the condition of 'if' is true then following
                three statements will execute; as there are multiple
                statements in the body of 'if' they are enclosed with
                curly braces; here gpa of two contiguous students are
                compared, if the larger gpa stays above the smaller one
                then they are swapped to always keep the student with
                smaller gpa above the larger one*/
                temp = stud[j - 1];
                /*array element stud[j-1] is stored in variable temp*/
                stud[j - 1] = stud[j];
                /*array element stud[j] is stored as array
                element stud[j-1], that is and stud[j] now becomes
                stud[j-1]*/

                stud[j] = temp;
                /*value of temp is stored as array element stud[j],
                that is stud[j-1] now becomes stud[j]*/
            }
            /*this closing curly brace indicates the end of 'if'*/
            else if (stud[j - 1].gpa == stud[j].gpa)
                /*if above 'if' condition is false then this 'else if'
                condition is checked; if this condition is true, means
                gpa of two contiguous students are equal then following
                'if' condition executes to sort students according to
                their obtained total marks in ascending order*/
                if (stud[j - 1].total < stud[j].total)
                {
                    /*if this condition is true then following three
                    statements will execute; means- here total of two
                    contiguous students are compared, if the larger
                    total stays above the smaller one then they are
                    swapped to always keep the student with smaller
                    total above the larger one*/
                    temp = stud[j - 1];
                    /*array element stud[j-1] is stored in variable
                    temp*/
                    stud[j - 1] = stud[j];
                    /*array element stud[j] is stored as array
                    element stud[j-1], that is and stud[j] now
                    becomes stud[j-1]*/
                    stud[j] = temp;
                    /*value of temp is stored as array element
                    stud[j], that is stud[j-1] now becomes
                    stud[j]*/
                }
    /*this closing curly brace indicates the end of
    'if' with condition*/
    for (i = 0; i < numstd; i++)
        /*this for loop is used to set position of each student
        according to sorted gpa; here i=0 is initialization, i<numstd is
        condition and i++ is increment; this for loop works as followsinitialization is done only once at the start; then condition is
        checked, if it is true statement in the body executes and value
        of i is incremented by 1; now the condition is checked again
        and if it is true the body executes again; these steps continue
        till the condition becomes false*/
        stud[i].pos = i + 1;
    /*member pos of structured exam type array stud[] is set to
    i+1; at 1st iteration position of first student is set to
    stud[0].pos=1, at 2nd iteration position of second student
    is set to stud[1].pos=2, and so on*/

    fp = fopen("c:\\myfiles\\Results.txt", "w");
    /*fopen() function opens a text file named Results.txt in the
    address C:\\myfiles\\ with write mode, indicated by “w”, and
    returns a FILE type pointer*/
    if (fp == NULL)
    {
        /*after opening a file in C, we must always check whether
        the file opens successfully; to do that we check the pointer
        returned by fopen() function; if it is NULL then file could not
        open; hence, here if the file failed to open the condition is
        true and following two statements will be executed*/
        puts("Cannot open source file!\n");
        /*output function puts() will display the string message
        inside the double quotation as it is on screen except
        newline character \n is replaced by enter*/
        exit(1);
        /*exit(1) function causes abnormal termination of the
        program; all buffers, temporary files, streams are deleted
        or cleared before the termination*/
    }
    /*this closing curly brace indicates the end of the 'if' body*/
    fprintf(fp, "\nResult according to merit position . . . \n");
    /*fprintf() function writes information inside the double
    quotation as it is in the file referred by the pointer fp; here
    enter is written in place of new line character*/
    for (i = 0; i < numstd; i++)
    {
        /*this for loop is used to display information of each student
        according to sorted gpa; in each iteration individual student is
        considered; here i=0 is initialization, i<numstd is condition and
        i++ is increment; this for loop works as follows- initialization
        is done only once at the start; then condition is checked, if
        it is true statements in the body execute and value of i is
        incremented by 1; now the condition is checked again and if it
        is true the body executes again; these steps continue till the
        condition becomes false*/
        fprintf(fp, "Roll No.: %d\n", stud[i].roll);
        /*fprintf() function will write information inside the
        double quotation as it is in the file referred by the pointer
        fp; here enter is written in place of new line character,
        value of stud[i].roll in place of %d format specifier*/
        for (j = 0; j < numsub; j++)
            /*this for loop is used to display information of all subjects
            of each student according to sorted gpa; in each iteration
            individual subject of a particular student is considered;
            here j=0 is initialization, j<numsub is condition and j++ is
            increment; this for loop works as follows- initialization
            is done only once at the start; then condition is checked,

            if it is true statements in the body execute and value of i
            is incremented by 1; now the condition is checked again and
            if it is true the body executes again; these steps continue
            till the condition becomes false*/
            fprintf(fp, "EEE-110%d: %s (%0.2f)\n", j + 1, stud[i].sub[j].letter, stud[i].sub[j].point);
        /*fprintf() function writes information in the quotation
        as it is in the file referred by the pointer fp; here
        format specifier %d is replaced by the value of j+1,
        %s by string value of stud[i].sub[j].letter, %0.2f by
        stud[i].sub[j].point with 2 decimal point precision and
        newline character \n by enter*/
        fprintf(fp, "GPA: %0.2f\nMerit Position: %d\n\n", stud[i].gpa, stud[i].pos);
        /*fprintf() function writes information in the quotation as
        it is in the file referred by the pointer fp; here format
        specifier %d is replaced by the value of stud[i].pos, %0.2f
        by stud[i].gpa with 2 decimal point precision and newline
        character \n by enter*/
    }
    /*this closing curly brace indicates the end of for (i=0;
    i<numstd; i++) loop*/
    fclose(fp);
    /*once a file is open in C, it is a good practice to close the
    file before end of the program or reuse the file to properly
    flush the output buffer; this is done here by the fclose()
    function which close the file referred by its argument fp*/
    for (i = 1; i <= numstd - 1; i++)
        /* this for loop is used to repeat the following 'for' loop as
        long as the condition satisfies; here i=1 is initialization,
        i<=numstd-1 is condition and i++ is increment; this for loop
        works as follows- initialization is done only once at the start;
        then condition is checked, if it is true statement in the body
        executes and value of i is incremented by 1; now the condition
        is checked again and if it is true the body executes again;
        these steps continue till the condition becomes false; this for
        loop together with the following for loop are used here to sort
        students according to their roll no in ascending order*/
        for (j = 1; j <= numstd - i; j++)
            /*this for loop is used to compare roll of each student
            and swap if necessary to place one student in its right
            position in each iteration; hence after each iteration,
            number of roll of students to be compared is decreased by 1,
            as the student which is already placed in right position in
            previous iteration does not need to be compared again in the
            subsequent iteration; here j=1 is initialization, j<numstd-i
            is condition and j++ is increment; this for loop works as

            follows- initialization is done only once at the start; then
            condition is checked, if it is true statement in the body
            executes and value of j is incremented by 1; now the condition
            is checked again and if it is true the body executes again;
            these steps continue till the condition becomes false*/
            if (stud[j - 1].roll > stud[j].roll)
            {
                /*if this condition is true then following three
                statements in the body of if, enclosed by curly braces,
                will execute; means- here roll of two contiguous
                students are compared, if the larger roll stays above
                the smaller one then they are swapped to always keep
                the student with smaller roll above the larger one*/
                temp = stud[j - 1];
                /*array element stud[j-1] is stored in variable temp*/
                stud[j - 1] = stud[j];
                /*array element stud[j] is stored as array element
                stud[j-1], that is and stud[j] now becomes stud[j-1]*/
                stud[j] = temp;
                /*value of temp is stored as array element
                stud[j], that is stud[j-1] now becomes stud[j]*/
            }
    /*this closing curly brace indicates the end of 'if'
    condition*/
    fp = fopen("c:\\myfiles\\Results.txt", "a");
    /*fopen() function opens the same text file, in which some information
    was saved, named Results.txt in the address C:\\myfiles\\ with
    append mode, indicated by “a”, and returns a FILE type pointer*/
    if (fp == NULL)
    {
        /*after opening a file in C, we must always check whether
        the file opens successfully; to do that we check the pointer
        returned by fopen() function; if it is NULL then file could not
        open; hence, here if the file failed to open the condition is
        true and following two statements will be executed*/
        puts("Cannot open target file!\n");
        /*output function puts() will display the string message
        inside the double quotation as it is on screen except
        newline character \n is replaced by enter*/
        exit(1);
        /*exit(1) function causes abnormal termination of the
        program; all buffers, temporary files, streams are deleted
        or cleared before the termination*/
    }
    /*this closing curly brace indicates the end of the 'if' body*/
    fprintf(fp, "\nResult according to roll no . . . . \n");
    /*fprintf() function will write information inside the double
    quotation as it is in the file referred by the pointer fp; here
    enter is written in place of new line character \n*/

    for (i = 0; i < numstd; i++)
    {
        /*this for loop is used to display information of each student
        according to sorted roll; in each iteration individual student
        is considered; here i=0 is initialization, i<numstd is
        condition and i++ is increment; this for loop works as followsinitialization is done only once at the start; then condition is
        checked, if it is true statements in the body execute and value
        of i is incremented by 1; now the condition is checked again
        and if it is true the body executes again; these steps continue
        till the condition becomes false*/
        fprintf(fp, "Roll No.: %d\n", stud[i].roll);
        /*fprintf() function will write information inside the
        double quotation as it is in the file referred by the pointer
        fp; here enter is written in place of newline character,
        value of stud[i].roll in place of %d format specifier*/
        for (j = 0; j < numsub; j++)
            /*this for loop is used to display information of all subjects
            of each student according to sorted gpa; in each iteration
            individual subject of a particular student is considered;
            here j=0 is initialization, j<numsub is condition and j++ is
            increment; this for loop works as follows- initialization
            is done only once at the start; then condition is checked,
            if it is true statements in the body execute and value of i
            is incremented by 1; now the condition is checked again and
            if it is true the body executes again; these steps continue
            till the condition becomes false*/
            fprintf(fp, "EEE-110%d: %s (%0.2f)\n", j + 1, stud[i].sub[j].letter, stud[i].sub[j].point);
        /*fprintf() function writes information in the quotation
        as it is in the file referred by the pointer fp; here
        format specifier %d is replaced by the value of j+1,
        %s by string value of stud[i].sub[j].letter, %0.2f by
        stud[i].sub[j].point with 2 decimal point precision and
        newline character \n by enter*/
        fprintf(fp, "GPA: %0.2f\nMerit Position: %d\n\n", stud[i].gpa, stud[i].pos);
        /*fprintf() function writes information in the quotation as
        it is in the file referred by the pointer fp; here format
        specifier %d is replaced by the value of stud[i].pos, %0.2f
        by stud[i].gpa with 2 decimal point precision and newline
        character \n by enter*/
    }
    /*this closing curly brace indicates the end of for (i=0;
    i<numstd; i++) loop*/
    fclose(fp);
    /*once a file is open in C, it is a good practice to close the
    file before end of the program or reuse the file to properly
    flush the output buffer; this is done here by the fclose()
    function which close the file referred by its argument fp*/

    fp = fopen("c:\\myfiles\\Results.txt", "r");
    /*fopen() function opens a text file, same file used before
    to write information of the students, named Result.txt in the
    address C:\\my files\\ with read mode, indicated by “r”, and
    returns a FILE type pointer*/
    if (fp == NULL)
    {
        /*after opening a file in C, we must always check whether
        the file opens successfully; to do that we check the pointer
        returned by fopen() function; if it is NULL then file could
        not open; hence, here if the file failed to open the condition
        is true and following two statements execute*/
        puts("Cannot open target file!\n");
        /*output function puts() will display the string message
        inside the double quotation as it is on screen except
        newline character \n is replaced by enter*/
        exit(1);
        /*exit(1) function causes abnormal termination of the
        program; all buffers, temporary files, streams are deleted
        or cleared before the termination*/
    }
    /*this closing curly brace indicates the end of the 'if'
    body*/
    printf("\n");
    /*output function printf() displays enter due to newline
    character \n*/
    while (1)
    {
        /*as the 1 means true, the condition of 'while' is always true
        and following statements in the body of while, enclosed by curly
        braces, execute in any case; in each iteration character is read
        sequentially from the file and displayed on screen*/
        ch = fgetc(fp);
        /*fgetc() function reads a character from the file referred by
        the pointer fp; at 1st iteration, it reads the 1st character
        (ch=1st character) and the pointer automatically points
        to the 2nd character; at 2nd iteration, it reads the 2nd
        character (ch=2nd character) and the pointer automatically
        points to the 3rd character; and so on*/
        if (ch == EOF)
            /*ch is compared with EOF, means whether we reach the endof-file or not; if yes, then the program steps out of the
            while loop*/
            break;
        /*program control immediately comes out of the while
        loop skipping following statement*/
        printf("%c", ch);
        /*output function printf() displays character ch due to
        format specifier %c*/
    }
    /*this closing curly brace indicates the end of while loop*/

    fclose(fp);
    /*once a file is open in C, it is a good practice to close the
    file before end of the program or reuse the file to properly
    flush the output buffer; this is done here by the fclose()
    function which close the file referred by its argument fp*/
    return 0;
    /*0 is returned as it is the standard for the successful
    execution of the program*/
}
/*the closing curly brace indicates the end of the body of main()
function and the end of the program; no statement will execute after
that curly brace*/