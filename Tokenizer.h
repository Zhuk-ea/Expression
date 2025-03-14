#ifndef TOKENIZER_H_
#define TOKENIZER_H_
 
#include <string>
#include <vector>

using namespace std;

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
        else {
            s += c;
        }
    }
    res.push_back(s);
    return res;
}



#endif