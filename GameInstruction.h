#ifndef GAMEINSTRUCTION_H
#define GAMEINSTRUCTION_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class GameInstruction
{
private:
    sf::Font font;
    sf::Text* text;

public:
    GameInstruction() {
        fstream file;
        file.open("GameInstruction.txt", ios::in);
        if (file.is_open()) {
            cout<<"Read the file successfully";
        } else {
            cout<<"Failed to read the file";
        }
    }

    void draw(sf::RenderWindow* window) {
        window->draw(*text);
    }

    void move(int x,int y) {
        text->move(sf::Vector2f(x,y));
    }

    void set_Color(sf::Color color) {
        text->setFillColor(color);
    }
};

#endif