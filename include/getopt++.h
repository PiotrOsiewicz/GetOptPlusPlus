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
#include <algorithm>
#include <set>
#include <functional>

namespace GOpp{

class Parser; 

std::vector<std::string> ConvertArgvToVect(int argc,char **argv);

template<typename T,typename X>
std::set<T> BuildSymTable(std::vector<X> Source, std::function<std::vector<T>(X)> ExtractVect)
{
	std::set<T> Result;
	for(auto a: Source){
		std::vector<T> Elements = ExtractVect(a);
		for(auto b:Elements){
			if(Result.insert(b).second == false){
				throw std::logic_error("Arguments have to be unique across the parameters");
			}
		}
	}
	return Result;
}

template<typename T>
bool AreSymTablesColliding(std::set<T> First,std::set<T> Second)
{
	for(auto a: First){
		if(Second.find(a) != Second.end()){
			return true;
		}
	}
	return false;
}

class Command{
	public:
		struct Definition{
			std::vector<std::string> ValidNames;
			std::vector<std::string> AcceptedArguments;
			unsigned MinArgCount;
			int MaxArgCount;
			enum class Flags{
				RequireUniqueArguments = (1 << 0),
				NoArgumentLimit = (1 << 1),
				RequireUniqueCommand = (1 << 2)
			};
			Flags Flags;//Not implemented yet.
			bool operator==(std::string Argument) const;
		};
		Command(int index,Definition Parameters);
		const Definition& GetProperties(void) const;
		const int& GetIndex(void) const;
		const std::vector<std::string>& GetArguments(void) const;
		bool HasName(const std::string& CandName) const;
		bool HasArgument(const std::string& CandArg) const;
		bool operator[](const std::string Argument) const;
		bool operator==(std::string Argument) const;
	private:
		bool HasValidArgCount(void) const;

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
		const std::vector<std::string>& GetArguments(void) const;
		const std::vector<std::shared_ptr<Command>> GetCommands(void) const; 
		const std::string GetProgramName(void) const;
		bool operator[](std::string Argument) const;
	private:
		bool AddCommand(std::string Argument, int indice,
				std::vector<Command::Definition> CommandParameters);
		void Parse(std::vector<std::string> Arguments, 
				std::vector<Command::Definition> CommandParameters);
		std::vector<std::shared_ptr<Command>> Commands;//Replace shared_ptr with plain variable?
		std::vector<std::string> Arguments;
		std::set<std::string> ArgSymTable;
		std::set<std::string> ParSymTable;
		std::shared_ptr<Command> LastUsedCommand = nullptr;
		std::string ProgramName;
};

}

#endif
