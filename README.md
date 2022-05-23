# metmice
A programming language.


## How does the language works ?
Basicly, it is half stack based, and half not.

There are three stacks : 
- the first one is for the output
- the second one is for computing
- the third one is for interracting with the system

There are also two types of computing : rpn and normal.
The rpn stands for _reversed polish numeration_. Basicly, you would in a first time give the two arguments and the the operator.
Example : `1 2 +` returns `3`.

For the syntax, there is one rule to follow : __THERE MUST BE SEMICOLONS AT THE END OF EACH EXPRESSIONS__.

Example : 

`1 + 2;`

or

```
if(true)
{
    push 1 "It is indeed";
};
```

### The types
In Metmice, there are 6 major types : 
- string
- int (integers)
- floats
- bools
- list
- none

However, you can define you own types.

For now, you can't convert types. It means that during you program, a string stays a string, a float stays a float etc...
As well as in a calculation, you can't use an operator between two differents types.

### The operators


<details>
   <summary>See all the operators</summary>

  | operator +  | bool | string | int | float | list | others | description
  | :---------- |:----:| :-----:| :-: | :---: | :--: | :----: | ----------:
  | bool        |      |        |     |       |      |        |
  | string      |      |        |     |       |      |        |
  | int         |      |        |  x  |       |      |        | adds two integers
  | float       |      |        |     |   x   |      |        | adds two floats
  | list        |  x   |   x    |  x  |   x   |  x   |   x    | adds the element at the end of the list  
  | others      |      |        |     |       |      |        |

  | operator -  | bool | string | int | float | list | others | description
  | :---------- |:----:| :-----:| :-: | :---: | :--: | :----: | ----------:
  | bool        |      |        |     |       |      |        |
  | string      |      |   x    |     |       |      |        | adds two strings (example : "a" + "b" gives "ab")
  | int         |      |        |  x  |       |      |        | substract two integers
  | float       |      |        |     |   x   |      |        | substract two floats
  | list        |      |        |     |       |      |        | 
  | others      |      |        |     |       |      |        |

  | operator *  | bool | string | int | float | list | others | description
  | :---------- |:----:| :-----:| :-: | :---: | :--: | :----: | ----------:
  | bool        |      |        |     |       |      |        |
  | string      |      |        |     |       |      |        |
  | int         |      |        |  x  |       |      |        | multiplies two integers
  | float       |      |        |     |   x   |      |        | multiplies two floats
  | list        |      |        |  x  |       |      |        | get the value of the list at the given index
  | others      |      |        |     |       |      |        |

  | operator /  | bool | string | int | float | list | others | description
  | :---------- |:----:| :-----:| :-: | :---: | :--: | :----: | ----------:
  | bool        |      |        |     |       |      |        |
  | string      |      |        |     |       |      |        |
  | int         |      |        |  x  |       |      |        | divides two integers
  | float       |      |        |     |   x   |      |        | divides two floats
  | list        |      |        |     |       |      |        | 
  | others      |      |        |     |       |      |        |

  | operator // | bool | string | int | float | list | others | description
  | :---------- |:----:| :-----:| :-: | :---: | :--: | :----: | ----------:
  | bool        |      |        |     |       |      |        |
  | string      |      |        |     |       |      |        |
  | int         |      |        |  x  |       |      |        | operates an euclidean division on two integers
  | float       |      |        |     |   x   |      |        | operates an euclidean division on two floats
  | list        |      |        |     |       |      |        | 
  | others      |      |        |     |       |      |        |

  | operator %  | bool | string | int | float | list | others | description
  | :---------- |:----:| :-----:| :-: | :---: | :--: | :----: | ----------:
  | bool        |      |        |     |       |      |        |
  | string      |      |        |     |       |      |        |
  | int         |      |        |  x  |       |      |        | operates a modulo on two integers
  | float       |      |        |     |   x   |      |        | operates a modulo on two floats
  | list        |      |        |     |       |      |        | 
  | others      |      |        |     |       |      |        |

  | operator ^  | bool | string | int | float | list | others | description
  | :---------- |:----:| :-----:| :-: | :---: | :--: | :----: | ----------:
  | bool        |      |        |     |       |      |        |
  | string      |      |        |     |       |      |        |
  | int         |      |        |  x  |       |      |        | operates a power on an integer to an other
  | float       |      |        |     |   x   |      |        | operates a power on a float to an other
  | list        |      |        |     |       |      |        | 
  | others      |      |        |     |       |      |        |
  
  | operator =  | bool | string | int | float | list | others | description
  | :---------- |:----:| :-----:| :-: | :---: | :--: | :----: | ----------:
  | bool        |  x   |   x    |  x  |   x   |  x   |   x    | Tests if the two values are the same. Returns a bool
  | string      |  x   |   x    |  x  |   x   |  x   |   x    |
  | int         |  x   |   x    |  x  |   x   |  x   |   x    | 
  | float       |  x   |   x    |  x  |   x   |  x   |   x    | 
  | list        |  x   |   x    |  x  |   x   |  x   |   x    | 
  | others      |  x   |   x    |  x  |   x   |  x   |   x    |

  | operator >  | bool | string | int | float | list | others | description
  | :---------- |:----:| :-----:| :-: | :---: | :--: | :----: | ----------:
  | bool        |      |        |     |       |      |        |
  | string      |      |        |     |       |      |        |
  | int         |      |        |  x  |       |      |        | tests if an integer is greater than an other
  | float       |      |        |     |   x   |      |        | tests if a float is greater that an other
  | list        |      |        |     |       |      |        | 
  | others      |      |        |     |       |      |        |

  | operator <  | bool | string | int | float | list | others | description
  | :---------- |:----:| :-----:| :-: | :---: | :--: | :----: | ----------:
  | bool        |      |        |     |       |      |        |
  | string      |      |        |     |       |      |        |
  | int         |      |        |  x  |       |      |        | tests if an integer is lower than an other
  | float       |      |        |     |   x   |      |        | tests if a float is lower that an other
  | list        |      |        |     |       |      |        | 
  | others      |      |        |     |       |      |        |
  
