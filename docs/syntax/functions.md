# Function Syntax

This page outlines the various ways to create functions and load functions declared in other files /
libraries.

## Global Functions

Functions are all part of the Global scope of a dusk program.

There are a few rules around the defninitions of functions.

- They must all have a name
- They must all have a body (excluding `external` functions)
- They may take 0 to N arguments
    - Arguments may be static or mutable
- They may have a void return type or have a specified return type
    - `void`
    - `bool`
    - `char`
    - `int`
    - `long`
    - `float`
    - `string`
    - `bool*`
    - `int*`
    - `float*`
    - `string*`
    - `Struct Types (Foo for a struct named Foo)`

All non heap allocated objects are deallocated upon returning from a function.

### Examples

A void function that has no arguments.

```
fn foo() -> void {
    1 + 1;
    return;
}
```

Functions that return void may exclude the functions return type. They may also exclude the return
at the end of the function, early returns must still be manually called with `return;`.

```
fn foo() {
    1 + 1;
}
```

Here is an example of a function that takes two arguments one is a mutable struct `Foo` named `a` the other
is a static `int` named `b` and returns an `int`.

```
fn bar(mut a: Foo, b: int) -> int {
    a.x = b;
    return b;
}
```

## External Functions

If you wish to use functions from a library compiled to be `C` compatible you may use the `extern` keyword
to load that function from a library included at compile time.

You must specify the name of the function and the arguments it takes an returns. However the types
used must match the `C` types of the function you are importing.

### Examples

Here is and example of including the C functions malloc.

```
extern malloc(n: int) -> void*;
```

## Include and Requires keywords


### Include

The include keyword is used to load other `Dusk` source files (`.ds`). Including a file on the
include path will automatically add it to the list of files to be compiiled without having to
manually specify it.

For example this would automaticall add the file `vector.ds` to the list of files to be compile.

```
include "vector.ds"
```

### Require

The require keyword adds a `C` library to be linked against automatically at compile time. The library
will be searched for on you `C` library path.

For example this would automatically add the `-lmylibrary` to the list of libraries to link against.

```
require "mylibrary"
```
