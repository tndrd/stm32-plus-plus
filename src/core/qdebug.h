#ifndef _QDEBUG_H
#define _QDEBUG_H

#include <vector>
#include <string>
#include <memory>

/* Uncomment this to enable proper polymorhpic output via qDebug() */
/* Otherwise anything except strings will be printed as "???"      */
/* Enabling this option increases executable size by >200Kb        */
// #define STM32PP_QDEBUG_EXTENDED

class Debug
{
public:
  using OutFnT = void(*)(const char*);

private:
  static OutFnT mOutFn;

public:
  static Debug& instance();
  static void setOutputFunc(OutFnT f);
  
  template <class T>
  Debug &operator <<(T);
};

Debug& qDebug();

template<>
Debug &Debug::operator<<(const char*);

#if defined(STM32PP_QDEBUG_EXTENDED)
#include <sstream>

template<typename T>
Debug& Debug::operator<<(T v)
{
    std::stringstream ss;
    ss << v;
    mOutFn(ss.str().c_str());
    return *this;
}
#else
template<typename T>
Debug& Debug::operator<<(T v)
{
    mOutFn("???");
    return *this;
}
#endif

#endif