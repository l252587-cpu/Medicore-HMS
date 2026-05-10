#include "GUI.h"
#include <sstream>
GUI::GUI() {
    fontLoaded = false;
    if (font.loadFromFile("arial.ttf")) fontLoaded = true;
    else if (font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) fontLoaded = true;
    else if (font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf")) fontLoaded = true;

    // --- SOFT STEEL BLUE THEME (Non-White Bright Theme) ---
    bgColor = sf::Color(241, 245, 249);          // Soft Blue-Gray background
    primaryColor = sf::Color(37, 99, 235);       // Royal Blue for Headers
    textColor = sf::Color(30, 41, 59);           // Dark Slate for Text
    buttonColor = sf::Color(255, 255, 255);      // White Buttons
    buttonHoverColor = sf::Color(219, 234, 254); // Light Sky Blue
    accentColor = sf::Color(225, 29, 72);        // Crimson for Back/Exit

    if (fontLoaded) cout << "SUCCESS: Font loaded!" << endl;
}
GUI::~GUI() {
    if (window.isOpen()) window.close();
}

void GUI::drawText(const string& text, float x, float y, int size, sf::Color color) {
    if (!fontLoaded || text.empty()) return;

    sf::Text t;
    t.setFont(font);
    t.setString(text);
    t.setCharacterSize(size);

    // CHANGE THIS: Ensure it uses the color passed in (which should be textColor)
    t.setFillColor(color);

    t.setPosition(x, y);
    window.draw(t);
}
void GUI::drawButton(const string& text, float x, float y, float w, float h, sf::Color color) {
    // 1. Draw a slightly darker border so the button is visible
    sf::RectangleShape btn(sf::Vector2f(w, h));
    btn.setPosition(x, y);
    btn.setFillColor(color);
    btn.setOutlineThickness(2);
    btn.setOutlineColor(sf::Color(200, 200, 200)); // Light grey border
    window.draw(btn);

    // 2. Draw the text inside using the DARK text color
    if (fontLoaded) {
        sf::Text t;
        t.setFont(font);
        t.setString(text);
        t.setCharacterSize(18);
        t.setFillColor(textColor); // <--- THIS MAKES IT VISIBLE

        // Center the text
        sf::FloatRect textRect = t.getLocalBounds();
        t.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        t.setPosition(x + w / 2.0f, y + h / 2.0f);

        window.draw(t);
    }
}

bool GUI::isMouseOver(float x, float y, float w, float h, sf::Vector2i mousePos) {
    return mousePos.x >= x && mousePos.x <= x + w && mousePos.y >= y && mousePos.y <= y + h;
}

void GUI::showSplashScreen() {
    window.create(sf::VideoMode(700, 500), "MediCore HMS", sf::Style::None);
    window.setFramerateLimit(60);
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed || e.type == sf::Event::KeyPressed) {
                window.close(); return;
            }
        }

        float elapsed = clock.getElapsedTime().asSeconds();
        if (elapsed > 3.5f) { window.close(); return; }

        // Clear with the new bright bgColor
        window.clear(bgColor);

        // --- 1. Medical Cross (Bright Primary Color) ---
        float centerX = 350, centerY = 180;
        sf::RectangleShape vPart(sf::Vector2f(20, 60)), hPart(sf::Vector2f(60, 20));
        vPart.setOrigin(10, 30); hPart.setOrigin(30, 10);
        vPart.setPosition(centerX, centerY); hPart.setPosition(centerX, centerY);
        vPart.setFillColor(primaryColor); hPart.setFillColor(primaryColor);
        window.draw(vPart); window.draw(hPart);

        // --- 2. Title (Deep Navy for readability) ---
        drawText("MEDICORE", 230, 240, 52, textColor);

        // --- 3. Subtitle ---
        drawText("Hospital Management System", 215, 310, 18, sf::Color(71, 85, 105));

        // --- 4. Loading Bar ---
        sf::RectangleShape barBg(sf::Vector2f(400, 8));
        barBg.setPosition(150, 400);
        barBg.setFillColor(sf::Color(226, 232, 240)); // Light Gray background bar
        window.draw(barBg);

        sf::RectangleShape barProgress(sf::Vector2f((elapsed / 3.5f) * 400, 8));
        barProgress.setPosition(150, 400);
        barProgress.setFillColor(primaryColor);
        window.draw(barProgress);

        window.display();
    }
}
int GUI::showLoginMenu() {
    window.create(sf::VideoMode(700, 500), "MediCore - Login Portal", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Layout Constants
    float btnW = 280;
    float btnH = 55;
    float btnX = (700 - btnW) / 2; // Perfectly Centered
    float startY = 180;
    float spacing = 65;

    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 4;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (isMouseOver(btnX, startY, btnW, btnH, mousePos)) { window.close(); return 1; }
                if (isMouseOver(btnX, startY + spacing, btnW, btnH, mousePos)) { window.close(); return 2; }
                if (isMouseOver(btnX, startY + spacing * 2, btnW, btnH, mousePos)) { window.close(); return 3; }
                if (isMouseOver(btnX, startY + spacing * 3 + 20, btnW, btnH, mousePos)) { window.close(); return 4; }
            }
        }

        window.clear(bgColor);

        // --- Header Section ---
        sf::RectangleShape header(sf::Vector2f(700, 100));
        header.setFillColor(sf::Color(15, 30, 60));
        window.draw(header);

        drawText("MediCore HMS", 240, 20, 36, primaryColor);
        drawText("Select your access level to continue", 215, 65, 16, sf::Color(148, 163, 184));

        // --- Buttons ---
        // Helper to determine color based on hover
        auto getBtnColor = [&](float x, float y) {
            return isMouseOver(x, y, btnW, btnH, mousePos) ? buttonHoverColor : buttonColor;
            };

        drawButton("Login as Patient", btnX, startY, btnW, btnH, getBtnColor(btnX, startY));
        drawButton("Login as Doctor", btnX, startY + spacing, btnW, btnH, getBtnColor(btnX, startY + spacing));
        drawButton("Login as Admin", btnX, startY + (spacing * 2), btnW, btnH, getBtnColor(btnX, startY + (spacing * 2)));

        // Exit button (Distinct Red style)
        sf::Color exitColor = isMouseOver(btnX, startY + spacing * 3 + 20, btnW, btnH, mousePos)
            ? sf::Color(185, 28, 28) : sf::Color(127, 29, 29);
        drawButton("Exit System", btnX, startY + spacing * 3 + 20, btnW, btnH, exitColor);

        window.display();
    }
    return 4;
}
// ... (Other GUI methods like showPatientMenu, showDoctorMenu follow same pattern)

