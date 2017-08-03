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
		void Parse(std::vector<std::string> Arguments,std::vector<CommandDefinition> CommandParameters);
		std::vector<std::shared_ptr<Command>> Commands;
		std::vector<std::string> Arguments;
		int ArgCount;
		std::string ProgramName;
};

Parser::Parser(int argc, char** argv,const std::vector<CommandDefinition> CommandParameters):
	Parser(ConvertArgvToVect(argc,argv),CommandParameters)
{
}

Parser::Parser(std::vector<std::string> Parameters,const std::vector<CommandDefinition> CommandParameters):
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

void Parser::Parse(std::vector<std::string> Argv,std::vector<CommandDefinition> CommandParameters)
{
	for(int a = 0;a<Argv.size();a++){
		bool WasUsed = false;
		if(Argv[a] == ProgramName) {
			continue;
		}
		for(int b = 0 ; b < CommandParameters.size() && !WasUsed ; b++){
			if( Argv[a] == CommandParameters[b].LongName || Argv[a] == CommandParameters[b].ShortName){
				std::shared_ptr<Command> Temp(new Command(a,CommandParameters[b]));
				this->Commands.push_back(Temp);
				WasUsed = true;
				break;
			}
		}
		if(WasUsed == false) { 
			for(int b = 0; b<Commands.size() && !WasUsed;b++){
				if(Commands[b]->Config.MaxArgCount > 0){
					for(int c = 0 ; c<Commands[b]->Config.AcceptedArguments.size() && 
							Commands[b]->Arguments.size() < Commands[b]->Config.MaxArgCount; c++){
						if(Argv[a] == Commands[b]->Config.AcceptedArguments[c]){
							Commands[b]->Arguments.push_back(Argv[a]);
							WasUsed = true;
							break;
						}
					}
				} else if(Commands[b]->Config.MaxArgCount == -1){
					WasUsed = true;
					Commands[b]->Arguments.push_back(Argv[a]);
					break;
				}
			}
		}
		if(WasUsed == false) {
			throw Argv[a];
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
