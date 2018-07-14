#include "depthwave.h"

Hydrax::Noise::FFT::Options fftopt;
Hydrax::Module::SimpleGrid::Options moduleSGopt;
Hydrax::Module::ProjectedGrid::Options modulePGopt;
SkyX::CloudLayer::Options cloudSKopt;
//SkyX::CloudLayer::Options cloudSKopt1;
bool cbx,cbx0,cbx1,cbx2,flg;
int grid;

//---------------------------------------------------------------------------
DEPTHWAVE::DEPTHWAVE(void)
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
DEPTHWAVE::~DEPTHWAVE(void)
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
bool DEPTHWAVE::configure(void)
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
void DEPTHWAVE::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    // Initialize the OverlaySystem (changed for Ogre 1.9)
    mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);
}
//---------------------------------------------------------------------------
void DEPTHWAVE::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    mCamera->setFarClipDistance(99999);
    mCamera->setNearClipDistance(0.1);

    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // Create a default camera controller
}
//---------------------------------------------------------------------------
void DEPTHWAVE::createFrameListener(void)
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
    //mTrayMgr->showFrameStats(OgreBites::TL_TOP);
    mTrayMgr->showBackdrop("SdkTrays/Shade");
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

    Ogre::String infoText = "[ESC]->Salir";
   mLabel= mTrayMgr->createLabel(OgreBites::TL_TOPRIGHT, "InfoPanel", infoText,160);
 mLabel->hide();
Ogre::String infoText1 = "[Reinicio]->Restablecer";
mLabel1= mTrayMgr->createLabel(OgreBites::TL_TOPRIGHT, "InfoPanel1",infoText1,160);
mLabel1->hide();
   mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 220, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();
 //   Ogre::StringVector item;
//   item.push_back("Elevacion de la nave");
//   mDetailsPanel1 = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel1", 260, item);
//   //mDetailsPanel1->setParamValue(0, Ogre::StringConverter::toString(mHydrax->getHeigth(pos)));
//   mDetailsPanel1->show();
    //Button
    mButton =mTrayMgr->createButton(OgreBites::TL_CENTER,"Button","Inicio",75);
    mButton->show();
    //
    mLabel3=mTrayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT,"Label3","Variables",220);
    mLabel3->show();
    //Slider
    mSlider=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider","Complejidad(2^n)",220,80,5,12,8);
   mSlider->show();
    //
    mSlider1=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider1","Velocidad del viento",220,80,0,70,71*100);
    mSlider1->show();
    //
    mSlider2=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider2","Fetch",220,80,0,1000,1001*1000);
    mSlider2->show();
    //
    mSlider3=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider3","Profundidad",220,80,0,500,501);
    mSlider3->show();
    //
    mSlider4=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider4","Viento X",220,80,0,90,91);
    mSlider4->show();
    //
    mSlider5=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider5","Viento Y",220,80,0,90,91);
    mSlider5->show();
    //
    mLabel2=mTrayMgr->createLabel(OgreBites::TL_BOTTOMLEFT,"Label2","Espectro",100);
    mLabel2->show();
    //ChekBox
    mCheckBox=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMLEFT,"ChekBox","Phillips",100);
    mCheckBox->show();
    //
    mCheckBox0=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMLEFT,"ChekBox0","  P-M  ",100);
    mCheckBox0->show();
    //
    mCheckBox1=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMLEFT,"ChekBox1","JONSWAP",100);
    mCheckBox1->show();
    //
    mCheckBox2=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMLEFT,"ChekBox2","  TMA  ",100);
    mCheckBox2->show();
   //
    mRoot->addFrameListener(this);
}
//---------------------------------------------------------------------------
void DEPTHWAVE::createScene(void)
{

    // Create a light, same parameters as sun component
    Ogre::Light* ogreLight = mSceneMgr->createLight("Light");
    ogreLight->setPosition(Ogre::Vector3(0.0f, 100.0f, 0.0f));
    ogreLight->setDiffuseColour(1, 1, 1);
    ogreLight->setSpecularColour(1.0f, 0.9f, 0.6f);
    // Light
    Ogre::Light *mLight0 = mSceneMgr->createLight("Light0");
    mLight0->setDiffuseColour(1, 1, 1);
    mLight0->setCastShadows(false);

    // Shadow caster
    Ogre::Light *mLight1 = mSceneMgr->createLight("Light1");
    mLight1->setType(Ogre::Light::LT_DIRECTIONAL);

    // Create the ground
//     Ogre:: MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,  Ogre::Plane(Ogre::Vector3(0,1,0), 0),
//          256, 256, 8, 8, true, 1,8,8, Ogre::Vector3::UNIT_Z);
//      Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
//      groundEntity->setMaterialName ("Examples/Rocky");
//      Ogre::SceneNode* groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
//      groundNode->attachObject(groundEntity);
//      groundNode->setPosition(0,-45,0);

 //  mSceneMgr->setSkyBox(true,"Examples/EveningSkyBox",10000,true);
    //mSceneMgr->setSkyBox(true,"Sky/Clouds",100,true);
   // mSceneMgr->setSkyBox(true,"Sky/ClubTropicana",100,true);
   // mSceneMgr->setSkyBox(true,"Sky/EarlyMorning",100,true);
    Ogre::Entity* ogreEntity = mSceneMgr->createEntity("Barrel.mesh");
    Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode->attachObject(ogreEntity);
    ogreNode->setScale(10,10,10);
         createWater();
         // Color gradients
         // Water
        // mWaterGradient = SkyX::ColorGradient();
         mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.779105)*0.4, 1));
         mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.729105)*0.3, 0.8));
         mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.25, 0.6));
         mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.2, 0.5));
         mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.1, 0.45));
         mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.058209,0.535822,0.679105)*0.025, 0));
         // Sun
         //mSunGradient = SkyX::ColorGradient();
         mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.5, 1.0f));
         mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.4, 0.75f));
         mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8,0.75,0.55)*1.3, 0.5625f));
         mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.6,0.5,0.2)*1.5, 0.5f));
         mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.5,0.5,0.5)*0.25, 0.45f));
        mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.5,0.5,0.5)*0.01, 0.0f));
