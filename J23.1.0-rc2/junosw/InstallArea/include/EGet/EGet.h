#pragma once
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>

struct EGet
{
    template<typename T>
    static T Parse(const char* str) ; 

    template<typename T>
    static T Get(const char* ekey, T fallback) ; 
};

template<typename T>
inline T EGet::Parse(const char* str_)
{
    std::string str(str_);
    std::istringstream iss(str);
    T tval ; 
    iss >> tval ; 
    return tval ; 
}
template int      EGet::Parse<int>(     const char*);  
template unsigned EGet::Parse<unsigned>(const char*);  
template float    EGet::Parse<float>(   const char*);  
template double   EGet::Parse<double>(  const char*);  


template<typename T>
inline T EGet::Get(const char* ekey, T fallback)
{
    char* v = getenv(ekey);
    if(v == nullptr) return fallback ; 
    return Parse<T>(v); 
}

template int      EGet::Get<int>(     const char*, int );  
template unsigned EGet::Get<unsigned>(const char*, unsigned );  
template float    EGet::Get<float>(   const char*, float );  
template double   EGet::Get<double>(  const char*, double );  


