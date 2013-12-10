#ifndef NVPROFILER_H
#define NVPROFILER_H

#define USE_NVPROFILER	0

#if USE_NVPROFILER
#include <Windows.h>
#include <gl\glut.h>
//#include <gl\GL.h>
// ********************************************************
// Set up NVPMAPI

#include "nvprofiler/tracedisplay.h"

#define NVPM_INITGUID
#include "../../../PerfKit_3.2.0.13289/inc/NvPmApi.Manager.h"
#define INSTRUMENTATION_ENABLED


#define PATH_TO_NVPMAPI_CORE L"D:/GITREPO/GEARREPO/PerfKit_3.2.0.13289/bin/win7_x86/NvPmApi.Core.dll"
// Simple singleton implementation for grabbing the NvPmApi
static NvPmApiManager S_NVPMManager;
static NvPmApiManager *GetNvPmApiManager()	{	return &S_NVPMManager;		}
static const NvPmApi *GetNvPmApi()			{	return S_NVPMManager.Api();	}

#define COUNTER_DISABLED ((NVPMCounterID) 0xFFFFFFFF)

class __declspec( dllexport ) nvProfiler
{
public:
	// A simple class to manage counters, sampling, and display of the information
	class NVDataProvider
	{
		public:
		static const GLuint counterEntryCount = 10;
		static const GLuint bufferEntryCount = 10;
			NVDataProvider()
				{
					// We're averaging these, so we'll need to initialize to zero
					for (GLuint i = 0; i < counterEntryCount; i++) {
						for (GLuint j = 0; j < bufferEntryCount; j++) {
							m_counterValues[i][j] = 0.0f;
						}
					}
					m_counterIndexArrayCount = 0;
					m_counterValuesRRIndex = 0;
				}
        
			virtual size_t nCounters() const
				{
					return m_counterIndexArrayCount;
				}

			virtual NVPMCounterID add(NVPMCounterID counterID)
				{
					if (GetNvPmApi()->AddCounter(g_hNVPMContext, counterID) == NVPM_OK) {
						m_counterIDArray[m_counterIndexArrayCount++] = counterID;
						return counterID;
					} else {
						return COUNTER_DISABLED;
					}
				}
            
			virtual GLuint add(char *counterName)
				{
					NVPMCounterID counterID;
					if (GetNvPmApi()->GetCounterIDByContext(g_hNVPMContext, counterName, &counterID) == NVPM_OK) {
						return add(counterID);
					} else {
						return COUNTER_DISABLED;
					}
				}
           
			virtual bool del(NVPMCounterID counterID)
				{
					if (GetNvPmApi()->RemoveCounter(g_hNVPMContext, counterID) == NVPM_OK) 
					{
						GLuint ii;
                    
						for(ii = 0; ii < m_counterIndexArrayCount; ii++) 
						{
							if(m_counterIDArray[ii] == counterID) 
							{
								// Overwrite this element with the last one on the list and decrement the list size
								m_counterIDArray[ii] = m_counterIDArray[m_counterIndexArrayCount - 1];
								m_counterIndexArrayCount--;
								break;
							}
						}
                    
						return true;
					} else {
						return false;
					}
				}

			virtual bool del(char *counterName)
				{
					NVPMCounterID counterID;
					if (GetNvPmApi()->GetCounterIDByContext(g_hNVPMContext, counterName, &counterID) == NVPM_OK) {
						return del(counterID);
					} else {
						return false;
					}
				}

			virtual bool removeAllCounters()
				{
					GetNvPmApi()->RemoveAllCounters(g_hNVPMContext);

					while (m_counterIndexArrayCount) {
						m_counterIDArray[--m_counterIndexArrayCount] = 0;
					}
                
					return true;
				}

			virtual bool sample()
				{
					GLuint ii;
					NVPMUINT unused;
					UINT64 events, cycles;

					// Sample the GPU counters
					GetNvPmApi()->Sample(g_hNVPMContext, NULL, &unused);

					// Retrieve the current sample values
					for (ii = 0; ii < m_counterIndexArrayCount; ii++) 
					{
						GetNvPmApi()->GetCounterValue(g_hNVPMContext, m_counterIDArray[ii], 0, &events, &cycles);
                    
						m_counterValues[ii][m_counterValuesRRIndex] =
							cycles ? (100.0f * (float) events / (float) cycles) : 0.0f;
					}
					m_counterValuesRRIndex++;
					if (m_counterValuesRRIndex >= bufferEntryCount) 
					{
						m_counterValuesRRIndex = 0;
					}
                
					return true;
				}
            
