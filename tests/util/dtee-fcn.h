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
# define TEST_FCN_REPL(ret, name, args) \
  TEST_FCN_DECL(ret, name, args); \
  ret name args; \
  static __attribute__((constructor)) void test_fcn_init_##name(void) { \
   real_##name = (void*)cygwin_internal(CW_HOOK, #name, name); \
  } \
  ret name args
# define TEST_FCN_NEXT(name) real_##name
#else
# define TEST_FCN_DECL(ret, name, args)
# define TEST_FCN_REPL(ret, name, args) ret name args
# define TEST_FCN_NEXT(name) dlsym(RTLD_NEXT, #name)
#endif
