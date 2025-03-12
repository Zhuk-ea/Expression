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
        shared_ptr<ExpressionInterface<T>> right = NULL;
        int order = 0;

        ExpressionInterface() = default;
        virtual ~ExpressionInterface() = default;
        virtual T eval(std::map<string, T> context) const = 0;
        virtual string to_str() const = 0;

        

};

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

template <typename T>
class Variable: public ExpressionInterface<T> {
    public:
        string name;
        Variable(string n);

        virtual ~Variable() override = default;

        virtual T eval(map<string, T> context) const override {
            return context[name];
        }
        virtual string to_str() const override {
            return name;
        }
};

template <typename T>
class Binary: public ExpressionInterface<T> {
    public:
        shared_ptr<ExpressionInterface<T>> left;
        Binary(FuncTypes func) {
            func_type = func;
            if (func == ADD || func == MINUS) {
                order = 1;
                return;
            }
            if (func == POW) {
                order = 3;
                return;
            }
            order = 2;
        }
        Binary(FuncTypes func,shared_ptr<ExpressionInterface<T>> &l) {
            func_type = func;
            func_type = func;
            if (func == ADD || func == MINUS) {
                order = 1;
                return;
            }
            if (func == POW) {
                order = 3;
                return;
            }
            order = 2;
            this->left = l;
        }
        Binary(FuncTypes func, shared_ptr<ExpressionInterface<T>> &l, shared_ptr<ExpressionInterface<T>> &r ) {
            func_type = func;
            if (func == ADD || func == MINUS) {
                order = 1;
                return;
            }
            if (func == POW) {
                order = 3;
                return;
            }
            order = 2;
            this->left = l;
            this->right = r;
        }
        Binary(FuncTypes func, const shared_ptr<ExpressionInterface<T>> &l, const shared_ptr<ExpressionInterface<T>> &r ) {
            func_type = func;
            if (func == ADD || func == MINUS) {
                order = 1;
                return;
            }
            if (func == POW) {
                order = 3;
                return;
            }
            order = 2;
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
            if (this->parent != NULL && this->parent->order == 1) { 
                s += "(";
            }
            s += this->left->to_str();

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
                case POW:
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
                case BRACKETS:
                    if (this->parent == NULL) {
                        return this->right->to_str();
                    }
                    else {
                        return "(" + this->right->to_str() + ")";
                    }
                }
                s += this->right->to_str();
                if (this->parent != NULL && this->parent->order == 1) { 
                    s += ")";
                }
                return s;
        }

        FuncTypes func_type;
        int order;
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
                return "ln(" + this->right ->to_str() + ")";
            case EXP:
                return "exp(" + this->right ->to_str() + ")";
            case SIN:
                return "sin(" + this->right ->to_str() + ")";
            case COS:
                return "cos(" + this->right ->to_str() + ")";
            case BRACKETS: 
                return "(" + this->right ->to_str() + ")";
            }
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
            string str = b.to_str();
            return Expression(str);
        }
        
        Expression<T>  operator+ (const Expression<T>& b) const {
            if (typeid(*Expr) == typeid(Value<T>) && typeid(*b.Expr) == typeid(Value<T>)) {
                shared_ptr<Value<T>> p (new Value(Expr->eval({}) + b.Expr->eval({})));
                return Expression<T>(p);
            }
            shared_ptr<ExpressionInterface<T>> t(new Binary(ADD, Expr, b.Expr));
            Expression res(t);
            string str = res.to_str();
            return Expression(str);
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
            string str = res.to_str();
            return Expression(str);
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
            string str = res.to_str();
            return Expression(str);
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
                        string str = res.to_str();
                        return Expression(str);
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
        shared_ptr<Expression<T>> diff(string context) const;
        
    private:
        shared_ptr<ExpressionInterface<T>> Expr;
};

template <typename T>
shared_ptr<Value<T>> create_value(T type, vector<string> &vec, int &index) { 
    if (vec[index][vec[index].size()-1] != 'i') {
        double t = stod(vec[0]);
        shared_ptr<Value<double>> res = make_shared<Value<double>>(t);
        ++index;
        return res;
    }
    else {
        string t = vec[index];
        t.pop_back();
        complex<double> c = stod(t)*1i;
        shared_ptr<Value<complex<double>>> res = make_shared<Value<complex<double>>>(c);
        ++index;
        return res;
    }
}

template <typename T>
shared_ptr<Variable<T>> create_variable(T type, vector<string> &vec, int &index) {
    if (vec[index][vec[index].size()-1] != 'i') {
        shared_ptr<Variable<double>> res = make_shared<Variable<double>>(vec[index]);
        ++index;
        return res;
    }
    else {
        string t = vec[index];
        t.pop_back();
        shared_ptr<Variable<complex<double>>> res = make_shared<Variable<complex<double>>>(t);
        ++index;
        return res;
    }
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
    ++index;
    shared_ptr<Mono<T>> res(new Mono<T>(func, pars(type, vec, index)));
    return res;
}

template <typename T>
shared_ptr<ExpressionInterface<T>> pars(T type, vector<string> &vec, int &index) {
    shared_ptr<ExpressionInterface<T>> root(new Mono<T>(BRACKETS));
    shared_ptr<ExpressionInterface<T>> now = root;
    if ('0' <= vec[index][0] <= '9') { // Получаем константу
        now = create_value(type, vec, index);
    }
    else if ('a' <= vec[index][0] <= 'z' || 'A' <= vec[index][0] <= 'Z'){ // Получаем переменную или функцию
        if (vec.size() > (index+1)) { // Не последний операнд
            if (vec[index+1][0] != '(') { // После не идут скобки
                shared_ptr<ExpressionInterface<T>> t = create_variable(type, vec, index);
                now->right = t;
                t->parent = now;
                now = t;
            }
            else { // После идут скобки, то есть это функция
                shared_ptr<ExpressionInterface<T>> t = create_mono(type, vec, index);
                now->right = t;
                t->parent = now;
                now = t;
            }
        }
        else { // Последний операнд, значит после не может быть скобок и это переменная
            shared_ptr<ExpressionInterface<T>> t = create_variable(type, vec, index);
            now->right = t;
            t->parent = now;
            now = t;
        }
    }
    else if (vec[index][0] == '(') { // Получаем скобку
        shared_ptr<ExpressionInterface<T>> t = create_mono(type, vec, index);
        now->right = t;
        t->parent = now;
        now = t;
    }
    else { // Получаем оператор
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
        while (true) {
            if (now->parent == NULL) {
                now->parent = b;
                b->left = now;
                now = b;
                root = b;
                break;
            }
            else if (now->parent->order <= b->order) {
                now->parent->right = b;
                b->parent = now->parent;
                now->parent = b;
                b->left = now;
                now = b;
                break;
            }
            else {
                now = now->parent;
            }
        }  
    }
    return root;
}


template <typename T>
inline Expression<T>::Expression(string &str) {
    vector<string> vec = tokenize(str);
    T type;
    int index = 0;
    this->Expr = pars(type,vec,index);
}


#endif