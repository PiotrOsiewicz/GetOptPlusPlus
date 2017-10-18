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
	cr_assert((*Parser.GetCommands()[0])["pl-pl"] == true);
	cr_assert((*Parser.GetCommands()[0]) == "-l");
	cr_assert((*Parser.GetCommands()[0]).GetArguments().size() == 1);
}

