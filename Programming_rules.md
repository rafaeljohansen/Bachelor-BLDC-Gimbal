# Programming rules
In order to cooperate at this project, we need to define a set of rules on how we're going 
to structure our code.

#### Naming
We use the camel case notation
* Types start with upper case: `MyClass`
* Functions and variables start with lower case: `myMethod`
* Constants are all upper case: `const double PI = 3.14159265358979323`
* Variables (provate or not) should never start with `_` or `__`. These are reserved for the
compiler. Use `t_` for parameters and `m_` for members.
  * `PrivateSize(int t_width, int t_height)`
  *  `m_width = t_width;`

#### Structure
* (Always) put the brackets on the next line:
```C++
if(true)
{
  //do something
}
```

* Brackets are not needed if you just got one line:
```C++
if(true)
  //do something

```

* AVOID spacing like this:
```C++
if ( true )
{
  //do something
}
```

* Adding spaces around math symbols makes the code much easier to read:
```C++
value = 2 * PI * sensorValue;
```