//         // Ambient
       //  mAmbientGradient = SkyX::ColorGradient();
         mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*1, 1.0f));
         mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*1, 0.6f));
         mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.6, 0.5f));
         mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.3, 0.45f));
         mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.1, 0.35f));
         mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1,1,1)*0.05, 0.0f));

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
                    mSkyX->getCloudsManager()->add(o);
                   // mSkyX->setLightingMode(1);
                    o1.InnerRadius=1;
                   // mSkyX->getAtmosphereManager()->setOptions(SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 0.5, 4));
                   mSkyX->getAtmosphereManager()->setOptions(SkyX::AtmosphereManager::Options());
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void DEPTHWAVE::destroyScene(void)
{
   mHydrax->~Hydrax();
}
//---------------------------------------------------------------------------
void DEPTHWAVE::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0.4,0.4,0.4));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//---------------------------------------------------------------------------
void DEPTHWAVE::setupResources(void)
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
void DEPTHWAVE::createResourceListener(void)
{
}
//---------------------------------------------------------------------------
void DEPTHWAVE::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//---------------------------------------------------------------------------
void DEPTHWAVE::go(void)
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
bool DEPTHWAVE::setup(void)
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
bool DEPTHWAVE::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
    mTrayMgr->refreshCursor();
    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // If dialog isn't up, then update the camera
        if(mButton->getState()==OgreBites::BS_DOWN){
            mTrayMgr->hideBackdrop();
            mTrayMgr->adjustTrays();
            // Position it at 500 in Z direction
            mCamera->setPosition(Ogre::Vector3(-Ogre::Math::Pow(2,grid)/2,Ogre::Math::Pow(2,grid)/2,Ogre::Math::Pow(2,grid)/2));
            // Look back along -Z
           // mCamera->lookAt(Ogre::Vector3(-81.6,68,117.7));
           // mCamera->setOrientation(Ogre::Quaternion(0.9,-0.21,-0.29,-0.065));
          //  mCameraMan->setStyle(OgreBites::CS_ORBIT);
            mTrayMgr->moveWidgetToTray(mButton, OgreBites::TL_BOTTOM, -1);
            mButton->setCaption("Reinicio");
          /*-*/
            mLabel->show();
            mLabel1->show();
                 //Slider
//               mSlider=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider","Complejidad(2^n)",256,80,3,9,7);
//               mSlider->show();
//               //
//               mSlider1=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider1","Velocidad del viento",256,80,0,25,26);
//               mSlider1->show();
//               //
//               mSlider2=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider2","Fecth",256,80,0,1000,1001);
//               mSlider2->show();
//               //
//               mSlider3=mTrayMgr->createThickSlider(OgreBites::TL_BOTTOMRIGHT,"Slider3","Profundidad",256,80,0,500,501);
//               mSlider3->show();
//               //ChekBox
//               mCheckBox=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMRIGHT,"ChekBox","Phillips",110);
//               mCheckBox->show();
//               //
//               mCheckBox0=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMRIGHT,"ChekBox0","  P-M  ",110);
//               mCheckBox0->show();
//               //
//               mCheckBox1=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMRIGHT,"ChekBox1","JONSWAP",110);
//               mCheckBox1->show();
//               //
//               mCheckBox2=mTrayMgr->createCheckBox(OgreBites::TL_BOTTOMRIGHT,"ChekBox2","  TMA  ",110);
//               mCheckBox2->show();
         /*-*/

 mCheckBox->setChecked(false);
  mCheckBox0->setChecked(false);
   mCheckBox1->setChecked(false);
   mCheckBox2->setChecked(false);
   flg=false;
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

        grid=mSlider->getValue();
        fftopt.Winds=mSlider1->getValue();
        fftopt.Fetch=mSlider2->getValue();
        fftopt.Depth=mSlider3->getValue();
        fftopt.WindDirection.x=mSlider4->getValue();
        fftopt.WindDirection.y=mSlider5->getValue();

        //
        if(mCheckBox->isChecked())
        {
           cbx=true;
        }
        else
        {
           cbx=false;
        }
         if(mCheckBox0->isChecked())
        {
            cbx0=true;

        }
         else
         {
            cbx0=false;
         }
         if(mCheckBox1->isChecked())
        {
            cbx1=true;
        }
         else
         {
            cbx1=false;
         }
        if(mCheckBox2->isChecked())
        {
           cbx2=true;

        }
        else
        {
           cbx2=false;

        }
        //
        if(cbx&&!flg)
        {
            fftopt.Spectrum=1;
            mHydrax->~Hydrax();
            createWater();
            flg=true;

        }
        if(cbx0&&!flg)
        {
            fftopt.Spectrum=2;
            mHydrax->~Hydrax();
            createWater();
            flg=true;
        }
        if(cbx1&&!flg)
        {
            fftopt.Spectrum=3;
            mHydrax->~Hydrax();
            createWater();
            flg=true;
        }
        if(cbx2&&!flg)
        {
            fftopt.Spectrum=4;
            mHydrax->~Hydrax();
            createWater();
            flg=true;
        }

   }
    /*-------------------------------------*/

  //  Ogre::Vector3 lightDir = mSkyX->getAtmosphereManager()->getSunDirection();
    Ogre::Vector3 lightDir = mSkyX->getController()->getSunDirection();

