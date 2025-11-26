#pragma once
#include <string.h>
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

void stringdg(const string& s,string &end){
    if(s.empty()){
        return;
    }
    for(unsigned char x : s){
        if(isspace(x)){
            continue;
        }
        if(x<128 &&!isalpha(x)){
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
