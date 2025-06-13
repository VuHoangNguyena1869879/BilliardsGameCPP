#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "GameText.h"
#include "Button.h"
#include <ctime>
#include "Timer.h"
#include "Table.h"
#include "Cuestick.h"

using namespace std;

typedef enum GameState {
        titleScreen,
        mainGame,
        gameOver,
        pauseScreen
} GameState;

class BillardGame 
{
private:
    sf::RenderWindow* window;
    GameText* title;
    Button* start;
    Button* exit;
    GameState currentState;
    GameState previousState;
    GameText* player1;
    Timer* clock;
    Button* cont;
    double timeLimit;
    Table* table;
    Cuestick* cuestick;
public:
    BillardGame() { 
        currentState = titleScreen;
        window = new sf::RenderWindow(sf::VideoMode(1850,1000,32),
                                     "Billard Game");
        window->setPosition(sf::Vector2i(35,45));
        title = new GameText("A Love of Thunder.ttf","Billiard Game",100);
        start = new Button(400,150,750,300);
        exit = new Button(400,150,750,460);
        cont = new Button(600,150,650,350);
        title->move(600,60);
        start->set_gameText("A Love of Thunder.ttf","Start",100);
        exit->set_gameText("A Love of Thunder.ttf","Exit",100);
        cont->set_gameText("A Love of Thunder.ttf","Continue",100);
        clock = new Timer();
        timeLimit = 20;
        table = new Table(330,150);
        cuestick = new Cuestick(400,300);
    }

    ~BillardGame() {
    }

    void run() {
        while (window->isOpen()) {
            handleEvents();
            updateGame();
            drawGame();
            
        }
    }
    private: 

        GameState get_State() {
            return currentState;
        }
        void set_State(GameState state) {
            currentState = state; 
        }
        void handleEvents() {
            switch (currentState) {  
                case titleScreen: 
                    sf::Event event;
                    previousState = get_State();
                    while (window->pollEvent(event)) {
                        if (event.type == sf::Event::Closed) window->close();
                        if (event.type == sf::Event::MouseButtonPressed) {
                            if (event.mouseButton.button == sf::Mouse::Left) {
                                if (exit->isMouseOver(event.mouseButton.x,event.mouseButton.y)) 
                                    window->close();
                                if (start->isMouseOver(event.mouseButton.x,event.mouseButton.y)) {
                                    clock->set_Start(time(NULL));
                                    currentState = mainGame;
                                    break;
                                }
                            }
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                            set_State(pauseScreen);
                        }      
                    }
                    break;
                case mainGame:
                        previousState = get_State();
                        clock->set_End(time(NULL));
                        if (timeLimit-clock->timeDiff() <= 0) window->close();
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                            set_State(pauseScreen);
                            break;
                        }
                        while (window->pollEvent(event)) {
                            if (event.type == sf::Event::Closed) window->close();
                            if (event.type == sf::Event::MouseButtonPressed) {
                                if (event.mouseButton.button == sf::Mouse::Left){
                                    if (!cuestick->isLocked()) {
                                        cuestick->lockRotation();
                                    } else {
                                        cuestick->lockRotation();
                                        cuestick->rotate(event.mouseButton.x,event.mouseButton.y,
                                                         cuestick->get_Position().x,
                                                         cuestick->get_Position().y);
                                        }
                                }
                            }
                            if (event.type == sf::Event::MouseMoved && !cuestick->isLocked()) {
                                cuestick->rotate(event.mouseMove.x,event.mouseMove.y,
                                                 cuestick->get_Position().x,
                                                 cuestick->get_Position().y);
                            }
                            break;
                        }
                    break;
                case gameOver:
                    previousState = get_State();
                    while (window->pollEvent(event)) {
                        if (event.type == sf::Event::Closed) window->close();
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                            set_State(pauseScreen);
                        }
                    }
                    break; 
                case pauseScreen:
                    while (window->pollEvent(event)) {
                        if (event.type == sf::Event::Closed) window->close(); 
                        if (event.type == sf::Event::MouseButtonPressed) {
                            if (event.mouseButton.button == sf::Mouse::Left) {
                                if (cont->isMouseOver(event.mouseButton.x,event.mouseButton.y)) {
                                    clock->set_Start(time(NULL));
                                    set_State(previousState);
                                }
                            }
                        }
                    }
                    break;
            }
        }

        void updateGame() {
            switch (currentState) {  
                case titleScreen: 
                    window->clear();
                    break;
                case mainGame:
                    window->clear();
                    break;
                case gameOver:
                    window->clear();
                    break; 
                case pauseScreen:
                    window->clear();
                    break;
            }
        }

        void drawGame() {
            switch (currentState) {  
                case titleScreen: 
                    title->draw(window);
                    start->draw(window);
                    exit->draw(window);
                    window->display();
                    break;
                case mainGame:
                    table->draw(window);
                    cuestick->draw(window);
                    window->display();
                    break;
                case gameOver:
                    window->display();
                    break; 
                case pauseScreen:
                    cont->draw(window);
                    window->display();
                    break;
            }
        }
};

int main() {
    BillardGame game;
    game.run();
    return 0;
}