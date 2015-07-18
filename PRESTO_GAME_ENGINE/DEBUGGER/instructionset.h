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

	//Since I don't need the 16 bit registers, doing every
	//32-bit register into a single INT is faster.
    #pragma pack(push, 1)
    struct cpuregs
    {
        int EAX;
        int EBX;
        int ECX;
        int EDX;
    }CPUREGS;
    #pragma pack(pop)

	//Processor flags. This tells us what the processor has.
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

	//CPU brand and model information
    char cpusignature[0x41];
    //unsigned int cpuinfo[4];

	//This is a parameter that is different for each CPU and we need to know.
    unsigned int maxleaf;
    InstructionSet();
private:

};
#endif // __instructionset_h__
