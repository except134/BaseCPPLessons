#pragma once

class Tasks
{
public:
	void Task1();
	void Task2();
	void Task3();
	void Task4();
	void Task5();

private:
	void Tasks3CreateFile1(const std::string& fname);
	void Tasks3CreateFile2(const std::string& fname);
	void Task4FileGlue(const std::string& fname1, const std::string& fname2);
	bool FileExists(std::string_view strFile);
	std::string GetFileNameFromFilePath(std::string_view strPath);
	std::string GetFileNameNoExtensionFromFilePath(std::string_view filepath);
};