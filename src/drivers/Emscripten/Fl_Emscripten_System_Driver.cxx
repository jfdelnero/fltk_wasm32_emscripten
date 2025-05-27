//
// Implementation of Emscripten system driver used for the web.
//
// Copyright 1998-2024 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

#include "Fl_Emscripten_System_Driver.H"
#include <emscripten.h>

Fl_Emscripten_System_Driver::Fl_Emscripten_System_Driver()
  : Fl_System_Driver() {}

double Fl_Emscripten_System_Driver::wait(double v) {
  static unsigned int wait_depth = 0;
  wait_depth++;

  double ret = Fl_System_Driver::wait(v);

  if (wait_depth == 1) {
    emscripten_sleep(0);
    Fl::flush();
  }

  wait_depth--;
  return ret;
}

void Fl_Emscripten_System_Driver::gettime(time_t *sec, int *usec) {
  time_t s;
  int us;
  double now = emscripten_get_now(); // In ms.

  // seconds rounded by truncation
  s = (time_t)(now / 1000.0);

  // get the remaining us.  
  us = (int)( ( now - ( ((double)s) * 1000.0 ) ) * 1000.0 ); 

  *sec = s;
  *usec = us;
}

char *Fl_Emscripten_System_Driver::strdup(const char *s) {
  return ::strdup(s);
}
