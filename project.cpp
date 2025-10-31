#include <bits/stdc++.h>
#include "mylib.h"
using namespace std;
const int MAX_DAUSACH = 10000;

struct Sach {
    char MASACH[24]; // Ma sach theo thu vien
    int trangthai; // =0 dc muon , =1 đã mượn, =2 đã thanh lý
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

struct MuonTra {
    char TENSACH[101];
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
    MT a = new nodeMuonTra();
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
    int sachmuon = 0 ;// max = 3;
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
    NhapChuoi("Ho: ", a.HO,51);
    NhapChuoi("Ten: ", a.TEN,31);
    int x;
    cout << "Phai(1.NAM , 2 NU): "; cin >> x ;
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
       cout << "Dieu chinh thanh cong "<<endl;
       return ;
    }
};
void indsmuontra(MT & a){   // in ds mượn trả
    MT p = a;
    while(p == NULL){
        cout << p->mt.MASACH <<" "<<p->mt.TENSACH<<" "<< p->mt.NgayMuon<<" "<<p->mt.NgayTra<<" "<<p->mt.trangthai2<<endl;
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
void changebook(DS_DAUSACH & a,const char* s,const char* t,int x){
    for(int i = 0 ; i < a.n;++i){
        if(a.nodes[i]->TENSACH == t){
            SACH tmp = a.nodes[i]->FirstSach;
            while(tmp != NULL){
                if(tmp->data.MASACH == s){
                    tmp->data.trangthai =x;
                    return;
                }
                tmp = tmp->next;
            }
        }
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
        if(b->dg.sachmuon >= 3){
          cout << "sinh vien nay dang muon 3 cuon sach"<<endl;
          return;
        }
        if(b->dg.trangthai == 0){
             cout << "the bi khoa "<<endl;
             return;
        }
        // thiếu nếu đang giữ sách quá 7 ngày thì ko cho mượn
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
                         b->dg.sachmuon ++;
                         temp->data.trangthai =1;
                         a.nodes[i]->slm ++;
                         return;
                        }
                        else{
                            while(p->next!=NULL){
                                p = p->next;
                            }
                            strcpy(tmp->mt.MASACH,a.nodes[i]->FirstSach->data.MASACH); // đã mượn
                         tmp->mt.trangthai2 = 0;
                            p->next = tmp;
                            b->dg.sachmuon ++;
                            temp->data.trangthai =1;
                             a.nodes[i]->slm ++;
                            return;
                        }
                    }
                    temp = temp->next;
                }
            }
        }
    }
};
void trasach(TreeDocGia& a,DS_DAUSACH & b, int x,const char* s,const char* t){
     if(a == NULL){
        return; 
    }
    else if(a->dg.MATHE > x){
       trasach(a->left,b,x,s,t);
    }
    else if(a->dg.MATHE < x){
        trasach(a->right,b,x,s,t);
    }
    else{
         changebook(b,s,t,0);
         MT temp = a->dg.dsmuontra;
         while(temp != NULL){
            if (temp->mt.MASACH == s){
                temp->mt.trangthai2 = 1 ;
            }
         }
    }
};
void top10book(DS_DauSach & a){  //10 sách dc mượn nhiều nhất (j)
    int cnt = 0;
    DauSach tmp[10];
    for (int i = 0; i < a->n ; ++i)
    {    if(cnt < 10){
         tmp[cnt++].slm = a->nodes[i]->slm;
         strcpy(tmp[cnt].TENSACH,a->nodes[i]->TENSACH);
    }
        else {
            for(int j = 0 ; j <cnt;++i){
                 if(tmp[j].slm <a->nodes[i]->slm){
                    tmp[j].slm = a->nodes[i]->slm;
                    strcpy(tmp[j].TENSACH ,a->nodes[i]->TENSACH);
                 }
            }
            for(int i = 0 ; i < cnt ; ++i){
                for(int j = i +1 ; j < cnt ; ++j){
                    if(tmp[i].slm < tmp[j].slm){
                        DauSach temp;
                        temp.slm = tmp[i].slm;
                        strcpy(temp.TENSACH,tmp[i].TENSACH);
                        tmp[i].slm = tmp[j].slm;
                        strcpy(tmp[i].TENSACH,tmp[j].TENSACH);
                        tmp[j].slm = temp.slm;
                        strcpy(tmp[j].TENSACH,temp.TENSACH);
                    }
                }
            }
        }
    }
    for(int i = 0 ; i < cnt; ++i){
        cout << tmp[i].TENSACH<<" "<<tmp[i].slm<<endl;
    }
};

