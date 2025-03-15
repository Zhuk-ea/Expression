#include "../include/Tests.h"

void plus_double_test() {
     // given
     Expression<double> a(5);
     Expression<double> b(2);
     Expression<double> correct_anw(7);
 
     // when 
     Expression<double> res = a + b;
     // then
 
     if (res.to_str() == correct_anw.to_str()) {
         cout << "OK\n";
     }
     else {
         cout << "FAIL\n";
     }
}

void plus_complex_test() {

    // given
    Expression<complex<double>> a(1.0 + 2i);
    Expression<complex<double>> b(2.0 + 1i);
    Expression<complex<double>> correct_anw(3.0 + 3i);

    // when 
    Expression<complex<double>> res = a + b;
    // then

    if (res.to_str() == correct_anw.to_str()) {
		cout << "OK\n";
	}
	else {
		cout << "FAIL\n";
	}
}

void plus_variable_and_create_form_str_test() {
    // given
    Expression<double> a(5);
    Expression<double> b("x");
    Expression<double> correct_anw("5 + x");

    // when 
    Expression<double> res = a + b;
    // then

    if (res.to_str() == correct_anw.to_str()) {
        cout << "OK\n";
    }
    else {
        cout << "FAIL\n";
    }
}

void eval_double_test() {
    // given
    Expression<double> a("x + 5");
    double correct_anw = 7;
    // when 
    double res = a.eval({{"x", 2}});
    // then

    if (res == correct_anw) {
        cout << "OK\n";
    }
    else {
        cout << "FAIL\n";
    }
}

void eval_complex_test() {
    // given
    Expression<complex<double>> a("x + 5");
    complex<double> correct_anw = {7.0 + 1i};
    // when 
    complex<double> res = a.eval({{"x", {2.0 + 1i}}});
    // then

    if (res == correct_anw) {
        cout << "OK\n";
    }
    else {
        cout << "FAIL\n";
    }
}

void diff_double_test() {
    // given
    Expression<double> a("5 + x");
    Expression<double> correct_anw("0 + 1");

    // when 
    Expression<double> res = a.diff("x");
    // then

    if (res.to_str() == correct_anw.to_str()) {
        cout << "OK\n";
    }
    else {
        cout << "FAIL\n";
    }
}


void diff_complex_test() {
    // given
    Expression<complex<double>> a("5 + xi");
    string correct_anw = "0.000000 + 1.000000i";

    // when 
    Expression<complex<double>>  res = a.diff("x");
    // then

    if (res.to_str() == correct_anw) {
        cout << "OK\n";
    }
    else {
        cout << "FAIL\n";
    }
}
