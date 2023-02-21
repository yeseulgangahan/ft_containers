#include "main.hpp"

int main(int argc, char** argv) {
	if (argc != 2)
	{                                                                                                                                                                                         
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	const int seed = atoi(argv[1]);
	srand(seed);

	/*
	**  vector<Buffer>
	*/
	ft::vector<Buffer> vector_buffer;

	for (int i = 0; i < COUNT; i++)
	{
		vector_buffer.push_back(Buffer());
	}

	for (int i = 0; i < COUNT; i++)
	{
		const int idx = rand() % COUNT; // [0,50) 사이에서 idx 결정
		vector_buffer[idx].idx = 5; // 그곳에 값 5를 넣는다
	}
	ft::vector<Buffer>().swap(vector_buffer); // 임시객체와 swap하면 비게 된다

	try
	{
		for (int i = 0; i < COUNT; i++)
		{
			const int idx = rand() % COUNT;
			vector_buffer.at(idx); // go CATCH
			std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
		}
	}
	catch(const std::exception& e)
	{
		//NORMAL ! :P
	}
	
	/*
	**  map<int, int>
	*/
	ft::map<int, int> map_int;
	
	for (int i = 0; i < COUNT; ++i)
	{
		map_int.insert(ft::make_pair(rand() % 10, rand()));
		// 삽입을 COUNT만큼 반복한다
	}

	int sum = 0;
	for (int i = 0; i < 10000; i++)
	{
		int access = rand() % 10;
		sum += map_int[access];
	}
	std::cout << "should be constant with the same seed: " << sum << std::endl;
	// "같은 seed를 넣었으면 다음의 결과도 일정해야 합니다."

	{
		ft::map<int, int> copy = map_int; // 대입연산자
	}
	
	/*
	**  MutantStack<char>
	** 	stack을 상속받고, iterator 기능을 추가한 사용자정의 class
	*/

	MutantStack<char> iterable_stack;
	for (char letter = 'a'; letter <= 'z'; letter++)
		iterable_stack.push(letter);

	// iterator 동작이 되어야 함
	for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
	{
		std::cout << *it; // a부터 z까지 출력
	}
	std::cout << std::endl;
	return (0);
}