#include "signalHandler.h"
#include <memory>

#include <mono/metadata/mono-config.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/tokentype.h>
#include <mono/metadata/mono-gc.h>
#include <mono/jit/jit.h>

namespace GEAR3D{
    namespace SignalHandler{

void SetupSignalHandler(int signal)
{
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_sigaction = HandleSignal;
    action.sa_flags = SA_SIGINFO;
    if(sigaction(signal, &action, NULL) < 0)
        printf("Error setting signal handler for signal %d\n",signal);
}

void SetupSignalHandlers()
{
#if GEAR_APPLE
#	if	WEBPLUG
    //Remove possible existing mach exception handlers which interfere with our exception handling (ie Firefox)
    mach_port_t current_task = mach_task_self();
    exception_mask_t exceptionMask = EXC_MASK_BAD_ACCESS | EXC_MASK_BAD_INSTRUCTION | EXC_MASK_ARITHMETIC | EXC_MASK_BREAKPOINT;
    task_set_exception_ports(current_task, exceptionMask, 0, EXCEPTION_DEFAULT, THREAD_STATE_NONE);
#   endif
    
    //#	if	!UNITY_IPHONE && !UNITY_PEPPER
    SetupSignalHandler(SIGSEGV);
    SetupSignalHandler(SIGBUS);
    SetupSignalHandler(SIGFPE);
    SetupSignalHandler(SIGILL);
    //#endif
#endif
    
    // Make sure abort() calls get passed to the crash handler
#if GEAR_WINDOWS
    _set_abort_behavior (0, _WRITE_ABORT_MSG);
    signal (SIGABRT, HandleAbort);
#endif
    
#if GEAR_WINDOWS || GEAR_APPLE || GEAR_ANDROID
    //this causes mono to forward the signal to us, if the signal is not mono related. (i.e. not a breakpoint, or a managed nullref exception, etc)
    mono_set_signal_chaining(1);
#endif
}

#if GEAR_WINDOWS || GEAR_APPLE
#if GEAR_APPLE

#include <dlfcn.h>
#include <cxxabi.h>

void PrintStackTraceOSX (void* context)
{
#if 0   //Compilation errors
    void *array[256];
    size_t size;
    size_t i;
    
    size = mono_backtrace_from_context (context, array, 256);
    
    printf("Obtained %zu stack frames.\n", size);
    
    for (i = 0; i < size; i++)
    {
        const char* symbolName = mono_pmip(array[i]);
        if (!symbolName)
        {
            Dl_info dlinfo;
            if (dladdr (array[i], &dlinfo))
            {
                symbolName = dlinfo.dli_sname;
                if (symbolName)
                {
                    int status = 0;
                    char* cname = abi::__cxa_demangle(symbolName, 0, 0, &status);
                    if (status == 0 && cname)
                        symbolName = cname;
                }
            }
            else
            {
                symbolName = "(Unknown)";
            }
        }
        
        printf_console ("#%-3d%016p in %s\n", i, array[i], symbolName);
    }
#endif
}


void HandleSignal (int i, __siginfo* info, void* p)
#endif
#if GEAR_WINDOWS
int __cdecl HandleSignal( EXCEPTION_POINTERS* ep )
#endif
{
    printf("Receiving unhandled NULL exception\n");

#if GEAR_APPLE
    printf("Launching bug reporter\n");
    PrintStackTraceOSX(p);
    //LaunchBugReporter (kCrashbug);
#elif GEAR_WINDOWS
//    winutils::ProcessInternalCrash(ep, false);
//    if( gUnityCrashHandler )
//    {
//        printf_console( "unity: Launch crash handler\n" );
//        return gUnityCrashHandler->ProcessCrash( ep );
//    }
//    return EXCEPTION_EXECUTE_HANDLER;
#else
#error "Unknown platform"
#endif
}

#endif // UNITY_OSX || UNITY_WIN

#if GEAR_WINDOWS
void __cdecl HandleAbort (int signal)
{
    printf("Received abort signal from the operating system\n");
//#if UNITY_EDITOR
//    LaunchBugReporter (kFatalError);
//#endif
}
#endif
        
    }
}
