\page development RogueDM development

## General goals

RogueDM follows an essential list of goals to ensure a consistent design. Bear
in mind that this are general principles but exceptions can apply.

### Code style

The followed code style guide is [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

### Object programming

The desing follows an **strong** approach to Object programming.

* Most logic is represented as object abstractions.
* All classes have a unique file for its definitions and other for its
  declarations. Another related types can be defined along this classes but
  try to avoid cluttering the files too much.
* The C++ main method is mapped to a class (see \ref src/main.cpp and
  \ref roguedm::Application).

### Documentation

Project documentation both for users and for developers lies close to the
code, in the `docs` folder.

The types and definitions are documented using [Doxygen](http://www.doxygen.nl/)
only in the `.hpp` files. `.cpp` files only have programming comments.

Any change requires and update in the documentation when applies.
