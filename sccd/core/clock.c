#include <sccd/core/clock.h>

#include <stdio.h>
#ifdef __ARM_ARCH_6M__
	/* Used for ARM Cortex-M0+ and more. */
	volatile unsigned int *DWT_CYCCNT = (unsigned int *)0xE0001004;
	volatile unsigned int *DWT_CONTROL = (unsigned int *)0xE0001000;
	volatile unsigned int *SCB_DEMCR = (unsigned int *)0xE000EDFC;
#endif

void sccd_cycles_init(void) {
#if __x86_64__ || __ppc64__

#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__)
	/*
	 * This relies on a Kernel module described in:
	 * http://blog.regehr.org/archives/794
	 */
	__asm__ volatile (
		"mcr p15, 0, %0, c15, c12, 0" : : "r" (1)
	);
#elif __ARM_ARCH_6M__
	*SCB_DEMCR = *SCB_DEMCR | 0x01000000;
	*DWT_CYCCNT = 0;
	*DWT_CONTROL = *DWT_CONTROL | 1;
#else
	#warning No timing init required on platform.
#endif
}

const char* sccd_cylces_unit(void) {
#ifdef SCCD_RIOT_TIMER
	return "µs";
#else
	return "cylces";
#endif
}

extern uint64_t xtimer_now64(void);

uint64_t sccd_cycles(void) {
	uint64_t cycles = 0;
#ifdef SCCD_RIOT_TIMER
	cycles = xtimer_now64();
#else
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
#elif __ARM_ARCH_6M__
	printf("ccount: %lu\n", *DWT_CYCCNT);
	cycles = *DWT_CYCCNT;
#else
#error unsupported platform
#endif
#endif
	return cycles;
}
