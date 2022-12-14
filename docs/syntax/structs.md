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

You can also implement a destructor function for structs by declaring a function named `__del__`. 
If this function is not implemented a default version will be created that frees the structs memory.

```
struct Foo {
    a: int,

    fn __del__(self: Foo) {
        // Custom destructor logic for fields if they need it
        // A call to free the struct objects memory will be added automatically.
    }
}
```

## Templates

Structs can also be created using the `template` keyword. The name for these template types must be
formatted like `struct_name<T>` where `T` can be any identifier to be templated over. Templates will
monomorphize all the fields and function arguments and return types into the correct types for the
templated type. These templated types are only created as needed.

For example here is a section of the `Vec<T>` template found in the standard library.

```
extern vpvec() -> void*;
extern del_vpvec(v: void*);
extern vppush_back(vec: void*, val: void*);
extern vppush_front(vec: void*, val: void*);
extern getvecvp(vec: void*, idx: int) -> void*;

template Vec<T> {
    vector: void*,
    size: int,
    exampleT: T, // example of a templated member variable
    exampleVT: Vec<T>, // example of a templated member variable

    fn Vec<T>(self: Vec<T>) -> Vec<T> {
        self.vector = vpvec();
        self.size = 0;
        return self;
    }

    fn append(self: Vec<T>, n: T) {
        n.__INCREF__(); // n isn't automatically __INCREF__'d on call into extern C func
        vppush_back(self.vector, n);
        self.size = self.size + 1;
    }

    fn prepend(self: Vec<T>, n: T) {
        n.__INCREF__(); // n isn't automatically __INCREF__'d on call into extern C func
        vppush_front(self.vector, n);
        self.size = self.size + 1;
    }

    fn get(self: Vec<T>, idx: int) -> T {
        self.__INCREF__(); // self isn't automatically __INCREF__'d on call into extern C func
        return getvecvp(self.vector, idx);
    }
}
```

Templated functions can be overriden for specific types if required. (Currently the names must be
manually magled to include struct type at the start, and most methods must also have types appended
to the end).

```
template Vec<T> {
    ...
}

fn Vec<String>__str__(self: Vec<String>) -> string {
    let mut ret: String = String("[ ");
    for (let mut i: int = 0; i < self.size; i++) {
        ret = ret + "'" + String(self.get(i)) + "'";
        if (i < self.size - 1) {
            ret = ret + ", ";
        }
    }
    ret = ret + " ]";
    return ret.to_str();
}
```