//Đánh mã sách tự động
string randomMaSach(char *ISBN){ // tạo mã sách có ISBN và 6 chữ số
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
    SACH p = ds->FirstSach;
    while (p != NULL) {
        if (strcmp(p->data.MASACH, Ma.c_str()) == 0)
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

    DauSach *p = new DauSach();
    cin.ignore();
    NhapMa("Nhap ISBN(13 so): ", p->ISBN, 15); 
    NhapChuoi("Nhap ten sach: ", p->TENSACH,101);
    p->SOTRANG = NhapSo("Nhap so trang: ");
    NhapChuoi("Nhap tac gia: ", p->TACGIA, 51);
    p->NAMXUATBAN = NhapSo("Nhap Nam xuat ban: ");
    NhapChuoi("Nhap the loai: ", p->THELOAI, 31);
    p->slsach = NhapSo("Nhap so luong sach: ");

    for (int i=0; i<p->slsach; i++){
        SACH newSach = new nodeSach();
        newSach->next = NULL;
        string MaSach;
        do {
            MaSach = randomMaSach(p->ISBN);
        } while(MaSachTrung(p, MaSach));
        strcpy(newSach->data.MASACH, MaSach.c_str()); // tao ma thu vien cho sach
        newSach->data.trangthai = 0; // set duoc muon
        strcpy(newSach->data.vitri, "Ke chinh");
        // Insert First vao dau sach
        newSach->next = p->FirstSach;
        p->FirstSach = newSach;
    }
    
    // Tạo mã sách tự động và kiểm tra trùng
    string MaSach;
    
        
    int ViTriChen = ds_dausach->n;
    for(int i = 0; i < ds_dausach->n; i++){
        if(strcmp(p->TENSACH, ds_dausach->nodes[i]->TENSACH) < 0){
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
    char isbn_can_xoa[15];
    cin.ignore(); 
    NhapMa("Nhap ISBN cua dau sach can xoa: ", isbn_can_xoa, 15);

    int vi_tri_xoa = -1;
    // Tìm vị trí của đầu sách cần xóa dựa trên ISBN
    for (int i = 0; i < ds_dausach->n; i++) {
        if (strcmp(ds_dausach->nodes[i]->ISBN, isbn_can_xoa) == 0) {
            vi_tri_xoa = i;
            break;
        }
    }

    // Nếu không tìm thấy
    if (vi_tri_xoa == -1) {
        cout << "Khong tim thay dau sach voi ISBN: " << isbn_can_xoa << endl;
        system("pause");
        return;
    }

    // Kiểm tra xem có sách nào đang được mượn không
    DauSach *dau_sach_can_xoa = ds_dausach->nodes[vi_tri_xoa];
    bool dang_duoc_muon = false;
    SACH p = dau_sach_can_xoa->FirstSach;
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
        system("pause");
        return;
    }

    // Nếu không, tiến hành xóa
    cout << "Ban co chac chan muon xoa dau sach '" << dau_sach_can_xoa->TENSACH << "' (ISBN: " << dau_sach_can_xoa->ISBN << ")? (Y/N): ";
    string xac_nhan;
    cin.ignore();
    getline(cin,xac_nhan);
    
    if (xac_nhan == "N" || xac_nhan == "n") {
        cout << "Da huy thao tac xoa." << endl;
        system("pause");
        return;
    }
    
    // Giải phóng danh sách các sách con 
    SACH current_sach = dau_sach_can_xoa->FirstSach;
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
    system("pause");
}

//Điều chỉnh đầu sách
void DieuChinhDauSach(DS_DauSach &ds_dausach) {
    char isbn_can_sua[15];
    cin.ignore(); 
    
    NhapMa("Nhap ISBN cua dau sach can hieu chinh: ", isbn_can_sua, 15);

    int vi_tri_sua = -1;
    // Tìm vị trí của đầu sách
    for (int i = 0; i < ds_dausach->n; i++) {
        if (strcmp(ds_dausach->nodes[i]->ISBN, isbn_can_sua) == 0) {
            vi_tri_sua = i;
            break;
        }
    }

    // Nếu không tìm thấy
    if (vi_tri_sua == -1) {
        cout << "Khong tim thay dau sach voi ISBN: " << isbn_can_sua << endl;
        system("pause");
        return;
    }

    DauSach *p_can_sua = ds_dausach->nodes[vi_tri_sua]; // gán đầu sách cần sửa

    // Kiểm tra xem có sách nào đang được mượn không
    bool dang_duoc_muon = false;
    SACH p_sach = p_can_sua->FirstSach;
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
        system("pause");
        return;
    }

    // Hiển thị menu chỉnh sửa
    // Tạo một bản sao tạm thời để lưu các thay đổi
    DauSach data_moi = *p_can_sua;
    bool ten_sach_thay_doi = false;
    char ten_sach_cu[101];
    strcpy(ten_sach_cu, p_can_sua->TENSACH); // Lưu lại tên cũ để so sánh

    int lua_chon;
    do {
        system("cls");
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
        cin.ignore(); // Xóa bộ đệm

        switch (lua_chon) {
            case 1:
                // Giả sử bạn có hàm NhapChuoi trong mylib.h
                NhapChuoi("Nhap ten sach moi: ", data_moi.TENSACH, 101);
                // Đánh dấu nếu tên sách (khóa sắp xếp) bị thay đổi
                if (strcmp(ten_sach_cu, data_moi.TENSACH) != 0) {
                    ten_sach_thay_doi = true;
                } else {
                    ten_sach_thay_doi = false; // Có thể người dùng gõ lại y như cũ
                }
                break;
            case 2:
                // Giả sử bạn có hàm NhapSo trong mylib.h
                data_moi.SOTRANG = NhapSo("Nhap so trang moi: ");
                break;
            case 3:
                NhapChuoi("Nhap tac gia moi: ", data_moi.TACGIA, 51);
                break;
            case 4:
                data_moi.NAMXUATBAN = NhapSo("Nhap nam xuat ban moi: ");
                break;
            case 5:
                NhapChuoi("Nhap the loai moi: ", data_moi.THELOAI, 31);
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
    strcpy(p_can_sua->TACGIA, data_moi.TACGIA);
    p_can_sua->NAMXUATBAN = data_moi.NAMXUATBAN;
    strcpy(p_can_sua->THELOAI, data_moi.THELOAI);
    
    // Xử lý nếu Tên Sách (khóa sắp xếp) bị thay đổi
    if (ten_sach_thay_doi) {
        cout << "Phat hien thay doi ten sach, dang cap nhat lai vi tri sap xep..." << endl;
        
        // Cập nhật tên mới vào con trỏ
        strcpy(p_can_sua->TENSACH, data_moi.TENSACH);

        // Lấy con trỏ ra khỏi mảng
        DauSach* p_can_di_chuyen = p_can_sua; // p_can_sua === ds_dausach->nodes[vi_tri_sua]

        // Dịch chuyển các phần tử sau nó lên 1 bậc để xóa nó khỏi vị trí cũ
        for (int i = vi_tri_sua; i < ds_dausach->n - 1; i++) {
            ds_dausach->nodes[i] = ds_dausach->nodes[i + 1];
        }
        
        // Giảm n tạm thời đi 1 (vì ta đã lấy 1 phần tử ra)
        // (Lưu ý: ta không giảm ds_dausach->n thật, chỉ cần tìm chỗ chèn trong phạm vi n-1)
        
        // Tìm vị trí chèn mới (giống logic hàm NhapDauSach)
        int vi_tri_chen_moi = ds_dausach->n - 1; // Mặc định chèn vào cuối
        for (int i = 0; i < ds_dausach->n - 1; i++) {
            if (strcmp(p_can_di_chuyen->TENSACH, ds_dausach->nodes[i]->TENSACH) < 0) {
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
    system("pause");
}

void In_DS_TheLoai(DS_DauSach &ds_dausach){
    char theloai[31];
    cout << "Nhap the loai: ";
    cin.ignore();
    cin.getline(theloai,31);
    cout <<"\n=======Sach theo the loai: " << theloai <<"========" << endl;
    bool found = false;
    int thutu = 0;
    for (int i=0; i<ds_dausach->n; i++){
        if(strcmp(ds_dausach->nodes[i]->THELOAI, theloai) == 0){
            found = true;
            thutu += 1;
            cout << thutu << ". " << ds_dausach->nodes[i]->TENSACH << endl; 
        }
        SACH p = ds_dausach->nodes[i]->FirstSach; // Insert First vao mang in
    }
    if (!found){
        cout << "Khong tim thay sach cua the loai nay!" << endl;
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
        if (strcmp(ds_dausach->nodes[i]->TENSACH, TenSach) == 0){
            found = true;
            cout << "ISBN: " << ds_dausach->nodes[i]->ISBN << endl;
            cout << "Ten sach: " << ds_dausach->nodes[i]->TENSACH << endl;
            cout << "Tac gia: " << ds_dausach->nodes[i]->TACGIA << endl;
            cout << "Nam xuat ban: " << ds_dausach->nodes[i]->NAMXUATBAN << endl;
            cout << "The loai: " << ds_dausach->nodes[i]->THELOAI << endl;
            cout << "Cac ma sach co trong sach: "<< endl;
            SACH p = ds_dausach->nodes[i]->FirstSach;
            int j = 1;
            while (p != NULL) {
                cout << j << ". " << p->data.MASACH << " - ";
                if (p->data.trangthai == 0) cout << "Cho muon duoc";
                else if (p->data.trangthai == 1) cout << "Da co doc gia muon";
                else cout << "Da thanh ly";
                cout << " Vi tri: " << p->data.vitri << endl;
                p = p->next;
                j++;
            }
        }
    }
}

int main() {
    DS_DauSach dsdausach = new DS_DAUSACH();
    TreeDocGia dsdocgia = NULL;
    TreeDocGia dshoten = NULL;

    
    do{
        system("cls");
        cout << "========== QUAN LY THU VIEN ==========" << endl;
        cout << "1. Quan ly doc gia" << endl;
        cout << "2. Quan ly dau sach" << endl;
        cout << "3. Quan ly muon/tra sach" << endl;
        cout << "4. Thong ke" << endl;
        cout << "0. Thoat" << endl;
        cout << "======================================" << endl;
        cout << "Vui long nhap lua chon: ";
        
        int lc; 
        cin >> lc;
        
        switch(lc){
            case 0:
                cout << "Tam biet!" << endl;
                return 0;
                
            case 1:{ // QUAN LY DOC GIA
                system("cls");
                cout << "===== QUAN LY DOC GIA =====" << endl;
                cout << "1. Them the doc gia" << endl;
                cout << "2. Xoa the doc gia" << endl;
                cout << "3. Hieu chinh the doc gia" << endl;
                cout << "4. Khoa the doc gia" << endl;
                cout << "5. In danh sach doc gia" << endl;
                cout << "0. Quay lai" << endl;
                cout << "===========================" << endl;
                cout << "Vui long nhap lua chon: ";
                
                int lc2; 
                cin >> lc2;
                
                switch (lc2){
                    case 1:{
                        system("cls");
                        TreeDocGia tmp = taothedocgia();
                        TreeDocGia temp = new nodeDocGia();
                        temp->dg = tmp->dg;
                        temp->left = temp->right = NULL;
                        caythedocgia(dsdocgia, tmp);
                        caythehoten(dshoten, temp);
                        cout << "\nThem thanh cong!" << endl;
                        system("pause");
                        break;
                    }
                    case 2:{
                        system("cls");
                        cout << "Nhap ma the muon xoa: ";
                        int x; 
                        cin >> x;
                        cout << "Ban co chac chan muon xoa ma the " << x << " hay khong (Y/N): ";
                        string xac_nhan;
                        cin.ignore();
                        getline(cin,xac_nhan);
                        if (xac_nhan == "N" || xac_nhan == "n"){
                            cout << "Da huy thao tac xoa!" << endl;
                            system("pause");
                            break;
                        }
                        xoathe(dsdocgia, x);
                        xoathe(dshoten, x);
                        cout << "\nXoa thanh cong!" << endl;
                        system("pause");
                        break;
                    }
                    case 3:{
                        system("cls");
                        TheDocGia tmp;
                        cout << "Nhap ma the muon dieu chinh: ";
                        cin >> tmp.MATHE;
                        cin.ignore();
                        NhapChuoi("Ho moi: ", tmp.HO, 51);
                        NhapChuoi("Ten moi: ", tmp.TEN, 31);
                        int g;
                        cout << "Phai (1.NAM, 2.NU): ";
                        cin >> g;
                        if(g == 1){
                            strcpy(tmp.PHAI, "Nam");
                        } else if(g == 2){
                            strcpy(tmp.PHAI, "Nu");
                        }
                        dieuchinhmathe(dsdocgia, tmp);
                        dieuchinhmathe(dshoten, tmp);
                        system("pause");
                        break;
                    }
                    case 4:{
                        system("cls");
                        cout << "Nhap ma the muon khoa: ";
                        int x; 
                        cin >> x;
                        khoathe(dsdocgia, x);
                        khoathe(dshoten, x);
                        cout << "\nKhoa the thanh cong!" << endl;
                        system("pause");
                        break;
                    }
                    case 5:{
                        system("cls");
                        cout << "1. In theo ho ten" << endl;
                        cout << "2. In theo ma the" << endl;
                        cout << "Lua chon: ";
                        int lc3; 
                        cin >> lc3;
                        system("cls");
                        if (lc3 == 1){
                            incay(dshoten);
                        } else if (lc3 == 2){
                            incay(dsdocgia);
                        }
                        system("pause");
                        break;
                    }
                }
                break;
            }
            
            case 2:{ // QUAN LY DAU SACH
                system("cls");
                cout << "===== QUAN LY DAU SACH =====" << endl;
                cout << "1. Nhap dau sach" << endl;
                cout << "2. Xoa dau sach" << endl;
                cout << "3. In danh sach theo the loai" << endl;
                cout << "4. Tim sach theo ten" << endl;
                cout << "0. Quay lai" << endl;
                cout << "============================" << endl;
                cout << "Lua chon: ";
                int lc2;
                cin >> lc2;
                if(lc2 == 1){
                    system("cls");
                    NhapDauSach(dsdausach);
                    system("pause");
                }
                else if (lc2 == 2){
                    system("cls");
                    XoaDauSach(dsdausach);
                    system("pause");
                }
                else if (lc2 == 3){
                    system("cls");
                    In_DS_TheLoai(dsdausach);
                    system("pause");
                }
                else if (lc2 == 4){
                    system("cls");
                    Tim_Sach_Ten(dsdausach);
                    system("pause");
                }
                break;
            }
            
            default:
                cout << "Lua chon khong hop le!" << endl;
                system("pause");
        }
    } while(1);
    delete dsdausach;
    return 0;
}