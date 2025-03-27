#include "../include/Tests.h"

using namespace std;

int main() {
    cout << "double Expr + Expr: ";
    plus_double_test();
    cout << "complex Expr + Expr: ";
    plus_complex_test();
    cout << "double Expr + variable and Expr from str: ";
    plus_variable_and_create_form_str_test();

    cout << "double eval: ";
    eval_double_test();
    cout << "double eval with unary minus: ";
    eval_double_test2();

    cout << "complex eval: ";
    eval_complex_test();

    cout << "double diff: ";
    diff_double_test();

    cout << "double diff2: ";
    diff_double_test2();

    cout << "double diff long expression: ";
    diff_double_long_expression_test();

    cout << "complex diff: ";
    diff_complex_test();
    

    return 0;
}