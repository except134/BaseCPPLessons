#pragma once

namespace ee
{
	class String
	{
	public:
		inline static void ToLower(std::string& instr)
		{
			std::transform(instr.begin(), instr.end(), instr.begin(), tolower);
		}

		inline static void ToLower(std::wstring& instr)
		{
			std::transform(instr.begin(), instr.end(), instr.begin(), tolower);
		}

		inline static std::string ToLower(const std::string& instr)
		{
			std::string ret = instr;
			ToLower(ret);
			return ret;
		}

		inline static std::wstring ToLower(const std::wstring& instr)
		{
			std::wstring ret = instr;
			ToLower(ret);
			return ret;
		}

		inline static void ToUpper(std::string& instr)
		{
			std::transform(instr.begin(), instr.end(), instr.begin(), toupper);
		}

		inline static void ToUpper(std::wstring& instr)
		{
			std::transform(instr.begin(), instr.end(), instr.begin(), toupper);
		}

		inline static std::string ToUpper(const std::string& instr)
		{
			std::string ret = instr;
			ToUpper(ret);
			return ret;
		}

		inline static std::wstring ToUpper(const std::wstring& instr)
		{
			std::wstring ret = instr;
			ToUpper(ret);
			return ret;
		}

		inline static void Trim(std::string& str, bool left = true, bool right = true)
		{
			std::string delims = " \t\r";

			if (right)
				str.erase(str.find_last_not_of(delims) + 1);
			if (left)
				str.erase(0, str.find_first_not_of(delims));
		}

		inline static void Trim(std::wstring& str, bool left = true, bool right = true)
		{
			std::wstring delims = L" \t\r";

			if (right)
				str.erase(str.find_last_not_of(delims) + 1);
			if (left)
				str.erase(0, str.find_first_not_of(delims));
		}

		inline static std::string Trim(const std::string& str, bool left = true, bool right = true)
		{
			std::string ret = str;
			Trim(ret, left, right);
			return ret;
		}

		inline static std::wstring Trim(const std::wstring& str, bool left = true, bool right = true)
		{
			std::wstring ret = str;
			Trim(ret, left, right);
			return ret;
		}

		inline static bool IsEqualsNoCase(const std::string& rStr1, const std::string& rStr2)
		{
			return ToLower(rStr1) == ToLower(rStr2);
		}

		inline static bool IsEqualsNoCase(const std::wstring& rStr1, const std::wstring& rStr2)
		{
			return ToLower(rStr1) == ToLower(rStr2);
		}

		inline static bool IsEqualsWithCase(const std::string& rStr1, const std::string& rStr2)
		{
			return rStr1 == rStr2;
		}

		inline static bool IsEqualsWithCase(const std::wstring& rStr1, const std::wstring& rStr2)
		{
			return rStr1 == rStr2;
		}

		inline static bool ParseForBool(const std::string& str)
		{
			if (IsEqualsNoCase(str, "true") || IsEqualsNoCase(str, "1") || IsEqualsNoCase(str, "yes") || IsEqualsNoCase(str, "on") || IsEqualsNoCase(str, "enable"))
				return true;
				
			return false;
		}

		inline static bool ParseForBool(const std::wstring& str)
		{
			if (IsEqualsNoCase(str, L"true") || IsEqualsNoCase(str, L"1") || IsEqualsNoCase(str, L"yes") || IsEqualsNoCase(str, L"on") || IsEqualsNoCase(str, L"enable"))
				return true;

			return false;
		}

		inline static void Replace(std::string& instring, const std::string::value_type& oldval, const std::string::value_type& newval)
		{
			std::replace(instring.begin(), instring.end(), oldval, newval);
		}

		inline static void Replace(std::wstring& instring, const std::wstring::value_type& oldval, const std::wstring::value_type& newval)
		{
			std::replace(instring.begin(), instring.end(), oldval, newval);
		}

		inline static std::string Replace(const std::string& instring, const std::string::value_type& oldval, const std::string::value_type& newval)
		{
			std::string ret = instring;
			Replace(ret, oldval, newval);
			return ret;
		}

		inline static std::wstring Replace(const std::wstring& instring, const std::wstring::value_type& oldval, const std::wstring::value_type& newval)
		{
			std::wstring ret = instring;
			Replace(ret, oldval, newval);
			return ret;
		}

		inline static std::string Replace(const std::string& s, const std::string& pattern, const std::string& replacement)
		{
			if (pattern.length() == 0) {
				return s;
			}

			std::string temp = "";

			size_t lastindex = 0;
			size_t nextindex = 0;
			do {
				nextindex = s.find(pattern, lastindex);
				if (nextindex == std::string::npos) {
					break;
				}
				temp += s.substr(lastindex, nextindex - lastindex) + replacement;
				lastindex = nextindex + pattern.length();
			} while (lastindex + pattern.length() <= s.length());
			return temp + (lastindex < s.length() ? s.substr(lastindex) : "");
		}

