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

const std::vector<std::string> ConvertArgvToVect(int argc,char **argv);

template<typename T,typename X>
std::set<T> BuildSymTable(std::vector<X> Source, std::function<std::vector<T>(const X)> ExtractVect)
{
	std::set<T> Result;
	for(auto a: Source){
		const std::vector<T> Elements = ExtractVect(a);
		for(auto b:Elements){
			if(Result.insert(b).second == false){
				throw std::logic_error("Arguments have to be unique across the parameters");
			}
		}
	}
	return Result;
}

class Command{
	public:
		struct Definition{
			enum Flags{
				RequireUniqueArguments = (1 << 0),
				RequireUniqueCommand = (1 << 1),
				NoArgumentLimit = (1 << 2),
				AcceptAnyArgument = (1 << 3)
			};
			const std::vector<std::string> ValidNames;
			const std::vector<std::string> AcceptedArguments;
			const unsigned MinArgCount;
			const int MaxArgCount;//Change to unsigned after implementing flags.
			Flags Flags;//Not implemented yet.
			bool operator==(const Definition Argument) const;
			inline bool HasName(const std::string& CandName) const;
			inline bool HasArgument(const std::string& CandArg) const;
		};
		Command(const unsigned index,const Definition Parameters);
		inline const Definition GetProperties(void) const;
		inline const unsigned GetIndex(void) const;
		inline const std::vector<std::string> GetArguments(void) const;
		inline bool HasName(const std::string& CandName) const;
		inline bool HasArgument(const std::string& CandArg) const;
		bool operator[](const std::string Argument) const;
		bool operator==(std::string Argument) const;
	private:
		inline bool HasValidArgCount(void) const;
		void AddArgument(const std::string Argument);
		const unsigned Index = 0;
		const Definition Properties;
		std::vector<std::string> Arguments;
		friend Parser;
};

class Parser{
	public:
		Parser(int argc, char** argv,
			const std::initializer_list<Command::Definition> CommandParameters);
		Parser(const std::vector<std::string> Parameters,
			const std::vector<Command::Definition> CommandParameters);
		Parser(const std::initializer_list<std::string> Parameters,const std::initializer_list<Command::Definition> CommandParameters);
		inline const std::vector<std::string> GetArguments(void) const;
		inline const std::vector<Command> GetCommands(void) const; 
		inline const std::string GetProgramName(void) const;
		bool operator[](std::string Argument) const;
	private:
		bool AddCommand(const std::string Argument, unsigned Indice,
				const std::vector<Command::Definition> CommandParameters);
		void Parse(const std::vector<std::string> Arguments, 
				const std::vector<Command::Definition> CommandParameters);
		std::vector<Command> Commands;//Replace shared_ptr with plain variable?
		std::vector<std::string> Arguments;
		std::set<std::string> ArgSymTable;
		std::set<std::string> ParSymTable;
		std::string ProgramName;
};

inline bool Command::Definition::HasName(const std::string& CandName) const
{
	return std::find(ValidNames.begin(),ValidNames.end(),CandName) != ValidNames.end();
}

inline bool Command::Definition::HasArgument(const std::string& CandArg) const
{
	return std::find(AcceptedArguments.begin(),AcceptedArguments.end(),CandArg)
		!= AcceptedArguments.end();
}

inline const std::vector<std::string> Parser::GetArguments(void) const
{
	return Arguments;
}

inline const std::vector<Command> Parser::GetCommands(void) const
{
	return Commands;
}

inline const std::string Parser::GetProgramName(void) const
{
	return ProgramName;
}

inline bool Command::HasArgument(const std::string& CandArg) const
{
	return std::find(Arguments.begin(),Arguments.end(),CandArg) != Arguments.end();
}

inline const Command::Definition Command::GetProperties(void) const
{
	return Properties;
}

inline const unsigned Command::GetIndex(void) const
{
	return Index;
}

inline const std::vector<std::string> Command::GetArguments(void) const
{
	return Arguments;
}

inline bool Command::HasValidArgCount(void) const
{
	return (Arguments.size() <= Properties.MaxArgCount || (Properties.Flags | Command::Definition::NoArgumentLimit) != 0)
		&& Arguments.size() >= Properties.MinArgCount;
}

}

#endif
