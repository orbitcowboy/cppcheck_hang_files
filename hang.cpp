/* -*- C++ -*- */

/**
 * @file   libdiehard.cpp
 * @brief  Replaces malloc and friends with DieHard versions.
 * @author Emery Berger <http://www.cs.umass.edu/~emery>
 * @note   Copyright (C) 2005 by Emery Berger, University of Massachusetts Amherst.
 */

#include <stdlib.h>
#include <new>

class std::bad_alloc;

volatile int anyThreadCreated = 1;

// -*- C++ -*-

/**
 * @file   diehard.h
 * @brief  The DieHard allocator.
 * @author Emery Berger <http://www.cs.umass.edu/~emery>
 */

#ifndef _DIEHARD_H_
#define _DIEHARD_H_

#if !defined(DIEHARD_REPLICATED)
#error "Must define DIEHARD_REPLICATED to 0 or 1."
#endif

// NB: This include must appear first.
#include "platformspecific.h"

#include <assert.h>
#include <stdlib.h>


// #define MAX_HEAP_SIZE (12*128*1024*1024)


#define MAX_HEAP_SIZE (12*32*1024*1024)
// #define MAX_HEAP_SIZE (12*8*1024*1024)

// #define MAX_HEAP_SIZE (12*2*32*1024*1024)

#include "sassert.h"
#ifndef _IFCLASS_H_
#define _IFCLASS_H_

template <bool b, class a, class c>
class IfClass;

template <class a, class b>
class IfClass<true, a, b> : public a {};

template <class a, class b>
class IfClass<false, a, b> : public b {};


#endif
#include "checkpoweroftwo.h"
#include "oneheap.h"
#include "staticlog.h"
// -*- C++ -*-

/**
 * @file   partitioninfo.h
 * @brief  All the information needed to manage one partition (size class).
 * @author Emery Berger <http://www.cs.umass.edu/~emery>
 * @note   Copyright (C) 2005 by Emery Berger, University of Massachusetts Amherst.
 */

#ifndef _PARTITIONINFO_H_
#define _PARTITIONINFO_H_

#include <assert.h>
#include <stdlib.h>

#include "log2.h"
#include "ifclass.h"
#include "mmapalloc.h"
#include "bumpalloc.h"
#include "staticlog.h"
#include "bitmap.h"


#if !defined(DIEHARD_REPLICATED)
#error "Must define DIEHARD_REPLICATED to 0 or 1."
#endif

extern size_t getRandom (void);

/**
 * @class PartitionInfo
 * @brief All the information needed to manage one partition (size class).
 *
 */

template <bool ElementsArePowerOfTwo>
class PartitionInfo
{
public:

    PartitionInfo (void)
        : _initialized (false)
    {}

    ~PartitionInfo (void) {}

    /**
     * Initialize the partition.
     *
     * @param i          the index of this partition.
     * @param sz         the size of objects in this partition.
     * @param threshold  the maximum number of allocations allowed.
     * @param start      the starting address of the partition.
     * @param len        the length of the partition, in bytes.
     * @param seed       a seed for the random number generator.
     */

    NO_INLINE void initialize (int i,
                               size_t sz,
                               int threshold,
                               char * start,
                               size_t len,
                               int seed)
    {
        _size = sz;
        _sizeClass = i;
        _partitionStart = start;
#if DIEHARD_MULTITHREADED != 1
        _threshold = threshold;
        _requested = 0;
#endif
        _elements = (len / sz);
        _elementsShift = log2 (_elements);
        _mask = _elements - 1;

        // Since the number of elements are known to be a power of two,
        // we eliminate an expensive mod operation (%) in
        // allocObject(). We check that they are indeed a power of two
        // here.
        assert ((_elements & (_elements - 1)) == 0);
        assert (_threshold > 0);

        // Initialize all bitmap entries to false.
        _allocated.reserve (_elements);
        _allocated.clear();
    }


    /// Fills an object with random values.
    inline void scrambleObject (char * ptr)
    {
#if DIEHARD_REPLICATED
        fillRandom (ptr, _size);
#else
        memset (ptr, 0, _size);
#endif
    }

    /// Fills the partition space with random values.
    NO_INLINE bool initializePartition (void)
    {
#if DIEHARD_REPLICATED
        fillRandom (_partitionStart, _elements * _size);
#endif
        _initialized = true;
        return true;
    }

