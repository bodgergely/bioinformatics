#pragma once

#include <algorithm>
#include <cctype>
#include <fstream>
#include <locale>
#include <map>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
    std::stringstream buf;
    buf << "[";
    for (int i = 0; i < vec.size(); i++) {
        buf << vec[i];
        buf << ", ";
    }
    auto res = buf.str();
    if(vec.size() > 0) {
        res.pop_back();
        res.pop_back();
    }
    res.push_back(']');
    os << res;
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& cont)
{
    std::stringstream buf;
    buf << "[";
    for (auto it = cont.begin(); it != cont.end(); it++) {
        buf << *it;
        buf << ", ";
    }
    auto res = buf.str();
    if(cont.size() > 0) {
        res.pop_back();
        res.pop_back();
    }
    res.push_back(']');
    os << res;
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& cont)
{
    std::stringstream buf;
    buf << "{";
    for (auto it = cont.begin(); it != cont.end(); it++) {
        buf << *it;
        buf << ", ";
    }
    auto res = buf.str();
    if(cont.size() > 0) {
        res.pop_back();
        res.pop_back();
    }
    res.push_back('}');
    os << res;
    return os;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& cont)
{
    std::stringstream buf;
    buf << "{";
    for (auto it = cont.begin(); it != cont.end(); it++) {
        buf << it->first;
        buf << ": ";
        buf << it->second;
    }
    auto res = buf.str();
    if(cont.size() > 0) {
        res.pop_back();
        res.pop_back();
    }
    res.push_back('}');
    os << res;
    return os;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& cont)
{
    std::stringstream buf;
    buf << "{";
    for (auto it = cont.begin(); it != cont.end(); it++) {
        buf << it->first;
        buf << ": ";
        buf << it->second;
    }
    auto res = buf.str();
    if(cont.size() > 0) {
        res.pop_back();
        res.pop_back();
    }
    res.push_back('}');
    os << res;
    return os;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p1)
{
    std::stringstream buf;
    buf << "(";
    buf << p1.first;
    buf << ", ";
    buf << p1.second;
    buf << ")";
    os << buf.str();
    return os;
}

// trim from start (in place)
static inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
static inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}

std::vector<std::string> split(const std::string& str, char delimiter = ' ')
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string toLower(const std::string& str)
{
    auto cp = str;
    std::transform(cp.begin(), cp.end(), cp.begin(),
                   [](unsigned char ch) { return std::tolower(ch); });
    return cp;
}

void toLowerIP(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char ch) { return std::tolower(ch); });
}

std::string toUpper(const std::string& str)
{
    auto cp = str;
    std::transform(cp.begin(), cp.end(), cp.begin(),
                   [](unsigned char ch) { return std::toupper(ch); });
    return cp;
}

void toUpperIP(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char ch) { return std::toupper(ch); });
}
