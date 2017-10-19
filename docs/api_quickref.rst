###################
API quick reference
###################

.. cpp:class:: GOpp::Parser


**Intent**

Parser is the object that user should interact with. Its responsibilites
include:

- parsing arguments and assigning them to appropiate parameters.
- providing interface to check state of parsed arguments.

**Assumptions**

After  being handed command line arguments in constructor, Parser should not be mutated in any way.
It can be thought of as a container (and interpreter) for Command
objects.

----

.. cpp:function:: GOpp::Parser::Parser(const std::vector<std::string> Parameters, const std::vector<Command::Definition> CommandParameters)

   :arg Parameters: Command line parameters. Size will be deduced from vector size.
   :arg CommandParameters: Definitions of viable parameters, along with their accepted arguments and flags.
   :returns: None:
   :throws invalid_argument: If 'argc' is smaller than 1.
   :throws logic_error: If `CommandParameters` Contains clashing names.

.. cpp:function:: GOpp::Parser::Parser(const std::initializer_list<std::string> Parameters,const std::initializer_list<Command::Definition> CommandParameters)

   Delegates work to vector-based constructor.

   :arg Parameters: Command line parameters. Size will be deduced from container size.
   :arg CommandParameters: Definitions of viable parameters, along with their accepted arguments and flags.
   :returns: None
   :throws invalid_argument: If `argc` is smaller than 1.
   :throws logic_error: If `CommandParameters` contains clashing names.

----

.. cpp:function:: GOpp::Parser::Parser(int argc, char** argv, const std::initializer_list<Command::Definition> CommandParameters)

   Uses C-style arguments and molds them to fit initializer-list
   constructor. 

   :arg argc: Argument count.
   :arg argv: C-style string argument array.
   :arg CommandParameters: Definitions of viable parameters, along with their accepted arguments and flags.
   :returns: None:
   :throws invalid_argument: If `argc` is smaller than 1.
   :throws logic_error: If `CommandParameters` contains clashing names.

