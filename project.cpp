#include <bits/stdc++.h>
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
typedef nodeSach* SACH;

struct DauSach {
    char ISBN[21];
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
TreeDocGia taothedocgia (){
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
void caythedocgia(TreeDocGia & a,TreeDocGia p){
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
void caythehoten(TreeDocGia & a,TreeDocGia p){
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
 void dieuchinhmathe(TreeDocGia& a , TheDocGia &tmp){
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
    // in(dsdocgia); ???
}