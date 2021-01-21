[![Total alerts](https://img.shields.io/lgtm/alerts/g/carloslack/volundr.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/carloslack/volundr/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/carloslack/volundr.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/carloslack/volundr/context:cpp)

<p align="left">
    <a href="https://github.com/carloslack/volundr/blob/master/LICENSE"><img alt="Software License" src="https://img.shields.io/badge/MIT-license-green.svg?style=flat-square"></a>
</p>


elf/            = ELF library
asm/            = syscalls
common/         = Common library
examples/       = General use of API (dummy trojan included)

## Load auto-completion before running the examples

     $ source ./completion.sh

## Example options

     $ ./run
    Use <example code> [[parameters] [-h]]
    Parameters:
    example-headers
    example-infect01
    example-long
    example-sctidx

## Trojan ET_EXEC example
    *** ET_DYN also supported ***


    $ cat victim.c
    #include <stdio.h>
    int main(int argc, char **argv) {
        int i;
        for (i = 0; i < 10; ++i)
            printf("%d\n",i);
        return 0;
    }
     ---snip---
     $ gcc victim.c && ./a.out
     0
     1
     2
     3
     4
     5
     6
     7
     8
     9
     ---snip---
     $ ./run example-infect01 a.out parasite
    Done!
    Try running a.out
     ---snip---
     $ ./a.out
    -= Objective reality doesn't exist! =-    <---------- yay!
    0
    1
    2
    3
    4
    5
    6
    7
    8
    9

## Parsing Example output

     $ ./run example-long -f a.out
    -= Volundr reading from a.out of 8608 bytes =-
    Elf Header:
    Magic:                               7f ELF
    Class:                               64-Bit object
    Data:                                2's complement, little endian
    Version:                             1 Current version
    OS ABI:                              UNIX System V ABI
    ABI Version:                         0
    Type:                                Executable file
    Machine:                             AMD x86-64 architecture
    Version:                             0x1
    Entry point address:                 0000000000400430
    Start of the program header:         64 (bytes into file)
    Start of the section header:         6624 (bytes into file)
    Flags:                               0000000000000000
    Size of this header:                 64 (bytes)
    Size of program headers:             56 (bytes)
    Number of program headers:           9
    Size of section header:              64 (bytes)
    Number of section headers:           31
    Section heander string table index:  28
    Program Headers:
          Type             Offset           Filesz           Vaddr            Paddr            Namesz           Align            Name
     0: 0000000000000006 0000000000000040 00000000000001f8 0000000000400040 0000000000400040 00000000000001f8 0000000000000008 Header table entry 
     1: 0000000000000003 0000000000000238 000000000000001c 0000000000400238 0000000000400238 000000000000001c 0000000000000001 Program interpreter 
     2: 0000000000000001 0000000000000000 0000000000000714 0000000000400000 0000000000400000 0000000000000714 0000000000200000 Loadable segment 
     3: 0000000000000001 0000000000000e10 0000000000000228 0000000000600e10 0000000000600e10 0000000000000230 0000000000200000 Loadable segment 
     4: 0000000000000002 0000000000000e28 00000000000001d0 0000000000600e28 0000000000600e28 00000000000001d0 0000000000000008 Dynamic linking 
     5: 0000000000000004 0000000000000254 0000000000000044 0000000000400254 0000000000400254 0000000000000044 0000000000000004 Auxiliary information 
     6: 000000006474e550 00000000000005e8 0000000000000034 00000000004005e8 00000000004005e8 0000000000000034 0000000000000004 GCC .eh_frame_hdr segment 
     7: 000000006474e551 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000010 Stack executability 
     8: 000000006474e552 0000000000000e10 00000000000001f0 0000000000600e10 0000000000600e10 00000000000001f0 0000000000000001 Read-only after relocation Section Headers:
          Type             Flags            Addr             Offset           Size             Link             Info             Align            Entsize
     0: 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000  
     1: 0000000000000001 0000000000000002 0000000000400238 0000000000000238 000000000000001c 0000000000000000 0000000000000000 0000000000000001 0000000000000000 .interp 
     2: 0000000000000007 0000000000000002 0000000000400254 0000000000000254 0000000000000020 0000000000000000 0000000000000000 0000000000000004 0000000000000000 .note.ABI-tag 
     3: 0000000000000007 0000000000000002 0000000000400274 0000000000000274 0000000000000024 0000000000000000 0000000000000000 0000000000000004 0000000000000000 .note.gnu.build-id 
     4: 000000006ffffff6 0000000000000002 0000000000400298 0000000000000298 000000000000001c 0000000000000005 0000000000000000 0000000000000008 0000000000000000 .gnu.hash 
     5: 000000000000000b 0000000000000002 00000000004002b8 00000000000002b8 0000000000000060 0000000000000006 0000000000000001 0000000000000008 0000000000000018 .dynsym 
     6: 0000000000000003 0000000000000002 0000000000400318 0000000000000318 000000000000003f 0000000000000000 0000000000000000 0000000000000001 0000000000000000 .dynstr 
     7: 000000006fffffff 0000000000000002 0000000000400358 0000000000000358 0000000000000008 0000000000000005 0000000000000000 0000000000000002 0000000000000002 .gnu.version 
     8: 000000006ffffffe 0000000000000002 0000000000400360 0000000000000360 0000000000000020 0000000000000006 0000000000000001 0000000000000008 0000000000000000 .gnu.version_r 
     9: 0000000000000004 0000000000000002 0000000000400380 0000000000000380 0000000000000018 0000000000000005 0000000000000000 0000000000000008 0000000000000018 .rela.dyn 
    10: 0000000000000004 0000000000000042 0000000000400398 0000000000000398 0000000000000030 0000000000000005 0000000000000018 0000000000000008 0000000000000018 .rela.plt 
    11: 0000000000000001 0000000000000006 00000000004003c8 00000000000003c8 000000000000001a 0000000000000000 0000000000000000 0000000000000004 0000000000000000 .init 
    12: 0000000000000001 0000000000000006 00000000004003f0 00000000000003f0 0000000000000030 0000000000000000 0000000000000000 0000000000000010 0000000000000010 .plt 
    13: 0000000000000001 0000000000000006 0000000000400420 0000000000000420 0000000000000008 0000000000000000 0000000000000000 0000000000000008 0000000000000000 .plt.got 
    14: 0000000000000001 0000000000000006 0000000000400430 0000000000000430 00000000000001a2 0000000000000000 0000000000000000 0000000000000010 0000000000000000 .text 
    15: 0000000000000001 0000000000000006 00000000004005d4 00000000000005d4 0000000000000009 0000000000000000 0000000000000000 0000000000000004 0000000000000000 .fini 
    16: 0000000000000001 0000000000000002 00000000004005e0 00000000000005e0 0000000000000008 0000000000000000 0000000000000000 0000000000000004 0000000000000000 .rodata 
    17: 0000000000000001 0000000000000002 00000000004005e8 00000000000005e8 0000000000000034 0000000000000000 0000000000000000 0000000000000004 0000000000000000 .eh_frame_hdr 
    18: 0000000000000001 0000000000000002 0000000000400620 0000000000000620 00000000000000f4 0000000000000000 0000000000000000 0000000000000008 0000000000000000 .eh_frame 
    19: 000000000000000e 0000000000000003 0000000000600e10 0000000000000e10 0000000000000008 0000000000000000 0000000000000000 0000000000000008 0000000000000000 .init_array 
    20: 000000000000000f 0000000000000003 0000000000600e18 0000000000000e18 0000000000000008 0000000000000000 0000000000000000 0000000000000008 0000000000000000 .fini_array 
    21: 0000000000000001 0000000000000003 0000000000600e20 0000000000000e20 0000000000000008 0000000000000000 0000000000000000 0000000000000008 0000000000000000 .jcr 
    22: 0000000000000006 0000000000000003 0000000000600e28 0000000000000e28 00000000000001d0 0000000000000006 0000000000000000 0000000000000008 0000000000000010 .dynamic 
    23: 0000000000000001 0000000000000003 0000000000600ff8 0000000000000ff8 0000000000000008 0000000000000000 0000000000000000 0000000000000008 0000000000000008 .got 
    24: 0000000000000001 0000000000000003 0000000000601000 0000000000001000 0000000000000028 0000000000000000 0000000000000000 0000000000000008 0000000000000008 .got.plt 
    25: 0000000000000001 0000000000000003 0000000000601028 0000000000001028 0000000000000010 0000000000000000 0000000000000000 0000000000000008 0000000000000000 .data 
    26: 0000000000000008 0000000000000003 0000000000601038 0000000000001038 0000000000000008 0000000000000000 0000000000000000 0000000000000001 0000000000000000 .bss 
    27: 0000000000000001 0000000000000030 0000000000000000 0000000000001038 0000000000000035 0000000000000000 0000000000000000 0000000000000001 0000000000000001 .comment 
    28: 0000000000000003 0000000000000000 0000000000000000 00000000000018cd 000000000000010c 0000000000000000 0000000000000000 0000000000000001 0000000000000000 .shstrtab 
    29: 0000000000000002 0000000000000000 0000000000000000 0000000000001070 0000000000000648 000000000000001e 000000000000002f 0000000000000008 0000000000000018 .symtab 
    30: 0000000000000003 0000000000000000 0000000000000000 00000000000016b8 0000000000000215 0000000000000000 0000000000000000 0000000000000001 0000000000000000 .strtab .dynsym has 4 symbols:
    Num        Val          Size    Type         Bind  Vis      Shndx Name
      0: 0000000000000000      0     NOTYPE      LOCAL DEFAULT    UND       
      1: 0000000000000000      0       FUNC     GLOBAL DEFAULT    UND printf
      2: 0000000000000000      0       FUNC     GLOBAL DEFAULT    UND __libc_start_main
      3: 0000000000000000      0     NOTYPE       WEAK DEFAULT    UND __gmon_start__
    .symtab has 67 symbols:
    Num        Val          Size    Type         Bind  Vis      Shndx Name
      0: 0000000000000000      0     NOTYPE      LOCAL DEFAULT    UND       
      1: 0000000000400238      0    SECTION      LOCAL DEFAULT      1       
      2: 0000000000400254      0    SECTION      LOCAL DEFAULT      2       
      3: 0000000000400274      0    SECTION      LOCAL DEFAULT      3       
      4: 0000000000400298      0    SECTION      LOCAL DEFAULT      4       
      5: 00000000004002b8      0    SECTION      LOCAL DEFAULT      5       
      6: 0000000000400318      0    SECTION      LOCAL DEFAULT      6       
      7: 0000000000400358      0    SECTION      LOCAL DEFAULT      7       
      8: 0000000000400360      0    SECTION      LOCAL DEFAULT      8       
      9: 0000000000400380      0    SECTION      LOCAL DEFAULT      9       
     10: 0000000000400398      0    SECTION      LOCAL DEFAULT     10       
     11: 00000000004003c8      0    SECTION      LOCAL DEFAULT     11       
     12: 00000000004003f0      0    SECTION      LOCAL DEFAULT     12       
     13: 0000000000400420      0    SECTION      LOCAL DEFAULT     13       
     14: 0000000000400430      0    SECTION      LOCAL DEFAULT     14       
     15: 00000000004005d4      0    SECTION      LOCAL DEFAULT     15       
     16: 00000000004005e0      0    SECTION      LOCAL DEFAULT     16       
     17: 00000000004005e8      0    SECTION      LOCAL DEFAULT     17       
     18: 0000000000400620      0    SECTION      LOCAL DEFAULT     18       
     19: 0000000000600e10      0    SECTION      LOCAL DEFAULT     19       
     20: 0000000000600e18      0    SECTION      LOCAL DEFAULT     20       
     21: 0000000000600e20      0    SECTION      LOCAL DEFAULT     21       
     22: 0000000000600e28      0    SECTION      LOCAL DEFAULT     22       
     23: 0000000000600ff8      0    SECTION      LOCAL DEFAULT     23       
     24: 0000000000601000      0    SECTION      LOCAL DEFAULT     24       
     25: 0000000000601028      0    SECTION      LOCAL DEFAULT     25       
     26: 0000000000601038      0    SECTION      LOCAL DEFAULT     26       
     27: 0000000000000000      0    SECTION      LOCAL DEFAULT     27       
     28: 0000000000000000      0       FILE      LOCAL DEFAULT    ABS crtstuff.c
     29: 0000000000600e20      0     OBJECT      LOCAL DEFAULT     21 __JCR_LIST__
     30: 0000000000400460      0       FUNC      LOCAL DEFAULT     14 deregister_tm_clones
     31: 00000000004004a0      0       FUNC      LOCAL DEFAULT     14 register_tm_clones
     32: 00000000004004e0      0       FUNC      LOCAL DEFAULT     14 __do_global_dtors_aux
     33: 0000000000601038      1     OBJECT      LOCAL DEFAULT     26 completed.7594
     34: 0000000000600e18      0     OBJECT      LOCAL DEFAULT     20 __do_global_dtors_aux_fini_array_entry
     35: 0000000000400500      0       FUNC      LOCAL DEFAULT     14 frame_dummy
     36: 0000000000600e10      0     OBJECT      LOCAL DEFAULT     19 __frame_dummy_init_array_entry
     37: 0000000000000000      0       FILE      LOCAL DEFAULT    ABS nada.c
     38: 0000000000000000      0       FILE      LOCAL DEFAULT    ABS crtstuff.c
     39: 0000000000400710      0     OBJECT      LOCAL DEFAULT     18 __FRAME_END__
     40: 0000000000600e20      0     OBJECT      LOCAL DEFAULT     21 __JCR_END__
     41: 0000000000000000      0       FILE      LOCAL DEFAULT    ABS       
     42: 0000000000600e18      0     NOTYPE      LOCAL DEFAULT     19 __init_array_end
     43: 0000000000600e28      0     OBJECT      LOCAL DEFAULT     22 _DYNAMIC
     44: 0000000000600e10      0     NOTYPE      LOCAL DEFAULT     19 __init_array_start
     45: 00000000004005e8      0     NOTYPE      LOCAL DEFAULT     17 __GNU_EH_FRAME_HDR
     46: 0000000000601000      0     OBJECT      LOCAL DEFAULT     24 _GLOBAL_OFFSET_TABLE_
     47: 00000000004005d0      2       FUNC     GLOBAL DEFAULT     14 __libc_csu_fini
     48: 0000000000000000      0     NOTYPE       WEAK DEFAULT    UND _ITM_deregisterTMCloneTable
     49: 0000000000601028      0     NOTYPE       WEAK DEFAULT     25 data_start
     50: 0000000000601038      0     NOTYPE     GLOBAL DEFAULT     25 _edata
     51: 00000000004005d4      0       FUNC     GLOBAL DEFAULT     15  _fini
     52: 0000000000000000      0       FUNC     GLOBAL DEFAULT    UND printf@@GLIBC_2.2.5
     53: 0000000000000000      0       FUNC     GLOBAL DEFAULT    UND __libc_start_main@@GLIBC_2.2.5
     54: 0000000000601028      0     NOTYPE     GLOBAL DEFAULT     25 __data_start
     55: 0000000000000000      0     NOTYPE       WEAK DEFAULT    UND __gmon_start__
     56: 0000000000601030      0     OBJECT     GLOBAL HIDDEN     25 __dso_handle
     57: 00000000004005e0      4     OBJECT     GLOBAL DEFAULT     16 _IO_stdin_used
     58: 0000000000400560    101       FUNC     GLOBAL DEFAULT     14 __libc_csu_init
     59: 0000000000601040      0     NOTYPE     GLOBAL DEFAULT     26   _end
     60: 0000000000400430     42       FUNC     GLOBAL DEFAULT     14 _start
     61: 0000000000601038      0     NOTYPE     GLOBAL DEFAULT     26 __bss_start
     62: 0000000000400526     53       FUNC     GLOBAL DEFAULT     14   main
     63: 0000000000000000      0     NOTYPE       WEAK DEFAULT    UND _Jv_RegisterClasses
     64: 0000000000601038      0     OBJECT     GLOBAL HIDDEN     25 __TMC_END__
     65: 0000000000000000      0     NOTYPE       WEAK DEFAULT    UND _ITM_registerTMCloneTable
     66: 00000000004003c8      0       FUNC     GLOBAL DEFAULT     11  _init
    
