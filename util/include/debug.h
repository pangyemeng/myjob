#ifndef __DEBUG_H__
#define __DEBUG_H__

//#define DEBUG

#define PRINT_INFO(X...)   ({ printf(X);})

#define PRINT_WARNING(X...)						\
        ({									\
                printf("\033[33m%s:%s:%d:", __FILE__,__FUNCTION__,__LINE__);\
		printf(X);						\
		printf("\033[0m");					\
	})

#define PRINT_ERROR(X...)     						\
        ({									\
                printf("\033[31m%s:%s:%d:", __FILE__,__FUNCTION__,__LINE__);\
		printf(X);						\
		printf("\033[0m");					\
        })

#ifdef DEBUG
#define PRINT_DEBUG(X...)                                               \
        ({									\
	        printf("%s:%s:%d:", __FILE__,__FUNCTION__,__LINE__);	\
		printf(X);						\
        })
#else
#define PRINT_DEBUG(X...)
#endif

#endif
