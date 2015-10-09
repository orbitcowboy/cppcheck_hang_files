int foo  (  int align  )   {  int off  (  =  0  %  align  ;  return off  )  ?  \  align - off  :  0  ;  \   }   
/*
(gdb) run --enable=all hang91.cpp 
Starting program: /usr/bin/cppcheck --enable=all hang91.cpp
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/i386-linux-gnu/libthread_db.so.1".
Checking hang91.cpp...
^C
Program received signal SIGINT, Interrupt.
0x082ad3d4 in std::string::compare (this=0x861c388, __s=0x8518767 "?")
    at /usr/include/c++/4.9/bits/basic_string.tcc:952
952	      const size_type __size = this->size();
(gdb) bt
#0  0x082ad3d4 in std::string::compare (this=0x861c388, __s=0x8518767 "?")
    at /usr/include/c++/4.9/bits/basic_string.tcc:952
#1  0x082acd0f in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs="return", __rhs=0x8518767 "?")
    at /usr/include/c++/4.9/bits/basic_string.h:2540
#2  0x083754bd in CheckOther::checkRedundantAssignment (this=0xbfffe098)
    at lib/checkother.cpp:487
#3  0x0838305c in CheckOther::runChecks (
    this=0x8605d80 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0xbfffe188, 
    settings=0xbfffe9f8, errorLogger=0xbfffe9e0) at lib/checkother.h:56
#4  0x083cb7db in CppCheck::checkFile (this=0xbfffe9e0, 
    code="int foo ( int align ) { int off ( = 0 % align ; return off ) ? \\ align - off : 0 ; \\ }\n", FileName=0x8618cbc "hang91.cpp", 
    checksums=std::__debug::set with 1 elements = {...}, 
    internalErrorFound=@0xbfffe47b: false) at lib/cppcheck.cpp:371
#5  0x083ca0fb in CppCheck::processFile (this=0xbfffe9e0, 
    filename="hang91.cpp", fileStream=...) at lib/cppcheck.cpp:239
#6  0x083c8bb2 in CppCheck::check (this=0xbfffe9e0, path="hang91.cpp")
    at lib/cppcheck.cpp:70
#7  0x082b7973 in CppCheckExecutor::check_internal (this=0xbfffef84, 
    cppcheck=..., argv=0xbffff094) at cli/cppcheckexecutor.cpp:810
#8  0x082b67f0 in CppCheckExecutor::check (this=0xbfffef84, argc=3, 
    argv=0xbffff094) at cli/cppcheckexecutor.cpp:183
 */
