/* GetOpt++ - a lightweight parser for command-line arguments.
 * Author: Piotr Osiewicz
 * License: MIT
 * Usage: Include this file in your project and initialize Parser
 */
#ifndef GetOptPlusPlus
#define GetOptPlusPlus

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <initializer_list>

namespace GOpp{

class Parser; 

static std::vector<std::string> ConvertArgvToVect(int argc,char **argv);

class Command{
	public:
		struct Definition{
			std::string LongName;
			std::string ShortName;
			int MinArgCount;
			int MaxArgCount;
			std::vector<std::string> AcceptedArguments;
			bool operator==(std::string Argument) const;
		};
		const Command::Definition GetProperties(void) const;
		const int& GetIndex(void) const;
		const std::vector<std::string> GetArguments(void) const;
		Command(int index,Definition Parameters);
		bool operator[](std::string Argument) const;
		bool operator==(std::string Argument) const;
	private:
		bool ValidateArgCount(void) const;
		bool AddArgument(std::string Argument);
		int Index = 0;
		const Definition Properties;
		std::vector<std::string> Arguments;
		friend Parser;
};



class Parser{
	public:
		Parser(int argc, char** argv,
			const std::initializer_list<Command::Definition> CommandParameters = std::initializer_list<Command::Definition>());
		Parser(std::vector<std::string> Parameters,
			const std::vector<Command::Definition> CommandParameters = std::vector<Command::Definition>());
		Parser(const std::initializer_list<std::string> Parameters,const std::initializer_list<Command::Definition> CommandParameters);
		const int& GetArgCount(void) const;
		const std::vector<std::string>& GetArguments(void) const;
		const std::vector<std::shared_ptr<Command>> GetCommands(void) const; 
		const std::string& GetProgramName(void) const;
		bool operator[](std::string Argument) const;
	private:
		bool AddCommand(std::string Argument, int indice,
				std::vector<Command::Definition> CommandParameters);
		void Parse(std::vector<std::string> Arguments, 
				std::vector<Command::Definition> CommandParameters);
		std::vector<std::shared_ptr<Command>> Commands;
		std::vector<std::string> Arguments;
		int ArgCount;
		std::string ProgramName;
};

Parser::Parser(int argc, char** argv,const std::initializer_list<Command::Definition> CommandParameters):
	Parser(ConvertArgvToVect(argc,argv),std::vector<Command::Definition>(CommandParameters.begin(),CommandParameters.end()))
{
}

Parser::Parser(std::initializer_list<std::string> Parameters,
		const std::initializer_list<Command::Definition> CommandParameters):
	Parser(std::vector<std::string>(Parameters.begin(),Parameters.end()),
			std::vector<Command::Definition>(CommandParameters.begin(),CommandParameters.end()))
{
}

Parser::Parser(std::vector<std::string> Parameters,
		const std::vector<Command::Definition> CommandParameters):
	ArgCount(Parameters.size())
{
	if(Parameters.size() <= 0) {
		throw std::invalid_argument("Parameters");
	} 
	ProgramName = Parameters[0];
	Arguments = Parameters;
	this->Parse(Arguments,std::vector<Command::Definition>(CommandParameters.begin(),CommandParameters.end()));
}

const int& Parser::GetArgCount(void) const
{
	return ArgCount;
}

const std::vector<std::string>& Parser::GetArguments(void) const
{
	return Arguments;
}
const std::vector<std::shared_ptr<Command>> Parser::GetCommands(void) const
{
	return Commands;
}

const std::string& Parser::GetProgramName(void) const
{
	return ProgramName;
}

bool Parser::operator[](std::string Argument) const
{
	for(int i = 0 ; i < Commands.size() ; i++){
		if(Commands[i]->Properties ==  Argument ){
			return true;
		}
	}
	return false;
}

bool Parser::AddCommand(std::string Argument,int Indice, std::vector<Command::Definition> CommandParameters)
{
	for(int b = 0 ; b < CommandParameters.size() ; b++){
		if(CommandParameters[b] == Argument){
			std::shared_ptr<Command> Temp(new Command(Indice,CommandParameters[b]));
			this->Commands.push_back(Temp);
			return true;
		}
	}
	return false;
}

void Parser::Parse(std::vector<std::string> Argv,std::vector<Command::Definition> CommandParameters)
{
	for(int a = 1;a<Argv.size();a++){
		bool WasUsed = this->AddCommand(Argv[a],a,CommandParameters);
		if(WasUsed == false) { 
			for(int b = 0; b<Commands.size() && !WasUsed;b++){
				WasUsed = Commands[b]->AddArgument(Argv[a]);
			}
		}
		if(WasUsed == false) {
			throw Argv[a];
		}
	}
	for(int b  = 0 ; b<Commands.size() ; b++){
		if(Commands[b]->ValidateArgCount() == false){
			throw Commands[b]->Properties.MinArgCount - Commands[b]->Arguments.size();
		}
	}
}

const Command::Definition Command::GetProperties(void) const
{
	return Properties;
}

const int& Command::GetIndex(void) const
{
	return Index;
}

const std::vector<std::string> Command::GetArguments(void) const
{
	return Arguments;
}

Command::Command(int index,Command::Definition Parameters):
		Index(index),Properties(Parameters)
{
}

bool Command::operator[](std::string Argument) const
{
	for(int i = 0 ; i < Arguments.size() ; i++){
		if(Arguments[i] == Argument){
			return true;
		}
	}
	return false;
}

bool Command::operator==(std::string Argument) const
{
	return Properties ==  Argument;
}

bool Command::AddArgument(std::string Argument)
{
	if(Properties.MaxArgCount > 0){
		for(int c = 0 ; c < Properties.AcceptedArguments.size() && 
			Arguments.size() < Properties.MaxArgCount; c++){
			if(Argument == Properties.AcceptedArguments[c]){
				Arguments.push_back(Argument);
				return true;
			}
		}
			
	} else if(Properties.MaxArgCount == -1){
		Arguments.push_back(Argument);
		return true;
	}
	return false;
}

bool Command::ValidateArgCount(void) const
{
	return (Properties.MaxArgCount == -1 || Arguments.size() <= Properties.MaxArgCount)
		&& Arguments.size() >= Properties.MinArgCount;
}

bool Command::Definition::operator==(std::string Argument) const
{
	return LongName == Argument || ShortName == Argument;
}

static std::vector<std::string> ConvertArgvToVect(int argc,char **argv)
{
	if(argv == nullptr || argc < 1) {
		throw std::invalid_argument("Argv");
	} 
	std::vector<std::string> ArgList;
	for(int i = 0;i<argc;i++){
		std::string temp(argv[i]);
		ArgList.push_back(temp);
	}
	return ArgList;
}
}

#endif
