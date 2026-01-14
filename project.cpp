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
#include <optional>
#include "mylib.h"

using namespace std;
const int MAX_DAUSACH = 10000;
const int lichthang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
struct Sach
{
    string MASACH; // mã sách thư viện
    int trangthai; // =0 chua muon , =1 đã mượn, =2 đã thanh lý
    string vitri;
};

struct nodeSach
{
    Sach data;
    nodeSach *next;
};
typedef nodeSach *SACH;
struct DauSach
{
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

struct DS_DAUSACH
{
    int n = 0;
    DauSach *nodes[MAX_DAUSACH];
};
typedef DS_DAUSACH *DS_DauSach;
struct Date
{
    int day;
    int month;
    int year;
};
Date time()
{
    Date a;
    time_t now = time(0);
    tm *time = localtime(&now);
    a.day = time->tm_mday;
    a.month = time->tm_mon + 1;
    a.year = time->tm_year + 1900;
    return a;
};
struct MuonTra
{
    string TENSACH;
    string MASACH;
    string NgayMuon;
    string NgayTra;
    int trangthai2; // =0 đang mượn , =1 đã trả , =2 là mất sách
};
struct nodeMuonTra
{
    MuonTra mt;
    nodeMuonTra *next;
};
typedef nodeMuonTra *MT;
MT makeMT()
{
    MT a = new nodeMuonTra();
    a->next = NULL;
    return a;
};
struct TheDocGia
{
    int MATHE = 0;
    string HO;
    string TEN;
    char PHAI[10];
    int trangthai = 1; // 0: bi khoa , 1: hoat dong;
    int sachmuon = 0;  // max = 3;
    int quahan = 0;
    MT dsmuontra = NULL;
};
struct DS_TheDocgia
{
    int cnt = 0;
    TheDocGia list[MAX_DAUSACH];
};
struct nodeDocGia
{
    TheDocGia dg;
    nodeDocGia *left, *right;
};
typedef nodeDocGia *TreeDocGia;
// TreeDocGia taothedocgia (){ // tạo cây
//     TheDocGia a ;
//     string s;
//     string stwo;
//     srand(time(0));
//     a.MATHE = 1000 + rand() % (10000 - 1000);
//     cout << "HO: "; cin.ignore();
//     getline(cin,s);
//     stringdg(s,a.HO);
//     while(a.HO.empty()){
//     cout << "HO: ";
//     getline(cin,s);
//     stringdg(s,a.HO);
//     }
//     cout << "Ten: ";
//     getline(cin,stwo);
//     stringdg(stwo,a.TEN);
//     while(a.TEN.empty()){
//     cout << "TEN: ";
//     getline(cin,stwo);
//     stringdg(stwo,a.TEN);
//     }
//     int x;
//     cout << "Phai(1.NAM , 2 NU): "; cin >> x ;
//     if(x == 1){
//         strcpy(a.PHAI, "Nam");
//     }
//     if(x==2){
//         strcpy(a.PHAI, "Nu");
//     }
//     for(int i = 0 ; i < a.HO.size();i++){
//         a.sum += a.HO[i];
//     }
//     for(int i = 0 ; i < a.TEN.size();i++){
//         a.sum += a.TEN[i];
//     }
//      TreeDocGia tmp = new nodeDocGia();
//      tmp->dg = a;
//      tmp->left =NULL;
//      tmp->right = NULL;
//      return tmp;
// };
bool sosanhten(TheDocGia a, TheDocGia b)
{
    if (a.TEN == b.TEN)
    {
        return a.HO > b.HO;
    }
    else
    {
        return a.TEN > b.TEN;
    }
}
void caythedocgia(TreeDocGia &a, TreeDocGia p)
{ // thêm cây theo mã thẻ (độc giả)
    if (a == NULL)
    {
        a = p;
        return;
    }
    if (a->dg.MATHE > p->dg.MATHE)
    {
        caythedocgia(a->left, p);
    }
    else if (a->dg.MATHE <= p->dg.MATHE)
    {
        caythedocgia(a->right, p);
    }
};
void sapxepten(TheDocGia p[], int x)
{
    for (int i = 0; i < x; ++i)
    {
        for (int j = i + 1; j < x; ++j)
        {
            if (sosanhten(p[i], p[j]))
            {
                TheDocGia tmp;
                tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
        }
    }
};
void caythehoten(TreeDocGia &a, TheDocGia p[], int &x)
{ // thêm cây theo tên (độc giả)
    if (a == NULL)
    {
        return;
    }
    caythehoten(a->left, p, x);
    p[x] = a->dg;
    x++;
    caythehoten(a->right, p, x);
};
void incay(TreeDocGia a)
{ // in cây
    if (a == nullptr)
    {
        return;
    }
    incay(a->left);
    cout << a->dg.MATHE << endl;
    cout << a->dg.HO << " " << a->dg.TEN << endl;
    cout << a->dg.PHAI << endl;
    if (a->dg.trangthai == 1)
    {
        cout << "The dang hoat dong " << endl;
    }
    else
    {
        cout << "The khong con hoat dong " << endl;
    }
    incay(a->right);
};
bool CheckMaThe(TreeDocGia t, int ma)
{
    if (t == NULL)
        return false;
    if (t->dg.MATHE == ma)
        return true;

    if (ma < t->dg.MATHE)
        return CheckMaThe(t->left, ma);
    else
        return CheckMaThe(t->right, ma);
}
void xoanode2la(TreeDocGia &a, TreeDocGia &p)
{
    if (a->left != NULL)
    {
        xoanode2la(a->left, p);
    }
    else
    {
        p->dg = a->dg;
        p = a;
        a = a->left;
    }
};
void xoathe(TreeDocGia &a, int x, int &s)
{ // xóa thẻ trong cây
    if (a == NULL)
    {
        return;
    }
    xoathe(a->left, x, s);
    xoathe(a->right, x, s);
    if (a->dg.MATHE == x)
    {
        if (a->dg.sachmuon != 0)
        {
            s = -1;
            return;
        }
        if (a->left == NULL)
        {
            TreeDocGia p = a;
            a = a->right;
            delete p;
            return;
        }
        else if (a->right == NULL)
        {
            TreeDocGia p = a;
            a = a->left;
            delete p;
            return;
        }
        else if (a->right == NULL && a->left == NULL)
        {
            delete a;
            a = nullptr;
            return;
        }
        else
        {
            TreeDocGia p = a;
            xoanode2la(a->right, p);
            delete p;
            return;
        }
    }
}
void dieuchinhmathe(TreeDocGia &a, TheDocGia &tmp)
{ // điều chỉnh thẻ
    if (a == NULL)
    {
        return;
    }
    dieuchinhmathe(a->left, tmp);
    if (a->dg.MATHE == tmp.MATHE)
    {
        a->dg.HO = tmp.HO;
        a->dg.TEN = tmp.TEN;
        strcpy(a->dg.PHAI, tmp.PHAI);
        return;
    }
    dieuchinhmathe(a->right, tmp);
};
void matsach(TreeDocGia &a, int x, string s, int &b)
{ // khóa thẻ
    if (a == NULL)
    {
        return;
    }
    if (a->dg.MATHE < x)
    {
        matsach(a->right, x, s, b);
    }
    else if (a->dg.MATHE > x)
    {
        matsach(a->left, x, s, b);
    }
    else
    {
        MT p = a->dg.dsmuontra;
        if (a->dg.dsmuontra == NULL)
        {
            b = 0;
            return;
        }
        while (p != NULL)
        {
            if (p->mt.TENSACH == s)
            {
                p->mt.trangthai2 = 2;
                a->dg.sachmuon--;
                b = 1;
                return;
            }
            p = p->next;
        }
        b = 0;
    }
};
void mokhoathe(TreeDocGia &a, int x, int b)
{ // khóa thẻ
    if (a == NULL)
    {
        return;
    }
    if (a->dg.MATHE == x)
    {
        a->dg.trangthai = b;
    }
    mokhoathe(a->left, x, b);
    mokhoathe(a->right, x, b);
};
bool Namnhuan(int x)
{
    return (x % 400 == 0) || (x % 4 == 0 && x % 100 != 0);
}
long long tinhngay(string s)
{
    stringstream ss(s);
    string in;
    int d, m, y;
    getline(ss, in, '/');
    d = stoi(in);
    getline(ss, in, '/');
    m = stoi(in);
    getline(ss, in, '/');
    y = stoi(in);
    long long total = 0;
    for (int i = 1; i < y; i++)
    {
        total += Namnhuan(i) ? 366 : 365;
    }
    for (int i = 1; i < m; i++)
    {
        if (i == 2 && Namnhuan(y))
        {
            total += 29;
        }
        else
        {
            total += lichthang[i];
        }
    }
    total += d;
    return total;
}
void checkdaymt(TreeDocGia &b, Date a)
{
    MT p = b->dg.dsmuontra;
    string homnay = to_string(a.day) + "/" + to_string(a.month) + "/" + to_string(a.year);
    int ketquat = tinhngay(homnay);
    while (p != NULL)
    {
        int ketqua;
        int ngaytra;
        if (!p->mt.NgayTra.empty())
        {
            ketqua = tinhngay(p->mt.NgayMuon);
            ngaytra = tinhngay(p->mt.NgayTra);
            if (ngaytra - ketqua > 7)
            {
                b->dg.trangthai = 0;
                b->dg.quahan = ngaytra - ketqua;
            }
            p = p->next;
            continue;
        }
        ketqua = tinhngay(p->mt.NgayMuon);
        if (ketquat - ketqua > 7)
        {
            b->dg.trangthai = 0;
            b->dg.quahan = ketquat - ketqua;
        }
        p = p->next;
    }
};
void luudsquahan(TreeDocGia &a, DS_TheDocgia &b, Date c)
{
    if (a == NULL)
    {
        return;
    }
    else
    {
        checkdaymt(a, c);
    }
    luudsquahan(a->left, b, c);
    if (a->dg.quahan >= 7)
    {
        b.list[b.cnt++] = a->dg;
    }
    luudsquahan(a->right, b, c);
};
void luudsquahantheoten(TreeDocGia &a, Date c)
{ // fix trường hợp hiện theo tên vẫn chưa bị khóa nếu mượn quá hạn
    if (a == NULL)
    {
        return;
    }
    else
    {
        checkdaymt(a, c);
    }
    luudsquahantheoten(a->left, c);
    luudsquahantheoten(a->right, c);
};
bool muontrungsach(TreeDocGia &b, string t)
{
    if (b == NULL)
    {
        return false;
    }
    else
    {
        MT p = b->dg.dsmuontra;
        while (p != NULL)
        {
            if (p->mt.TENSACH == t && p->mt.trangthai2 == 0)
            {
                return false;
            }
            p = p->next;
        }
        return true;
    }
};
void savefiletree(TreeDocGia &a, ofstream &f)
{
    if (a == NULL)
    {
        return;
    }
    savefiletree(a->left, f);
    f << a->dg.MATHE << "|" << a->dg.HO << " " << a->dg.TEN << "|" << a->dg.PHAI << "|" << a->dg.trangthai << "|" << a->dg.sachmuon << "|" << a->dg.quahan << endl;
    MT p = a->dg.dsmuontra;
    if (p != NULL)
    {
        while (p != NULL)
        {
            f << p->mt.MASACH << "|" << p->mt.TENSACH << "|" << p->mt.NgayMuon << "|" << p->mt.NgayTra << "|" << p->mt.trangthai2 << endl;
            p = p->next;
        }
    }
    f << "------------" << endl;
    savefiletree(a->right, f);
};
void loadfiledocgia(TreeDocGia &a, ifstream &f)
{
    string doc;
    while (getline(f, doc))
    {
        MT head = NULL;
        MT tail = NULL;
        TheDocGia tmp;
        stringstream ss(doc);
        string s;
        tmp.dsmuontra = NULL;
        getline(ss, s, '|');
        tmp.MATHE = stoi(s);
        getline(ss, s, '|');
        int pos = s.rfind(' ');
        if (pos != string::npos)
        {
            tmp.HO = s.substr(0, pos);
            tmp.TEN = s.substr(pos + 1);
        }
        getline(ss, s, '|');
        strcpy_s(tmp.PHAI, s.c_str());
        getline(ss, s, '|');
        tmp.trangthai = stoi(s);
        getline(ss, s, '|');
        tmp.sachmuon = stoi(s);
        getline(ss, s, '|');
        tmp.quahan = stoi(s);
        getline(f, doc);
        while (doc != "------------")
        {
            MuonTra line;
            stringstream ss(doc);
            getline(f, doc);
            getline(ss, s, '|');
            line.MASACH = s;
            getline(ss, s, '|');
            line.TENSACH = s;
            getline(ss, s, '|');
            line.NgayMuon = s;
            getline(ss, s, '|');
            line.NgayTra = s;
            getline(ss, s, '|');
            line.trangthai2 = stoi(s);
            MT ds = makeMT();
            ds->mt = line;
            if (head == NULL)
            {
                head = ds;
                tail = ds;
            }
            else
            {
                tail->next = ds;
                tail = tail->next;
            }
            tmp.dsmuontra = head;
        }
        TreeDocGia temp = new nodeDocGia();
        temp->dg = tmp;
        temp->left = temp->right = NULL;
        caythedocgia(a, temp);
    }
};
void savefilesach(DS_DauSach &ds_dausach, ofstream &f)
{
    if (ds_dausach == NULL)
    {
        return;
    }
    f << ds_dausach->n << endl;
    f << "********" << endl;
    for (int i = 0; i < ds_dausach->n; i++)
    { // Lưu đầu sách
        f << ds_dausach->nodes[i]->ISBN << "|"
          << ds_dausach->nodes[i]->TENSACH << "|"
          << ds_dausach->nodes[i]->SOTRANG << "|"
          << ds_dausach->nodes[i]->TACGIA << "|"
          << ds_dausach->nodes[i]->NAMXUATBAN << "|"
          << ds_dausach->nodes[i]->THELOAI << "|"
          << ds_dausach->nodes[i]->slsach << endl;

        // Lưu sách con
        SACH p = ds_dausach->nodes[i]->dms;
        while (p != NULL)
        {
            f << p->data.MASACH << "|"
              << p->data.trangthai << "|"
              << p->data.vitri << endl;
            p = p->next;
        }
        f << ds_dausach->nodes[i]->slm << endl;
        f << "------------" << endl;
    }
}
void loadfilesach(DS_DauSach &ds_dausach, ifstream &f)
{
    string line;
    getline(f, line);
    ds_dausach->n = stoi(line);
    getline(f, line);

    for (int i = 0; i < ds_dausach->n; i++)
    {
        ds_dausach->nodes[i] = new DauSach();

        getline(f, line);
        stringstream ss(line);
        string value;

        getline(ss, value, '|');
        ds_dausach->nodes[i]->ISBN = value;
        getline(ss, value, '|');
        chuanhoachuoi(value);
        ds_dausach->nodes[i]->TENSACH = value;
        getline(ss, value, '|');
        ds_dausach->nodes[i]->SOTRANG = stoi(value);
        getline(ss, value, '|');
        chuanhoachuoi(value);
        ds_dausach->nodes[i]->TACGIA = value;
        getline(ss, value, '|');
        ds_dausach->nodes[i]->NAMXUATBAN = stoi(value);
        getline(ss, value, '|');
        chuanhoachuoi(value);
        ds_dausach->nodes[i]->THELOAI = value;
        getline(ss, value, '|');
        ds_dausach->nodes[i]->slsach = stoi(value);

        // Doc danh muc sach con
        SACH tail = NULL;
        for (int j = 0; j < ds_dausach->nodes[i]->slsach; j++)
        {
            if (!getline(f, line) || line == "------------")
            {
                // cout << "Loi: File data bi hong, thieu thong tin sach con." << endl;
                break;
            }
            stringstream ss_sach(line);
            SACH newsach = new nodeSach();

            getline(ss_sach, value, '|');
            newsach->data.MASACH = value;
            getline(ss_sach, value, '|');
            newsach->data.trangthai = stoi(value);
            getline(ss_sach, value, '|');
            newsach->data.vitri = value;

            newsach->next = NULL;

            if (tail == NULL)
            {
                ds_dausach->nodes[i]->dms = newsach;
            }
            else
            {
                tail->next = newsach;
            }
            tail = newsach;
        }
        if (getline(f, line))
        {
            try
            {
                ds_dausach->nodes[i]->slm = stoi(line);
            }
            catch (...)
            {
                // Truong hop dong do bi loi hoac la "------------"
                // cout << "Loi: Khong doc duoc SLM cho ISBN " << ds_dausach->nodes[i]->ISBN << endl;
                ds_dausach->nodes[i]->slm = 0;
                if (line != "------------")
                {
                    getline(f, line); // Doc va bo qua dong "------------"
                }
            }
        }
        else
        {
            ds_dausach->nodes[i]->slm = 0; // Het file
        }

        if (ds_dausach->nodes[i]->slm != 0 || (ds_dausach->nodes[i]->slm == 0 && getline(f, line)))
        {
            if (line != "------------")
            {
                getline(f, line); // Doc va bo qua dong "------------"
            }
        }
    }
}

// Tạo mã sách có ISBN + 6 chữ số(tự động)
string RandomMaSach(string &ISBN)
{
    string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string result;
    while (1)
    {
        result = "";
        bool fullLetter = false, fullDigit = false;
        for (int i = 0; i < 6; i++)
        {
            char c = chars[rand() % chars.size()];
            result += c;
            if (isalpha(c))
                fullLetter = true;
            if (isdigit(c))
                fullDigit = true;
        }
        if (fullLetter && fullDigit)
            break; // Bỏ trường hợp full chữ hoặc full số
    }

    return string(ISBN) + "-" + result;
}

// Check mã sách trùng
bool MaSachTrung(DauSach *ds_dausach, string &MaSach)
{
    SACH p = ds_dausach->dms;
    while (p != NULL)
    {
        if (p->data.MASACH == MaSach)
            return true;
        p = p->next;
    }
    return false;
}

// Random vị trí sách cho sách bản sao
string RandomVitriSach()
{
    string dske[] = {"Kệ chính", "Kệ phụ 1", "Kệ phụ 2"};
    int r = rand() % 3;
    return dske[r];
}

// Insertion Sort theo TÊN SÁCH (Tăng dần)
void SortTen(DS_DauSach &ds_dausach)
{
    // Duyệt từ phần tử thứ 2 trở đi
    for (int i = 1; i < ds_dausach->n; i++)
    {
        DauSach *key = ds_dausach->nodes[i]; // Lưu con trỏ sách đang xét
        int j = i - 1;

        // Dời các phần tử đứng trước nó mà có Tên lớn hơn nó ra sau
        while (j >= 0 && ds_dausach->nodes[j]->TENSACH > key->TENSACH)
        {
            ds_dausach->nodes[j + 1] = ds_dausach->nodes[j];
            j--;
        }
        // Chèn key vào vị trí trống sau khi dời
        ds_dausach->nodes[j + 1] = key;
    }
}

// Insertion Sort theo THỂ LOẠI (Tăng dần)
// (Nếu trùng thể loại thì sắp xếp theo Tên)
void SortTheLoai(DS_DauSach &ds_dausach)
{
    for (int i = 1; i < ds_dausach->n; i++)
    {
        DauSach *key = ds_dausach->nodes[i];
        int j = i - 1;

        /* 2 điều kiện để dời:
        1. Khi thể loại chưa đúng thứ tự
        2. Khi thể loại bằng nhau và tên sách chưa đúng thứ tự
        */
        while (j >= 0 && (ds_dausach->nodes[j]->THELOAI > key->THELOAI ||
                          (ds_dausach->nodes[j]->THELOAI == key->THELOAI && ds_dausach->nodes[j]->TENSACH > key->TENSACH)))
        {
            ds_dausach->nodes[j + 1] = ds_dausach->nodes[j];
            j--;
        }
        ds_dausach->nodes[j + 1] = key;
    }
}

// Thêm bản sao (Thêm 1 cuốn vào đầu sách đã có)
void ThemBanSao(DS_DauSach &ds_dausach, string isbn, int soLuongThem)
{
    int pos = -1;
    for (int i = 0; i < ds_dausach->n; i++)
        if (ds_dausach->nodes[i]->ISBN == isbn) // Tìm sách theo isbn
        {
            pos = i;
            break;
        }
    if (pos == -1)
        return;

    DauSach *book = ds_dausach->nodes[pos];

    // Thêm bản sao dựa trên số lượng thêm
    for (int k = 0; k < soLuongThem; k++)
    {
        SACH newSach = new nodeSach();
        string MaSach;
        do
        {
            MaSach = RandomMaSach(book->ISBN);
        } while (MaSachTrung(book, MaSach));

        newSach->data.MASACH = MaSach;
        newSach->data.trangthai = 0;
        newSach->data.vitri = RandomVitriSach();

        newSach->next = book->dms;
        book->dms = newSach;
        book->slsach++;
    }
}

// Trả về: 1 (Thành công), -1 (Đang mượn - thất bại), 0 (Không tìm thấy)
int ThanhLySach(DauSach *d, string maSach)
{
    SACH p = d->dms;
    while (p != NULL)
    {
        if (p->data.MASACH == maSach)
        {
            // Nếu sách trạng thái sang 1 (đang được mượn) thì không cho thanh lý
            if (p->data.trangthai == 1)
                return -1;
            // Chuyển trạng thái sang 2 (đã thanh lý)
            p->data.trangthai = 2;
            return 1;
        }
        p = p->next;
    }
    return 0;
}
