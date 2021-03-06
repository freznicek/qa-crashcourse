![Quality-Assurance](pictures/quality_assurance.jpg)

# QA crash-course

The goals of this course are:
* basic overview what the Quality Assurance stands for
* Intro to QA mindset, thinking and reasoning
* practical session for novices
* ... all fast track way within 10 minutes of your time

## Intro
>If you haven't tried it, assume it's broken.
>
>Unknown

## Why Quality Engineering (QA)?
The basic goals are.

* *Before* you ship software to wider audience:
  * assess that software meets the (customer) requirements
  * identify & document/fix bugs or imperfections
* Control the software quality over supported life cycle

## QA Testing types

### Why there are so many testing terms and types?
 * functional testing
 * regression testing
 * exploratory testing
 * `<color>`box testing
 * ...
 
### Why the diagrams look so complex?

#### Testing quadrants
 ![Testing quadrants](pictures/testing_quadrants.png)
 ![Testing quadrants and pyramid](pictures/testing_quadrants_pyramid.png)

#### Testing manifesto
 ![Testing manifesto](pictures/testing_manifesto.png)

#### It is extremely important to look at software under test from different angles!

## QA processes

## QA mindset
 * analytical thinking
 * critical thinking (question everything)
 * troubleshooting expertise
 * attention to detail
 * systematic approach in solving problems
 * at least basic programming knowledge
 * soft-skills
   * adaptable to changing priorities
   * able to communicate well both within and outside the QA team
   * firm, positive and blameless attitude
 * ...

## Practical session (with triangle test client)

Let's go through very basic Linux QA practical session on laboratory single purpose command-line `./triangle` application which detects whther triangle defined by three arguments - triangle side lengths is valid (== can be constructed).

### Pre-requisite, compile the application

Let's assume triangle program package is not ready yet and prepared GNU make target `rebuild` only.
```
# you need GNU make, g++ and C++ STL libraries
$ make rebuild
rm -f triangle triangle.stripped
g++ -g -std=c++11 /tmp/qa-crashcourse.git/src/triangle/triangle.cpp -o /tmp/qa-crashcourse.git/triangle
cp -f /tmp/qa-crashcourse.git/triangle /tmp/qa-crashcourse.git/triangle.stripped
strip /tmp/qa-crashcourse.git/triangle.stripped
```

### Analyzing the architecture & overall approach

Architecture analysis should always take place at the very beginning, before you dig in into details how the software is working.

We should ask yourself questions like:
 * What is the application's goal/aim?
 * Is the application well documented? (for customer, for QA)
 * Do you find application architecture and the way user interact with the application appropriate?
 * Are there available QA process documents or I'm supposed to create them?
   * How I approach testing when no QA process documents available?
   * How I create defect report?
   * How I contact developer with a question?

### Analyzing the documentation

#### triangle -h|--help

