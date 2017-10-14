#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "getopt++.h"

TEST_CASE("main")
{
  std::vector<char*> argv{"test","test2"};
  GOpp::Parser CommandLine(1,argv.data());
}
