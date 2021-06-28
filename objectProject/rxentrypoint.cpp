#include "pch.h"

#include "adsmigr.h"
#include "adsdef.h"
#include "adscodes.h"
#include "acestext.h"
#include "acedads.h"

#include "SampleCustEnt.h"
#include "SampleReactor.h"
#include "SampleDrawOverrule.h"

SampleDrawOverrule* g_pSampleOverrule;

void initapp()
{
	g_pSampleOverrule = new SampleDrawOverrule();
	AcRxOverrule::addOverrule(AcDbLine::desc(), g_pSampleOverrule);

	SampleCustEnt::rxInit();//ע���Զ���ʵ��
	SampleReactor::rxInit();
	acrxBuildClassHierarchy();//����������ʱ����
}

void unloadapp()
{
	if (g_pSampleOverrule)
	{
		AcRxOverrule::removeOverrule(AcDbLine::desc(), g_pSampleOverrule);
		delete g_pSampleOverrule;
		g_pSampleOverrule = nullptr;
	}

	deleteAcRxClass(SampleCustEnt::desc());//ע���Զ���ʵ��
	deleteAcRxClass(SampleReactor::desc());//ע���Զ���ʵ��
}


extern "C" AcRx::AppRetCode zcrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
	switch (msg)
	{
		case AcRx::kInitAppMsg:
		{
			acrxDynamicLinker->unlockApplication(appId);
			acrxDynamicLinker->registerAppMDIAware(appId);
			initapp();
			
		}
		break;
		case AcRx::kUnloadAppMsg:
		{
			unloadapp();
		}
		break;
		
		case AcRx::kLoadDwgMsg:
		{	
		}
		break;
		case AcRx::kUnloadDwgMsg:
		{
			// Add your code here.
		}
		break;
		case AcRx::kInvkSubrMsg:
		{
		}
		break;
		
		default:
			break;
	}
	return AcRx::kRetOK;
}



#ifdef _WIN64
#pragma comment(linker, "/export:zcrxEntryPoint,PRIVATE")
#pragma comment(linker, "/export:zcrxGetApiVersion,PRIVATE")
#else // WIN32
#pragma comment(linker, "/export:_zcrxEntryPoint,PRIVATE")
#pragma comment(linker, "/export:_zcrxGetApiVersion,PRIVATE")
#endif

