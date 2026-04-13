#include "gtest/gtest.h"


#include <iostream>


int main(int argc, char **argv) {  

  testing::InitGoogleTest(&argc, argv);  
  std::cout << "Running main() from gtest_main.cc";  
  
  return RUN_ALL_TESTS();  
} 
