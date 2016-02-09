#pragma once

#ifdef _M_IX86
#ifndef UINTX
#define UINTX UINT32
#endif
#else
#ifndef UINTX
#define UINTX UINT64
#endif
#endif