//    bool preForceDisableShadows = mForceDisableShadows;
//    mForceDisableShadows = (lightDir.y > 0.15f) ? true : false;

//    if (preForceDisableShadows != mForceDisableShadows)
//    {
//    setShadowMode(mSceneMgr, static_cast<ShadowMode>(mShadowMode));
//    }

    // Calculate current color gradients point
  //  float point = (-lightDir.y + 1.0f) / 2.0f;
    float point = (-lightDir.y) / 2.0f;
    mHydrax->setWaterColor(mWaterGradient.getColor(point));

  ///  Ogre::Vector3 sunPos = mCamera->getDerivedPosition() - lightDir*mSkyX->getMeshManager()->getSkydomeRadius(mCamera)*0.1;
    Ogre::Vector3 sunPos = mCamera->getDerivedPosition() - lightDir*mSkyX->getSceneManager()->getSkyDomeGenParameters().skyDomeDistance*0.1;
    mHydrax->setSunPosition(sunPos);



   Ogre::Light *Light0 = mSceneMgr->getLight("Light0"),
   *Light1 = mSceneMgr->getLight("Light1");

   // Light0->setPosition(mCamera->getDerivedPosition() - lightDir*mSkyX->getMeshManager()->getSkydomeRadius(mCamera)*0.02);
    Light0->setPosition(mCamera->getDerivedPosition() - lightDir*mSkyX->getSceneManager()->getSkyDomeGenParameters().skyDomeDistance*0.02);
    Light1->setDirection(lightDir);

    Ogre::Vector3 sunCol = mSunGradient.getColor(point);
   Light0->setSpecularColour(sunCol.x, sunCol.y, sunCol.z);
    Ogre::Vector3 ambientCol = mAmbientGradient.getColor(point);
    Light0->setDiffuseColour(ambientCol.x, ambientCol.y, ambientCol.z);
   mHydrax->setSunColor(sunCol);

