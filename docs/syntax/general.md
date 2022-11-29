# General Syntax

This page covers the two most basic nodes in the abstract syntax tree that `Dusk` creates, Expressions
and Statements. These types make up what the average 'line' of code is in `Dusk`.

The other two constructs that make up a `Dusk` program are `Functions` and `Structs`, each of these are
used to create the overall structure of the program.

## Expressions

Expressions are the smallest unit of the abstract syntax tree that Dusk creates.

### Expression Atomic

Expression Atomics are the simplest unit of the Expressions, they all produce a single atomic value.

Here is a list of the various atomic expressions.

- Booleans
    - `True`
    - `Flase`
- Integers
    - `123`
- Floats
    - `1.5`
- Strings
    - `"Hello"`
- Array Literals
    -  TODO
- Struct Objects
    - Method Calls `Foo.bar()`
    - Member Values `Foo.a`

### Binary Operator

Binary operators are used for various math operations and boolean operations.

Here is a list of the various binary operators, each have and expression on the left and the
right of the operator.

- `+` Add
- `-` Subtract
- `*` Multiply
- `/` Divide
- `%` Modulo
- `==` Equal
- `!=` Not Equal
- `<` Less Than
- `>` Greater Than
- `<=` Less Than Or Equal To
- `>=` Greater Than Or Equal To
- `||` Logical Or
- `&&` Logical And

### Unary Operators

Unary Expressions can be used to modify a single expression.

Here is a list of the various unary operators.

- `-<expression>` Negate the expression (Integral / Floating numbers only)
- `!<expression>` Not the expression (boolean types only)
- `<expression>++` Increment the expression, equivalent to `<expression> = <expression> + 1`

### Assignment Expressions

Assignment expressions modify already created variables.

For example this sets the variable foo to the value of the expression.

`foo = <expression>`

You can also use `[` and `]` to assign to a specific index if the variable is an array type.

For example this would set the `bar` objects array at the index of the value `expression 1` produces,
to the value `expression 2` produces.

`bar[<expression 1>] = <expression 2>`

### Break Expression

Not Yet Implemented.

## Statements

Statements make up the body of functions, and there are various types.

### Expression Statement

Expression statments are most often used to invoke expressions that have side effects. They are any
Expression followed by a semicolon.

`<expression>;`

Call a function `foo`.

```
foo()
```

Change the value of a variable.

```
a = 10;
```


### Assignment Statement

Assignment statements initialize new variables, and may or may not set them to an initial value.
They also specify the mutability of the resulting variable.

Types of Variables `<type>`
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

For example this would initialise a mutable variable named `a` of type `int` with no initial value.

```
let mut a: int;
```

This would initialise a static float named `b` to the value `3.14`.

```
let b: float = 3.14;
```

This would initialise a static string named `c` to the value `Hello World!`.

```
let c: string = "Hello World!";
```

Finally this would initialise a mutable struct named `Foo` into the variable `d`.

```
let mut d: Foo = Foo();
```

### Return Statement

Return expressions are used to return a value / expression from a function.

An example of a return statement is.

```
return <expression>;
```

### If Statement

If statments are used as control flow to execute different pieces of code based on a condition.

This is an example of an if statment.

```
if <expression> {
    <statements>
}
```

You can also create If-Else statements.

```
if <expression> {
    <statements>
} else {
    <statements>
}
```

If-ElseIf

TODO

### For Loop Statement

For loops can be used to iterate over a range of values.

Here is an example of one.


```
for (<assignment statment>; <comparison expression>; <increment expression>) {
    <statments>
}
```

### While Loop Statement

While loops can be used to iterate over a range of values.

Here is an example of one.

```
while <comparison expression> {
    <statements>
}
```

### Statement Blocks

Statment blocks can be used to control the scope of variables.

Anything inbetween a block of `{` and `}` will have its own local scope that will be destroyed at the
end of the block.

```
{
    // Scope A
    {
        // Scope B
    } // Scope B Destroyed
    {
        // Scope C
    } // Scope C Destroyed
} // Scope A Destroyed
```

## Other

Comments can be created using `//`.

```
// This is a comment
```

Multiline Comments can be created using `/* ... */`.

```
/* This is a comment
That spans multiple lines
*/
```
