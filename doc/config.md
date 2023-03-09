# Config

Config provides several types for managing argument parsing (`Arg`, `Options`) for command line applications, and more 
simplified argument processing for `int main(int argc, char * argv[])` in the form of  `Args`.

It also provides property and configuration management via `Section`, `Properties`, and `Configuration`

The `Arg` type provides functionality similar to `getopt`, `gflags`, or `optparse` in Python.  It lets you specify a set of alternative option names and values for each argument (and will collect multiple values). `Options` manages a list of `Arg`'s which can be added to using the `Option(...)` method which specifies a key, and a number of optional aliases.  This returns a reference to `Arg` which you can then call `Bind(...)` on.

Each `Arg` can be bound to a variable whos value will be populated as arguments are parsed.  Valid values include `bool, int, long long, double, String`.  


`Options` can also bind args to a `Multimap<String,...>`  or `Callback<void,...>` where the second type must match one of the 5 supported primitive types.


`Properties` is just a wrapper for a `Multimap<String,String>` with some additonal helper methods like `Contains(...)`

A `Section` has a `Name` and `Properties` and maps to a section within a .ini configuration file.

`Configuration` contains a list of `Section`'s and can load a .ini file, providing methods to query sections and properties.  It is a simple interface that returns pointers to `String` or `Section` or null if the query failed.



# Types

- Arg
- Options
- Args
- Section
- Properties
- Configuration
- ConfigurationFile
- ConfigurationParser

# Usage




# Implementation