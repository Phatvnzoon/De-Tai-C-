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
