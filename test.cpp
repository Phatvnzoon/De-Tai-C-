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
enum CurrentTab {
    TAB_DAUSACH,
    TAB_DOCGIA,
    TAB_MUONTRA
};
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
int FormNhapMaXoa(const Font &font,TreeDocGia a,string tieuDe) {
    // 1. Tạo cửa sổ nhỏ
    float winW = 350.f;
    float winH = 220.f;
    RenderWindow popup(VideoMode({(unsigned int)winW, (unsigned int)winH}), "Ma The", Style::Titlebar | Style::Close);
    popup.setFramerateLimit(60);

    // 2. Giao diện
    Text title(font);
    title.setString(tieuDe);
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
    txtBtn.setString("CAP NHAT");
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
class Button {
public:
    RectangleShape shape;
    Text text;
    Color colorIdle;  // Màu bình thường
    Color colorHover; // Màu khi chuột chỉ vào

    // Constructor
    Button(float x, float y, float w, float h, string t, const Font& font, Color btnColor) 
        : text(font) // SFML 3.0: Gắn font ngay
    {
        // 1. Setup hình dáng nút
        shape.setPosition({x, y});
        shape.setSize({w, h});
        
        colorIdle = btnColor;
        colorHover = Color(btnColor.r + 50, btnColor.g + 50, btnColor.b + 50); // Làm sáng màu lên tí
        
        shape.setFillColor(colorIdle);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(Color::White);

        // 2. Setup chữ
        text.setString(t);
        text.setCharacterSize(20);
        text.setFillColor(Color::White);

        // 3. Căn giữa chữ vào nút (Công thức chuẩn SFML 3.0)
        FloatRect textRect = text.getLocalBounds();
        text.setOrigin({textRect.size.x / 2.0f, textRect.size.y / 2.0f});
        text.setPosition({
            x + w / 2.0f,
            y + h / 2.0f - 5.f // Trừ 5f để bù chiều cao font
        });
    }

    // Hàm kiểm tra chuột có đang click vào nút không
    bool isClicked(Vector2f mousePos) {
        return shape.getGlobalBounds().contains(mousePos);
    }

    // Hàm cập nhật hiệu ứng (đổi màu khi di chuột)
    void update(Vector2f mousePos) {
        if (shape.getGlobalBounds().contains(mousePos)) {
            shape.setFillColor(colorHover); // Sáng lên
        } else {
            shape.setFillColor(colorIdle);  // Bình thường
        }
    }

    void draw(RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
};
bool FormSuaDocGia(const Font &font, TheDocGia &data) {
    float winW = 400.f; float winH = 450.f;
    RenderWindow popup(VideoMode({(unsigned int)winW, (unsigned int)winH}), "Hieu Chinh Doc Gia", Style::Titlebar | Style::Close);
    popup.setFramerateLimit(60);

    // 1. Setup ô nhập
    InputField inputHo(50.f, 80.f, 300.f, 30.f, "HO:", font);
    InputField inputTen(50.f, 160.f, 300.f, 30.f, "TEN:", font);

    // --- QUAN TRỌNG: ĐIỀN THÔNG TIN CŨ VÀO Ô ---
    // Để khi mở lên, người dùng thấy tên cũ (ví dụ: "Nguyen Van A")
    inputHo.content = data.HO;
    inputHo.text.setString(data.HO);
    
    inputTen.content = data.TEN;
    inputTen.text.setString(data.TEN);
    // -------------------------------------------

    // 2. Setup Giới tính (Vẫn lấy từ data cũ)
    Text txtGender(font);
    txtGender.setString("GIOI TINH: [1] NAM   [2] NU");
    txtGender.setCharacterSize(16);
    txtGender.setPosition({50.f, 230.f});

    Text txtSelected(font);
    // Kiểm tra giới tính cũ để hiển thị đúng
    int phaiSelection = (strcmp(data.PHAI, "Nam") == 0) ? 1 : 2;
    txtSelected.setString(phaiSelection == 1 ? "Dang chon: NAM" : "Dang chon: NU");
    txtSelected.setCharacterSize(16);
    txtSelected.setFillColor(Color::Green);
    txtSelected.setPosition({50.f, 260.f});

    // Nút Cập nhật
    RectangleShape btnSave({120.f, 40.f});
    btnSave.setPosition({140.f, 350.f});
    btnSave.setFillColor(Color::Blue);
    Text txtBtn(font); txtBtn.setString("CAP NHAT");
    txtBtn.setPosition({155.f, 358.f}); txtBtn.setCharacterSize(18);

    Text txtError(font); txtError.setFillColor(Color::Red); txtError.setCharacterSize(16);
    txtError.setPosition({50.f, 310.f});

    while (popup.isOpen()) {
        while (const optional event = popup.pollEvent()) {
            if (event->is<Event::Closed>()) { popup.close(); return false; }
            
            if (const auto* key = event->getIf<Event::KeyPressed>()) {
                if (key->code == Keyboard::Key::Escape) { popup.close(); return false; }
                if (key->code == Keyboard::Key::Num1) { phaiSelection = 1; txtSelected.setString("Dang chon: NAM"); }
                if (key->code == Keyboard::Key::Num2) { phaiSelection = 2; txtSelected.setString("Dang chon: NU"); }
            }

            inputHo.handleEvent(*event, popup);
            inputTen.handleEvent(*event, popup);

            if (const auto* mouseBtn = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseBtn->button == Mouse::Button::Left) {
                    Vector2f mousePos(static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y));
                    
                    if (btnSave.getGlobalBounds().contains(mousePos)) {
                        // Check lỗi nhập liệu
                        TheDocGia tempCheck; tempCheck.HO=""; tempCheck.TEN="";
                        stringdg(inputHo.content, tempCheck.HO);
                        stringdg(inputTen.content, tempCheck.TEN);

                        if (tempCheck.HO.empty() || tempCheck.TEN.empty()) {
                            txtError.setString("Ten khong hop le!"); continue;
                        }

                        // --- CẬP NHẬT DỮ LIỆU MỚI VÀO BIẾN data ---
                        data.HO = tempCheck.HO;
                        data.TEN = tempCheck.TEN;
                        if (phaiSelection == 1) strcpy(data.PHAI, "Nam");
                        else strcpy(data.PHAI, "Nu");

                        // TÍNH LẠI SUM
                        data.sum = 0;
                        for(char c : data.HO) data.sum += c;
                        for(char c : data.TEN) data.sum += c;
                        
                        // --- TUYỆT ĐỐI KHÔNG ĐỤNG VÀO data.trangthai ---
                        // Như vậy trạng thái cũ vẫn được bảo toàn

                        popup.close();
                        return true;
                    }
                }
            }
        }
        popup.clear(Color(40, 40, 40));
        inputHo.draw(popup); inputTen.draw(popup);
        popup.draw(txtGender); popup.draw(txtSelected);
        popup.draw(btnSave); popup.draw(txtBtn); popup.draw(txtError);
        popup.display();
    }
    return false;
}
// Trả về true nếu người dùng bấm XÁC NHẬN, false nếu Hủy
bool FormTraSach(const Font &font, int &outMaThe, string &outMaSach, string &outTenSach) {
    float winW = 400.f; float winH = 400.f; 
    RenderWindow popup(VideoMode({(unsigned int)winW, (unsigned int)winH}), "Tra Sach", Style::Titlebar | Style::Close);
    popup.setFramerateLimit(60);

    // Setup 3 ô nhập liệu
    InputField inputMaThe(50.f, 50.f, 300.f, 30.f, "MA DOC GIA:", font);
    InputField inputMaSach(50.f, 130.f, 300.f, 30.f, "MA SACH:", font);
    InputField inputTenSach(50.f, 210.f, 300.f, 30.f, "TEN SACH:", font);

    // Nút bấm
    RectangleShape btnOk({120.f, 40.f});
    btnOk.setPosition({140.f, 300.f}); btnOk.setFillColor(Color(150, 0, 150));
    
    Text txtBtn(font); txtBtn.setString("XAC NHAN"); txtBtn.setCharacterSize(18);
    FloatRect tr = txtBtn.getLocalBounds();
    txtBtn.setOrigin({tr.size.x/2, tr.size.y/2});
    txtBtn.setPosition({200.f, 318.f});

    Text txtError(font); txtError.setFillColor(Color::Red); txtError.setCharacterSize(16);
    txtError.setPosition({50.f, 260.f});

    while (popup.isOpen()) {
        while (const optional event = popup.pollEvent()) {
            if (event->is<Event::Closed>()) { popup.close(); return false; }
            if (const auto* key = event->getIf<Event::KeyPressed>()) {
                if (key->code == Keyboard::Key::Escape) { popup.close(); return false; }
            }

            inputMaThe.handleEvent(*event, popup);
            inputMaSach.handleEvent(*event, popup);
            inputTenSach.handleEvent(*event, popup);

            if (const auto* mouseBtn = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseBtn->button == Mouse::Button::Left) {
                    Vector2f mousePos(static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y));
                    
                    if (btnOk.getGlobalBounds().contains(mousePos)) {
                        
                        // CHECK LỖI
                        int tempMaThe = -1;
                        number(inputMaThe.content, tempMaThe); 

                        if (tempMaThe == -1) {
                            txtError.setString("Loi: Ma the phai la so!"); continue;
                        }
                        if (inputMaSach.content.empty() || inputTenSach.content.empty()) {
                            txtError.setString("Loi: Khong duoc de trong!"); continue;
                        }
                        string tempMaSach = "";
                        chuanhoamasach(inputMaSach.content, tempMaSach);
                        string tempTenSach = "";
                        stringdg(inputTenSach.content, tempTenSach);
                        if (tempMaSach.empty()) {
                            txtError.setString("Loi: Ma sach khong hop le!"); 
                            inputMaSach.clear(); // Xóa để nhập lại
                            continue;
                        }
                        if (tempTenSach.empty()) {
                            txtError.setString("Loi: Ten sach khong hop le!"); 
                            inputTenSach.clear();
                            continue;
                        }
                        outMaThe = tempMaThe;
                        outMaSach = inputMaSach.content;
                        outTenSach = inputTenSach.content;

                        popup.close();
                        return true; // Báo là đã nhập xong
                    }
                }
            }
        }
        popup.clear(Color(40, 40, 40));
        inputMaThe.draw(popup); inputMaSach.draw(popup); inputTenSach.draw(popup);
        popup.draw(btnOk); popup.draw(txtBtn); popup.draw(txtError);
        popup.display();
    }
    return false;
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
void indsmuontra(MT a, INFORBORAD &board) {
    MT p = a;
    bool coSach = false;
    while(p != NULL) {
        // Chỉ in sách đang mượn (trangthai2 == 0)
        if(p->mt.trangthai2 == 0) {
            // Tạo chuỗi thông tin: MA SACH | TEN SACH | NGAY MUON | NGAY TRA
            string info = p->mt.MASACH + " | " + p->mt.TENSACH + " | " + p->mt.NgayMuon;
            board.print(info);
            coSach = true;
        }
        p = p->next;
    }
    if (!coSach) {
        board.print("(Doc gia nay hien khong muon cuon nao)");
    }
}
void checkdsmuonsach(TreeDocGia a, int x, INFORBORAD &board) {
    if (a == NULL) {
        return;
    }
    else if (a->dg.MATHE > x) {
        checkdsmuonsach(a->left, x, board);
    }
    else if (a->dg.MATHE < x) {
        checkdsmuonsach(a->right, x, board);
    }
    else {
        // TÌM THẤY: Gọi hàm in ở trên
        indsmuontra(a->dg.dsmuontra, board);
    }   
}
int main(){
    DS_DauSach dsdausach = new DS_DAUSACH();
    TreeDocGia dsdocgia = NULL;
    TreeDocGia dshoten = NULL;
    DS_TheDocgia quahan;
    ifstream Fout("D:/code/thedocgiadata.txt");
        loadfiledocgia(dsdocgia,dshoten,Fout);
        Fout.close();
    ifstream FoutSach("D:/code/danhmucsachdata.txt");
    FoutSach.close();
    RenderWindow Window(VideoMode({800,600}),"Thu Vien");
    INFORBORAD myConsole(30.f, 80.f, 20, "C:/Windows/Fonts/arial.ttf");
    myConsole.clear();
    Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        cout << "Loi font!" << endl;
        return -1;
    }
    Button btnAdd(600.f, 150.f, 150.f, 50.f, "THEM MOI", font, Color(0, 150, 0));
    Button btnDel(600.f, 220.f, 150.f, 50.f, "XOA THE", font, Color(0, 150, 0));
    Button btnEdit(600.f, 290.f, 150.f, 50.f, "DIEU CHINH", font, Color(0, 150, 0));
    Button btnlock(600.f, 360.f, 150.f, 50.f, "KHOA THE", font, Color(0, 150, 0));
    CurrentTab currentTab = TAB_DOCGIA;
    float tabW = 150.f; float tabH = 40.f;
    Button btnTabSach(30.f, 10.f, tabW, tabH, "DAU SACH", font, Color(100, 100, 100));
    Button btnTabDocGia(190.f, 10.f, tabW, tabH, "DOC GIA", font, Color(0, 100, 200)); // Màu xanh (Active)
    Button btnTabMuon(350.f, 10.f, tabW, tabH, "DS MUON", font, Color(100, 100, 100));
    float btnX = 620.f; float btnW = 150.f; float btnH = 50.f;
    Button btnSach_Them(btnX, 80.f, btnW, btnH, "THEM SACH", font, Color(0, 150, 0));
    Button btnSach_Xoa(btnX, 150.f, btnW, btnH, "XOA SACH", font, Color(0, 150, 0));
    //
    Button btnMuon_in(600.f, 150.f, 150.f, 50.f, "IN DS MUON", font, Color(0, 150, 0));
    Button btnMuon_Tra(600.f, 220.f, 150.f, 50.f, "TRA SACH", font, Color(0, 150, 0));
    auto RefreshList = [&]() {
        myConsole.clear();   
        btnTabSach.colorIdle = Color(100, 100, 100);
        btnTabDocGia.colorIdle = Color(100, 100, 100);
        btnTabMuon.colorIdle = Color(100, 100, 100);

        // 2. Tab nào đang chọn thì đổi colorIdle thành Xanh
        if (currentTab == TAB_DAUSACH) btnTabSach.colorIdle = Color(0, 100, 200);
        if (currentTab == TAB_DOCGIA)  btnTabDocGia.colorIdle = Color(0, 100, 200);
        if (currentTab == TAB_MUONTRA) btnTabMuon.colorIdle = Color(0, 100, 200);
       if (currentTab == TAB_DOCGIA) {
            myConsole.print("MA THE | HO TEN | PHAI | TRANG THAI");
            myConsole.print("=============================================");
            incay(dsdocgia, myConsole);
        }
        else if (currentTab == TAB_DAUSACH) {
            myConsole.print("ISBN | TEN SACH | SL | TON KHO");
            myConsole.print("==========================================");
            myConsole.print("(Dang phat trien chuc nang Sach...)");
        }
        else {
            myConsole.print("MA SACH | MA THE | NGAY MUON | HAN TRA");
            myConsole.print("=======================================");
            myConsole.print("(Vui long chon chuc nang IN DS MUON)");
            
        }
    };
    RefreshList();
    while(true){
            Vector2i pixelPos = Mouse::getPosition(Window);
            Vector2f mousePos(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));
            btnTabSach.update(mousePos); btnTabDocGia.update(mousePos); btnTabMuon.update(mousePos);
            if (currentTab == TAB_DOCGIA) {
            btnAdd.update(mousePos); btnDel.update(mousePos); 
            btnEdit.update(mousePos); btnlock.update(mousePos);}
            else if (currentTab == TAB_DAUSACH) {
            btnSach_Them.update(mousePos); btnSach_Xoa.update(mousePos);
        }
        while(optional event = Window.pollEvent()){
            if(event->is<Event::Closed>()){
                Window.close();
            }
            if (const auto* mouseBtn = event->getIf<Event::MouseButtonPressed>()){
                if (mouseBtn->button == Mouse::Button::Left){
                    Vector2f clickPos(static_cast<float>(mouseBtn->position.x), 
                                      static_cast<float>(mouseBtn->position.y));
                    if (btnTabDocGia.isClicked(clickPos)) { currentTab = TAB_DOCGIA; RefreshList(); }
                    if (btnTabSach.isClicked(clickPos)) { currentTab = TAB_DAUSACH; RefreshList(); }
                    if (btnTabMuon.isClicked(clickPos)) { currentTab = TAB_MUONTRA; RefreshList(); }       
        if (currentTab == TAB_DOCGIA) {                      
        if (btnDel.isClicked(clickPos)) {
        int maXoa = FormNhapMaXoa(font,dsdocgia,"Nhap Ma Can Xoa:");
        if (maXoa != -1) {
        xoathe(dsdocgia, maXoa); 
        RefreshList(); 
        cout << "Da xoa thanh cong: " << maXoa << endl;
    }
}
                 if (btnAdd.isClicked(clickPos)) {
                    TreeDocGia newNode = FormTaoDocGia(font);
                    if (newNode != NULL) {
                        caythedocgia(dsdocgia, newNode);
                        cout << "Da them: " << newNode->dg.HO << " " << newNode->dg.TEN << endl;
                        myConsole.clear();
                        incay(dsdocgia, myConsole);
                    }
                }
                if (btnEdit.isClicked(clickPos)) {  
                int maCanSua = FormNhapMaXoa(font,dsdocgia,"NHAP MA CAN SUA:"); 
                if (maCanSua != -1) {     
                TheDocGia dataMoi;
                dataMoi.MATHE = maCanSua;                         
                if (FormSuaDocGia(font, dataMoi)) {                              
                dieuchinhmathe(dsdocgia, dataMoi);
                cout << "Da cap nhat thong tin!" << endl;
                RefreshList(); // 
               }              
              }
    }        
            if(btnlock.isClicked(clickPos)){
                int makhoa = FormNhapMaXoa(font,dsdocgia,"NHAP MA CAN KHOA:");
                if(makhoa != -1){
                    khoathe(dsdocgia,makhoa);
                    RefreshList();
                }
            }
            }
          else if (currentTab == TAB_DAUSACH) {
                        if (btnSach_Them.isClicked(clickPos)) {                           
                            // FormTaoSach(...)
                        }
                    }
           else if(currentTab == TAB_MUONTRA){
            if (btnMuon_in.isClicked(clickPos)) {
               int macanxem = FormNhapMaXoa(font,dsdocgia,"MA THE CAN IN DANH SACH");
               if(macanxem!=-1){
            myConsole.clear();
            myConsole.print("MA SACH | MA THE | NGAY MUON | HAN TRA");
            myConsole.print("=======================================");
            myConsole.print(">> DANH SACH MUON CUA: " + to_string(macanxem));
            checkdsmuonsach(dsdocgia, macanxem, myConsole);
               }
            }
            if (btnMuon_Tra.isClicked(clickPos)) {
    int maThe = -1;
    string maSach = "";
    string tenSach = "";
    if (FormTraSach(font, maThe, maSach, tenSach) == true) {
        trasach(dsdocgia, dsdausach, maThe, maSach, tenSach);
        RefreshList();
    }
}
           }           
        }
        }
            Window.clear(Color::Black);
            btnTabSach.draw(Window); btnTabDocGia.draw(Window); btnTabMuon.draw(Window);
            if (currentTab == TAB_DOCGIA) {
            btnAdd.draw(Window);
            btnDel.draw(Window);
            btnEdit.draw(Window);
            btnlock.draw(Window);}
            else if (currentTab == TAB_DAUSACH) {
            btnSach_Them.draw(Window); 
            btnSach_Xoa.draw(Window);
        }
        else if(currentTab = TAB_MUONTRA){
            btnMuon_in.draw(Window);
            btnMuon_Tra.draw(Window);
        }
            myConsole.draw(Window);
            Window.display();
        }
    }
    delete dsdausach;
    return 0;
}
