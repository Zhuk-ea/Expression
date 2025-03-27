#include "../include/Tokenizer.h"

// 5 + (-10)
vector<string> tokenize(string &str) {
    vector<string> res;
    string s = "";
    for (int i = 0; str[i] != '\0'; ++i) {
        char c = str[i];
        if (c == ' ') {
            res.push_back(s);
            s = "";
        }
        else if (c == '(') {
            if (s != "") {
                res.push_back(s);
            }
            s = c;
            res.push_back(s);
            s = "";
        }
        else if (c == ')') {
            res.push_back(s);
            s = c;
        }
        else if (c == '^' && s != "") {
            res.push_back(s);
            s = c;
            res.push_back(s);
            s = "";
        }
        else if (c == '-' && (res.size() == 0 || res.back()[0] == '(')) {
            res.push_back("0");
            res.push_back("-");
            s = "";
        }

        else {
            s += c;
        }
    }
    res.push_back(s);
    return res;
}