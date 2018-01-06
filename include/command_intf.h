#ifndef GetOptPlusPlus_Command_INTF
#define GetOptPlusPlus_Command_INTF
#include <string>
#include <vector>
#include "command_fwd.h"
#include "parser_fwd.h"
namespace GOpp{
class Command{
	public:
		struct Definition{
			struct Flags{
				static const unsigned RequireUniqueArguments = (1 << 0);
				static const unsigned NoArgumentLimit = (1 << 1);
				static const unsigned AcceptAnyArgument = (1 << 2);
			};
			const std::vector<std::string> ValidNames;
			const std::vector<std::string> AcceptedArguments;
			const unsigned MinArgCount;
			const unsigned MaxArgCount;//Change to unsigned after implementing flags.
			unsigned Flags;//Not implemented yet.
			bool operator==(const Definition& Argument) const;
			inline bool HasName(const std::string& CandName) const;
			inline bool HasArgument(const std::string& CandArg) const;
		};
		Command(const unsigned& index,const Definition& Parameters);
		inline const Definition& GetDefinition(void) const;
		inline const unsigned GetIndex(void) const;
		inline const std::vector<std::string>& GetArguments(void) const;
		inline bool HasName(const std::string& CandName) const;
		inline bool HasArgument(const std::string& CandArg) const;
		bool operator[](const std::string& Argument) const;
		bool operator==(const std::string& Argument) const;
	private:
		inline bool HasValidArgCount(void) const;
		void AddArgument(const std::string& Argument);
		const unsigned Index = 0;
		const Definition Def;
		std::vector<std::string> Arguments;
		friend Parser;
};
}
#endif
