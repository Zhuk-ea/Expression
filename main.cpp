#include <iostream>
#include "Expression.h"
#include "Tokenizer.h"


using namespace std;

int main() {
    string s1 = "x + y";
    string s2 = "x * sin(x^2)";

    Expression<double> a(s2);
    Expression<double> b = a.diff("x");
    Expression<complex<double>> c(s1);
    cout << a.to_str() << endl;
    cout << b.to_str() << endl;
    

    return 0;
}