Assuming the `-h` and `--help` should work
```
$ ./triangle -h
triangle <a-side-length> <b-side-length> <c-side-length> usage:
  triangle validity tester, where <?-side-length> is length of a triangle side.

  returns ecode of 0 / 1 / 2 with meaning: valid / invalid / input error

✗ 2 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle --help
triangle <a-side-length> <b-side-length> <c-side-length> usage:
  triangle validity tester, where <?-side-length> is length of a triangle side.

  returns ecode of 0 / 1 / 2 with meaning: valid / invalid / input error

✗ 2 root@freznicek-j ~/qa-crashcourse.git/src/triangle
```
Ok works, but help functionality *is* valid function and thus should return exit code of 0.
Moreover application usage does not list the help option (`-h|--help` which calls for an improvement).
[Bug #1](https://github.com/freznicek/qa-crashcourse/issues/3).

#### triangle man/info pages

We need to crosscheck that man page exists (not completely relevant here, but for completeness).
```
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle
$ man triangle
No manual entry for triangle
✗ 16 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ info triangle
 => no triangle node found
✓ root@freznicek-j.dev ~/qa-crashcourse.git/src/triangle
```
In this case we find that Linux standard documentation is not present. At this stage we do not build standard deb/rpm package and thus it is low priority Bug #2.

### Basic functionality

#### Positive test scenarios

```
$ ./triangle 1 1 1
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 5 6 7
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle
```

Very basic scenarios work as expected.

#### Negative scenario, a triangle side length of zero

```
$ ./triangle 1 1 0
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle
```
This is correct you cannot construct triangle when one side is missing (has length of 0).

#### Negative scenario, a side longer than then the sum of the other two

```
$ ./triangle 1 1 3
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle
```
This is evidently right, you cannot construct triangle when one side is greater than the other two.

#### Negative scenario, additional argument\[s\]

```
$ ./triangle 5 6 7 8
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle
```
When we supply more arguments than specified in documentation we expect application to raise the non-zero exit code (in cact exit code == 2). The above behavior is thus incorrect leading to Bug #3.


### Advanced test scenarios

#### Negative scenario, invalid input (not numbers)
```
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle
$ ./triangle K L M
terminate called after throwing an instance of 'std::invalid_argument'
  what():  stod
Aborted (core dumped)
✗ 134 root@freznicek-j ~/qa-crashcourse.git/src/triangle
$ gdb ./triangle core
GNU gdb (Debian 7.7.1+dfsg-5) 7.7.1
...
Reading symbols from ./triangle...done.
[New LWP 9693]
Core was generated by `./triangle K L M'.
Program terminated with signal SIGABRT, Aborted.
#0  0x00007f63dd85a067 in __GI_raise (sig=sig@entry=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:56
56      ../nptl/sysdeps/unix/sysv/linux/raise.c: No such file or directory.
(gdb) bt
#0  0x00007f63dd85a067 in __GI_raise (sig=sig@entry=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:56
#1  0x00007f63dd85b448 in __GI_abort () at abort.c:89
#2  0x00007f63de147b3d in __gnu_cxx::__verbose_terminate_handler () at ../../../../src/libstdc++-v3/libsupc++/vterminate.cc:95
#3  0x00007f63de145bb6 in __cxxabiv1::__terminate (handler=<optimized out>) at ../../../../src/libstdc++-v3/libsupc++/eh_terminate.cc:47
#4  0x00007f63de145c01 in std::terminate () at ../../../../src/libstdc++-v3/libsupc++/eh_terminate.cc:57
#5  0x00007f63de145e19 in __cxxabiv1::__cxa_throw (obj=0x1df30c0, tinfo=0x7f63de3d57f0 <typeinfo for std::invalid_argument>, dest=0x7f63de15a4a0 <std::invalid_argument::~invalid_argument()>)
    at ../../../../src/libstdc++-v3/libsupc++/eh_throw.cc:87
#6  0x00007f63de19ba16 in std::__throw_invalid_argument (__s=<optimized out>) at ../../../../../src/libstdc++-v3/src/c++11/functexcept.cc:79
#7  0x00000000004013b6 in __gnu_cxx::__stoa<double, double, char> (__convf=0x400be0 <strtod@plt>, __name=0x4014ab "stod", __str=0x1df3028 "K", __idx=0x0)
    at /usr/include/c++/4.9/ext/string_conversions.h:65
#8  0x00000000004011b7 in std::stod (__str="K", __idx=0x0) at /usr/include/c++/4.9/bits/basic_string.h:2889
#9  0x0000000000400f2f in main (argc=4, argv=0x7ffc6af9b478) at triangle.cpp:49
(gdb) quit
$ kill -l | grep -F " 6)"
 6) SIGABRT      7) SIGBUS       8) SIGFPE       9) SIGKILL     10) SIGUSR1
