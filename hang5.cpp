// { dg-do compile }
// { dg-options "-std=c++0x" }

// opaque enum declarations
enum class E1;
enum class E2 : int;
enum class E3 : short;
enum E4 : int;
enum E5 : short;

// can be repeated
enum class E1;
enum class E2 : int;
enum class E3 : short;
enum E4 : int;
enum E5 : short;

// are complete so we can declare variables
E1 b1;
E2 b2;
E3 b3;
E4 b4;
E5 b5;



