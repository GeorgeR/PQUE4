/* include/pqxx/config.h.  Generated from config.h.in by configure.  */
/* include/pqxx/config.h.in.  Generated from configure.ac by autoheader.  */

/* most gcc compilers know a function __attribute__((__const__)) */
#define GCC_CONST_CALL __attribute__((__const__))

/* define if the compiler supports basic C++11 syntax */
#define HAVE_CXX11 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `pq' library (-lpq). */
#define HAVE_LIBPQ 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* System supports poll(). */
#define HAVE_POLL 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
/* #undef HAVE_SYS_SELECT_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "libpqxx"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "Jeroen T. Vermeulen"

/* Define to the full name of this package. */
#define PACKAGE_NAME "libpqxx"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libpqxx 6.1.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libpqxx"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "6.1.0"

/* Define if compiler supports [[deprecated]] attribute */
#define PQXX_HAVE_DEPRECATED 1

/* Define if the compiler supports std::experimental::optional. */
/* #undef PQXX_HAVE_EXP_OPTIONAL */

/* Define if g++ supports const attribute. */
#define PQXX_HAVE_GCC_CONST 1

/* Define if g++ supports deprecated attribute, as in g++ 4.0 */
/* #undef PQXX_HAVE_GCC_DEPRECATED */

/* Define if g++ supports pure attribute */
#define PQXX_HAVE_GCC_PURE 1

/* Define if g++ supports visibility attribute, as in g++ 4.0 */
#define PQXX_HAVE_GCC_VISIBILITY 1

/* Define if the compiler supports std::optional. */
/* #undef PQXX_HAVE_OPTIONAL */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "6.1.0"
