#ifndef DEPTHWAVE_H
#define DEPTHWAVE_H

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <SdkTrays.h>
#include <SdkCameraMan.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
//Skyx include
#include<SKYX/SkyX.h>
//Hydrax includes
#include "Hydrax.h"
#include "Perlin.h"
#include "FFT.h"
#include "ProjectedGrid.h"
#include "SimpleGrid.h"

//--------------------------//
class DEPTHWAVE: public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    DEPTHWAVE(void);
    virtual ~DEPTHWAVE(void);

    virtual void go(void);
protected:
    virtual bool setup();
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void createScene(void); // Override me!
    virtual void destroyScene(void);
    virtual void createViewports(void);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    // Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    // Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);

    void createWater();
    void gridScreenshots(Ogre::RenderWindow* pRenderWindow, Ogre::Camera* pCamera, const int& pGridSize, const Ogre::String& pFileName, const Ogre::String& pFileExtention, const bool& pStitchGridImages);


    Ogre::Root*                 mRoot;
    Ogre::Camera*               mCamera;
    Ogre::SceneManager*         mSceneMgr;
    Ogre::RenderWindow*         mWindow;
    Ogre::String                mResourcesCfg;
    Ogre::String                mPluginsCfg;

    Ogre::OverlaySystem*        mOverlaySystem;
    Ogre::Viewport*             mViewport;
    // OgreBites
    OgreBites::InputContext     mInputContext;
    OgreBites::SdkTrayManager*	mTrayMgr;
    OgreBites::SdkCameraMan*    mCameraMan;     	// Basic camera controller
    OgreBites::ParamsPanel*     mDetailsPanel;   	// Sample details panel
    OgreBites::ParamsPanel*     mDetailsPanel1;
    OgreBites::Label*      mLabel;
    OgreBites::Label*      mLabel1;
    OgreBites::Label*      mLabel2;
    OgreBites::Label*      mLabel3;
    OgreBites::Button*          mButton;
    OgreBites::Slider*         mSlider;//grid
    OgreBites::Slider*         mSlider1;//windspeed
    OgreBites::Slider*         mSlider2;//fecth
    OgreBites::Slider*         mSlider3;//depth
    OgreBites::Slider*         mSlider4;//winddirx
    OgreBites::Slider*         mSlider5;//winddiry
    OgreBites::CheckBox*   mCheckBox;
    OgreBites::CheckBox*   mCheckBox0;
    OgreBites::CheckBox*   mCheckBox1;
    OgreBites::CheckBox*   mCheckBox2;


    bool                        mCursorWasVisible;	// Was cursor visible before dialog appeared?
    bool                        mShutDown;

    //OIS Input devices
    OIS::InputManager*          mInputManager;
    OIS::Mouse*                 mMouse;
    OIS::Keyboard*              mKeyboard;

    // Added for Mac compatibility
    Ogre::String                 m_ResourcePath;

    //Temporal Variables
    double prev_rot;
    Ogre::OverlayElement* panel;
    Ogre::Overlay* overlay;
    Ogre::TextAreaOverlayElement* mTextArea = 0;
    SkyX::SkyX *mSkyX;
    SkyX::BasicController *mBasicController;

    // Color gradients
    SkyX::ColorGradient mWaterGradient,mSunGradient,mAmbientGradient;
    Hydrax::Hydrax *mHydrax=0;
    SkyX::CloudLayer::Options o;
         SkyX::AtmosphereManager::Options o1;


};

#endif // DEPTHWAVE_H
