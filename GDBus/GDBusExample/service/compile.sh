#!/bin/bash
gcc -I/usr/include/gio-unix-2.0/ -I/usr/include/glib-2.0/ -I/usr/lib/aarch64-linux-gnu/glib-2.0/include ../gen/gen.c service.c -o server -lpthread -lgio-2.0 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0