/* Compile and run: g++ -std=c++11 -W -Wall -O3 -o main main.cpp && ./main
 */

#include <iostream>
#include <cstdint>

#include "bstree.hpp"

template <class T>
void print(const T& data)
{
	std::cout << data << " ";
}

int main()
{
	bstree<int> bst({18, 11, 8, 5, 9, 16, 13, 78, 65, 90});
	std::cout << "PRE ORDER: \t";
	bst.pre_order(print);
	std::cout << "\n";
	std::cout << "IN ORDER: \t";
	bst.in_order(print);
	std::cout << "\n";
	std::cout << "POS ORDER: \t";
	bst.pos_order(print);
	std::cout << "\n";

	std::cout << bst.find(90) << std::endl;
	std::cout << bst.find(18) << std::endl;
	std::cout << bst.find(19) << std::endl;

	std::cout << "HEIGHT: " << bst.height() << std::endl;

	bst.remove(18);
	bst.remove(78);
	bst.remove(65);
	bst.remove(90);
	bst.remove(16);
	bst.remove(11);
	bst.remove(8);
	bst.remove(13);
	bst.remove(9);
	bst.remove(5);
	bst.in_order(print);
	std::cout << std::endl;

	return 0;
}
