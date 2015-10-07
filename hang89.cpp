int f(){ 
	int i , j , k , x 0 ;    
	0 ;    
	for ( i = t3 , i < t1 ; i++ ) 
	  for ( j = 0 ; j < = j++ ) 
	    extern return t1 ,        
}
/*(gdb) bt
#0  Token::Match (tok=0x861cb78, pattern=0x8538527 "%oror%|&&|?|:", varid=0)
    at lib/token.cpp:655
#1  0x084e1197 in skipValueInConditionalExpression (valuetok=0x861cdd0)
    at lib/valueflow.cpp:247
#2  0x084e87cb in valueFlowForward (startToken=0x861cc68, endToken=0x861d268, 
    var=0x861d408, varid=2, values=std::__debug::list = {...}, 
    constValue=true, tokenlist=0xbfffe198, errorLogger=0xbfffe9f0, 
    settings=0xbfffea08) at lib/valueflow.cpp:1386
#3  0x084e9d9f in valueFlowAfterAssign (tokenlist=0xbfffe198, 
    symboldatabase=0x861bf80, errorLogger=0xbfffe9f0, settings=0xbfffea08)
    at lib/valueflow.cpp:1522
#4  0x084ef1cb in ValueFlow::setValues (tokenlist=0xbfffe198, 
    symboldatabase=0x861bf80, errorLogger=0xbfffe9f0, settings=0xbfffea08)
    at lib/valueflow.cpp:2295
#5  0x08491999 in Tokenizer::tokenize (this=0xbfffe198, code=..., 
    FileName=0x8618cbc "hang89.cpp", configuration="", noSymbolDB_AST=false)
    at lib/tokenize.cpp:1730
#6  0x083cb38a in CppCheck::checkFile (this=0xbfffe9f0, 
    code="int f(){\nint i , j , k , x 0 ;\n0 ;\nfor ( i = t3 , i < t1 ; i++ )\nfor ( j = 0 ; j < = j++ )\nextern return t1 ,\n}\n", 
    FileName=0x8618cbc "hang89.cpp", 
    checksums=std::__debug::set with 0 elements, 
    internalErrorFound=@0xbfffe48b: false) at lib/cppcheck.cpp:337
---Type <return> to continue, or q <return> to quit---
#7  0x083ca0bf in CppCheck::processFile (this=0xbfffe9f0, 
    filename="hang89.cpp", fileStream=...) at lib/cppcheck.cpp:239
#8  0x083c8b76 in CppCheck::check (this=0xbfffe9f0, path="hang89.cpp")
    at lib/cppcheck.cpp:70
#9  0x082b79b3 in CppCheckExecutor::check_internal (this=0xbfffef94, 
    cppcheck=..., argv=0xbffff0a4) at cli/cppcheckexecutor.cpp:810
#10 0x082b6830 in CppCheckExecutor::check (this=0xbfffef94, argc=2, 
    argv=0xbffff0a4) at cli/cppcheckexecutor.cpp:183
#11 0x082be268 in main (argc=2, argv=0xbffff0a4) at cli/main.cpp:136
*/
