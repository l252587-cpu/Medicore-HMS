#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class GUI {
public:
    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded;
    
    sf::Color bgColor;
    sf::Color primaryColor;
    sf::Color textColor;
    sf::Color buttonColor;
    sf::Color buttonHoverColor;
    sf::Color accentColor;
    GUI();
    ~GUI();

    // Helper Methods
    void drawText(const string& text, float x, float y, int size, sf::Color color);
    void drawButton(const string& text, float x, float y, float w, float h, sf::Color color);
    bool isMouseOver(float x, float y, float w, float h, sf::Vector2i mousePos);
    string getInput(const string& prompt);

    // Screens
    void showSplashScreen();
    int showLoginMenu();
    bool showLoginScreen(const string& role, int& id, string& password);
    void showMessage(const string& message);
    int showPatientMenu(const string& name, float balance);
    int showDoctorMenu(const string& name, const string& spec);
    int showAdminMenu();
    int showSelectionList(const string& title, const string options[], int count);
};

#endif