// ==================== LOGIN MENU ====================

//int GUI::showLoginMenu() {
//    window.create(sf::VideoMode(700, 500), "MediCore - Login");
//    window.setFramerateLimit(60);
//
//    float btnX = 250, btnW = 200, btnH = 45;
//    float btn1Y = 210, btn2Y = 270, btn3Y = 330, btn4Y = 400;
//
//    while (window.isOpen()) {
//        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//        sf::Event event;
//
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//                return 4;
//            }
//            if (event.type == sf::Event::MouseButtonPressed) {
//                if (isMouseOver(btnX, btn1Y, btnW, btnH, mousePos)) { window.close(); return 1; }
//                if (isMouseOver(btnX, btn2Y, btnW, btnH, mousePos)) { window.close(); return 2; }
//                if (isMouseOver(btnX, btn3Y, btnW, btnH, mousePos)) { window.close(); return 3; }
//                if (isMouseOver(btnX, btn4Y, btnW, btnH, mousePos)) { window.close(); return 4; }
//            }
//        }
//
//        window.clear(bgColor);
//
//        sf::RectangleShape header(sf::Vector2f(700, 80));
//        header.setPosition(0, 0);
//        header.setFillColor(sf::Color(15, 40, 80));
//        window.draw(header);
//
//        drawText("MediCore", 270, 15, 32, primaryColor);
//        drawText("Hospital Management System", 195, 52, 16, sf::Color(148, 163, 184));
//        drawText("Login As", 300, 155, 22, textColor);
//
//        sf::Color c1 = isMouseOver(btnX, btn1Y, btnW, btnH, mousePos) ? buttonHoverColor : buttonColor;
//        sf::Color c2 = isMouseOver(btnX, btn2Y, btnW, btnH, mousePos) ? buttonHoverColor : buttonColor;
//        sf::Color c3 = isMouseOver(btnX, btn3Y, btnW, btnH, mousePos) ? buttonHoverColor : buttonColor;
//        sf::Color c4 = isMouseOver(btnX, btn4Y, btnW, btnH, mousePos) ? sf::Color(127, 29, 29) : sf::Color(69, 10, 10);
//
//        drawButton("Patient", btnX, btn1Y, btnW, btnH, c1);
//        drawButton("Doctor", btnX, btn2Y, btnW, btnH, c2);
//        drawButton("Admin", btnX, btn3Y, btnW, btnH, c3);
//        drawButton("Exit", btnX, btn4Y, btnW, btnH, c4);
//
//        window.display();
//    }
//    return 4;
//}

