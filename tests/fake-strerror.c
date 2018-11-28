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

TEST_FCN_DECL(char *, strerror, (int errnum));
#ifdef __GLIBC__
TEST_FCN_DECL(char *, strerror_r, (int errnum, char *buf, size_t buflen));
#else
TEST_FCN_DECL(int, strerror_r, (int errnum, char *buf, size_t buflen));
#endif

#ifdef __GLIBC__
static char *dtee_test_fake_strerror_r(int errnum, char *buf, size_t buflen) {
#else
static int dtee_test_fake_strerror_r(int errnum, char *buf, size_t buflen) {
#endif
	int errno_copy = errno;
#ifdef __GLIBC__
	char *(*next_strerror_r)(int, char *, size_t) = TEST_FCN_NEXT(strerror_r);
#else
	int (*next_strerror_r)(int, char *, size_t) = TEST_FCN_NEXT(strerror_r);
#endif

	memset(buf, 0, buflen);

	switch (errnum) {
#define STRERROR_FOR(__errnum) \
	case __errnum: \
		snprintf(buf, buflen, #__errnum); \
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
		errno = errno_copy;
		return (*next_strerror_r)(errnum, buf, buflen);
	}

	errno = errno_copy;
#ifdef __GLIBC__
	return buf;
#else
	return 0;
#endif
}

static char *dtee_test_fake_strerror(int errnum) {
	static char buf[1024];

#ifdef __GLIBC__
	return dtee_test_fake_strerror_r(errnum, buf, sizeof(buf));
#else
	int errno_copy = errno;
	char *(*next_strerror)(int) = TEST_FCN_NEXT(strerror);

	errno = errno_copy;
	if (dtee_test_fake_strerror_r(errnum, buf, sizeof(buf)) != 0) {
		errno = errno_copy;
		return (*next_strerror)(errnum);
	}

	errno = errno_copy;
	return buf;
#endif
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

// glibc implements the POSIX strerror_r as __xpg_strerror_r
// which then calls the old strerror_r function and assumes
// that errnum is unknown if the return value is not buf
#ifdef __GLIBC__
TEST_FCN_REPL(char *, strerror_r, (int errnum, char *buf, size_t buflen)) {
#else
TEST_FCN_REPL(int, strerror_r, (int errnum, char *buf, size_t buflen)) {
#endif
	int errno_copy = errno;
#ifdef __GLIBC__
	char *(*next_strerror_r)(int, char *, size_t) = TEST_FCN_NEXT(strerror_r);
#else
	int (*next_strerror_r)(int, char *, size_t) = TEST_FCN_NEXT(strerror_r);
#endif
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

#ifdef __GLIBC__
static int dtee_test_fake___xpg_strerror_r(int errnum, char *buf, size_t buflen) {
# ifdef __GLIBC__
	dtee_test_fake_strerror_r(errnum, buf, buflen);
	return 0;
# else
	return dtee_test_fake_strerror_r(errnum, buf, buflen);
# endif
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
