#define __xpg_strerror_r __dtee__hide____xpg_strerror_r
#define strerror_r __dtee__hide__strerror_r__
#define strerror __dtee__hide__strerror__

#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#undef strerror
#undef strerror_r
#undef __xpg_strerror_r

#include "is-dtee.h"
#include "dtee-fcn.h"

#if defined(__GLIBC__) || defined(__CYGWIN__)
# define CHAR_STRERROR_R 1
typedef char * strerror_r_t;
#else
# define CHAR_STRERROR_R 0
typedef int strerror_r_t;
#endif

#if defined(__CYGWIN__)
// winsup/cygwin/errno.cc uses an internal strerror implementation for C++
// error messages, so rewrite the strings in _sys_errlist instead 🤠
# define REWRITE_ERRLIST 1
#else
# define REWRITE_ERRLIST 0
#endif

TEST_FCN_DECL(char *, strerror, (int errnum));
TEST_FCN_DECL(strerror_r_t, strerror_r, (int errnum, char *buf, size_t buflen));

static char *dtee_test_strerror(int errnum) {
	switch (errnum) {
#define STRERROR_FOR(__errnum) \
	case __errnum: \
		return #__errnum; \
		break;

#ifdef EACCES
	STRERROR_FOR(EACCES)
#endif
#ifdef EAFNOSUPPORT
	STRERROR_FOR(EAFNOSUPPORT)
#endif
#ifdef EAGAIN
	STRERROR_FOR(EAGAIN)
#endif
#ifdef EBADF
	STRERROR_FOR(EBADF)
#endif
#ifdef ECHILD
	STRERROR_FOR(ECHILD)
#endif
#ifdef ECONNREFUSED
	STRERROR_FOR(ECONNREFUSED)
#endif
#ifdef ECONNRESET
	STRERROR_FOR(ECONNRESET)
#endif
#ifdef EINVAL
	STRERROR_FOR(EINVAL)
#endif
#ifdef EIO
	STRERROR_FOR(EIO)
#endif
#ifdef EMFILE
	STRERROR_FOR(EMFILE)
#endif
#ifdef ENOENT
	STRERROR_FOR(ENOENT)
#endif
#ifdef ENOSPC
	STRERROR_FOR(ENOSPC)
#endif
#ifdef ENOTCONN
	STRERROR_FOR(ENOTCONN)
#endif
#ifdef EPERM
	STRERROR_FOR(EPERM)
#endif
#ifdef EPIPE
	STRERROR_FOR(EPIPE)
#endif
#ifdef EROFS
	STRERROR_FOR(EROFS)
#endif

#undef STRERROR_FOR

	default:
		return NULL;
	}
}

static strerror_r_t dtee_test_fake_strerror_r(int errnum, char *buf, size_t buflen) {
	int errno_copy = errno;
	strerror_r_t (*next_strerror_r)(int, char *, size_t) = TEST_FCN_NEXT(strerror_r);

	memset(buf, 0, buflen);
	errno = errno_copy;

	char *ret = dtee_test_strerror(errnum);
	if (ret != NULL) {
		strncpy(buf, ret, buflen - 1);
		errno = errno_copy;
	} else {
		return (*next_strerror_r)(errnum, buf, buflen);
	}

#if CHAR_STRERROR_R
	return buf;
#else
	return 0;
#endif
}

static char *dtee_test_fake_strerror(int errnum) {
	int errno_copy = errno;
	char *(*next_strerror)(int) = TEST_FCN_NEXT(strerror);
	errno = errno_copy;

	char *ret = dtee_test_strerror(errnum);
	if (ret != NULL) {
		return ret;
	}

	return (*next_strerror)(errnum);
}

#if !defined(__APPLE__)
static char *dtee_test_fake_strerror_l(int errnum, locale_t locale __attribute__((unused))) {
	return dtee_test_fake_strerror(errnum);
}
#endif

TEST_FCN_REPL(char *, strerror, (int errnum)) {
	int errno_copy = errno;
	char *(*next_strerror)(int) = TEST_FCN_NEXT(strerror);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee() || dtee_test_is_ppid_dtee() || dtee_test_is_dtee_test()) {
			next_strerror = dtee_test_fake_strerror;
		}

		active = false;
	}

	errno = errno_copy;
	return (*next_strerror)(errnum);
}

TEST_FCN_REPL(strerror_r_t, strerror_r, (int errnum, char *buf, size_t buflen)) {
	int errno_copy = errno;
	strerror_r_t (*next_strerror_r)(int, char *, size_t) = TEST_FCN_NEXT(strerror_r);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee() || dtee_test_is_ppid_dtee() || dtee_test_is_dtee_test()) {
			next_strerror_r = dtee_test_fake_strerror_r;
		}

		active = false;
	}

	errno = errno_copy;
	return (*next_strerror_r)(errnum, buf, buflen);
}

#if !defined(__APPLE__)
TEST_FCN_REPL(char *, strerror_l, (int errnum, locale_t locale)) {
	int errno_copy = errno;
	char *(*next_strerror_l)(int, locale_t) = TEST_FCN_NEXT(strerror_l);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee() || dtee_test_is_ppid_dtee() || dtee_test_is_dtee_test()) {
			next_strerror_l = dtee_test_fake_strerror_l;
		}

		active = false;
	}

	errno = errno_copy;
	return (*next_strerror_l)(errnum, locale);
}
#endif

#if CHAR_STRERROR_R
static int dtee_test_fake___xpg_strerror_r(int errnum, char *buf, size_t buflen) {
	if (dtee_test_fake_strerror_r(errnum, buf, buflen) == NULL) {
		errno = EINVAL;
		return -1;
	}
	return 0;
}

TEST_FCN_REPL(int, __xpg_strerror_r, (int errnum, char *buf, size_t buflen)) {
	int errno_copy = errno;
	int (*next___xpg_strerror_r)(int, char *, size_t) = TEST_FCN_NEXT(__xpg_strerror_r);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee() || dtee_test_is_ppid_dtee() || dtee_test_is_dtee_test()) {
			next___xpg_strerror_r = dtee_test_fake___xpg_strerror_r;
		}

		active = false;
	}

	errno = errno_copy;
	return (*next___xpg_strerror_r)(errnum, buf, buflen);
}
#endif

#if REWRITE_ERRLIST
static void __attribute__((constructor)) dtee_test_rewrite_errlist(void) {
	char **errlist = (char **)_sys_errlist; // 🙈

	for (int i = 0; i < _sys_nerr; i++) {
		char *msg = dtee_test_strerror(i);
		if (msg != NULL) {
			errlist[i] = msg;
		}
	}
}
#endif
