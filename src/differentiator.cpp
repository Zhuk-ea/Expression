using namespace std;
#include "../include/Expression.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

/*

Exzample of commands:

./differentiator --eval 'x + y' x=5 y=2
output: 7

./differentiator --c --eval 'x + y' 'x=(5,1)' 'y=(2,0)'
output: (7,1)

./differentiator --diff "10 * x^2" --by x
output: 0.000000 * x^2.000000 + 10.000000 * 2.000000 * x^(2.000000 - 1.000000)


*/

complex<double> get_complex_val(char * str) {
    using namespace complex_literals;
    string s = str;
    char* c = strtok(str, ",");
    string r = c;
    r.erase(0,1);
    double re = stod(r);
    c = strtok(NULL, ",");
    string l = c;
    l.pop_back();
    double im = stod(l);
    complex<double> res = re + im * 1i;
    return res;
}

int main(int argc, char* argv[]) {
    if (argc > 1) { // если передаем аргументы, то argc будет больше 1(в зависимости от кол-ва аргументов)
            int index = 2;
            if (strcmp(argv[1], "--c") != 0) {
                if (argv[1] == "--r") {
                    index++;
                }
                Expression<double> expr(argv[index]);
                if (strcmp(argv[index-1], "--eval") == 0) {
                    map<string, double> context;
                    for (int i = index + 1; i < argc; ++i) {
                        char* str = argv[i];
                        char* name = strtok(str, "=");
                        char* val = strtok(NULL, "=");
                        context.insert({name, stod(val)});
                    }
                    cout << expr.eval(context);
                }
                else if (strcmp(argv[index-1], "--diff") == 0) {
                    string by;
                    if (strcmp(argv[index+1], "--by") != 0) {
                        cout << "Error, no --by << endl" << endl;
                        return 1;
                    }
                    by = argv[index+2];
                    Expression<double> dif = expr.diff(by);
                    cout << dif.to_str() << endl;
                }
                return 0;
            }
            else {
                index = 3;
                Expression<complex<double>> expr(argv[index]);

                if (strcmp(argv[index-1], "--eval") == 0) {
                    map<string, complex<double>> context;
                    for (int i = index + 1; i < argc; ++i) {
                        char* str = argv[i];
                        char* name = strtok(str, "=");
                        char* val = strtok(NULL, "=");
                        context.insert({name, get_complex_val(val)});
                    }
                    cout << expr.eval(context) << endl;
                }
                else if (strcmp(argv[index-1], "--diff") == 0) {
                    string by;
                    if (strcmp(argv[index+1], "--by") != 0) {
                        cout << "Error, no --by << endl" << endl;
                        return 1;
                    }
                    by = argv[index+2];
                    Expression<complex<double>> dif = expr.diff(by);
                    cout << dif.to_str() << endl;
                }

            }

    } 
    else {
        cout << "Error, not arguments" << endl;
    }

    return 0;
}