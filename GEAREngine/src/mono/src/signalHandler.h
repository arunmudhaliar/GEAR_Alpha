#pragma once


#include <signal.h>

namespace GEAR3D {
    namespace SignalHandler{
        
void SetupSignalHandler(int signal);
void SetupSignalHandlers();

#if _WIN32 || __APPLE__
#if __APPLE__
void PrintStackTraceOSX (void* context);
void HandleSignal (int i, __siginfo* info, void* p);
#endif
#if _WIN32
int __cdecl HandleSignal( EXCEPTION_POINTERS* ep );
#endif

#if _WIN32
void __cdecl HandleAbort (int signal);
#endif
#endif

    }
}