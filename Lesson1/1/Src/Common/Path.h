#pragma once

namespace ee
{
	namespace fs = std::filesystem;

	static const char* const	PATH_SEPARATORS			= "/\\";
	static const wchar_t* const	PATH_SEPARATORSW		= L"/\\";
	static uint					PATH_SEPARATORS_LEN		= 2;

	static const char			UNIX_PATH_SEPARATOR		= '/';
	static const char			WINDOWS_PATH_SEPARATOR	= '\\';

	static const wchar_t		UNIX_PATH_SEPARATORW = '/';
	static const wchar_t		WINDOWS_PATH_SEPARATORW = '\\';

	struct FileInfo
	{
		FileInfo(const std::wstring& _name, bool _folder) : name(_name), folder(_folder) { }
		FileInfo(const std::string& _name, bool _folder) : name(String::MultibyteToUnicode(_name)), folder(_folder) { }
		std::wstring	name;
		bool			folder;
	};
	typedef std::vector<FileInfo> VectorFileInfo;

	class Path
	{
	public:
		inline static std::string GetFilePath(const std::string& filename)
		{
			std::string::size_type slash = filename.find_last_of(PATH_SEPARATORS);
			if (slash == std::string::npos)
				return std::string();
			else
				return std::string(filename, 0, slash);
		}

		inline static std::wstring GetFilePath(const std::wstring& filename)
		{
			std::wstring::size_type slash = filename.find_last_of(PATH_SEPARATORSW);
			if (slash == std::wstring::npos)
				return std::wstring();
			else
				return std::wstring(filename, 0, slash);
		}

		inline static std::string RemoveTrailingSlash(const std::string& path)
		{
			bool removeSlash = ((String::EndsWith(path, "/") || String::EndsWith(path, "\\")))/*&&!isRoot(f)*/;
			return removeSlash ? path.substr(0, path.length() - 1) : path;
		}

		inline static std::wstring RemoveTrailingSlash(const std::wstring& path)
		{
			bool removeSlash = ((String::EndsWith(path, L"/") || String::EndsWith(path, L"\\")))/*&&!isRoot(f)*/;
			return removeSlash ? path.substr(0, path.length() - 1) : path;
		}

		inline static std::string AddTrailingSlash(const std::string& path)
		{
			bool slash = ((String::EndsWith(path, "/") || String::EndsWith(path, "\\")));
			return slash ? path : path + WINDOWS_PATH_SEPARATOR;
		}

		inline static std::wstring AddTrailingSlash(const std::wstring& path)
		{
			bool slash = ((String::EndsWith(path, L"/") || String::EndsWith(path, L"\\")));
			return slash ? path : path + WINDOWS_PATH_SEPARATORW;
		}

		inline static void ReplaceSlashFromWinToUnix(std::string& path)
		{
			std::replace(path.begin(), path.end(), WINDOWS_PATH_SEPARATOR, UNIX_PATH_SEPARATOR);
		}

		inline static void ReplaceSlashFromWinToUnix(std::wstring& path)
		{
			std::replace(path.begin(), path.end(), WINDOWS_PATH_SEPARATORW, UNIX_PATH_SEPARATORW);
		}

		inline static void ReplaceSlashFromUnixToWin(std::string& path)
		{
			std::replace(path.begin(), path.end(), UNIX_PATH_SEPARATOR, WINDOWS_PATH_SEPARATOR);
		}

		inline static void ReplaceSlashFromUnixToWin(std::wstring& path)
		{
			std::replace(path.begin(), path.end(), UNIX_PATH_SEPARATORW, WINDOWS_PATH_SEPARATORW);
		}

		inline static bool IsDirectory(const std::string& filename)
		{
			return fs::is_directory(fs::path(filename));
		}

		inline static bool IsDirectory(const std::wstring& filename)
		{
			return fs::is_directory(fs::path(filename));
		}

		inline static bool IsEmpty(const std::string& filename)
		{
			return fs::is_empty(fs::path(filename));
		}

		inline static bool IsEmpty(const std::wstring& filename)
		{
			return fs::is_empty(fs::path(filename));
		}

