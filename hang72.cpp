void TestFunction ()
{
	test.Enumerate ([&] (const Object obj) -> void {
		if (IsTrue (obj)) {
			// do nothing
		} else if (false) {
			// do nothing
		}
	});
}
/*(gdb) bt
#0  0x00007ffff789e5c8 in ?? () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#1  0x00007ffff789e669 in __gnu_debug::_Safe_sequence_base::_M_get_mutex() ()
   from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#2  0x00007ffff789e927 in ?? () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#3  0x00000000008f36aa in __gnu_debug::_Safe_iterator<std::__cxx1998::_List_iterator<ExecutionPath*>, std::__debug::list<ExecutionPath*, std::allocator<ExecutionPath*> > >::_M_attach (this=0x7fffffffb440, __seq=0x7fffffffb900)
    at /usr/include/c++/5/debug/safe_iterator.h:451
#4  0x00000000008f2ee9 in __gnu_debug::_Safe_iterator<std::__cxx1998::_List_iterator<ExecutionPath*>, std::__debug::list<ExecutionPath*, std::allocator<ExecutionPath*> > >::_Safe_iterator(__gnu_debug::_Safe_iterator<std::__cxx1998::_List_iterator<ExecutionPath*>, std::__debug::list<ExecutionPath*, std::allocator<ExecutionPath*> > >&&) (this=0x7fffffffb440, 
    __x=<unknown type in /usr/bin/cppcheck, CU 0x100e39b, DIE 0x1044cb9>)
    at /usr/include/c++/5/debug/safe_iterator.h:202
#5  0x00000000008f3cd1 in std::_Iter_base<__gnu_debug::_Safe_iterator<std::__cxx1998::_List_iterator<ExecutionPath*>, std::__debug::list<ExecutionPath*, std::allocator<ExecutionPath*> > >, false>::_S_base (
    __it=<error reading variable: Cannot access memory at address 0x10>)
    at /usr/include/c++/5/bits/stl_iterator_base_types.h:216
#6  0x00000000008f3d1c in std::__niter_base<__gnu_debug::_Safe_iterator<std::__cxx1998::_List_iterator<ExecutionPath*>, std::__debug::list<ExecutionPath*, std::allocator<ExecutionPath*> > > > (__it=)
#7  0x00000000008f3a19 in std::__copy_move_a2<false, __gnu_debug::_Safe_iterator<std::__cxx1998::_List_iterator<ExecutionPath*>, std::__debug::list<ExecutionPath*, std::allocator<ExecutionPath*> > >, std::back_insert_iterator<std::__debug::list<ExecutionPath*, std::allocator<ExecutionPath*> > > > (__first=, __last=, 
    __result=...) at /usr/include/c++/5/bits/stl_algobase.h:436
#8  0x00000000008f3231 in std::copy<__gnu_debug::_Safe_iterator<std::__cxx1998::_List_iterator<ExecutionPath*>, std::__debug::list<ExecutionPath*, std::allocator<ExecutionPath*> > >, std::back_insert_iterator<std::__debug::list<ExecutionPath*, std::allocator<ExecutionPath*> > > > (__first=, __last=, __result=...)
    at /usr/include/c++/5/bits/stl_algobase.h:470
#9  0x00000000008f24e2 in ExecutionPath::checkScope (tok=0xd8c9d0, 
    checks=std::__debug::list = {...}) at lib/executionpath.cpp:438
#10 0x00000000008f2baf in checkExecutionPaths (symbolDatabase=0xd87d80, 
    c=0x7fffffffba60) at lib/executionpath.cpp:481
#11 0x00000000008af63c in CheckUninitVar::executionPaths (this=0x7fffffffbaf0)
    at lib/checkuninitvar.cpp:1040
#12 0x00000000008b5950 in CheckUninitVar::runSimplifiedChecks (
    this=0xd610a0 <_ZN12_GLOBAL__N_18instanceE>, tokenizer=0x7fffffffbe00, 
    settings=0x7fffffffd0f8, errorLogger=0x7fffffffd0c0)
    at lib/checkuninitvar.h:51
#13 0x00000000008dbb87 in CppCheck::checkFile (this=0x7fffffffd0c0, 
    code="void TestFunction ()\n{\ntest.Enumerate ([&] (const Object obj) -> void{\nif (IsTrue (obj)) {\n\n} else if (false) {\n\n}\n});\n}\n", 
    FileName=0xd81290 "hang72.cpp", 
    checksums=std::__debug::set with 1 elements = {...}, 
    internalErrorFound=@0x7fffffffc2cb: false) at lib/cppcheck.cpp:398
#14 0x00000000008da12d in CppCheck::processFile (this=0x7fffffffd0c0, 
    filename="hang72.cpp", fileStream=...) at lib/cppcheck.cpp:239
#15 0x00000000008d8b4a in CppCheck::check (this=0x7fffffffd0c0, 
    path="hang72.cpp") at lib/cppcheck.cpp:70
#16 0x0000000000799412 in CppCheckExecutor::check_internal (
    this=0x7fffffffdbf0, cppcheck=..., argv=0x7fffffffdda8)
    at cli/cppcheckexecutor.cpp:806
#17 0x0000000000798133 in CppCheckExecutor::check (this=0x7fffffffdbf0, 
    argc=2, argv=0x7fffffffdda8) at cli/cppcheckexecutor.cpp:200
#18 0x00000000007a3840 in main (argc=2, argv=0x7fffffffdda8)
    at cli/main.cpp:129
*/
