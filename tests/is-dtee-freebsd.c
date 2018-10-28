#include "is-dtee.h"

#include <sys/param.h>
#include <sys/syscall.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/user.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <kvm.h>
#include <libgen.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static const char *dtee_test_read_proc_basename(pid_t pid, char *buf, size_t buflen) {
	memset(buf, 0, buflen);

	kvm_t *kd;
	int cnt = 0;
	struct kinfo_proc *proc;

	kd = kvm_open(NULL, "/dev/null", NULL, O_RDONLY, NULL);
	if (kd != NULL) {
		proc = kvm_getprocs(kd, KERN_PROC_PID, pid, &cnt);
		if (proc != NULL && cnt > 0) {
			strncpy(buf, basename(proc[0].ki_comm), buflen - 1);
		}
		kvm_close(kd);
	}

	return buf;
}

static pid_t dtee_test_real_getpid(void) {
	// Don't call getpid() because it may be fake
	return syscall(SYS_getpid);
}

bool dtee_test_is_dtee(void) {
	bool is_dtee = false;
	static __thread bool active = false;

	if (!active) {
		char buf[PATH_MAX + 1];

		active = true;
		is_dtee = __dtee_test_is_dtee(dtee_test_read_proc_basename(dtee_test_real_getpid(), buf, sizeof(buf)));
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
		is_dtee_test = __dtee_test_is_dtee_test(dtee_test_read_proc_basename(dtee_test_real_getpid(), buf, sizeof(buf)));
		active = false;
	}

	return is_dtee_test;
}
