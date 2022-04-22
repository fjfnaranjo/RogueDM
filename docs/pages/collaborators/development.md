\page development RogueDM development

## General goals

RogueDM follows an essential list of goals to ensure a consistent design. Bear
in mind that this are general principles but exceptions can apply.

### Code style

The followed code style guide is [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

#### Exceptions

Import every individual file containing the symbols used, but:

* Don't include extra SDL headers already included in the main or SDL project
  headers (import `SDL.h` or `SDL_image.h`, but don't import `SDL_events.h`).
* Don't include `memory` when using the `macros.hpp` file for the sole purpose
  of satisfying a symbol in the actual macro file (`memory` is imported there
  for that reason).

### Import sorting

Group and sort the imports following the next order:

1. In a source file, the correponding header file, isolated.
2. Standard library imports.
3. SDL imports with the base `SDL.h` file first.
4. Own project imports:
  1. Files used at the same level
  2. Files in different levels grouped by level.
  3. Files in the main directory and other general imports.

Order the groups/levels following how their use is ordered in the file and sort
individual files by name.

### Object programming

The desing follows a **strong** approach to Object programming.

* Most logic is represented as object abstractions.
* All classes have an unique file for its declarations and other for its
  definitions. Another related types can be defined along this classes but
  try to avoid cluttering the files too much.
* The C++ main method is mapped to a class (see \ref src/main.cpp and
  \ref roguedm::Application).

### Documentation

Project documentation both for users and for developers lies close to the
code, in the `docs` folder.

The types and definitions are documented using [Doxygen](http://www.doxygen.nl/)
only in the `.hpp` files. `.cpp` files only have programming comments.

Any change requires and update in the documentation when applies.
