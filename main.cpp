#include <cstdlib>
#include <ctime>
#include <iostream>	
#include <chrono>
#include <thread>
#include <vector>
#ifndef _SUMARRAYH_
#define _SUMARRAYH_	

using namespace std::chrono;

using namespace std;


class SumArray
{
private:
	int size;
	int num_threads;
	vector<int>values;
	int result;
	void calc_sum_part(int l_index, int r_index, int index);

public:
	SumArray(int _size, int _num_threads);
	~SumArray();
	int calculate_using_threads();
	int calculate_without_threads();
	void printArray();
};


SumArray::SumArray(int _size, int _num_threads) : size(_size), num_threads(_num_threads)
{
	const int min_value = 0;
	const int max_value = 10;
	srand(time(0));
	for (int i = 0; i < size; i++)
	{
		values.push_back(min_value + rand() % (max_value - min_value + 1));
	}
}

SumArray::~SumArray()
{

}

void SumArray::printArray()
{
	cout << "Array:" << endl;
	for (int i = 0; i < size; i++)
	cout << values[i] << " ";
	cout << "\n" << endl;
}

void SumArray::calc_sum_part(int l_index, int r_index, int index)
{
	int s = 0;
	while (l_index <= r_index)
	{
		s += values[l_index++];
	}
	result += s;
}

int SumArray::calculate_without_threads()
{
	int s = 0;
	for (int i = 0; i < values.size(); i++)
	{
		s += values[i];
	}
	return s;
}

int SumArray::calculate_using_threads()
{
	vector<thread> threads;
	int l_index = 0;
	int r_index = 0;
	int elements_count = size / num_threads;
	result = 0;

	for (int i = 0; i < num_threads; i++)
	{
		if (i == num_threads - 1)
			r_index = values.size();
		else
			r_index += elements_count;

		thread t(&SumArray::calc_sum_part, this, l_index, r_index - 1, i);

		threads.push_back(move(t));
		l_index += elements_count;
	}

	for (auto& t : threads)
	{
		if (t.joinable())
			t.join();

	}
	return result;
}


int main()
{
	const int array_size = 100000;
	const int number_threads = 4;
	SumArray sum(array_size, number_threads);

	auto t_start = system_clock::now();
	int s = sum.calculate_using_threads();
	auto t_end = system_clock::now();
	long diff = duration_cast<milliseconds>(t_end - t_start).count();
	cout << "SUM (using threads: " << number_threads << ") is " << s << " Time (ms): " <<
		diff << endl;

	t_start = system_clock::now();
	s = sum.calculate_without_threads();
	t_end = system_clock::now();
	diff = duration_cast<milliseconds>(t_end - t_start).count();
	cout << "SUM (main thread) is " << s << " Time (ms): " << diff << endl;

	return 0;
}


#endif