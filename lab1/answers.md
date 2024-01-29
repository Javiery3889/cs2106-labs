## Question 1.1  
> The #include “queue.h” statement looks for the queue.h file in the currently 
> directory, where does the #include <stdio.h> statement look for the stdio.h file?  

It looks for it in the standard C library in a predefined library. 
On Linux it could be stored in a directory in /usr/include/ or /usr/lib.  
The exact location depends on the implementation and version of C used and the platform.  

## Question 1.2
> You will notice that _queue, _front and _rear are declared as “static”. What does the 
> “static” declaration mean?  

Static is used with global variables and functions to set their scope to the containing file. In local variables, static is used to store the variable in the statically allocated memory instead of the automatically allocated memory. While the language does not dictate the implementation of either type of memory, statically allocated memory is typically reserved in data segment of the program at compile time, while the automatically allocated memory is normally implemented as a transient call stack.

## Question 1.3
> What causes this error (or warning)?  

deq and enq is not explicitly declared within lab1p1.c itself, but rather included in queue. However
it is not explicitly defined in queue.h either.

## Question 1.4
> Copy and paste the prototypes for enq and deq you defined in queue.h here  

void enq(double data);
double deq();

## Question 2.1
| Variable | Global / Local | Address        |
|----------|----------------|----------------|
| p1       |  G             | 0x5566d6d27018 |
| p2       |  G             | 0x5566d6d27020 |
| p3       |  G             | 0x5566d6d27028 |
| p4       |  G             | 0x5566d6d27030 |
| w        |  G             | 0x5566d6d27038 |
| x        |  G             | 0x7ffc2145c6ac |
| y        |  G             | 0x7ffc2145c6a8 |
| z        |  G             | 0x7ffc2145c6b4 |

## Question 2.2
| Variable | Location (S, D, T or H) |
|----------|-------------------------|
| p1       |  D                      |
| p2       |  D                      |
| p3       |  D                      |
| p4       |  D                      |
| w        |  D                      |
| x        |  S                      |
| y        |  S                      |
| z        |  S                      |

p1, p2, p3, p4, and w, all have the same prefix for their memory meaning they are near each other. As they are global variables and static, it means they are in data. 

x,y,z are local variables so they will be in the stack frame of fun1 

## Question 2.3
Static variables are created in the data segment. This allows their value to be preserved even after their function has returned, which would delete the stack frame of that function. Storing it in data allows this to be avoided 

## Question 2.4
It means that that variable lifetime is no longer limited to the lifetime of its function, but can persist throughout the duration of the program. The scope is still within a function and cannot be accessed outside the function directly. 

Static in a global variable means the global variable is scoped to the current file and cannot be accessed from elsewhere.  

## Question 2.5
Added the static key word to acc: 

  static int acc = 0; 

This allows the value of acc to be persisted. Because static variables are only initialized once, the value do not get wiped out every call and it can keep the value are the end of each accumulate call. 

## Question 3.2

gcc -g testlist.c llist.c -o testlist

