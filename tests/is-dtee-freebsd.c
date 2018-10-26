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
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static bool dtee_test_is_proc_dtee(pid_t pid) {
	bool is_dtee = false;
	kvm_t *kd;
	int cnt = 0;
	struct kinfo_proc *proc;

	kd = kvm_open(NULL, "/dev/null", NULL, O_RDONLY, NULL);
	if (kd != NULL) {
		proc = kvm_getprocs(kd, KERN_PROC_PID, pid, &cnt);
		if (proc != NULL && cnt > 0) {
			const char *base_program_name = basename(proc[0].ki_comm);

			is_dtee = !strcmp(base_program_name, "dtee");
		}
		kvm_close(kd);
	}

	return is_dtee;
}

bool dtee_test_is_dtee(void) {
	bool is_dtee = false;
	static __thread bool active = false;

	if (!active) {
		active = true;

		// Don't call getpid() because it may be fake
		pid_t pid = syscall(SYS_getpid);
		is_dtee = dtee_test_is_proc_dtee(pid);

		active = false;
	}

	return is_dtee;
}

bool dtee_test_is_ppid_dtee(void) {
	bool is_ppid_dtee = false;
	static __thread bool active = false;

	if (!active) {
		active = true;

		is_ppid_dtee = dtee_test_is_proc_dtee(getppid());

		active = false;
	}

	return is_ppid_dtee;
}
