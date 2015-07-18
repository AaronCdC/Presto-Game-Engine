#include "instructionset.h"
#include <cstdlib>
InstructionSet::InstructionSet()
{
        //cpusignature = (char*)malloc(0x40);
        //memset(cpusignature,0x0,0x40);

#ifdef _DEBUG
        unsigned int* REGSPTR = (unsigned int*)&CPUREGS;
#endif

        memset(&CPUREGS, 0x0, sizeof(CPUREGS));
		memset(&cpusignature, 0x0, 0x43);
        __cpuid((int*)&CPUREGS, 0);
        maxleaf = 0x80000000 + CPUREGS.EAX;
		//if(maxleaf < 0x80000004)
        //return; //Unsupported brand string

	//Get the CPU brand name and model
    for(int x = 0x80000002; x <= maxleaf; x++)
    {

        __cpuid((int*)&CPUREGS, x);
        memcpy((char*)(cpusignature + (x - 0x80000002)*sizeof(unsigned int)*4), (char*)&CPUREGS, sizeof(unsigned int)*4);

    }

	//Get the CPU information
	__cpuid((int*)&CPUREGS, 0x80000001);

    std::bitset<sizeof(unsigned int)*8> EDXBITS(CPUREGS.EDX);

        PFLAGS.PYSICAL_ADDRESS_E = EDXBITS.test(6);
        PFLAGS.MULTIPROC_CAPABLE = EDXBITS.test(19);
        PFLAGS.EXT_MMX = EDXBITS.test(22);
        PFLAGS.MMX_INSTRUCTIONS = EDXBITS.test(23);
        PFLAGS.LONGMODE = EDXBITS.test(29);
        PFLAGS.EXT_3DNOW = EDXBITS.test(30);
        PFLAGS.B3DNOW = EDXBITS.test(31);
        PFLAGS.MULTIPROCESSOR_CAPABLE = EDXBITS.test(19);


}
