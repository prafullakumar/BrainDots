//
//  BallContactListener.h
//  BrainDotsDebug
//
//  Created by Nguyen Minh Hoang on 2015/09/02.
//
//

#ifndef __BrainDotsDebug__BallContactListener__
#define __BrainDotsDebug__BallContactListener__

#include <Box2D/Box2D.h>
#include <vector>
#include "SceneManager.h"
#include "GameScene.h"

class GameScene;
struct BallContact {
    b2Fixture* fixtureA;
    b2Fixture* fixtureB;
    b2Vec2 collisionPoint;
    bool operator==(const BallContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};

class BallContactListener: public b2ContactListener
{
public:
    std::vector<BallContact> _contacts;
    
    BallContactListener();
    ~BallContactListener();
    
    void setFixtureForBall(b2Fixture* fixBallA, b2Fixture* fixBallB);
    void setListConveyorBelt(std::vector<ConveyorBelt> list);
    
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
private:
    b2Fixture* mFixBallA, *mFixBallB;
    std::vector<ConveyorBelt> mListConveyorBelt;
};


#endif /* defined(__BrainDotsDebug__BallContactListener__) */
