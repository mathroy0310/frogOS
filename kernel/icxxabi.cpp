/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   icxxabi.cpp                                  ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 21:12:05 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 22:52:08 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#define ATEXIT_MAX_FUNCS 128

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned uarch_t;

struct atexit_func_entry_t {
	/*
	 * Each member is at least 4 bytes large. Such that each entry is 12bytes.
	 * 128 * 12 = 1.5KB exact.
	 **/
	void (*destructor_func)(void *);
	void *obj_ptr;
	void *dso_handle;
};

atexit_func_entry_t __atexit_funcs[ATEXIT_MAX_FUNCS];
uarch_t             __atexit_func_count = 0;

int __cxa_atexit(void (*f)(void *), void *objptr, void *dso) {
	if (__atexit_func_count >= ATEXIT_MAX_FUNCS) {
		return -1;
	};
	__atexit_funcs[__atexit_func_count].destructor_func = f;
	__atexit_funcs[__atexit_func_count].obj_ptr = objptr;
	__atexit_funcs[__atexit_func_count].dso_handle = dso;
	__atexit_func_count++;
	return 0;
};

void __cxa_finalize(void *f) {
	uarch_t i = __atexit_func_count;
	if (!f) {
		while (i--) {
			if (__atexit_funcs[i].destructor_func) {
				(*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
			};
		};
		return;
	};

	while (i--) {
		if (__atexit_funcs[i].destructor_func == f) {
			(*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
			__atexit_funcs[i].destructor_func = 0;
		};
	};
};

#ifdef __cplusplus
};
#endif
