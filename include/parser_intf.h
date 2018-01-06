#ifndef GetOptPlusPlus_Parser_INTF
#define GetOptPlusPlus_Parser_INTF
#include <set>
#include "command_fwd.h"
#include "parser_fwd.h"
namespace GOpp{
class Parser{
	public:
		Parser(const std::vector<Command::Definition> &CommandParameters);
		inline const std::vector<Command>& GetCommands(void) const; 
		bool operator[](const std::string& Argument) const;
		void Parse(const std::vector<std::string>& Arguments);
	private:
		bool AddCommand(const std::string& Argument, const unsigned& Indice);
		std::vector<Command> Commands;
		std::set<std::string> ArgSymTable;
		std::set<std::string> ParSymTable;
		const std::vector<Command::Definition> CommandDefs;
};
}

#endif
