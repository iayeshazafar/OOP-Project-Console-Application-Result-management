#include <iostream>
#include <iomanip>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <thread>
#include <chrono>

using namespace std;

// ===== Utilities =====
void gotoxy(int x,int y){
    COORD c; c.X=x; c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
void setColor(int c){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

// ===== Structures =====
struct Student{
    string name, father, institute, degree, session;
}st;

struct Subject{
    string code, title;
    int marks;
    int creditHours;
    char grade;
    float gpa;
};

// ===== Global Variables =====
vector<Subject> sem1, sem2;
int totalMarks=0;
float totalWeightedGPA=0;
int totalCreditHours=0;

// ===== Grade Functions =====
char calcGrade(int marks){
    if(marks>=85) return 'A';
    if(marks>=70) return 'B';
    if(marks>=60) return 'C';
    if(marks>=50) return 'D';
    return 'F';
}
float calcGPA(char grade){
    if(grade=='A') return 4.0;
    if(grade=='B') return 3.0;
    if(grade=='C') return 2.0;
    if(grade=='D') return 1.0;
    return 0.0;
}

// ===== Splash Screen =====
void splashScreen(){
    system("cls");
    
    string uni = "University of the Punjab";
    string degree = "Bachelor's Studies Information Technology";
    int colors[] = {11,13,14,10,9,12};

    // University name
    gotoxy((120-uni.length())/2,5);
    for(size_t i=0;i<uni.length();i++){
        setColor(colors[i%6]);
        cout<<uni[i]; cout.flush();
        this_thread::sleep_for(chrono::milliseconds(40));
    }

    // Degree name
    gotoxy((120-degree.length())/2,7);
    for(size_t i=0;i<degree.length();i++){
        setColor(colors[(i+2)%6]);
        cout<<degree[i]; cout.flush();
        this_thread::sleep_for(chrono::milliseconds(30));
    }

    // Loading bar
    gotoxy((120-30)/2,10);
    setColor(14);
    cout<<"Loading: [";
    for(int i=0;i<20;i++) cout<<" ";
    cout<<"]";

    for(int i=0;i<=20;i++){
        gotoxy((120-30)/2 + 10 + i,10);
        setColor(colors[i%6]);
        cout<<char(219); cout.flush();
        this_thread::sleep_for(chrono::milliseconds(120));
    }

    // Dots animation
    gotoxy((120-30)/2,12);
    for(int j=0;j<3;j++){
        for(int i=0;i<6;i++){
            setColor(colors[i%6]);
            cout<<"."; cout.flush();
            this_thread::sleep_for(chrono::milliseconds(150));
        }
        gotoxy((120-30)/2,12);
        cout<<"      "; // clear dots
    }

    this_thread::sleep_for(chrono::milliseconds(300));

    // Clear and reset to black
    system("cls");
    setColor(7);
}

// ===== Draw Box =====
void drawBox(int x,int y,int w,int h,int col){
    setColor(col);
    gotoxy(x,y); cout<<char(201); for(int i=0;i<w;i++) cout<<char(205); cout<<char(187);
    for(int i=1;i<=h;i++){
        gotoxy(x,y+i); cout<<char(186);
        gotoxy(x+w+1,y+i); cout<<char(186);
    }
    gotoxy(x,y+h+1); cout<<char(200); for(int i=0;i<w;i++) cout<<char(205); cout<<char(188);
    setColor(7);
}

// ===== Layout =====
void drawLayout(){
    drawBox(2,2,112,8,11);
    setColor(11); gotoxy(44,3); cout<<"University of the Punjab";
    setColor(13); gotoxy(38,4); cout<<"Bachelor's Studies Information Technology";

    drawBox(2,11,55,18,9);
    drawBox(59,11,55,18,9);
    setColor(14); gotoxy(5,12); cout<<"FIRST SEMESTER";
    gotoxy(62,12); cout<<"SECOND SEMESTER";

    setColor(11);
    gotoxy(4,13); cout<<"Code";
    gotoxy(12,13); cout<<"Title";
    gotoxy(36,13); cout<<"Marks";
    gotoxy(42,13); cout<<"CH";
    gotoxy(47,13); cout<<"Grade";
    gotoxy(52,13); cout<<"GPA";

    gotoxy(61,13); cout<<"Code";
    gotoxy(69,13); cout<<"Title";
    gotoxy(93,13); cout<<"Marks";
    gotoxy(99,13); cout<<"CH";
    gotoxy(104,13); cout<<"Grade";
    gotoxy(109,13); cout<<"GPA";

    drawBox(2,30,112,10,13);
    setColor(13); gotoxy(5,31); cout<<"DATA ENTRY AREA";

    drawBox(2,41,112,3,11);
    gotoxy(5,42); setColor(11);
    cout<<"Programmed By: <<Ayesha Zafar>>| C++ Console Project";
    setColor(7);
}

// ===== Student Info =====
void inputStudentInfo(){
    setColor(14);
    gotoxy(5,6); cout<<"Student Name  : "; setColor(7); getline(cin, st.name);
    setColor(14); gotoxy(5,7); cout<<"Father Name   : "; setColor(7); getline(cin, st.father);
    setColor(14); gotoxy(5,8); cout<<"Institute     : "; setColor(7); getline(cin, st.institute);
    setColor(14); gotoxy(60,6); cout<<"Degree        : "; setColor(7); getline(cin, st.degree);
    setColor(14); gotoxy(60,7); cout<<"Session       : "; setColor(7); getline(cin, st.session);
}

// ===== Display Semester =====
void displaySemester(vector<Subject>& sem,int startX){
    int y=15;
    for(auto &s: sem){
        gotoxy(startX,y); cout<<left<<setw(8)<<s.code;
        gotoxy(startX+8,y); cout<<left<<setw(24)<<s.title.substr(0,23);
        gotoxy(startX+32,y); cout<<setw(5)<<s.marks;
        gotoxy(startX+38,y); cout<<setw(4)<<s.creditHours;
        if(s.grade=='F') setColor(12); else setColor(9);
        gotoxy(startX+43,y); cout<<setw(5)<<s.grade;
        gotoxy(startX+48,y); cout<<fixed<<setprecision(1)<<s.gpa;
        setColor(7);
        y++;
    }
}

// ===== Stats =====
void showStats(){
    gotoxy(5,39); setColor(11);
    cout<<"Total Marks: "<<totalMarks;
    gotoxy(35,39); cout<<"Average GPA: ";
    if(totalCreditHours>0)
        cout<<fixed<<setprecision(2)<<totalWeightedGPA/totalCreditHours;
    else cout<<"0.00";
    setColor(7);
}

// ===== Add Subject =====
void addSubject(vector<Subject>& sem,int startX){
    Subject s;
    bool errorFlag;
    do{
        errorFlag = false;
        try{
            gotoxy(5,33); cout<<string(60,' ');
            gotoxy(5,34); cout<<string(60,' ');
            gotoxy(5,35); cout<<string(60,' ');
            gotoxy(5,36); cout<<string(60,' ');
            gotoxy(5,37); cout<<string(60,' ');

            gotoxy(5,33); cout<<"Subject Code   : "; cin>>s.code;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');

            gotoxy(5,34); cout<<"Title          : "; getline(cin,s.title);

            gotoxy(5,35); cout<<"Marks          : "; 
            if(!(cin >> s.marks)) throw runtime_error("Invalid input");
            if(s.marks<0||s.marks>100) throw out_of_range("marks");

            gotoxy(5,36); cout<<"Credit Hours   : "; 
            if(!(cin >> s.creditHours)) throw runtime_error("Invalid input");
            if(s.creditHours<1||s.creditHours>4) throw out_of_range("ch");

        }catch(...){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            gotoxy(5,37); cout<<"Invalid input! Press Enter.";
            cin.get();
            errorFlag = true;
        }
    }while(errorFlag);

    gotoxy(5,37); cout<<string(60,' ');

    s.grade = calcGrade(s.marks);
    s.gpa   = calcGPA(s.grade);

    totalMarks += s.marks;
    totalWeightedGPA += s.gpa * s.creditHours;
    totalCreditHours += s.creditHours;

    sem.push_back(s);
    displaySemester(sem,startX);
    showStats();
}

// ===== Save =====
void saveToFile(){
    string fname;
    gotoxy(5,34); cout<<string(60,' '); gotoxy(5,34); cout<<"Enter filename to save (with .txt): "; cin>>fname;

    ofstream fout(fname);
    if(!fout){ gotoxy(5,35); cout<<"Error opening file!"; return;}
    fout<<"University of the Punjab\n";
    fout<<"Bachelor's Studies Information Technology\n\n";
    fout<<"Student Name: "<<st.name<<"\nFather Name: "<<st.father<<"\nInstitute: "<<st.institute<<"\nDegree: "<<st.degree<<"\nSession: "<<st.session<<"\n\n";

    fout<<"FIRST SEMESTER:\n";
    for(auto &s: sem1) fout<<s.code<<" "<<s.title<<" "<<s.marks<<" "<<s.creditHours<<" "<<s.grade<<" "<<fixed<<setprecision(1)<<s.gpa<<"\n";

    fout<<"SECOND SEMESTER:\n";
    for(auto &s: sem2) fout<<s.code<<" "<<s.title<<" "<<s.marks<<" "<<s.creditHours<<" "<<s.grade<<" "<<fixed<<setprecision(1)<<s.gpa<<"\n";

    fout<<"\nTotal Marks: "<<totalMarks<<"\nAverage GPA: ";
    if(totalCreditHours>0) fout<<fixed<<setprecision(2)<<totalWeightedGPA/totalCreditHours; else fout<<"0.00";
    fout.close();
    gotoxy(5,35); cout<<"File saved successfully as "<<fname;
}

// ===== Menu =====
void menu(){
    int choice;
    while(true){
        gotoxy(5,32); cout<<string(100,' ');
        gotoxy(5,33); cout<<string(100,' ');
        gotoxy(5,34); cout<<string(100,' ');

        gotoxy(5,32);
        cout<<"Menu Options: 1.Add Sem1  2.Add Sem2  3.Save & Exit  Enter choice: ";
        if(cin >> choice){
            if(choice==1 || choice==2){
                int maxSubs = 6;
                vector<Subject>& sem = (choice==1)? sem1 : sem2;
                int remaining = maxSubs - sem.size();
                if(remaining==0){
                    gotoxy(5,33); cout<<"Semester full! Max 6 subjects.";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                    continue;
                }
                int n;
                while(true){
                    gotoxy(5,33); cout<<string(100,' ');
                    gotoxy(5,33); cout<<"How many subjects to add (1-"<<remaining<<")? ";
                    if(cin >> n){
                        if(n>=1 && n<=remaining) break;
                        else{
                            gotoxy(5,34); cout<<string(100,' ');
                            gotoxy(5,34); cout<<"Invalid! Enter 1 to "<<remaining<<". Press Enter.";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                        }
                    } else {
                        gotoxy(5,34); cout<<string(100,' ');
                        gotoxy(5,34); cout<<"Invalid input! Numbers only. Press Enter.";
                        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                    }
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                for(int i=0;i<n;i++){
                    if(choice==1) addSubject(sem1,4);
                    else addSubject(sem2,61);
                }
            }
            else if(choice==3){
                saveToFile();
                break;
            }
            else{
                gotoxy(5,33); cout<<"Invalid choice! Press Enter to retry.";
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
            }
        } else{
            gotoxy(5,33); cout<<"Invalid input! Numbers only. Press Enter.";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
        }
    }
}

// ===== Main =====
int main(){
    splashScreen();     // <-- Flashy startup
    drawLayout();
    inputStudentInfo();
    menu();
    gotoxy(0,46);
    return 0;
}

