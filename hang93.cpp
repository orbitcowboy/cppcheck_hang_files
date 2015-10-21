operator - h { } typedef typename x ; tyjenamf typename x +
/*(gdb) bt
#0  0x00000000007856a4 in std::char_traits<char>::length (__s=0xa5adbb "==")
    at /usr/include/c++/5/bits/char_traits.h:263
#1  0x0000000000786593 in std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::compare (this=0x30b21988, __s=0xa5adbb "==") at /usr/include/c++/5/bits/basic_string.tcc:1398
#2  0x0000000000785e57 in std::operator==<char, std::char_traits<char>, std::allocator<char> > (
    __lhs=";", __rhs=0xa5adbb "==") at /usr/include/c++/5/bits/basic_string.h:4934
#3  0x000000000098d8d5 in Token::update_property_info (this=0x30b21950) at lib/token.cpp:94
#4  0x00000000009170f9 in Token::str<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&> (this=0x30b21950, s=";") at lib/token.h:79
#5  0x0000000000990193 in Token::insertToken (this=0x30b21880, tokenStr=";", prepend=false)
    at lib/token.cpp:878
#6  0x0000000000997a96 in Tokenizer::copyTokens (dest=0xd6aec0, first=0xd6ac50, last=0xd6aab0, 
    one_line=true) at lib/tokenize.cpp:162
#7  0x000000000099d9ed in Tokenizer::simplifyTypedef (this=0x7fffffffbe40) at lib/tokenize.cpp:1292
#8  0x00000000009aa2d5 in Tokenizer::simplifyTokenList1 (this=0x7fffffffbe40, 
    FileName=0xd607f0 "hang93.cpp") at lib/tokenize.cpp:3444
#9  0x00000000009a024b in Tokenizer::tokenize (this=0x7fffffffbe40, code=..., 
    FileName=0xd607f0 "hang93.cpp", configuration="", noSymbolDB_AST=false) at lib/tokenize.cpp:1716
#10 0x00000000008c1a74 in CppCheck::checkFile (this=0x7fffffffd100, 
    code="operator - h { } typedef typename x ; tyjenamf typename x +\n", 
    FileName=0xd607f0 "hang93.cpp", checksums=std::__debug::set with 0 elements, 
    internalErrorFound=@0x7fffffffc30b: false) at lib/cppcheck.cpp:337
#11 0x00000000008c0853 in CppCheck::processFile (this=0x7fffffffd100, filename="hang93.cpp", 
    fileStream=...) at lib/cppcheck.cpp:239
#12 0x00000000008bf270 in CppCheck::check (this=0x7fffffffd100, path="hang93.cpp") at lib/cppcheck.cpp:70
#13 0x0000000000791d6a in CppCheckExecutor::check_internal (this=0x7fffffffdc30, cppcheck=..., 
    argv=0x7fffffffdde8) at cli/cppcheckexecutor.cpp:810
#14 0x000000000079092f in CppCheckExecutor::check (this=0x7fffffffdc30, argc=2, argv=0x7fffffffdde8)
    at cli/cppcheckexecutor.cpp:183
#15 0x000000000079927a in main (argc=2, argv=0x7fffffffdde8) at cli/main.cpp:136
*/
