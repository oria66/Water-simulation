/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"

#include<SKYX/SkyX.h>

#include "Hydrax.h"
#include "Perlin.h"
#include "FFT.h"
#include "ProjectedGrid.h"
#include "SimpleGrid.h"
//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

protected:
    virtual void createScene(void);
  virtual  bool frameRenderingQueued(const Ogre::FrameEvent& evt);

        SkyX::SkyX *mSkyX;
        SkyX::BasicController *mBasicController;
        // Color gradients
        //SkyX::ColorGradient mWaterGradient,mSunGradient,mAmbientGradient;
   Hydrax::Hydrax *mHydrax;
   // Ogre::AnimationState* ogreCameraAnimationState;
   // Light
   //Ogre::Light *mLight0 = mSceneMgr->createLight("Light0");

};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
