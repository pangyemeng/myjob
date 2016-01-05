#ifndef   _MACRO_LogModule
#define   _MACRO_LogModule

#include <stdio.h>
#include <pthread.h>
#include "macro_define.h"

#define   _LOG_BUFFSIZE  1024*1024*4
#define   _SYS_BUFFSIZE  1024*1024*8
#define	  _LOG_PATH_LEN  250
#define   _LOG_MODULE_LEN 32

typedef  enum LogLevel {  
	LL_DEBUG = 1,
	LL_TRACE = 2,
	LL_NOTICE = 3, 
	LL_WARNING = 4, 
	LL_ERROR = 5,
}LogLevel;

class Log_Writer
{
	public:
		Log_Writer()
		{
			m_system_level = LL_NOTICE;
			//fp = stderr;
			fp = NULL;
			m_issync = false;
			m_isappend = true;
			m_filelocation[0] ='\0'; 
			pthread_mutex_init(&m_mutex, NULL);
		}
		~Log_Writer(){
			logclose();
		}
		bool loginit(LogLevel l, const  char *filelocation, bool append = true, bool issync = false);
		bool log(LogLevel l,char *logformat,...);
		LogLevel get_level();
		bool logclose();
	private:
		const char* logLevelToString(LogLevel l);
		bool checklevel(LogLevel l);
		int premakestr(char* m_buffer, LogLevel l);
		bool _write(char *_pbuffer, int len);
	private:
		enum LogLevel m_system_level;
		FILE* fp;
		bool m_issync;
		bool m_isappend;
		char m_filelocation[_LOG_PATH_LEN];
		pthread_mutex_t m_mutex;
		static __thread char m_buffer[_LOG_BUFFSIZE];
};

extern Log_Writer WARN_W;
extern Log_Writer INFO_W;

bool log_init(LogLevel l, const char* p_modulename, const char* p_logdir);

#endif