			virtual float value(const GLuint counterIndex) const
				{
					GLuint entryIndex, arrayIndex;
					GLfloat runningTotal = 0.0f;
                
					// Find this counter index
					for (arrayIndex = 0; arrayIndex < m_counterIndexArrayCount; arrayIndex++) {
						if(m_counterIDArray[arrayIndex] == counterIndex)
							break;
					}
                
					if(arrayIndex < m_counterIndexArrayCount) {
						for (entryIndex = 0; entryIndex < bufferEntryCount; entryIndex++) {
							runningTotal += 
								m_counterValues[arrayIndex][entryIndex] / (float)bufferEntryCount;
						}
					}
                
					return runningTotal;
				}

		protected:
			NVPMCounterID m_counterIDArray[counterEntryCount];
			GLuint m_counterIndexArrayCount;

			// Maintain a round-robin style buffer and display the average of the
			// the last bufferEntryCount samples.
			GLfloat m_counterValues[counterEntryCount][bufferEntryCount];
			GLuint  m_counterValuesRRIndex;
	};

	static NVPMContext g_hNVPMContext;
	static bool G_bNVPMInitialized;
	static bool g_bPerformanceAnalyze;
	static int g_MeshProfileCounter;
	static char *counterNameRealNameMap[4][2];

	static NVPMCounterID counterID[4];


	static CTrace<float> realtimeTraces[4];
	static CTrace<float> analysisTraces[4];
	static COGLTraceDisplay realtimeTraceDisplay;
	static COGLTraceDisplay analysisTraceDisplay;

	//******************************************************************************

	static NVDataProvider g_nvDataProvider;
	static NVDataProvider *nvDataProvider;


	static void EnableRuntimeCounters()
	{
		// Initialize the GPU counter display
		counterID[0] = nvDataProvider->add(counterNameRealNameMap[0][0]);
		counterID[1] = nvDataProvider->add(counterNameRealNameMap[1][0]);
		counterID[2] = nvDataProvider->add(counterNameRealNameMap[2][0]);
		counterID[3] = nvDataProvider->add(counterNameRealNameMap[3][0]);
	}
	
	static void InitNVPM(HGLRC rcCurrentContext)
	{
		G_bNVPMInitialized=false;
		g_bPerformanceAnalyze=false;
		g_MeshProfileCounter=0;
		nvDataProvider = &g_nvDataProvider;

		counterNameRealNameMap[0][0] = "gpu_idle";
		counterNameRealNameMap[0][1] = "GPU Idle";
		counterNameRealNameMap[1][0] = "shader_busy";
		counterNameRealNameMap[1][1] = "Shader Busy";
		counterNameRealNameMap[2][0] = "rop_busy";
		counterNameRealNameMap[2][1] = "ROP Busy";
		counterNameRealNameMap[3][0] = "gpu_busy";
		counterNameRealNameMap[3][1] = "GPU Busy";

		counterID[0] = COUNTER_DISABLED;
		counterID[1] = COUNTER_DISABLED;
		counterID[2] = COUNTER_DISABLED;
		counterID[3] = COUNTER_DISABLED;

		realtimeTraceDisplay.Position(0.37f, 0.07f, 0.62f, 0.2f);
		analysisTraceDisplay.Position(0.01f, 0.07f, 0.35f, 0.2f);
		// Initialize NVPerfKit
		if(GetNvPmApiManager()->Construct(PATH_TO_NVPMAPI_CORE) != S_OK) 
		{
			//gbnDisplay->setLabelString(string("NVPerfSDK failed to initialize - no GPU data will be available"));
			return;
		}

		if(GetNvPmApi()->Init() != NVPM_OK) 
		{
			//gbnDisplay->setLabelString(string("NVPerfSDK failed to initialize - no GPU data will be available"));
			return;
		}

		G_bNVPMInitialized = true;

		//HGLRC rcCurrentContext = wglGetCurrentContext();

		if(GetNvPmApi()->CreateContextFromOGLContext((APIContextHandle) rcCurrentContext, &g_hNVPMContext) != NVPM_OK) 
		{
			//gbnDisplay->setLabelString(string("NVPerfSDK failed to initialize - no GPU data will be available"));
			return;
		}

		EnableRuntimeCounters();

		realtimeTraces[0].name(counterNameRealNameMap[0][1]);
		realtimeTraces[1].name(counterNameRealNameMap[1][1]);
		realtimeTraces[2].name(counterNameRealNameMap[2][1]);
		realtimeTraces[3].name(counterNameRealNameMap[3][1]);

		realtimeTraces[0].min(0.0f);
		realtimeTraces[1].min(0.0f);
		realtimeTraces[2].min(0.0f);
		realtimeTraces[3].min(0.0f);

		realtimeTraces[0].max(100.0f);
		realtimeTraces[1].max(100.0f);
		realtimeTraces[2].max(100.0f);
		realtimeTraces[3].max(100.0f);

		realtimeTraceDisplay.BackgroundColor(0.1f, 0.1f, 0.2f, 0.3f);
		realtimeTraceDisplay.Insert(&realtimeTraces[0], 0.0f, 0.7f, 0.0f); // gpu_idle
		realtimeTraceDisplay.Insert(&realtimeTraces[1], 0.5f, 0.0f, 0.0f); // pixel_shader_busy
		realtimeTraceDisplay.Insert(&realtimeTraces[2], 0.0f, 0.0f, 0.8f); // vertex_shader_busy
		realtimeTraceDisplay.Insert(&realtimeTraces[3], 0.7f, 0.0f, 0.7f); // shader_waits_for_texture

		analysisTraces[0].name("Shader");
		analysisTraces[1].name("Texture");
		analysisTraces[2].name("Blend");
		analysisTraces[3].name("Frame Buffer");

		analysisTraces[0].min(0.0f);
		analysisTraces[1].min(0.0f);
		analysisTraces[2].min(0.0f);
		analysisTraces[3].min(0.0f);

		analysisTraces[0].max(100.0f);
		analysisTraces[1].max(100.0f);
		analysisTraces[2].max(100.0f);
		analysisTraces[3].max(100.0f);

		analysisTraceDisplay.BackgroundColor(0.1f, 0.1f, 0.2f, 0.3f);
		analysisTraceDisplay.Insert(&analysisTraces[0], 0.0f, 0.0f, 0.5f); // SHD
		analysisTraceDisplay.Insert(&analysisTraces[1], 0.0f, 0.0f, 0.8f); // TEX
		analysisTraceDisplay.Insert(&analysisTraces[2], 0.5f, 0.0f, 0.0f); // ROP
		analysisTraceDisplay.Insert(&analysisTraces[3], 0.0f, 0.7f, 0.0f); // FB

		realtimeTraceDisplay.DrawText(true);
		analysisTraceDisplay.DrawText(true);

		realtimeTraceDisplay.SetTextDrawColumn(false);
		analysisTraceDisplay.SetTextDrawColumn(false);

		realtimeTraceDisplay.SetDrawDoubleColumn(false);
		analysisTraceDisplay.SetDrawDoubleColumn(true);
	}

