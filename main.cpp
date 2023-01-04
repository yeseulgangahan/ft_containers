//#if 1 //CREATE A REAL STL EXAMPLE
//    #include <vector>
//    #include <map>
//    #include <stack>
//    namespace ft = std;
//#else
//    #include <vector.hpp>
//    // #include <map.hpp>
//    // #include <stack.hpp>
//#endif

#include "vector.hpp"
#include <iostream>

int  main()
{
  ft::vector<int> v;

  v.push_back(42);
  std::cout << v.at(0) << std::endl;
}



//#include <iostream>
//#include <string>

//#define MAX_RAM 4294967296
//#define BUFFER_SIZE 4096

//namespace my
//{
//  struct Buffer
//  {
//      int data;
//      char buff[BUFFER_SIZE];
//  };

//  class Class
//  {
//    private:
//        int data;
//        char *buff;
//    public:
//    Class() : buff(new char()) {}
//    ~Class() { delete buff; }
//  };
//}

//// COUNT: the maximum number of my::Buffer
//#define COUNT (MAX_RAM / (int)sizeof(my::Buffer))

//template <typename _Type>
//void test_vector_with() {

//  {
//    /* CONSTRUCTOR, DESTRUCTOR */

//    ft::vector<_Type> v;
    
//    std::allocator<_Type> a;
//    ft::vector<_Type> v2(a);

//    ft::vector<_Type> v3(10, _Type());
    
//    ft::vector<_Type>::iterator iter = v.begin();
//    ft::vector<_Type>::iterator iter2 = ++v.begin();
//    ft::vector<_Type> v4(v3.begin(), ++v3.begin());

//    ft::vector<_Type> v5(v);
//    if (v5 != v)
//      std::cout << "Error: copy constructor failed" << std::endl;

//    ft::vector<_Type> v6 = v;
//    if (v6 != v)
//      std::cout << "Error: assignement failed" << std::endl;
//  }
//  {
//    /* CAPACITY */


//  }
//  { /* PUSH_BACK, SWAP */
//    ft::vector<_Type> v;
//    try
//    {
//      for (int i = 0; i < COUNT + 1; i++)
//      {
//        v.push_back(_Type());
//      }
//    }
//    catch(const std::exception& e)
//    {
//      std::cerr << "push_back() exception is thrown" << std::endl;
//    }

//    for (int i = 0; i < COUNT; i++)
//    {
//      const int idx = rand() % COUNT;
//      v[idx].data = 5;
//    }
//    ft::vector<_Type>().swap(v);

//    try
//    {
//      for (int i = 0; i < COUNT; i++)
//      {
//        const int idx = rand() % COUNT;
//        v.at(idx);
//        std::cerr << "Error: swap() is not working!!" <<std::endl;
//      }
//    }
//    catch(const std::exception& e) {}
//  }
//  {/* INSERT, ERASE, POP_BACK */
//    ft::vector<_Type> v;
    

//  }
//  { /* ASSIGN, POP_BACK */
//    std::vector<_Type> v1;
//    std::vector<_Type> v2;
//    std::vector<_Type> v3;

//    v1.assign (COUNT,_Type()); 
//    try
//    {
//      for (int i = 0; i < COUNT; i++)
//      {
//        const int idx = rand() % COUNT;
//        v1.at(idx);
//      }
//    }
//    catch(const std::exception& e)
//    {
//        std::cerr << "Error: assign()-fill is not working!!" <<std::endl;
//    }

//    v2.assign (v1.begin() + 1, v1.end() - 1);
//    try
//    {
//      for (int i = 0; i < COUNT - 2; i++)
//      {
//        const int idx = rand() % COUNT - 2;
//        v2.at(idx);
//      }
//    }
//    catch(const std::exception& e)
//    {
//        std::cerr << "Error: assign()-range is not working!!" <<std::endl;
//    }

//    try
//    {
//      int count = COUNT - 2;
//      while (!v2.empty())
//      {
//        v2.pop_back();
//        count--;
//      }
//      if (count != 0)
//        throw std::exception();
//    }
//    catch(const std::exception& e)
//    {
//      std::cerr << "Error: pop_back() is not working!!" << std::endl;
//    }
//  }
//}

//void test_vector(std::string testname) {
//  std::cout << testname << std::endl;

//  test_vector_with<int>();
//  //test_vector_with<my::Buffer>();
//  //test_vector_with<my::Class>();
//}

//int main(int argc, char** argv) {
//  if (argc != 2)
//  {
//      std::cerr << "Usage: ./test seed" << std::endl;
//      std::cerr << "Provide a seed please" << std::endl;
//      std::cerr << "Count value:" << COUNT << std::endl;
//      return 1;
//  }
//  const int seed = atoi(argv[1]);
//  srand(seed); // rand()로 랜덤한 숫자를 받을 수 있도록 설정

//  test_vector("\n\n *  TEST VECTOR  * ");
//  std::cout << std::endl;
//  return (0);
//}
