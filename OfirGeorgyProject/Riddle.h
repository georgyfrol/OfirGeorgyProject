#pragma once
#include <string>

using namespace std;

class Riddle {
    int x, y;
    string question;
    string answer;
public:
    
    Riddle(int x, int y, string q, string a); 

    int getX() const { return x; }
    int getY() const { return y; }
    string getQuestion() const { return question; }
    string getAnswer() const { return answer; }
    bool checkAnswer(const string& playerAnswer) const;
};