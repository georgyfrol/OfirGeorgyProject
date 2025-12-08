#include "Riddle.h"
#include <iostream>
#include <cctype>

Riddle::Riddle(int _x, int _y, string _q, string _a)
    : x(_x), y(_y), question(_q), answer(_a)
{
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