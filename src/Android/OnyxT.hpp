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
*/

#ifndef XCSOAR_ANDROID_ONYXT_HPP
#define XCSOAR_ANDROID_ONYXT_HPP

#include "Java/Object.hxx"
#include "Java/Class.hxx"
#include "Java/Global.hxx"
#include "NativeView.hpp"

#include <assert.h>



namespace OnyxT {

  /**
   * Initialize Onyx T series specific functions
   *
   * @return false if the operation has failed, e.g. because the
   * (undocumented) API methods to be called have not been found in the usual place
   */

  bool Initialize(NativeView *_nativeView, JNIEnv *_env);
  /**
   * Enter FastMode to eliminate full refresh of screen
   *
   * @return false if the operation has failed, e.g. because the
   * (undocumented) API method was not initialized
   */
  bool EnterFastMode();

  /**
   * Exit FastMode to restore full (slow) refresh of screen
   */
  bool ExitFastMode();

  /**
   * Set update mode of the eInk screen of Onyx Boox devices
   */
  bool setWaveformAndScheme(int updateMode);
}

#endif
