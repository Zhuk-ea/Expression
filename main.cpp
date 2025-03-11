#include <iostream>
#include "Expression.h"


using namespace std;

int main() {
    cout << "Start\n";
    Expression a(10);
    Expression b(5);
    Expression c = a + b;
    cout << c.to_str() << endl;

    return 0;
}