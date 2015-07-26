enum x : enum x{} () :
/*(gdb) bt
#0  std::__cxx1998::_Deque_base<Token*, std::allocator<Token*> >::_M_initialize_map (this=0x7fffffffb660, __num_elements=0)
    at /usr/include/c++/4.9/bits/stl_deque.h:618
#1  0x0000000000843002 in std::__cxx1998::_Deque_base<Token*, std::allocator<Token*> >::_Deque_base(std::__cxx1998::_Deque_base<Token*, std::allocator<Token*> >&&) (this=0x7fffffffb660, 
    __x=<unknown type in /usr/bin/cppcheck, CU 0xfd9773, DIE 0x1078fea>)
    at /usr/include/c++/4.9/bits/stl_deque.h:476
#2  0x000000000083d322 in std::__cxx1998::deque<Token*, std::allocator<Token*> >::deque(std::__cxx1998::deque<Token*, std::allocator<Token*> >&&) (
    this=0x7fffffffb660, 
    __x=<unknown type in /usr/bin/cppcheck, CU 0xfd9773, DIE 0x1070141>)
    at /usr/include/c++/4.9/bits/stl_deque.h:861
#3  0x0000000000838dc3 in std::__debug::deque<Token*, std::allocator<Token*> >::deque(std::__debug::deque<Token*, std::allocator<Token*> >&&) (
    this=0x7fffffffb660, 
    __x=<unknown type in /usr/bin/cppcheck, CU 0xfd9773, DIE 0x10682b1>)
    at /usr/include/c++/4.9/debug/deque:114
#4  0x0000000000834b7a in std::stack<Token*, std::__debug::deque<Token*, std::allocator<Token*> > >::stack(std::__debug::deque<Token*, std::allocator<Token*> >&&) (this=0x7fffffffb660, 
    __c=<unknown type in /usr/bin/cppcheck, CU 0xfd9773, DIE 0x10604b8>)
    at /usr/include/c++/4.9/bits/stl_stack.h:142
#5  0x00000000008b0dfa in Tokenizer::copyTokens (dest=0xe1c67d0, 
    first=0xc5bc10, last=0xc5bc10, one_line=true) at lib/tokenize.cpp:163
#6  0x00000000008d893b in Tokenizer::simplifyEnum (this=0x7fffffffc130)
    at lib/tokenize.cpp:7982
#7  0x00000000008c30f5 in Tokenizer::simplifyTokenList1 (this=0x7fffffffc130, 
    FileName=0xc58a38 "hang51.c") at lib/tokenize.cpp:3497
#8  0x00000000008b9393 in Tokenizer::tokenize (this=0x7fffffffc130, code=..., 
    FileName=0xc58a38 "hang51.c", configuration="", noSymbolDB_AST=false)
    at lib/tokenize.cpp:1742
#9  0x00000000007e1e70 in CppCheck::checkFile (this=0x7fffffffd150, 
    code="enum x : enum x{} () :\n", FileName=0xc58a38 "hang51.c", 
    checksums=std::__debug::set with 0 elements, 
    internalErrorFound=@0x7fffffffc69b: false) at lib/cppcheck.cpp:337
#10 0x00000000007e0bce in CppCheck::processFile (this=0x7fffffffd150, 
    filename="hang51.c", fileStream=...) at lib/cppcheck.cpp:239
#11 0x00000000007df63c in CppCheck::check (this=0x7fffffffd150, 
    path="hang51.c") at lib/cppcheck.cpp:70
#12 0x00000000006ad40c in CppCheckExecutor::check_internal (
    this=0x7fffffffdbf0, cppcheck=..., argv=0x7fffffffdda8)
    at cli/cppcheckexecutor.cpp:806
#13 0x00000000006ac179 in CppCheckExecutor::check (this=0x7fffffffdbf0, 
    argc=2, argv=0x7fffffffdda8) at cli/cppcheckexecutor.cpp:200
#14 0x00000000006b7301 in main (argc=2, argv=0x7fffffffdda8)
....*/
