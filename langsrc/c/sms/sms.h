

struct StudentInfo
{
    char ID[10];
    char Name[20];
    char Email[30];
    char Phone[20];
    int  NumberOfCourse;
};

struct CourseInfo
{
    char StudentID[10];
    char Code[10];
    char Name[20];
};
struct StudentInfo Students[100];
struct CourseInfo Courses[500];
int i,j;
int TotalStudents = 0;
int TotalCourse = 0;
char StudentID[10];

void Menu();
void AddNewStudent();
void ShowAllStudents();
int  SearchStudent(char StudentID[10]);
void EditStudent(int StudentFoundIndex);
void DeleteStudent(int StudentIndex);
void DeleteAllStudents();
int  IsAlreadyExists(char GivenLine[30],char InfoType, char StudentID[300]);
void ErrorAndRestart(char *Error[100]);
void DeleteCourseByIndex(int CourseIndex);
void DeleteStudentByIndex(int CourseIndex);
void GoBackOrExit();
void ExitProject();
void DataSeed();
