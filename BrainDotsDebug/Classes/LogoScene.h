//
//  LogoScene.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/03.
//
//

#ifndef __BrainDotsDebug__LogoScene__
#define __BrainDotsDebug__LogoScene__

#include <cocos2d.h>
#include "SceneManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Network/ClientRequest.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ClientRequest.h"
#endif

USING_NS_CC;

class LogoScene: public Layer
{
public:
    
    static Scene* createScene();
    
    CREATE_FUNC(LogoScene);
    
    virtual bool init();

private:
    
    void addLogo();
    
    void transferData();
    
    void moveDataToSDcard(std::string filename);
    
    void loadResources();
    
    void loadImage(Texture2D* texture);
    
    void loadSounds();
    
    void loadMapLevels();
    
    void startSplashScreen();
    
    void nextScene();
    
    virtual void onExit() override;
    
};

#endif /* defined(__BrainDotsDebug__LogoScene__) */
