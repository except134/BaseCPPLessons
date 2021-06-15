#pragma once

namespace ee
{
	class DateTime
	{
	public:
		DateTime();
		~DateTime();

		static std::wstring GetCurDate();
		static std::wstring GetCurTime();
		static std::wstring GetCurDateTime();
	};
}




