extern "C" void abort () ; struct C { __complex__ long double c ; } ; void foo() { C x = {2+2i} ; int n = 1 ; C y = (n==1) ?} x : (C){3+3i}{ ; if (__imag__ y.c ! = 2) abort () ; } int main(void) { foo () ; return 0 ; } 