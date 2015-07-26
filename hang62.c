void crc() { int toread ; long long nleft ; unsigned char buf[(128 * 1024)] ; nleft = 0 ; while (toread = (nleft < (2147483647 * 2U + 1U)) ?) nleft : (2147483647 *( 2U + 1U) ) ; } 
/*(gdb) bt
#0  0x00000000006a2900 in std::string::_M_data (this=0xc5c5a0)
    at /usr/include/c++/4.9/bits/basic_string.h:293
#1  0x00000000006a291e in std::string::_M_rep (this=0xc5c5a0)
    at /usr/include/c++/4.9/bits/basic_string.h:301
#2  0x00000000006a3114 in std::string::size (this=0xc5c5a0)
    at /usr/include/c++/4.9/bits/basic_string.h:725
#3  0x00000000006a1507 in std::string::compare (this=0xc5c5a0, 
    __s=0x96fea7 "{") at /usr/include/c++/4.9/bits/basic_string.tcc:952
#4  0x00000000006a0de3 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (__lhs="char", __rhs=0x96fea7 "{")
    at /usr/include/c++/4.9/bits/basic_string.h:2540
#5  0x0000000000913bdb in getProgramMemory (tok=0xc5dde0, varid=2, value=...)
    at lib/valueflow.cpp:164
#6  0x0000000000919ed4 in valueFlowForward (startToken=0xc5cf30, 
    endToken=0xc5e940, var=0xc5f030, varid=2, 
    values=std::__debug::list = {...}, constValue=true, 
    tokenlist=0x7fffffffc130, errorLogger=0x7fffffffd150, 
    settings=0x7fffffffd180) at lib/valueflow.cpp:1091
#7  0x000000000091b664 in valueFlowAfterAssign (tokenlist=0x7fffffffc130, 
    symboldatabase=0xc5b600, errorLogger=0x7fffffffd150, 
    settings=0x7fffffffd180) at lib/valueflow.cpp:1256
#8  0x00000000009200ad in ValueFlow::setValues (tokenlist=0x7fffffffc130, 
    symboldatabase=0xc5b600, errorLogger=0x7fffffffd150,
#9  0x00000000007df63c in CppCheck::check (this=0x7fffffffd150, 
    path="hang62.c") at lib/cppcheck.cpp:70
#10 0x00000000006ad40c in CppCheckExecutor::check_internal (
    this=0x7fffffffdbf0, cppcheck=..., argv=0x7fffffffdda8)
    at cli/cppcheckexecutor.cpp:806
#11 0x00000000006ac179 in CppCheckExecutor::check (this=0x7fffffffdbf0, 
    argc=2, argv=0x7fffffffdda8) at cli/cppcheckexecutor.cpp:200
#12 0x00000000006b7301 in main (argc=2, argv=0x7fffffffdda8)
    at cli/main.cpp:129
* */
