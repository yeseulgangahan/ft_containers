#if 1 //CREATE A REAL STL EXAMPLE
	#include <vector>
	#include <map>
	#include <stack>
	namespace ft = std;
#else
	#include <vector.hpp>
	// #include <map.hpp>
	// #include <stack.hpp>
#endif

#include <iostream>
#include <string>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096

namespace my
{
	struct Buffer
	{
		int idx;
		char buff[BUFFER_SIZE];
	};

	class Class
	{
		private:
			int *_a;
		public:
		Class() : _a(new int(42)) {}
		~Class() { delete _a; }
	};
}

// COUNT: the maximum number of my::Buffer
#define COUNT (MAX_RAM / (int)sizeof(my::Buffer))

template <typename _Type>
void test_vector_with() {
	{
		ft::vector<_Type> v;
		std::allocator<_Type> a;
		ft::vector<_Type> v2(a);
		ft::vector<_Type> v3(10, _Type());
		ft::vector<_Type> v4(v);
	}
}

void test_vector(std::string testname) {
	std::cout << testname << std::endl;

	test_vector_with<int>();
	test_vector_with<double>();
	test_vector_with<std::string>();
	test_vector_with<my::Buffer>();
	test_vector_with<my::Class>();
}

int main(int argc, char** argv) {
	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	const int seed = atoi(argv[1]);
	srand(seed); // rand()로 랜덤한 숫자를 받을 수 있도록 설정

	test_vector("\n\n *  TEST VECTOR  * ");
	std::cout << std::endl;
	return (0);
}
