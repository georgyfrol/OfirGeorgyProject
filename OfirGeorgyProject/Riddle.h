#pragma once
#include <string>

using namespace std;

class Riddle {
    int x, y;
    string question;
    string answer;
public:
    Riddle(string q, string a);

    void setPosition(int _x, int _y);

    int getX() const { return x; }
    int getY() const { return y; }
    string getQuestion() const { return question; }
    string getAnswer() const { return answer; }

    bool checkAnswer(const string& playerAnswer) const;
};