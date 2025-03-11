#ifndef EXPRESSION_H_
#define EXPRESSION_H_
 
#include <string>
#include <map>
#include <memory>
#include <complex>
#include <cmath>


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
        int order;

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
            this->order = 0;
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
        shared_ptr<ExpressionInterface<T>> right;
        Binary(FuncTypes func, int ord) {
            func_type = func;
            order = ord;
        }
        Binary(FuncTypes func, int ord, shared_ptr<ExpressionInterface<T>> &l, shared_ptr<ExpressionInterface<T>> &r ) {
            func_type = func;
            order = ord;
            left = l;
            right = r;
        }
        Binary(FuncTypes func, int ord, const shared_ptr<ExpressionInterface<T>> &l, const shared_ptr<ExpressionInterface<T>> &r ) {
            func_type = func;
            order = ord;
            left = l;
            right = r;
        }
    
        virtual ~Binary() override = default;
    
        virtual T eval(map<string, T> context) const override {
            switch (func_type) {
                case ADD:
                    return left->eval(context) + right->eval(context);
                case MINUS:
                    return left->eval(context) - right->eval(context);
                case MULTIPLY:
                    return left->eval(context) * right->eval(context);
                case DIVIDE:
                    return left->eval(context) / right->eval(context);
                case POW:
                    return pow(left->eval(context), right->eval(context));
                }
            return 0;
        }
        virtual string to_str() const override {
            string s = "";
            if (this->parent != NULL && this->parent->order == 1) { 
                s += "(";
            }
            s += left->to_str();

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
                    if (left->order == 0) {
                        s += left->to_str() + "^";
                    }
                    else {
                        s += "(" + left->to_str() + ")^";
                    }
                    if (right->order == 0) {
                        s += right->to_str();
                    }
                    else {
                        s += "(" + right->to_str() + ")";
                    }
                    return s;
                }
                s += right->to_str();
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
        shared_ptr<ExpressionInterface<T>> child;
        Mono(FuncTypes func) {
            func_type = func;
        }

        virtual ~Mono() override = default;
        virtual T eval(map<string, T> context) const override {
            switch (func_type)
            {
            case LN:
                return log(child->eval(context));
            case EXP:
                return exp(child->eval(context));
            case SIN:
                return sin(child->eval(context));
            case COS:
                return cos(child->eval(context));
            case BRACKETS:
                return child->eval(context);
            }
            return Value_t();
        }
        virtual string to_str() const override {
            switch (func_type)
            {
            case LN:
                return "ln(" + child->to_str() + ")";
            case EXP:
                return "exp(" + child->to_str() + ")";
            case SIN:
                return "sin(" + child->to_str() + ")";
            case COS:
                return "cos(" + child->to_str() + ")";
            case BRACKETS: 
                return "(" + child->to_str() + ")";
            }
        }

        FuncTypes func_type;
        int order;
    };



template <typename T>
class Expression {
    public:
        //Expression(string str);
        Expression(T num) {
            shared_ptr<Value<T>> t(new Value(num));
            Expr = t;
        }
        Expression(const shared_ptr<ExpressionInterface<T>> &p) {
            Expr = p;
        }
        
        Expression<T>  operator+ (const Expression<T>& b) const {
            if (typeid(*Expr) == typeid(Value<T>) && typeid(*b.Expr) == typeid(Value<T>)) {
                shared_ptr<Value<T>> p (new Value(Expr->eval({}) + b.Expr->eval({})));
                return Expression<T>(p);
            }
            shared_ptr<ExpressionInterface<T>> add(new Binary(ADD, 1, Expr, b.Expr));
            return Expression(add);
        }

        
        Expression<T> operator=(const Expression<T>& b) {
            string str = b.to_str();
            Expression res(str);
            return res;
        }
        /*
        Expression& operator+=(const Expression& b);
        Expression  operator* (const Expression& b);
        Expression& operator*=(const Expression& b);
        */

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
#endif