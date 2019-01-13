\page application-design Application design

This page summarizes most of the design decissions about how RogueDM code is
structured.

## The main class delegation

The C++ main function delegates its two responsabilities (handling of program
arguments and generation of a exit status code) to the
\ref roguedm::Application class. This class is just a simple class with a
method \ref roguedm::Application::run that shares the signature of the main
function.

## The application stage loop

RogueDM can be run in different modes called "stages". The two basic stages are
the "GUI stage" and the "server stage".

To provide the application with the ability of switching between stages a
"stage loop" is present in the \ref roguedm::Application::run method of the
\ref roguedm::Application class.

Each state is responsible for returning a valid combination of an exit status
and the next stage to be run in the loop. The application will terminate and
return the last reported exit status if:

* The last reported exit status isn't 0.
* The special \ref RDM_STAGE_EXIT value is defined as the next stage. 

### Responsabilities of the stages

An stage will either do an special hidden task or display some kind of
interface to the user.

Unless a special stage is run for a specific purpose, the stages have also two
common responsabilities:

* Reacting to commands from some \ref roguedm::CommandHandlerInterface.
* Create and update a set of \ref roguedm::GameComponentInterface. 

## The application "command handlers"

As an interactive application, each stage should respond to some kind of input.
Input can be of different nature. It can be network input, input from a
command line interface of the rich visual input of the GUI stage.

This input will always be represented as a text command. Interaction of this
commands and the application components is handled by the
\ref roguedm::CommandHandlerInterface.

The input lives attached to a kind of bus system when producers and
consumers are linked toguether and can share commands between them.

This resembles a command pattern design when the command structure is always
form as a combination of simple text messages.

The bus is not an explicit mechanism and the current stage is responsible of
relaying messages between command handlers.

## The application "components"

Interactivity of an application like RogueDM requires a form of perpetual loop
when a set of "components" are constantly updated to provide the illusion of
realtime.

This components implement the \ref roguedm::GameComponentInterface and the
current stage is responsible of their definition and management.

### Namespaces, the Game component and the game rulesets

Most of the fundamental application bahavior is handled in the \ref roguedm
namespace but an special namespace and component exists to handle the actual
game rulesets (we are talking here about the RPG/board game beign played in
the VTT either locally or over the network).

The \ref roguedm_main::Game component in the \ref roguedm_main namespace is
responsible of this labor and you can find more information about it in the
\ref game-rulesets documentation.
