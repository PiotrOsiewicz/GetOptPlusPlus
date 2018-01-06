#include <functional>
#include <getopt++/command_impl.h>
#include <getopt++/parser_impl.h>
#include <getopt++/misc.h>
#include <criterion/criterion.h>
#include <iostream>

Test(Parser,NoArgs)
{
	cr_assert_throw(GOpp::Parser Parser({}),std::invalid_argument);
}

Test(Parser,ClashingNames)
{
	cr_assert_throw(GOpp::Parser Parser({{{"-test"},{},0,1},{{"-l"},{"-test"},0,1}}),std::logic_error);
}

Test(Parser,InvalidCountTooLittle)
{
	GOpp::Parser Parser({{{"-test"},{"param"},1,1}});
	cr_assert_throw(Parser.Parse({"-test"}),std::invalid_argument);
}

Test(Parser,InvalidCountTooMuch)
{
	GOpp::Parser Parser({{{"-test"},{"param"},1,1}});
	cr_assert_throw(Parser.Parse({"-test","param","param"}),std::invalid_argument);
}

Test(Parser,ValidCount)
{
	GOpp::Parser Parser({{{"-test"},{"param"},1,1,}});
	cr_assert_none_throw(Parser.Parse({"-test","param"}));
}

Test(Parser,UnlimitedArgs)
{
	GOpp::Parser Parser({{{"-hey"},{},0,0,GOpp::Command::Definition::Flags::AcceptAnyArgument | GOpp::Command::Definition::Flags::NoArgumentLimit}});
	cr_assert_none_throw(Parser.Parse({"-hey","toy","testing"}));
}

Test(Parser,NoParameter)
{
	GOpp::Parser Parser({{{"-test"},{"param"},1,1}});
	cr_assert_throw(Parser.Parse({"-test"}),std::invalid_argument);
}

Test(Parser,Rubbish)
{
	GOpp::Parser Parser({{{"NotRubbishStuff"},{},0,1}});
	cr_assert_throw(Parser.Parse({"./test","rubbishstuffhere"}),std::invalid_argument);
}

Test(Constructor,InitializesCommandsProperly)
{
	GOpp::Parser Parser({{{"-l"},{"pl-pl"},0,1}});
	Parser.Parse({"-l","pl-pl"});
	cr_assert(Parser.GetCommands().size() == 1);
	cr_assert(Parser.GetCommands()[0].GetIndex() == 0);
	cr_assert(Parser.GetCommands()[0].GetDefinition().HasName("-l") == true);
	cr_assert(Parser["-l"] == true);
	cr_assert(Parser.GetCommands()[0] == "-l");
	cr_assert(Parser.GetCommands()[0]["pl-pl"] == true);
	cr_assert(Parser.GetCommands()[0]["en-gb"] == false);
	cr_assert(Parser.GetCommands()[0].GetArguments().size() == 1);
}

Test(Command,InitializesProperly)
{
	const GOpp::Command::Definition Mock = {{"-p"},{"x"},0,1};
	GOpp::Parser Parser({Mock});
	Parser.Parse({"-p"});
	cr_assert(Parser.GetCommands().size() == 1);
	cr_assert(Parser.GetCommands()[0].GetDefinition() == Mock);
}

Test(Flags,AcceptAnyArgument)
{
	GOpp::Parser Parser({{{"-l"},{},0,5,GOpp::Command::Definition::Flags::AcceptAnyArgument}});
	Parser.Parse({"-l","1","2","3","4","5"});
	cr_assert(Parser.GetCommands()[0].GetArguments().size() == 5);
}

Test(Flags,NoArgumentLimit)
{
	GOpp::Parser Parser({{{"-l"},{"1"},0,0,GOpp::Command::Definition::Flags::NoArgumentLimit}});
	Parser.Parse({"-l","1","1","1","1"});
	cr_assert(Parser.GetCommands()[0].GetArguments().size() == 4);
}

// Temporarily not working.. :|
Test(MiscFunctions,BuildSymTableExceptions)
{
	std::vector<GOpp::Command::Definition> Mock = {{{"-l","--language"},{"-pl-pl"},0,1},{{"-l","-lick"},{"-pl-pl"},0,1}};
	std::function<std::vector<std::string>(const GOpp::Command::Definition)> GetNames = [](GOpp::Command::Definition Arg) {
		return Arg.ValidNames;
	};
	std::function<std::vector<std::string>(const GOpp::Command::Definition)> GetArgs = [](GOpp::Command::Definition Arg) {
		return Arg.AcceptedArguments;
	};

	cr_assert_throw(GOpp::BuildSymTable(Mock,GetArgs),std::logic_error); //Relies on template argument deduction.
	cr_assert_throw(GOpp::BuildSymTable(Mock,GetNames) ,std::logic_error);

}
