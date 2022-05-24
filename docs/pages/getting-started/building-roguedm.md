\page building-roguedm Building RogueDM

## Getting the code

The code is hosted in https://github.com/fjfnaranjo/RogueDM.

Clone this repository with Git or use the download button in GitHub's
interface.

## Dependencies

*This instructions are just for Linux. Sorry ^^ .*

First, you need a [CMake](https://cmake.org) building toolkit.

> CMake is an open-source, cross-platform family of tools designed to build,
> test and package software. CMake is used to control the software compilation
> process using simple platform and compiler independent configuration files,
> and generate native makefiles and workspaces that can be used in the
> compiler environment of your choice.
>
> -- CMake's website

Also, you need a basic C++ building environment with the next libraries:

* SDL2
* SDL2_image
* SDL2_net
* SDL2_ttf
* lua5.3

We use the [SDL 2](https://www.libsdl.org) library for the graphical interface
and the Lua 5.3 library to allow scripting of the game ruleset.

On Debian 10 you can install all with the next command:

`sudo apt install build-essential cmake libsdl2-dev libsdl2-image-dev libsdl2-net-dev libsdl2-ttf-dev liblua5.3-dev`

## Actual build

Now that you are ready follow standar's CMake procedure to build RogueDM. From
the project root:

```
mkdir build
cd build
cmake ..
make -j4
```

## Running the built application

The build left an executable application for your platform in the `build\src`
folder. The application needs some resources in the project root to work
property. To run it you have a few options:

* Run the application from the project root with `build\src\roguedm_app`.
* Copy the built application to the project root.
* Symlink the built application to the project root.
* Copy the application and the `env` folder contents to some other place.

## Building this documentation

To build this docs, you need a recent version of Doxygen. Issue the next
command in the project root.

`doxygen docs/doxyfile`
