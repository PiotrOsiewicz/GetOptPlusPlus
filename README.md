![GetOpt++ Logo](/getoptpp_logo.png)

### C++ Library for command-line argument parsing
#### Current version: 0.1.0 - version number frozen

GetOpt++ is a small library for command-line argument parsing. It is
lightweight and performance-focused.

[Documentation - under construction](http://getoptplusplus.readthedocs.io/en/latest/)  

[![codecov](https://codecov.io/gh/PiotrOsiewicz/GetOptPlusPlus/branch/master/graph/badge.svg)](https://codecov.io/gh/PiotrOsiewicz/GetOptPlusPlus)
[![BuildStatus](https://travis-ci.org/PiotrOsiewicz/GetOptPlusPlus.svg?branch=master)](https://travis-ci.org/PiotrOsiewicz/GetOptPlusPlus)

### Usage

Include getopt++.h in your source file and construct the GOppParser with argc
as first parameter and argv as second parameter.  


    GOpp::Parser CommandLine(argc,argv,{{{PossibleNames},{AcceptedArguments},MinArgCnt,MaxArgCnt}}}).  


### FAQ
#### Thrown exceptions:  
	std::invalid_argument:
	1. null argv or empty vector passed to Parser constructor.  
	2. Unrecognized arguments.
	std::logic_error:
	1. Parameter and argument names collide during parsing.
This list is incomplete.
	
*Font* used in our glorious logo is [Lato](www.latofonts.com/lato-free-fonts/)
by Łukasz Dziedzic.

