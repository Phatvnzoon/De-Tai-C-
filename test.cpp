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
#include <optional>
#include "project.cpp"
using namespace sf;
using namespace std;

// Include các struct và hàm từ project.cpp
class INFORBORAD {
private:
    vector<string> lines; // Danh sách các dòng chữ đang lưu
    Font font;
    Text textObj;
    int maxLines;       // Số dòng tối đa hiển thị được
    float startX, startY;
    float lineHeight;

public:
    // Khởi tạo vị trí và kích thước chữ
    INFORBORAD(float x, float y, int maxL, string fontPath)
        : startX(x),          
          startY(y),          
          maxLines(maxL),    
          lineHeight(25.0f),  
          textObj(font)       
    {
       
        if (!font.openFromFile(fontPath)) { 
         
             cout << "LOI: Khong tim thay file font tai: " << fontPath << endl;
        }

     
      
        textObj.setCharacterSize(20);
        textObj.setFillColor(sf::Color::White);
    }

   
    void print(std::string content) {
        lines.push_back(content);
        if (lines.size() > maxLines) {
            lines.erase(lines.begin());
        }
    }

    void clear() {
        lines.clear();
    }

    // Hàm vẽ: Đặt trong vòng lặp draw của Main
    void draw(sf::RenderWindow &window) {
        for (int i = 0; i < lines.size(); i++) {
            textObj.setString(lines[i]);
            textObj.setPosition(Vector2f(startX ,startY + i * lineHeight));
            window.draw(textObj);
        }
    }
};
// InputField.h hoặc để đầu file main
class InputField {
public:
    RectangleShape box;
    Text text;
    Text label; // <--- Biến mới để hiện chữ "HO:", "TEN:"
    string content;
    bool isFocus = false;

    // CẬP NHẬT LẠI DÒNG NÀY (Thêm tham số labelStr và const Font &font)
    InputField(float x, float y, float w, float h, string labelStr, const Font &font) 
        : text(font), label(font) // SFML 3.0: Gắn font ngay lập tức
    {
        // Setup ô nhập
        box.setPosition({x, y});
        box.setSize({w, h});
        box.setFillColor(Color::White);
        box.setOutlineThickness(2);
        box.setOutlineColor(Color(100, 100, 100));

        // Setup chữ người dùng nhập
        text.setCharacterSize(18);
        text.setFillColor(Color::Black);
        text.setPosition({x + 5.f, y + 5.f});

        // Setup Nhãn (Label) - Đây là phần mới thêm vào
        label.setString(labelStr);
        label.setCharacterSize(18);
        label.setFillColor(Color::White);
        label.setPosition({x, y - 25.f}); // Nằm phía trên ô nhập
    }

    void handleEvent(const Event& event, const RenderWindow& window) {
        // 1. Xử lý Click chuột
        if (const auto* mouseBtn = event.getIf<Event::MouseButtonPressed>()) {
            if (mouseBtn->button == Mouse::Button::Left) {
                Vector2f mousePos(static_cast<float>(mouseBtn->position.x), 
                                  static_cast<float>(mouseBtn->position.y));
                
                if (box.getGlobalBounds().contains(mousePos)) {
                    isFocus = true;
                    box.setOutlineColor(Color::Blue);
                } else {
                    isFocus = false;
                    box.setOutlineColor(Color(100, 100, 100));
                }
            }
        }

        // 2. Xử lý Nhập phím
        if (isFocus) {
            if (const auto* textEvent = event.getIf<Event::TextEntered>()) {
                if (textEvent->unicode == 8) { // Backspace
                    if (!content.empty()) content.pop_back();
                } 
                else if (textEvent->unicode < 128 && textEvent->unicode > 31) {
                    content += static_cast<char>(textEvent->unicode);
                }
                text.setString(content);
            }
        }
    }

    void draw(RenderWindow &window) {
        window.draw(box);
        window.draw(text);
        window.draw(label); // Vẽ thêm nhãn
    }
    
