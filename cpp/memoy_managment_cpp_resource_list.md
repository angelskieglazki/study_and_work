# Memory Management  Lecture - Breda University of applied sciences - Games
This is the resource repositry for my memory managment lecture / workshop series at [Breda University of applied sciences - Games](https://www.buas.nl/en/games). It is mainly targeted for game developers

https://www.youtube.com/watch?v=LIb3L4vKZ7U
https://github.com/FelixPetriconi/AllocatorBuilder
https://github.com/lukaszlaszko/allocators

## Content
- [Great Reads directly about Memory Managment](#great-reads)
- [Great Youtube Videos directly about Memory Managment](#great-youtube-videos)
- [Great C++ PMR related Talks, Reads](#pmr---polymorphic-allocator)
- [Related Videos & Articles as important](#related-talks-and-reads-important)
- [Memory Debugger](#memory-debugger)
- [Libs, frameworks](#libs)
- [Other related Lists](#other-useful-link-lists)
- [Other Stuff](#other-useful-link-lists)

## Great Reads

- [A great compilation of memroy mangement information mtrebi/memory-allocators](https://github.com/mtrebi/memory-allocators)
- [CPU Cache Wikipedia](https://en.wikipedia.org/wiki/CPU_cache)
- [CPU Caches and Why You Care](https://www.aristeia.com/TalkNotes/ACCU2011_CPUCaches.pdf)
- [Why software developers should care about CPU caches](https://medium.com/software-design/why-software-developers-should-care-about-cpu-caches-8da04355bb8a)
- [std::bit_cast *Watch: CppCon 2019: Timur Doumler “Type punning in modern C++” and C++ Weekly - Ep 185 - Stop Using reinterpret_cast! first*](https://en.cppreference.com/w/cpp/numeric/bit_cast)
- [Memory Allocation Strategies](https://www.gingerbill.org/series/memory-allocation-strategies/)
- [Virtual Memory Tricks (Examples are in C but still transferable to C++)](https://ourmachinery.com/post/virtual-memory-tricks/)
- [Make your programs run faster by better using the data cache](https://johnysswlab.com/make-your-programs-run-faster-by-better-using-the-data-cache/)
- [C++: Custom memory allocation (tutorial)](https://www.gamedev.net/articles/programming/general-and-gameplay-programming/c-custom-memory-allocation-r3010/)
- [mtrebi/memory-allocators (example tutorial)](https://github.com/mtrebi/memory-allocators)
- [Dynamic Memory Allocation and Virtual Memory](https://www.cprogramming.com/tutorial/virtual_memory_and_heaps.html)
- [System Programming](https://github.com/angrave/SystemProgramming/wiki#3-memory-and-allocators)
  - [Memory and Allocators- System Programming](https://github.com/angrave/SystemProgramming/wiki#3-memory-and-allocators)
  - [Introduction to Virtual Memory](https://github.com/angrave/SystemProgramming/wiki/Virtual-Memory%2C-Part-1%3A-Introduction-to-Virtual-Memory)
  - [Memory mapped files and Shared memory](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-6%3A-Memory-mapped-files-and-Shared-memory)
- [	Building a Low-Fragmentation Memory System for 64-bit Games](https://www.gdcvault.com/play/1023005/Building-a-Low-Fragmentation-Memory) GDC - Aaron MacDougall (Sony London Studio) If you have acess to the VAULT you can watch it otherweise here are the notes.
- [Writing a Game Engine from Scratch - Part 2: Memory](https://www.gamasutra.com/blogs/MichaelKissner/20151104/258271/Writing_a_Game_Engine_from_Scratch__Part_2_Memory.php) Gamasutra article
- [What Every Programmer Should Know About Memory](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf)

## Great Youtube Videos

- [CppCon 2017: John Lakos “Local ('Arena') Memory Allocators (part 1 of 2)”](https://www.youtube.com/watch?v=nZNd5FjSquk)
- [CppCon 2017: John Lakos “Local ('Arena') Memory Allocators (part 2 of 2)”](https://www.youtube.com/watch?v=CFzuFNSpycI)
- [CppCon 2019: John Lakos “Value Proposition: Allocator-Aware (AA) Software”](https://www.youtube.com/watch?v=ebn1C-mTFVk)
- [CppCon 2017: Bob Steagall “How to Write a Custom Allocator”](https://www.youtube.com/watch?v=kSWfushlvB8)
- [CppCon 2015: Andrei Alexandrescu “std::allocator...”](https://www.youtube.com/watch?v=LIb3L4vKZ7U)
- [Taming Dynamic Memory - An Introduction to Custom Allocators - Andreas Weis [ACCU 2019]](https://www.youtube.com/watch?v=IGtKstxNe14)
- [C++Now Michael Spencer. Alignment in C++: Use, Limitations, and Extension](https://youtu.be/uSZFrmhayIM)
- [Alisdair Meredith. Making Allocators Work. CppCon, 2014. Part1](http://youtu.be/YkiYOP3d64E)
- [Alisdair Meredith. Making Allocators Work. CppCon 2014. Part2](http://youtu.be/Q5kyiFevMJQ)
- [Alisdair Meredith. Allocators in C++11. C++Now May 2013](https://youtu.be/v7B_8IbHjxA)
- [Alisdair Meredith. An allocator model for std2. CppCon Sept. 25, 2017](https://youtu.be/oCi_QZ6K_qk) *about the evolvment of the allocators from C++98 to C++17
- [C++Now 2017: Bob Steagall “Testing the Limits of Allocator Awareness"](https://www.youtube.com/watch?v=fmJfKm9ano8&feature=youtu.be) - how to test your allocator
- [Pablo Halpern. Modern Allocators: The Good Parts. CppCon](https://youtu.be/v3dz-AKOVL8) Gives a simple example for polymorpic allocators
- [CppCon 2019: Alisdair Meredith, Pablo Halpern “Getting Allocators out of Our Way”](https://www.youtube.com/watch?v=RLezJuqNcEQ)
- [CppCon 2017 Sergey Zubkov. From Security to Performance to GPU Programming: Exploring Modern Allocators](https://youtu.be/HdQ4aOZyuHw)
- [CppCon 2014 Stephan Lavavej. STL Features and Implementation Techniques](https://youtu.be/dTeKf5Oek2c)
- [CppCon 2015: Scott Wardle “Memory and C++ debugging at Electronic Arts"](https://www.youtube.com/watch?v=8KIvWJUYbDA) *EASTL* related
- [Debugging memory corruptoins in C++ - Mateusz Nowak - Meeting C++ 2019](https://youtu.be/epkwzXMdk98)
- [Introduction to Memory Management in Linux](https://www.youtube.com/watch?v=7aONIVSXiJ8) Introduction to Memory Management in Linux - Matt Porter, Konsulko
- [Lectures](https://people.inf.ethz.ch/omutlu/lecture-videos.html)

## PMR - polymorphic allocator
- [C++Now 2018 Arthur O’Dwyer. An Allocator is a Handle to a Heap.](https://youtu.be/0MdSJsCTRkY)
- [CppCon 2018: Arthur O'Dwyer “An Allocator is a Handle to a Heap”](https://www.youtube.com/watch?v=IejdKidUwIg)
- [C++ Weekly - Ep 248 - Understand the C++17 PMR Standard Allocators and Track All the Things](https://www.youtube.com/watch?v=Zt0q3OEeuB0&feature=youtu.be)
- [C++ Weekly - Ep 222 - 3.5x Faster Standard Containers With PMR!](https://www.youtube.com/watch?v=q6A7cKFXjY0)
- [C++ Weekly - Ep 245 - PMR: Mistakes Were Made (By Me)](https://www.youtube.com/watch?v=6BLlIj2QoT8)
- [C++ Weekly - Ep 250 - Custom Allocation - How, Why, Where (Huge multi threaded gains and more!)](https://www.youtube.com/watch?v=5VrX_EXYIaM)
- [CppCon 2017: Pablo Halpern “Allocators: The Good Parts”](https://www.youtube.com/watch?v=v3dz-AKOVL8)

## Related talks and reads (important)

*Talks*
- [C++Now 2019: Arthur O'Dwyer “Trivially Relocatable”](https://www.youtube.com/watch?v=SGdfPextuAU&t=312s)
- [Writing cache friendly C++ - Jonathan Müller - Meeting C++ 2018](https://www.youtube.com/watch?v=Nz9SiF0QVKY)
- [CppCon 2019: Timur Doumler “Type punning in modern C++”](https://www.youtube.com/watch?v=_qzMpk-22cc)
- [C++ Weekly - Ep 185 - Stop Using reinterpret_cast!](https://www.youtube.com/watch?v=L06nbZXD2D0)
- [C++ Weekly - Ep 236 - Creating Allocator-Aware Types](https://www.youtube.com/watch?v=2LAsqp7UrNs)
- [What do you mean by "cache friendly"? - Björn Fahller - code::dive 2019](https://www.youtube.com/watch?v=Fzbotzi1gYs)
- [Data-Oriented Design](https://www.youtube.com/watch?v=16ZF9XqkfRY)
- [Memory & Caches](https://www.youtube.com/watch?v=4_smHyqgDTU&ab_channel=MattGodbolt) by Matt Godbolt
- [CppCon 2014: Herb Sutter "Lock-Free Programming (or, Juggling Razor Blades), Part I"](https://www.youtube.com/watch?v=c1gO9aB9nbs)
- [CppCon 2014: Herb Sutter "Lock-Free Programming (or, Juggling Razor Blades), Part II"](https://www.youtube.com/watch?v=CmxkPChOcvw)
- [CppCon 2015: Scott Wardle “Memory and C++ debugging at Electronic Arts”](https://www.youtube.com/watch?v=8KIvWJUYbDA)

*Reads*
- [Writing cache-friendly code - Alexander Toktarev](https://www.stardog.com/blog/writing-cache-friendly-code/)
- [Writing Cache-Friendly C++ - Jonathan Müller - Meeting C++ 2018](https://www.jonathanmueller.dev/talk/meetingcpp2018/)
- [Benchmarks of Cache-Friendly Data Structures in C++](https://tylerayoung.com/2019/01/29/benchmarks-of-cache-friendly-data-structures-in-c/)
- [Computer Organization | Locality and Cache friendly code](https://www.geeksforgeeks.org/computer-organization-locality-and-cache-friendly-code/)
- [Data Locality - Game Programming PatternsOptimization Patterns](https://gameprogrammingpatterns.com/data-locality.html)
- [Writing Cache-Friendly Code Gerson Robboy Portland State University](http://web.cecs.pdx.edu/~jrb/cs201/lectures/cache.friendly.code.pdf)
- [Writing Cache Friendly Code](https://courses.engr.illinois.edu/cs232/sp2009/lectures/X18.pdf)
- [What every programmer should know about memory, Part 1](https://lwn.net/Articles/250967/)
- [Avoiding instruction cache misses -  Paweł Dziepak](https://paweldziepak.dev/2019/06/21/avoiding-icache-misses/)
- [On lists, cache, algorithms, and microarchitecture - Paweł Dziepak](https://paweldziepak.dev/2019/05/02/on-lists-cache-algorithms-and-microarchitecture/)
- [Atomics](https://www.modernescpp.com/index.php/atomics) ModernesC++.com
- [C++ Core Guidelines: Concurrency and lock-free Programming](https://www.modernescpp.com/index.php/c-core-guidelines-concurrency-and-lock-free-programming)
- [C++ Core Guidelines: The Remaining Rules to Lock-Free Programming](https://www.modernescpp.com/index.php/c-core-guidelines-the-remaining-rules-to-lock-free-programming)
- [The C++ memory model and operations on atomic types](https://livebook.manning.com/book/c-plus-plus-concurrency-in-action-second-edition/chapter-5/v-7/)
- [Data oriented design book](https://dataorienteddesign.com/dodbook/)
- [TLSF: a New Dynamic Memory Allocator for Real-Time Systems](http://www.gii.upv.es/tlsf/files/ecrts04_tlsf.pdf)
  - [TLSF: Memory allocator real time embedded systems](http://www.gii.upv.es/tlsf/)
- [Memory stomp allocator for Unreal Engine 4](https://pzurita.wordpress.com/2015/06/29/memory-stomp-allocator-for-unreal-engine-4/)
  - [Memory corruption / Memory Stomp](https://en.wikipedia.org/wiki/Memory_corruption)

## Memory Debugger

- [VLD Memory Debugger (Windows)](https://github.com/KindDragon/vld/wiki/Using-Visual-Leak-Detector)
- [Valgrind (Linux)](https://valgrind.org/)
  - [Valgrind getting started](https://valgrind.org/docs/manual/quick-start.html)
  - [C++ Weekly - Ep 86 - Valgrind](https://www.youtube.com/watch?v=3l0BQs2ThTo)
- [Dr Memory (Linux & Windows)](https://drmemory.org/)
- [AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer)
  - [Clang AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html)
  - [Finding hard to find bugs with Address Sanitizer - Marshall Clow - Meeting C++ 2019](https://www.youtube.com/watch?v=1CcuD7EwhOY)
  - [Status quo: clang-tidy & AddressSanitizer on Windows - Victor Ciura - code::dive 2019](https://www.youtube.com/watch?v=Iz4C29yul2U)
  - [AddressSanitizerLeakSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer)
  - [AddressSanitizer for Windows: x64 and Debug Build Support](https://devblogs.microsoft.com/cppblog/asan-for-windows-x64-and-debug-build-support/)
- [CLion Google sanitizers](https://www.jetbrains.com/help/clion/google-sanitizers.html)
- [MTuner](https://github.com/milostosic/MTuner)
- [Finding memory leaks using the crt lib](https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019)
- [Debugging Heap Corruption](http://www.daviddahlbacka.com/BugCleaner/DebuggingHeapCorruption.pdf)
- [How to monitor a silent process exit using GFlags](https://blogs.sap.com/2019/12/20/how-to-monitor-a-silent-process-exit-using-gflags/)
- [Find memory leaks with the CRT library (Windows)](https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019)


## Libs

- [EASTL - A console standard lib](https://github.com/electronicarts/EASTL)
- [dlmalloc](https://github.com/ARMmbed/dlmalloc) : Doug Lea's malloc
- [deluxe68](https://github.com/deplinenoise/deluxe68) : A simple register allocator frontend for 68k assembly
- [KernelReadWriteMemory](https://github.com/Zer0Mem0ry/KernelReadWriteMemory) : Simple code to manipulate the memory of a usermode process from kernel.
- [malloc_geiger](https://github.com/laserallan/malloc_geiger) : 
- [mimalloc-bench](https://github.com/daanx/mimalloc-bench) : Suite for benchmarking malloc implementations.
- [allocator_bench](https://github.com/SamSaffron/allocator_bench) : bench of various memory allocators
- [mio](https://github.com/mandreyel/mio) : Cross-platform C++11 header-only library for memory mapped file IO
- [snmalloc](https://github.com/microsoft/snmalloc) : Message passing based allocator
- [mimalloc](https://github.com/microsoft/mimalloc) : mimalloc is a compact general purpose allocator with excellent performance.
- [hardened_malloc](https://github.com/GrapheneOS/hardened_malloc) : Hardened allocator designed for modern systems. It has integration into Android's Bionic libc and can be used externally with musl and glibc as a dynamic library for use on other Linux-based platfo…
- [objectpool](https://github.com/bitshifter/objectpool) : Object pool implementation in C++11
- [rampin](https://github.com/FRex/rampin) : Try to make Windows preload file into RAM by memory mapping it
- [tgc](https://github.com/orangeduck/tgc) : A Tiny Garbage Collector for C
- [Mesh](https://github.com/plasma-umass/Mesh) : A memory allocator that automatically reduces the memory footprint of C/C++ applications.
- [memory-dumper](https://github.com/alexandernst/memory-dumper) : A tool for dumping files from processes memory
- [alloc-test](https://github.com/node-dot-cpp/alloc-test) : Cross-platform benchmarking for memory allocators, aiming to be as close to real world as it is practical
- [memory-allocators](https://github.com/mtrebi/memory-allocators) : Custom memory allocators in C++ to improve the performance of dynamic memory allocation
- [rpmalloc](https://github.com/mjansson/rpmalloc) : Public domain cross platform lock free thread caching 16-byte aligned memory allocator implemented in C
- [jemalloc](https://github.com/jemalloc/jemalloc)
- [cpp-mempool](https://github.com/hardikp/cpp-mempool) : C++ header-only mempool library
- [tiny](https://github.com/foonathan/tiny) : low-level library for minimizing the size of your types
- [memory](https://github.com/foonathan/memory) : STL compatible C++ memory allocator library using a new RawAllocator concept that is similar to an Allocator but easier to use and write.

## Mixed stuff

- [EASTL Gitter Chat](https://gitter.im/electronicarts/EASTL)

## Other useful link lists

- [Memory Performance Tools](https://github.com/MattPD/cpplinks/blob/master/performance.tools.md#memory) a list by [MattPD](https://github.com/MattPD)
- [Atomics, lock free, memory model](https://github.com/MattPD/cpplinks/blob/master/atomics.lockfree.memory_model.md) a list by [MattPD](https://github.com/MattPD)
- [Cache](https://github.com/MattPD/cpplinks/blob/master/comparch.micro.channels.md#cache) a list by [MattPD](https://github.com/MattPD)
- [Prefetch](https://github.com/MattPD/cpplinks/blob/master/comparch.micro.channels.md#prefetch) a list by [MattPD](https://github.com/MattPD)
- [Memory Bus](https://github.com/MattPD/cpplinks/blob/master/comparch.micro.channels.md#memory-bus) a list by [MattPD](https://github.com/MattPD)

