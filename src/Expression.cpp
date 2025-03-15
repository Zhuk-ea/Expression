#include "Expression.h"



string to_string(const complex<double> &value) {
    string s = "";
    if (value.real() != 0 || value.imag() == 0) {
        s += to_string(value.real());
    } 
    double a = value.imag();
    if (a > 0) {
        if (value.real() != 0) {
            s += " + ";
        }
        s +=  to_string(value.imag()) + "i";
    }
    else if (a < 0) {
        if (value.real() != 0) {
            s += " - ";
        }
        s += to_string(abs(value.imag())) + "i";
    }
    return s;
}



double var_eval(map<string, double> context, const string &name) {
    return context[name];
}
complex<double> var_eval(map<string, complex<double>> &context, const string &name) {
    if (name.back() == 'i') {
        string s = name;
        s.pop_back();
        return context[s]*1i;
    }
    return context[name];
}


double var_diff(double type, const string &name, string &context) {
    if (name == context)
        return 1;
    return 0;
}

complex<double> var_diff(complex<double> type, const string &name, string &context) {
    string s = name;
    if (s.back() == 'i') {
        s.pop_back();
    }
    if (s == context) {
        return 1i;
    }
    return 0;
}


shared_ptr<Value<double>> create_value(double type, vector<string> &vec, int &index) { 
    double t = stod(vec[index]);
    shared_ptr<Value<double>> res = make_shared<Value<double>>(t);
    ++index;
    return res;
}
shared_ptr<Value<complex<double>>> create_value(complex<double> type, vector<string> &vec, int &index) { 
    using namespace complex_literals;
    complex<double> c;
    if (vec[index].back() != 'i') {
        double t = stod(vec[index]);
        c = t + 0i;
    }
    else {
        string s = vec[index];
        s.pop_back(); 
        double t = stod(vec[index]);
        c = t*1i;
    }
    shared_ptr<Value<complex<double>>> res = make_shared<Value<complex<double>>>(c);
    ++index;
    return res;
}