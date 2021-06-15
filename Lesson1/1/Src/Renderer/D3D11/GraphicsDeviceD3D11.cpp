#include "ExceptEnginePCH.h"

using namespace ee;
using namespace DirectX;

GraphicsDeviceD3D11::GraphicsDeviceD3D11(bool debuglayer) : GraphicsDevice()
{
	if (gSysInfo->NumberOfCPULogicalProcessors() >= 4)
		NumberOfGraphicsThreads = gSysInfo->NumberOfCPULogicalProcessors() - 2;
	else
		NumberOfGraphicsThreads = 1;

	commandLists.resize(NumberOfGraphicsThreads);
	deviceContexts.resize(NumberOfGraphicsThreads);
	stencilRef.resize(NumberOfGraphicsThreads);
	blendFactor.resize(NumberOfGraphicsThreads);

	for(int i = 0; i < NumberOfGraphicsThreads; i++) {
		SafeNullptr(commandLists[i]);
		SafeNullptr(deviceContexts[i]);
		stencilRef[i] = 0;
		blendFactor[i] = XMFLOAT4(1, 1, 1, 1);
	}


}

GraphicsDeviceD3D11::~GraphicsDeviceD3D11()
{
}

