template<typename> 
struct A {}; template<typename...T> struct A<T::T...> {}; A<int> a; 
