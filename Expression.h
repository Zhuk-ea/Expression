#ifndef EXPRESSION_H_
#define EXPRESSION_H_
 
#include <string>
#include <map>
#include <memory>
#include <complex>
#include <cmath>
#include <stdlib.h>
#include "Tokenizer.h"


using namespace std;


typedef long double Value_t;


enum FuncTypes {
    ADD,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POW,
    LN,
    EXP,
    SIN,
    COS,
    BRACKETS
};

template <typename T>
class ExpressionInterface {
    public:
        shared_ptr<ExpressionInterface<T>> parent = NULL;
        shared_ptr<ExpressionInterface<T>> left = NULL;
        shared_ptr<ExpressionInterface<T>> right = NULL;
        int order = 0;

        ExpressionInterface() = default;
        virtual ~ExpressionInterface() = default;
        virtual T eval(std::map<string, T> context) const = 0;
        virtual string to_str() const = 0;

        

};

string to_string(const complex<double> &value) {
    string s = "";
    if (value.real() != 0) {
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

template <typename T>
class Value: public ExpressionInterface<T> {
    public:
        T value;
        Value(T val) {
            value = val;
        }

        virtual ~Value() override = default;

        virtual T eval(map<string, T> context) const override {
            return value;
        }
        T eval() const {
            return value;
        }
        virtual string to_str() const override {
            return to_string(value);
        }
};


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


template <typename T>
class Variable: public ExpressionInterface<T> {
    public:
        string name;
        Variable(string n) {
            name = n;
        }

        virtual ~Variable() override = default;

        virtual T eval(map<string, T> context) const override {
            return var_eval(context, name);
        }
        virtual string to_str() const override {
            return name;
        }
};

template <typename T>
class Binary: public ExpressionInterface<T> {
    public:
        FuncTypes func_type;
        Binary(FuncTypes func) {
            func_type = func;
            if (func == ADD || func == MINUS) {
                this->order = 1;
                return;
            }
            if (func == POW) {
                this->order = 3;
                return;
            }
            this->order = 2;
        }
        Binary(FuncTypes func,shared_ptr<ExpressionInterface<T>> &l) {
            func_type = func;
            func_type = func;
            if (func == ADD || func == MINUS) {
                this->order = 1;
                return;
            }
            if (func == POW) {
                this->order = 3;
                return;
            }
            this->order = 2;
            this->left = l;
        }
        Binary(FuncTypes func, shared_ptr<ExpressionInterface<T>> &l, shared_ptr<ExpressionInterface<T>> &r ) {
            func_type = func;
            if (func == ADD || func == MINUS) {
                this->order = 1;
                return;
            }
            if (func == POW) {
                this->order = 3;
                return;
            }
            this->order = 2;
            this->left = l;
            this->right = r;
        }
        Binary(FuncTypes func, const shared_ptr<ExpressionInterface<T>> &l, const shared_ptr<ExpressionInterface<T>> &r ) {
            func_type = func;
            if (func == ADD || func == MINUS) {
                this->order = 1;
                return;
            }
            if (func == POW) {
                this->order = 3;
                return;
            }
            this->order = 2;
            this->left = l;
            this->right = r;
        }
    
        virtual ~Binary() override = default;
    
        virtual T eval(map<string, T> context) const override {
            switch (func_type) {
                case ADD:
                    return this->left->eval(context) + this->right ->eval(context);
                case MINUS:
                    return this->left->eval(context) - this->right ->eval(context);
                case MULTIPLY:
                    return this->left->eval(context) * this->right ->eval(context);
                case DIVIDE:
                    return this->left->eval(context) / this->right ->eval(context);
                case POW:
                    return pow(this->left->eval(context), this->right ->eval(context));
                }
            return 0;
        }
        virtual string to_str() const override {
            string s = "";
            if (this->left->order < this->order && this->left->order != 0) {
                s += "(" + this->left->to_str() + ")";
            }
            else {
                s += this->left->to_str();
            }

            switch (func_type) {
                case ADD:
                    s += " + ";
                    break;
                case MINUS:
                    s+= " - ";
                    break;
                case MULTIPLY:
                    s += " * ";
                    break;
                case DIVIDE:
                    s += " / ";
                    break;
                case POW: {
                    string s = "";
                    if (this->left->order == 0) {
                        s += this->left->to_str() + "^";
                    }
                    else {
                        s += "(" + this->left->to_str() + ")^";
                    }
                    if (this->right ->order == 0) {
                        s += this->right->to_str();
                    }
                    else {
                        s += "(" + this->right ->to_str() + ")";
                    }
                    return s;
                    break;
                    }
                case BRACKETS:
                    return "(" + this->right->to_str() + ")";
                }

                if ((this->right->order < this->order) && this->right->order != 0) {
                    s += "(" + this->right->to_str() + ")";
                }
                else {
                    s += this->right->to_str();
                }
                return s;
        }
    };

template <typename T>
class Mono: public ExpressionInterface<T> {
    public:
        Mono(FuncTypes func) {
            func_type = func;
        }
        Mono(FuncTypes func, shared_ptr<ExpressionInterface<T>> &r) {
            func_type = func;
            this->right = r;
        }
        Mono(FuncTypes func, const shared_ptr<ExpressionInterface<T>> &r) {
            func_type = func;
            this->right = r;
        }

        virtual ~Mono() override = default;
        virtual T eval(map<string, T> context) const override {
            switch (func_type)
            {
            case LN:
                return log(this->right ->eval(context));
            case EXP:
                return exp(this->right ->eval(context));
            case SIN:
                return sin(this->right ->eval(context));
            case COS:
                return cos(this->right ->eval(context));
            case BRACKETS:
                return this->right ->eval(context);
            }
            return Value_t();
        }
        virtual string to_str() const override {
            switch (func_type)
            {
            case LN:
                return "ln(" + this->right->to_str() + ")";
            case EXP:
                return "exp(" + this->right->to_str() + ")";
            case SIN:
                return "sin(" + this->right->to_str() + ")";
            case COS:
                return "cos(" + this->right->to_str() + ")";
            case BRACKETS: 
                if (this->right == NULL) {
                    cout << "ERRRROR!";
                    return "";
                }
                return "(" + this->right->to_str() + ")";
            }
            return "";
        }

        FuncTypes func_type;
        int order;
    };

template <typename T>
class Expression {
    public:
        Expression<T>(string &str);
        Expression(double num) {
            shared_ptr<Value<double>> t(new Value<double>(num));
            Expr = t;
        }
        Expression(complex<double> num) {
            shared_ptr<Value<complex<double>>> t(new Value<complex<double>>(num));
            Expr = t;
        }
        Expression(const shared_ptr<ExpressionInterface<T>> &p) {
            Expr = p;
        }

        Expression<T> operator=(const Expression<T>& b) {
            return Expression(b->Expr);
        }
        
        Expression<T>  operator+ (const Expression<T>& b) const {
            if (typeid(*Expr) == typeid(Value<T>) && typeid(*b.Expr) == typeid(Value<T>)) {
                shared_ptr<Value<T>> p (new Value(Expr->eval({}) + b.Expr->eval({})));
                return Expression<T>(p);
            }
            shared_ptr<ExpressionInterface<T>> t(new Binary(ADD, Expr, b.Expr));
            Expression res(t);
            return res;
        }

        Expression& operator+=(const Expression& b) {
            return this + b;
        }

        Expression<T>  operator* (const Expression<T>& b) const {
            if (typeid(*Expr) == typeid(Value<T>) && typeid(*b.Expr) == typeid(Value<T>)) {
                shared_ptr<Value<T>> p (new Value(Expr->eval({}) * b.Expr->eval({})));
                return Expression<T>(p);
            }
            shared_ptr<ExpressionInterface<T>> t(new Binary(MULTIPLY, Expr, b.Expr));
            Expression res(t);
            return res;
        }

        Expression& operator*=(const Expression& b) {
            return this * b;
        }

        Expression<T>  operator-(const Expression<T>& b) const {
            if (typeid(*Expr) == typeid(Value<T>) && typeid(*b.Expr) == typeid(Value<T>)) {
                shared_ptr<Value<T>> p (new Value(Expr->eval({}) - b.Expr->eval({})));
                return Expression<T>(p);
            }
            shared_ptr<ExpressionInterface<T>> t(new Binary(MINUS,  Expr, b.Expr));
            Expression res(t);
            return res;
        }
        Expression& operator-=(const Expression& b) {
            return this - b;
        }

        Expression<T>  operator/ (const Expression<T>& b) const {
            if (typeid(*Expr) == typeid(Value<T>) && typeid(*b.Expr) == typeid(Value<T>)) {
                try {
                    if (b.Expr->eval({})) {
                        shared_ptr<Value<T>> p (new Value(Expr->eval({}) / b.Expr->eval({})));
                        return Expression<T>(p);
                        }
                    throw "Division by zero!";
                }
                catch (...) {
                    cout << "Division by zero!" << endl;
                }
            }

            if (typeid(*b.Expr) == typeid(Value<T>)) {
                try {
                    if (b.Expr->eval({})) {
                        shared_ptr<ExpressionInterface<T>> t(new Binary(DIVIDE,  Expr, b.Expr));
                        Expression res(t);
                        return res;
                    }
                    throw "Division by zero!";
                }
                catch (...) {
                    cout << "Division by zero!" << endl;
                }
            }
        }
        Expression& operator/=(const Expression& b) {
            return this / b;
        }

        T eval(map<string, T> context) const {
            return Expr->eval(context);
        }
        string to_str() const {
            return  Expr->to_str();
        }
        //shared_ptr<Expression<T>> diff(string context) const;
        
    private:
        shared_ptr<ExpressionInterface<T>> Expr;
};

template <typename T>
shared_ptr<ExpressionInterface<T>> pars(T type, vector<string> &vec, int &index);


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

template <typename T>
shared_ptr<Variable<T>> create_variable(T type, vector<string> &vec, int &index) {
    shared_ptr<Variable<T>> res = make_shared<Variable<T>>(vec[index]);
    ++index;
    return res;
}


template <typename T>
shared_ptr<Mono<T>> create_mono(T type, vector<string> &vec, int &index) { 
    FuncTypes func;
    switch (vec[index][0]){
        case 's':
            func = SIN;
            break;
        case '(':
            func = BRACKETS;
            break;
        case 'c':
            func = COS;
            break;
        case 'l':
            func = LN;
            break;
        case 'e':
            func = EXP;
            break;
    }
    if (func != BRACKETS) {
        ++index;
    }
    ++index;
    shared_ptr<Mono<T>> res(new Mono<T>(func, pars(type, vec, index)));
    return res;
}

template <typename T>
shared_ptr<ExpressionInterface<T>> pars(T type, vector<string> &vec, int &index) {
    shared_ptr<ExpressionInterface<T>> root(new Mono<T>(BRACKETS));
    shared_ptr<ExpressionInterface<T>> now = root;
    auto siz = vec.size();
    //cout << "Start pars [ " << vec[index]<< " ]-----------------\n";
    if ('0' <= vec[index][0] && vec[index][0] <= '9') { // Получаем константу
        //cout << "Get const\n";
        shared_ptr<Value<T>> t = create_value(type, vec, index);
        root->right = t;
        if (index == (siz-1)) { // Последний элемент выражения
            //cout << "End pars -----------------\n";
            return root->right;
        }
    }
    else if (('a' <= vec[index][0] && vec[index][0] <= 'z') || ('A' <= vec[index][0] && vec[index][0] <= 'Z')){ // Получаем переменную или функцию
        //cout << "Get var or func\n";
        if (vec.size() > (index+1)) { // Не последний операнд
            if (vec[index+1][0] != '(') { // После не идут скобки
                shared_ptr<ExpressionInterface<T>> t = create_variable(type, vec, index);
                root->right = t;
            }
            else { // После идут скобки, то есть это функция
                shared_ptr<ExpressionInterface<T>> t = create_mono(type, vec, index);
                root->right = t;
            }
        }
        else { // Последний операнд, значит после не может быть скобок и это переменная
            shared_ptr<ExpressionInterface<T>> t = create_variable(type, vec, index);
            root->right = t;
            //cout << "End pars -----------------\n";
            return root->right;
        }
    }
    else if (vec[index][0] == '(') { // Получаем скобку
        //cout << "Get open bracket\n";
        shared_ptr<ExpressionInterface<T>> t = create_mono(type, vec, index);
        root->right = t;
    }
    else {
        cout << "ERRROR!\n";
    }
    if (index == siz) { 
        //cout << "End pars -----------------\n";
        return root->right;
    }

    //cout <<"| " << index << endl;
    if (vec[index][0] == ')') { // Закрываем скобку
        //cout << "Get close bracket\n";
        ++index;
        //cout << "End pars -----------------\n";
        return root->right;
    }

    //cout << vec[index] << endl;
    FuncTypes func;
    switch (vec[index][0]) {
        case '+':
            func = ADD;
            break;
        case '-':
            func = MINUS;
            break;
        case '*':
            func = MULTIPLY;
            break;
        case '/':
            func = DIVIDE;
            break;
        case '^':
            func = POW;
            break;
    }
    shared_ptr<Binary<T>> b(new Binary<T>(func));
    b->left = root->right;
    root->right = b;
    ++index; 
    
    shared_ptr<ExpressionInterface<T>> temp = pars(type, vec, index);
    if (typeid(*temp) != typeid(Binary<T>) || (temp->order >= b->order)) {
        b->right = temp;
        //cout << "End pars(0) -----------------\n";
        return root->right;
    }
    
    shared_ptr<ExpressionInterface<T>> ttemp = temp->left;
    root->right = temp;
    b->right = ttemp;
    temp->left = b;
    //cout << "End pars(1) -----------------\n";
    return root->right;
}

template <typename T>
inline Expression<T>::Expression(string &str) {
    T type;
    int index = 0;
    vector<string> vec = tokenize(str);
    /*
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << endl;
    }
    cout << "-----\n";
    */
    this->Expr = pars(type, vec, index);
}


#endif