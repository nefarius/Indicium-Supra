#pragma once
#include <string>

unsigned long procIdByProcName(const std::string&);
unsigned long procIdByWindowName(const std::string&);

#define LOG_REGION() (std::string(typeid(this).name()) + std::string("::") + std::string(__func__))
