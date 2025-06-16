#include "is-dtee.h"

#include <sys/types.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#if !defined(__OpenBSD__) && !defined(__NetBSD__)
# include <sys/user.h>
#endif
#include <fcntl.h>
#include <kvm.h>
#include <libgen.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static const char *dtee_test_read_proc_basename(pid_t pid, char *buf, size_t buflen) {
	memset(buf, 0, buflen);

	kvm_t *kd;
	int cnt = 0;
#if defined(__NetBSD__)
	struct kinfo_proc2 *proc;
#else
	struct kinfo_proc *proc;
#endif
	char errbuf[_POSIX2_LINE_MAX] = { 0 };

#if defined(__FreeBSD__) || defined(__DragonFly__)
	kd = kvm_openfiles(NULL, "/dev/null", NULL, O_RDONLY, errbuf);
#elif defined(__OpenBSD__) || defined(__NetBSD__)
	kd = kvm_openfiles(NULL, NULL, NULL, O_RDONLY | KVM_NO_FILES, errbuf);
#else
# error "Unknown BSD"
#endif
	if (kd != NULL) {
#if defined(__FreeBSD__) || defined(__DragonFly__)
		proc = kvm_getprocs(kd, KERN_PROC_PID, pid, &cnt);
#elif defined(__OpenBSD__)
		proc = kvm_getprocs(kd, KERN_PROC_PID, pid, sizeof(*proc), &cnt);
#elif defined(__NetBSD__)
		proc = kvm_getproc2(kd, KERN_PROC_PID, pid, sizeof(*proc), &cnt);
#else
# error "Unknown BSD"
#endif
		if (proc != NULL && cnt > 0) {
#if defined(__FreeBSD__)
			strncpy(buf, basename(proc[0].ki_comm), buflen - 1);
#elif defined(__OpenBSD__) || defined(__NetBSD__)
			strncpy(buf, basename(proc[0].p_comm), buflen - 1);
#elif defined(__DragonFly__)
			strncpy(buf, basename(proc[0].kp_comm), buflen - 1);
#else
# error "Unknown BSD"
#endif
		}
		kvm_close(kd);
	} else {
		fprintf(stderr, "is-dtee: kvm_openfiles: %s\n", errbuf);
	}

	return buf;
}

bool __dtee_test_is_dtee_impl(void) {
	bool is_dtee = false;
	static __thread bool active = false;

	if (!active) {
		char buf[PATH_MAX + 1];

		active = true;
		is_dtee = __dtee_test_is_dtee(dtee_test_read_proc_basename(getpid(), buf, sizeof(buf)));
		active = false;
	}

	return is_dtee;
}

bool dtee_test_is_ppid_dtee(void) {
	bool is_ppid_dtee = false;
	static __thread bool active = false;

	if (!active) {
		char buf[PATH_MAX + 1];

		active = true;
		is_ppid_dtee = __dtee_test_is_dtee(dtee_test_read_proc_basename(getppid(), buf, sizeof(buf)));
		active = false;
	}

	return is_ppid_dtee;
}

bool dtee_test_is_dtee_test(void) {
	bool is_dtee_test = false;
	static __thread bool active = false;

	if (!active) {
		char buf[PATH_MAX + 1];

		active = true;
		is_dtee_test = __dtee_test_is_dtee_test(dtee_test_read_proc_basename(getpid(), buf, sizeof(buf)));
		active = false;
	}

	return is_dtee_test;
}