// ==================== LOGIN SCREEN ====================

bool GUI::showLoginScreen(const string& role, int& id, string& password) {
    window.create(sf::VideoMode(500, 420), "MediCore - " + role + " Login");
    window.setFramerateLimit(60);

    string idStr = "";
    string passStr = "";
    bool typingId = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); return false; }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (isMouseOver(100, 130, 300, 40, mousePos)) typingId = true;
                if (isMouseOver(100, 210, 300, 40, mousePos)) typingId = false;
                if (isMouseOver(175, 310, 150, 40, mousePos)) {
                    if (!idStr.empty() && !passStr.empty()) {
                        id = 0;
                        for (char c : idStr) if (isdigit(c)) id = id * 10 + (c - '0');
                        password = passStr; window.close(); return true;
                    }
                }
                if (isMouseOver(175, 365, 150, 35, mousePos)) { window.close(); return false; }
            }
            if (event.type == sf::Event::TextEntered) {
                char c = (char)event.text.unicode;
                if (c == '\b') {
                    if (typingId && !idStr.empty()) idStr.pop_back();
                    else if (!typingId && !passStr.empty()) passStr.pop_back();
                }
                else if (c >= 32 && c < 127) {
                    if (typingId) idStr += c; else passStr += c;
                }
            }
        }

        window.clear(bgColor);

        // --- Header (Fixed Overlap) ---
        sf::RectangleShape header(sf::Vector2f(500, 85));
        header.setFillColor(primaryColor);
        window.draw(header);
        drawText("MediCore Portal", 145, 12, 26, sf::Color::White); //
        drawText(role + " Login", 205, 48, 14, sf::Color(191, 219, 254)); // Moved down

        // --- ID Input (Lightened Bar) ---
        drawText("ID Number", 100, 110, 14, sf::Color(100, 116, 139));
        sf::RectangleShape idBox(sf::Vector2f(300, 40));
        idBox.setPosition(100, 130);
        idBox.setFillColor(sf::Color(226, 232, 240)); // Soft Gray Bar
        idBox.setOutlineColor(typingId ? primaryColor : sf::Color(203, 213, 225));
        idBox.setOutlineThickness(2);
        window.draw(idBox);
        drawText(idStr + (typingId ? "|" : ""), 115, 138, 18, textColor); // Visible Text

        // --- Password Input (Lightened Bar) ---
        drawText("Password", 100, 190, 14, sf::Color(100, 116, 139));
        sf::RectangleShape passBox(sf::Vector2f(300, 40));
        passBox.setPosition(100, 210);
        passBox.setFillColor(sf::Color(226, 232, 240)); // Soft Gray Bar
        passBox.setOutlineColor(!typingId ? primaryColor : sf::Color(203, 213, 225));
        passBox.setOutlineThickness(2);
        window.draw(passBox);
        string stars(passStr.size(), '*');
        drawText(stars + (!typingId ? "|" : ""), 115, 218, 18, textColor);

        // Buttons
        sf::Vector2i m = sf::Mouse::getPosition(window);
        drawButton("Login", 175, 310, 150, 40, isMouseOver(175, 310, 150, 40, m) ? buttonHoverColor : buttonColor);
        drawButton("Back", 175, 365, 150, 35, isMouseOver(175, 365, 150, 35, m) ? sf::Color(254, 226, 226) : buttonColor);

        window.display();
    }
    return false;
}

// ==================== SHOW MESSAGE ====================

void GUI::showMessage(const string& message) {
    window.create(sf::VideoMode(500, 200), "MediCore");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (isMouseOver(175, 130, 150, 40, mousePos)) {
                    window.close();
                    return;
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return ||
                    event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    return;
                }
            }
        }

        window.clear(bgColor);

        sf::RectangleShape header(sf::Vector2f(500, 60));
        header.setPosition(0, 0);
        header.setFillColor(sf::Color(15, 40, 80));
        window.draw(header);

        drawText("MediCore", 185, 15, 22, primaryColor);
        drawText(message, 30, 80, 16, textColor);

        sf::Color btnC = isMouseOver(175, 130, 150, 40, mousePos) ? buttonHoverColor : buttonColor;
        drawButton("OK", 175, 130, 150, 40, btnC);

        window.display();
    }
}
#include "GUI.h"
#include <sstream>

