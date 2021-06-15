#pragma once

namespace ee
{
	class Environment
	{
	public:
		inline static std::string GetEnv(const std::string& env)
		{
			char* libvar;
			size_t requiredSize;
			std::string ret="";

			getenv_s(&requiredSize, nullptr, 0, env.c_str());
			if (requiredSize == 0) {
				LOG.Write(L"%s environment doesn't exist!\n",env.c_str());
				return std::string("");
			}

			libvar = (char*)malloc(requiredSize * sizeof(char));
			if (!libvar) {
				LOG.Write(L"Failed to allocate memory for environment variable %s.\n",env.c_str());
				return std::string("");
			}

			getenv_s(&requiredSize, libvar, requiredSize, env.c_str());
			ret.assign(libvar);
			free(libvar);

			return ret;
		}

		inline static HMODULE GetProcessHandle()
		{
			return GetModuleHandle(0);
		}

		inline static HMODULE GetProcessHandle(const std::wstring& module)
		{
			return GetModuleHandle(module.c_str());
		}

		inline static std::wstring GetExecutableFullFileName()
		{
			wchar_t ret[MAX_PATH];
			if(GetModuleFileName(0,ret,MAX_PATH))
				return std::wstring(ret);
			return L"";
		}

		inline static std::wstring GetExecutablePath()
		{
			return Path::AddTrailingSlash(Path::GetFilePath(Environment::GetExecutableFullFileName()));
		}

		inline static std::wstring GetAppName()
		{
			VersionInfo ver(GetProcessHandle());
			wchar_t ret[MAX_PATH];
			ver.AsString(ret);
			return std::wstring(ver.GetProductName());
		}

		inline static std::wstring GetAppVersion()
		{
			VersionInfo ver(GetProcessHandle());
			wchar_t ret[MAX_PATH];
			ver.AsString(ret);
			return String::Replace(ret,',','.');
		}

		inline static std::wstring GetArch()
		{
#ifdef _WIN64
			return L"Win64";
#else
			return L"Win32";
#endif
		}

		inline static std::wstring GetBuild()
		{
#ifdef _DEBUG
			return L"Debug";
#else
			return L"Release";
#endif
		}

		inline static std::wstring GetCompilatorInfo()
		{
			std::wstring ret=L"Unknown";
#if _MSC_VER
			ret=L"Visual C++ " xlita(_MSC_FULL_VER);
#endif
			return ret;
		}

	};
}