	static void SampleAndRenderStats()
	{
		if (g_bPerformanceAnalyze) {
			UINT64 value = 8, cycle = 0;

			// Grab the data we are interested in...
			if (GetNvPmApi()->GetCounterValueByName(g_hNVPMContext, "SHD Bottleneck", 0, &value, &cycle) == NVPM_OK && cycle) {
				analysisTraces[0].insert(value);
			}
			if (GetNvPmApi()->GetCounterValueByName(g_hNVPMContext, "SHD SOL", 0, &value, &cycle) == NVPM_OK && cycle) {
				analysisTraces[0].insert(value);
			}

			if (GetNvPmApi()->GetCounterValueByName(g_hNVPMContext, "TEX Bottleneck", 0, &value, &cycle) == NVPM_OK && cycle) {
				analysisTraces[1].insert(value);
			}
			if (GetNvPmApi()->GetCounterValueByName(g_hNVPMContext, "TEX SOL", 0, &value, &cycle) == NVPM_OK && cycle) {
				analysisTraces[1].insert(value);
			}

			if (GetNvPmApi()->GetCounterValueByName(g_hNVPMContext, "ROP Bottleneck", 0, &value, &cycle) == NVPM_OK && cycle) {
				analysisTraces[2].insert(value);
			}
			if (GetNvPmApi()->GetCounterValueByName(g_hNVPMContext, "ROP SOL", 0, &value, &cycle) == NVPM_OK && cycle) {
				analysisTraces[2].insert(value);
			}

			if (GetNvPmApi()->GetCounterValueByName(g_hNVPMContext, "FB Bottleneck", 0, &value, &cycle) == NVPM_OK && cycle) {
				analysisTraces[3].insert(value);
			}
			if (GetNvPmApi()->GetCounterValueByName(g_hNVPMContext, "FB SOL", 0, &value, &cycle) == NVPM_OK && cycle) {
				analysisTraces[3].insert(value);
			}

			nvDataProvider->removeAllCounters();

			EnableRuntimeCounters();

			g_bPerformanceAnalyze = false;

		} else {
			nvDataProvider->sample();

			for (GLuint ii = 0; ii < 4; ii++) {
				if(counterID[ii] != COUNTER_DISABLED)
					realtimeTraces[ii].insert(nvDataProvider->value(counterID[ii]));
				else
					realtimeTraces[ii].insert(0.0f);
			}
		}

		realtimeTraceDisplay.Display(CTraceDisplay::LINE_STREAM);
		analysisTraceDisplay.Display(CTraceDisplay::BAR);
	}
};


#endif

#endif