// ... [Keep your existing constructor and helper methods here] ...

string GUI::getInput(const string& prompt) {
    window.create(sf::VideoMode(600, 350), "Input Required", sf::Style::Titlebar | sf::Style::Close);
    string inputStr = "";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); return ""; }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8 && !inputStr.empty()) inputStr.pop_back();
                else if (event.text.unicode == 13) window.close();
                else if (event.text.unicode < 128 && event.text.unicode > 31)
                    inputStr += static_cast<char>(event.text.unicode);
            }
        }
        window.clear(bgColor);
        drawText(prompt, 50, 60, 22, primaryColor); //

        sf::RectangleShape box(sf::Vector2f(500, 50));
        box.setPosition(50, 120);
        box.setFillColor(sf::Color(226, 232, 240)); // New Light Bar
        box.setOutlineThickness(2);
        box.setOutlineColor(primaryColor);
        window.draw(box);

        drawText(inputStr + "|", 65, 130, 24, textColor); // High Contrast Text
        drawText("Press ENTER to submit", 50, 200, 16, sf::Color(148, 163, 184));
        window.display();
    }
    return inputStr;
}

int GUI::showSelectionList(const string& title, const string options[], int count) {
    window.create(sf::VideoMode(700, 700), title, sf::Style::Titlebar | sf::Style::Close);
    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); return -1; }
            if (event.type == sf::Event::MouseButtonPressed) {
                for (int i = 0; i < count; i++) {
                    if (isMouseOver(50, 100 + i * 50, 600, 40, mousePos)) {
                        window.close();
                        return i;
                    }
                }
            }
        }
        window.clear(bgColor);
        drawText(title, 50, 40, 26, primaryColor);
        for (int i = 0; i < count; i++) {
            sf::Color c = isMouseOver(50, 100 + i * 50, 600, 40, mousePos) ? buttonHoverColor : buttonColor;
            drawButton(options[i], 50, 100 + i * 50, 600, 40, c);
        }
        window.display();
    }
    return -1;
}
// ==================== PATIENT MENU ====================

int GUI::showPatientMenu(const string& patientName, float balance) {
    window.create(sf::VideoMode(700, 620), "MediCore - Patient");
    window.setFramerateLimit(60);

    float btnX = 200, btnW = 300, btnH = 42;
    float startY = 170;
    float gap = 50;

    string options[] = {
        "1. Book Appointment",
        "2. Cancel Appointment",
        "3. View My Appointments",
        "4. View My Medical Records",
        "5. View My Bills",
        "6. Pay Bill",
        "7. Top Up Balance",
        "8. Logout"
    };

    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 8;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                for (int i = 0; i < 8; i++) {
                    if (isMouseOver(btnX, startY + i * gap, btnW, btnH, mousePos)) {
                        window.close();
                        return i + 1;
                    }
                }
            }
        }

        window.clear(bgColor);

        // header
        sf::RectangleShape header(sf::Vector2f(700, 100));
        header.setPosition(0, 0);
        header.setFillColor(sf::Color(15, 40, 80));
        window.draw(header);

        drawText("Welcome, " + patientName, 20, 12, 24, primaryColor);
        drawText("Patient Portal", 20, 50, 16, sf::Color(148, 163, 184));

        // balance box
        sf::RectangleShape balBox(sf::Vector2f(180, 38));
        balBox.setPosition(500, 32);
        balBox.setFillColor(sf::Color(20, 83, 45));
        balBox.setOutlineColor(sf::Color(34, 197, 94));
        balBox.setOutlineThickness(1);
        window.draw(balBox);

        ostringstream ss;
        ss << "PKR " << (int)balance;
        drawText(ss.str(), 520, 42, 16, sf::Color(34, 197, 94));

        drawText("Patient Menu", 280, 128, 18, textColor);

        // divider
        sf::RectangleShape div(sf::Vector2f(500, 1));
        div.setPosition(100, 155);
        div.setFillColor(sf::Color(30, 58, 138));
        window.draw(div);

        for (int i = 0; i < 8; i++) {
            sf::Color c = isMouseOver(btnX, startY + i * gap, btnW, btnH, mousePos)
                ? buttonHoverColor : buttonColor;
            if (i == 7) c = isMouseOver(btnX, startY + i * gap, btnW, btnH, mousePos)
                ? sf::Color(127, 29, 29) : sf::Color(69, 10, 10);
            drawButton(options[i], btnX, startY + i * gap, btnW, btnH, c);
        }

        window.display();
    }
    return 8;
}

