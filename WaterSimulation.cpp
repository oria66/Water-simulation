#include "WaterSimulation.h"

#include "Hydrax.h"
#include "SimpleGrid.h"
#include "Module.h"
#include "Noise.h"
#include "FFT.h"

using namespace std;
using namespace Ogre;
// Hydrax pointer
//#define Grid 8
//#define Grid 32
//#define Grid 64
//#define Grid 128
#define Grid 256
//#define Grid 512
Hydrax::Hydrax* mHydrax = 0;
Ogre::Vector3 pos(0,0,0);
Ogre::TextAreaOverlayElement* textArea=0;
Hydrax::Module::SimpleGrid::Options opt;
Hydrax::Noise::FFT::Options fftOptions;
int flag=0;
Hydrax::Module::SimpleGrid *mModule=0;
//---------------------------------------------------------------------------
WaterSimulation::WaterSimulation(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mDetailsPanel(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
    mOverlaySystem(0)


{

    m_ResourcePath = "";

    prev_rot = 0;
}

//---------------------------------------------------------------------------
WaterSimulation::~WaterSimulation(void)
{
    if (mTrayMgr) delete mTrayMgr;
    if (mCameraMan) delete mCameraMan;
    if (mOverlaySystem) delete mOverlaySystem;

    // Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//---------------------------------------------------------------------------
bool WaterSimulation::configure(void)
{
    // Show the configuration dialog and initialise the system.
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg.
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise.
        // Here we choose to let the system create a default rendering window by passing 'true'.
        mWindow = mRoot->initialise(true, "Water Simulation");

        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
void WaterSimulation::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    // Initialize the OverlaySystem (changed for Ogre 1.9)
    mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);
}
//---------------------------------------------------------------------------
void WaterSimulation::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    mCamera->setFarClipDistance(99999);
    mCamera->setNearClipDistance(0.1);

    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // Create a default camera controller
}
//---------------------------------------------------------------------------
void WaterSimulation::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    // Set initial mouse clipping size
    windowResized(mWindow);

    // Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showBackdrop("SdkTrays/Bands");
    mTrayMgr->showCursor();
         // Create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();
   // Ogre::StringVector item;
    //item.push_back("Elevacion de la nave");
  // mDetailsPanel1 = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel1", 260, item);
  // mDetailsPanel1->setParamValue(0, Ogre::StringConverter::toString(mHydrax->getHeigth(pos)));
  // mDetailsPanel1->hide();
    //Button
    mButton =mTrayMgr->createButton(OgreBites::TL_CENTER,"Button","Run",75);
    mButton->show();
    //Slider
    mSlider=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider","Spectrum",256,80,0,3,4);
    mSlider->show();
    //
    mSlider1=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider1","WindSpeed",256,80,0,25,25);
    mSlider1->show();
    //
    mSlider2=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider2","Fecth",256,80,0,100000,1000);
    mSlider2->show();
    //
    mSlider3=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider3","Depth",256,80,0,500,100);
    mSlider3->show();
    //ChekBox
    mCheckBox=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMRIGHT,"ChekBox","Phillips",110);
    mCheckBox->show();
    //
    mCheckBox0=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMRIGHT,"ChekBox0","  P-M  ",110);
    mCheckBox0->show();
    //
    mCheckBox1=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMRIGHT,"ChekBox1","JONSWAP",110);
    mCheckBox1->show();
    //
    mCheckBox2=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMRIGHT,"ChekBox2","  TMA  ",110);
    mCheckBox2->show();
    mRoot->addFrameListener(this);
}
//---------------------------------------------------------------------------
void WaterSimulation::createScene(void)
{          // set background and some fog

 // mViewport->setBackgroundColour(ColourValue(1.0f, 1.0f, 0.8f));
  //mSceneMgr->setFog(Ogre::FOG_LINEAR, ColourValue(1.0f, 1.0f, 1.0f));



  // disable default camera control so the character can do its own


  // use a small amount of ambient lighting
    //mSceneMgr->setAmbientLight(ColourValue(1, 1, 1));

    Ogre::Light *mLight0 = mSceneMgr->createLight("Light0");
    mLight0->setDiffuseColour(1, 1, 1);
    mLight0->setCastShadows(false);

    // Shadow caster
    Ogre::Light *mLight1 = mSceneMgr->createLight("Light1");
    mLight1->setType(Ogre::Light::LT_DIRECTIONAL);

        // Create Hydrax object
        mHydrax = new Hydrax::Hydrax(mSceneMgr, mCamera, mWindow->getViewport(0));

        // Use CG shaders for Hydrax
        mHydrax->setShaderMode(Hydrax::MaterialManager::SM_CG);
//mHydrax->setWaterColor(Ogre::Vector3(53.0/255.0,67.0/255.0,94.0/255.0));

        // Create hydrax geometry, simple grid
        Hydrax::Module::SimpleGrid::Options hydraxModuleOptions;
        hydraxModuleOptions.Complexity=Grid;
                hydraxModuleOptions.MeshSize=Grid;
                        fftOptions.Resolution=Grid;
                        fftOptions.PhysicalResolution=Grid;

        fftOptions.Spectrum=0;




           mModule  = new Hydrax::Module::SimpleGrid(mHydrax,new Hydrax::Noise::FFT(fftOptions), Hydrax::MaterialManager::NM_VERTEX,hydraxModuleOptions);
        // Tell Hydrax to use this geometry module.
        mHydrax->setModule(mModule, true);

        // Configure sun component.
         //mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_SUN));
          // mHydrax->setSunPosition(Ogre::Vector3(0.0f, 1000.0f, 0.0f));
          // mHydrax->setSunStrength(1.75f);
         // mHydrax->setSunArea(150.0f);
         //  mHydrax->setSunColor(Ogre::Vector3(1.0f, 0.9f, 0.6f));

        /*   // Create a light, same parameters as sun component
           Ogre::Light* ogreLight = mSceneMgr->createLight("Light");
           ogreLight->setPosition(Ogre::Vector3(0.0f, 100.0f, 0.0f));
           ogreLight->setDiffuseColour(1, 1, 1);
           ogreLight->setSpecularColour(1.0f, 0.9f, 0.6f);*/

        //   // Configure foam component.
      //    mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_FOAM));
        //  mHydrax->setFoamMaxDistance(750.0f);
        //  mHydrax->setFoamScale(1.075f);
       //   mHydrax->setFoamStart(1.0f);
      //    mHydrax->setFoamTransparency(1.0f);

           // Configure depth component.
         //  mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_DEPTH));
        //   mHydrax->setDepthLimit(1.0f);


        mHydrax->create();



        // Add a cloudy sky
      //  mSceneMgr->setSkyBox(true, "Sky/Clouds");

        // set shadow properties
       mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE);
        mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
        mSceneMgr->setShadowTextureSize(1024);
        mSceneMgr->setShadowTextureCount(1);



        // Add a beach (or kind of...)
            Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 1);
           Ogre::MeshManager::getSingleton().createPlane("beachPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 256, 256, 128, 128, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
            Ogre::Entity* ogreBeach = mSceneMgr->createEntity("beach", "beachPlane");
           ogreBeach->setMaterialName("Examples/BeachStones");
            Ogre::SceneNode* ogreBeachNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
           ogreBeachNode->setPosition(0,-10,0);


            // Add the depth technique to our material
                mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Examples/BeachStones"))->createTechnique());

                mBasicController = new SkyX::BasicController();
                           mSkyX = new SkyX::SkyX(mSceneMgr, mBasicController);
                           mSkyX->create();
                           mSkyX->getVCloudsManager()->getVClouds()->setDistanceFallingParams(Ogre::Vector2(2,-1));
                           mRoot->addFrameListener(mSkyX);
                           mWindow->addListener(mSkyX);
                           mCamera->setFarClipDistance(500000);
                           mCamera->setAutoAspectRatio(true);
                           //Ogre::Vector3 lightDir = mSkyX->getController()->getSunDirection();
                          // o.Scale=2;
                           //mSkyX->getCloudsManager()->add(o);
                           //mSkyX->setLightingMode(1);
                          // o1.InnerRadius=1;
                           mSkyX->getAtmosphereManager()->setOptions(SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 0.5, 4));
                           //mBasicController->setMoonPhase(0.75f);
                           //Ogre::Vector3 lightDir = mSkyX->getAtmosphereManager()->getSunDirection();
                           Ogre::Vector3 lightDir = mSkyX->getController()->getSunDirection();


                           // Calculate current color gradients point
                           //float point = (-lightDir.y + 1.0f) / 2.0f;
                           float point = (-lightDir.y+1) / 2.0f;
                           mHydrax->setWaterColor(mWaterGradient.getColor(point));

                          // Ogre::Vector3 sunPos = mCamera->getDerivedPosition() - lightDir*mSkyX->getMeshManager()->getSkydomeRadius(mCamera)*0.1;
                           Ogre::Vector3 sunPos = mCamera->getDerivedPosition() - lightDir*mSkyX->getSceneManager()->getSkyDomeGenParameters().skyDomeDistance*0.1;
                           mHydrax->setSunPosition(sunPos);



                           Ogre::Light *Light0 = mSceneMgr->getLight("Light0"),
                           *Light1 = mSceneMgr->getLight("Light1");

                           Light0->setPosition(mCamera->getDerivedPosition() - lightDir*mSkyX->getMeshManager()->getSkydomeRadius(mCamera)*0.02);
                           //Light0->setPosition(mCamera->getDerivedPosition() - lightDir*mSkyX->getSceneManager()->getSkyDomeGenParameters().skyDomeDistance*0.02);
                           Light1->setDirection(lightDir);

                           Ogre::Vector3 sunCol = mSunGradient.getColor(point);
                           Light0->setSpecularColour(sunCol.x, sunCol.y, sunCol.z);
                           Ogre::Vector3 ambientCol = mAmbientGradient.getColor(point);
                           Light0->setDiffuseColour(ambientCol.x, ambientCol.y, ambientCol.z);
                          // mHydrax->setSunColor(sunCol);

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void WaterSimulation::destroyScene(void)
{
   mHydrax->remove();
}
//---------------------------------------------------------------------------
void WaterSimulation::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//---------------------------------------------------------------------------
void WaterSimulation::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//---------------------------------------------------------------------------
void WaterSimulation::createResourceListener(void)
{
}
//---------------------------------------------------------------------------
void WaterSimulation::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//---------------------------------------------------------------------------
void WaterSimulation::go(void)
{
#ifdef _DEBUG
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources_d.cfg";
    mPluginsCfg = m_ResourcePath + "plugins_d.cfg";
#else
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#endif
#else
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources.cfg";
    mPluginsCfg = m_ResourcePath + "plugins.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
#endif

    if (!setup())
            return;

        mRoot->startRendering();

    // Clean up
   destroyScene();
}
//---------------------------------------------------------------------------
bool WaterSimulation::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
    createScene();


    createFrameListener();

    return true;
}
//---------------------------------------------------------------------------
bool WaterSimulation::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

  //  pos=bodyScene->getPosition();
  //   bodyScene->setPosition(pos.x,mHydrax->getHeigth(pos),pos.z);



    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // If dialog isn't up, then update the camera
        if(mButton->getState()==OgreBites::BS_DOWN){
            mTrayMgr->hideBackdrop();
            mTrayMgr->adjustTrays();
         //   mTrayMgr->moveWidgetToTray(mDetailsPanel1, OgreBites::TL_TOPLEFT, 0);
            // Position it at 500 in Z direction
            mCamera->setPosition(Ogre::Vector3(-81.6,68,117.7));
            // Look back along -Z
            mCamera->lookAt(Ogre::Vector3(-81.6,68,117.7));
            mCamera->setOrientation(Ogre::Quaternion(0.9,-0.21,-0.29,-0.065));
            mCameraMan->setStyle(OgreBites::CS_ORBIT);
           // mDetailsPanel1->show();
            if(mButton->isVisible()){
                mTrayMgr->removeWidgetFromTray("Button");
               // mButton->hide();
            }
            mTrayMgr->refreshCursor();

            mTrayMgr->moveWidgetToTray(mButton, OgreBites::TL_BOTTOM, -1);
          //mCameraMan->setStyle(OgreBites::CS_ORBIT);

        }
        if (mDetailsPanel->isVisible())          // If details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
