# Multi-threaded log utility

Each thread logs into its own file, under `/tmp/<prgname>/<thread id>.log`.

Language : C++

Function : `aubin::per_thread_log( <format>, <data 1>, <data 2> ... <data n> );`
