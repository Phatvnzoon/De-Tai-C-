#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

const int MAX_DAUSACH = 10000;

struct Sach {
    char MASACH[16];
    int trangthai;
    char vitri[51];
};

struct nodeSach {
    Sach data;
    nodeSach *next;
};
typedef nodeSach* PTRSACH;

struct DauSach {
    char ISBN[21];
    char TENSACH[101];
    int SOTRANG;
    char TACGIA[51];
    int NAMXUATBAN;
    char THELOAI[31];
    PTRSACH FirstSach = NULL;
};

struct DS_DAUSACH {
    int n = 0;
    DauSach *nodes[MAX_DAUSACH];
};  

struct Date{
    int day;
    int month;
    int year;
};

struct MuonTra {
    char MASACH[16];
    char NgayMuon[11];
    char NgayTra[11];
    int trangthai;
};

struct nodeMuonTra {
    MuonTra mt;
    nodeMuonTra *next;
};
typedef nodeMuonTra* PTRMT;

struct TheDocGia {
    int MATHE;
    char HO[51];
    char TEN[31];
    char PHAI[4];
    int trangthai = 1; // 0: bi khoa , 1: hoat dong, 2: bị mất
    PTRMT dsmuontra = NULL;
};

struct nodeDocGia {
    TheDocGia dg;
    nodeDocGia *left, *right;
};
typedef nodeDocGia* TreeDocGia;

int main() {
    DS_DAUSACH dsdausach;
    TreeDocGia dsdocgia = NULL;
    
    return 0;
}