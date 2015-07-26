template 
<int I> void f(int I> {} {} <int (*)[2]); g(int 
template f<1>(int void template (*)[2]) template void void (*)[I+2]) g<1>(int 
(*)[3]); 

/*(gdb) bt
#0  _int_malloc (av=av@entry=0xb7e1e420 <main_arena>, bytes=bytes@entry=88)
    at malloc.c:3775
#1  0xb7ce8f98 in __GI___libc_malloc (bytes=88) at malloc.c:2891
#2  0xb7ed23d4 in operator new(unsigned int) ()
   from /usr/lib/i386-linux-gnu/libstdc++.so.6
#3  0x083ef1e4 in Token::insertToken (this=0x457e8f78, tokenStr=..., 
    originalNameStr=..., prepend=false) at lib/token.cpp:867
#4  0x08441ef0 in TokenList::addtoken (this=0xbfffe308, tok=0x457e7140, 
    lineno=4, fileno=0) at lib/tokenlist.cpp:163
#5  0x083dcf92 in TemplateSimplifier::expandTemplate (tokenlist=..., 
    tok=0x854c6a8, name=..., typeParametersInDeclaration=..., newName=..., 
    typesUsedInTemplateInstantiation=..., templateInstantiations=...)
    at lib/templatesimplifier.cpp:769
#6  0x083e00fe in TemplateSimplifier::simplifyTemplateInstantiations (
    tokenlist=..., errorlogger=0xbfffeb0c, _settings=0xbfffeb24, 
    tok=0x854c6a8, templateInstantiations=..., expandedtemplates=...)
    at lib/templatesimplifier.cpp:1266
#7  0x083e0bc0 in TemplateSimplifier::simplifyTemplates (tokenlist=..., 
    errorlogger=0xbfffeb0c, _settings=0xbfffeb24, 
    _codeWithTemplates=@0xbfffe36c: true) at lib/templatesimplifier.cpp:1379
#8  0x083fff07 in Tokenizer::simplifyTemplates (this=0xbfffe308)
    at lib/tokenize.cpp:2250
#9  0x08406ec3 in Tokenizer::simplifyTokenList1 (this=0xbfffe308, */
