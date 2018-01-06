#include <stdexcept>
#include <getopt++/command_impl.h>
using namespace GOpp;


Command::Command(const unsigned& index,const Command::Definition& Parameters):
		Index(index),Def(Parameters)
{
}

[[deprecated("Confusing, use HasArgument instead")]]
bool Command::operator[](const std::string& Argument) const
{
	//Was given 'Argument' used?
	for(int i = 0 ; i < Arguments.size() ; i++){
		if(Arguments[i] == Argument){
			return true;
		}
	}
	return false;
}

bool Command::operator==(const std::string& Argument) const
{
	return std::find(Def.ValidNames.begin(),Def.ValidNames.end(),Argument) != Def.ValidNames.end();
}

void Command::AddArgument(const std::string& Argument)
{ 
	if((Def.Flags & GOpp::Command::Definition::Flags::RequireUniqueArguments) != 0 && //If requires unique arguments
			std::find(Arguments.begin(),Arguments.end(),Argument) != Arguments.end()){//And given argument has already 
		throw std::invalid_argument("Arguments have to be unique");
	}
	std::size_t i = 0;
	while(Arguments.size() < Def.MaxArgCount ||( Def.Flags & GOpp::Command::Definition::Flags::NoArgumentLimit)){
		if( (Def.Flags & GOpp::Command::Definition::Flags::AcceptAnyArgument) || (i < Def.AcceptedArguments.size() && Argument == Def.AcceptedArguments[i])) {
			Arguments.push_back(Argument);
			return;
		}
		i++;
	}
	throw std::invalid_argument("Invalid argument: " + Argument);
}

bool Command::Definition::operator==(const Command::Definition& Argument) const
{
	return Argument.ValidNames == this->ValidNames &&
		Argument.AcceptedArguments == this->AcceptedArguments &&
		Argument.MinArgCount == this->MinArgCount &&
		Argument.MaxArgCount == this->MaxArgCount;
}

