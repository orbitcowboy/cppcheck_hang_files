extern void xxx (int) __attribute__((noreturn));
typedef void voidfn (int);
__volatile extern voidfn xxx;

extern void xxx (int) __attribute__((noreturn));
enum {typedef void voidfn (int)};
__volatile extern voidfn xxx;
extern void xxx (int) __attribute__((noreturn));
typedef void voidfn (int);
__volatile extern voidfn xxx;
// { dg-options "-std=gnu++0x" }

struct Something {};
Something ___;
const ( static_assert < mutable mutable #define unsigned not_eq enum || this std:: { typename register typename explicit protected const nullptr #if if try typeid >= % nullptr enum namespace union static_assert virtual alignof2 typedef double static_cast signed if / typename operator } namespace and_eq compl std:: catch && bool #end #elif delete1 false ( false typeid constexpr goto goto void static_assert enum ] #elif } std::vector int catch goto case std::list mutable true static decltype decltype while vector register *= #define or #if throw and_eq dynamic_cast for #end volatile xor_eq typeid public std::list true auto #elif asm typedef else

                            template <class F>
                            struct Trial
                {
                    F f;
                    public:
                    Trial() : f() {}
                        Trial( const F& ff ) : f(ff) { }
                        template <typename... Args>
                        struct Sig
                        {
                            typedef int ResultType;
                        };

                        template <typename... Args>
                        struct Sig<Something,Args...>
                        {
                            typedef int ResultType;
                        };

                        template <typename... Args>
                        typename Sig<Something,Args...>::ResultType operator()(const Something& s,
                                const Args&... args) const
                        {
                            return f(args...);
                        }
                        template <typename... Args>
                        typename Sig<Args...>::ResultType operator()(const Args&... args) const
                        {
                            return f(args...);
                        }
                    };

struct Internal
{

    template <typename... Args>
    struct Sig
    {
        typedef int ResultType;
    };

    template <typename... Args>
    struct Sig<Something,Args...>
    {
        typedef int ResultType;
    };

    template <typename... Args>
    int operator()(const Args&... args) const
    {
        int n = sizeof...(Args);
        return n;
    }

    static Trial<Internal>& full()
    {
        static Trial<Internal> f;
        return f;
    }
};

static Trial<Internal>& internal = Internal::full();

                                   int main()
{
    int n = 0;
    n = internal(___,1,2,3);
    return 0;
}

// { dg-options "-std=gnu++0x" }

struct Something {};
Something ___;

template <class F>
struct Trial
{
    F f;
public:
    Trial() : f() {}
    Trial( const F& ff ) : f(ff) { }
    template <typename... Args>
    struct Sig
    {
        typedef int ResultType;
    };

    template <typename... Args>
    struct Sig<Something,Args...>
    {
        typedef int ResultType;
    };

    template <typename... Args>
    typename Sig<Something,Args...>::ResultType operator()(const Something& s,
            const Args&... args) const
    {
        return f(args...);
    }
    template <typename... Args>
    typename Sig<Args...>::ResultType operator()(const Args&... args) const
    {
        return f(args...);
    }
};

struct Internal
{

    template <typename... Args>
    struct Sig
    {
        typedef int ResultType;
    };

    template <typename... Args>
    struct Sig<Something,Args...>
    {
        typedef int ResultType;
    };

    template <typename... Args>
    int operator()(const Args&... args) const
    {
        int n = sizeof...(Args);
        return n;
    }

    static Trial<Internal>& full()
    {
        static Trial<Internal> f;
        return f;
    }
};

static Trial<Internal>& internal = Internal::full();

                                   int main()
{
    int n = 0;
    n = internal(___,1,2,3);
    return 0;
}

// { dg-do run  }
template<class T> class D
{
public:
    int f();
};

template<class T> int D<T>::f()
{
    return sizeof(T);
}

template<template<class> class E,class D> class C : E<D>
{
public:
    int f();
};

template<template<class> class E,class D> int C<E,D>::f()
{
    return E<D>::f();
}

class E : C<D,int>
{
public:
    int f()
    {
        return C< ::D,int>::f();
    }
};

int main()
{
    E c;
    c.f();
}

// Test for range-based for loop
// Test the loop with a custom iterator
// with begin/end in std

// { dg-do compile }
// { dg-options "-std=c++0x" }

struct iterator
{
    int x;
    iterator(int v) :x(v) {}
    iterator &operator ++()
    {
        ++x;
        return *this;
    }
    int operator *()
    {
        return x;
    }
    bool operator != (const iterator &o)
    {
        return x != o.x;
    }
};

struct container
{
    int min, max;
    container(int a, int b) :min(a), max(b) {}
};

namespace std
{
    iterator begin(container &c)
    {
        return iterator(c.min);
    }

    iterator end(container &c)
    {
        return iterator(c.max + 1);
    }
}

int main()
{
    container c(1,4);
    for (iterator it : c)
    {
    }
}

// { dg-options "-std=c++0x" }
template<int...> struct A
{
    void foo();
};

struct B
{
    template<int N> friend void A<N>::A::foo(); // { dg-error "declared as friend" }
};
namespace foo
{

    const cl_idecoded_float (integer_decode_float) (const cl_DF& x)
    {

        return cl_idecoded_float(
            Q_to_I(mant), // Mantisse (>0, <2^53) als Bignum
            L_to_FN(exp-(DF_mant_len+1)), // e-53 als Fixnum
            (sign>=0 ? cl_I(1) : cl_I(-1)) // (-1)^s erzeugen
        );

    }
}
namespace foo
{

    const cl_idecoded_float (integer_decode_float) (const cl_DF& x)
    {

        return cl_idecoded_float(
            Q_to_I(mant), // Mantisse (>0, <2^53) als Bignum
            L_to_FN(exp-(DF_mant_len+1)), // e-53 als Fixnum
            (sign>=0 ? cl_I(1) : cl_I(-1)) // (-1)^s erzeugen
        );

    }
}
# ifdef THREADS
#   ifdef PCR
PCR_Th_ML GC_allocate_ml;
#   else
#     ifdef SRC_M3
#     else
#	ifdef GC_SOLARIS_THREADS
mutex_t GC_allocate_ml;	/* Implicitly initialized.	*/
#	else
#          if defined(GC_WIN32_THREADS)
#             if defined(GC_PTHREADS)
pthread_mutex_t GC_allocate_ml = PTHREAD_MUTEX_INITIALIZER;
#	      elif defined(GC_DLL)
__declspec(dllexport) CRITICAL_SECTION GC_allocate_ml;
#	      else
CRITICAL_SECTION GC_allocate_ml;
#	      endif
#          else
#             if defined(GC_PTHREADS) && !defined(GC_SOLARIS_THREADS)
#		if defined(USE_SPIN_LOCK)
pthread_t GC_lock_holder = NO_THREAD;
#	        else
pthread_mutex_t GC_allocate_ml = PTHREAD_MUTEX_INITIALIZER;
                                 pthread_t GC_lock_holder = NO_THREAD;
#		endif
#    	      else
--> declare allocator lock here
#	      endif
#	   endif
#	endif
#     endif
#   endif
# endif

int main() {}



class foo
{
    virtual void bar4 () = A::f;
};



// { dg-options "-std=c++0x" }

template<class T, T t = (T)0>
struct S
{
    void
    foo(decltype(t) = t);
};
class foo
{
    virtual void bar2 () = __null;
};
struct A
{
    static void g(int);
};
class L
{
public:
    L(int);
};

class R
{
    friend L::L(int);
};
int g ();

struct S
{
int i : (false ? g () : 1);
};

/*
void f () {
  switch (0) {
  case (3, 0): // { dg-error "" }
    break;
  }
}

*/
class A
{
private:
    void f();
};

class B
{
    friend void A::f();
};
void f()
{
    int i;
    if MACRO()
        i =1;
}


struct S
{
    static int i;

    friend void f(int = i);
    friend void A::g(int = i);
};

void
bar()
{
    S<int> s;
    s.foo();
}
// g++ -c crash16.cpp -std=c++0x









struct
{
    char c;
}






// { dg-do "compile" }
// { dg-options "-std=gnu++0x" }

template<int i> class X { /* ... */ };
X< 1>2 > x1;    // // { dg-error "numeric constant" }
X<(1>2)> x2;    // Okay.

template<class T> class Y { /* ... */ };
Y<X<1>> x3;     // Okay, same as "Y<X<1> > x3;".
     Y<X<6>>1>> x4;  // { dg-error "numeric constant" }
     Y<X<(6>>1)>> x5;  // Okay





