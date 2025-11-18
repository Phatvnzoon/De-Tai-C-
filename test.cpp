#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "mylib.h"

using namespace std;

// Include các struct và hàm từ project.cpp
const int MAX_DAUSACH = 10000;

struct Sach {
    string MASACH;
    int trangthai;
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

struct DS_DAUSACH {
    int n = 0;
    DauSach *nodes[MAX_DAUSACH];
};
typedef DS_DAUSACH* DS_DauSach;

struct Date {
    int day;
    int month;
    int year;
};

struct MuonTra {
    string TENSACH;
    string MASACH;
    string NgayMuon;
    string NgayTra;
    int trangthai2;
};

struct nodeMuonTra {
    MuonTra mt;
    nodeMuonTra *next;
};
typedef nodeMuonTra* MT;

struct TheDocGia {
    int MATHE = 0;
    int sum = 0;
    string HO;
    string TEN;
    char PHAI[4];
    int trangthai = 1;
    int sachmuon = 0;
    int quahan = 0;
    MT dsmuontra = NULL;
};

struct DS_TheDocgia {
    int cnt = 0;
    TheDocGia list[MAX_DAUSACH];
};

struct nodeDocGia {
    TheDocGia dg;
    nodeDocGia *left, *right;
};
typedef nodeDocGia* TreeDocGia;

// Các hàm load file (giữ nguyên từ project.cpp)
Date time() {
    Date a;
    time_t now = ::time(0);
    tm* time_ptr = localtime(&now);
    a.day = time_ptr->tm_mday;
    a.month = time_ptr->tm_mon + 1;
    a.year = time_ptr->tm_year + 1900;
    return a;
}

void loadfilesach(DS_DauSach &ds_dausach, ifstream &f) {
    string line;
    getline(f, line);
    ds_dausach->n = stoi(line);
    getline(f, line);

    for (int i = 0; i < ds_dausach->n; i++) {
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

        SACH tail = NULL;
        for (int j = 0; j < ds_dausach->nodes[i]->slsach; j++) {
            getline(f, line);
            stringstream ss_sach(line);
            SACH newsach = new nodeSach();
            
            getline(ss_sach, value, '|'); newsach->data.MASACH = value;
            getline(ss_sach, value, '|'); newsach->data.trangthai = stoi(value);
            getline(ss_sach, value, '|'); newsach->data.vitri = value;
            newsach->next = NULL;
            
            if (tail == NULL) {
                ds_dausach->nodes[i]->dms = newsach;
            } else {
                tail->next = newsach;
            }
            tail = newsach;
        }
        
        getline(f, line);
        try {
            ds_dausach->nodes[i]->slm = stoi(line);
        } catch (...) {
            ds_dausach->nodes[i]->slm = 0;
        }
        getline(f, line);
    }
}

MT makeMT() {
    MT a = new nodeMuonTra();
    a->next = NULL;
    return a;
}

void caythedocgia(TreeDocGia &a, TreeDocGia p) {
    if (a == NULL) {
        a = p;
        return;
    }
    if (a->dg.MATHE > p->dg.MATHE) {
        caythedocgia(a->left, p);
    } else {
        caythedocgia(a->right, p);
    }
}

void caythehoten(TreeDocGia &a, TreeDocGia p) {
    if (a == NULL) {
        a = p;
        return;
    }
    if (a->dg.sum > p->dg.sum) {
        caythehoten(a->left, p);
    } else {
        caythehoten(a->right, p);
    }
}

void loadfiledocgia(TreeDocGia &a, TreeDocGia &b, ifstream &f) {
    string doc;
    while (getline(f, doc)) {
        TheDocGia tmp;
        stringstream ss(doc);
        string s;
        tmp.dsmuontra = NULL;
        
        getline(ss, s, '|'); tmp.MATHE = stoi(s);
        getline(ss, s, '|');
        int pos = s.find(' ');
        if (pos != string::npos) {
            tmp.HO = s.substr(0, pos);
            tmp.TEN = s.substr(pos + 1);
        }
        getline(ss, s, '|'); strcpy(tmp.PHAI, s.c_str());
        getline(ss, s, '|'); tmp.trangthai = stoi(s);
        getline(ss, s, '|'); tmp.sum = stoi(s);
        getline(ss, s, '|'); tmp.sachmuon = stoi(s);
        getline(ss, s, '|'); tmp.quahan = stoi(s);
        
        getline(f, doc);
        MT head = NULL, tail = NULL;
        while (doc != "------------") {
            MuonTra line;
            stringstream ss2(doc);
            getline(ss2, s, '|'); line.MASACH = s;
            getline(ss2, s, '|'); line.TENSACH = s;
            getline(ss2, s, '|'); line.NgayMuon = s;
            getline(ss2, s, '|'); line.NgayTra = s;
            getline(ss2, s, '|'); line.trangthai2 = stoi(s);
            
            MT ds = makeMT();
            ds->mt = line;
            if (head == NULL) {
                head = ds;
                tail = ds;
            } else {
                tail->next = ds;
                tail = tail->next;
            }
            if (!getline(f, doc)) break;
        }
        tmp.dsmuontra = head;
        
        TreeDocGia temp = new nodeDocGia();
        temp->dg = tmp;
        temp->left = temp->right = NULL;
        TreeDocGia tempdocten = new nodeDocGia();
        tempdocten->dg = tmp;
        tempdocten->left = tempdocten->right = NULL;
        caythedocgia(a, temp);
        caythehoten(b, tempdocten);
    }
}

// Class InputBox cho SFML
class InputBox {
private:
    sf::RectangleShape box;
    sf::Text displayText;
    sf::Text labelText;
    string input;
    bool isActive;
    bool isPassword;
    int maxLength;

public:
    InputBox(float x, float y, float width, float height, sf::Font& font, 
             const string& label = "", int maxLen = 50, bool pwd = false) 
        : isActive(false), isPassword(pwd), maxLength(maxLen) {
        
        box.setSize(sf::Vector2f(width, height));
        box.setPosition(x, y);
        box.setFillColor(sf::Color::White);
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color(100, 100, 100));