		inline static std::wstring Replace(const std::wstring& s, const std::wstring& pattern, const std::wstring& replacement)
		{
			if (pattern.length() == 0) {
				return s;
			}

			std::wstring temp = L"";

			size_t lastindex = 0;
			size_t nextindex = 0;
			do {
				nextindex = s.find(pattern, lastindex);
				if (nextindex == std::wstring::npos) {
					break;
				}
				temp += s.substr(lastindex, nextindex - lastindex) + replacement;
				lastindex = nextindex + pattern.length();
			} while (lastindex + pattern.length() <= s.length());
			return temp + (lastindex < s.length() ? s.substr(lastindex) : L"");
		}

		inline static bool BeginsWith(const std::string& test, const std::string& pattern)
		{
			if (test.size() >= pattern.size()) {
				for (int i = 0; i < (int)pattern.size(); ++i) {
					if (pattern[i] != test[i]) {
						return false;
					}
				}
				return true;
			}
			else {
				return false;
			}
		}

		inline static bool BeginsWith(const std::wstring& test, const std::wstring& pattern)
		{
			if (test.size() >= pattern.size()) {
				for (int i = 0; i < (int)pattern.size(); ++i) {
					if (pattern[i] != test[i]) {
						return false;
					}
				}
				return true;
			}
			else {
				return false;
			}
		}

		inline static bool EndsWith(const std::string& test, const std::string& pattern)
		{
			if (test.size() >= pattern.size()) {
				size_t te = test.size() - 1;
				size_t pe = pattern.size() - 1;
				for (int i = (int)pattern.size() - 1; i >= 0; --i) {
					if (pattern[pe - i] != test[te - i]) {
						return false;
					}
				}
				return true;
			}
			else {
				return false;
			}
		}

		inline static bool EndsWith(const std::wstring& test, const std::wstring& pattern)
		{
			if (test.size() >= pattern.size()) {
				size_t te = test.size() - 1;
				size_t pe = pattern.size() - 1;
				for (int i = (int)pattern.size() - 1; i >= 0; --i) {
					if (pattern[pe - i] != test[te - i]) {
						return false;
					}
				}
				return true;
			}
			else {
				return false;
			}
		}

		template<typename T>
		inline static T From(const std::string& str)
		{
			std::istringstream is(str);
			T t;
			is >> t;
			return t;
		}

		template<typename T>
		inline static T From(const std::wstring& str)
		{
			std::wistringstream is(str);
			T t;
			is >> t;
			return t;
		}

		inline static std::string BoolToStrA(bool val)
		{
			return val ? "True" : "False";
		}

		inline static std::wstring BoolToStrW(bool val)
		{
			return val ? L"True" : L"False";
		}

		inline static bool MultibyteToUnicode(const char* strSource, wchar_t* wstrDestination)
		{
			size_t cchDestChar = strlen(strSource) + 1;

			if (wstrDestination == nullptr || strSource == nullptr || cchDestChar < 1)
				return false;

			int nResult = MultiByteToWideChar(CP_ACP, 0, strSource, -1, wstrDestination, (int)cchDestChar);
			wstrDestination[cchDestChar - 1] = 0;

			if (nResult == 0)
				return false;

			return true;
		}

		inline static bool UnicodeToMultibyte(const wchar_t* wstrSource, char* strDestination)
		{
			size_t cchDestChar = wcslen(wstrSource) + 1;

			if (strDestination == nullptr || wstrSource == nullptr || cchDestChar < 1)
				return false;

			int nResult = WideCharToMultiByte(CP_ACP, 0, wstrSource, -1, strDestination, int(cchDestChar*sizeof(char)), nullptr, nullptr);
			strDestination[cchDestChar - 1] = 0;

			if (nResult == 0)
				return false;

			return true;
		}

		inline static bool MultibyteToUnicode(const std::string& strSource, std::wstring& wstrDestination)
		{
			auto retbuf = std::make_unique<wchar_t[]>(strSource.size() + 1);
			bool ret = MultibyteToUnicode(strSource.c_str(), retbuf.get());
			if (!ret) {
				return false;
			}
			wstrDestination.assign(retbuf.get());
			return true;
		}

		inline static bool UnicodeToMultibyte(const std::wstring& strSource, std::string& wstrDestination)
		{
			auto retbuf = std::make_unique<char[]>(strSource.size() + 1);
			bool ret = UnicodeToMultibyte(strSource.c_str(), retbuf.get());
			if (!ret) {
				return false;
			}
			wstrDestination.assign(retbuf.get());
			return true;
		}

		inline static std::wstring MultibyteToUnicode(const std::string& strSource)
		{
			std::wstring ret;
			MultibyteToUnicode(strSource, ret);
			return ret;
		}

		inline static std::string UnicodeToMultibyte(const std::wstring& strSource)
		{
			std::string ret;
			UnicodeToMultibyte(strSource, ret);
			return ret;
		}
	};

}

