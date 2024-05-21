#include <ptlog.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MS_TO_NANO 1000000

static void wait( unsigned ms ) {
   const timespec p = { 0, ms * MS_TO_NANO };
   aubin::per_thread_log( "wait( %u )|before", ms );
   nanosleep( &p, NULL );
   aubin::per_thread_log( "wait( %u )|after", ms );
}

void * thread( void * ) {
   for( unsigned i = 0; i < 100; ++i ) {
      u_int64_t delay_ms = 200ULL;
      delay_ms *= rand();
      delay_ms /= RAND_MAX;
      delay_ms += 20;
      wait((unsigned)delay_ms );
   }
   return NULL;
}

int main( void ) {
   pthread_t premier;
   pthread_create( &premier  , NULL, thread, NULL );
   pthread_setname_np( premier, "premier" );
   pthread_t second;
   pthread_create( &second   , NULL, thread, NULL );
   pthread_t troisieme;
   pthread_create( &troisieme, NULL, thread, NULL );
   pthread_t quatrieme;
   pthread_create( &quatrieme, NULL, thread, NULL );
   pthread_setname_np( quatrieme, "quatrieme" );
   void * ignored_result = NULL;
   aubin::per_thread_log( "main.join( premier )" );
   pthread_join( premier  , &ignored_result );
   aubin::per_thread_log( "main.join( second )" );
   pthread_join( second   , &ignored_result );
   aubin::per_thread_log( "main.join( troisieme )" );
   pthread_join( troisieme, &ignored_result );
   aubin::per_thread_log( "main.join( quatrieme )" );
   pthread_join( quatrieme, &ignored_result );
   aubin::per_thread_log( "All threads done, exiting." );
   return 0;
}
