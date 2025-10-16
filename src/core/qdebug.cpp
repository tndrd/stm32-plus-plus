#include "qdebug.h"
#include <stdio.h>
#include <memory>

Debug& Debug::instance() 
{
  static auto self = std::make_unique<Debug>();
  return *self;
}

Debug& qDebug() { return Debug::instance(); }

void qDebug_default_outfn(const char *s)
{
    printf(s);
}

Debug::OutFnT Debug::mOutFn = qDebug_default_outfn;

void Debug::setOutputFunc(OutFnT f)
{
    mOutFn = f;
}

template<> Debug& Debug::operator<<(const char* str)
{
  mOutFn(str);
  return *this;
}
