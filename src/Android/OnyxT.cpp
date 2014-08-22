/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2014 The XCSoar Project
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


This file is implementing some undocumented function calls of the Onyx Boox Android implementation
in order to switch the A2 mode on and off. A workaround for the spurious dither problem is
implemented as well.


*/

#include "Android/OnyxT.hpp"
#include "LogFile.hpp"
#include "Version.hpp"
#include "tchar.h"


static JNIEnv      *env;
static NativeView  *nativeView;
static jmethodID    setWaveformAndScheme_method;
static jmethodID    resetWaveformAndScheme_method;


/**
 * Initialize Onyx-Boox-specific functionality
 */
bool
OnyxT::Initialize(NativeView *_nativeView, JNIEnv *_env)
{
  nativeView=_nativeView;
  env=_env;
  Java::Class vcls(env, "org/xcsoar/NativeView");

  /* TODO: There is a potential problem here: If the API should change at some point,
   * and the methods are no longer available, the Program will crash. That is because
   * (setWaveformAndScheme_method!=nullptr) checking is not enough: After an unsuccsessful call to
   * GetStaticMethodID(), the JVM will crash after a while with "Attempt to access stale reference".
   * Has something to do with the garbage collector, possibly a GlobalRef to the Class is required -
   * But I do not see why.
   */
  setWaveformAndScheme_method = env->GetStaticMethodID(vcls, "setWaveformAndScheme", "(III)V");
  resetWaveformAndScheme_method = env->GetStaticMethodID(vcls, "resetWaveformAndScheme", "()V");

  if(setWaveformAndScheme_method!=nullptr){
    return true;
  } else {
    LogFormat("Initialization of Onyx-T specific functions failed");
    return false;
  }
}

/**
 * Switch on Fast Screen Mode for the Onyx T
 *
 * As workaround to the unreliable dither problem, switches both dither and inversion flag on
 * (Dither only would be 0xd01)
 *
 * Two separate methods are defined in analogy to Nook, for a future unification to a common epaper screen interface.
 */
bool
OnyxT::EnterFastMode()
{
  return setWaveformAndScheme(0xb01);
}

/**
 * Revert to normal Screen Mode for the Onyx T series
 */
bool
OnyxT::ExitFastMode()
{
  return setWaveformAndScheme(0x5);
}

bool
OnyxT::setWaveformAndScheme(int updateMode)
{
  if(setWaveformAndScheme_method!=nullptr){
    LogFormat(_T("Set WaveformAndScheme %d"), updateMode);
    Java::Class vcls(env, "org/xcsoar/NativeView");
    env->CallStaticVoidMethod(vcls, setWaveformAndScheme_method, updateMode,-1,0x7fffffff);
    return true;
  } else
    return false;
}
