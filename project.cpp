#include <bits/stdc++.h>
using namespace std;
#include "mylib.h"

const int MAX_DAUSACH = 10000;

struct Sach {
    char MASACH[16];
    int trangthai; // =0 dc muon , =1 đã mượn, =2 đã thanh lý
    char vitri[51];
};

struct nodeSach {
    Sach data;
    nodeSach *next;
};
typedef nodeSach* SACH;

struct DauSach {
    char ISBN[21];
    char TENSACH[101];
    int SOTRANG;
    char TACGIA[51];
    int NAMXUATBAN;
    char THELOAI[31];
    SACH FirstSach = NULL;
    int slm = 0;
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
    int trangthai2; // =0 đang mượn , =1 đã trả , =2 là mất sách
};

struct nodeMuonTra {
    MuonTra mt;
    nodeMuonTra *next;
};
typedef nodeMuonTra* MT;
MT makeMT(){
    MT a;
    a->next = NULL;
    return a;
};
struct TheDocGia {
    int MATHE=0;
    int sum=0;
    char HO[51];
    char TEN[31];
    char PHAI[4];
    int trangthai = 1; // 0: bi khoa , 1: hoat dong;
    MT dsmuontra = NULL;
};

struct nodeDocGia {
    TheDocGia dg;
    nodeDocGia *left, *right;
};
typedef nodeDocGia* TreeDocGia;
TreeDocGia taothedocgia (){ // tạo cây
     TheDocGia a ;
    srand(time(0));
    a.MATHE = 1000 + rand() % (10000 - 1000);
    cout << "Ho : "; cin >> a.HO;
    
    cout << "Ten: ";cin >> a.TEN;
    int x;
    cout << "Phai : 1.NAM , 2 NU "; cin >> x ;
    if(x == 1){
        strcpy(a.PHAI, "Nam");
    }
    if(x==2){
        strcpy(a.PHAI, "Nu");
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
void caythedocgia(TreeDocGia & a,TreeDocGia p){ // thêm cây theo ma the (độc giả)
    if (a ==NULL){
           a = p ;
           return;
    }
    if(a->dg.MATHE > p->dg.MATHE){
        caythedocgia(a->left,p);
    }
     else if(a->dg.MATHE <= p->dg.MATHE){
        caythedocgia(a->right,p);
     }
};
void caythehoten(TreeDocGia & a,TreeDocGia p){ // thêm cây theo tên (độc giả)
    if (a==NULL){
           a = p ;
           return;
    }
    if(a->dg.sum > p->dg.sum){
        caythedocgia(a->left,p);
    }
    else if(a->dg.sum <= p->dg.sum) {
        caythedocgia(a->right,p);
    }
};
void incay(TreeDocGia a){ // in cây
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
void khoathe(TreeDocGia & a,int x){ // khóa thẻ
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
void xoathe(TreeDocGia& a, int x ){   // xóa thẻ trong cây
        if(a==NULL){
        return;
    }
        xoathe(a->left,x); 
        xoathe(a->right,x);
       if(a->dg.MATHE== x){ 
        if(a->left == NULL){
            TreeDocGia p = a;
            a = a->right;
            delete p;

        }
        else if(a->right == NULL){
            TreeDocGia p = a;
            a = a->left;
            delete p;
        }
        else if(a->right == NULL&& a->left == NULL){
            delete a;
            a = nullptr;
           }
        else {
            TreeDocGia p = a;
            xoanode2la(a->right,p);
            delete p;
        }
        return;
    }
};
 void dieuchinhmathe(TreeDocGia& a , TheDocGia &tmp){ // điều chỉnh thẻ
    if(a==NULL){
        return ;
    }
    dieuchinhmathe(a->left,tmp);
    dieuchinhmathe(a->right,tmp);
    if(a->dg.MATHE == tmp.MATHE){
       a->dg = tmp;
       cout << "chỉnh thành công "<<endl;
       return ;
    }
};
void indsmuontra(MT & a){   // in ds mượn trả
    MT p = a;
    while(p == NULL){
        cout << p->mt.MASACH <<" "<< p->mt.NgayMuon<<" "<<p->mt.NgayTra<<" "<<p->mt.trangthai2<<endl;
        p = p->next;
    }
};
void checkdsmuonsach(TreeDocGia & a, int x ){  // in danh sách mượn trả
    if(a == NULL){
        return;
    }
    else if(a->dg.MATHE > x){
        checkdsmuonsach(a->left,x);
    }
    else if(a->dg.MATHE < x){
        checkdsmuonsach(a->right,x);
    }
    else{
        indsmuontra(a->dg.dsmuontra);
    }   
};
void muonsach(DS_DAUSACH & a, TreeDocGia & b,const char* s,int x){
    if(b == NULL){
        return; 
    }
    else if(b->dg.MATHE > x){
        muonsach(a,b->left,s,x);
    }
    else if(b->dg.MATHE < x){
        muonsach(a,b->right,s,x);
    }
    else {
        for(int i = 0 ; i < a.n;++i){
            if(strcmp(a.nodes[i]->TENSACH,s)==0){ // dò tên sách trùng
                SACH temp = a.nodes[i]->FirstSach;
                while(temp == NULL){      
                    if(temp->data.trangthai == 0){ // dò sách đó có ai mượn chưa
                        MT tmp = makeMT();
                        MT p = b->dg.dsmuontra;
                        if(p == NULL){
                         strcpy(tmp->mt.MASACH,temp->data.MASACH);
                         tmp->mt.trangthai2 = 0;
                         return;
                        }
                        else{
                            while(p->next!=NULL){
                                b->dg.dsmuontra = b->dg.dsmuontra->next;
                            }
                            strcpy(tmp->mt.MASACH,a.nodes[i]->FirstSach->data.MASACH); // đã mượn
                         tmp->mt.trangthai2 = 0;
                            p->next = tmp;
                        }
                        temp->data.trangthai = 1;
                        return;
                    }
                    temp = temp->next;
                }
            }
        }
    }
};
//Đánh mã sách tự động
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
        if (fullLetter && fullDigit) break; //Bỏ trường hợp full chữ hoặc full số
    }   
    
    return result;
};
bool MaSachTrung(DauSach *ds, string &Ma){
    SACH p = ds->FirstSach;
    while (p != NULL) {
        if (strcmp(p->data.MASACH, Ma.c_str()) == 0)
            return true;
        p = p->next;
    }
    return false;
};
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
};
int main() {
    DS_DAUSACH dsdausach;
    TreeDocGia dsdocgia = NULL;
    TreeDocGia dshoten = NULL;
    while(1){
        cout << "1. nhập thẻ độc giả"<<endl;
        cout << "2.in DS thẻ độc giả theo maso "<<endl;
        cout << "3.in DS thẻ độc giả theo ho ten "<<endl;
        cout << "4.Khoa thẻ độc giả "<<endl;
        cout << "5.xóa thẻ độc giả "<<endl;
        cout << "6.điều chỉnh thẻ độc giả "<<endl;
        int lc; cin >>lc;
        if(lc == 1){
            TreeDocGia tmp = taothedocgia();
            TreeDocGia temp = new nodeDocGia();
            temp->dg =tmp->dg;
            temp->left = temp->right = NULL;
            caythedocgia(dsdocgia,tmp);
            caythehoten(dshoten,temp);
        }
        else if(lc == 2){
           incay(dsdocgia); 
        }
        else if(lc == 3){
           incay(dshoten); 
        }
        else if(lc == 4){
            cout <<"ma the muon khoa "<<endl;
            int x ; cin >>x;
          khoathe(dsdocgia,x);
          khoathe(dshoten,x);
        }
        else if(lc == 5){
            cout <<"ma the muon khoa "<<endl;
            int x ; cin >>x;
           xoathe(dsdocgia,x);
           xoathe(dshoten,x);
        }
        else if(lc == 6){
            TheDocGia tmp;
            cout <<"ma the muon dieu chinh "<<endl;
            cin >> tmp.MATHE;
            cout << "New Ho : "; cin >> tmp.HO;
    
    cout << "New Ten: ";cin >> tmp.TEN;
    int g;
    cout << "New Phai : 1.NAM , 2 NU "; cin >> g ;
    if(g == 1){
        strcpy(tmp.PHAI, "Nam");
    }
    if(g==2){
        strcpy(tmp.PHAI, "Nu");
    }       
           dieuchinhmathe(dsdocgia,tmp);
           dieuchinhmathe(dshoten,tmp);
        }
    }
    // dsdocgia = taothedocgia();
    // in(dsdocgia);aw dawd a wd awdaw
}