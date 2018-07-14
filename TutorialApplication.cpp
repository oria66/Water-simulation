/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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

#include "TutorialApplication.h"
#define GRID 128
Hydrax::Noise::FFT::Options fftopt;
Hydrax::Module::SimpleGrid::Options moduleSGopt;
Hydrax::Module::ProjectedGrid::Options modulePGopt;
SkyX::CloudLayer::Options cloudSKopt;
SkyX::CloudLayer::Options cloudSKopt1;



//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}
//---------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
    //ogreCameraAnimationState->addTime(evt.timeSinceLastFrame);

    /*-----------------------------------------------------*/
    //Ogre::Vector3 lightDir = mSkyX->getAtmosphereManager()->getSunDirection();
    //Ogre::Vector3 lightDir = mSkyX->getController()->getSunDirection();


    // Calculate current color gradients point
    //float point = (-lightDir.y + 1.0f) / 2.0f;
  //  float point = (-lightDir.y) / 2.0f;
   // mHydrax->setWaterColor(mWaterGradient.getColor(point));

   // Ogre::Vector3 sunPos = mCamera->getDerivedPosition() - lightDir*mSkyX->getMeshManager()->getSkydomeRadius(mCamera)*0.1;
    //Ogre::Vector3 sunPos = mCamera->getDerivedPosition() - lightDir*mSkyX->getSceneManager()->getSkyDomeGenParameters().skyDomeDistance*0.1;
   // mHydrax->setSunPosition(sunPos);



   // Ogre::Light *Light0 = mSceneMgr->getLight("Light0"),
    //*Light1 = mSceneMgr->getLight("Light1");

   // Light0->setPosition(mCamera->getDerivedPosition() - lightDir*mSkyX->getMeshManager()->getSkydomeRadius(mCamera)*0.02);
    //Light0->setPosition(mCamera->getDerivedPosition() - lightDir*mSkyX->getSceneManager()->getSkyDomeGenParameters().skyDomeDistance*0.02);
   // Light1->setDirection(lightDir);

   // Ogre::Vector3 sunCol = mSunGradient.getColor(point);
   // Light0->setSpecularColour(sunCol.x, sunCol.y, sunCol.z);
   // Ogre::Vector3 ambientCol = mAmbientGradient.getColor(point);
   // Light0->setDiffuseColour(ambientCol.x, ambientCol.y, ambientCol.z);
   // mHydrax->setSunColor(sunCol);
    /*-----------------------------------------------------*/
   HydraxLOG(Ogre::StringConverter::toString(mHydrax->getHeigth(Ogre::Vector2(0,0))));
    mHydrax->update(evt.timeSinceLastFrame);
  //  mSkyX->update(evt.timeSinceLastFrame);
    return ret;
}
//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
//    // Create your scene here :)
//   // mLight0->setDiffuseColour(1, 1, 1);
//   // mLight0->setCastShadows(false);
//    // Color gradients
//    // Water
//    //mWaterGradient = SkyX::ColorGradient();
//    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.779105)*0.4, 1));
//    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.729105)*0.3, 0.8));
//    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.25, 0.6));
//    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.2, 0.5));
//    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.1, 0.45));
//    mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.025, 0));
//    // Sun
//    //mSunGradient = SkyX::ColorGradient();
//    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.5, 1.0f));
//    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.4, 0.75f));
//    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.3, 0.5625f));
//    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.6,0.5,0.2)*1.5, 0.5f));
//    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.5,0.5,0.5)*0.25, 0.45f));
//    mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.5,0.5,0.5)*0.01, 0.0f));
//    // Ambient
//    //mAmbientGradient = SkyX::ColorGradient();
//    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*1, 1.0f));
//    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*1, 0.6f));
//    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.6, 0.5f));
//    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.3, 0.45f));
//    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.1, 0.35f));
//    mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.05, 0.0f));
    //mCamera->setPosition(Ogre::Vector3(1683, 50, 2116)); // направляем камеру
        //mCamera->lookAt(Ogre::Vector3(0, 0, 0));

        //// Create Hydrax object
       mHydrax = new Hydrax::Hydrax(mSceneMgr, mCamera, mWindow->getViewport(0));

        //// Use CG shaders for Hydrax
        mHydrax->setShaderMode(Hydrax::MaterialManager::SM_CG);

        //// Configure main options
        mHydrax->setPosition(Ogre::Vector3::ZERO);
        mHydrax->setPlanesError(0.5);
        mHydrax->setFullReflectionDistance(1e+011f);
        mHydrax->setGlobalTransparency(0.0f);
        mHydrax->setNormalDistortion(0.075f);

      //  mHydrax->setWaterColor(Ogre::Vector3(0.14f, 0.36f, 0.42f));

        fftopt.PhysicalResolution=GRID;
        fftopt.Resolution=GRID;
        fftopt.WindDirection=Ogre::Vector2(10,0);
        fftopt.Amplitude=0.00348;
        fftopt.Depth=200;
        fftopt.Fetch=400*1000;
        fftopt.Winds=15;
        fftopt.Spectrum=2;

        moduleSGopt.ChoppyWaves=true;
        moduleSGopt.Complexity=GRID;
        moduleSGopt.MeshSize=GRID;
        moduleSGopt.Smooth=true;

        modulePGopt.ChoppyWaves=true;
        modulePGopt.Complexity=GRID;
        modulePGopt.Smooth=true;
        modulePGopt.ForceRecalculateGeometry=true;

        // Create our projected grid module
        Hydrax::Module::ProjectedGrid *mModulePG
                = new Hydrax::Module::ProjectedGrid(// Hydrax parent pointer
                                                    mHydrax,
                                                    // Noise module
                                                    new Hydrax::Noise::Perlin(/*Generic one*/),
                                                    // Base plane
                                                    Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),
                                                    // Normal mode
                                                    Hydrax::MaterialManager::NM_VERTEX,
                                                    // Projected grid options
                                                    modulePGopt);
        Hydrax::Module::SimpleGrid *mModuleSG
                = new Hydrax::Module::SimpleGrid(mHydrax,new Hydrax::Noise::FFT(fftopt),Hydrax::MaterialManager::NM_VERTEX,moduleSGopt);


        // Set our module
        mHydrax->setModule(mModuleSG,true);


        //// Component configuration.
     //   // You can comment each of following blocks.
     //   // You can also load all those parameters from a config file.

     //   // Configure sun component.
        mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_SUN));
        mHydrax->setSunPosition(Ogre::Vector3(0.0f, 100.0f, 0.0f));
        mHydrax->setSunStrength(1.75f);
        mHydrax->setSunArea(150.0f);
        mHydrax->setSunColor(Ogre::Vector3(1.0f, 0.9f, 0.6f));

     //   // Create a light, same parameters as sun component
        Ogre::Light* ogreLight = mSceneMgr->createLight("Light");
        ogreLight->setPosition(Ogre::Vector3(0.0f, 100.0f, 0.0f));
        ogreLight->setDiffuseColour(1, 1, 1);
        ogreLight->setSpecularColour(1.0f, 0.9f, 0.6f);

     //   // Configure foam component.
        mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_FOAM));
        mHydrax->setFoamMaxDistance(75000000.0f);
        mHydrax->setFoamScale(0.0075f);
        mHydrax->setFoamStart(0.0f);
        mHydrax->setFoamTransparency(1.0f);

        //// Configure depth component.
        mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_DEPTH));
        mHydrax->setDepthLimit(90.0f);

        //// Configure smooth component.
        mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_SMOOTH));
        mHydrax->setSmoothPower(5.0f);

        //// Configure caustics component.
        mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_CAUSTICS));
        mHydrax->setCausticsScale(135.0f);
        mHydrax->setCausticsPower(10.5f);
        mHydrax->setCausticsEnd(0.8f);

        //// Configure underwater rendering
        mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_UNDERWATER));
     //
        //// Configure underwater rendering
        mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_UNDERWATER_REFLECTIONS));

        //// Configure god rays component.
        mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_UNDERWATER_GODRAYS));
        mHydrax->setGodRaysExposure(Ogre::Vector3(0.76f, 2.46f, 2.29f));
        mHydrax->setGodRaysIntensity(0.015f);
        mHydrax->getGodRaysManager()->setSimulationSpeed(5.0f);
        mHydrax->getGodRaysManager()->setNumberOfRays(100);
        mHydrax->getGodRaysManager()->setRaysSize(0.03f);
        mHydrax->getGodRaysManager()->setObjectIntersectionsEnabled(false);

        //// Configure hydrax rtt automatically.

     // mHydrax->getRttManager()->setTextureSize(Hydrax::RttManager::RTT_REFLECTION, Hydrax::Size(0, 0));
      // mHydrax->getRttManager()->setTextureSize(Hydrax::RttManager::RTT_REFRACTION, Hydrax::Size(0, 0));
      //  mHydrax->getRttManager()->setTextureSize(Hydrax::RttManager::RTT_DEPTH, Hydrax::Size(0, 0));
      //  mHydrax->getRttManager()->setTextureSize(Hydrax::RttManager::RTT_DEPTH_REFLECTION, Hydrax::Size(0, 0));
      //  mHydrax->getRttManager()->setTextureSize(Hydrax::RttManager::RTT_GPU_NORMAL_MAP, Hydrax::Size(0, 0));

        // mBasicController = new SkyX::BasicController();
        // mSkyX = new SkyX::SkyX(mSceneMgr, mBasicController);
        // mSkyX->create();
        // mSkyX->getVCloudsManager()->getVClouds()->setDistanceFallingParams(Ogre::Vector2(2,-1));
         //mSkyX->getMeshManager()->setGeometryParameters(2,2);
         //mRoot->addFrameListener(mSkyX);
         //mWindow->addListener(mSkyX);
         //mCamera->setFarClipDistance(1000);
        // mCamera->setAutoAspectRatio(true);
        // cloudSKopt.Height=1000;
         //cloudSKopt.HeightVolume=1000;
        // mSkyX->getCloudsManager()->add(cloudSKopt);
        // cloudSKopt1.Height=900;
        // mSkyX->getCloudsManager()->add(cloudSKopt1);




        //// Everything configured, create.
        mHydrax->create();

        //// Add a cloudy sky
        //mSceneMgr->setSkyDome(true, "Examples/CloudySky");

        //// Add a beach (or kind of...)
        Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -200);
        Ogre::MeshManager::getSingleton().createPlane("beachPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, GRID, GRID, GRID/4, GRID/4, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
        Ogre::Entity* ogreBeach = mSceneMgr->createEntity("beach", "beachPlane");
        ogreBeach->setMaterialName("Examples/BeachStones");
        Ogre::SceneNode* ogreBeachNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        ogreBeachNode->attachObject(ogreBeach);

        ////// Add the depth technique to our material
        mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Examples/BeachStones"))->createTechnique());
       mCamera->setNearClipDistance(0.01f) ;
        mCamera->setFarClipDistance(99999);

        // set shadow properties
//        mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
//        mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
//        mSceneMgr->setShadowTextureSize(1024);
//        mSceneMgr->setShadowTextureCount(1);

}
//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
