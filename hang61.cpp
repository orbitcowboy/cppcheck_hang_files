void f (int x) { if (x) ; if (x) ; else (void)0; if (x) (void)0; else ;( if (x) (void)0; else) (void)0; }
/*(gdb) bt 
#0  0x00007ffff7914169 in ?? () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#1  0x00007ffff79141be in ?? () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#2  0x00000000006a0bc6 in __gnu_debug::_Safe_iterator_base::_Safe_iterator_base
    (this=0x7fffffffba90, __seq=0x7fffffffbe40, __constant=false)
    at /usr/include/c++/4.9/debug/safe_base.h:89
Python Exception <class 'ValueError'> Cannot find type const std::__cxx1998::_List_iterator<ExecutionPath*>::_Node: 
#3  0x00000000007acef1 in __gnu_debug::_Safe_iterator<std::__cxx1998::_List_iterator<ExecutionPath*>, std::__debug::list<ExecutionPath*, std::allocator<ExecutionPath*> > >::_Safe_iterator (this=0x7fffffffba90, __i=, __seq=0x7fffffffbe30)
    at /usr/include/c++/4.9/debug/safe_iterator.h:149
#4  0x00000000007aa69e in std::__debug::list<ExecutionPath*, std::allocator<ExecutionPath*> >::begin (this=0x7fffffffbe30)
    at /usr/include/c++/4.9/debug/list:189
#5  0x00000000007bead1 in UninitVar::use (checks=std::__debug::list = {...}, 
    tok=0xc5e910, mode=0) at lib/checkuninitvar.cpp:251
#6  0x00000000007bef7d in UninitVar::use (checks=std::__debug::list = {...}, 
    tok=0xc5e910) at lib/checkuninitvar.cpp:310
#7  0x00000000007c1e8d in UninitVar::parseCondition (this=0xc5c190, tok=..., 
    checks=std::__debug::list = {...}) at lib/checkuninitvar.cpp:878
#8  0x00000000007f66b2 in ExecutionPath::checkScope (tok=0xc5e840, 
    checks=std::__debug::list = {...}) at lib/executionpath.cpp:391
#9  0x00000000007f7025 in checkExecutionPaths (symbolDatabase=0xc60170, 
    c=0x7fffffffbe90) at lib/executionpath.cpp:481
#10 0x00000000007b7628 in CheckUninitVar::executionPaths (this=0x7fffffffbf20)
#11 0x00000000007bd714 in CheckUninitVar::runSimplifiedChecks (
    this=0xc41340 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0x7fffffffc130, 
    settings=0x7fffffffd180, errorLogger=0x7fffffffd150)
    at lib/checkuninitvar.h:51
#12 0x00000000007e2ad2 in CppCheck::checkFile (this=0x7fffffffd150, 
    code="void f (int x) { if (x) ; if (x) ; else (void)0; if (x) (void)0; else ;( if (x) (void)0; else) (void)0; }\n", FileName=0xc59a38 "hang61.cpp", 
    checksums=std::__debug::set with 1 elements = {...}, 
    internalErrorFound=@0x7fffffffc69b: false) at lib/cppcheck.cpp:398
#13 0x00000000007e103e in CppCheck::processFile (this=0x7fffffffd150, 
    filename="hang61.cpp", fileStream=...) at lib/cppcheck.cpp:239
#14 0x00000000007dfaac in CppCheck::check (this=0x7fffffffd150, 
    path="hang61.cpp") at lib/cppcheck.cpp:70
#15 0x00000000006ad87c in CppCheckExecutor::check_internal (
    this=0x7fffffffdbf0, cppcheck=..., argv=0x7fffffffdda8)
    at cli/cppcheckexecutor.cpp:806
#16 0x00000000006ac5e9 in CppCheckExecutor::check (this=0x7fffffffdbf0, 
    argc=2, argv=0x7fffffffdda8) at cli/cppcheckexecutor.cpp:200
#17 0x00000000006b7771 in main (argc=2, argv=0x7fffffffdda8)
    at cli/main.cpp:129
*/ 