    /// Randomly choose one of the available objects.
    inline char * allocObject (void)
    {
        if (!_initialized)
            initializePartition();

        // Probe the heap enough to ensure that the odds are at least
        // 2^MAX_ITERATIONS to 1 against missing an item.

        int iterations = 0;

        do
        {

#if DIEHARD_MULTITHREADED != 1
            if (_requested >= _threshold)
            {
                return NULL;
            }
#endif

            IfClass<ElementsArePowerOfTwo, FastObjectIndex, SlowObjectIndex> calculator;
            const unsigned long rng = getRandom();
            const int objectIndex = calculator (rng, _elements, _mask);

            assert (objectIndex >= 0);
            assert (objectIndex < _elements);

            bool success = _allocated.tryToSet (objectIndex);

#if DIEHARD_MULTITHREADED == 1
            iterations++;
#endif

            if (success)
            {
                // Got it.
#if DIEHARD_MULTITHREADED != 1
                _requested++;
#endif
                char * ptr = (char *) _partitionStart + (objectIndex * _size);
                scrambleObject (ptr);
                return ptr;
            }

        }
        while (iterations < MAX_ITERATIONS);
        return NULL;
    }

    /// Free an object.
    inline void freeObject (void * ptr)
    {
        const int offsetFromPartition = ((char *) ptr - _partitionStart);

        // Check to make sure this object is valid: a valid object from
        // this partition must be a multiple of the partition's object
        // size.
        if ((offsetFromPartition & ~(_size - 1)) == offsetFromPartition)
        {
            // We have a valid object, so we free it.

            IfClass < ElementsArePowerOfTwo,
                    FastIndexFromOffset,
                    SlowIndexFromOffset > calculator;
            const int objectIndex = calculator (offsetFromPartition, _size, _sizeClass);

            bool reallyDidReset = _allocated.reset (objectIndex);

            if (!reallyDidReset)
            {
                // Already free!
                return;
            }

#if DIEHARD_MULTITHREADED != 1
            _requested--;
#endif

#if 1
            // This object is empty; if it's at least a page long, tell the
            // virtual memory manager that we don't need its space anymore.
            if (_size >= 4096)
            {
                MmapWrapper::dontneed (_partitionStart + objectIndex * _size, _size);
            }
#endif
        }
    }

    /// Fill a range with random values.
    inline bool fillRandom (void * ptr, size_t sz)
    {
        unsigned long * l = (unsigned long *) ptr;
        register unsigned long lv1 = getRandom();
        register unsigned long lv2 = getRandom();
        for (int i = 0; i < sz / sizeof(double); i ++)
        {
            // Minimum object size is a double (two longs), so we unroll the
            // loop a bit for efficiency.
            *l++ = lv1;
            *l++ = lv2;
        }
        return true;
    }

private:

    // Prohibit copying and assignment.
    PartitionInfo (const PartitionInfo&);
    PartitionInfo& operator= (const PartitionInfo&);

    /// The allocation bitmap, grabbed in 1MB chunks from MmapAlloc.
    class BitMapSource :
        public OneHeap<BumpAlloc<MmapAlloc, 1024 * 1024> > {};

    class SlowObjectIndex
    {
    public:
        inline int operator() (unsigned long rng,
                               int elts,
                               unsigned long) const
        {
            return rng % elts;
        }
    };

    class FastObjectIndex
    {
    public:
        inline int operator() (unsigned long rng,
                               int,
                               unsigned long mask) const
        {
            return rng & mask;
        }
    };

    class SlowIndexFromOffset
    {
    public:
        inline int operator()(int offset, size_t size, int) const
        {
            return offset / size;
        }
    };

    class FastIndexFromOffset
    {
    public:
        inline int operator()(int offset, size_t, int sizeClass) const
        {
            return offset >> (sizeClass + StaticLog<sizeof(double)>::VALUE);
        }
    };


    /// The maximum number of times we will probe in the heap for an object.
    enum { MAX_ITERATIONS = 24 };

    /// True iff this partition has been used for allocation.
    bool _initialized;

    /// Bit map of allocated objects.
    BitMap<BitMapSource> _allocated;

    /// Size of objects in this partition.
    size_t _size;

    /// And the corresponding size class.
    int _sizeClass;

    /// The start of the partition.
    char * _partitionStart;

#if DIEHARD_MULTITHREADED != 1

    /// The maximum number of objects to allocate.
    int _threshold;

    /// Number of objects requested for this partition.
    int _requested;
#endif

    /// The total number of objects in this partition.
    int _elements;

    /// The log base 2 of _elements, for shifting.
    int _elementsShift;

    /// The number of _elements, minus 1 (for bitmasking).
    unsigned long _mask;
};

#endif

/* -*- C++ -*- */

#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

#if defined(unix)
#include <sched.h>
#endif

#if defined(__SVR4)
#include <thread.h>
#endif

#if defined(__sgi)
#include <mutex.h>
#endif

// -*- C++ -*-

