# Struct Syntax

Structs are heap allocated objects that contain various named values, and collections of methods.

All functions within a struct support operator overloading.

Structs are also extandable, including structs defined within the standard library, if you define
a struct again with the same name the definitions of each struct will be merged together.

For example this would be merged into a single `Foo` struct with both `a` and `b` member values.

```
struct Foo {
    a: int,
}

struct Foo {
    b: float,
}
```

## Member Values

Structs allow you to define multiple values that are contained within one object.

The syntax for declaring a member value named `a` that is of type `int` is. All member value definitions
must be follwed by a comma. A struct may contain any number of member values.

```
a: int,
```

## Member Functions

Member functions are called using the `.` syntax off of a struct type object (e.g. `FooStruct.bar()`).

All  member functions support operator overloading.

Creating functions that have the same name as the Struct type will be used as constructors and must
take the first parameter `self` of the type of the struct and return the structs type.

You may also include external functions within a struct.

Here is an example of a struct constructor.

```
struct Foo {
    a: int,
    b: float,

    fn Foo(self: Foo) -> Foo {
        self.a = 0;
        self.b = 0.0;
        return self;
    }

    fn Foo(self: Foo, n: int) -> Foo {
        self.a = n;
        self.b = 0.0;
        return self;
    }
}
```

Member functions can also be declared and overloaded.

```
struct Foo {
    a: int,
    b: float,

    fn inc(self: Foo, n: int) {
        self.a = n;
    }

    fn inc(self: Foo, n: float) {
        self.b = n;
    }
}
```

## Operators

Structs also support operator overloading using specific function names to define the behaviour.

These functions may also be overloaded.

Here is a list of the operator functions that may be overloaded.

- `__add__`
- `__sub__`
- `__mul__`
- `__div__`
- `__mod__`
- `__eq__`
- `__neq__`
- `__lt__`
- `__gt__`
- `__le__`
- `__ge__`
- `__and__`
- `__or__`

There is also one more internal functions that can be overloaded.

`__str__`, this function is called with the struct instance as the only parameter and must return a
string type. It is called automatically if a struct is found as an argument to `print` or `printf`.

```
struct Foo {

    fn __str__(self: Foo) -> string {
        return "Foo Struct";
    }
}
```

For example implementing the add operator for a struct.

```
struct Foo {
    a: int,

    fn __add__(self: Foo, n: int) -> int {
        return self.a + n;
    }

    fn __add__(self: Foo, n: Foo) -> int {
        return self.a + n.a;
    }

    fn __add__(n: int, self: Foo) -> int {
        return self.a + n;
    }
}
```
