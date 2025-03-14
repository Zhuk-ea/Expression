#include <iostream>
#include "Expression.h"
#include "Tokenizer.h"


using namespace std;

int main() {
    string s1 = "x + y";
    string s2 = "123 + sin(10) * (x + y)";

    Expression<double> a(s2);
    Expression<complex<double>> c(s1);
    cout << a.to_str() << endl;
    

    return 0;
}