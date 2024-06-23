#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <numeric>

using namespace std;

vector<string> lexer(const string& text) {
    vector<string> tokens;
    string token;
    bool is_double = false;
    string spec = "-+=/*<>%&| ()@!";
    string d_spec = "<>=!";

    for (char ch : text) {
        if (spec.find(ch) != std::string::npos) {
            if (is_double && (d_spec.find(ch) != std::string::npos)) {
                tokens.back() += ch;
            }
            else {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
                if (ch != ' ') {
                    tokens.push_back(string(1, ch));
                    is_double = true;
                }
            }
        } else {
            is_double = false;
            token += ch;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}