//        if (mDetailsPanel1->isVisible()){
//        //mDetailsPanel1->setParamValue(0, Ogre::StringConverter::toString(mHydrax->getHeigth(pos)));
//    }


        if(mSlider->isVisible()){
             if(mSlider->getValue()==0){
                 if (flag==1) {
                 fftOptions.Spectrum=0;
                  mHydrax->~Hydrax();
                 /*-----------------------------*/
                 mHydrax = new Hydrax::Hydrax(mSceneMgr, mCamera, mWindow->getViewport(0));
                 // Use CG shaders for Hydrax
                 mHydrax->setShaderMode(Hydrax::MaterialManager::SM_CG);
                 // Create hydrax geometry, simple grid
                 Hydrax::Module::SimpleGrid::Options hydraxModuleOptions;
                 hydraxModuleOptions.Complexity=Grid;
                 hydraxModuleOptions.MeshSize=Grid;
                 mModule  = new Hydrax::Module::SimpleGrid(mHydrax,new Hydrax::Noise::FFT(fftOptions), Hydrax::MaterialManager::NM_VERTEX,hydraxModuleOptions);
                 // Tell Hydrax to use this geometry module.
                 mHydrax->setModule(mModule, true);

                  /*------------------------------*/
                 mHydrax->create();

                      flag=0;}}

            if(mSlider->getValue()==1){
                if(flag==0||flag==2){
                    fftOptions.Spectrum=1;
                    fftOptions.Amplitude=0.0081;
                    fftOptions.WindDirection=Ogre::Vector2(10,0);
                    fftOptions.Scale=1.0;
                    fftOptions.Resolution=fftOptions.PhysicalResolution=Grid;
                    mHydrax->~Hydrax();
                    /*-----------------------------*/
                    mHydrax = new Hydrax::Hydrax(mSceneMgr, mCamera, mWindow->getViewport(0));
                    // Use CG shaders for Hydrax
                    mHydrax->setShaderMode(Hydrax::MaterialManager::SM_CG);
                    // Create hydrax geometry, simple grid
                    mHydrax->setWaterColor(Ogre::Vector3(53.0/255.0,67.0/255.0,94.0/255.0));
                    Hydrax::Module::SimpleGrid::Options hydraxModuleOptions;
                    hydraxModuleOptions.Complexity=Grid;
                    hydraxModuleOptions.MeshSize=Grid;
                    hydraxModuleOptions.ChoppyWaves=true;
                    mModule  = new Hydrax::Module::SimpleGrid(mHydrax,new Hydrax::Noise::FFT(fftOptions), Hydrax::MaterialManager::NM_VERTEX,hydraxModuleOptions);
                    // Tell Hydrax to use this geometry module.
                    mHydrax->setModule(mModule, true);
                    //                 // Configure sun component.
                   mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_SUN));
                    mHydrax->setSunPosition(Ogre::Vector3(0.0f, 1000.0f, 0.0f));
                    mHydrax->setSunStrength(1.75f);
                    mHydrax->setSunArea(150.0f);
                    //                mHydrax->setSunColor(Ogre::Vector3(1.0f, 0.9f, 0.6f));
                    //                 //   // Configure foam component.
                    mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_FOAM));
                    mHydrax->setFoamMaxDistance(750.0f);
                    mHydrax->setFoamScale(1.075f);
                    mHydrax->setFoamStart(1.0f);
                    mHydrax->setFoamTransparency(1.0f);
                    //                 // Configure depth component.
                   mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_DEPTH));
                   mHydrax->setDepthLimit(10.0f);
