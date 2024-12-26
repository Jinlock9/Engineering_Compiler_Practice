#include <iomanip>
#include <iostream>

#include "Datatype.h"

auto isSize(std::any value)->bool {
    return value.type() == typeid(size_t);
}

auto toSize(std::any value)->size_t {
    return std::any_cast<size_t>(value);
}

auto isNull(std::any value)->bool {
    return value.type() == typeid(nullptr_t);
}

auto isTrue(std::any value)->bool {
    return isBoolean(value) && toBoolean(value);
}

auto isFalse(std::any value)->bool {
    return isBoolean(value) && toBoolean(value) == false;
}

auto isBoolean(std::any value)->bool {
    return value.type() == typeid(bool);
}

auto toBoolean(std::any value)->bool {
    return std::any_cast<bool>(value);
}

auto isNumber(std::any value)->bool {
    return value.type() == typeid(double);
}

auto toNumber(std::any value)->double {
    return std::any_cast<double>(value);
}

auto isString(std::any value)->bool {
    return value.type() == typeid(std::string);
}

auto toString(std::any value)->std::string {
    return std::any_cast<std::string>(value);
}

auto isArray(std::any value)->bool {
    return value.type() == typeid(Array *);
}

auto toArray(std::any value)->Array * {
    return std::any_cast<Array *>(value);
}

auto getValueOfArray(std::any object, std::any index)->std::any {
    auto i = static_cast<size_t>(toNumber(index));
    if (i < toArray(object)->values.size())
        return toArray(object)->values[i];
    return nullptr;
}

auto setValueOfArray(std::any object, std::any index, std::any value)->std::any {
    auto i = static_cast<size_t>(toNumber(index));
    if (i < toArray(object)->values.size())
        toArray(object)->values[i] = value;
    return value;
}

auto isMap(std::any value)->bool {
    return value.type() == typeid(Map *);
}

auto toMap(std::any value)->Map* {
    return std::any_cast<Map *>(value);
}

auto getValueOfMap(std::any object, std::any key)->std::any {
    if (toMap(object)->values.count(toString(key)))
        return toMap(object)->values[toString(key)];
    return nullptr;
}

auto setValueOfMap(std::any object, std::any key, std::any value)->std::any {
    toMap(object)->values[toString(key)] = value;
    return value;
}

auto isFunction(std::any value)->bool {
    return value.type() == typeid(Function *);
}

auto toFunction(std::any value)->Function * {
    return std::any_cast<Function *>(value);
}

auto isBuiltinFunction(std::any value)->bool {
    return value.type() == typeid(std::function<std::any(std::vector<std::any>)>);
}

auto toBuiltinFunction(std::any value)->std::function<std::any(std::vector<std::any>)> {
    return std::any_cast<std::function<std::any(std::vector<std::any>)>>(value);
}

auto operator<<(std::ostream& stream, std::any& value)->std::ostream& {
    if (isNull(value)) {
        stream << "null";
    }
    else if (isBoolean(value)) {
        stream << std::boolalpha << std::any_cast<bool>(value);
    }
    else if (isNumber(value)) {
        printf("%g", toNumber(value));
    }
    else if (isString(value)) {
        stream << toString(value);
    }
    else if (isArray(value)) {
        stream << "[ ";
        for (auto& value : toArray(value)->values)
        stream << value << " ";
        stream << "]";
    }
    else if (isMap(value)) {
        stream << "{ ";
        for (auto& [key, value] : toMap(value)->values)
        stream << key << ":" << value << " ";
        stream << "}";
    }
    return stream;
}