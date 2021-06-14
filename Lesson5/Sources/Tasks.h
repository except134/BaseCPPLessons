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
	void Task1MassiveWriter(double* var, size_t sz);
	void Task2MassiveReverser(int* var, size_t sz);
	void Task3MassiveFiller(int* var, size_t sz);
	void Task4MassiveSlider(int* var, size_t sz, int slide);
	bool Task5MassiveBalancer(int* var, size_t sz);
};