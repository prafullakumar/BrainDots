//
//  BallContact.h
//  BrainDots
//
//  Created by Nguyen Minh Hoang on 2015/08/25.
//
//

#ifndef __BrainDots__BallContact__
#define __BrainDots__BallContact__

#include <Box2D/Box2D.h>
#include <vector>

struct BallContact {
    b2Fixture* fixtureA;
    b2Fixture* fixtureB;
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
    
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#endif /* defined(__BrainDots__BallContact__) */