/*

  Heap Layers: An Extensible Memory Allocation Infrastructure

  Copyright (C) 2000-2003 by Emery Berger
  http://www.cs.umass.edu/~emery
  emery@cs.umass.edu

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



#ifndef _CPUINFO_H_
#define _CPUINFO_H_

#if defined(_WIN32)
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#endif


#if !defined(_WIN32)
#include <pthread.h>
#endif

#if defined(__SVR4) // Solaris
#include <sys/lwp.h>
extern "C" unsigned int lwp_self(void);
#include <thread.h>
extern "C" int _thr_self(void);
#endif

#if defined(__linux)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#endif

#if defined(__sgi)
#include <sys/types.h>
#include <sys/sysmp.h>
#include <sys/sysinfo.h>
#endif

#if defined(hpux)
#include <sys/mpctl.h>
#endif

#if defined(_WIN32)
extern __declspec(thread) int localThreadId;
#endif

#if defined(__SVR4) && defined(MAP_ALIGN)
extern volatile int anyThreadStackCreated;
#endif

/**
 * @class CPUInfo
 * @author Emery Berger <http://www.cs.umass.edu/~emery>
 *
 * @brief Architecture-independent wrapper to get number of CPUs.
 */

class CPUInfo
{
public:
    CPUInfo (void)
    {}

    inline static int getNumProcessors (void)
    {
        static int _numProcessors = computeNumProcessors();
        return _numProcessors;
    }

    static inline int getThreadId (void);
    static int computeNumProcessors (void);

};


int CPUInfo::computeNumProcessors (void)
{
    static int np = 0;
    if (!np)
    {
#if defined(__linux)
        // Ugly workaround.  Linux's sysconf indirectly calls malloc() (at
        // least on multiprocessors).  So we just read the info from the
        // proc file ourselves and count the occurrences of the word
        // "processor".

        // We only parse the first 32K of the CPU file.  By my estimates,
        // that should be more than enough for at least 64 processors.
        enum { MAX_PROCFILE_SIZE = 32768 };
        char line[MAX_PROCFILE_SIZE];
        int fd = open ("/proc/cpuinfo", O_RDONLY);
        if (!fd)
        {
            return 1;
        }
        else
        {
            read (fd, line, MAX_PROCFILE_SIZE);
            char * str = line;
            np = 0;
            while (str)
            {
                str = strstr(str, "processor");
                if (str)
                {
                    np++;
                    str++;
                }
            }
            close (fd);
            return np;
        }
#elif defined(_WIN32)
        SYSTEM_INFO infoReturn[1];
        GetSystemInfo (infoReturn);
        np = (int) (infoReturn->dwNumberOfProcessors);
        return np;
#elif defined(__sgi)
        np = (int) sysmp(MP_NAPROCS);
        return np;
#elif defined(hpux)
        np = mpctl(MPC_GETNUMSPUS, NULL, NULL); // or pthread_num_processors_np()?
        return np;
#elif defined(_SC_NPROCESSORS_ONLN)
        np = (int) (sysconf(_SC_NPROCESSORS_ONLN));
        return np;
#else
        np = 2;
        // Unsupported platform.
        // Pretend we have at least two processors. This approach avoids the risk of assuming
        // we're on a uniprocessor, which might lead clever allocators to avoid using atomic
        // operations for all locks.
#endif
    }
    else
    {
        return np;
    }
}

// Note: when stacksize arg is NULL for pthread_attr_setstacksize [Solaris],
// stack size is 1 MB for 32-bit arch, 2 MB for 64-bit arch.
// pthread_attr_getstacksize
// pthread_attr_setstackaddr
// pthread_attr_getstackaddr
// PTHREAD_STACK_SIZE is minimum.
// or should we just assume we have __declspec(thread) or __thread?

#if defined(USE_THREAD_KEYWORD)
extern __thread int localThreadId;
#endif


int CPUInfo::getThreadId (void)
{
#if defined(__SVR4)
    size_t THREAD_STACK_SIZE;
    if (sizeof(size_t) <= 4)
    {
        THREAD_STACK_SIZE = 1048576;
    }
    else
    {
        // 64-bits.
        THREAD_STACK_SIZE = 1048576 * 2;
    }
    if (0)   // !anyThreadStackCreated) {
    {
        // We know a priori that all stack variables
        // are on different stacks. Since no one has created
        // a special one, we are in control, and thus all stacks
        // are 1 MB in size and on 1 MB boundaries.
        // (Actually: 1 MB for 32-bits, 2 MB for 64-bits.)
        char buf;
        return (((size_t) &buf) & ~(THREAD_STACK_SIZE - 1)) >> 20;
    }
    else
    {
        return (int) pthread_self();
    }
#elif defined(_WIN32)
    // It looks like thread id's are always multiples of 4, so...
    return GetCurrentThreadId() >> 2;
#elif defined(__BEOS__)
    return find_thread(0);
#elif defined(USE_THREAD_KEYWORD)
    return localThreadId;
#elif defined(__linux) || defined(PTHREAD_KEYS_MAX)
    // Consecutive thread id's in Linux are 1024 apart;
    // dividing off the 1024 gives us an appropriate thread id.
    return (int) pthread_self() >> 10; // (>> 10 = / 1024)
#elif defined(POSIX) // FIX ME??
    return (int) pthread_self();
#elif USE_SPROC
    // This hairiness has the same effect as calling getpid(),
    // but it's MUCH faster since it avoids making a system call
    // and just accesses the sproc-local data directly.
    int pid = (int) PRDA->sys_prda.prda_sys.t_pid;
    return pid;
#else
    return 0;
#endif
}

