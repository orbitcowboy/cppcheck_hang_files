void f ( int *i ) {[ if ( ! i ) return ; else] { __builtin_printf ( "Hi" ) ; *i=0 ; } } 
/*
#0  0x000000000078d738 in std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::compare (this=0xd8c0b8, __s=0xa64127 "return") at /usr/include/c++/5/bits/basic_string.tcc:1401
#1  0x000000000078cfb5 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs=";", __rhs=0xa64127 "return") at /usr/include/c++/5/bits/basic_string.h:4934
#2  0x00000000008b8a97 in UninitVar::parse (this=0xd8e6b0, tok=..., checks=std::__debug::list = {...}) at lib/checkuninitvar.cpp:758
#3  0x00000000008f1bbe in ExecutionPath::checkScope (tok=0xd8c080, checks=std::__debug::list = {...}) at lib/executionpath.cpp:458
#4  0x00000000008f17a0 in ExecutionPath::checkScope (tok=0xd8b840, checks=std::__debug::list = {...}) at lib/executionpath.cpp:429
#5  0x00000000008f1f11 in checkExecutionPaths (symbolDatabase=0xd87030, c=0x7fffffffb950) at lib/executionpath.cpp:481
#6  0x00000000008ae9e4 in CheckUninitVar::executionPaths (this=0x7fffffffb9e0) at lib/checkuninitvar.cpp:1040
#7  0x00000000008b4cb2 in CheckUninitVar::runSimplifiedChecks (this=0xd600a0 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0x7fffffffbcf0, settings=0x7fffffffcfe8, errorLogger=0x7fffffffcfb0)
    at lib/checkuninitvar.h:51
#8  0x00000000008daee9 in CppCheck::checkFile (this=0x7fffffffcfb0, code="void f ( int *i ) {[ if ( ! i ) return ; else] { __builtin_printf ( \"Hi\" ) ; *i=0 ; } }\n", FileName=0xd80150 "hang67.cpp", 
    checksums=std::__debug::set with 1 elements = {...}, internalErrorFound=@0x7fffffffc1bb: false) at lib/cppcheck.cpp:398
#9  0x00000000008d948f in CppCheck::processFile (this=0x7fffffffcfb0, filename="hang67.cpp", fileStream=...) at lib/cppcheck.cpp:239
#10 0x00000000008d7eac in CppCheck::check (this=0x7fffffffcfb0, path="hang67.cpp") at lib/cppcheck.cpp:70
#11 0x0000000000799132 in CppCheckExecutor::check_internal (this=0x7fffffffdae0, cppcheck=..., argv=0x7fffffffdc98) at cli/cppcheckexecutor.cpp:806
#12 0x0000000000797e53 in CppCheckExecutor::check (this=0x7fffffffdae0, argc=2, argv=0x7fffffffdc98) at cli/cppcheckexecutor.cpp:200
#13 0x00000000007a3560 in main (argc=2, argv=0x7fffffffdc98) at cli/main.cpp:129

 */
