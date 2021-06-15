#include "ExceptEnginePCH.h"

using namespace ee;

GraphicsDeviceVulkan::GraphicsDeviceVulkan(bool debuglayer) : GraphicsDevice()
{
	if (gSysInfo->NumberOfCPULogicalProcessors() >= 4)
		NumberOfGraphicsThreads = gSysInfo->NumberOfCPULogicalProcessors() - 2;
	else
		NumberOfGraphicsThreads = 1;
}

GraphicsDeviceVulkan::~GraphicsDeviceVulkan()
{

}

