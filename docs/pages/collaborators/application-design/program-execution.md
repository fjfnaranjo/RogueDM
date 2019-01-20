\page program-execution Program execution

## The main function delegation

The C++ main function delegates its two responsabilities (handling of program
arguments and generation of a exit status code) to the
\ref roguedm::Application class. This class is just a plain class with a
method \ref roguedm::Application::run that shares the signature of the main
function.

## The application stage loop

\image html stage.png

RogueDM can be run in different modes called "stages". The two basic stages are
the "GUI stage" and the "CLI stage".

To provide the application with the ability of switching between stages a
"stage loop" is present in the \ref roguedm::Application::run method of the
\ref roguedm::Application class.

### Responsabilities of the stages

#### Common responsabilities

The stages have a few common responsabilities:

* Declare a \ref roguedm::Network reference as a null shared pointer (some IO
class will define it later).
* Declare a \ref roguedm_game::Game instance as a null shared pointer (some IO
class will define it later).
* Create the correspondant IO class and passing the previous two instances and
a funtion to broadcast commands in the shared command bus.
 * \ref roguedm_gui::Sdl2IO for the GUI stage.
 * \ref roguedm::CliIO for the CLI stage.
* Implement the shared command bus broadcast.  and handle some commands:
 * Configuration or operative system related commands.
* Update a set of \ref roguedm::GameComponentInterface.

The stages also return a complex value of type \ref roguedm::StageResponse.

All the stages and their related code is in the stages folder.

#### Delegating application control to another stage

Each state is responsible for returning a valid combination of an exit status
and the next stage to be run in the loop. The application will terminate and
return the last reported exit status if:

* The last reported exit status isn't 0.
* The special \ref RDM_STAGE_EXIT value is defined as the next stage.