        displayText.setFont(font);
        displayText.setCharacterSize(18);
        displayText.setFillColor(sf::Color::Black);
        displayText.setPosition(x + 5, y + 10);

        labelText.setFont(font);
        labelText.setString(label);
        labelText.setCharacterSize(16);
        labelText.setFillColor(sf::Color::Black);
        labelText.setPosition(x, y - 25);
    }

    void handleEvent(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            isActive = box.getGlobalBounds().contains(mousePos);
            box.setOutlineColor(isActive ? sf::Color::Blue : sf::Color(100, 100, 100));
        }

        if (isActive && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8 && !input.empty()) { // Backspace
                input.pop_back();
            } else if (event.text.unicode == 13) { // Enter
                isActive = false;
                box.setOutlineColor(sf::Color(100, 100, 100));
            } else if (event.text.unicode < 128 && event.text.unicode >= 32 
                       && input.length() < maxLength) {
                input += static_cast<char>(event.text.unicode);
            }
            
            if (isPassword) {
                displayText.setString(string(input.length(), '*'));
            } else {
                displayText.setString(input);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(labelText);
        window.draw(box);
        window.draw(displayText);
    }

    string getValue() const { return input; }
    void setValue(const string& val) { 
        input = val; 
        displayText.setString(isPassword ? string(val.length(), '*') : val);
    }
    void clear() { 
        input.clear(); 
        displayText.setString("");
    }
    bool getActive() const { return isActive; }
};

// Class Button
class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    bool isHovered;

public:
    Button(float x, float y, float width, float height, 
           sf::Font& font, const string& label, sf::Color color = sf::Color(70, 130, 180)) {
        
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(x, y);
        shape.setFillColor(color);
        
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            x + (width - textBounds.width) / 2,
            y + (height - textBounds.height) / 2 - 5
        );
        
        isHovered = false;
    }

    void handleEvent(sf::Event& event, sf::RenderWindow& window) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        isHovered = shape.getGlobalBounds().contains(mousePos);
        
        if (isHovered) {
            shape.setFillColor(sf::Color(100, 160, 210));
        } else {
            shape.setFillColor(sf::Color(70, 130, 180));
        }
    }

    bool isClicked(sf::Event& event, sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            return shape.getGlobalBounds().contains(mousePos);
        }
        return false;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    void setPosition(float x, float y) {
        shape.setPosition(x, y);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            x + (shape.getSize().x - textBounds.width) / 2,
            y + (shape.getSize().y - textBounds.height) / 2 - 5
        );
    }
};

