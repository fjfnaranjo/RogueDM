\page main-concepts Main concepts

This concepts are used extensively in the documentation. Get used to them
because they are essential to understand RogueDM architecture.

## "IO classes" as the user interfaces

The different interfaces that RogueDM uses to communicate with the user have
a special name to avoid naming collisions with object oriented class
interfaces. We call them "IO classes" or just "IO".

All the IO classes are identified by a "IO" suffix. This classes will be also
command handlers and components (see below).

The current IO classes are:

* The SDL 2 user interface in the sdl2 folder: \ref roguedm_gui::Sdl2IO.
* The CLI user interface in the cli folder: \ref roguedm::CliIO.

Giving the complexity of this IO classes all their logic is under their
respective folders. Also, the declarations of the SDL 2 IO class live in an
specific namespace called \ref roguedm_gui.

## The application "components"

Interactivity of an application like RogueDM requires some form of perpetual
loop where a set of "components" are constantly updated to provide the illusion
of realtime.

This components implement the \ref roguedm::GameComponentInterface and the
\ref roguedm::Application class is responsible of their definition and
management of the loop.

## The application "commands", the "command handlers" and the "command bus"

As an interactive application, RogueDM should respond to some kind of input
events. Input events can be of different nature. They can be network input,
input from the command line interface or input from the GUI.

This input events will always be represented as a text command and from now on
we will call them just "commands".

Interaction of this commands and the application components is handled by the
\ref roguedm::CommandHandlerInterface. The commands will be propagated to the
application components by the \ref roguedm::Application class. This propagation
is a simple broadcast from all producers to all consumers.

We are not using an explicit bus implementation here, but we will call this
command propagation mechanishm "command bus" anyway.

The \ref roguedm::Network component will handle how this commands are sent
to another instances of RogueDM connected over the network.

## Namespaces, the Game component and the game rulesets

Most of the fundamental application bahavior is handled in the \ref roguedm
namespace.

The \ref roguedm_gui namespace handle all the interactions related to the
\ref roguedm_gui::Sdl2IO IO class.

A special namespace exists to handle the actual game rulesets (we are talking
here about the RPG/board game beign played in the VTT either locally or over
the network). The \ref roguedm_game::Game component in the \ref roguedm_game
namespace is responsible of this labor and you can find more information about
it in the \ref game-rulesets documentation.
