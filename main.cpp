#include <iostream>
#include "Expression.h"
#include "Tokenizer.h"


using namespace std;

int main() {
    string s1 = "x + y";
    string s2 = "123i * xi + sin(x + y) * y";

    Expression<double> a(s1);
    Expression<complex<double>> c(s1);
    cout << c.to_str() << endl;
    cout << c.eval({{"x", 2.3}, {"y", 2i}}) << endl;
    

    return 0;
}