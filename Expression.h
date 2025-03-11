#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <string>
#include <map>
#include <memory>
#include <complex>

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
    COS
};

class ExpressionInterface {
    public:
        shared_ptr<ExpressionInterface> parent;
        int order;

        ExpressionInterface() = default;
        virtual ~ExpressionInterface() = default;
        virtual Value_t eval(std::map<string, Value_t> context) const = 0;
        virtual string to_str() const = 0;

        

};

class Expression {
    public:
        Expression(string variable);
        template <typename T>
        Expression(T num);
        
        /*
        Expression  operator+ (const Expression& b);
        Expression& operator+=(const Expression& b);
        Expression  operator* (const Expression& b);
        Expression& operator*=(const Expression& b);
        */

        Value_t eval(map<string, Value_t> context) const; 
        Value_t diff(string context) const;
        
    private:
        ExpressionInterface & Expr;
};

class Value: public ExpressionInterface {
    public:
        Value_t value;
        Value(Value_t val);

        virtual ~Value() override = default;

        virtual Value_t eval(map<string, Value_t> context) const override;
        virtual string to_str() const override;
};

class Variable: public ExpressionInterface {
    public:
        string name;
        Variable(string n);

        virtual ~Variable() override = default;

        virtual Value_t eval(map<string, Value_t> context) const override;
        virtual string to_str() const override;
};


class Binary: public ExpressionInterface {
    public:
        shared_ptr<ExpressionInterface> left;
        shared_ptr<ExpressionInterface> right;
        Binary(string n);
    
        virtual ~Binary() override = default;
    
        virtual Value_t eval(map<string, Value_t> context) const override;
        virtual string to_str() const override;

        int func_type;
        int order;
        string name;
    };


class Mono: public ExpressionInterface {
    public:
        shared_ptr<ExpressionInterface> child;
        Mono(string n);

    
        virtual ~Mono() override = default;
    
        virtual Value_t eval(map<string, Value_t> context) const override;
        virtual string to_str() const override;

        FuncTypes func_type;
        string name;
    };

#endif