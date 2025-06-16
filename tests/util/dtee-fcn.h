#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
 * dlsym() is called once up front even on Unix otherwise the following
 * sequence of events can happen on Linux with glibc when wrapping write():
 *
 *   write()
 *     dlsym()
 *       pthread_mutex_lock()
 *         <signal handler>
 *           boost_asio_signal_handler()
 *             write()
 *               dlsym()
 *                 pthread_mutex_lock()
 *                   <deadlock>
 *
 * Even with a recursive mutex, because it's not expecting a signal handler
 * during the mutex lock to be attempting to acquire the same mutex, it hangs.
 */

#if defined(__APPLE__)
# define TEST_FCN_DECL(ret, name, args) ret name args
# define TEST_FCN_REPL(ret, name, args) \
  TEST_FCN_DECL(ret, name, args); \
  ret repl_##name args; \
   __attribute__((used)) static struct { const void *fake; const void *real; } _interpose_##name \
   __attribute__((section ("__DATA,__interpose"))) = { (const void*)&repl_##name, (const void*)&name }; \
  ret repl_##name args
# define TEST_FCN_NEXT(name) name
#elif defined(__CYGWIN__)
# include <sys/cygwin.h>
# define TEST_FCN_DECL(ret, name, args) static ret (*real_##name) args
# ifdef __cplusplus
#  define TEST_FCN_REPEAT_DECL(ret, name, args)
#  define TEST_FCN_CAST(ret, name, args, call) reinterpret_cast<ret (*) args>(call)
# else
#  define TEST_FCN_REPEAT_DECL(ret, name, args) TEST_FCN_DECL(ret, name, args)
#  define TEST_FCN_CAST(ret, name, args, call) (void*)call
# endif
# define TEST_FCN_REPL(ret, name, args) \
  TEST_FCN_REPEAT_DECL(ret, name, args); \
  ret name args; \
  static __attribute__((constructor(2000))) void test_fcn_init_##name(void) { \
    real_##name = TEST_FCN_CAST(ret, name, args, cygwin_internal(CW_HOOK, #name, name)); \
  } \
  ret name args
# define TEST_FCN_NEXT(name) real_##name
#else
# include <dlfcn.h>
# define TEST_FCN_DECL(ret, name, args) static ret (*real_##name) args
# ifdef __cplusplus
#  define TEST_FCN_REPEAT_DECL(ret, name, args)
#  define TEST_FCN_CAST(ret, name, args, call) reinterpret_cast<ret (*) args>(call)
# else
#  define TEST_FCN_REPEAT_DECL(ret, name, args) TEST_FCN_DECL(ret, name, args)
#  define TEST_FCN_CAST(ret, name, args, call) (void*)call
# endif
# define TEST_FCN_REPL(ret, name, args) \
  TEST_FCN_REPEAT_DECL(ret, name, args); \
  ret name args; \
  static __attribute__((constructor(2000))) void test_fcn_init_##name(void) { \
    real_##name = TEST_FCN_CAST(ret, name, args, dlsym(RTLD_NEXT, #name)); \
  } \
  ret name args
# define TEST_FCN_NEXT(name) real_##name
#endif

#ifdef __cplusplus
}
#endif
