#pragma once
#include "core/exception.hh"


namespace ui
{
    class CtxException : public Exception
    { public: using Exception::Exception; };
}