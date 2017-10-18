## GetOpt++
### C++ Library for command-line argument parsing
#### Current version: 0.1.0 - version number frozen

GetOpt++ is a small library for command-line argument parsing. It is
lightweight and performance-focused.

###  [Documentation](http://getoptplusplus.readthedocs.io/en/latest/)

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
	
      
