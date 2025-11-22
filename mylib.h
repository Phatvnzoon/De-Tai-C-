#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <limits>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <limits>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
using namespace std;
#define Enter 13
//const int WHITE=15;
#define PASSWORD "abcdef"
//const int WHITE=15;
#define Backspace 8
// add
void clrscr() {
	system("cls");
}

void chuanhoa(string & s ){
    s[0] = toupper(s[0]);
   for(int i = 1 ; i < s.size();i++){
      s[i] = tolower(s[i]);
   }
}
void chuanhoachuoi(string &s) {
    if (s.empty()) return;
    
    // Bỏ bớt space trong chuỗi nếu dư
    string temp;
    stringstream ss(s);
    string word;
    bool first = true;
    
    while (ss >> word) {
        if (!first) temp += " ";
        chuanhoa(word); 
        temp += word;
        first = false;
    }

    s = temp;
}
//Check nhap so
long long NhapSo(const string& prompt) {
    string input;
    while (1) {
        cout << prompt;
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

void NhapChuoi(const string &prompt, string &end) 
{
    string input; 
    end.clear();   

    while (true) {
        cout << prompt;
        
        
        if (!getline(cin, input)) return; 

        //Khong duoc rong
        if (input.empty()) {
            cout << "vui long nhap chuoi" << endl;
            continue; 
        }

        // Chi duoc chua chu cai va khoang trang
        bool valid = true; 
        for (char x : input) {
            
            if (isspace(x)) {
                continue; // Neu la khoang trang, bo qua, kiem tra ky tu tiep
            }
            if (!isalpha(x)) {
                // Neu ky tu khong phai khoang trang VA cung khong phai chu cai
                cout << "Vui long chi nhap tu" << endl;
                valid = false; 
                break;         
            }
        }

        if (!valid) {
            continue;
        }
        end = input;
        chuanhoachuoi(end);
        
        return; 
    }
}

void NhapISBN(const string& prompt, string &result) {
    cout << prompt;
    result.clear(); 

    while (result.length() < 13) {
        char ch = _getch();

        // Kiem tra DAU CACH
        if (ch == ' ') {
            cout << "\nLoi: Khong duoc phep nhap dau cach! Vui long nhap lai tu dau." << endl;
            Sleep(1000);
            cout << prompt;
            result.clear();
            continue; 
        }
        else if (isalpha(ch)){
            cout << "\nLoi: Khong duoc phep nhap chu! Vui long nhap lai tu dau." << endl;
            Sleep(1000);
            cout << prompt;
            result.clear();
            continue;
        }
        else if (ch == Backspace) { 
            if (!result.empty()) {
                result.erase(result.length() - 1, 1);
                cout << "\b \b";   // Xoa ky tu tren man hinh
            }
        }    
        // Xu ly CHU SO
        else if (isdigit(ch)) {
            result += ch; // Them so vao chuoi
            cout << ch;     // Hien thi so do ra man hinh
        }

    }
    cout << endl;
    return;
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
            cout<<"vui long chi nhap tu"<<endl;
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