//                    //                  /*------------------------------*/
                    mHydrax->create();
                    // Add the depth technique to our material
           //         mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Examples/BeachStones"))->createTechnique());


                 flag=1;
                }
            }
            if((mSlider->getValue()==2)){
                if(flag==1||flag==3){
                fftOptions.Spectrum=2;
                fftOptions.Fetch=50000;
                fftOptions.Winds=2;
                fftOptions.WindDirection=Ogre::Vector2(10,0);
                fftOptions.Resolution=fftOptions.PhysicalResolution=Grid;
                mHydrax->~Hydrax();
                /*-----------------------------*/
                mHydrax = new Hydrax::Hydrax(mSceneMgr, mCamera, mWindow->getViewport(0));
                // Use CG shaders for Hydrax
                mHydrax->setShaderMode(Hydrax::MaterialManager::SM_CG);
                // Create hydrax geometry, simple grid
                Hydrax::Module::SimpleGrid::Options hydraxModuleOptions;
                hydraxModuleOptions.Complexity=Grid;
                hydraxModuleOptions.MeshSize=Grid;
                //hydraxModuleOptions.ChoppyWaves=true;
                mModule  = new Hydrax::Module::SimpleGrid(mHydrax,new Hydrax::Noise::FFT(fftOptions), Hydrax::MaterialManager::NM_VERTEX,hydraxModuleOptions);
                // Tell Hydrax to use this geometry module.
                mHydrax->setModule(mModule, true);
//                // Configure sun component.
              mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_SUN));
               mHydrax->setSunPosition(Ogre::Vector3(0.0f, 1000.0f, 0.0f));
              mHydrax->setSunStrength(1.75f);
              mHydrax->setSunArea(150.0f);
               mHydrax->setSunColor(Ogre::Vector3(1.0f, 0.9f, 0.6f));
