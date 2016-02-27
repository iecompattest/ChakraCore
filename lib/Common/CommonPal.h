//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once


#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4995) /* 'function': name was marked as #pragma deprecated */

// === Windows Header Files ===
#define INC_OLE2                 /* for windows.h */
#define CONST_VTABLE             /* for objbase.h */
#include <windows.h>

/* Don't want GetObject and GetClassName to be defined to GetObjectW and GetClassNameW */
#undef GetObject
#undef GetClassName
#undef Yield /* winbase.h defines this but we want to use it for Js::OpCode::Yield; it is Win16 legacy, no harm undef'ing it */
#pragma warning(pop)

typedef wchar_t wchar16;

// xplat-todo: get a better name for this macro
#define CH_WSTR(s) L##s

#define get_cpuid __cpuid

#else // !_WIN32

#include "pal.h"
#include "inc/rt/palrt.h"
#include "inc/rt/no_sal2.h"
#include "inc/rt/oaidl.h"
#include <emmintrin.h>

typedef char16_t wchar16;
#define CH_WSTR(s) u##s

// xplat-todo: verify below is correct
#include <cpuid.h>
inline int get_cpuid(int cpuInfo[4], int function_id)
{
    return __get_cpuid(
            static_cast<unsigned int>(function_id),
            reinterpret_cast<unsigned int*>(&cpuInfo[0]),
            reinterpret_cast<unsigned int*>(&cpuInfo[1]),
            reinterpret_cast<unsigned int*>(&cpuInfo[2]),
            reinterpret_cast<unsigned int*>(&cpuInfo[3]));
}

#define _BitScanForward BitScanForward
#define _BitScanForward64 BitScanForward64
#define _BitScanReverse BitScanReverse
#define _BitScanReverse64 BitScanReverse64
#define _bittest BitTest
#define _bittestandset BitTestAndSet
#define _interlockedbittestandset InterlockedBitTestAndSet

#ifdef PAL_STDCPP_COMPAT
// SAL.h doesn't define these if PAL_STDCPP_COMPAT is defined
// Apparently, some C++ headers will conflict with this-
// not sure which ones but stubbing them out for now in linux-
// we can revisit if we do hit a conflict
#define __in    _SAL1_Source_(__in, (), _In_)
#define __out   _SAL1_Source_(__out, (), _Out_)

#define fclose          PAL_fclose
#define fflush          PAL_fflush
#define fwprintf        PAL_fwprintf
#define wcschr          PAL_wcschr
#define wcscmp          PAL_wcscmp
#define wcslen          PAL_wcslen
#define wcsncmp         PAL_wcsncmp
#define wcsrchr         PAL_wcsrchr
#define wcsstr          PAL_wcsstr
#define wprintf         PAL_wprintf

#define stdout          PAL_stdout
#endif // PAL_STDCPP_COMPAT

#define FILE PAL_FILE

// These are not available in pal
#define fwprintf_s      fwprintf
#define wmemcmp         wcsncmp
// sprintf_s overloaded in safecrt.h. Not sure why palrt.h redefines sprintf_s.
#undef sprintf_s

