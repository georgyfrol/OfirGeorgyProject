#include "Riddle.h"
#include <iostream>
#include <cctype>

Riddle::Riddle(string _q, string _a)
    : x(-1), y(-1), question(_q), answer(_a){}

void Riddle::setPosition(int _x, int _y) {
    x = _x;
    y = _y;
}

bool Riddle::checkAnswer(const string& playerAnswer) const {
    if (playerAnswer.length() != answer.length()) return false;

    for (size_t i = 0; i < answer.length(); i++) {
        if (tolower(playerAnswer[i]) != tolower(answer[i])) {
            return false;
        }
    }
    return true;
}