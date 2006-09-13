#ifndef COIN_SOGUICLICKCOUNTER_H
#define COIN_SOGUICLICKCOUNTER_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifndef SOWIN_INTERNAL
#error this is a private header file
#endif /* !SOWIN_INTERNAL */

#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFVec3f.h>

#include <Inventor/nodekits/SoBaseKit.h>

class SbColor;

class SoGuiClickCounter : public SoBaseKit {
  typedef SoBaseKit inherited;

  SO_KIT_HEADER(SoGuiClickCounter);

  SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
  SO_KIT_CATALOG_ENTRY_HEADER(surfaceMaterial);
  SO_KIT_CATALOG_ENTRY_HEADER(surfaceTexture);
  SO_KIT_CATALOG_ENTRY_HEADER(surfaceTexCoords);
  SO_KIT_CATALOG_ENTRY_HEADER(surfaceCoords);
  SO_KIT_CATALOG_ENTRY_HEADER(surfaceFaceSet);

public:
  static void initClass(void);

  SoGuiClickCounter(void);

  SoSFVec3f size;

  SoSFInt32 value;
  SoSFTrigger click;

  SoSFInt32 first;
  SoSFInt32 last;

  virtual void handleEvent(SoHandleEventAction * action);

  virtual void setSurfaceColor(const SbColor & value);

  void sizeUpdate(void);
  void firstUpdate(void);

protected:
  virtual ~SoGuiClickCounter(void);

  virtual SbBool readInstance(SoInput * in, unsigned short flags);

private:
  void * internals;

};

#endif // !COIN_SOGUICLICKCOUNTER_H