</details>

### Variables
Now you know how to operate operations in Metmice.

But maybe you wants to define a variable ...

As a matter of fact, it is really simple : 

all you need to do is add an `->` (asignement operator) and a variable name.

For instance, to define a variable called _'toto'_ which contains the value "Hello World!" :

```
"Hello World!" -> toto;
```

Then you can reuse the variable whenever you wants : 

```
push 1 toto;
``` 
(it prints the value of _'toto'_, here "Hello World!")


They are two types of variables local and global. Global variables are the one you defines outside a function, and local is the one you will define - you've guess it - inside a function.

### Functions
The functions declaration goes like this : 
`function_name(arg1, arg2)[none]{ your code goes there};`

For example : 

```
add(a, b)[none]
{
   return a + b;
};
```

Then to execute the function you place an exclamation point before the function name `!` and then you pass the arguments between parentethis.

Example : 

```
!add(1, 3);
```

```
!add(5, 6) -> variable;
```

### Keywords
There are a lot of keywords : 
- return => returns the value in a function
  ```
  toto(x)[none]
  {
    return x + 1;
  };
  ``` 
- break => breaks the loop
  ```
  for(i, 0, 10)
  {
    break;
  }
  ```
- push => pushes the second argument on a given stack number
  ```
  push 1 "Hello World! \n";
  ```  
- include => include the file given as a string
  ```
  include "std.mtmc";
  ```
- type => pushes on the compute stack the type of a given value, as a string
  ```
  type "test";
  ```
- global => globalize a variable in a function (not recomended)
  ```
  toto(x)[none]
  {
    x + 1 -> y;
    global y;
  }
  !print(y); 
  ```
- length => pushes on the compute stack the lenght of a list
  ```
  length [1, 2, 3];
  !print( + 0);
  ```
- pop => pops a given index in a list
  ```
  [1, 2, 3] -> list;
  pop -1 list;
  ```
- operator => binds an operator with a function
  ```
  operator "&" "!and";
  ```

Beware, you can not use all the keywords wherever you wants !

### Loops
They are two types of loops : for loops and while loops

The for loops takes 3 arguments : the name of the variable, the starting point and the ending point. Both starting and ending points must be integers.

Example : 
```
for(i, 0, 10)
{
  push 1 i;
  push 1 "\n";
};
```

The while loop takes as an argument a boolean. It will iterate while the boolean is not 'false'.

Example :
```
0 -> i;
while (i < 0)
{
  push 1 i;
  i + 1 -> i;
};
```

### Structures
In metmice, you can define a structure using `struct`.
  
for example : 
```
struct t
{
  0 -> sum;
  get_sum(self)[none]
  {
    return self.sum;
  };
};
```

Inside the curly brackets, you can define some attributes (variable that are only for a given object) and some function attributes.
You can then create an object with the type you precedently difined :
```
!t() -> test;
```

In order to access one of its atribute : use a point between the variable name and the attribute name : 
```
push 1 test.sum;
```

To execute one of its function attribute, use an exclamation point followed by the variable name and the function name and the arguments :
```
!test.get_type();
```

You might notice that the object is passed as an argument (here `self`). 

  
## How to execute you code
In bash (Linux) and Zsh : 
```
metmice-x.x your_program_name.mtmc [-options]
```
  
Options are :
- --strict_disabled or -S to disable a strict analyse of the code 
- --debug_disabled or -d to not debug the code
- --ast_show or -A to show the abstract syntax tree
- --clear_stack_disabled or -c to desable the compute stack clearing
- --help or -h to show the help message
- --print_full_error or -e to print the full error calling stack

## How to start a new program
The first and most recommended thing to do when you create a ".mtmc" file is to include the standard librairie.
To do so, just write at the begining of your file : 
`include "./path/to/the/file/std.mtmc`

With that done, you can now use all the functions you will need !


