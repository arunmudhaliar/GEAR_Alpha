#include "nvProfiler.h"

#if USE_NVPROFILER
NVPMContext nvProfiler::g_hNVPMContext;
bool nvProfiler::G_bNVPMInitialized;
bool nvProfiler::g_bPerformanceAnalyze;
int nvProfiler::g_MeshProfileCounter;
char* nvProfiler::counterNameRealNameMap[4][2];

NVPMCounterID nvProfiler::counterID[4];


CTrace<float> nvProfiler::realtimeTraces[4];
CTrace<float> nvProfiler::analysisTraces[4];
COGLTraceDisplay nvProfiler::realtimeTraceDisplay;
COGLTraceDisplay nvProfiler::analysisTraceDisplay;

nvProfiler::NVDataProvider nvProfiler::g_nvDataProvider;
nvProfiler::NVDataProvider* nvProfiler::nvDataProvider;
#endif