// PAL LoadLibraryExW not supported
#define LOAD_LIBRARY_SEARCH_SYSTEM32     0
// winerror.h
#define FACILITY_JSCRIPT                 2306
#define JSCRIPT_E_CANTEXECUTE            _HRESULT_TYPEDEF_(0x89020001L)
#define DISP_E_UNKNOWNINTERFACE          _HRESULT_TYPEDEF_(0x80020001L)
#define DISP_E_MEMBERNOTFOUND            _HRESULT_TYPEDEF_(0x80020003L)
#define DISP_E_UNKNOWNNAME               _HRESULT_TYPEDEF_(0x80020006L)
#define DISP_E_NONAMEDARGS               _HRESULT_TYPEDEF_(0x80020007L)
#define DISP_E_EXCEPTION                 _HRESULT_TYPEDEF_(0x80020009L)
#define DISP_E_BADINDEX                  _HRESULT_TYPEDEF_(0x8002000BL)
#define DISP_E_UNKNOWNLCID               _HRESULT_TYPEDEF_(0x8002000CL)
#define DISP_E_ARRAYISLOCKED             _HRESULT_TYPEDEF_(0x8002000DL)
#define DISP_E_BADPARAMCOUNT             _HRESULT_TYPEDEF_(0x8002000EL)
#define DISP_E_PARAMNOTOPTIONAL          _HRESULT_TYPEDEF_(0x8002000FL)
#define DISP_E_NOTACOLLECTION            _HRESULT_TYPEDEF_(0x80020011L)
#define TYPE_E_DLLFUNCTIONNOTFOUND       _HRESULT_TYPEDEF_(0x8002802FL)
#define TYPE_E_TYPEMISMATCH              _HRESULT_TYPEDEF_(0x80028CA0L)
#define TYPE_E_OUTOFBOUNDS               _HRESULT_TYPEDEF_(0x80028CA1L)
#define TYPE_E_IOERROR                   _HRESULT_TYPEDEF_(0x80028CA2L)
#define TYPE_E_CANTCREATETMPFILE         _HRESULT_TYPEDEF_(0x80028CA3L)
#define TYPE_E_CANTLOADLIBRARY           _HRESULT_TYPEDEF_(0x80029C4AL)
#define STG_E_TOOMANYOPENFILES           _HRESULT_TYPEDEF_(0x80030004L)
#define STG_E_ACCESSDENIED               _HRESULT_TYPEDEF_(0x80030005L)
#define STG_E_INSUFFICIENTMEMORY         _HRESULT_TYPEDEF_(0x80030008L)
#define STG_E_NOMOREFILES                _HRESULT_TYPEDEF_(0x80030012L)
#define STG_E_DISKISWRITEPROTECTED       _HRESULT_TYPEDEF_(0x80030013L)
#define STG_E_READFAULT                  _HRESULT_TYPEDEF_(0x8003001EL)
#define STG_E_SHAREVIOLATION             _HRESULT_TYPEDEF_(0x80030020L)
#define STG_E_LOCKVIOLATION              _HRESULT_TYPEDEF_(0x80030021L)
#define STG_E_MEDIUMFULL                 _HRESULT_TYPEDEF_(0x80030070L)
#define STG_E_INVALIDNAME                _HRESULT_TYPEDEF_(0x800300FCL)
#define STG_E_INUSE                      _HRESULT_TYPEDEF_(0x80030100L)
#define STG_E_NOTCURRENT                 _HRESULT_TYPEDEF_(0x80030101L)
#define STG_E_CANTSAVE                   _HRESULT_TYPEDEF_(0x80030103L)
#define REGDB_E_CLASSNOTREG              _HRESULT_TYPEDEF_(0x80040154L)
#define MK_E_UNAVAILABLE                 _HRESULT_TYPEDEF_(0x800401E3L)
#define MK_E_INVALIDEXTENSION            _HRESULT_TYPEDEF_(0x800401E6L)
#define MK_E_CANTOPENFILE                _HRESULT_TYPEDEF_(0x800401EAL)
#define CO_E_APPNOTFOUND                 _HRESULT_TYPEDEF_(0x800401F5L)
#define CO_E_APPDIDNTREG                 _HRESULT_TYPEDEF_(0x800401FEL)
#define GetScode(hr) ((SCODE) (hr))
// activscp.h
#define SCRIPT_E_RECORDED   0x86664004L

// _countof
#if defined _M_X64 || defined _M_ARM || defined _M_ARM64
#define _UNALIGNED __unaligned
#else
#define _UNALIGNED
#endif

#ifdef __cplusplus
extern "C++"
{
  template <typename _CountofType, size_t _SizeOfArray>
  char(*__countof_helper(_UNALIGNED _CountofType(&_Array)[_SizeOfArray]))[_SizeOfArray];

#define __crt_countof(_Array) (sizeof(*__countof_helper(_Array)) + 0)
}
#else
#define __crt_countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#ifndef _countof
#define _countof __crt_countof
#endif
// _countof

#define ARRAYSIZE(A) _countof(A)

//
//  Singly linked list structure. Can be used as either a list head, or
//  as link words.
//
typedef struct _SINGLE_LIST_ENTRY {
  struct _SINGLE_LIST_ENTRY *Next;
} SINGLE_LIST_ENTRY, *PSINGLE_LIST_ENTRY;

#if defined(_WIN64)

//
// The type SINGLE_LIST_ENTRY is not suitable for use with SLISTs.  For
// WIN64, an entry on an SLIST is required to be 16-byte aligned, while a
// SINGLE_LIST_ENTRY structure has only 8 byte alignment.
//
// Therefore, all SLIST code should use the SLIST_ENTRY type instead of the
// SINGLE_LIST_ENTRY type.
//

#pragma warning(push)
#pragma warning(disable:4324)   // structure padded due to align()

