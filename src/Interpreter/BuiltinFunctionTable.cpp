#include <any>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <cmath>

#include "Datatype.h"

std::map<std::string, std::function<std::any(std::vector<std::any>)>> builtinFunctionTable = {
    {"length", [](std::vector<std::any> values)->std::any {
        if (values.size() == 1 && isArray(values[0]))
        return static_cast<double>(toArray(values[0])->values.size());
        if (values.size() == 1 && isMap(values[0]))
        return static_cast<double>(toMap(values[0])->values.size());
        return 0.0;
    }},
    {"push", [](std::vector<std::any> values)->std::any {
        if (values.size() == 2 && isArray(values[0])) {
        toArray(values[0])->values.push_back(values[1]);
        return values[0];
        }
        return nullptr;
    }},
    {"pop", [](std::vector<std::any> values)->std::any {
        if (values.size() == 1 && isArray(values[0]) && toArray(values[0])->values.size() != 0) {
        auto result = toArray(values[0])->values.back();
        toArray(values[0])->values.pop_back();
        return result;
        }
        return nullptr;
    }},
    {"erase", [](std::vector<std::any> values)->std::any {
        if (values.size() == 2 && isMap(values[0]) && isString(values[1]) &&
            toMap(values[0])->values.count(toString(values[1]))) {
        auto result = toMap(values[0])->values.at(toString(values[1]));
        toMap(values[0])->values.erase(toString(values[1]));
        return result;
        }
        return nullptr;
    }},
    {"clock", [](std::vector<std::any> values)->std::any {
        (void) values;
        return static_cast<double>(clock());
    }},
    {"sqrt", [](std::vector<std::any> values)->std::any {
        return sqrt(toNumber(values[0]));
    }},
};