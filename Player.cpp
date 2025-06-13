#include <string>
#include<ctime>
#include "Player.h"

using namespace std;
Player::Player(string name){
    this->name = name;
    score = 0;
}
int Player::get_Score(){
    return 0;
}
string Player::get_Name(){
    return 0;
}
int Player::set_Score(int score){
    this->score = score;
}
string Player::set_Name(string name){
    this->name=name;
}
<<<<<<< HEAD

void Player::increase_score(){
=======
int Player::get_TimeLimit(){
    return 0;
}
void Player::increaseScore(){
>>>>>>> c3c53885e640b13020892d89a37bbfbea0028f2f
    score++;
}
void Player::resetScore(){
    score = 0;
}