		inline static bool Create(const std::string& filename)
		{
			return fs::create_directory(fs::path(filename));
		}

		inline static bool Create(const std::wstring& filename)
		{
			return fs::create_directory(fs::path(filename));
		}

		inline static bool CreateRecursive(const std::string& filename)
		{
			return fs::create_directories(fs::path(filename));
		}

		inline static bool CreateRecursive(const std::wstring& filename)
		{
			return fs::create_directories(fs::path(filename));
		}

		inline static bool Delete(const std::string& filename)
		{
			return fs::remove(fs::path(filename));
		}

		inline static bool Delete(const std::wstring& filename)
		{
			return fs::remove(fs::path(filename));
		}

		inline static uintmax_t DeleteAll(const std::string& filename)
		{
			return fs::remove_all(fs::path(filename));
		}

		inline static uintmax_t DeleteAll(const std::wstring& filename)
		{
			return fs::remove_all(fs::path(filename));
		}

		inline static bool IsAbsolutePath(const char* path)
		{
			if (IsCharAlpha(path[0]) && path[1] == ':')
				return true;
			return path[0] == UNIX_PATH_SEPARATOR || path[0] == WINDOWS_PATH_SEPARATOR;
		}

		inline static bool IsAbsolutePath(const wchar_t* path)
		{
			if (IsCharAlphaW(path[0]) && path[1] == ':')
				return true;
			return path[0] == UNIX_PATH_SEPARATORW || path[0] == WINDOWS_PATH_SEPARATORW;
		}

		inline static std::string ConcatenatePath(const std::string& _base, const std::string& _name)
		{
			if (_base.empty() || IsAbsolutePath(_name.c_str())) {
				return _name;
			}
			else {
				if (String::EndsWith(_base, "\\") || String::EndsWith(_base, "/"))
					return _base + _name;

				return _base + WINDOWS_PATH_SEPARATOR + _name;
			}
		}

		inline static std::wstring ConcatenatePath(const std::wstring& _base, const std::wstring& _name)
		{
			if (_base.empty()||IsAbsolutePath(_name.c_str())) {
				return _name;
			} else {
				if (String::EndsWith(_base, L"\\") || String::EndsWith(_base, L"/"))
					return _base + _name;

				return _base + WINDOWS_PATH_SEPARATORW + _name;
			}
		}

		inline static bool IsReservedDir(const char* _fn)
		{
			return (_fn[0] == '.' && _fn[1] == 0);
		}

		inline static bool IsReservedDir(const wchar_t* _fn)
		{
			return (_fn[0] == '.' && _fn[1] == 0);
		}

		inline static bool IsParentDir(const char* _fn)
		{
			// if ".."
			return (_fn[0] == '.' && _fn[1] == '.' && _fn[2] == 0);
		}

		inline static bool IsParentDir(const wchar_t* _fn)
		{
			// if ".."
			return (_fn[0] == '.' && _fn[1] == '.' && _fn[2] == 0);
		}

		inline static void GetSystemFileList(VectorFileInfo& _result, const std::string& _folder, const std::string& _mask)
		{
			//FIXME add optional parameter?
			bool ms_IgnoreHidden = true;

			intptr_t lHandle;
			int res;
			struct _finddata_t tagData;

			// pattern can contain a directory name, separate it from mask
			size_t pos = _mask.find_last_of(PATH_SEPARATORS);
			std::string directory;
			if (pos != _mask.npos)
				directory = _mask.substr(0, pos);

			std::string full_mask = ConcatenatePath(_folder, _mask);

			lHandle = _findfirst(full_mask.c_str(), &tagData);
			res = 0;
			while (lHandle != -1 && res != -1) {
				if ((!ms_IgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0) && !IsReservedDir(tagData.name)) {
					_result.push_back(FileInfo(ConcatenatePath(directory, tagData.name), (tagData.attrib & _A_SUBDIR) != 0));
				}
				res = _findnext(lHandle, &tagData);
			}
			// Close if we found any files
			if (lHandle != -1)
				_findclose(lHandle);
		}

