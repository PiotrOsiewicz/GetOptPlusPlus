#ifndef GetOptPlusPlus_Parser_IMPL
#define GetOptPlusPlus_Parser_IMPL
#include "parser_intf.h"
namespace GOpp{
inline const std::vector<Command>& Parser::GetCommands(void) const
{
	return Commands;
}

}
#endif
