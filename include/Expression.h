#ifndef EXPRESSION_H_
#define EXPRESSION_H_
 
#include <string>
#include <map>
#include <memory>
#include <complex>
#include <cmath>
#include <stdlib.h>
#include <iostream>
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
        shared_ptr<ExpressionInterface<T>> left = NULL;
        shared_ptr<ExpressionInterface<T>> right = NULL;
        int order = 0;
        FuncTypes func_type;

        ExpressionInterface() = default;
        virtual ~ExpressionInterface() = default;
        virtual T eval(std::map<string, T> context) const = 0;
        virtual string to_str() const = 0;
        virtual shared_ptr<ExpressionInterface<T>> diff(string & context) const = 0;

        

};

string to_string(const complex<double> &value);

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
        virtual shared_ptr<ExpressionInterface<T>> diff(string & context) const {
            T val = 0;
            return shared_ptr<ExpressionInterface<T>>(new Value<T>(val));
        }
        
};


double var_eval(map<string, double> context, const string &name);
complex<double> var_eval(map<string, complex<double>> &context, const string &name);


double var_diff(double type, const string &name, string &context);

complex<double> var_diff(complex<double> type, const string &name, string &context);

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
        virtual shared_ptr<ExpressionInterface<T>> diff(string & context) const {
            T type;
            T val = var_diff(type, name, context);
            return shared_ptr<Value<T>>(new Value<T>(val));
        }
};


template <typename T>
class Binary: public ExpressionInterface<T> {
    public:
        Binary(FuncTypes func) {
            this->func_type = func;
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
            this->func_type = func;
            this->func_type = func;
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
            this->func_type = func;
            if (func == ADD || func == MINUS) {
                this->order = 1;
                this->left = l;
                this->right = r;
                return;
            }
            if (func == POW) {
                this->order = 3;
                this->left = l;
                this->right = r;
                return;
            }
            this->order = 2;
            this->left = l;
            this->right = r;
        }
        Binary(FuncTypes func, const shared_ptr<ExpressionInterface<T>> &l, const shared_ptr<ExpressionInterface<T>> &r ) {
            this->func_type = func;
            if (func == ADD || func == MINUS) {
                this->order = 1;
                this->left = l;
                this->right = r;
                return;
            }
            if (func == POW) {
                this->order = 3;
                this->left = l;
                this->right = r;
                return;
            }
            this->order = 2;
            this->left = l;
            this->right = r;
        }
    
        virtual ~Binary() override = default;
    
