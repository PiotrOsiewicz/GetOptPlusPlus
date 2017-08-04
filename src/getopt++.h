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

namespace GOpp{

class Parser; 

static std::vector<std::string> ConvertArgvToVect(int argc,char **argv);

class CommandDefinition{
	public:
		std::string LongName = "";
		std::string ShortName = "";
		int MinArgCount = 0;
		int MaxArgCount = 0;
		const std::vector<std::string> AcceptedArguments = std::vector<std::string>();
};

class Command{
	public:
		const CommandDefinition GetConfig(void) const;
		const int& GetIndex(void) const;
		Command(int index,CommandDefinition Parameters);
	private:
		bool ValidateArgCount(void) const;
		bool AddArgument(std::string Argument);
		int Index = 0;
		const CommandDefinition Config;
		std::vector<std::string> Arguments;
		friend Parser;
};

class Parser{
	public:
		Parser(int argc, char** argv,
			const std::vector<CommandDefinition> CommandParameters = std::vector<CommandDefinition>());
		Parser(std::vector<std::string> Parameters,
			const std::vector<CommandDefinition> CommandParameters = std::vector<CommandDefinition>());
		const int& GetArgCount(void) const;
		const std::vector<std::string>& GetArguments(void) const;
		const std::vector<std::shared_ptr<Command>> GetCommands(void) const; 
		const std::string& GetProgramName(void) const;
	private:
		bool AddCommand(std::string Argument, int indice,
				std::vector<CommandDefinition> CommandParameters);
		void Parse(std::vector<std::string> Arguments, 
				std::vector<CommandDefinition> CommandParameters);
		std::vector<std::shared_ptr<Command>> Commands;
		std::vector<std::string> Arguments;
		int ArgCount;
		std::string ProgramName;
};

Parser::Parser(int argc, char** argv,const std::vector<CommandDefinition> CommandParameters):
	Parser(ConvertArgvToVect(argc,argv),CommandParameters)
{
}

Parser::Parser(std::vector<std::string> Parameters,
		const std::vector<CommandDefinition> CommandParameters):
	ArgCount(Parameters.size())
{
	if(Parameters.size() <= 0) {
		throw std::invalid_argument("Parameters");
	} 
	ProgramName = Parameters[0];
	Arguments = Parameters;
	this->Parse(Arguments,CommandParameters);
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
bool Parser::AddCommand(std::string Argument,int Indice, std::vector<CommandDefinition> CommandParameters)
{
	for(int b = 0 ; b < CommandParameters.size() ; b++){
		if( Argument == CommandParameters[b].LongName ||
				Argument == CommandParameters[b].ShortName){
			std::shared_ptr<Command> Temp(new Command(Indice,CommandParameters[b]));
			this->Commands.push_back(Temp);
			return true;
		}
	}
	return false;
}

void Parser::Parse(std::vector<std::string> Argv,std::vector<CommandDefinition> CommandParameters)
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
			throw Commands[b]->Config.MinArgCount - Commands[b]->Arguments.size();
		}
	}
}

const CommandDefinition Command::GetConfig(void) const
{
	return Config;
}

Command::Command(int index,CommandDefinition Parameters):
		Index(index),Config(Parameters)
{
}

bool Command::AddArgument(std::string Argument)
{
	if(Config.MaxArgCount > 0){
		for(int c = 0 ; c < Config.AcceptedArguments.size() && 
			Arguments.size() < Config.MaxArgCount; c++){
			if(Argument == Config.AcceptedArguments[c]){
				Arguments.push_back(Argument);
				return true;
			}
		}
			
	} else if(Config.MaxArgCount == -1){
		Arguments.push_back(Argument);
		return true;
	}
	return false;
}

bool Command::ValidateArgCount(void) const
{
	return (Config.MaxArgCount == -1 || Arguments.size() <= Config.MaxArgCount)
		&& Arguments.size() >= Config.MinArgCount;
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
