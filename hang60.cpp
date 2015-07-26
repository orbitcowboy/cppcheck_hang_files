template < typename > struct A { } ; template < typename > struct A < INVALID > : A < int[ > { }] ; 
/*(gdb) bt
#0  0x08488b9e in Token::update_property_info (this=0xf630d10)
    at lib/token.cpp:77
#1  0x0841f1a4 in Token::str<std::string const&> (this=0xf630d10, s="{")
    at lib/token.h:77
#2  0x0848b656 in Token::insertToken (this=0xf630c98, tokenStr="{", 
    originalNameStr="", prepend=false) at lib/token.cpp:939
#3  0x084eb890 in TokenList::addtoken (this=0xbfffe1e8, tok=0xf6305e8, 
    lineno=1, fileno=0) at lib/tokenlist.cpp:163
#4  0x08477a66 in TemplateSimplifier::expandTemplate (tokenlist=..., 
    tok=0x8627ef8, name="A", 
    typeParametersInDeclaration=std::__debug::vector of length 1, capacity 1 = {...}, newName="A<INVALID>", 
    typesUsedInTemplateInstantiation=std::__debug::vector of length 1, capacity 1 = {...}, templateInstantiations=std::__debug::list = {...})
    at lib/templatesimplifier.cpp:821
#5  0x0847aba9 in TemplateSimplifier::simplifyTemplateInstantiations (
    tokenlist=..., errorlogger=0xbfffea40, _settings=0xbfffea58, 
    tok=0x8627ef8, templateInstantiations=std::__debug::list = {...}, 
    expandedtemplates=std::__debug::set with 1 elements = {...})
    at lib/templatesimplifier.cpp:1316
#6  0x0847b661 in TemplateSimplifier::simplifyTemplates (tokenlist=..., 
    errorlogger=0xbfffea40, _settings=0xbfffea58, 
    _codeWithTemplates=@0xbfffe24c: true) at lib/templatesimplifier.cpp:1429
#7  0x0849ea1f in Tokenizer::simplifyTemplates (this=0xbfffe1e8)
    at lib/tokenize.cpp:2435
#8  0x084a513e in Tokenizer::simplifyTokenList1 (this=0xbfffe1e8, 
    FileName=0x862243c "hang60.cpp") at lib/tokenize.cpp:3576
#9  0x0849ad84 in Tokenizer::tokenize (this=0xbfffe1e8, code=..., 
    FileName=0x862243c "hang60.cpp", configuration="", noSymbolDB_AST=false)
    at lib/tokenize.cpp:1742
#10 0x083d2b94 in CppCheck::checkFile (this=0xbfffea40, 
    code="template < typename > struct A { } ; template < typename > struct A < INVALID > : A < int[ > { }] ;\n", FileName=0x862243c "hang60.cpp", 
    checksums=std::__debug::set with 0 elements, 
    internalErrorFound=@0xbfffe4db: false) at lib/cppcheck.cpp:337
#11 0x083d18c9 in CppCheck::processFile (this=0xbfffea40, 
    filename="hang60.cpp", fileStream=...) at lib/cppcheck.cpp:239
#12 0x083d0380 in CppCheck::check (this=0xbfffea40, path="hang60.cpp")
    at lib/cppcheck.cpp:70
#13 0x082b67bf in CppCheckExecutor::check_internal (this=0xbfffefe4, 
    cppcheck=..., argv=0xbffff0f4) at cli/cppcheckexecutor.cpp:791
#14 0x082b5756 in CppCheckExecutor::check (this=0xbfffefe4, argc=2, 
    argv=0xbffff0f4) at cli/cppcheckexecutor.cpp:185
#15 0x082bd046 in main (argc=2, argv=0xbffff0f4) at cli/main.cpp:129

*/