typedef struct DECLSPEC_ALIGN(16) _SLIST_ENTRY {
  struct _SLIST_ENTRY *Next;
} SLIST_ENTRY, *PSLIST_ENTRY;

#pragma warning(pop)

#else

typedef struct _SINGLE_LIST_ENTRY SLIST_ENTRY, *PSLIST_ENTRY;

#endif // _WIN64

#if defined(_AMD64_)

typedef union DECLSPEC_ALIGN(16) _SLIST_HEADER {
  struct {  // original struct
    ULONGLONG Alignment;
    ULONGLONG Region;
  } DUMMYSTRUCTNAME;
  struct {  // x64 16-byte header
    ULONGLONG Depth : 16;
    ULONGLONG Sequence : 48;
    ULONGLONG Reserved : 4;
    ULONGLONG NextEntry : 60; // last 4 bits are always 0's
  } HeaderX64;
} SLIST_HEADER, *PSLIST_HEADER;

#elif defined(_X86_)

typedef union _SLIST_HEADER {
  ULONGLONG Alignment;
  struct {
    SLIST_ENTRY Next;
    WORD   Depth;
    WORD   CpuId;
  } DUMMYSTRUCTNAME;
} SLIST_HEADER, *PSLIST_HEADER;

#elif defined(_ARM_)

typedef union _SLIST_HEADER {
  ULONGLONG Alignment;
  struct {
    SLIST_ENTRY Next;
    WORD   Depth;
    WORD   Reserved;
  } DUMMYSTRUCTNAME;
} SLIST_HEADER, *PSLIST_HEADER;

#endif

PALIMPORT VOID PALAPI InitializeSListHead(IN OUT PSLIST_HEADER ListHead);
PALIMPORT PSLIST_ENTRY PALAPI InterlockedPushEntrySList(IN OUT PSLIST_HEADER ListHead, IN OUT PSLIST_ENTRY  ListEntry);
PALIMPORT PSLIST_ENTRY PALAPI InterlockedPopEntrySList(IN OUT PSLIST_HEADER ListHead);


// Use overloaded versions of InterlockedExchangeAdd
#define InterlockedExchangeAdd __InterlockedExchangeAdd
inline long InterlockedExchangeAdd(
    IN OUT long volatile *Addend,
    IN long Value)
{
    return __sync_fetch_and_add(Addend, Value);
}
inline unsigned long InterlockedExchangeAdd(
    IN OUT unsigned long volatile *Addend,
    IN unsigned long Value)
{
    return __sync_fetch_and_add(Addend, Value);
}

inline long InterlockedExchangeSubtract(
    IN OUT long volatile *Addend,
    IN long Value)
{
    return __sync_fetch_and_sub(Addend, Value);
}
inline unsigned long InterlockedExchangeSubtract(
    IN OUT unsigned long volatile *Addend,
    IN unsigned long Value)
{
    return __sync_fetch_and_sub(Addend, Value);
}


// xplat-todo: implement these for JIT and Concurrent/Partial GC
uintptr_t _beginthreadex(
   void *security,
   unsigned stack_size,
   unsigned ( __stdcall *start_address )( void * ),
   void *arglist,
   unsigned initflag,
   unsigned *thrdaddr);
BOOL WINAPI GetModuleHandleEx(
  _In_     DWORD   dwFlags,
  _In_opt_ LPCTSTR lpModuleName,
  _Out_    HMODULE *phModule
);

// xplat-todo: implement this function to get the stack bounds of the current
// thread
// For Linux, we could use pthread_getattr_np to get the stack limit (end)
// and then use the stack size to calculate the stack base
int GetCurrentThreadStackBounds(char** stackBase, char** stackEnd);

// xplat-todo: cryptographically secure PRNG?
errno_t rand_s(unsigned int* randomValue);

#endif // _WIN32


// Use intsafe.h for internal builds (currently missing some files with stdint.h)
#if defined(_WIN32) && defined(NTBUILD)
#define ENABLE_INTSAFE_SIGNED_FUNCTIONS 1
#include <intsafe.h>
#else
#include <stdint.h>
#endif


#ifdef _MSC_VER
// ms-specific keywords
#define _ABSTRACT abstract
// MSVC2015 does not support C++11 semantics for `typename QualifiedName` declarations
// outside of template code.
#define _TYPENAME
#else
#define _ABSTRACT
#define _TYPENAME typename
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900
// "noexcept" not supported before VS 2015
#define _NOEXCEPT
#else
#define _NOEXCEPT noexcept
#endif
