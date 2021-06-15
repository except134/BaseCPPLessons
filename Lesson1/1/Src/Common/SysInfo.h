#pragma once

namespace ee
{
	extern VariableString	VarOSInfo;
	extern VariableString	VarOSVersion;
	extern VariableString	VarOSArch;
	extern VariableString	VarOSCompName;

	extern VariableString	VarCpuInfo;
	extern VariableInt		VarCPUFreq;
	extern VariableInt		VarCPUCount;
	extern VariableInt		VarCPUCores;

	extern VariableInt		VarMemTotal;

	extern VariableInt		VarGpuCount;

	class OSInfo
	{
	public:
		bool Collect(IWbemServices* pSvc);

		void Print()
		{
			LOG.Write(L"\n    Operation system information:\n");
			LOG.Write(L"        Name:               %s\n", name.c_str());
			LOG.Write(L"        Version:            %s\n", ver.c_str());
			LOG.Write(L"        Architecture:       %s\n", arch.c_str());
			LOG.Write(L"        Computer name:      %s\n", compname.c_str());
		}

	private:
		std::wstring name;
		std::wstring ver;
		std::wstring arch;
		std::wstring compname;
	};

	class CPUInfo
	{
	public:
		bool Collect(IWbemServices* pSvc);

		void Print()
		{
			LOG.Write(L"\n    CPU information:\n");
			LOG.Write(L"        Model:              %s\n", name.c_str());
			LOG.Write(L"        Speed:              %d Mhz\n", speed);
			LOG.Write(L"        Cores:              %d\n", cores);
			LOG.Write(L"        Logical processors: %d\n", logical);
		}

		int NumberOfCores() { return cores; }
		int NumberOfLogicalProcessors() { return logical; }

	private:
		std::wstring	name;
		int				speed;
		int				cores;
		int				logical;
	};

	class MemInfo
	{
	public:
		bool Collect(IWbemServices* pSvc);

		void Print()
		{
			LOG.Write(L"\n    Memory information:\n");
			LOG.Write(L"        Total:              %d Mb\n", total);
		}

	private:
		uint total;
	};

	class GPUInfo
	{
	public:
		bool Collect(IWbemServices* pSvc);

		void Print()
		{
			LOG.Write(L"\n    GPU information:\n");
			for (auto i : adapters) {
				LOG.Write(L"        Model:              %s\n", i.name.c_str());
				LOG.Write(L"        Driver version:     %s\n", i.drvver.c_str());
				LOG.Write(L"        Memory:             %d Mb\n\n", i.mem);
			}
		}

	private:
		struct GPUAdapter
		{
			std::wstring	name;
			std::wstring	drvver;
			uint			mem;
		};

		std::vector<GPUAdapter>	adapters;
	};

	class AudioInfo
	{
	public:
		AudioInfo() {}
		~AudioInfo()
		{
		}

		bool Collect(IWbemServices* pSvc);
		
		void Print()
		{
			LOG.Write(L"\n    Audio information:\n");
			for (auto i : adapters) {
				LOG.Write(L"        Model:              %s\n", i.name.c_str());
			}
		}

	private:
		struct AudioAdapter
		{
			std::wstring	name;
		};

		std::vector<AudioAdapter>	adapters;
	};

	class LocaleInfo
	{
	public:
		wchar_t*	m_language;
		wchar_t*	m_country;

		LocaleInfo() : m_language(nullptr), m_country(nullptr) {}
		~LocaleInfo() { delete[] m_language; delete[] m_country; }

		bool Collect();

		void Print()
		{
			LOG.Write(L"\n    Locale information:\n");
			LOG.Write(L"        Language:           %s\n", m_language);
			LOG.Write(L"        Country:            %s\n", m_country);
		}
	};

	class SysInfo
	{
	public:
		void Collect()
		{
			HRESULT hres;

			hres = CoInitializeEx(0, COINIT_MULTITHREADED);
			if (FAILED(hres)) {
				LOG.Write(L"Failed to initialize COM library. Error code = 0x%x\n", hres);
				return;
			}

			hres = CoInitializeSecurity(
				nullptr,
				-1,                          // COM authentication
				nullptr,                        // Authentication services
				nullptr,                        // Reserved
				RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
				RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
				nullptr,                        // Authentication info
				EOAC_NONE,                   // Additional capabilities 
				nullptr                         // Reserved
				);


			if (FAILED(hres)) {
				LOG.Write(L"Failed to initialize security. Error code = 0x%x\n",hres);
				CoUninitialize();
				return;
			}

			IWbemLocator* pLoc = nullptr;

			hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&pLoc);

			if (FAILED(hres)) {
				LOG.Write(L"Failed to create IWbemLocator object. Err code = 0x%x\n", hres);
				CoUninitialize();
				return;
			}

			IWbemServices* pSvc = nullptr;

			hres = pLoc->ConnectServer(
				_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
				nullptr,                    // User name. nullptr = current user
				nullptr,                    // User password. nullptr = current
				0,                       // Locale. nullptr indicates current
				0,                    // Security flags.
				0,                       // Authority (for example, Kerberos)
				0,                       // Context object 
				&pSvc                    // pointer to IWbemServices proxy
				);

			if (FAILED(hres)) {
				LOG.Write(L"Could not connect. Error code = 0x%x\n", hres);
				pLoc->Release();
				CoUninitialize();
				return;
			}

			hres = CoSetProxyBlanket(
				pSvc,                        // Indicates the proxy to set
				RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
				RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
				nullptr,                        // Server principal name 
				RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
				RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
				nullptr,                        // client identity
				EOAC_NONE                    // proxy capabilities 
				);

			if (FAILED(hres)) {
				LOG.Write(L"Could not set proxy blanket. Error code = 0x%x\n", hres);
				pSvc->Release();
				pLoc->Release();
				CoUninitialize();
				return;               
			}

			osInfo.Collect(pSvc);
			cpuInfo.Collect(pSvc);
			memInfo.Collect(pSvc);
			gpuInfo.Collect(pSvc);
			audioInfo.Collect(pSvc);
			localeInfo.Collect();

			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
		}

		void Print()
		{
			LOG.Write(L"\nCollecting system information:\n");
			osInfo.Print();
			cpuInfo.Print();
			memInfo.Print();
			gpuInfo.Print();
			audioInfo.Print();
			localeInfo.Print();
			LOG.Write(L"\n");
		}

		const int NumberOfCPUCores() { return cpuInfo.NumberOfCores(); }
		const int NumberOfCPULogicalProcessors() { return cpuInfo.NumberOfLogicalProcessors(); }

	private:
		OSInfo		osInfo;
		CPUInfo		cpuInfo;
		MemInfo		memInfo;
		GPUInfo		gpuInfo;
		AudioInfo	audioInfo;
		LocaleInfo	localeInfo;
	};

	extern SysInfo* gSysInfo;

}

