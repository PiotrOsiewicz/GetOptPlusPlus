#include <stdexcept>
#include <getopt++/command_impl.h>
#include <getopt++/parser_impl.h>
#include <getopt++/misc.h>

using namespace GOpp;

Parser::Parser(const std::vector<Command::Definition>& CommandParameters): CommandDefs(CommandParameters)
{
	try{
		ArgSymTable = BuildSymTable<std::string,Command::Definition>(CommandParameters, [](const Command::Definition Def) {return Def.AcceptedArguments; });
		ParSymTable = BuildSymTable<std::string,Command::Definition>(CommandParameters, [](const Command::Definition Def) {return Def.ValidNames; });
	} catch(std::logic_error e){
		throw e;
	}
	std::set<std::string> Intersection;
	std::set_intersection(ArgSymTable.begin(), ArgSymTable.end(),ParSymTable.begin(),ParSymTable.end(),std::inserter(Intersection,Intersection.begin()));
	
	if(Intersection.size() != 0){//If two sets have common element,
		//then there is name collision
		throw std::logic_error("Colliding symbol tables");
	}
}

[[deprecated("Confusing,use HasCommand instead")]]
bool Parser::operator[](const std::string& Argument) const
{
	//Was given 'Argument' used
	for(std::size_t i = 0 ; i < Commands.size() ; i++){
		if(std::find(Commands[i].Def.ValidNames.begin(),Commands[i].Def.ValidNames.end(),Argument) != Commands[i].Def.ValidNames.end() ){
			return true;
		}
	}
	return false;
}

bool Parser::AddCommand(const std::string& Argument,const unsigned& Indice)
{
	for(int b = 0 ; b < CommandDefs.size() ; b++){
		if(std::find(CommandDefs[b].ValidNames.begin(),CommandDefs[b].ValidNames.end(), Argument) != CommandDefs[b].ValidNames.end()){
			this->Commands.push_back(Command(Indice,CommandDefs[b]));
			return true;
		}
	}
	return false;
}

void Parser::Parse(const std::vector<std::string>& Argv)
{
	for(int a = 0;a<Argv.size();a++){
		if(Commands.size() > 0 &&
			(Commands.back().Def.HasArgument(Argv[a]) ||
			(Commands.back().Def.Flags & Command::Definition::Flags::AcceptAnyArgument) != 0 ) &&
			ParSymTable.find(Argv[a]) == ParSymTable.end()) {
			Commands.back().AddArgument(Argv[a]);
		} else if(ParSymTable.find(Argv[a]) != ParSymTable.end()){//If it was not valid argument, but it's valid Command
			this->AddCommand(Argv[a],a);
		} else{
			throw std::invalid_argument(Argv[a]);
		}
	}
	for(int b  = 0 ; b<Commands.size() ; b++){
		if(Commands[b].HasValidArgCount() == false){
			throw std::invalid_argument("");
		}
	}
}

