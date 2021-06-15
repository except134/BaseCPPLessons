#include "ExceptEnginePCH.h"

using namespace ee;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    try
    {
		LOG.Start(L"./ExceptEngine.log");

		VulkanTest vt;
		vt.Test1();

		gSysInfo->Collect();
		gSysInfo->Print();

		ConfigDialog cfg;
		if (cfg.Display(hInstance, L"./ExceptEngine.ini"))
			cfg.GetParams(*gConfigParams);
		else
			return 0;

		std::unique_ptr<GraphicsDevice> gd;
		switch (gConfigParams->Renderer) {
			case RENDERER::RENDERER_D3D11:
				gd = std::make_unique<GraphicsDeviceD3D11>();
			break;
			case RENDERER::RENDERER_D3D12:
				gd = std::make_unique<GraphicsDeviceD3D12>();
			break;
			case RENDERER::RENDERER_VULKAN:
				gd = std::make_unique<GraphicsDeviceVulkan>();
			break;
			default: {
				LOG.Write(L"You must choose renderer at ExceptEngine.ini file. (RendererD3D11, RendererD3D12 or RendererVulkan)");
			}
		}

		XAudioWaveData waveData;
		waveData.LoadFromFile(L"./Data/test.wav");

		XAudio3DSound pSource;
		pSource.CreateSound(&waveData);
		pSource.SetLoopFlag(true);
		//pSource.SetPan( -1 );
		pSource.SetEmitterPostion(-15, 0, -10);
		pSource.SetEmitterRadius(1);
		pSource.SetVolume(2);
		pSource.Play();

		pSource.UsingReverb(true);

		float x = -15;
		int signe = 1;
		while (1)
		{
			x += 0.05f * signe;

			if (abs(x) > 15.0f)
				signe *= -1;

			Sleep(16);
			pSource.SetEmitterPostion(x, 0, 10);
			pSource.Apply();

		}

    }
    catch (std::exception&e)
    {
        MessageBoxA(nullptr, e.what(), "Standard exception occurred", 0);
    }
    catch (...)
    {
        MessageBoxA(nullptr, "Unknown", "Exception occurred", 0);
    }

    return 0;
}