#endif


#if defined(_MSC_VER)

#if !defined(NO_INLINE)
#pragma inline_depth(255)
#define NO_INLINE __declspec(noinline)
#define INLINE __forceinline
#define inline __forceinline
#endif

#else

#endif

// -*- C++ -*-

#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#if defined(__sparc) && !defined(__GNUC__)
extern "C" volatile unsigned long MyInterlockedExchange (unsigned long *, unsigned long);
#endif


class Atomic
{
public:

    inline static volatile unsigned long
    exchange (unsigned long * oldval, unsigned long newval)
    {
#if defined(_WIN32) && defined(_MSC_VER)
        return InterlockedExchange ((volatile LONG *) oldval, newval);
#elif defined(__sparc) && !defined(__GNUC__)
        return MyInterlockedExchange (oldval, newval);
#elif defined(__sparc) && defined(__GNUC__)
        asm volatile ("swap [%1],%0"
                      :"=r" (newval)
                      :"r" (oldval), "0" (newval)
                      : "memory");
#elif defined(__i386__)
        asm volatile ("lock; xchgl %0, %1"
                      : "=r" (newval)
                      : "m" (*oldval), "0" (newval)
                      : "memory");
#elif defined(__sgi)
        newval = test_and_set (oldval, newval);
#elif defined(__ppc) || defined(__powerpc__) || defined(PPC)
        // Contributed by Maged Michael.
        int ret;
        asm volatile (
            "La..%=0:    lwarx %0,0,%1 ;"
            "      cmpw  %0,%2;"
            "      beq La..%=1;"
            "      stwcx. %2,0,%1;"
            "      bne- La..%=0;"
            "La..%=1:    isync;"
            : "=&r"(ret)
            : "r"(oldval), "r"(newval)
            : "cr0", "memory");
        return ret;
#elif defined(__arm__)
        // Contributed by Bo Granlund.
        long result;
        asm volatile (
            "\n\t"
            "swp     %0,%2,[%1] \n\t"
            ""
            : "=&r"(result)
            : "r"(oldval), "r"(newval)
            : "memory");
        return (result);
#else
#error "No implementation is available for this platform."
#endif
        return newval;
    }

};

#endif


#if defined(_WIN32)
#define _WIN32_WINNT 0x0500

// NOTE: Below is the new "pause" instruction, which is inocuous for
// previous architectures, but crucial for Intel chips with
// hyperthreading.  See
// http://www.usenix.org/events/wiess02/tech/full_papers/nakajima/nakajima.pdf
// for discussion.

#define _MM_PAUSE {__asm{_emit 0xf3};__asm {_emit 0x90}}
#include <windows.h>
#else
#define _MM_PAUSE
#endif

extern volatile int anyThreadCreated;

class SpinLockType
{
public:

    SpinLockType (void)
        : mutex (UNLOCKED)
    {}

    ~SpinLockType (void)
    {}

    inline bool lock (void)
    {
        // A yielding lock (with an initial spin).
        if (anyThreadCreated)
        {
            if (Atomic::exchange (const_cast<unsigned long *>(&mutex), LOCKED)
                    != UNLOCKED)
            {
                contendedLock();
            }
        }
        else
        {
            mutex = LOCKED;
        }
        return true;
    }

