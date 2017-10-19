#include <getopt++.h>
#include <criterion/criterion.h>

Test(Parser,NoArgs)
{
	cr_assert_throw(GOpp::Parser Parser({},{}),std::invalid_argument);
}

Test(Parser,ClashingNames)
{
	cr_assert_throw(GOpp::Parser Parser({"./test","-test","-l","-test"},
				{{{"-test"},{},0,1},{{"-l"},{"-test"},0,1}}),std::logic_error);
}

Test(Parser,InvalidCountTooLittle)
{
	cr_assert_throw(GOpp::Parser Parser({"./test","-test"},{{{"-test"},{"param"},1,1}}),std::invalid_argument);
}

Test(Parser,InvalidCountTooMuch)
{
	cr_assert_throw(GOpp::Parser Parser({"./test","-test","param","param"},{{{"-test"},{"param"},1,1}}),std::invalid_argument);
}

Test(Parser,ValidCount)
{
	cr_assert_throw(GOpp::Parser Parser({"./test","-test","param"},{{{"-test"},{"param"},1,1}}),std::invalid_argument);
}

Test(Parser,UnlimitedArgs)
{
	cr_assert_none_throw(GOpp::Parser Parser({"./test","-hey","toy","testing"},{{{"-hey"},{},0,-1}}));
}

Test(Parser,NoParameter)
{
	cr_assert_throw(GOpp::Parser Parser({"./test","param"},{{{"-test"},{"param"},1,1}}),std::invalid_argument);
}

Test(Parser,Rubbish)
{
	cr_assert_throw(GOpp::Parser Parser({"./test","rubbishstuffhere"},{{{"-NotrubibishStuff"},{},0,1}}),std::invalid_argument);
}

Test(Constructor,InitializesProperly)
{
	GOpp::Parser Parser({"./test"},{{}});
	cr_assert(Parser.GetArguments().size() == 1);
	cr_assert(Parser.GetCommands().size() == 0);
	cr_assert(Parser.GetProgramName() == "./test");
	cr_assert(Parser.GetProgramName() == Parser.GetArguments()[0]);
	cr_assert(Parser["test"] == false);
}

Test(Constructor,InitializesCommandsProperly)
{
	GOpp::Parser Parser({"./test","-l","pl-pl"},{{{"-l"},{"pl-pl"},0,1}});
	cr_assert(Parser.GetArguments().size() == 3);
	cr_assert(Parser.GetCommands().size() == 1);
	cr_assert(Parser.GetProgramName() == "./test");
	cr_assert(Parser.GetProgramName() == Parser.GetArguments()[0]);
	cr_assert(Parser.GetCommands()[0]->GetIndex() == 1);
	cr_assert(Parser.GetCommands()[0]->HasName("-l") == true);
	cr_assert(Parser["-l"] == true);
	cr_assert((*Parser.GetCommands()[0]) == "-l");
	cr_assert((*Parser.GetCommands()[0])["pl-pl"] == true);
	cr_assert((*Parser.GetCommands()[0])["en-gb"] == false);
	cr_assert((*Parser.GetCommands()[0]).GetArguments().size() == 1);
}

Test(Constructor,CconstructorInitializesProperly)
{
	const int Size = 3;
	const int MaxLength = 6;
	char **Argv = new char*[Size];
	for(int i = 0 ; i < Size; i++){
		Argv[i] = new char[MaxLength+1];
		Argv[i][MaxLength] = '\0';
	}
	std::strcpy(Argv[0],"./test");
	std::strcpy(Argv[1],"-l");
	std::strcpy(Argv[2],"pl-pl");
	GOpp::Parser Parser(Size,Argv,{{{"-l"},{"pl-pl"},0,1}});
	cr_assert(Parser.GetArguments().size() == 3);
	cr_assert(Parser.GetCommands().size() == 1);
	cr_assert(Parser.GetProgramName() == "./test");
	cr_assert(Parser.GetProgramName() == Parser.GetArguments()[0]);
	cr_assert(Parser.GetCommands()[0]->GetIndex() == 1);
	cr_assert(Parser.GetCommands()[0]->HasName("-l") == true);
	cr_assert(Parser["-l"] == true);
	cr_assert((*Parser.GetCommands()[0]) == "-l");
	cr_assert((*Parser.GetCommands()[0])["pl-pl"] == true);
	cr_assert((*Parser.GetCommands()[0])["en-gb"] == false);
	cr_assert((*Parser.GetCommands()[0]).GetArguments().size() == 1);

	for(int i = 0; i < Size; i++){
		delete Argv[i];
	}
	delete Argv;
}

Test(Command,InitializesProperly)
{
	const GOpp::Command::Definition Mock = {{"-p"},{"x"},0,1};
	GOpp::Parser Parser({"./test","-p"},{Mock});
	cr_assert(Parser.GetCommands().size() == 1);
	cr_assert(Parser.GetCommands()[0]->GetProperties() == Mock);
}

Test(MiscFunctions,ConvertArgvToVectArguments)
{
	char **mock = new char*[5];
	for(auto a: {0,1,2,3,4}){
		mock[a] = nullptr;
	}
	
	cr_assert_throw(GOpp::ConvertArgvToVect(-1,mock),std::invalid_argument);
	cr_assert_throw(GOpp::ConvertArgvToVect(5,mock),std::invalid_argument);
	cr_assert_throw(GOpp::ConvertArgvToVect(5,nullptr),std::invalid_argument);
	
	for(auto a: {0,1,2,3,4}){
		mock[a] = new char[2];
		mock[a][0] = 'a';
		mock[a][1] = '\0';
	}
	
	cr_assert_none_throw(GOpp::ConvertArgvToVect(5,mock));
	
	for(auto a: {0,1,2,3,4}){
		delete mock[a];
	}
	
	delete mock;
}
/* Temporarily not working.. :|
Test(MiscFunctions,BuildSymTableExceptions)
{
	std::vector<GOpp::Command::Definition> Mock = {{{"-l","--language"},{"-pl-pl"},0,1},{{"-l","-lick"},{"-pl-pl"},0,1}};
	std::function<std::vector<std::string>(GOpp::Command::Definition)> GetNames = [](GOpp::Command::Definition Arg) {
		return Arg.ValidNames;
	};
	std::function<std::vector<std::string>(GOpp::Command::Definition)> GetArgs = [](GOpp::Command::Definition Arg) {
		return Arg.AcceptedArguments;
	};

	cr_assert_throw(GOpp::BuildSymTable <std::string, GOpp::Command::Definition> (
			Mock,GetArgs),std::logic_error);
	cr_assert_throw(GOpp::BuildSymTable<std::string, GOpp::Command::Definition >(Mock,GetNames) ,std::logic_error);

}*/
