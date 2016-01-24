#include <sccd/core/clock.h>

void sccd_cycles_init() {
#if __x86_64__ || __ppc64__

#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__)
	/*
	 * This relies on a Kernel module described in:
	 * http://blog.regehr.org/archives/794
	 */
	__asm__ volatile (
		"mcr p15, 0, %0, c15, c12, 0" : : "r" (1)
	);
#else
	#warning No timing init required on platform.
#endif
}

uint64_t sccd_cycles() {
	uint64_t cycles = 0;
#if __x86_64__ || __ppc64__
	uint32_t hi = 0, lo = 0;
	__asm__ volatile (
		"cpuid\n\t"/*serialize*/
		"rdtsc\n\t"/*read the clock*/
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t"
		: "=r" (hi), "=r" (lo):: "%rax", "%rbx", "%rcx", "%rdx"
	);
	cycles = ((uint64_t) lo) | (((uint64_t) hi) << 32);
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__)
	unsigned int r;
	/* __asm__ volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) ); */
	__asm__ volatile ("mrc p15, 0, %0, c15, c12, 1":"=r" (r));
	cycles = r;
#else
#error unsupported platform
#endif
	return cycles;
}