// ==================== DOCTOR MENU ====================

int GUI::showDoctorMenu(const string& doctorName, const string& spec) {
    window.create(sf::VideoMode(700, 530), "MediCore - Doctor");
    window.setFramerateLimit(60);

    float btnX = 200, btnW = 300, btnH = 42;
    float startY = 160;
    float gap = 52;

    string options[] = {
        "1. View Today's Appointments",
        "2. Mark Appointment Complete",
        "3. Mark Appointment No-Show",
        "4. Write Prescription",
        "5. View Patient Medical History",
        "6. Logout"
    };

    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 6;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                for (int i = 0; i < 6; i++) {
                    if (isMouseOver(btnX, startY + i * gap, btnW, btnH, mousePos)) {
                        window.close();
                        return i + 1;
                    }
                }
            }
        }

        window.clear(bgColor);

        sf::RectangleShape header(sf::Vector2f(700, 100));
        header.setPosition(0, 0);
        header.setFillColor(sf::Color(15, 40, 80));
        window.draw(header);

        drawText("Dr. " + doctorName, 20, 12, 24, primaryColor);
        drawText("Specialization: " + spec, 20, 50, 16, sf::Color(148, 163, 184));
        drawText("Doctor Portal", 20, 72, 14, sf::Color(100, 116, 139));

        drawText("Doctor Menu", 285, 120, 18, textColor);

        sf::RectangleShape div(sf::Vector2f(500, 1));
        div.setPosition(100, 148);
        div.setFillColor(sf::Color(30, 58, 138));
        window.draw(div);

        for (int i = 0; i < 6; i++) {
            sf::Color c = isMouseOver(btnX, startY + i * gap, btnW, btnH, mousePos)
                ? buttonHoverColor : buttonColor;
            if (i == 5) c = isMouseOver(btnX, startY + i * gap, btnW, btnH, mousePos)
                ? sf::Color(127, 29, 29) : sf::Color(69, 10, 10);
            drawButton(options[i], btnX, startY + i * gap, btnW, btnH, c);
        }

        window.display();
    }
    return 6;
}

// ==================== ADMIN MENU ====================

int GUI::showAdminMenu() {
    window.create(sf::VideoMode(700, 680), "MediCore - Admin");
    window.setFramerateLimit(60);

    float btnX = 200, btnW = 300, btnH = 40;
    float startY = 145;
    float gap = 48;

    string options[] = {
        "1. Add Doctor",
        "2. Remove Doctor",
        "3. View All Patients",
        "4. View All Doctors",
        "5. View All Appointments",
        "6. View Unpaid Bills",
        "7. Discharge Patient",
        "8. View Security Log",
        "9. Generate Daily Report",
        "10. Logout"
    };

    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 10;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                for (int i = 0; i < 10; i++) {
                    if (isMouseOver(btnX, startY + i * gap, btnW, btnH, mousePos)) {
                        window.close();
                        return i + 1;
                    }
                }
            }
        }

        window.clear(bgColor);

        sf::RectangleShape header(sf::Vector2f(700, 90));
        header.setPosition(0, 0);
        header.setFillColor(sf::Color(15, 40, 80));
        window.draw(header);

        drawText("Admin Panel", 265, 12, 28, primaryColor);
        drawText("MediCore Administration", 230, 52, 16, sf::Color(148, 163, 184));

        drawText("Admin Menu", 285, 108, 18, textColor);

        sf::RectangleShape div(sf::Vector2f(500, 1));
        div.setPosition(100, 133);
        div.setFillColor(sf::Color(30, 58, 138));
        window.draw(div);

        for (int i = 0; i < 10; i++) {
            sf::Color c = isMouseOver(btnX, startY + i * gap, btnW, btnH, mousePos)
                ? buttonHoverColor : buttonColor;
            if (i == 9) c = isMouseOver(btnX, startY + i * gap, btnW, btnH, mousePos)
                ? sf::Color(127, 29, 29) : sf::Color(69, 10, 10);
            drawButton(options[i], btnX, startY + i * gap, btnW, btnH, c);
        }

        window.display();
    }
    return 10;
}