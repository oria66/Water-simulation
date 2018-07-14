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
#include <SkyX.h>

//--------------------------//



//---------------------------------------------------------------------------

class WaterSimulation : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    WaterSimulation(void);
    virtual ~WaterSimulation(void);

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
 //  OgreBites::ParamsPanel*     mDetailsPanel1;
   OgreBites::Button*          mButton;
   OgreBites::Slider*         mSlider;//spectrum
   OgreBites::Slider*         mSlider1;//windspeed
   OgreBites::Slider*         mSlider2;//fecth
   OgreBites::Slider*         mSlider3;//depth
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

    // Entities
    Ogre::Entity	*bodyEntity;
    Ogre::SceneNode *bodyScene;
    Ogre::SceneNode *ground;
    Ogre::Entity* ogreEntity2;
    Ogre::SceneNode* ogreNode2;
    //Temporal Variables
    double prev_rot;
     Ogre::OverlayElement* panel;
     Ogre::Overlay* overlay;
     Ogre::TextAreaOverlayElement* mTextArea = 0;
     SkyX::SkyX* mSkyX=0;
     SkyX::BasicController* mBasicController=0;
     SkyX::CloudLayer::Options o;
     SkyX::AtmosphereManager::Options o1;
     // Color gradients
     SkyX::ColorGradient mWaterGradient,
     mSunGradient,
     mAmbientGradient;



#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader m_StaticPluginLoader;
#endif
};



//---------------------------------------------------------------------------

//#endif // WATERSIMULATION_H
