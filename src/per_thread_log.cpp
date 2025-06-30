#include <per_thread_log.h>

#include <libgen.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include <map>

#define LOG_PATH_FORMAT "/tmp/%s/%016lx.log"

namespace aubin {

   void per_thread_log( const char * format... ) {
      // Formatage de la ligne de log
      va_list args;
      va_start( args, format );
      char user_msg[1000];
      vsprintf( user_msg, format, args );
      va_end( args );

      // Élaboration du chemin du fichier de log
      static std::map<pthread_t, FILE *> logs;
      static char * prog_name = NULL;
      if ( ! prog_name ) {
         char prog_path[PATH_MAX];
         if( readlink( "/proc/self/exe", prog_path, PATH_MAX ) > 0 ) {
            prog_name = strdup( basename( prog_path ));
         }
      }
      pthread_t                             this_thread   = pthread_self();
      std::map<pthread_t, FILE *>::iterator thread_log_it = logs.find( this_thread );
      FILE * log = NULL;
      if( thread_log_it == logs.end()) {
         char path[1000];
         sprintf( path, LOG_PATH_FORMAT, prog_name, this_thread );
         char * parent_dir = dirname( path );
         mkdir( parent_dir, 0777 );
         sprintf( path, LOG_PATH_FORMAT, prog_name, this_thread );
         log = fopen( path, "wt" );
         if( log ) {
            logs[this_thread] = log;
         }
         else {
            perror( path );
         }
      }
      else {
         log = thread_log_it->second;
      }

      // Ajout d'une ligne au fichier de log
      if( log ) {
         timespec now_nano;
         clock_gettime( CLOCK_REALTIME, &now_nano );
         time_t now;
         time( &now );
         char buf[sizeof( "2024-05-19 14:32:15" )];
         strftime( buf, sizeof( buf ), "%F %T", gmtime( &now ));
         char thread_name[1000];
         pthread_getname_np( this_thread, thread_name, sizeof( thread_name ));
         if(( 0 == strcmp( prog_name, thread_name ))&&( getpid() != gettid())) {
            thread_name[0] = '\0';
         }
         fprintf( log, "%s.%09ld;'%s';%s\n", buf, now_nano.tv_nsec, thread_name, user_msg );
         fflush( log );
      }
      else {
         fprintf( stderr, "%s:%d:'log' est nul !\n", __FILE__, __LINE__ );
      }
   }
}
