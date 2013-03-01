#ifdef _USE_DOUBLE
#define tReal double
#define _R(x) x
#else
#define tReal float
#define _R(x) x##f
#endif