/* lowlevelzs/mman.h - a simple mman.h wrapper */
#pragma once
#include "llzs_config.h"

#ifdef WIN32
/* mman-win32 - see src/lowlevel/win32_mman.c */
# ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
#  define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
# endif

/* All the headers include this file. */
# ifndef _MSC_VER
#  include <_mingw.h>
# endif

# include <sys/types.h>

# define S_IRWXG (S_IRWXU >> 3)
# define S_IRWXO (S_IRWXG >> 3)

# define PROT_NONE       0
# define PROT_READ       1
# define PROT_WRITE      2
# define PROT_EXEC       4

# define MAP_FILE        0
# define MAP_SHARED      1
# define MAP_PRIVATE     2
# define MAP_TYPE        0xf
# define MAP_FIXED       0x10
# define MAP_ANONYMOUS   0x20
# define MAP_ANON        MAP_ANONYMOUS

# define MAP_FAILED      ((void *)-1)
# define madvise(A,L,V)
# define MADV_SEQUENTIAL

# ifdef __cplusplus
extern "C" {
# endif

void*   mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
int     munmap(void *addr, size_t len);

# ifdef __cplusplus
};
# endif
#else
# include <sys/mman.h>
#endif
