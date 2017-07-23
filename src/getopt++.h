/* GetOpt++ - a lightweight parser for command-line arguments.
 * Author: Piotr Osiewicz
 * License: MIT
 * Usage: Include this file in your project and initialize GOppParser
 */

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

class Argument{
	public:
		const std::string& GetName(void) const;
		const int& GetIndex(void) const;
		Argument(int index,std::string n_Name): Index(index), Name(n_Name) {};
	private:
		int Index; 
		std::string Name;
	
};

class Command: public Argument{
	public:
		const std::string& GetName(void) const;
		const int& GetIndex(void) const;
		virtual void Execute(void) = 0;
	private:
		int MinArgCnt = 0;
		int MaxArgCnt = 0;
		int Index;
		std::string Name;
		std::vector<Argument*> Arguments;
		std::vector<std::string> AcceptedArguments;
};

class GOppParser{
	public:
		GOppParser(int argc, char** argv);
		~GOppParser();
		const int& GetArgCnt(void) const;
		const std::vector<Argument*>& GetArgumentList(void) const;
		const std::vector<Command*>& GetCommandList(void) const; 
		const std::string& GetProgramName(void) const;
	private:
		void Parse(std::vector<Argument> Argv);
		std::vector<Command*> CommandList;
		std::vector<Argument*> ArgumentList;
		int ArgCnt;
		std::string ProgramName;
};

const std::string& Argument::GetName(void) const
{
	return Name;
}

const int& Argument::GetIndex(void) const
{
	return Index;
}

const int& GOppParser::GetArgCnt(void) const
{
	return ArgCnt;
}

const std::vector<Argument*>& GOppParser::GetArgumentList(void) const
{
	return ArgumentList;
}
const std::vector<Command*>& GOppParser::GetCommandList(void) const
{
	return CommandList;
}

const std::string& GOppParser::GetProgramName(void) const
{
	return ProgramName;
}

GOppParser::GOppParser(int argc, char **argv): ArgCnt(argc)
{
	if(argv == nullptr && argc < 1) {
		throw std::invalid_argument("Argv");
	} 
	ProgramName = std::string(argv[0]);
	std::vector<Argument*> ArgList(ArgCnt);
	for(int i = 1;i<argc;i++){
		std::string temp(argv[i]);
		Argument *A = new Argument(i,temp);
		ArgList.push_back(A);
	}
	ArgumentList = ArgList;
}

GOppParser::~GOppParser()
{
	for(int i = 0;i < ArgumentList.size(); i++){
		delete ArgumentList[i];
	}
}
void GOppParser::Parse(std::vector<Argument> Argv)
{

}
