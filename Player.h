#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include<ctime>

using namespace std;
class Player{
    private:   
        string name;
        float score;
        time_t timeLimit;
    public:
        Player(string name);
        virtual int get_Score(); //virtual function to get the score of players
        virtual string get_Name(); //virtual function to get the score of players
        int set_Score(int score); //function to set the score of players
        string set_Name(string name); //function to set the score of players
        void increaseScore(); //function to increase the score of players
        void resetScore(); //function to reset the score of players
        virtual int get_Score();
        virtual string get_Name();
        int set_Score(int score);
        string set_Name(string name);
        virtual int get_TimeLimit();
        void increaseScore();
        void resetScore();

};
#endif
