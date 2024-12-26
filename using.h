#ifndef _USING_H
#define _USING_H

#define _ARG0(ZERO, ...) ZERO
#define _ARG1ANDREMANDER(ZERO, ONE, ...) ONE, __VA_ARGS__

#define __CCAT(X, Y) X##Y
#define _CCAT(x, y) __CCAT(x, y)
#define _UNIQUE_VAR_NAME(countval) _CCAT(latch, countval)

#define using(...)                        \
    int _UNIQUE_VAR_NAME(__LINE__) = 0; \
    for (_ARG0(__VA_ARGS__); _UNIQUE_VAR_NAME(__LINE__) < 1; ++_UNIQUE_VAR_NAME(__LINE__), _ARG1ANDREMANDER(__VA_ARGS__))


#endif //_USING_H