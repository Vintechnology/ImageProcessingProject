#include <iostream>
#include <chrono>
#include "linalg.h"

int main() {
	try {
		linalg::Matrix m{ {-1,0,1},
						  {-2,0,2},
						  {-1,0,1} };

		linalg::Matrix m2{ {1,1,0} };

		auto start = std::chrono::system_clock::now();
		linalg::Matrix r = m + m2;
		auto end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
		std::cout << r << std::endl;
		std::cout << "Time passed: " << elapsed.count()*1e-9 <<" seconds"<< std::endl;
	}
	catch (std::runtime_error ex) {
		std::cout << "[RUNTIME ERROR]: " << ex.what() << std::endl;
	}
	system("pause");
	return 0;
}