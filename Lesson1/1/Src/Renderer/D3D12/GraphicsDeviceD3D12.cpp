#include "ExceptEnginePCH.h"

using namespace ee;

GraphicsDeviceD3D12::GraphicsDeviceD3D12(bool debuglayer) : GraphicsDevice()
{
	if (gSysInfo->NumberOfCPULogicalProcessors() >= 4)
		NumberOfGraphicsThreads = gSysInfo->NumberOfCPULogicalProcessors() - 2;
	else
		NumberOfGraphicsThreads = 1;
}

GraphicsDeviceD3D12::~GraphicsDeviceD3D12()
{

}