// Enum cho các màn hình
enum Screen {
    MENU,
    THEM_DOC_GIA,
    IN_DOC_GIA,
    THEM_DAU_SACH,
    IN_DAU_SACH,
    TIM_SACH,
    MUON_SACH,
    DS_MUON,
    DOC_GIA_QUA_HAN,
    TRA_SACH,
    TOP_10_SACH
};

int main() {
    // Load dữ liệu
    DS_DauSach dsdausach = new DS_DAUSACH();
    TreeDocGia dsdocgia = NULL;
    TreeDocGia dshoten = NULL;
    
    ifstream Fout("thedocgiadata.txt");
    if (Fout) {
        loadfiledocgia(dsdocgia, dshoten, Fout);
        Fout.close();
    }
    
    ifstream FoutSach("danhmucsachdata.txt");
    if (FoutSach) {
        loadfilesach(dsdausach, FoutSach);
        FoutSach.close();
    }

    // Tạo cửa sổ SFML
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Quan Ly Thu Vien");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Nếu không load được, sử dụng font mặc định
        return -1;
    }

    Screen currentScreen = MENU;

    // Tạo các button cho menu chính
    vector<Button> menuButtons;
    string menuLabels[] = {
        "1. Them/Xoa/Hieu chinh the doc gia",
        "2. In danh sach doc gia",
        "3. Them/Xoa/Hieu chinh dau sach",
        "4. In danh sach dau sach",
        "5. Tim sach theo ten",
        "6. Muon sach",
        "7. Danh sach muon cua doc gia",
        "8. Doc gia muon qua han",
        "9. Tra sach",
        "10. Top 10 sach muon nhieu nhat"
    };

    for (int i = 0; i < 10; i++) {
        menuButtons.push_back(Button(50, 100 + i * 60, 500, 50, font, menuLabels[i]));
    }

    Button backButton(50, 700, 150, 50, font, "Quay lai", sf::Color(200, 50, 50));

    // Text hiển thị
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setCharacterSize(32);
    titleText.setFillColor(sf::Color::Black);
    titleText.setPosition(400, 20);
    titleText.setString("QUAN LY THU VIEN");

    sf::Text contentText;
    contentText.setFont(font);
    contentText.setCharacterSize(16);
    contentText.setFillColor(sf::Color::Black);
    contentText.setPosition(50, 150);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Xử lý sự kiện theo màn hình
            if (currentScreen == MENU) {
                for (size_t i = 0; i < menuButtons.size(); i++) {
                    menuButtons[i].handleEvent(event, window);
                    if (menuButtons[i].isClicked(event, window)) {
                        currentScreen = static_cast<Screen>(i + 1);
                    }
                }
            } else {
                backButton.handleEvent(event, window);
                if (backButton.isClicked(event, window)) {
                    currentScreen = MENU;
                }
            }
        }

        window.clear(sf::Color(240, 240, 240));

        if (currentScreen == MENU) {
            window.draw(titleText);
            for (auto& btn : menuButtons) {
                btn.draw(window);
            }
        } else {
            // Vẽ nội dung cho các màn hình khác
            sf::Text screenTitle;
            screenTitle.setFont(font);
            screenTitle.setCharacterSize(24);
            screenTitle.setFillColor(sf::Color::Black);
            screenTitle.setPosition(50, 50);

            switch (currentScreen) {
                case IN_DOC_GIA:
                    screenTitle.setString("DANH SACH DOC GIA");
                    window.draw(screenTitle);
                    // TODO: Hiển thị danh sách độc giả
                    contentText.setString("Chuc nang dang phat trien...");
                    window.draw(contentText);
                    break;
                    
                case IN_DAU_SACH:
                    screenTitle.setString("DANH SACH DAU SACH");
                    window.draw(screenTitle);
                    // TODO: Hiển thị danh sách đầu sách
                    contentText.setString("Chuc nang dang phat trien...");
                    window.draw(contentText);
                    break;
                    
                // Thêm các case khác tương tự
                default:
                    screenTitle.setString("Chuc nang dang phat trien");
                    window.draw(screenTitle);
                    break;
            }

            backButton.draw(window);
        }

        window.display();
    }

    // Cleanup
    delete dsdausach;
    
    return 0;
}