\page app-configuration Application configuration

Application configuration is stored in a map of maps representing the
common INI file structure Section->Setting->Value. This memory representation
is kept in the field \ref roguedm::Config.sections inside the singleton
\ref roguedm::Config .

## Accesing empty values

Because of the nature of C++ STD maps, trying to access a nonexistent value
will create the setting an assing an empty value to it. Avoid this calling
first the \ref roguedm::Config.hasSetting method.

## Configuration file

The configuration is read from a config file in the current user's config
directory. If the file is missing a new one will be created from a template.