#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <limits>
#include <cmath>

using namespace std;
const int MAX_DAUSACH = 10000;

struct Sach {
    string MASACH; // Ma sach theo thu vien
    int trangthai; // =0 chua muon , =1 đã mượn, =2 đã thanh lý
    string vitri;
};

struct nodeSach {
    Sach data;
    nodeSach *next;
};
typedef nodeSach* SACH;

struct DauSach {
    string ISBN;
    string TENSACH;
    int SOTRANG;
    string TACGIA;
    int NAMXUATBAN;
    string THELOAI;
    SACH dms = NULL;
    int slsach = 0;
    int slm = 0;
};

struct DS_DAUSACH { // Danh mục sách
    int n = 0;
    DauSach *nodes[MAX_DAUSACH];
};
typedef DS_DAUSACH* DS_DauSach;
struct Date{
    int day;
    int month;
    int year;
};
Date time(){
    Date a;
    time_t now = time(0);
    tm* time = localtime(&now);
    a.day = time->tm_mday;
    a.month =time->tm_mon +1;
    a.year = time->tm_year +1900;
    return a;
};
struct MuonTra {
    string TENSACH;
    string MASACH;
    string NgayMuon;
    string NgayTra;
    int trangthai2; // =0 đang mượn , =1 đã trả , =2 là mất sách
};
struct nodeMuonTra {
    MuonTra mt;
    nodeMuonTra *next;
};
typedef nodeMuonTra* MT;
MT makeMT(){
    MT a = new nodeMuonTra();
    a->next = NULL;
    return a;
};
struct TheDocGia {
    int MATHE=0;
    int sum=0;
    string HO;
    string TEN;
    char PHAI[4];
    int trangthai = 1; // 0: bi khoa , 1: hoat dong;
    int sachmuon = 0 ;// max = 3;
    int quahan =0;
    MT dsmuontra = NULL;
};
struct DS_TheDocgia
{  int cnt =0;
    TheDocGia list[MAX_DAUSACH];
};
struct nodeDocGia {
    TheDocGia dg;
    nodeDocGia *left, *right;
};
typedef nodeDocGia* TreeDocGia;
TreeDocGia taothedocgia (){ // tạo cây
    TheDocGia a ;
    string s;
    string stwo;
    srand(time(0));
    a.MATHE = 1000 + rand() % (10000 - 1000);
    cout << "HO: "; cin.ignore();
    getline(cin,s);
    stringdg(s,a.HO);
    while(a.HO.empty()){
    cout << "HO: ";
    getline(cin,s);
    stringdg(s,a.HO);
    }
    cout << "Ten: "; 
    getline(cin,stwo);
    stringdg(stwo,a.TEN);
    while(a.TEN.empty()){
    cout << "TEN: ";
    getline(cin,stwo);
    stringdg(stwo,a.TEN);
    }
    int x;
    cout << "Phai(1.NAM , 2 NU): "; cin >> x ;
    if(x == 1){
        strcpy(a.PHAI, "Nam");
    }
    if(x==2){
        strcpy(a.PHAI, "Nu");
    }
    for(int i = 0 ; i < a.HO.size();i++){
        a.sum += a.HO[i];
    }
    for(int i = 0 ; i < a.TEN.size();i++){
        a.sum += a.TEN[i];
    }
     TreeDocGia tmp = new nodeDocGia();
     tmp->dg = a;
     tmp->left =NULL;
     tmp->right = NULL;
     return tmp;
};
void caythedocgia(TreeDocGia & a,TreeDocGia p){ // thêm cây theo mã thẻ (độc giả)
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
        cout <<"The dang hoat dong "<<endl;
    }
    else {
        cout <<"The khong con hoat dong "<<endl;
    }
    incay(a->right);
};
bool CheckMaThe(TreeDocGia t, int ma) {
    if (t == NULL) return false;
    if (t->dg.MATHE == ma) return true;
    
    if (ma < t->dg.MATHE) 
        return CheckMaThe(t->left, ma);
    else 
        return CheckMaThe(t->right, ma);
}
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
        return ;
    }
        if (x < a->dg.MATHE) {
        return xoathe(a->left, x);
    }
    else if (x > a->dg.MATHE) {
        return xoathe(a->right, x);
    }
       else{ 
        if(a->left == NULL){
            TreeDocGia p = a;
            a = a->right;
            delete p;
            return ;

        }
        else if(a->right == NULL){
            TreeDocGia p = a;
            a = a->left;
            delete p;
            return ;
        }
        else if(a->right == NULL&& a->left == NULL){
            delete a;
            a = nullptr;
            return ;
           }
        else {
            TreeDocGia p = a;
            xoanode2la(a->right,p);
            delete p;
            return ;
        }
    }
}
 void dieuchinhmathe(TreeDocGia& a , TheDocGia &tmp){ // điều chỉnh thẻ
    if(a==NULL){
        return ;
    }
    dieuchinhmathe(a->left,tmp);
    if(a->dg.MATHE == tmp.MATHE){
       a->dg = tmp;
       cout << "Dieu chinh thanh cong "<<endl;
       return ;
    }
    dieuchinhmathe(a->right,tmp);
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
// void indsmuontra(MT  a){   // in ds mượn trả đang mượn
//     MT p = a;
//     while(p != NULL){
//         if(p->mt.trangthai2==0){
//         cout << p->mt.MASACH <<" "<<p->mt.TENSACH<<" "<< p->mt.NgayMuon<<" "<<p->mt.NgayTra<<" "<<p->mt.trangthai2<<endl;
//         }
//         p = p->next;
//     }
// };
// void checkdsmuonsach(TreeDocGia  a, int x ){  // in danh sách mượn trả
//     if(a == NULL){
//         return;
//     }
//     else if(a->dg.MATHE > x){
//         checkdsmuonsach(a->left,x);
//     }
//     else if(a->dg.MATHE < x){
//         checkdsmuonsach(a->right,x);
//     }
//     else{
//         indsmuontra(a->dg.dsmuontra);
//     }   
// };
// bool changebook(DS_DauSach & a,const string s,const string t,int x,String & thongbao){
//     for(int i = 0 ; i < a->n;++i){
//         if(a->nodes[i]->TENSACH == t){
//             SACH tmp = a->nodes[i]->dms;
//             while(tmp != NULL){
//                 if(tmp->data.MASACH == s){
//                     tmp->data.trangthai =x;
//                     thongbao = "Trả Thành Công!";
//                     return true ;
//                 }
//                 tmp = tmp->next;
//             }
//             thongbao = "Mã Sách Không Tồn Tại!";
//             return false;
//         }
//         thongbao = "Tên Sách Không Tồn Tại!";
//         return false;
//     }
// };
int tinhngay(string &s){
    stringstream ss(s);
    string in;
    int ketqua=0;
    getline(ss,in,'/');ketqua += stoi(in);
    getline(ss,in,'/');ketqua += stoi(in)*30;
     getline(ss,in,'/');ketqua += stoi(in)*365;
     return ketqua;
     
}
void checkdaymt(TreeDocGia & b,Date  a){
    MT p = b->dg.dsmuontra;
    string s;
    int ketquat = a.day + a.month*30 +a.year *365;
    while (p != NULL){
        int ketqua;
        int ngaytra;
        if(p->mt.trangthai2 ==1){
           ketqua = tinhngay(p->mt.NgayMuon);
           ngaytra = tinhngay(p->mt.NgayTra);
       if(ngaytra-ketqua >=7){
        b->dg.trangthai =0 ;
        b->dg.quahan = ngaytra-ketqua;
       }
            p = p->next;
            continue;
        }
       ketqua = tinhngay(p->mt.NgayMuon);
       if(ketquat - ketqua >= 7){
          b->dg.trangthai = 0;
          b->dg.quahan = ketquat - ketqua;
       }
       p = p->next;
    }
};
// void muonsach(DS_DauSach & a, TreeDocGia & b,const string s,int x){
//     if(b == NULL){
//         return; 
//     }
//     else if(b->dg.MATHE > x){
//         muonsach(a,b->left,s,x);
//     }
//     else if(b->dg.MATHE < x){
//         muonsach(a,b->right,s,x);
//     }
//     else {
//         Date t = time();
//         if(b->dg.dsmuontra!=NULL){
//             checkdaymt(b,t);
//         };
//         if(b->dg.sachmuon >= 3){
//           cout << "sinh vien nay dang muon 3 cuon sach"<<endl;
//           return;
//         }
//         if(b->dg.trangthai == 0){
//              cout << "the bi khoa "<<endl;
//              return;
//         }
//         for(int i = 0 ; i < a->n;++i){
//             if(a->nodes[i]->TENSACH == s){ // dò tên sách trùng
//                 SACH temp = a->nodes[i]->dms;
//                 while(temp != NULL){      
//                     if(temp->data.trangthai == 0){ // dò sách đó có ai mượn chưa
//                         MT tmp = makeMT();
//                         MT p = b->dg.dsmuontra;
//                          p = tmp;
//                          tmp->mt.MASACH = temp->data.MASACH;
//                          tmp->mt.TENSACH = a->nodes[i]->TENSACH;
//                          tmp->mt.trangthai2 = 0;
//                          b->dg.sachmuon ++;
//                          temp->data.trangthai =1;
//                          a->nodes[i]->slm ++;
//                          tmp->mt.NgayMuon = to_string(t.day) + "/"+ to_string(t.month) + "/" +to_string(t.year);
//                         if(b->dg.dsmuontra == NULL){
//                             b->dg.dsmuontra = tmp;
//                          cout << " muon thanh cong"<<endl;
//                          return;
//                         }
//                         else{
//                             while(p->next!=NULL){
//                                 p = p->next;
//                             }
//                              p->next = tmp;
//                              cout << " muon thanh cong"<<endl;
//                             return;
//                         }
//                     }
//                     temp = temp->next;
//                 }
//             }
//         }
//     }
// };
// void trasach(TreeDocGia& a,DS_DauSach & b, int x,string s, string t){
//      if(a == NULL){
//         return; 
//     }
//     else if(a->dg.MATHE > x){
//        trasach(a->left,b,x,s,t);
//     }
//     else if(a->dg.MATHE < x){
//         trasach(a->right,b,x,s,t);
//     }
//     else{
//         Date TIME = time();
//          changebook(b,s,t,0);
         
//          MT temp = a->dg.dsmuontra;
//          while(temp != NULL){
//             if (temp->mt.MASACH == s){
//                 temp->mt.trangthai2 = 1 ;
//                 temp->mt.NgayTra = to_string(TIME.day)+"/"+to_string(TIME.month)+"/"+to_string(TIME.year);
//                 return;
//             }
//             temp = temp->next;
//          }
//     }
// };
void top10book(DS_DauSach & a){  //10 sách dc mượn nhiều nhất (j)
    int cnt = 0;
    DauSach tmp[10];
    for (int i = 0; i < a->n ; ++i)
    {    if(cnt < 10){
         tmp[cnt].slm = a->nodes[i]->slm;
         tmp[cnt].TENSACH = a->nodes[i]->TENSACH;
         cnt++;
    }
        else {
            for(int j = 0 ; j <cnt;++j){
                 if(tmp[j].slm <a->nodes[i]->slm){
                    tmp[j].slm = a->nodes[i]->slm;
                    tmp[j].TENSACH =a->nodes[i]->TENSACH;
                 }
            }
            for(int i = 0 ; i < cnt ; ++i){
                for(int j = i +1 ; j < cnt ; ++j){
                    if(tmp[i].slm < tmp[j].slm){
                        DauSach temp;
                        temp.slm = tmp[i].slm;
                        temp.TENSACH=tmp[i].TENSACH;
                        tmp[i].slm = tmp[j].slm;
                        tmp[i].TENSACH=tmp[j].TENSACH;
                        tmp[j].slm = temp.slm;
                        tmp[j].TENSACH=temp.TENSACH;
                    }
                }
            }
        }
    }
    for(int i = 0 ; i < cnt; ++i){
        // thieu in
    }
};
void luudsquahan(TreeDocGia & a , DS_TheDocgia & b,Date c){
    if(a== NULL){
        return;
    }
    else{
        checkdaymt(a,c);
    }
    luudsquahan(a->left,b,c);
     if(a->dg.quahan >= 7){
        b.list[b.cnt++] = a->dg;
     }
    luudsquahan(a->right,b,c);
};
// void inquahan(DS_TheDocgia & a){ // câu i
//     for(int i = 0 ; i < a.cnt; i++){
//         for(int j = i+1; j < a.cnt; j ++){
//             if(a.list[i].quahan < a.list[j].quahan){
//                 TheDocGia tmp = a.list[i];
//                 a.list[i] = a.list[j];
//                 a.list[j] = tmp;
//             }
//         }
//     }
//     for(int i = 0 ; i < a.cnt; i++){
//         cout << a.list[i].MATHE <<endl;
//         cout << a.list[i].HO <<" "<<a.list[i].TEN <<endl;
//         cout << a.list[i].PHAI <<endl;
//         cout << a.list[i].quahan <<endl;
//         cout << a.list[i].trangthai <<endl;
//     }
// };
void savefiletree(TreeDocGia& a,ofstream& f){
    if(a == NULL){
        return;
    }
    savefiletree(a->left,f);
    f<<a->dg.MATHE<<"|"<<a->dg.HO<<" "<<a->dg.TEN<<"|"<<a->dg.PHAI<<"|"<<a->dg.trangthai<<"|"<<a->dg.sum<<"|"<<a->dg.sachmuon<<"|"<<a->dg.quahan<<endl;
    MT p = a->dg.dsmuontra;
    if(p!=NULL){
    while(p!=NULL){
      f<<p->mt.MASACH<<"|"<<p->mt.TENSACH<<"|"<<p->mt.NgayMuon<<"|"<<p->mt.NgayTra<<"|"<<p->mt.trangthai2<<endl;
      p =p->next;
    }
    }
    f<<"------------"<<endl;
    savefiletree(a->right,f);
};
void loadfiledocgia(TreeDocGia & a ,TreeDocGia & b, ifstream & f ){
    string doc;
    while(getline(f,doc)){
        MT head=NULL;MT tail = NULL;
        TheDocGia tmp;
        stringstream ss(doc);
        string s;
        tmp.dsmuontra = NULL;
        getline(ss, s, '|'); tmp.MATHE = stoi(s);
        getline(ss, s, '|');
        int pos = s.find(' ');
        if (pos != string::npos) {
        tmp.HO = s.substr(0,pos);
        tmp.TEN = s.substr(pos +1);}
        getline(ss, s, '|'); strcpy_s(tmp.PHAI, s.c_str());
        getline(ss, s, '|'); tmp.trangthai = stoi(s);
        getline(ss, s, '|'); tmp.sum = stoi(s);
        getline(ss, s, '|'); tmp.sachmuon = stoi(s);
        getline(ss, s, '|'); tmp.quahan = stoi(s);
        getline(f,doc);
        while(doc !="------------" ){
            MuonTra line;
            stringstream ss(doc);
            getline(f,doc);
            getline(ss, s, '|'); line.MASACH = s ;
            getline(ss, s, '|'); line.TENSACH =s;
            getline(ss, s, '|'); line.NgayMuon = s;
            getline(ss, s, '|'); line.NgayTra = s;
            getline(ss, s, '|'); line.trangthai2 = stoi(s);
            MT ds = makeMT();
            ds->mt = line;
            if(head == NULL){
                head = ds;
                tail = ds;
            }
            else{ tail->next = ds;
                tail = tail->next;}
            tmp.dsmuontra = head;    
        }
       TreeDocGia temp = new nodeDocGia();
       temp->dg = tmp;
       temp->left = temp->right = NULL;
       TreeDocGia tempdocten = new nodeDocGia();
       tempdocten->dg = tmp;
       tempdocten->left = tempdocten->right = NULL;
       caythedocgia(a,temp);
       caythehoten(b,tempdocten);
    }
};
void savefilesach(DS_DauSach &ds_dausach, ofstream &f){
    if(ds_dausach == NULL){
        return;
    }
    f << ds_dausach->n << endl; 
    f << "********" << endl;
    for (int i = 0; i < ds_dausach->n; i++) { //Lưu đầu sách
        f << ds_dausach->nodes[i]->ISBN << "|"
          << ds_dausach->nodes[i]->TENSACH << "|" 
          << ds_dausach->nodes[i]->SOTRANG << "|"
          << ds_dausach->nodes[i]->TACGIA << "|"
          << ds_dausach->nodes[i]->NAMXUATBAN << "|"
          << ds_dausach->nodes[i]->THELOAI << "|"
          << ds_dausach->nodes[i]->slsach << endl;
        
        // Lưu sách con
        SACH p = ds_dausach->nodes[i]->dms;
        while (p != NULL) {
            f << p->data.MASACH << "|"
              << p->data.trangthai << "|"
              << p->data.vitri << endl;
            p = p->next;
        }
        f << ds_dausach->nodes[i]->slm << endl;
        f << "------------" << endl;
    }
}
void loadfilesach(DS_DauSach &ds_dausach, ifstream &f){
    string line;
    getline(f,line); 
    ds_dausach->n = stoi(line);
    getline(f,line); 

    for(int i = 0; i < ds_dausach->n; i++) {
        ds_dausach->nodes[i] = new DauSach();
        
        
        getline(f, line);
        stringstream ss(line);
        string value;

        getline(ss, value, '|'); ds_dausach->nodes[i]->ISBN = value;
        getline(ss, value, '|'); ds_dausach->nodes[i]->TENSACH = value;
        chuanhoachuoi(ds_dausach->nodes[i]->TENSACH);
        getline(ss, value, '|'); ds_dausach->nodes[i]->SOTRANG = stoi(value);
        getline(ss, value, '|'); ds_dausach->nodes[i]->TACGIA = value;
        chuanhoachuoi(ds_dausach->nodes[i]->TACGIA);
        getline(ss, value, '|'); ds_dausach->nodes[i]->NAMXUATBAN = stoi(value);
        getline(ss, value, '|'); ds_dausach->nodes[i]->THELOAI = value;
        chuanhoachuoi(ds_dausach->nodes[i]->THELOAI);
        getline(ss, value, '|'); ds_dausach->nodes[i]->slsach = stoi(value);

        // Doc danh muc sach con
        SACH tail = NULL;
        for (int j=0; j<ds_dausach->nodes[i]->slsach; j++){
            if (!getline(f, line) || line == "------------") {
                //cout << "Loi: File data bi hong, thieu thong tin sach con." << endl;
                break; 
            } 
            stringstream ss_sach(line);
            SACH newsach = new nodeSach();
            
            getline(ss_sach, value, '|'); newsach->data.MASACH = value;
            getline(ss_sach, value, '|'); newsach->data.trangthai = stoi(value);
            getline(ss_sach, value, '|'); newsach->data.vitri = value;
            
            newsach->next = NULL;
            
            if(tail == NULL) {
                ds_dausach->nodes[i]->dms = newsach;
            } else {
                tail->next = newsach;
            }
            tail = newsach;
        }
        if (getline(f, line)) { 
            try {
                ds_dausach->nodes[i]->slm = stoi(line);
            } catch (...) {
                // Truong hop dong do bi loi hoac la "------------"
                //cout << "Loi: Khong doc duoc SLM cho ISBN " << ds_dausach->nodes[i]->ISBN << endl;
                ds_dausach->nodes[i]->slm = 0;
                if (line != "------------") {
                   getline(f, line); // Doc va bo qua dong "------------"
                }
            }
        } else {
            ds_dausach->nodes[i]->slm = 0; // Het file
        }
        
        if (ds_dausach->nodes[i]->slm != 0 || (ds_dausach->nodes[i]->slm == 0 && getline(f, line))) {
            if (line != "------------") {
                getline(f, line); // Doc va bo qua dong "------------"
            }
        }
    }
}
//Đánh mã sách tự động
string randomMaSach(string &ISBN){ // tạo mã sách có ISBN và 6 chữ số
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
    
    return string(ISBN) + "-" + result;
}

//Check mã sách trùng
bool MaSachTrung(DauSach *ds, string &Ma){
    SACH p = ds->dms;
    while (p != NULL) {
        if (p->data.MASACH == Ma)
            return true;
        p = p->next;
    }
    return false;
}

//Nhập đầu sách
void NhapDauSach(DS_DauSach &ds_dausach){
    if (ds_dausach->n >= MAX_DAUSACH){
        cout << "Danh sach da day, khong the nhap!" << endl;
        Sleep(1000);
        return;
    }

    DauSach* p = new DauSach();
    NhapISBN("Nhap ISBN(13 chu so): ", p->ISBN);
    NhapChuoi("Nhap ten sach: ", p->TENSACH);
    p->SOTRANG = NhapSo("Nhap so trang: ");
    NhapChuoi("Nhap tac gia: ", p->TACGIA);
    p->NAMXUATBAN = NhapSo("Nhap Nam xuat ban: ");
    NhapChuoi("Nhap the loai: ", p->THELOAI);
    p->slsach = NhapSo("Nhap so luong sach: ");

    for (int i=0; i<p->slsach; i++){
        SACH newSach = new nodeSach();
        newSach->next = NULL;
        // Tạo mã sách tự động và kiểm tra trùng
        string MaSach;
        do {
            MaSach = randomMaSach(p->ISBN);
        } while(MaSachTrung(p, MaSach));
        newSach->data.MASACH = MaSach; // tao ma thu vien cho sach
        newSach->data.trangthai = 0; // set duoc muon
        // hỏi người dùng muốn cất kệ nào
        while (1){
            cout << "Chon vi tri ban muon de sach thu "<< i+1 << ": " <<endl;
            cout << "1. Ke chinh"<< endl;
            cout << "2. Ke phu 1" << endl;
            cout << "3. Ke phu 2" << endl;
            cout << "Lua chon cua ban: ";
            int lc; cin >> lc;
            switch (lc){
                case 1: newSach->data.vitri = "Ke chinh"; break;
                case 2: newSach->data.vitri = "Ke phu 1"; break;
                case 3: newSach->data.vitri = "Ke phu 2"; break;
            }
            break;   
        }
        // Insert First vao dau sach
        newSach->next = p->dms;
        p->dms = newSach;
    }
    
    
    // dung thuat toan chen theo thu tu tang dan
    int ViTriChen = ds_dausach->n;
    for(int i = 0; i < ds_dausach->n; i++){
        if(p->TENSACH < ds_dausach->nodes[i]->TENSACH){
            ViTriChen = i;
            break;
        }
    }
    // Dịch chuyển các phần tử để chèn vào đúng vị trí
    for(int i = ds_dausach->n; i > ViTriChen; i--){
        ds_dausach->nodes[i] = ds_dausach->nodes[i-1];
    }
    ds_dausach->nodes[ViTriChen] = p; // thêm phần tử vào danh sách
    ds_dausach->n++;// tăng số lượng phần tử
    cout << "\nDa them thanh cong!" << endl;
}
void XoaDauSach(DS_DauSach & ds_dausach){
    string isbn_can_xoa;
    cin.ignore(); 
    NhapISBN("Nhap ISBN cua dau sach can xoa: ", isbn_can_xoa);

    int vi_tri_xoa = -1;
    // Tìm vị trí của đầu sách cần xóa dựa trên ISBN
    for (int i = 0; i < ds_dausach->n; i++) {
        if (ds_dausach->nodes[i]->ISBN == isbn_can_xoa) {
            vi_tri_xoa = i;
            break;
        }
    }

    // Nếu không tìm thấy
    if (vi_tri_xoa == -1) {
        cout << "Khong tim thay dau sach voi ISBN: " << isbn_can_xoa << endl;
        
        return;
    }

    // Kiểm tra xem có sách nào đang được mượn không
    DauSach *dau_sach_can_xoa = ds_dausach->nodes[vi_tri_xoa];
    bool dang_duoc_muon = false;
    SACH p = dau_sach_can_xoa->dms;
    while (p != NULL) {
        if (p->data.trangthai == 1) { // 1 = đã mượn
            dang_duoc_muon = true;
            break;
        }
        p = p->next;
    }

    // Nếu đang có sách được mượn, không cho xóa
    if (dang_duoc_muon) {
        cout << "Khong the xoa dau sach nay. Co sach dang duoc doc gia muon." << endl;
        
        return;
    }

    // Nếu không, tiến hành xóa
    cout << "Ban co chac chan muon xoa dau sach '" << dau_sach_can_xoa->TENSACH << "' (ISBN: " << dau_sach_can_xoa->ISBN << ")? (Y/N): ";
    string xac_nhan;
    cin.ignore();
    getline(cin,xac_nhan);
    
    if (xac_nhan == "N" || xac_nhan == "n") {
        cout << "Da huy thao tac xoa." << endl;
        
        return;
    }
    
    // Giải phóng danh sách các sách con 
    SACH current_sach = dau_sach_can_xoa->dms;
    while (current_sach != NULL) {
        SACH temp = current_sach;
        current_sach = current_sach->next;
        delete temp;
    }
    delete dau_sach_can_xoa;

    // Dịch chuyển các phần tử còn lại trong mảng nodes[] để lấp chỗ trống
    for (int i = vi_tri_xoa; i < ds_dausach->n - 1; i++) {
        ds_dausach->nodes[i] = ds_dausach->nodes[i + 1];
    }

    // Giảm số lượng đầu sách
    ds_dausach->n--;
    ds_dausach->nodes[ds_dausach->n] = NULL; 

    cout << "Da xoa dau sach thanh cong!" << endl;
    
}

//Điều chỉnh đầu sách
void DieuChinhDauSach(DS_DauSach &ds_dausach) {
    string isbn_can_sua;
    cin.ignore(); 
    
    NhapISBN("Nhap ISBN cua dau sach can hieu chinh: ", isbn_can_sua);

    int vi_tri_sua = -1;
    // Tìm vị trí của đầu sách
    for (int i = 0; i < ds_dausach->n; i++) {
        if (ds_dausach->nodes[i]->ISBN == isbn_can_sua) {
            vi_tri_sua = i;
            break;
        }
    }

    // Nếu không tìm thấy
    if (vi_tri_sua == -1) {
        cout << "Khong tim thay dau sach voi ISBN: " << isbn_can_sua << endl;
        
        return;
    }

    DauSach *p_can_sua = ds_dausach->nodes[vi_tri_sua]; // gán đầu sách cần sửa

    // Kiểm tra xem có sách nào đang được mượn không
    bool dang_duoc_muon = false;
    SACH p_sach = p_can_sua->dms;
    while (p_sach != NULL) {
        if (p_sach->data.trangthai == 1) { // 1 = đã mượn
            dang_duoc_muon = true;
            break;
        }
        p_sach = p_sach->next;
    }

    // Nếu đang có sách được mượn, không cho sửa
    if (dang_duoc_muon) {
        cout << "Khong the hieu chinh dau sach nay. Co sach dang duoc doc gia muon." << endl;
        cout << "Vui long tra het sach truoc khi hieu chinh." << endl;
        
        return;
    }
    // bản sao tạm thời để lưu các thay đổi
    DauSach data_moi = *p_can_sua;
    bool ten_sach_thay_doi = false;
    string ten_sach_cu;
    ten_sach_cu = p_can_sua->TENSACH; // Lưu lại tên cũ để so sánh

    int lua_chon;
    do {
        cout << "HIEU CHINH DAU SACH (ISBN: " << p_can_sua->ISBN << ")" << endl;
        cout << "-----------------------------------" << endl;
        cout << "1. Ten sach: " << data_moi.TENSACH << endl;
        cout << "2. So trang: " << data_moi.SOTRANG << endl;
        cout << "3. Tac gia: " << data_moi.TACGIA << endl;
        cout << "4. Nam xuat ban: " << data_moi.NAMXUATBAN << endl;
        cout << "5. The loai: " << data_moi.THELOAI << endl;
        cout << "-----------------------------------" << endl;
        cout << "ISBN, So luong sach, So luot muon KHONG duoc phep thay doi truc tiep." << endl;
        cout << "0. Luu thay doi va Thoat" << endl;
        cout << "-----------------------------------" << endl;
        cout << "Nhap lua chon cua ban: ";
        cin >> lua_chon;
        cin.ignore(); 
        string s;
        switch (lua_chon) {
            case 1:
                cout << "Nhap ten sach moi: "; cin >> s;
                stringdg(s,ten_sach_cu);
                // Đánh dấu nếu tên sách (khóa sắp xếp) bị thay đổi
                if (ten_sach_cu != data_moi.TENSACH) {
                    ten_sach_thay_doi = true;
                } else {
                    ten_sach_thay_doi = false; // Có thể người dùng gõ lại y như cũ
                }
                break;
            case 2:
                data_moi.SOTRANG = NhapSo("Nhap so trang moi: ");
                break;
            case 3:
                NhapChuoi("Nhap tac gia moi: ", data_moi.TACGIA);
                break;
            case 4:
                data_moi.NAMXUATBAN = NhapSo("Nhap nam xuat ban moi: ");
                break;
            case 5:
                NhapChuoi("Nhap the loai moi: ", data_moi.THELOAI);
                break;
            case 0:
                cout << "Dang luu thay doi..." << endl;
                break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                Sleep(1000);
                break;
        }

    } while (lua_chon != 0);

    // Cập nhật dữ liệu
    // Cập nhật các trường dữ liệu đơn giản
    p_can_sua->SOTRANG = data_moi.SOTRANG;
    p_can_sua->TACGIA = data_moi.TACGIA;
    p_can_sua->NAMXUATBAN = data_moi.NAMXUATBAN;
    p_can_sua->THELOAI = data_moi.THELOAI;
    
    // Xử lý nếu Tên Sách bị thay đổi
    if (ten_sach_thay_doi) {
        cout << "Phat hien thay doi ten sach, dang cap nhat lai vi tri sap xep..." << endl;
        
        // Cập nhật tên mới vào con trỏ
        p_can_sua->TENSACH = data_moi.TENSACH;

        // Lấy con trỏ ra khỏi mảng
        DauSach* p_can_di_chuyen = p_can_sua; // p_can_sua === ds_dausach->nodes[vi_tri_sua]

        // Dịch chuyển các phần tử sau nó lên 1 bậc 
        for (int i = vi_tri_sua; i < ds_dausach->n - 1; i++) {
            ds_dausach->nodes[i] = ds_dausach->nodes[i + 1];
        }
        
        // Giảm n tạm thời đi 1 (vì ta đã lấy 1 phần tử ra)
        // (Lưu ý: ta không giảm ds_dausach->n thật, chỉ cần tìm chỗ chèn trong phạm vi n-1)
        
        // Tìm vị trí chèn mới 
        int vi_tri_chen_moi = ds_dausach->n - 1; // Mặc định chèn vào cuối
        for (int i = 0; i < ds_dausach->n - 1; i++) {
            if (p_can_di_chuyen->TENSACH < ds_dausach->nodes[i]->TENSACH) {
                vi_tri_chen_moi = i;
                break;
            }
        }
        
        // Dịch chuyển các phần tử từ vị trí chèn mới về sau 1 bậc
        for (int i = ds_dausach->n - 1; i > vi_tri_chen_moi; i--) {
            ds_dausach->nodes[i] = ds_dausach->nodes[i - 1];
        }

        // Đặt con trỏ vào đúng vị trí mới
        ds_dausach->nodes[vi_tri_chen_moi] = p_can_di_chuyen;
    }

    cout << "Da cap nhat dau sach thanh cong!" << endl;
    
}
void In_DS_TheLoai(DS_DauSach &ds_dausach){
    if (ds_dausach == NULL || ds_dausach->n == 0){
        cout << "Khong co dau sach trong thu vien.\n";
        return;
    }

    // Thu thap danh sach the loai duy nhat
    string theloai_list[MAX_DAUSACH];
    int theloai_count = 0;

    for (int i = 0; i < ds_dausach->n; ++i){
        const string &tl = ds_dausach->nodes[i]->THELOAI;
        bool found = false;
        for (int j = 0; j < theloai_count; ++j){
            if (theloai_list[j] == tl){
                found = true;
                break;
            }
        }
        if (!found){ 
            theloai_list[theloai_count] = tl;
            theloai_count++;
        }
    }

    // Sap xep the loai tang dan (bubble sort)
    for (int i = 1; i < theloai_count; i++) {
        string key = theloai_list[i];
        int j = i - 1;
        // Di chuyen cac phan tu lon hon key len truoc
        while (j >= 0 && theloai_list[j] > key) {
            theloai_list[j + 1] = theloai_list[j];
            j = j - 1;
        }
        theloai_list[j + 1] = key;
    }

    // In theo tung the loai, trong moi the loai sap xep dau sach theo ten tang dan
    for (int ti = 0; ti < theloai_count; ++ti){
        cout << "==== The loai: " << theloai_list[ti] << " ====" << endl;
        // thu thap cac dau sach thuoc the loai nay
        DauSach* group[MAX_DAUSACH];
        int group_count = 0;
        for (int i = 0; i < ds_dausach->n; ++i){
            if (ds_dausach->nodes[i]->THELOAI == theloai_list[ti]){
                group[group_count++] = ds_dausach->nodes[i];
            }
        }
        // sap xep group theo TENSACH 
        for (int i = 1; i < group_count; i++) {
            DauSach* key = group[i];
            int j = i - 1;
            // So sanh TENSACH de di chuyen
            while (j >= 0 && group[j]->TENSACH > key->TENSACH) {
                group[j + 1] = group[j];
                j = j - 1;
            }
            group[j + 1] = key;
        }
        // In chi tiet tung dau sach
        for (int i = 0; i < group_count; ++i){
            DauSach* d = group[i];
            cout << "ISBN: " << d->ISBN << " | Ten: " << d->TENSACH
                 << " | Tac gia: " << d->TACGIA << " | Nam: " << d->NAMXUATBAN
                 << " | So luong sach: " << d->slsach << " | Luot muon: " << d->slm << endl;
        }
        cout << endl;
    }
}

void Tim_Sach_Ten(DS_DauSach &ds_dausach){
    char TenSach[101];
    cout << "Nhap ten sach muon in: ";
    cin.ignore();
    cin.getline(TenSach, 101);
    bool found = false;
    cout << "\n========Sach " << TenSach << "=========" <<endl;
    for (int i = 0; i < ds_dausach->n; i++){
        if (ds_dausach->nodes[i]->TENSACH == TenSach){
            cout << "ISBN: " << ds_dausach->nodes[i]->ISBN << endl;
            cout << "Ten sach: " << ds_dausach->nodes[i]->TENSACH << endl;
            cout << "Tac gia: " << ds_dausach->nodes[i]->TACGIA << endl;
            cout << "Nam xuat ban: " << ds_dausach->nodes[i]->NAMXUATBAN << endl;
            cout << "The loai: " << ds_dausach->nodes[i]->THELOAI << endl;

        }
    }
}
// int main() {
    // DS_DauSach dsdausach = new DS_DAUSACH();
    // TreeDocGia dsdocgia = NULL;
    // TreeDocGia dshoten = NULL;
    // DS_TheDocgia quahan;
    // ifstream Fout("D:/code/thedocgiadata.txt");
    //     // if(!Fout){
    //     //  cout << "No"<<endl;
    //     //     }
    //     loadfiledocgia(dsdocgia,dshoten,Fout);
    //     Fout.close();
    // ifstream FoutSach("D:/code/danhmucsachdata.txt");
    // // if(!FoutSach){
    // //     cout << "Khong the mo file" << endl;
    // // }
    // loadfilesach(dsdausach, FoutSach);
    // FoutSach.close();
    
    // delete dsdausach;
    
// }