		inline static void GetSystemFileList(VectorFileInfo& _result, const std::wstring& _folder, const std::wstring& _mask)
		{
			//FIXME add optional parameter?
			bool ms_IgnoreHidden = true;

			intptr_t lHandle;
			int res;
			struct _wfinddata_t tagData;

			// pattern can contain a directory name, separate it from mask
			size_t pos = _mask.find_last_of(PATH_SEPARATORSW);
			std::wstring directory;
			if (pos != _mask.npos)
				directory = _mask.substr(0, pos);

			std::wstring full_mask = ConcatenatePath(_folder, _mask);

			lHandle = _wfindfirst(full_mask.c_str(), &tagData);
			res = 0;
			while (lHandle != -1 && res != -1) {
				if ((!ms_IgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0) && !IsReservedDir(tagData.name)) {
					_result.push_back(FileInfo(ConcatenatePath(directory, tagData.name), (tagData.attrib & _A_SUBDIR) != 0));
				}
				res = _wfindnext(lHandle, &tagData);
			}
			// Close if we found any files
			if (lHandle != -1)
				_findclose(lHandle);
		}

		inline std::string GetSystemCurrentFolder()
		{
			char buff[MAX_PATH + 1];
			::GetCurrentDirectoryA(MAX_PATH, buff);
			return buff;
		}

		inline std::wstring GetSystemCurrentFolder(bool wide)
		{
			wchar_t buff[MAX_PATH + 1];
			::GetCurrentDirectoryW(MAX_PATH, buff);
			return buff;
		}

		typedef std::vector<std::string> VectorAString;
		inline void ScanFolder(VectorAString& _result, const std::string& _folder, bool _recursive, const std::string& _mask, bool _fullpath)
		{
			std::string folder = _folder;
			if (!folder.empty() && *folder.rbegin() != UNIX_PATH_SEPARATOR && *folder.rbegin() != WINDOWS_PATH_SEPARATOR)
				folder += "/";

			VectorFileInfo result;
			GetSystemFileList(result, folder, _mask);

			for (VectorFileInfo::const_iterator item = result.begin(); item != result.end(); ++item) {
				if (item->folder)
					continue;

				if (_fullpath)
					_result.push_back(folder + String::UnicodeToMultibyte(item->name));
				else
					_result.push_back(String::UnicodeToMultibyte(item->name));
			}

			if (_recursive) {
				GetSystemFileList(result, folder, "*");

				for (VectorFileInfo::const_iterator item = result.begin(); item != result.end(); ++item) {
					if (!item->folder
						|| String::UnicodeToMultibyte(item->name) == ".."
						|| String::UnicodeToMultibyte(item->name) == ".") 
						continue;
					ScanFolder(_result, folder + String::UnicodeToMultibyte(item->name), _recursive, _mask, _fullpath);
				}
			}
		}

		typedef std::vector<std::wstring> VectorWString;
		inline void ScanFolder(VectorWString& _result, const std::wstring& _folder, bool _recursive, const std::wstring& _mask, bool _fullpath)
		{
			std::wstring folder = _folder;
			if (!folder.empty() && *folder.rbegin() != UNIX_PATH_SEPARATORW && *folder.rbegin() != WINDOWS_PATH_SEPARATORW) 
				folder += L"/";

			VectorFileInfo result;
			GetSystemFileList(result, folder, _mask);

			for (VectorFileInfo::const_iterator item = result.begin(); item != result.end(); ++item) {
				if (item->folder) 
					continue;

				if (_fullpath)
					_result.push_back(folder + item->name);
				else
					_result.push_back(item->name);
			}

			if (_recursive) {
				GetSystemFileList(result, folder, L"*");

				for (VectorFileInfo::const_iterator item = result.begin(); item != result.end(); ++item) {
					if (!item->folder
						|| item->name == L".."
						|| item->name == L".") 
						continue;
					ScanFolder(_result, folder + item->name, _recursive, _mask, _fullpath);
				}
			}
		}

	};
}