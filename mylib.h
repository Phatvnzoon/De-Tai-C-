#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <windows.h>
#include <math.h>

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

// Nhập số
long long NhapSo(char* prompt) {
    char ch;
    char str[20];
    int i = 0;
    
    printf("%s", prompt);
    
    while (1) {
        ch = getch(); // nhập ký tự từ bàn phím trả về mã ASCII
        
        if (ch == Enter) {
            if (i == 0) {
                // Chưa nhập số nào
                printf("\nVui long nhap so!");
                printf("\n%s", prompt);
                continue;
            }
            str[i] = '\0';
            printf("\n");
            return atoll(str); // chuyển ký tự thành ký số long long
        }
        else if (ch == Backspace) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        }
        else if (isdigit(ch)) {
            if (i < 19) {
                str[i] = ch;
                printf("%c", ch);
                i++;
            }
        }
        else if (atoll(str) < 0){{
            continue;
        }}
        else{
            // hiển thị ký tự sai và thông báo
            printf("%c <- Vui long chi nhap so!", ch);
            Sleep(1000);
            int lenNoti = 25; // độ dài " <- Vui long chi nhap so!"
            for (int j = 0; j < lenNoti; j++) {
                printf("\b \b"); // xoá noti
            }
            printf("\b \b"); // xoá ký sai
        }
    }
}

// Nhập chuỗi
void NhapChuoi(char* prompt, char* result, int max_length) {
    char ch;
    int i = 0;
    
    printf("%s", prompt);
    
    while (1) {
        ch = getch();
        
        if (ch == Enter) {
            if (i == 0) {
                printf("\nVui long nhap chuoi!");
                printf("\n%s", prompt);
                continue;
            }
            result[i] = '\0';
            printf("\n");
            return;
        }
        else if (ch == Backspace) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        }
        else if (isalpha(ch) || ch == ' ') {
            if (i < max_length - 1) {
                result[i] = ch;
                printf("%c", ch);
                i++;
            }
        }
        else{
            // hiển thị ký tự sai và thông báo
            printf("%c <- Vui long chi nhap chu cai va khoang trang!", ch);
            Sleep(1000);
            int lenNoti = 46; // độ dài " <- Vui long chi nhap chu cai va khoang trang!"
            for (int j = 0; j < lenNoti; j++) {
                printf("\b \b"); // xoá noti
            }
            printf("\b \b"); // xoá ký sai
        }
    }
}

// Nhập mã
void NhapMa(char* prompt, char* result, int max_length) {
    char ch;
    int i = 0;
    
    printf("%s", prompt);
    
    while (1) {
        ch = getch();
        
        if (ch == Enter) {
            if (i == 0) {
                printf("\nVui long nhap ma!");
                printf("\n%s", prompt);
                continue;
            }
            result[i] = '\0';
            printf("\n");
            return;
        }
        else if (ch == Backspace) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        }
        else if (isalpha(ch)) {
            if (i < max_length - 1) {
                // Tự động chuyển thành chữ hoa
                result[i] = toupper(ch);
                printf("%c", result[i]);
                i++;
            }
        }
        else if (isdigit(ch) || ch == '-' || ch == '_') {
            if (i < max_length - 1) {
                result[i] = ch;
                printf("%c", ch);
                i++;
            }
        }
        else{
            // hiển thị ký tự sai và thông báo
            printf("%c <- Vui long nhap theo yeu cau!", ch);
            Sleep(1000);
            int lenNoti = 31; // độ dài " <- Vui long nhap theo yeu cau!"
            for (int j = 0; j < lenNoti; j++) {
                printf("\b \b"); // xoá noti
            }
            printf("\b \b"); // xoá ký sai
        }
    }
}