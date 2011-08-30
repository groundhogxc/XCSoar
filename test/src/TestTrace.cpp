/* Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2010 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Replay/IGCParser.hpp"
#include "IO/FileLineReader.hpp"
#include "Engine/Trace/Trace.hpp"
#include "Engine/Task/TaskStats/CommonStats.hpp"
#include "Engine/Navigation/Aircraft.hpp"
#include "Printing.hpp"
#include "TestUtil.hpp"

#include <windef.h>
#include <assert.h>
#include <cstdio>

static void
on_advance(Trace &trace,
           const GeoPoint &loc, const fixed speed,
           const Angle bearing, const fixed alt,
           const fixed baroalt, const fixed t)
{
  AircraftState new_state;
  new_state.location = loc;
  new_state.ground_speed = speed;
  new_state.altitude = alt;
  new_state.track = bearing;
  new_state.time = t;
  new_state.altitude_agl = alt;

  if (t>fixed_one) {
    trace.append(new_state);

    trace.optimise_if_old();

  }
// get the trace, just so it's included in timing
  TracePointVector v;
  trace.get_trace_points(v);
  if (trace.size()>1) {
//    assert(abs(v.size()-trace.size())<2);
  }
}

static bool
TestTrace(const char *filename, unsigned ntrace, bool output=false)
{
  FileLineReaderA reader(filename);
  if (reader.error()) {
    fprintf(stderr, "Failed to open %s\n", filename);
    return false;
  }

  printf("# %d", ntrace);  
  Trace trace(1000, ntrace);

  char *line;
  int i = 0;
  for (; (line = reader.read()) != NULL; i++) {
    if (output && (i % 500 == 0)) {
      putchar('.');
      fflush(stdout);
    }

    IGCFix fix;
    if (!IGCParseFix(line, fix))
      continue;

    on_advance(trace,
               fix.location, fixed(30), Angle::zero(),
               fix.gps_altitude, fix.pressure_altitude,
               fixed(fix.time.GetSecondOfDay()));
  }
  putchar('\n');
  printf("# samples %d\n", i);
  return true;
}


int main(int argc, char **argv)
{
  if (argc < 3) {
    unsigned n = 100;
    if (argc > 1) {
      n = atoi(argv[1]);
    }
    TestTrace("test/data/09kc3ov3.igc", n);
  } else {
    assert(argc >= 3);
    unsigned n = atoi(argv[2]);
    plan_tests(n);
    
    for (unsigned i=2; i<2+n; i++) {
      unsigned nt = pow(2,i);
      char buf[100];
      sprintf(buf," trace size %d", nt);
      ok(TestTrace(argv[1], nt),buf, 0);
    }
  }
  return 0;
}
