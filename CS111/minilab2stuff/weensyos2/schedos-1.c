#include "schedos-app.h"
#include "x86sync.h"

/*****************************************************************************
 * schedos-1
 *
 *   This tiny application prints red "1"s to the console.
 *   It yields the CPU to the kernel after each "1" using the sys_yield()
 *   system call.  This lets the kernel (schedos-kern.c) pick another
 *   application to run, if it wants.
 *
 *   The other schedos-* processes simply #include this file after defining
 *   PRINTCHAR appropriately.
 *
 *****************************************************************************/

#ifndef PRINTCHAR
#define PRINTCHAR	('1' | 0x0C00)
#endif

#define ALTSYNC //signal for alternate sync

#ifndef PRIORITY
#define PRIORITY 6
#endif

#ifndef SHARE
#define SHARE 10
#endif

void
start(void)
{

  sys_set_priority(PRIORITY);
  sys_set_share(SHARE);
  sys_yield();

	int i;

	for (i = 0; i < RUNCOUNT; i++) {
		
#ifndef ALTSYNC
	  sys_print(PRINTCHAR);
#endif
#ifdef ALTSYNC
	  while(atomic_swap(&write_lock, 1) != 0)
	    continue;
	  // Write characters to the console, yielding after each one.
		*cursorpos++ = PRINTCHAR;
		atomic_swap(&write_lock, 0);
#endif
		sys_yield();
	}

	sys_exit(0);
}
