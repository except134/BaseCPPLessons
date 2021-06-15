#pragma once

#define NOMINMAX
#include <Windows.h>
#include <exception>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <cassert>
#include <cmath>
#include <functional>
#include <sstream>
#include <stack>
#include <map>
#include <io.h>
#include <memory>
#include <Wbemidl.h>
#include <comdef.h>
#include <MMSystem.h>
#include <stdint.h>
#include <mutex>
#include <atomic>
#include <limits>

#pragma comment(lib, "version.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "winmm.lib")

#include <d3d11.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#include <vulkan/vulkan.h>

#pragma comment(lib, "VKstatic.1.lib")
#pragma comment(lib, "Cfgmgr32.lib")

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>
#include <xapofx.h>
#pragma comment(lib,"xaudio2.lib")

#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>

#pragma comment(lib,"ogg.lib")
#pragma comment(lib,"vorbis.lib")
#pragma comment(lib,"vorbisenc.lib")
#pragma comment(lib,"vorbisfile.lib")

#define lita(arg)       #arg
#define xlita(arg)      lita(arg)
#define cat3(w,x,z)     w##.##x##.##z##\000
#define cat4(w,x,z,a)   w##.##x##.##z##.##a##\000
#define xcat3(w,x,z)    cat3(w,x,z)
#define xcat4(w,x,z,a)  cat4(w,x,z,a)
#define VERSION_STRING	xlita(xcat4(EXCEPTENGINE_VERSION_MAJOR,EXCEPTENGINE_VERSION_MINOR,EXCEPTENGINE_VERSION_BUILDNO,EXCEPTENGINE_VERSION_EXTEND))

#define USE_LOGGING

#include "Res/resource.h"
#include "Res/VersionNo.h"

#include "Common/Types.h"
#include "Common/Helpers.h"
#include "Common/String.h"
#include "Common/Path.h"
#include "Common/VersionInfo.h"
#include "Common/DateTime.h"
#include "Common/WriterASCII.h"
#include "Common/Logger.h"
#include "Common/Environment.h"
#include "Common/IniFile.h"
#include "Common/Variable.h"
#include "Common/SysInfo.h"

class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

	std::wstring ToString()const;

	HRESULT ErrorCode = S_OK;
	std::wstring FunctionName;
	std::wstring Filename;
	int LineNumber = -1;
};

#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = ee::String::MultibyteToUnicode(__FILE__);                       \
    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#endif

#include "Config/ConfigParams.h"
#include "Config/ConfigDialog.h"

#include "Audio/AudioMaster.h"
#include "Audio/AudioWaveData.h"
#include "Audio/AudioOGGData.h"
#include "Audio/AudioSound.h"
#include "Audio/Audio3DSound.h"

#include "Renderer/GraphicsCommon.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/D3D11/GraphicsDeviceD3D11.h"
#include "Renderer/D3D12/GraphicsDeviceD3D12.h"
#include "Renderer/Vulkan/GraphicsDeviceVulkan.h"



