#ifndef NUMBER_H
#define NUMBER_H

#ifdef USE_DOUBLES
typedef double number_t;
#define NUMBER_FMT "lf"
#else
typedef float number_t;
#define NUMBER_FMT "f"
#endif

#endif /* NUMBER_H */
