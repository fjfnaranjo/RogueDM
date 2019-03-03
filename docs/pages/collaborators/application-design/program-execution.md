\page program-execution Program execution

## The main function delegation

The C++ main function delegates its two responsabilities (handling of program
arguments and generation of a exit status code) to the
\ref roguedm::Application class.

## The Application class

This class has this responsabilities:

* Declare a \ref roguedm::Network reference as a null shared pointer (some IO
class will define it later).
* Declare a \ref roguedm_game::Game instance as a null shared pointer (some IO
class will define it later).
* Create the correspondant IO classes and pass the previous two instances and
a funtion to broadcast commands in the shared command bus.
 * \ref roguedm_gui::Sdl2IO
 * \ref roguedm::CliIO
* Implement the shared command bus broadcast and handle some commands:
* Update a set of \ref roguedm::GameComponentInterface in a main loop.
