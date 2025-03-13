#include <iostream>
#include "Expression.h"
#include "Tokenizer.h"


using namespace std;

int main() {
    string s1 = "123 + sin(x) * y";
    string s2 = "123 * x + sin(x + y) * y";

    Expression<double> a(s2);
    cout << a.to_str() << endl;
    

    return 0;
}