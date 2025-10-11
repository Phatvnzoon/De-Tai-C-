#include <bits/stdc++.h>
#include "mylib.h" 
using namespace std;

const int MAX_DAUSACH = 10000;

//DS_dau sach chua danh sach dau sach
// dau sach chua moi sach co ma cu the
struct Sach {
    char MASACH[16];
    int trangthai;
    char vitri[51];
};

struct nodeSach {
    Sach data;
    nodeSach *next;
};
typedef nodeSach* SACH;

struct DauSach {
    char ISBN[15];
    char TENSACH[101];
    int SOTRANG;
    char TACGIA[51];
    int NAMXUATBAN;
    char THELOAI[31];
    SACH FirstSach = NULL;
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
typedef nodeMuonTra* MT;

struct TheDocGia {
    int MATHE;
    int sum = 0; // Minh
    char HO[51];
    char TEN[31];
    char PHAI[4];
    int trangthai = 1; // 0: bi khoa , 1: hoat dong, 2: bị mất
    MT dsmuontra = NULL;
};

struct nodeDocGia {
    TheDocGia dg;
    nodeDocGia *left, *right;
};
typedef nodeDocGia* TreeDocGia;

//Câu a, maybe b
TreeDocGia taothedocgia (){
     TheDocGia a ;
    srand(time(0));
    a.MATHE = 1000 + rand() % (10000 - 1000);
    cout << "Ho: "; cin >> a.HO;
    
    cout << "Ten: ";cin >> a.TEN;
    int x;
    cout << "Phai(1.NAM , 2 NU):  "; cin >> x ;
    if(x == 1){
        strcpy(a.PHAI, "NAM");
    }
    if(x==2){
        strcpy(a.PHAI, "NU");
    }
    for(int i = 0 ; i < a.HO[i]!='\0';i++){
        a.sum += a.HO[i];
    }
    for(int i = 0 ; i < a.TEN[i]!='\0';i++){
        a.sum += a.TEN[i];
    }
     TreeDocGia tmp = new nodeDocGia();
     tmp->dg = a;
     tmp->left =NULL;
     tmp->right = NULL;
     return tmp;
};
void in(TheDocGia  a){
    cout << a.MATHE <<endl;
    cout << a.HO <<" "<<a.TEN<<endl;
    cout <<a.PHAI <<endl;
};
void caythedocgia(TreeDocGia & a,TreeDocGia p){
    if (a ==NULL){
           a = p ;
           return;
    }
    if(a->dg.MATHE > p->dg.MATHE){
        caythedocgia(a->left,p);
    }
     if(a->dg.MATHE <= p->dg.MATHE){
        caythedocgia(a->right,p);
     }
};
void caythehoten(TreeDocGia &a,TreeDocGia p){
    if (a==NULL){
           a = p ;
           return;
    }
    if(a->dg.sum > p->dg.sum){
        caythehoten(a->left,p);
    }
    if(a->dg.sum <= p->dg.sum) {
        caythehoten(a->right,p);
    }
};

void incay(TreeDocGia a){
    if (a==nullptr){
        return;
    }
    incay(a->left);
    cout << a->dg.MATHE <<endl;
    cout << a->dg.HO <<" "<<a->dg.TEN<<endl;
    cout <<a->dg.PHAI <<endl;
    if(a->dg.trangthai == 1){
        cout <<"thẻ đang hoạt động "<<endl;
    }
    else {
        cout <<"thẻ ko còn hoạt động "<<endl;
    }
    incay(a->right);
};

void khoathe(TreeDocGia & a,int x){
    if(a==NULL){
        return;
    }
    if ( a->dg.MATHE == x){
        a->dg.trangthai = 0;
    }
    khoathe(a->left,x);
    khoathe(a->right,x);
};

void xoanode2la(TreeDocGia& a,TreeDocGia &p){
    if(a->left!=NULL){
        xoanode2la(a->left,p);
    }
    else{
        p->dg = a->dg;
        p = a;
        a = a->left;
    }
};

void xoathe(TreeDocGia& a, int x ){
    if(a==NULL){
        cout << "Khong tim thay ma the!" << endl;
        Sleep(1000);
        return;
    }
    if(a->dg.MATHE > x){
        xoathe(a->left,x);
    }
    else if (a->dg.MATHE < x){
        xoathe(a->right,x);
    }
    else{
        TreeDocGia p = a;
        if(p->left == NULL){
            p = p->right;
            delete p;

        }
        else if(p->right == NULL){
            p = p->right;
            delete p;
        }
        else {
            xoanode2la(a->right,p);
            delete p;
        }
        return;
    }
};

//Câu C
string randomMaSach(){ // tạo mã sách có 6 chữ số
    string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string result;
    while(1){
        result = "";
        bool fullLetter = false, fullDigit = false;
        for (int i = 0; i < 6; i++) {
            char c = chars[rand() % chars.size()];
            result += c; 
            if (isalpha(c)) fullLetter = true;
            if (isdigit(c)) fullDigit = true;
        }
        if (fullLetter && fullDigit) break; //loai bo truong hop full chu hoac full so
    }   
    
    return result;
}

bool MaSachTrung(DauSach *ds, string &Ma){
    SACH p = ds->FirstSach;
    while (p != NULL) {
        if (strcmp(p->data.MASACH, Ma.c_str()) == 0)
            return true;
        p = p->next;
    }
    return false;
}

void NhapDauSach(DS_DAUSACH &ds_dausach){
    if (ds_dausach.n >= MAX_DAUSACH){
        cout << "Danh sach da day, khong the nhap!";
        return;
    }

    DauSach *p = new DauSach();
    NhapMa("Nhap ISBN(13 so): ", p->ISBN, 15);
    NhapChuoi("Nhap ten sach: ", p->TENSACH,101);
    p->SOTRANG = NhapSo("Nhap so trang: ");
    NhapChuoi("Nhap tac gia: ", p->TACGIA, 51);
    p->NAMXUATBAN = NhapSo("Nhap Nam xuat ban: ");
    NhapChuoi("Nhap the loai: ", p->THELOAI, 31);
    int slsach = NhapSo("Nhap so luong sach: ");

    for(int i = 0; i < slsach; i++){
        SACH newSach = new nodeSach();
        
        // Tạo mã sách tự động và kiểm tra trùng
        string maSach;
        do {
            maSach = randomMaSach();
        } while(MaSachTrung(p, maSach));
        
        strcpy(newSach->data.MASACH, maSach.c_str());
        newSach->data.trangthai = 0; // 0: trong kho
        NhapChuoi("Nhap vi tri: ", newSach->data.vitri, 51);
        
        // Thêm vào đầu danh sách liên kết
        newSach->next = p->FirstSach;
        p->FirstSach = newSach;
        
        cout << "Da tao sach voi ma: " << maSach << endl;
    }

    int viTriChen = ds_dausach.n;
    for(int i = 0; i < ds_dausach.n; i++){
        if(strcmp(p->TENSACH, ds_dausach.nodes[i]->TENSACH) < 0){
            viTriChen = i;
            break;
        }
    }
    
    // Dịch chuyển các phần tử để chèn vào đúng vị trí
    for(int i = ds_dausach.n; i > viTriChen; i--){
        ds_dausach.nodes[i] = ds_dausach.nodes[i-1];
    }

    ds_dausach.nodes[ds_dausach.n] = p; // thêm phần tử vào danh sách
    ds_dausach.n++;

    cout << "\nDa them thanh cong!" << endl;
}

int main() {
    DS_DAUSACH dsdausach;
    TreeDocGia dsdocgia = NULL;
    TreeDocGia dshoten = NULL;
    int lc1 = 0;
    int lc2 = 0;
    while(1){
        system("cls");
        cout << "====== MENU CHINH ======" << endl;
        cout <<"1.Nhap the doc gia: " << endl;
        cout <<"2.In danh sach doc gia: " << endl;
        cout <<"3.Khoa the doc gia" << endl;
        cout <<"4.Nhap thong tin dau sach: " << endl;
        cout <<"0.Thoat" << endl;
        
        cout <<"Lua chon cua ban(0..4): ";
        cin >> lc1;

        if(lc1 == 0){
            system("cls");
            cout << "Cam on ba da su dung chuong trinh!" << endl;
            Sleep(1000);
            break;
        }
        switch (lc1){
            case 1:{
                system("cls");
                cout <<"1.Them doc gia: " << endl;
                cout <<"2.Xoa doc gia: " << endl;
                cout <<"3.Hieu chinh doc gia: " << endl;
                cout <<"Lua chon cua ban(1..3): ";
                cin >> lc2;
                switch (lc2){
                    case 1:{
                        TreeDocGia tmp = taothedocgia();
                        TreeDocGia temp = new nodeDocGia();
                        temp->dg =tmp->dg;
                        temp->left = temp->right = NULL;
                        caythedocgia(dsdocgia,tmp);
                        caythehoten(dshoten,temp);
                    } 
                    case 2:{
                        int mathe = NhapSo("Nhap ma the muon xoa: ");
                        xoathe(dsdocgia,mathe);
                        cout << "Da xoa doc gia co ma the " << mathe << " thanh cong!" << endl;
                        }
                        Sleep(1000);
                        break;
                    }
                    //case 3:
            }
            case 2:{
                system("cls");
                cout <<"1.Theo ho+ten: " << endl;
                cout <<"2.Theo ma doc gia: " << endl;
                cout <<"Nhap lua chon cua ban(1..2): ";
                cin >> lc2;
                switch(lc2){
                    case 1: incay(dshoten); break;
                    case 2: incay(dsdocgia); break;
                }
            } 
            case 3:{
                system("cls");
                int x;
                cout << "Nhap ma the muon khoa: ";
                cin >> x;
                khoathe(dsdocgia,x);
                khoathe(dshoten,x);
            }
            case 4:{
                
            }
        }
            
    }
    return 0;
}


    