#ifndef __Definition
#define __Definition

#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <algorithm>
#include <fstream>
#include "M5Core2.h"
#include <regex>

typedef std::vector<String> vstring;
typedef std::vector<std::vector<String>> v2string;

typedef std::vector<int> vint;
typedef std::vector<float> vfloat;
typedef std::vector<std::vector<int>> v2int;
typedef std::vector<std::vector<std::vector<int>>> v3int;
typedef std::vector<std::vector<std::vector<std::vector<int>>>> v4int;
typedef std::vector<std::vector<String>> SourceCode;

inline bool canConvertToInt(String str)
{
    // 正規表現: 整数を表す形式（オプションで正負符号を含む）
    std::regex intRegex("^[-+]?\\d+$");
    return std::regex_match(str.c_str(), intRegex);
}

inline bool canConvertToFloat(String str)
{
    // 正規表現: 浮動小数点数を表す形式（例: 123, -123.45, .67, -0.8）
    std::regex floatRegex("^[-+]?\\d*(\\.\\d+)?$");
    return std::regex_match(str.c_str(), floatRegex);
}

inline void canConvertToIntTry(String str)
{
    if (!canConvertToInt(str))
    {
        throw std::runtime_error("Error: Cannot convert to int.");
    }
}

inline void canConvertToFloatTry(String str)
{
    if (!canConvertToFloat(str))
    {
        throw std::runtime_error("Error: Cannot convert to float.");
    }
}

#endif