    inline bool trylock (void)
    {
        if (anyThreadCreated)
        {
            if (mutex == LOCKED)
            {
                return false;
            }
            else
            {
                if (Atomic::exchange (const_cast<unsigned long *>(&mutex), LOCKED)
                        == LOCKED)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
        else
        {
            mutex = LOCKED;
            return true;
        }
    }


    inline void unlock (void)
    {
        if (anyThreadCreated)
        {
#if 1
#if defined(_WIN32)
            __asm {}
#elif !defined(sparc)
            asm volatile ("" : : : "memory");
#endif
#endif
            // SFENCE here?
            // MyInterlockedExchange (const_cast<unsigned long *>(&mutex), UNLOCKED);
        }
        mutex = UNLOCKED;
    }

    private:

    NO_INLINE
    void contendedLock (void)
    {
        int spinCount = 1;
        do
        {
            if (Atomic::exchange (const_cast<unsigned long *>(&mutex), LOCKED)
                    == UNLOCKED)
            {
                // We got the lock.
                return;
            }
            // printf ("contention!\n");
            _MM_PAUSE;
            // Exponential back-off protocol.
            for (volatile int q = 0; q < spinCount; q++)
            {
            }
            spinCount <<= 1;
            if (spinCount > MAX_SPIN_LIMIT)
            {
                yieldProcessor();
                spinCount = 1;
            }
        }
        while (1);
    }

    // Is this system a multiprocessor?
    inline bool onMultiprocessor (void)
    {
        CPUInfo cpuInfo;
        return (cpuInfo.getNumProcessors() > 1);
    }

    inline void yieldProcessor (void)
    {
#if defined(_WIN32)
        Sleep(0);
#else
#if defined(__SVR4)
        thr_yield();
#else
        sched_yield();
#endif
#endif
    }

    enum { UNLOCKED = 0, LOCKED = 1 };

    enum { MAX_SPIN_LIMIT = 1024 };

    union
    {
        double _dummy;
        volatile unsigned long mutex;
    };

    double _spacing[8];

};

#endif


extern size_t getRandom();

// Successful search odds: (1 + ln(1/(1-lambda)))/lambda
// For lambda (load threshold) = Numerator/Denominator
// : expected probes to find an empty slot
//  = 1/(1-Numerator/Denominator)
//  = 1/((D-N)/D)
//  = Denominator / (Denominator - Numerator)
//
// e.g. if N/D = 1/2, expected probes = 2/(2-1) = 2.
// if N/D = 1/infinity, expected probes = inf/(inf-1) = 1.

/**
 * @class  DieHardHeap
 * @brief  The DieHard randomized allocator.
 * @author Emery Berger <http://www.cs.umass.edu/~emery>
 * @param  MaxHeapSize  the size of the heap (a power of two).
 * @param  LoadFactorThresholdNumerator  the numerator of the threshold fraction.
 * @param  LoadFactorThresholdDenominator  the denominator of the threshold fraction.
 */

template < int MaxHeapSize,
         int LoadFactorThresholdNumerator = 1,
         int LoadFactorThresholdDenominator = 2 >
class DieHardHeap
{
    public:

    /// @param seed   the seed for the random number generator.
    DieHardHeap (int seed = 0)
    : _heap (NULL),
    _seed (seed)
    {
    }

    ~DieHardHeap (void)
    {
        // Dispose of the heap.
        if (_heap)
            MmapWrapper::unmap ((void *) _heap, MaxHeapSize);
    }

    private:

    void init (void)
    {
        // Get memory for the heap and initialize it.
        _heap = (char*) MmapWrapper::map (MaxHeapSize); //  + 4 * MaxObjectSize);
#if 0
        _heap = (char *) ((size_t) (_heap + 1) & ~(2 * MaxObjectSize - 1));
        MmapWrapper::protect ((void *) (((size_t) _redzone_1 + (REDZONE_SIZE / 2 - 1)) & ~(REDZONE_SIZE / 2 - 1)), REDZONE_SIZE / 2);
        MmapWrapper::protect ((void *) (((size_t) _redzone_2 + (REDZONE_SIZE / 2 - 1)) & ~(REDZONE_SIZE / 2 - 1)), REDZONE_SIZE / 2);
#endif

        initializeHeap (_seed);
        //    fprintf (stderr, "initialized!\n");
    }

    // Prohibit copying and assignment.
    DieHardHeap (const DieHardHeap&);
    DieHardHeap& operator= (const DieHardHeap&);

    // Ensure that the load factor fraction makes sense.  It has to be
    // between zero and one, and the denominator cannot be 0.

    enum { EnsureLoadFactorLessThanOne =
               sassert < (LoadFactorThresholdNumerator < LoadFactorThresholdDenominator) >::VALUE
         };

    enum { EnsureLoadFactorMoreThanZero =
               sassert < (LoadFactorThresholdNumerator > 0) >::VALUE
         };

    enum { EnsureLoadFactorDefined =
               sassert < (LoadFactorThresholdDenominator > 0) >::VALUE
         };

    /**
     * @brief Initialize the heap.
     * @param seed  the seed for the random number generator.
     */
    NO_INLINE void initializeHeap (int seed)
    {
        if (_heap == NULL)
            return;

        // Initialize the size class table.  DO NOT CHANGE THESE VALUES!
        // But if you do, they should be powers of two: if not,
        // change the template argument to _elements to false.

        size_t sizeTable[] = { 8, 16, 32, 64, 128, 256, 512,
                               1024, 2048, 4096, 8192, MaxObjectSize
                             };

        for (int i = 0; i < NumberOfClasses; i++)
        {
            _size[i] = sizeTable[i];
        }

        if (_heap == NULL)
        {
            fprintf (stderr, "The mmap call failed to obtain enough memory.\n");
            abort();
        }

        // Compute the load factor for each partition.
        const float loadFactor =
            1.0 * LoadFactorThresholdNumerator * PartitionSize
            / (1.0 * LoadFactorThresholdDenominator);

        // Initialize each partition (size class).
        for (int i = NumberOfClasses - 1; i >= 0; i--)
        {
            _pInfo[i].initialize (i,
                                  _size[i],
                                  (int) loadFactor / _size[i],
                                  const_cast<char *>(_heap) + i * PartitionSize,
                                  PartitionSize,
                                  seed + i + 1);
        }
    }

    /// How many size classes there are.
    enum { NumberOfClasses = 12 };

    /// The allocatable space of each size class.
    enum { PartitionSize = MaxHeapSize / NumberOfClasses };

    /// The maximum size for any object managed here (i.e., not with mmap).
    enum { MaxObjectSize = 16 * 1024 };


    CheckPowerOfTwo<MaxObjectSize> ch1;
    CheckPowerOfTwo<PartitionSize> ch2;
    CheckPowerOfTwo < PartitionSize / MaxObjectSize > ch3;

    enum { REDZONE_SIZE = 16384 };

    // An area we will page-protect to reduce the risk of buffer overflows.
    char _redzone_1[REDZONE_SIZE];

    /// The seed for the RNG.
    int _seed;

    /// The start of the heap.
    char * _heap;

    /**
     * The information about each "partition" (size class): template arg
     * true iff elements are always a power of two.
     */
    PartitionInfo<true> _pInfo[NumberOfClasses];

    /// The sizes corresponding to each size class.
    size_t _size[NumberOfClasses];

    // An area we will page-protect to reduce the risk of buffer overflows.
    char _redzone_2[REDZONE_SIZE];

    private:

    class SlowCalculatePartitionNumber
    {
        public:
        inline int operator()(char * p) const
        {
            return ((int) p) / PartitionSize;
        }
    };

    class FastCalculatePartitionNumber
    {
        public:
        // Only for powers-of-two.
        inline int operator()(char * p) const
        {
            return ((int) p) >> StaticLog<PartitionSize>::VALUE;
        }
    };

    /// Compute the size class for a given size.
    static inline int sizeClass (size_t sz)
    {
        return log2(sz) - StaticLog<sizeof(double)>::VALUE;
    }

    /// Given a size class, return the maximum size.
    inline size_t classToSize (int index) const
    {
        assert (NumberOfClasses == (sizeof(_size) / sizeof(int)));
        assert (index >= 0);
        assert (index < (sizeof(_size) / sizeof(int)));
        size_t val = 1 << (index + StaticLog<sizeof(double)>::VALUE);
        assert (_size[index] == val);
        return val;
    }

    /// Find out which partition the pointer is in.
    inline int getSizeClass (void * ptr) const
    {
        assert ((PartitionSize & (PartitionSize - 1)) == 0);
        IfClass < IsPowerOfTwo<PartitionSize>::VALUE,
                FastCalculatePartitionNumber, SlowCalculatePartitionNumber > calculator;
        const int partitionNumber = calculator ((char*) ((char *) ptr - _heap));
        assert (partitionNumber >= 0);
        assert (partitionNumber < NumberOfClasses);
        return partitionNumber;
    }

    /// @return one allocated object.
    inline char * allocateObject (size_t sz)
    {
        if (sz > MaxObjectSize)
        {
            return (char *) allocateDirect (sz);
        }

        if (!_heap)
        {
            init();
            if (!_heap)
            {
                return NULL;
            }
        }

        // malloc requires that the minimum request (even 0) be large
        // enough to hold a double.
        if (sz < sizeof(double))
        {
            sz = sizeof(double);
        }

        // Figure out which size class we're in = which partition to allocate from.
        int sc = sizeClass (sz);

        while (sc < NumberOfClasses)
        {
            // Allocate only if we would not be crossing the load factor threshold.
            char * ptr = _pInfo[sc].allocObject();
            if (ptr)
            {
                return ptr;
            }
            // We're out of memory in this partition.  Request an object
            // from the next size class up (twice as large as requested).
            sc++;
        }

#if 0
        char buf[255];
        sprintf (buf, "returning NULL for request of %d (class = %d)\n", sz, sizeClass(sz));
        fprintf (stderr, buf);
#endif

        return NULL;
    }

    /// Allocate an object directly using memory-mapping.
    NO_INLINE void * allocateDirect (size_t sz)
    {
        _directLock.lock();
        void * ptr = MmapWrapper::map (sz);
        if (ptr != NULL)
        {
            // Register this object.
            setLargeObjectEntry (ptr, sz);

#if DIEHARD_REPLICATED
            // Fill in the space with random values.
            unsigned long * l = (unsigned long *) ptr;
            register unsigned long lv1 = getRandom();
            register unsigned long lv2 = getRandom();
            for (int i = 0; i < sz / sizeof(double); i ++)
            {
                // Minimum object size is two longs (a double), so we unroll the
                // loop a bit for efficiency.
                *l++ = lv1;
                *l++ = lv2;
            }
#else
            memset (ptr, 0, sz);
#endif
        }
        _directLock.unlock();
        return ptr;
    }

    /// Free a large object via munmap.
    void freeDirect (void * ptr)
    {
        _directLock.lock();
        size_t sz = getLargeObjectSize (ptr);
        if (sz != 0)
        {
            MmapWrapper::unmap (ptr, sz);
            clearLargeObjectEntry (ptr);
        }
        _directLock.unlock();
    }

    class LargeObjectManager
    {
        public:

        inline int getIndex (void * ptr) const
        {
            return (size_t) ptr >> StaticLog<MaxObjectSize>::VALUE;
        }

        inline size_t get (void * ptr) const
        {
            return values[getIndex(ptr)];
        }

        inline void set (void * ptr, size_t sz)
        {
            values[getIndex(ptr)] = sz;
        }

        inline void clear (void * ptr)
        {
            values[getIndex(ptr)] = 0;
        }

        private:
        size_t values[1 << (32 - StaticLog<MaxObjectSize>::VALUE)];
    };

    SpinLockType _directLock;
    LargeObjectManager _lgManager;

    size_t getLargeObjectSize (void * ptr)
    {
        return _lgManager.get (ptr);
    }

    void clearLargeObjectEntry (void * ptr)
    {
        _lgManager.clear (ptr);
    }

    void setLargeObjectEntry (void * ptr, size_t sz)
    {
        _lgManager.set (ptr, sz);
    }

    public:

    /// @return an object with randomized contents.
    inline void * malloc (size_t sz)
    {
        char * ptr = allocateObject (sz);
        if (ptr == NULL)
        {
            // Now we are stuck - we need to allocate the object using mmap.
            ptr = (char *) allocateDirect (sz);
        }
        return ptr;
    }

    /// Relinquish an object.
    inline bool free (void * ptr)
    {
        if (ptr != NULL)
        {
            // Free any object that's in the heap.
            if (isOnHeap(ptr))
            {
                _pInfo[getSizeClass(ptr)].freeObject (ptr);
            }
            else
            {
                // Outside of the heap => this object is either invalid, or it
                // was allocated by mmap.
                freeDirect (ptr);
            }
        }
        return true; // This is for compatibility with newer versions of DieHard
    }

    /// @return true iff this object is on the heap.
    bool isOnHeap (void * ptr) const
    {
        return (((size_t) ptr >= (size_t) _heap) &&
                ((size_t) ptr < (size_t) (_heap + MaxHeapSize)));
    }


    bool allocatedFromDieHard (void * ptr)
    {
        if (isOnHeap(ptr))
        {
            return true;
        }
        _directLock.lock();
        size_t sz = getLargeObjectSize (ptr);
        bool result = false;
        if (sz != 0)
        {
            result = true;
        }
        _directLock.unlock();
        return result;
    }


    /// @return the size of a given object.
    inline size_t getSize (void * ptr)
    {
        if (isOnHeap(ptr))
        {
            int sc = getSizeClass (ptr);
            size_t sz = classToSize (sc);
            return sz - (((size_t) ptr - (size_t) _heap) % sz);
        }
        else
        {
            return getLargeObjectSize (ptr);
        }
    }

};

#endif

#include "realrandomvalue.h"
#include "randomnumbergenerator.h"
#include "version.h"

/*
  We have to intercept time and date functions (times, getrusage).
  For now, we simply use bogus time functions with known values.

 */

#if DIEHARD_REPLICATED

#if defined(linux)

#include <dlfcn.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/resource.h>

extern "C" {

    clock_t times (struct tms *buf)
    {
        // We'll make our own bogus, fixed time.
        static int count = 1;
        if (buf != NULL)
        {
            buf->tms_utime = count;
            buf->tms_stime = count;
            buf->tms_cutime = count;
            buf->tms_cstime = count;
            count++;
        }
        return 0;
    }

    int getrusage (int who, struct rusage *usage)
    {
        static int count = 1;
        if (usage != NULL)
        {
            usage->ru_utime.tv_sec = count;
            usage->ru_utime.tv_usec = count;
            usage->ru_stime.tv_sec = count;
            usage->ru_stime.tv_usec = count;
            usage->ru_maxrss = 0;
            usage->ru_ixrss = 0;
            usage->ru_idrss = 0;
            usage->ru_isrss = 0;
            usage->ru_minflt = 0;
            usage->ru_majflt = 0;
            usage->ru_nswap = 0;
            usage->ru_inblock = 0;
            usage->ru_oublock = 0;
            usage->ru_msgsnd = 0;
            usage->ru_msgrcv = 0;
            usage->ru_nsignals = 0;
            usage->ru_nvcsw = 0;
            usage->ru_nivcsw = 0;
            count++;
        }
        return 0;
    }

    clock_t clock (void)
    {
        return (clock_t) - 1;
    }
}

#endif // linux

#endif // replicated


template <class Super>
class TheCustomHeapTemplate : public Super
{
    public:
    TheCustomHeapTemplate (void) :
#if defined(linux)
    Super (RealRandomValue::value())
#elif defined(_WIN32)
    Super (GetCurrentProcessId())
#else
    Super (getpid())
#endif
    {
#if defined(linux)
        // fprintf (stderr, "Using the DieHard runtime system (Linux version).\nCopyright (C) 2005 Emery Berger, University of Massachusetts Amherst.\n");
#endif
    }
};

#include "ansiwrapper.h"
#include "reentrantheap.h"

typedef ANSIWrapper<ReentrantHeap<TheCustomHeapTemplate<DieHardHeap<MAX_HEAP_SIZE, 1, 2> > > > TheCustomHeapType;

static TheCustomHeapType * getCustomHeap (void)
{
    static char theHeapBuf[sizeof(TheCustomHeapType)];
    static TheCustomHeapType * theCustomHeap
        = new ((void *) theHeapBuf) TheCustomHeapType;
    return theCustomHeap;
}

extern "C" bool isOnHeap (void * ptr)
{
    return getCustomHeap()->isOnHeap (ptr);
}

#if defined(_WIN32)
#pragma warning(disable:4273)
#endif

#include "wrapper.cpp"

#if defined(_WIN32)
#ifndef CUSTOM_DLLNAME
#define CUSTOM_DLLNAME DllMain
#endif

#ifndef DIEHARD_PRE_ACTION
#define DIEHARD_PRE_ACTION
#endif
#endif // _WIN32

/*
  Thread-specific random number generator stuff.
*/

#if defined(_WIN32)
#define THREAD_LOCAL __declspec(thread)
#else
#define THREAD_LOCAL __thread
#endif // _WIN32

static THREAD_LOCAL double rngBuf[sizeof(RandomNumberGenerator) / sizeof(double) + 1];
static THREAD_LOCAL RandomNumberGenerator * threadSpecificRNG = NULL;

static int threadCount = 0;
static unsigned long mv1 = 0;
static unsigned long mv2 = 0;

static void initializeRNG (void)
{
#if !defined(_WIN32)
    long v1 = RealRandomValue::value();
    long v2 = RealRandomValue::value();
#else
    long v1;
    long v2;
    if (mv1 == 0)
    {
        mv1 = RealRandomValue::value();
        mv2 = RealRandomValue::value();
    }
    v1 = mv1 + threadCount;
    v2 = mv2 + threadCount;
#endif
    threadSpecificRNG = (RandomNumberGenerator *) rngBuf;
    new ((char *) rngBuf) RandomNumberGenerator(v1, v2);
    //  int tc = threadCount;
    //  printf ("initialized an rng (%x) with v1=%d, v2=%d\n", (void *) threadSpecificRNG, v1, v2);
}

inline size_t getRandom (void)
{
    if (threadSpecificRNG == NULL)
    {
        initializeRNG();
        threadCount++;
    }
    return threadSpecificRNG->next();
}

#if defined(_WIN32)

extern "C" {

    BOOL WINAPI CUSTOM_DLLNAME (HANDLE hinstDLL, DWORD fdwReason, LPVOID lpreserved)
    {
        char buf[255];

        switch (fdwReason)
        {

        case DLL_PROCESS_ATTACH:
                initializeRNG ();
            sprintf (buf, "Using the DieHard runtime system (Windows DLL, version %d.%d).\nCopyright (C) 2006 Emery Berger, University of Massachusetts Amherst.\n", VERSION_MAJOR_NUMBER, VERSION_MINOR_NUMBER);
            fprintf (stderr, buf);
            DIEHARD_PRE_ACTION;
            break;

        case DLL_PROCESS_DETACH:
                DIEHARD_POST_ACTION;
            break;

        case DLL_THREAD_ATTACH:
                //    threadCount++;
                initializeRNG ();
            break;

        case DLL_THREAD_DETACH:
                break;

        default:
                return TRUE;
        }

        return TRUE;
    }

}

#endif // WIN32

