#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <limits>
#include <bits/stdc++.h>
using namespace std;
#define Enter 13
//const int WHITE=15;
#define PASSWORD "abcdef"
//const int WHITE=15;
#define Backspace 8
// add
 
char* Pwd () { 
     static char S[40]; 
     int i=0;
     while ((S[i]= getch()) != Enter && i<39) 
     { 
        printf ("%c", '*') ; 
        i++;
     }
     S[i]='\0';
     return S;
}
int CheckPwd () {
    int dem =0; 
    for ( dem =1 ; dem <=3 ; dem++)
    { printf( "Password :"); 
      if (strcmp(Pwd(),PASSWORD) ==0)   return 1;
      else printf ( "\nPassword sai. Hay nhap lai\n")  ; 
    }
    return 0;  
}

void gotoxy(short x,short y)
{
        HANDLE hConsoleOutput;
        COORD Cursor_an_Pos = { x,y};
        hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hConsoleOutput , Cursor_an_Pos);
}  

int wherex( void )
{
    HANDLE hConsoleOutput;  
    
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
    return screen_buffer_info.dwCursorPosition.X;
}

int wherey( void )
{
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
    return screen_buffer_info.dwCursorPosition.Y;
}
void clreol( ) {
COORD coord;
DWORD written;
CONSOLE_SCREEN_BUFFER_INFO info;
GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
coord.X = info.dwCursorPosition.X;
coord.Y = info.dwCursorPosition.Y;
FillConsoleOutputCharacter (GetStdHandle(STD_OUTPUT_HANDLE), ' ',
  info.dwSize.X - info.dwCursorPosition.X * info.dwCursorPosition.Y, coord, &written);
gotoxy (info.dwCursorPosition.X , info.dwCursorPosition.Y );
}

void SetColor(WORD color)
{
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    wAttributes &= 0xfff0;
    wAttributes |= color;

    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
void SetBGColor(WORD color)
{
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    color <<= 4;
    wAttributes &= 0xff0f;
    wAttributes |= color;

    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}



void clrscr() {
	system("cls");
}

//Check nhap so
long long NhapSo(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        cin.ignore();
        getline(cin, input);
        if (input.empty()) {
            cout << "Loi: Vui long nhap so!" << endl;
            continue;
        }
        bool valid = true;
        for (char ch : input) {
            if (!isdigit(ch)) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            cout << "Loi: Vui long chi nhap so!" << endl;
            continue;
        }
        try {
            long long result = stoll(input);
            if (result < 0) {
                cout << "Loi: So phai lon hon hoac bang 0!" << endl;
                continue;
            }
            return result;
        }
        catch (...) {
            cout << "Loi: So qua lon!" << endl;
        }
    }
}

void NhapChuoi(const string& prompt, char* result, int max_length) {
    string input;
    while (true) {
        cout << prompt;
        cin.ignore();
        getline(cin, input);
        if (input.empty()) {
            cout << "Loi: Vui long nhap chuoi!" << endl;
            continue;
        }
        bool valid = true;
        for (char ch : input) {
            if (!isalpha(ch) && ch != ' ') {
                valid = false;
                break;
            }
        }
        if (!valid) {
            cout << "Loi: Vui long chi nhap chu cai va khoang trang!" << endl;
            continue;
        }
        strncpy(result, input.c_str(), max_length - 1);
        result[max_length - 1] = '\0';
        return;
    }
}

void NhapMa(const string& prompt, char* result, int max_length) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            cout << "Loi: Vui long nhap ma!" << endl;
            continue;
        }
        bool valid = true;
        for (char ch : input) {
            if (!isalnum(ch) && ch != '-' && ch != '_') {
                valid = false;
                break;
            }
        }
        if (!valid) {
            cout << "Loi: Vui long chi nhap chu cai, so, '-' hoac '_'!" << endl;
            continue;
        }
        for (char& ch : input) {
            ch = toupper(ch);
        }
        strncpy(result, input.c_str(), max_length - 1);
        result[max_length - 1] = '\0';
        return;
    }
}
void chuanhoa(string & s ){
    s[0] = toupper(s[0]);
   for(int i = 1 ; i < s.size();i++){
      s[i] = tolower(s[i]);
   }
}
void stringdg(const string& s,string &end){
    if(s.empty()){
        cout <<"vui long nhap ten"<<endl;
        return;
    }
    for(char x : s){
        if(isspace(x)){
            continue;
        }
        if(!isalpha(x)){
            cout<<" word only"<<endl;
            return;
        }
        }
    string tmp;
    stringstream ss(s);
    while(ss >> tmp){
        chuanhoa(tmp);
        if(!end.empty()){
            end +=' ';
        }
        end+=tmp;
}
};
void number(const string  s , int &x){
    if(s.empty()){
        cout << "pls insert number"<<endl;
        return ;
    }
    for(char t : s){
        if(isspace(t)){
            continue;
        }
        if(!isdigit(t)){
            cout <<"number only"<<endl;
            return;
        }
    }
   stringstream ss(s);
   string temp;
   string kq;
   while(ss >> temp){
    kq += temp;
   }
   x = stoi(kq);
}
string chuanhoa_kitu_sach(string s){
    stringstream ss(s);
    string temp;// luu tam 1 tu
    string result = "";
    bool isFirstword = true;
    while (ss >> temp){
        s[0] = toupper(temp[0]);
        for (int i=1; i<temp.length(); i++){
            s[i] = tolower(temp[i]);
        }
    }
    if (isFirstword){
        result = temp;
        isFirstword = false;
    }
    else{
        result += " " + temp;
    }
    return result;
}
void chuanhoamasach(string & a,string &b){
    string temp;
    for(char &x : a){
        if(isalpha(x)){
            x = toupper(x);
        }
    }
    stringstream ss(a);
    while(ss>>temp){
        b += temp;
    }
};
