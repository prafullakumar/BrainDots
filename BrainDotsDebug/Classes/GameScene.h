#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <ui/CocosGUI.h>
#include "BallContactListener.h"
#include "TiledBodyCreator.h"
#include "SceneManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "B2DebugDraw/B2DebugDrawLayer.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "B2DebugDrawLayer.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Clipper/clipper.hpp"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "clipper.hpp"
#endif
#include "TexturePolygon.h"
#include "GB2ShapeCache-x.h"

USING_NS_CC;
using namespace cocos2d::ui;

class RayCastMultipleCallback : public b2RayCastCallback {
public:
    enum {
        e_maxCount = 3
    };
    
    RayCastMultipleCallback() {
        m_count = 0;
    }
    
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
        b2Body* body = fixture->GetBody();
        void* userData = body->GetUserData();
        if (userData) {
            int32 index = *(int32*)userData;
            if (index == 0)
                return -1.0f;
        }
        
        b2Assert(m_count < e_maxCount);
        
        m_points[m_count] = point;
        m_normals[m_count] = normal;
        ++m_count;
        
        if (m_count == e_maxCount)
            return 0.0f;
        return 1.0f;
    }
    
    b2Vec2 m_points[e_maxCount];
    b2Vec2 m_normals[e_maxCount];
    int32 m_count;
};

class RayCastClosestCallback : public b2RayCastCallback {
public:
    RayCastClosestCallback() {
        m_hit = false;
    }
    
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
        b2Body* body = fixture->GetBody();
        void* userData = body->GetUserData();
        if (userData) {
            int32 index = *(int32*)userData;
            if (index == 0)
                return -1.0f;
        }
        
        m_hit = true;
        m_point = point;
        m_normal = normal;
        return fraction;
    }
    
    bool m_hit;
    b2Vec2 m_point;
    b2Vec2 m_normal;
};

class BallContactListener;
class TiledBodyCreator;
class GameScene : public cocos2d::LayerColor
{
public:
    
    // game constructor
    GameScene();
    ~GameScene();

    // init all components
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    CREATE_FUNC(GameScene);
    
    // touch components
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    void touchButtonEvent(Ref* sender, Widget::TouchEventType type);
    
    // draw grids
    void drawGrids();
    
    // physic components
    void initPhysics();
    void initMapLevel(int level);
    void initPhysicObjects();
    void initWall(b2Body* body, b2Fixture* _wallFixture[],float outside, uint16 categorybits, uint16 maskbits);
    void initBalls();
    
    // execute touch and body
    void addRectangleBetweenPointsToBody(b2Body* body, Vec2 start, Vec2 end);
    Vec2 checkInsideBox2d(Vec2 start, Vec2 end);
    Vec2 checkInsideBox2dByRayCats(Vec2 start, Vec2 end);
    std::vector<Vec2> getListPointsIn2Point(Vec2 start, Vec2 end);
    
    // game components
    void endGame();
    void removeTiledMap();
    void removeAllObjects();
    void backMenu();
    
    // animation gameover
    void animationSuccess(Vec2 collisionPoint);
    void animationFail(Vec2 collisionPoint, std::string explosionName);
    void explosionBall(b2Body* ball);
    void explosionRing(std::string name, Vec2 point);
    
    // update game
    void update(float dt);
    
private:
    // size and pos screen
    Size visibleSize;
    Vec2 origin;
    
    // map variable
    TMXTiledMap* map;
    
    // reader map
    TiledBodyCreator* tiledmap;
    
    // test draw node
    DrawNode* drawnode;
    
    // control game
    bool isSuccess = false;
    bool isFail = false;
    bool m_bClearBox;
    
    // physics variable
    b2World* world;
    b2Body* currentPlatformBody;
    b2PolygonShape boxShape;
    b2FixtureDef boxFixtureDef;
    b2Body* ballA;
    b2Body* ballB;
    b2Fixture* _ballAFixture;
    b2Fixture* _ballBFixture;
    b2Fixture* _wallFixture1[4];
    b2Fixture* _wallFixture2[4];
    b2Fixture* electricictyFixture;
    b2Fixture* switchFixture;
    
    // contacts physics
    BallContactListener* _ballContactListener;
    
    // touch variable
    Vec2 collisionPoint = Vec2::ZERO;
    Vec2 collisionFailA = Vec2::ZERO;
    Vec2 collisionFailB = Vec2::ZERO;
    Vec2 posballA, posballB;
    Vec2 posErrorDraw;
    bool isErrorDraw = false;
    std::vector<Vec2> platformPoints;
    RenderTexture *target;
    Sprite *brush;
    std::vector<ConveyorBelt> listConveyorBelt;
    
    std::vector<std::string> listNameTexture;
    // capture end games
    std::string filenameCapture;
    void afterCaptured(bool succeed, const std::string& outputFile);
    void showShareLayer(std::string filename);
};

#endif // __HELLOWORLD_SCENE_H__