/*----------------------------------------*/
   mSkyX->update(evt.timeSinceLastFrame);
    // mHydrax->update(evt.timeSinceLastFrame);

   mHydrax->update(evt.timeSinceLastFrame);


      return true;
}
//---------------------------------------------------------------------------
void DEPTHWAVE::createWater(){

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

   //mHydrax->setWaterColor(Ogre::Vector3(0.0f, 0.0f, 0.0f));

    fftopt.PhysicalResolution=Ogre::Math::Pow(2,grid);
    fftopt.Resolution=Ogre::Math::Pow(2,grid);
    fftopt.WindDirection=Ogre::Vector2(10,0);
//    fftopt.Amplitude=0.00348;
//    fftopt.Depth=200;
//    fftopt.Fetch=400*1000;
//    fftopt.Winds=15;
//    fftopt.Spectrum=2;

    moduleSGopt.ChoppyWaves=true;
    moduleSGopt.Complexity=Ogre::Math::Pow(2,grid);
    moduleSGopt.MeshSize=Ogre::Math::Pow(2,grid);
    moduleSGopt.Smooth=true;
   // moduleSGopt.Strength=100;

    modulePGopt.ChoppyWaves=true;
    modulePGopt.Complexity=Ogre::Math::Pow(2,grid);
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
    mHydrax->setModule(mModulePG,true);


    //// Component configuration.
 //   // You can comment each of following blocks.
 //   // You can also load all those parameters from a config file.

 //   // Configure sun component.
    mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_SUN));
    mHydrax->setSunPosition(Ogre::Vector3(0.0f, 100.0f, 0.0f));
    mHydrax->setSunStrength(1.75f);
    mHydrax->setSunArea(150.0f);
    mHydrax->setSunColor(Ogre::Vector3(1.0f, 0.9f, 0.8f));
// mHydrax->setSunColor(Ogre::Vector3(181.0f/255.0f, 178.0f/255.0f, 172.0f/255.0f));


   // Configure foam component.
    mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_FOAM));
    mHydrax->setFoamMaxDistance(75000000.0f);
    mHydrax->setFoamScale(0.0075f);
    mHydrax->setFoamStart(0.0f);
    mHydrax->setFoamTransparency(1.0f);

    // Configure depth component.
    mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_DEPTH));
    mHydrax->setDepthLimit(90.0f);

    // Configure smooth component.
    mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_SMOOTH));
    mHydrax->setSmoothPower(5.0f);

    // Configure caustics component.
    mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_CAUSTICS));
    mHydrax->setCausticsScale(135.0f);
    mHydrax->setCausticsPower(10.5f);
    mHydrax->setCausticsEnd(0.8f);

    // Configure underwater rendering
    mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_UNDERWATER));
 // Configure underwater rendering
    mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_UNDERWATER_REFLECTIONS));

    //// Configure god rays component.
    mHydrax->setComponents(static_cast<Hydrax::HydraxComponent> (mHydrax->getComponents() | Hydrax::HYDRAX_COMPONENT_UNDERWATER_GODRAYS));
    mHydrax->setGodRaysExposure(Ogre::Vector3(0.76f, 2.46f, 2.29f));
    mHydrax->setGodRaysIntensity(0.015f);
    mHydrax->getGodRaysManager()->setSimulationSpeed(5.0f);
    mHydrax->getGodRaysManager()->setNumberOfRays(100);
    mHydrax->getGodRaysManager()->setRaysSize(0.03f);
    mHydrax->getGodRaysManager()->setObjectIntersectionsEnabled(false);
   // Everything configured, create.
    mHydrax->create();
    ////// Add the depth technique to our material
    mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Examples/BeachStones"))->createTechnique());
}

