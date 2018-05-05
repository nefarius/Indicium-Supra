#pragma once
#include <string>

#define LOG_REGION() (std::string(typeid(this).name()) + std::string("::") + std::string(__func__))
