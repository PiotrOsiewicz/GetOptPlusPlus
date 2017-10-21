#include <getopt++.h>
using namespace GOpp;

Parser::Parser(int argc, char** argv,const std::initializer_list<Command::Definition> CommandParameters):
	Parser(ConvertArgvToVect(argc,argv),std::vector<Command::Definition>(CommandParameters.begin(),CommandParameters.end()))
{
}

Parser::Parser(const std::initializer_list<std::string> Parameters,
		const std::initializer_list<Command::Definition> CommandParameters):
	Parser(std::vector<std::string>(Parameters.begin(),Parameters.end()),
			std::vector<Command::Definition>(CommandParameters.begin(),CommandParameters.end()))
{
}

Parser::Parser(const std::vector<std::string> Parameters,
		const std::vector<Command::Definition> CommandParameters)
{
	if(Parameters.size() <= 0) {
		throw std::invalid_argument("Parameters");
	} 
	ProgramName = Parameters[0];
	Arguments = Parameters;
	try{
		ArgSymTable = BuildSymTable<std::string,Command::Definition>(CommandParameters, [](const Command::Definition Def) {return Def.AcceptedArguments; });
		ParSymTable = BuildSymTable<std::string,Command::Definition>(CommandParameters, [](const Command::Definition Def) {return Def.ValidNames; });
	} catch(std::logic_error){
		throw;
	}
	std::set<std::string> Intersection;
	std::set_intersection(ArgSymTable.begin(), ArgSymTable.end(),ParSymTable.begin(),ParSymTable.end(),std::inserter(Intersection,Intersection.begin()));
	
	if(Intersection.size() != 0){//If two sets have common element
		throw std::logic_error("Colliding symbol tables");
	}
	this->Parse(Arguments,CommandParameters);
}

bool Parser::operator[](std::string Argument) const
{
	//Was given 'Argument' used
	for(std::size_t i = 0 ; i < Commands.size() ; i++){
		if(std::find(Commands[i].Properties.ValidNames.begin(),Commands[i].Properties.ValidNames.end(),Argument) != Commands[i].Properties.ValidNames.end() ){
			return true;
		}
	}
	return false;
}

bool Parser::AddCommand(const std::string Argument,unsigned Indice,const std::vector<Command::Definition> CommandParameters)
{
	for(int b = 0 ; b < CommandParameters.size() ; b++){
		if(std::find(CommandParameters[b].ValidNames.begin(),CommandParameters[b].ValidNames.end(), Argument) != CommandParameters[b].ValidNames.end()){
			this->Commands.push_back(Command(Indice,CommandParameters[b]));
			return true;
		}
	}
	return false;
}

void Parser::Parse(const std::vector<std::string> Argv,const std::vector<Command::Definition> CommandParameters)
{
	for(int a = 1;a<Argv.size();a++){
		if(Commands.size() > 0 &&
			(Commands.back().Properties.HasArgument(Argv[a]) ||
			(Commands.back().Properties.Flags | Command::Definition::NoArgumentLimit) != 0 ) &&
			ParSymTable.find(Argv[a]) == ParSymTable.end()) {
			Commands.back().AddArgument(Argv[a]);
		} else if(ParSymTable.find(Argv[a]) != ParSymTable.end()){
			this->AddCommand(Argv[a],a,CommandParameters);
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

Command::Command(const unsigned index,const Command::Definition Parameters):
		Index(index),Properties(Parameters)
{
}

bool Command::operator[](std::string Argument) const
{
	//Was given 'Argument' used?
	for(int i = 0 ; i < Arguments.size() ; i++){
		if(Arguments[i] == Argument){
			return true;
		}
	}
	return false;
}

bool Command::operator==(std::string Argument) const
{
	return std::find(Properties.ValidNames.begin(),Properties.ValidNames.end(),Argument) != Properties.ValidNames.end();
}

void Command::AddArgument(const std::string Argument)
{ 
	if((Properties.Flags | GOpp::Command::Definition::RequireUniqueArguments) != 0 && 
			std::find(Arguments.begin(),Arguments.end(),Argument) != Arguments.end()){
		throw std::invalid_argument("Arguments have to be unique");
	}
	if(Properties.MaxArgCount > 0 && Arguments.size() <= Properties.MaxArgCount){
		for(int c = 0 ; c < Properties.AcceptedArguments.size(); c++){
			if(Argument == Properties.AcceptedArguments[c]){
				Arguments.push_back(Argument);
				return;
			}
		}
	} else if((Properties.Flags | GOpp::Command::Definition::NoArgumentLimit) != 0){
		Arguments.push_back(Argument);
		return;
	}
	throw std::invalid_argument("Invalid argument");
}

bool Command::Definition::operator==(Command::Definition Argument) const
{
	return Argument.ValidNames == this->ValidNames &&
		Argument.AcceptedArguments == this->AcceptedArguments &&
		Argument.MinArgCount == this->MinArgCount &&
		Argument.MaxArgCount == this->MaxArgCount;
}

const std::vector<std::string> GOpp::ConvertArgvToVect(int argc,char **argv)
{
	if(argv == nullptr || argc < 1) {
		throw std::invalid_argument("Argv");
	} 
	
	std::vector<std::string> ArgList;
	for(int i = 0;i<argc;i++){
		if(argv[i] == nullptr){
			throw std::invalid_argument("nullptr");
		}
		std::string temp(argv[i]);
		ArgList.push_back(temp);
	}
	return ArgList;
}

