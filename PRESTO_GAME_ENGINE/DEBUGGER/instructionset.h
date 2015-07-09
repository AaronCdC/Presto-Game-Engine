#ifndef __instructionset_h__
#define __instructionset_h__

#include <intrin.h>
#include <iostream>
#include <vector>
#include <bitset>
#include <cstring>


class InstructionSet
{
public:

    #pragma pack(push, 1)
    struct cpuregs
    {
        int EAX;
        int EBX;
        int ECX;
        int EDX;
    }CPUREGS;
    #pragma pack(pop)

    struct procFlags
    {
        bool PYSICAL_ADDRESS_E;
        bool MULTIPROC_CAPABLE;
        bool EXT_MMX;
        bool MMX_INSTRUCTIONS;
        bool LONGMODE;
        bool EXT_3DNOW;
        bool B3DNOW;
        bool MULTIPROCESSOR_CAPABLE;

    }PFLAGS;
    char cpusignature[0x41];
    unsigned int cpuinfo[4];
    unsigned int maxleaf;
    InstructionSet();
private:

};
#endif // __instructionset_h__