//                //   // Configure foam component.
               mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_FOAM));
              mHydrax->setFoamMaxDistance(750.0f);
             mHydrax->setFoamScale(0.075f);
              mHydrax->setFoamStart(1.0f);
               mHydrax->setFoamTransparency(1.0f);
               // Configure depth component.
             mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_DEPTH));
              mHydrax->setDepthLimit(10.0f);
//                 /*------------------------------*/
                mHydrax->create();

                flag=2;

               Ogre::LogManager::getSingletonPtr()->logMessage("*** OKK ***");
                }
            }
            if((mSlider->getValue()==3)){
                if(flag==2){
                 fftOptions.Spectrum=3;
                 fftOptions.WindDirection=Ogre::Vector2(0.5,1);
                 fftOptions.Winds=10;
                 fftOptions.Fetch=1000;
                 fftOptions.Depth=100;
                 fftOptions.Resolution=fftOptions.PhysicalResolution=Grid;
                  mHydrax->~Hydrax();
                 /*-----------------------------*/
                 mHydrax = new Hydrax::Hydrax(mSceneMgr, mCamera, mWindow->getViewport(0));
                 // Use CG shaders for Hydrax
                 mHydrax->setShaderMode(Hydrax::MaterialManager::SM_CG);
                 // Create hydrax geometry, simple grid
                 Hydrax::Module::SimpleGrid::Options hydraxModuleOptions;
                 hydraxModuleOptions.Complexity=Grid;
                 hydraxModuleOptions.MeshSize=Grid;
                 mModule  = new Hydrax::Module::SimpleGrid(mHydrax,new Hydrax::Noise::FFT(fftOptions), Hydrax::MaterialManager::NM_VERTEX,hydraxModuleOptions);
                 // Tell Hydrax to use this geometry module.
                 mHydrax->setModule(mModule, true);
//                 // Configure sun component.
//                 mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_SUN));
//                 mHydrax->setSunPosition(Ogre::Vector3(0.0f, 1000.0f, 0.0f));
//                 mHydrax->setSunStrength(1.75f);
//                 mHydrax->setSunArea(150.0f);
//                 mHydrax->setSunColor(Ogre::Vector3(1.0f, 0.9f, 0.6f));
//                 //   // Configure foam component.
//                 mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_FOAM));
//                 mHydrax->setFoamMaxDistance(750.0f);
//                 mHydrax->setFoamScale(0.075f);
//                 mHydrax->setFoamStart(1.0f);
//                 mHydrax->setFoamTransparency(1.0f);
//                 //// Configure depth component.
//                 mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_DEPTH));
//                 mHydrax->setDepthLimit(10.0f);
//                  /*------------------------------*/
                mHydrax->create();
                flag=3;
                }
            }
        }

        }
    mSkyX->update(evt.timeSinceLastFrame);
     mHydrax->update(evt.timeSinceLastFrame);
      return true;
}
//---------------------------------------------------------------------------
bool WaterSimulation::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;

        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_LSHIFT)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if(arg.key==OIS::KC_M){

        pos=bodyScene->getPosition();
         bodyScene->setPosition((pos.x)+1,mHydrax->getHeigth(pos),pos.z);
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

    mCameraMan->injectKeyDown(arg);
    return true;
}
//---------------------------------------------------------------------------
bool WaterSimulation::keyReleased(const OIS::KeyEvent &arg)
{
    mCameraMan->injectKeyUp(arg);
    return true;
}
//---------------------------------------------------------------------------
bool WaterSimulation::mouseMoved(const OIS::MouseEvent &arg)
{
    if (mTrayMgr->injectMouseMove(arg)) return true;
    mCameraMan->injectMouseMove(arg);
    return true;

}
//---------------------------------------------------------------------------
bool WaterSimulation::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);
    return true;
}
//---------------------------------------------------------------------------
bool WaterSimulation::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    mCameraMan->injectMouseUp(arg, id);
    return true;
   }

//---------------------------------------------------------------------------
// Adjust mouse clipping area
void WaterSimulation::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
//---------------------------------------------------------------------------
// Unattach OIS before window shutdown (very important under Linux)
void WaterSimulation::windowClosed(Ogre::RenderWindow* rw)
{
    // Only close for window that created OIS (the main window in these demos)
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
//---------------------------------------------------------------------------
