#pragma once
#include "../../../GEAREngine/src/GEARFirstInclude.h"
#include "../../core/gxDebug.h"

#include <signal.h>

namespace GEAR3D {
    namespace SignalHandler{
        
void SetupSignalHandler(int signal);
void SetupSignalHandlers();

#if GEAR_WINDOWS || GEAR_APPLE
#if GEAR_APPLE
void PrintStackTraceOSX (void* context);
void HandleSignal (int i, __siginfo* info, void* p);
#endif
#if GEAR_WINDOWS
int __cdecl HandleSignal( EXCEPTION_POINTERS* ep );
#endif

#if GEAR_WINDOWS
void __cdecl HandleAbort (int signal);
#endif
#endif

    }
}