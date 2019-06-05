#include <iostream>
#include <chrono>
#include "linalg.h"

int main() {
	try {
		linalg::Matrix m{ {-1,0,1},
						  {-2,0,2},
						  {-1,0,1} };

		linalg::Matrix m2{ {1,1,0} };

		linalg::Matrix matrix(10, linalg::Vector(10));

		std::cout << linalg::shape(matrix) << std::endl;
	}
	catch (std::runtime_error ex) {
		std::cout << "[RUNTIME ERROR]: " << ex.what() << std::endl;
	}
	system("pause");
	return 0;
}