#include <iostream>
#include <windows.h>
#include <thread>
#include <random>
#include <chrono>
#include <vector>
#include <numeric>
#include "Timer.cpp"
using namespace std;

class Total
{
public:	
	int _N;//number of threads
	int _size;//size of array
	vector<int>vec = vector<int>(_size);
	vector<thread> threads;
	int number = _size/_N;//size of one part	
	int _total = 0;
	int _p = 0;

	Total(int N, int M) : _N(N), _size(M)
	{		
		
		srand(static_cast<unsigned int>(time(0)));
		for (int i = 0; i < _size; i++)
		{
			vec[i] = rand()%10;			
		}
	}
	

	int with_threads()
	{		
		for (int i = 0; i < _N; i++)
		{		
			thread t(&Total::calc_part, this, i);
			threads.push_back(std::move(t));           
		}

		for (auto& t : threads)
		{
			if (t.joinable())
				t.join();
		}
	 return _total;
	}

    int no_threads()
    {
	 return accumulate((begin(vec)), (end(vec)), 0);
    }

	void calc_part(int i)
	{
		_p = accumulate((begin(vec) + (i * number)), (begin(vec) + ((i + 1) * number)), 0);
		_total += _p;	
	}

};

int main()
{
	int N = 4;// num of parts
	int M = 10000000;// size of array
	int T = 0;
	if (M % N)
	{
		cout << "Error of condition" << endl;
		exit(1);
	}

	Total total(N, M);	
	Timer t1;//strart
	T = total.no_threads();
	cout << "Without threads the total = " << T << "\tTime elapsed: " << t1.elapsed() << '\n';//finish

	//Total total2(N, M);	
	Timer t2;//strart
	T = total.with_threads();
	cout << "With " << N << " threads the total = " << T << "\tTime elapsed : " << t2.elapsed() << '\n';//finish	

	return 0;
}//for quad-core processor, M = 10000000 and 8 threads average ratio was 0,202/0,083 s
//             -                             4 threads average ratio was 0,208/0,083 s
//	       -			     2 threads average ratio was 0,193/0,106 s	