//---------------------------------------------------------------------------
bool DEPTHWAVE::keyPressed( const OIS::KeyEvent &arg )
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
    else if (arg.key == OIS::KC_P)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".pNG");
       // gridScreenshots(mWindow,mCamera,1024,"DEPWTHWAVE",".png",false);
    }
    else if(arg.key==OIS::KC_M){

        //pos=bodyScene->getPosition();
        // bodyScene->setPosition((pos.x)+1,mHydrax->getHeigth(pos),pos.z);
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

    mCameraMan->injectKeyDown(arg);
    return true;
}
//---------------------------------------------------------------------------
bool DEPTHWAVE::keyReleased(const OIS::KeyEvent &arg)
{
    mCameraMan->injectKeyUp(arg);
    return true;
}
//---------------------------------------------------------------------------
bool DEPTHWAVE::mouseMoved(const OIS::MouseEvent &arg)
{
    if (mTrayMgr->injectMouseMove(arg)) return true;
    mCameraMan->injectMouseMove(arg);
    return true;

}
//---------------------------------------------------------------------------
bool DEPTHWAVE::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);
    return true;
}
//---------------------------------------------------------------------------
bool DEPTHWAVE::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    mCameraMan->injectMouseUp(arg, id);
    return true;
   }

//---------------------------------------------------------------------------
// Adjust mouse clipping area
void DEPTHWAVE::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
void DEPTHWAVE::gridScreenshots(Ogre::RenderWindow* pRenderWindow, Ogre::Camera* pCamera, const int& pGridSize, const Ogre::String& pFileName, const Ogre::String& pFileExtention, const bool& pStitchGridImages)
{
  /* Parameters:
   *  pRenderWindow:    Pointer to the render window.  This could be "mWindow" from the ExampleApplication,
   *              the window automatically created obtained when calling
   *              Ogre::Root::getSingletonPtr()->initialise(false) and retrieved by calling
   *              "Ogre::Root::getSingletonPtr()->getAutoCreatedWindow()", or the manually created
   *              window from calling "mRoot->createRenderWindow()".
   *  pCamera:      Pointer to the camera "looking at" the scene of interest
   *  pGridSize:      The magnification factor.  A 2 will create a 2x2 grid, doubling the size of the
                screenshot.  A 3 will create a 3x3 grid, tripling the size of the screenshot.
   *  pFileName:      The filename to generate, without an extention.  To generate "MyScreenshot.png" this
   *              parameter would contain the value "MyScreenshot".
   *  pFileExtention:    The extention of the screenshot file name, hence the type of graphics file to generate.
   *              To generate "MyScreenshot.png" this parameter would contain ".png".
   *  pStitchGridImages:  Determines whether the grid screenshots are (true) automatically stitched into a single
   *              image (and discarded) or whether they should (false) remain in their unstitched
   *              form.  In that case they are sequentially numbered from 0 to
   *              pGridSize * pGridSize - 1 (if pGridSize is 3 then from 0 to 8).
   *
  */
  Ogre::String gridFilename;

  if(pGridSize <= 1)
  {
    // Simple case where the contents of the screen are taken directly
    // Also used when an invalid value is passed within pGridSize (zero or negative grid size)
    gridFilename = pFileName + pFileExtention;

    pRenderWindow->writeContentsToFile(gridFilename);
  }
  else
  {
    // Generate a grid of screenshots
    pCamera->setCustomProjectionMatrix(false); // reset projection matrix
    Ogre::Matrix4 standard = pCamera->getProjectionMatrix();
    double nearDist = pCamera->getNearClipDistance();
    double nearWidth = (pCamera->getWorldSpaceCorners()[0] - pCamera->getWorldSpaceCorners()[1]).length();
    double nearHeight = (pCamera->getWorldSpaceCorners()[1] - pCamera->getWorldSpaceCorners()[2]).length();
    Ogre::Image sourceImage;
    Ogre::uchar* stitchedImageData;

    // Process each grid
    for (int nbScreenshots = 0; nbScreenshots < pGridSize * pGridSize; nbScreenshots++)
    {
      // Use asymmetrical perspective projection. For more explanations check out:
      // http://www.cs.kuleuven.ac.be/cwis/research/graphics/INFOTEC/viewing-in-3d/node8.html
      int y = nbScreenshots / pGridSize;
      int x = nbScreenshots - y * pGridSize;
      Ogre::Matrix4 shearing(
        1, 0,(x - (pGridSize - 1) * 0.5) * nearWidth / nearDist, 0,
        0, 1, -(y - (pGridSize - 1) * 0.5) * nearHeight / nearDist, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
      Ogre::Matrix4 scale(
        pGridSize, 0, 0, 0,
        0, pGridSize, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
      pCamera->setCustomProjectionMatrix(true, standard * shearing * scale);
      Ogre::Root::getSingletonPtr()->renderOneFrame();
      gridFilename = pFileName + Ogre::StringConverter::toString(nbScreenshots) + pFileExtention;

      // Screenshot of the current grid
      pRenderWindow->writeContentsToFile(gridFilename);

      if(pStitchGridImages)
      {
        // Automatically stitch the grid screenshots
        sourceImage.load(gridFilename, "General"); // Assumes that the current directory is within the "General" resource group
        int sourceWidth = (int) sourceImage.getWidth();
        int sourceHeight = (int) sourceImage.getHeight();
        Ogre::ColourValue colourValue;
        int stitchedX, stitchedY, stitchedIndex;

        // Allocate memory for the stitched image when processing the screenshot of the first grid
        if(nbScreenshots == 0)
          stitchedImageData = new Ogre::uchar[(sourceImage.getWidth() * pGridSize) * (sourceImage.getHeight() * pGridSize) * 3]; // 3 colors per pixel

        // Copy each pixel within the grid screenshot to the proper position within the stitched image
        for(int rawY = 0; rawY < sourceHeight; rawY++)
        {
          for(int rawX = 0; rawX < sourceWidth; rawX++)
          {
            colourValue = sourceImage.getColourAt(rawX, rawY, 0);
            stitchedX = x * sourceWidth + rawX;
            stitchedY = y * sourceHeight + rawY;
            stitchedIndex = stitchedY * sourceWidth * pGridSize + stitchedX;
            Ogre::PixelUtil::packColour(colourValue,
                          Ogre::PF_R8G8B8,
                          (void*) &stitchedImageData[stitchedIndex * 3]);
          }
        }
        // The screenshot of the grid is no longer needed
        remove(gridFilename.c_str());
      }
    }
    pCamera->setCustomProjectionMatrix(false); // reset projection matrix

    if(pStitchGridImages)
    {
      // Save the stitched image to a file
      Ogre::Image targetImage;
      targetImage.loadDynamicImage(stitchedImageData,
                    sourceImage.getWidth() * pGridSize,
                    sourceImage.getHeight() * pGridSize,
                    1, // depth
                    Ogre::PF_R8G8B8,
                    false);
      targetImage.save(pFileName + pFileExtention);
      delete[] stitchedImageData;
    }
  }
}
//---------------------------------------------------------------------------
// Unattach OIS before window shutdown (very important under Linux)
void DEPTHWAVE::windowClosed(Ogre::RenderWindow* rw)
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
        DEPTHWAVE app;

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