```
As documentation claims invalid input should result in exit code of 2, which is not the case. Moreover we can see that application *aborted* due to an c++ exception `std::__throw_invalid_argument` leading to `SIGABRT` with exit code of 134 (128+6).
The Bug #4 should contain:
 * highlight of invalid exit code
 * suggestion to catch the exceptions caused by invalid string to number conversion (`std::stod (__str="K", __idx=0x0)`).

Notes:
 * Above backtrace is generated by `gdb` debugger based on installed debugging symbols `apt-get install libstdc++6-4.9-dbg libc-dbg` and also by the fact that `triangle` has got the debugging symbols too.
 * Try to repeat this scenario using `triable.stripped` application. Will you see the complete traceback of of `gdb` ?
 * Running debugging / instrumentation / linting tools driftes us closer to whitebox testing.

#### Negative scenario, invalid input (negative numbers)
```
$ ./triangle -2 3 4
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 2 -3 4
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 2 3 -4
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle
```
We expect that triangle's side length cannot be negative, which is the case of first and second execution.
The last, third execution proves that the negative-side-length check is missing for third argument leading to Bug #5.

#### Positive scenario, unusual number forms (floating point, signs, types)
```
$ ./triangle 5.0 6 7
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 5 6.0 7
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 5 6 7.0
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle +5.0 +6.0 +7.0
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle -5.0 6.0 7.0
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 5.0 -6.0 7.0
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 5.0 6.0 -7.0
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle
```
Above executions try to mix the integer/floating point numbers with no findings.
The third argument being negative is indeed not checked (previously filled Bug #5).

```
$ ./triangle 1 1000 1000
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1 1000 10000
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1e0 1e0 1e0
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1e0 1e1 1e1
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1 10 10
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1e0 1e1 10
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1e0 1e+1 10
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1e+0 1e+1 10
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1e+0 1e+1 1e+0
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1e+0 1e+1 1
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1e+0 1e+1 1e+1
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1e+0 1e+1 10
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle
```
Above executions unveil that third argument does not support exponential number format, but the others do. Bug #6.


#### Positive scenario, boundary and precision testing

##### Is scenario `a + b == c` handled correctly?
```
$ ./triangle 1 1 2
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1 2 1
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 2 1 1
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle
```
Surprisingly enough there is failing case when `a + c == b`, other executions are correct. Bug #7.

##### How big side numbers are allowed?
```
$ ./triangle 1000 1000 1000
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 100000 100000 100000
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 10000000 10000000 10000000
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1000000000 1000000000 1000000000
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 100000000000 100000000000 100000000000
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 10000000000000 10000000000000 10000000000000
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ python -c 'import sys,math;print(math.ceil(math.log(float(sys.argv[1]))/math.log(2)))' 10000000000000
44.0
```
Bingo!, we found that 44 bit integer is not accepted. Consulting with documentation (which says nothing) we need to decide for bug #8. It is good to get more information whether it applies to specific argument or all:
```
$ ./triangle 1000000000000 1000000000000 1000000000000
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1000000000000 1000000000000 100000000
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 1000000000000 1000000000000 100000
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle
```
Indeed it is again the trird argument making troubles (probably different type)!

##### Precision testing?
```
$ ./triangle 2 1 1
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 2 1 1.1
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 2 1 1.5
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 2 1 1.9
✗ 1 root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 2 1 2
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle

$ ./triangle 2 1 2.0
✓ root@freznicek-j ~/qa-crashcourse.git/src/triangle
```
Executions indicate that third argument is probably kept internally as integer. Would you be able to find a way how to aprove the other arguments are not the same case?


## How to create software defect

1. Follow the defect tracker template, [Bug #1 example](https://github.com/freznicek/qa-crashcourse/issues/3), [defect templates](issue_templates/).
1. Ask yourself what else you need to know if you are tasked to fix the defect.


## Summary / takeaways

The above lines clearly show the QA mindset in above practical testing experiments.

### The highlights:
 * figure out different testing scenarios based on desired functionality and experience
 * clearly decide whether the behavior is correct or not based on the facts
 * analyse the behavior and prove/disprove the hypothesis
 * understand programming basics (reading of source code)

### Practice yourself:
 * find other testing scenarios
 * find all above listed issues by making detailed  [triangle application code-review](src/triangle/triangle.cpp).
 * create precisely defined defect reports for other bugs following the [Bug #1 example](https://github.com/freznicek/qa-crashcourse/issues/3)

### Remember
>If you haven't tried it, assume it's broken.
>
>Unknown

## References
* articles
  * https://en.wikipedia.org/wiki/Software_testing
  * https://github.com/tiansiyuan/SQA
  * https://stackify.com/software-testing-tips/
  * http://www.agilebuddha.com/scrum/agile-testing-is-achieving-quality-in-everything/
  * http://swtester.blogspot.cz/2015/04/agile-testing-automation.html
* books
  * https://www.amazon.com/Software-Quality-Assurance-Theory-Implementation/dp/0201709457
  * https://www.amazon.com/Handbook-Software-Quality-Assurance-Fourth/dp/1596931868
  * http://www.netobjectives.com/files/books/lasd/role-quality-assurance-lean-agile-software-development.pdf
