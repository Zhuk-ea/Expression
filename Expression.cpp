#include "Expression.h"

using namespace std;

Variable::Variable(string n) {
    name = n;
    order = 0;
}

Value_t Variable::eval(map<string, Value_t> context) const {
    return context[name];
}

string Variable::to_str() const {
    return name;
}

Value::Value(Value_t val) {
    value = val;
    order = 0;
}

Value_t Value::eval(map<string, Value_t> context) const {
    return Value_t(value);
}

string Value::to_str() const {
    return to_string(value);
}
