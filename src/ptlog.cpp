#include <ptlog.h>

#include <libgen.h>
#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <map>

namespace aubin {

   void per_thread_log( const char * format... ) {
      static std::map<pthread_t, FILE *> logs;
      va_list args;
      va_start( args, format );
      char buffer1[1000];
      vsprintf( buffer1, format, args );
      va_end( args );
      char name[40];
      pthread_t this_thread = pthread_self();
      pthread_getname_np( this_thread, name, sizeof( name ));
      char buffer2[1040];
      sprintf( buffer2, "%s;%s", name, buffer1 );
      std::map<pthread_t, FILE *>::iterator it = logs.find( this_thread );
      FILE * log = NULL;
      if( it == logs.end()) {
         char path[1000];
         sprintf( path, "/tmp/%s-%04lu.log", name, this_thread );
         char * parent_dir = dirname( path );
         mkdir( parent_dir, 0777 );
         sprintf( path, "/tmp/%s-%04lu.log", name, this_thread );
         log = fopen( path, "wt" );
         if( log ) {
            logs[this_thread] = log;
         }
         else {
            perror( path );
         }
      }
      else {
         log = it->second;
      }
      if( log ) {
         timespec now_nano;
         clock_gettime( CLOCK_REALTIME, &now_nano );
         time_t now;
         time( &now );
         char buf[sizeof( "2024-05-19 14:32:15" )];
         strftime( buf, sizeof( buf ), "%F %T", gmtime( &now ));
         fprintf( log, "%s.%09ld;%s\n", buf, now_nano.tv_nsec, buffer2 );
         fflush( log );
      }
      else {
         fprintf( stderr, "%s:%d:'log' est nul !\n", __FILE__, __LINE__ );
      }
   }
}
