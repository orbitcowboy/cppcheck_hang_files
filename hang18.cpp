template<typename> 
struct A {}; template<typename...T> struct A<T::T...> {}; A<int> a; 
/*(gdb) bt
#0  _int_malloc (av=av@entry=0x7ffff7339c00 <main_arena>, 
    bytes=bytes@entry=152) at malloc.c:3769
#1  0x00007ffff6ff950e in __GI___libc_malloc (bytes=152) at malloc.c:2895
#2  0x00007ffff78bc698 in operator new(unsigned long) ()
   from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#3  0x00000000008aa1a0 in Token::insertToken (this=0x24643eb0, tokenStr="::", 
    originalNameStr="", prepend=false) at lib/token.cpp:938
#4  0x000000000090ec5c in TokenList::addtoken (this=0x7fffffffc130, 
    tok=0x107c2b80, lineno=2, fileno=0) at lib/tokenlist.cpp:163
#5  0x000000000089382c in TemplateSimplifier::expandTemplate (tokenlist=..., 
    tok=0xc5c7f0, name="A", 
    typeParametersInDeclaration=std::__debug::vector of length 1, capacity 1 = {...}, 
    newName="A<int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int::int"..., 
    typesUsedInTemplateInstantiation=std::__debug::vector of length 1, capacity 1 = {...}, templateInstantiations=std::__debug::list = {...})
    at lib/templatesimplifier.cpp:803
#6  0x00000000008968ea in TemplateSimplifier::simplifyTemplateInstantiations (
    tokenlist=..., errorlogger=0x7fffffffd150, _settings=0x7fffffffd180, 
    tok=0xc5c7f0, templateInstantiations=std::__debug::list = {...}, 
    expandedtemplates=std::__debug::set with 20 elements = {...})
---Type <return> to continue, or q <return> to quit---
    at lib/templatesimplifier.cpp:1311
#7  0x0000000000897331 in TemplateSimplifier::simplifyTemplates (
    tokenlist=..., errorlogger=0x7fffffffd150, _settings=0x7fffffffd180, 
    _codeWithTemplates=@0x7fffffffc1f4: true)
    at lib/templatesimplifier.cpp:1424
#8  0x00000000008bcc5f in Tokenizer::simplifyTemplates (this=0x7fffffffc130)
    at lib/tokenize.cpp:2426
#9  0x00000000008c3345 in Tokenizer::simplifyTokenList1 (this=0x7fffffffc130, 
    FileName=0xc58a38 "hang18.cpp") at lib/tokenize.cpp:3570
#10 0x00000000008b9393 in Tokenizer::tokenize (this=0x7fffffffc130, code=..., 
    FileName=0xc58a38 "hang18.cpp", configuration="", noSymbolDB_AST=false)
    at lib/tokenize.cpp:1742
#11 0x00000000007e1e70 in CppCheck::checkFile (this=0x7fffffffd150, 
    code="template<typename>\nstruct A {}; template<typename...T> struct A<T::T...> {}; A<int> a;\n", FileName=0xc58a38 "hang18.cpp", 
    checksums=std::__debug::set with 0 elements, 
    internalErrorFound=@0x7fffffffc69b: false) at lib/cppcheck.cpp:337
#12 0x00000000007e0bce in CppCheck::processFile (this=0x7fffffffd150, 
    filename="hang18.cpp", fileStream=...) at lib/cppcheck.cpp:239
#13 0x00000000007df63c in CppCheck::check (this=0x7fffffffd150, 
    path="hang18.cpp") at lib/cppcheck.cpp:70
#14 0x00000000006ad40c in CppCheckExecutor::check_internal (
    this=0x7fffffffdbf0, cppcheck=..., argv=0x7fffffffdda8)
....*/
