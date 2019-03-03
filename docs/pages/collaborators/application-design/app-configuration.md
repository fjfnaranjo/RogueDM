\page app-configuration Application configuration

\image html a-config.png

Application configuration is stored in a map of maps representing the
common INI file structure Section->Setting->Value. This memory representation
is kept in the field \ref roguedm::Config.sections inside the singleton
\ref roguedm::Config .

## Configuration file

The configuration is read from a config file in the current user's config
directory. If the file is missing a new one will be created from a template.

## Saving configuration

The ability to persist changes to configuration in disk is not present. It
will require first to differentiate between things we want to keep in disk and
things we only need in memory. Then, some way of updating the file can be
developed.
