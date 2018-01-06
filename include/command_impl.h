#ifndef GetOptPlusPlus_Command_IMPL
#define GetOptPlusPlus_Command_IMPL
#include <algorithm>
#include "command_intf.h"
namespace GOpp{
inline bool Command::Definition::HasName(const std::string& CandName) const
{
	return std::move(std::find(ValidNames.begin(),ValidNames.end(),CandName) != ValidNames.end());
}

inline bool Command::Definition::HasArgument(const std::string& CandArg) const
{
	return std::move(std::find(AcceptedArguments.begin(),AcceptedArguments.end(),CandArg)
		!= AcceptedArguments.end());
}
inline bool Command::HasArgument(const std::string& CandArg) const
{
	return std::find(Arguments.begin(),Arguments.end(),CandArg) != Arguments.end();
}

inline const Command::Definition& Command::GetDefinition(void) const
{
	return Def;
}

//[[deprecated("Code smell, Command knows its index..")]]
inline const unsigned Command::GetIndex(void) const
{
	return std::move(Index);
}

inline const std::vector<std::string>& Command::GetArguments(void) const
{
	return std::move(Arguments);
}

inline bool Command::HasValidArgCount(void) const
{
	return std::move((Arguments.size() <= Def.MaxArgCount ||
		(Def.Flags | Command::Definition::Flags::NoArgumentLimit) != 0) && Arguments.size() >= Def.MinArgCount);
}
}
#endif
