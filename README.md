# StackTracer
A simple single-headered program to prints out the call stacks

# Platform Support
* Currently only supports windows applications for both x86 and x64 architecture
* Linux support will be added later

# Usage
The header file `StackTrace.hpp` contains a function `StackTrace()` that will output the stack calls in form of `std::vector<std::string>>`

# Example
An example of the output: 
```
  [0] stackTrace
  [1] IException::calculateStack_
  [2] IException::IException
  [3] foo3
  [4] foo2
  [5] foo1
  [6] main
  [7] invoke_main
  [8] __scrt_common_main_seh
  [9] __scrt_common_main
  [10] mainCRTStartup
  [11] BaseThreadInitThunk
  [12] RtlUserThreadStart
```
