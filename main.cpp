#include <iostream>
#include "Expression.h"
#include "Tokenizer.h"


using namespace std;

int main() {
    string s = "123 + sin(x) * y";
    Expression<double> a(s);
    

    return 0;
}