#if defined(__APPLE__)
# define TEST_FCN_DECL(ret, name, args) ret name args
# define TEST_FCN_REPL(ret, name, args) \
  ret name args; \
  ret repl_##name args; \
   __attribute__((used)) static struct { const void *fake; const void *real; } _interpose_##name \
   __attribute__((section ("__DATA,__interpose"))) = { (const void*)&repl_##name, (const void*)&name }; \
  ret repl_##name args
# define TEST_FCN_NEXT(name) name
#else
# define TEST_FCN_DECL(ret, name, args)
# define TEST_FCN_REPL(ret, name, args) ret name args
# define TEST_FCN_NEXT(name) dlsym(RTLD_NEXT, #name)
#endif