        virtual T eval(map<string, T> context) const override {
            switch (this->func_type) {
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

        virtual shared_ptr<ExpressionInterface<T>> diff(string & context) const;

        virtual string to_str() const override {
            string s = "";
            if (this->left->order < this->order && this->left->order != 0) {
                s += "(" + this->left->to_str() + ")";
            }
            else {
                s += this->left->to_str();
            }

            switch (this->func_type) {
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
                return log(this->right->eval(context));
            case EXP:
                return exp(this->right->eval(context));
            case SIN:
                return sin(this->right->eval(context));
            case COS:
                return cos(this->right ->eval(context));
            case BRACKETS:
                return this->right->eval(context);
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
        virtual shared_ptr<ExpressionInterface<T>> diff(string & context) const;

        FuncTypes func_type;
        int order;
    };

template <typename T>
shared_ptr<ExpressionInterface<T>> optimize(shared_ptr<ExpressionInterface<T>> expr) {
    if (typeid(*expr) == typeid(Mono<T>)) {
        expr->left = optimize(expr->left);
    }
    else if (typeid(*expr) == typeid(Binary<T>)) {
        expr->left = optimize(expr->left);
        expr->right = optimize(expr->right);
        switch (expr->func_type) {
            case ADD:    
                if (typeid(*expr->left) == typeid(Value<T>) && expr->left->eval({}) == (T)0) {
                    return expr->right;
                }
                if (typeid(*expr->right) == typeid(Value<T>) && expr->right->eval({}) == (T)0) {
                    return expr->left;
                }
            case MINUS:
                if (typeid(*expr->right) == typeid(Value<T>) && expr->right->eval({}) == (T)0) {
                    return expr->left;
                }
            case MULTIPLY:
                if ((typeid(*expr->left) == typeid(Value<T>) && expr->left->eval({}) == (T)0) || typeid(*expr->right) == typeid(Value<T>) && expr->right->eval({}) == (T)0) {
                    return shared_ptr<Value<T>>(new Value<T>((T)0));
                }
            case DIVIDE:
                if (typeid(*expr->right) == typeid(Value<T>) && expr->right->eval({}) == (T)1) {
                    return expr->left;
                }
            case POW:
                if (typeid(*expr->right) == typeid(Value<T>) && expr->right->eval({}) == (T)0) {
                    return shared_ptr<Value<T>>(new Value<T>((T)1));
                }
            }
    }
    return expr;
}

template <typename T>
class Expression {
    public:
        Expression<T>(string &str);
        Expression<T>(const char * str);
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
        Expression(shared_ptr<ExpressionInterface<T>> &p) {
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
        Expression diff(string& context) {
            shared_ptr<ExpressionInterface<T>> p = optimize(Expr->diff(context));
            return Expression(p);
        }
        Expression diff(const char* context) {
            string s = context;
            shared_ptr<ExpressionInterface<T>> p = optimize(Expr->diff(s));
            return Expression(p);
        }
        Expression diff(char* context) {
            string s = context;
            shared_ptr<ExpressionInterface<T>> p = optimize(Expr->diff(s));
            return Expression(p);
        }

        
    private:
        shared_ptr<ExpressionInterface<T>> Expr;
};

template <typename T>
shared_ptr<ExpressionInterface<T>> pars(T type, vector<string> &vec, int &index);


shared_ptr<Value<double>> create_value(double type, vector<string> &vec, int &index);
shared_ptr<Value<complex<double>>> create_value(complex<double> type, vector<string> &vec, int &index);

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
    if ('0' <= vec[index][0] && vec[index][0] <= '9') {
        shared_ptr<Value<T>> t = create_value(type, vec, index);
        root->right = t;
        if (index == (siz-1)) {
            return root->right;
        }
    }
    else if (('a' <= vec[index][0] && vec[index][0] <= 'z') || ('A' <= vec[index][0] && vec[index][0] <= 'Z')){ 
        if (vec.size() > (index+1)) {
            if (vec[index+1][0] != '(') { 
                shared_ptr<ExpressionInterface<T>> t = create_variable(type, vec, index);
                root->right = t;
            }
            else { 
                shared_ptr<ExpressionInterface<T>> t = create_mono(type, vec, index);
                root->right = t;
            }
        }
        else { 
            shared_ptr<ExpressionInterface<T>> t = create_variable(type, vec, index);
            root->right = t;
            return root->right;
        }
    }
    else if (vec[index][0] == '(') { 
        shared_ptr<ExpressionInterface<T>> t = create_mono(type, vec, index);
        root->right = t;
    }
    else {
        cout << "ERRROR!\n";
    }
    if (index == siz) { 
        return root->right;
    }


    if (vec[index][0] == ')') { 
        ++index;
        return root->right;
    }

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
        return root->right;
    }
    
    shared_ptr<ExpressionInterface<T>> ttemp = temp->left;
    root->right = temp;
    b->right = ttemp;
    temp->left = b;
    return root->right;
}

template <typename T>
inline Expression<T>::Expression(string &str) {
    T type;
    int index = 0;
    vector<string> vec = tokenize(str);
    this->Expr = pars(type, vec, index);
}

template <typename T>
inline Expression<T>::Expression(const char *str) {
    T type;
    int index = 0;
    string s = str;
    vector<string> vec = tokenize(s);
    this->Expr = pars(type, vec, index);
}

template <typename T>
inline shared_ptr<ExpressionInterface<T>> Binary<T>::diff(string &context) const {
    
    shared_ptr<ExpressionInterface<T>> l = this->left;
    shared_ptr<ExpressionInterface<T>> r = this->right;
    shared_ptr<ExpressionInterface<T>> l_diff = this->left->diff(context);
    shared_ptr<ExpressionInterface<T>> r_diff = this->right->diff(context);

    switch (this->func_type) {
        case ADD: 
            return shared_ptr<Binary<T>>(new Binary<T>(ADD, l_diff, r_diff));
        case MINUS:
            return shared_ptr<Binary<T>>(new Binary<T>(MINUS, l_diff, r_diff));
        case MULTIPLY:
            return shared_ptr<Binary<T>>(new Binary<T>(ADD,
                shared_ptr<Binary<T>>(new Binary<T>(MULTIPLY, l_diff, r)),
                shared_ptr<Binary<T>>(new Binary<T>(MULTIPLY, l, r_diff))
            ));
        case DIVIDE:
            return shared_ptr<Binary<T>>(new Binary<T>(DIVIDE,
                shared_ptr<Binary<T>>(new Binary(MINUS,
                    shared_ptr<Binary<T>>(new Binary(MULTIPLY, l_diff, r)),
                    shared_ptr<Binary<T>>(new Binary(MULTIPLY, l, r_diff))
                )),
                shared_ptr<Binary<T>>(new Binary<T>(POW, r, shared_ptr<Value<T>>(new Value<T>(2))))
            ));

        case POW:
            if (typeid(*r) == typeid(Value<T>)) {
                return shared_ptr<Binary<T>>(new Binary<T>(MULTIPLY,
                    r, shared_ptr<Binary<T>>(new Binary<T>(POW, l, 
                        shared_ptr<Binary<T>>(new Binary<T>(MINUS, 
                            r, shared_ptr<Value<T>>(new Value<T>(1))
                        ))
                    ))
                ));
            }
    
            shared_ptr<Binary<T>> p (new Binary<T>(MULTIPLY, r, shared_ptr<Mono<T>>(new Mono<T>(LN, l))));
            return shared_ptr<Binary<T>>(new Binary<T>(MULTIPLY, 
                shared_ptr<Binary<T>>(new Binary<T>(POW, r, l)), p->diff(context)
            ));
        }
   return shared_ptr<ExpressionInterface<T>>();
}


template <typename T>
inline shared_ptr<ExpressionInterface<T>> Mono<T>::diff(string &context) const {
    shared_ptr<ExpressionInterface<T>> r = this->right;
    shared_ptr<ExpressionInterface<T>> r_diff = this->right->diff(context);
    switch (func_type) {
        case LN:
            return shared_ptr<Binary<T>>(new Binary<T>(MULTIPLY, 
                r_diff,
                shared_ptr<Binary<T>>(new Binary<T>(DIVIDE, shared_ptr<Value<T>>(new Value<T>(1)), r))
            ));
        case EXP:
            return shared_ptr<Binary<T>>(new Binary<T>(MULTIPLY, 
                shared_ptr<Mono<T>>(new Mono<T>(func_type, r)), r_diff));
        case SIN:
            return shared_ptr<Binary<T>>(new Binary<T>(MULTIPLY, 
                r_diff, shared_ptr<Mono<T>>(new Mono<T>(COS, r))));
        case COS:
            return shared_ptr<Binary<T>>(new Binary<T>(MULTIPLY, 
                r_diff, shared_ptr<Binary<T>>(new Binary<T>(MULTIPLY, 
                    shared_ptr<Value<T>>(new Value<T>(-1)), shared_ptr<Mono<T>>(new Mono<T>(SIN, r))))
                ));
        case BRACKETS: 
            return r_diff;
        }
    return shared_ptr<ExpressionInterface<T>>();

}

#endif