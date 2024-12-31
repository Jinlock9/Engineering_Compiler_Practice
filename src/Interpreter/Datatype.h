#ifndef __DATATYPE_H__
#define __DATATYPE_H__

#include <any>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

#include "../Parser/Node.h"

struct Object {
    bool isMarked = false;
    virtual ~Object() {}
};

struct Array: Object {
    std::vector<std::any> values;
};

struct Map: Object {
    std::map<std::string, std::any> values;
};

auto isSize(std::any value)->bool;
auto toSize(std::any value)->size_t;

auto isNull(std::any value)->bool;

auto isTrue(std::any value)->bool;
auto isFalse(std::any value)->bool;
auto isBoolean(std::any value)->bool;
auto toBoolean(std::any value)->bool;

auto isNumber(std::any value)->bool;
auto toNumber(std::any value)->double;

auto isString(std::any value)->bool;
auto toString(std::any value)->std::string;

auto isArray(std::any value)->bool;
auto toArray(std::any value)->Array*;
auto getValueOfArray(std::any object, std::any index)->std::any;
auto setValueOfArray(std::any object, std::any index, std::any value)->std::any;

auto isMap(std::any value)->bool;
auto toMap(std::any value)->Map*;
auto getValueOfMap(std::any object, std::any key)->std::any;
auto setValueOfMap(std::any object, std::any key, std::any value)->std::any;

auto isFunction(std::any)->bool;
auto toFunction(std::any)->Function*;

auto isBuiltinFunction(std::any value)->bool;
auto toBuiltinFunction(std::any value)->std::function<std::any(std::vector<std::any>)>;

auto operator<<(std::ostream& stream, std::any& value)->std::ostream&;

#endif // __DATATYPE_H__