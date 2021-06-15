#include "ExceptEnginePCH.h"

namespace ee
{
	SysInfo		lgSysInfo;
	SysInfo*	gSysInfo = &lgSysInfo;
}

using namespace ee;

namespace ee
{
	VariableString	VarOSInfo(L"OSInfo", L"Operation system information string", std::wstring());
	VariableString	VarOSVersion(L"OSVersion", L"Operation system version string", std::wstring());
	VariableString	VarOSArch(L"OSArch", L"Operation system architecture string", std::wstring());
	VariableString	VarOSCompName(L"OSCompName", L"Computer name string", std::wstring());

	VariableString	VarCpuInfo(L"CPUInfo", L"CPU information string", std::wstring());
	VariableInt		VarCPUFreq(L"CPUFrequency", L"CPU frequency in MHz", 0);
	VariableInt		VarCPUCount(L"CPUCount", L"count of CPU's", 0);
	VariableInt		VarCPUCores(L"CPUCores", L"count of CPU cores", 0);

	VariableInt		VarMemTotal(L"MemTotal", L"Total memory size in Mb", 0);

	VariableInt		VarGpuCount(L"GPUCount", L"count of GPU's", 0);
	
	VariableInt		VarAudioCount(L"AudioCount", L"count of audio devices", 0);
}

bool OSInfo::Collect(IWbemServices* pSvc)
{
	HRESULT hres;

	IEnumWbemClassObject* pEnumerator = nullptr;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_OperatingSystem"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		nullptr,
		&pEnumerator);

	if (FAILED(hres)) {
		LOG.Write(L"Query for operating system information failed. Error code = 0x%x", hres);
		return false; 
	}

	IWbemClassObject* pclsObj;
	ULONG uReturn = 0;

	while (pEnumerator)	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (0 == uReturn) {
			break;
		}

		VARIANT vtProp;

		hr = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
		name = vtProp.bstrVal;
		VariantClear(&vtProp);
		VarOSInfo = name;

		hr = pclsObj->Get(L"Version", 0, &vtProp, 0, 0);
		ver = vtProp.bstrVal;
		VariantClear(&vtProp);
		VarOSVersion = ver;

		hr = pclsObj->Get(L"OSArchitecture", 0, &vtProp, 0, 0);
		arch = vtProp.bstrVal;
		VariantClear(&vtProp);
		VarOSArch = arch;

		hr = pclsObj->Get(L"CSName", 0, &vtProp, 0, 0);
		compname = vtProp.bstrVal;
		VariantClear(&vtProp);
		VarOSCompName = compname;

		pclsObj->Release();
	}

	pEnumerator->Release();
	return true;
}

bool CPUInfo::Collect(IWbemServices* pSvc)
{
	HRESULT hres;

	IEnumWbemClassObject* pEnumerator = nullptr;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_Processor"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		nullptr,
		&pEnumerator);

	if (FAILED(hres)) {
		LOG.Write(L"Query for processor information failed. Error code = 0x%x", hres);
		return false;
	}

	IWbemClassObject* pclsObj;
	ULONG uReturn = 0;

	while (pEnumerator)	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (0 == uReturn) {
			break;
		}

		VARIANT vtProp;

		hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		name = vtProp.bstrVal;
		VariantClear(&vtProp);
		VarCpuInfo = name;

		hr = pclsObj->Get(L"CurrentClockSpeed", 0, &vtProp, 0, 0);
		speed = vtProp.intVal;
		VariantClear(&vtProp);
		VarCPUFreq = speed;

		hr = pclsObj->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
		if (hr != S_OK)
			cores = 1;
		else
			cores = vtProp.intVal;
		VariantClear(&vtProp);
		VarCPUCores = cores;

		hr = pclsObj->Get(L"NumberOfLogicalProcessors", 0, &vtProp, 0, 0);
		if (hr != S_OK)
			logical = 1;
		else
			logical = vtProp.intVal;
		VariantClear(&vtProp);
		VarCPUCount = logical;

		pclsObj->Release();
	}

	pEnumerator->Release();

	return true;
}

bool MemInfo::Collect(IWbemServices* pSvc)
{
	HRESULT hres;

	IEnumWbemClassObject* pEnumerator = nullptr;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_PhysicalMemory"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		nullptr,
		&pEnumerator);

	if (FAILED(hres)) {
		LOG.Write(L"Query for memory information failed. Error code = 0x%x", hres);
		return false;
	}

	IWbemClassObject* pclsObj;
	ULONG uReturn = 0;

	total = 0;

	while (pEnumerator)	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (0 == uReturn) {
			break;
		}

		VARIANT vtProp;

		hr = pclsObj->Get(L"Capacity", 0, &vtProp, 0, 0);
		ulong mem = String::From<ulong>(vtProp.bstrVal);
		total += (uint)(mem / 1024 / 1024);
		VariantClear(&vtProp);

		pclsObj->Release();
	}

	VarMemTotal = total;

	pEnumerator->Release();

	return true;
}

bool GPUInfo::Collect(IWbemServices* pSvc)
{
	HRESULT hres;

	IEnumWbemClassObject* pEnumerator = nullptr;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_VideoController"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		nullptr,
		&pEnumerator);

	if (FAILED(hres)) {
		LOG.Write(L"Query for GPU information failed. Error code = 0x%x", hres);
		return false;
	}

	IWbemClassObject* pclsObj;
	ULONG uReturn = 0;

	while (pEnumerator)	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (0 == uReturn) {
			break;
		}

		VARIANT vtProp;

		GPUAdapter ad;

		hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		ad.name = vtProp.bstrVal;
		VariantClear(&vtProp);

		hr = pclsObj->Get(L"DriverVersion", 0, &vtProp, 0, 0);
		ad.drvver = vtProp.bstrVal;
		VariantClear(&vtProp);

		hr = pclsObj->Get(L"AdapterRAM", 0, &vtProp, 0, 0);
		ad.mem = vtProp.uintVal / 1024 / 1024;
		VariantClear(&vtProp);

		adapters.push_back(ad);

		pclsObj->Release();
	}

	VarGpuCount = (int)adapters.size();

	pEnumerator->Release();

	return true;
}

bool AudioInfo::Collect(IWbemServices* pSvc)
{
	HRESULT hres;

	IEnumWbemClassObject* pEnumerator = nullptr;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_SoundDevice"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		nullptr,
		&pEnumerator);

	if (FAILED(hres)) {
		LOG.Write(L"Query for GPU information failed. Error code = 0x%x", hres);
		return false;
	}

	IWbemClassObject* pclsObj;
	ULONG uReturn = 0;

	while (pEnumerator) {
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (0 == uReturn) {
			break;
		}

		VARIANT vtProp;

		AudioAdapter ad;

		hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		ad.name = vtProp.bstrVal;
		VariantClear(&vtProp);

		adapters.push_back(ad);

		pclsObj->Release();
	}

	VarAudioCount = (int)adapters.size();

	pEnumerator->Release();

	return true;
}

bool LocaleInfo::Collect()
{
	int size;
	bool languageSuccess = false;

	if (!languageSuccess) {
		size = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE, nullptr, 0);
		m_language = new wchar_t[size + 1];
		if (GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE, m_language, size)==0) 
			throw std::exception("Couldn't get locale details");
	}

	size = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGCOUNTRY, nullptr, 0);
	m_country = new wchar_t[size + 1];
	if(GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGCOUNTRY, m_country, size)==0)
		throw std::exception("Couldn't get country details");

	return true;
}