    void clear() {
        content = "";
        text.setString("");
    }
};
TreeDocGia FormTaoDocGia(const Font &font) {
    RenderWindow popup(VideoMode({400, 550}), "Them Doc Gia", Style::Titlebar | Style::Close);
    popup.setFramerateLimit(60);
    // Tạo các ô nhập liệu
    InputField inputHo(50.f, 50.f, 300.f, 30.f, "HO:", font);
    InputField inputTen(50.f, 130.f, 300.f, 30.f, "TEN:", font);
    //
    Text txtError(font);
    txtError.setCharacterSize(16);
    txtError.setFillColor(Color::Red); // Màu đỏ cho sợ
    txtError.setPosition({50.f, 460.f}); // Hiện bên dưới nút Lưu
    txtError.setString(""); // Ban đầu rỗng
    // Text hiển thị giới tính
    Text txtGender(font); // SFML 3.0: Phải đưa font vào
    txtGender.setString("GIOI TINH: [1] NAM   [2] NU");
    txtGender.setCharacterSize(26);
    txtGender.setPosition({50.f, 200.f});

    Text txtSelected(font);
    txtSelected.setString("Dang chon: NAM");
    txtSelected.setCharacterSize(26);
    txtSelected.setFillColor(Color::Green);
    txtSelected.setPosition({50.f, 230.f});
    
    int phaiSelection = 1; // 1: Nam, 2: Nu

    // Nút Lưu
    RectangleShape btnSave({120.f, 40.f});
    btnSave.setPosition({140.f, 400.f});
    btnSave.setFillColor(Color::Blue);
    
    Text txtBtn(font);
    txtBtn.setString("LUU");
    txtBtn.setCharacterSize(20);
    txtBtn.setPosition({160.f, 408.f});

    // VÒNG LẶP FORM (Chặn màn hình chính)
    while (popup.isOpen()) {
        // Event Loop kiểu SFML 3.0
        while (const optional event = popup.pollEvent()) {
            
            // Xử lý thoát form (Escape) hoặc đóng cửa sổ
            if (event->is<Event::Closed>()) {
                popup.close();
                return NULL;
            }
            if (const auto* key = event->getIf<Event::KeyPressed>()) {
                if (key->code == Keyboard::Key::Escape) return NULL;

                // Chọn giới tính bằng phím số
                if (key->code == Keyboard::Key::Num1) {
                    phaiSelection = 1;
                    txtSelected.setString("Dang chon: NAM");
                }
                if (key->code == Keyboard::Key::Num2) {
                    phaiSelection = 2;
                    txtSelected.setString("Dang chon: NU");
                }
            }

            // Xử lý nhập liệu cho các ô Input
            // Lưu ý: Phải truyền *event (giá trị bên trong optional)
            inputHo.handleEvent(*event, popup);
            inputTen.handleEvent(*event, popup);

            // Xử lý click nút LƯU
            if (const auto* mouseBtn = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseBtn->button == Mouse::Button::Left) {
                    Vector2f mousePos(static_cast<float>(mouseBtn->position.x), 
                                      static_cast<float>(mouseBtn->position.y));
                    
                    if (btnSave.getGlobalBounds().contains(mousePos)) {
                        // --- LOGIC LƯU DỮ LIỆU ---
                        TheDocGia a;
                        stringdg(inputHo.content,a.HO);
                        stringdg(inputTen.content,a.TEN);
                        if (a.HO.empty() || a.TEN.empty()) {
        
        
        txtError.setString("LOI: Ten khong hop le ( so hoac de trong)!");
        
        // Reset ô nhập để bắt nhập lại
        inputHo.clear();
        inputTen.clear();
        
        // Quay lại vòng lặp, không lưu, không thoát form
        continue; 
    }
                        txtError.setString("");
                        // Tính sum (từ code cũ của bạn)
                        if (phaiSelection == 1) strcpy(a.PHAI, "Nam");
                        else strcpy(a.PHAI, "Nu");
                        srand(time(0));
                        a.MATHE = 1000 + rand() % (9000);
                        a.sum = 0;
                        for(char c : a.HO) a.sum += c;
                        for(char c : a.TEN) a.sum += c;
                        a.trangthai = 1;

                        TreeDocGia tmp = new nodeDocGia();
                        tmp->dg = a;
                        tmp->left = NULL;
                        tmp->right = NULL;
                        
                        return tmp; // Trả về node mới và thoát form
                    }
                }
            }
        }

        // VẼ FORM
        popup.clear(Color(40, 40, 40)); // Nền xám

        inputHo.draw(popup);
        inputTen.draw(popup);
        popup.draw(txtGender);
        popup.draw(txtSelected);
        popup.draw(btnSave);
        popup.draw(txtBtn);
        popup.draw(txtError);
        popup.display();
    }
    return NULL;
}
int FormNhapMaXoa(const Font &font,TreeDocGia a) {
    // 1. Tạo cửa sổ nhỏ
    float winW = 350.f;
    float winH = 220.f;
    RenderWindow popup(VideoMode({(unsigned int)winW, (unsigned int)winH}), "Xoa Doc Gia", Style::Titlebar | Style::Close);
    popup.setFramerateLimit(60);

    // 2. Giao diện
    Text title(font);
    title.setString("NHAP MA THE CAN XOA:");
    title.setCharacterSize(18);
    FloatRect textRect = title.getLocalBounds();
    title.setOrigin({textRect.size.x / 2.0f, textRect.size.y / 2.0f});
    title.setPosition({winW / 2.0f, 40.f});
    float inputW = 220.f;
    float inputH = 35.f;
    float inputX = (winW - inputW) / 2.0f; 
    InputField inputMa(inputX, 80.f, inputW, inputH, "", font);
    //
    Text txtError(font);
    txtError.setCharacterSize(16);
    txtError.setFillColor(Color::Red); // Màu đỏ
    // Đặt vị trí dưới nút Xóa
    txtError.setPosition({50.f, 200.f}); 
    txtError.setString(""); // Ban đầu để rỗng
    // Nút Xóa
    float btnW = 120.f;
    float btnH = 40.f;
    float btnX = (winW - btnW) / 2.0f;
    float btnY = 150.f;
    RectangleShape btnDelete({btnW, btnH});
    btnDelete.setPosition({btnX, btnY});
    btnDelete.setFillColor(Color::Red);

    Text txtBtn(font);
    txtBtn.setString("XOA NGAY");
    txtBtn.setCharacterSize(18);
    FloatRect btnTextRect = txtBtn.getLocalBounds();
    txtBtn.setOrigin({btnTextRect.size.x / 2.0f, btnTextRect.size.y / 2.0f});
    txtBtn.setPosition({
        btnX + btnW / 2.0f,  
        btnY + btnH / 2.0f - 5.f 
    });
    while (popup.isOpen()) {
        while (const optional event = popup.pollEvent()) {
            if (event->is<Event::Closed>()) { popup.close(); return -1; }
            if (const auto* key = event->getIf<Event::KeyPressed>()) {
                if (key->code == Keyboard::Key::Escape) { popup.close(); return -1; }
            }

            inputMa.handleEvent(*event, popup);

            if (const auto* mouseBtn = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseBtn->button == Mouse::Button::Left) {
                    Vector2f mousePos(static_cast<float>(mouseBtn->position.x), 
                                      static_cast<float>(mouseBtn->position.y));
                    
                    if (btnDelete.getGlobalBounds().contains(mousePos)) {
                        
                        // --- LOGIC TẬN DỤNG HÀM NUMBER CŨ ---
                        
                        // 1. Gán giá trị mặc định là -1 (giả sử lỗi trước)
                        int maCanXoa = -1; 
                        
                        // 2. Gọi hàm number cũ của bạn
                        // Nếu nhập sai (chữ/rỗng) -> hàm return luôn -> maCanXoa vẫn là -1
                        // Nếu nhập đúng (123) -> hàm chạy đến cuối -> gán maCanXoa = 123
                        number(inputMa.content, maCanXoa);

                        // 3. Kiểm tra giá trị sau khi gọi
                        if (maCanXoa == -1) {
                            // TRƯỜNG HỢP LỖI (Vẫn là -1)
                            
                            // Hiện thông báo lên GUI (thay vì cout)
                            txtError.setString("Loi: Vui long nhap so!");
                            
                            // Căn giữa dòng lỗi
                            FloatRect errRect = txtError.getLocalBounds();
                            txtError.setOrigin({errRect.size.x / 2.0f, 0});
                            txtError.setPosition({winW / 2.0f, 200.f});

                            // Xóa ô nhập để bắt nhập lại
                            inputMa.clear();

                            // Bỏ qua, không đóng form
                            continue; 
                        }
                        if (CheckMaThe(a,maCanXoa) == false) {
                            
                            // Báo lỗi "Không có mã trùng"
                            txtError.setString("Loi: Ma the nay khong ton tai!");
                            
                            // Căn giữa lại dòng lỗi (vì nội dung chữ thay đổi -> độ dài đổi)
                            FloatRect errRect = txtError.getLocalBounds();
                            txtError.setOrigin({errRect.size.x / 2.0f, 0});
                            txtError.setPosition({winW / 2.0f, 200.f});

                            // Xóa ô nhập để nhập lại
                            inputMa.clear();

                            // QUAN TRỌNG: Tiếp tục vòng lặp, không đóng form
                            continue;
                        }

                        // --- TRƯỜNG HỢP THÀNH CÔNG (maCanXoa != -1) ---
                        popup.close();
                        return maCanXoa;
                    }
                }
            }
        }
        popup.clear(Color(40, 40, 40));
        popup.draw(title);
        inputMa.draw(popup);
        popup.draw(btnDelete);
        popup.draw(txtBtn);
        popup.draw(txtError);
        popup.display();
    }
    return -1;
}
void incay(TreeDocGia a,INFORBORAD & b){ // in cây
    if (a==nullptr){
        return;
    }
    incay(a->left,b);
    string info = to_string(a->dg.MATHE)+" | " + string(a->dg.HO)+" "+string(a->dg.TEN)+" | "+string(a->dg.PHAI)+" | "+((a->dg.trangthai == 1) ? "Hoat Dong" : "Khoa The");
    b.print(info);
    incay(a->right,b);
};
int main(){
        DS_DauSach dsdausach = new DS_DAUSACH();
    TreeDocGia dsdocgia = NULL;
    TreeDocGia dshoten = NULL;
    DS_TheDocgia quahan;
    ifstream Fout("D:/code/thedocgiadata.txt");
        loadfiledocgia(dsdocgia,dshoten,Fout);
        Fout.close();
    ifstream FoutSach("D:/code/danhmucsachdata.txt");
    // loadfilesach(dsdausach, FoutSach);
    FoutSach.close();
    RenderWindow Window(VideoMode({800,600}),"Thu Vien");
    INFORBORAD myConsole(50, 50, 20, "C:/Windows/Fonts/arial.ttf");
    myConsole.clear();
    Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        cout << "Loi font!" << endl;
        return -1;
    }
    Text txtMenu(font);
    txtMenu.setString("[N] Them Doc Gia | [X/DEL] Xoa Doc Gia");
    txtMenu.setCharacterSize(20);
    txtMenu.setFillColor(Color::Yellow);
    txtMenu.setPosition({50.f, 20.f});
    auto RefreshList = [&]() {
    myConsole.clear();
    myConsole.print("MA THE | HO TEN | PHAI | TRANG THAI");
    myConsole.print("==================================================");
     incay(dsdocgia, myConsole);
    };
    RefreshList();
    while(true){
        while(optional event = Window.pollEvent()){
            if(event->is<Event::Closed>()){
                Window.close();
            }
            if (const auto* key = event->getIf<Event::KeyPressed>()){
                if (key->code == Keyboard::Key::Delete || key->code == Keyboard::Key::X) {
        int maXoa = FormNhapMaXoa(font,dsdocgia);

    if (maXoa != -1) {
        // Form đã check tồn tại rồi, giờ chỉ việc XÓA thôi
        xoathe(dsdocgia, maXoa); // Hoặc hàm xoathe của bạn
        
        RefreshList(); // Cập nhật bảng hiển thị
        cout << "Da xoa thanh cong: " << maXoa << endl;
    }
}
                 if (key->code == Keyboard::Key::N) {
                    TreeDocGia newNode = FormTaoDocGia(font);
                    if (newNode != NULL) {
                        caythedocgia(dsdocgia, newNode);
                        cout << "Da them: " << newNode->dg.HO << " " << newNode->dg.TEN << endl;
                        myConsole.clear();
                        incay(dsdocgia, myConsole);
                    }
                }
            }
            Window.clear(Color::Black);
            Window.draw(txtMenu);
            myConsole.draw(Window);
            Window.display();
        }
    }
    delete dsdausach;
    return 0;
}
