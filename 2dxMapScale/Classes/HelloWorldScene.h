#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
//    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
//    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
    
private:
    cocos2d::CCTMXTiledMap *m_pTileMap;
    
    float m_curAnchorPoint;
    
    float m_curDistance;
    float m_beganDistance;
    float m_lastScale;
    
    cocos2d::CCPoint m_centerPoint;
    cocos2d::CCPoint m_beganPoint;
    cocos2d::CCPoint m_curPoint;
    cocos2d::CCPoint m_lastPoint;
    
    cocos2d::CCSize m_winSize;
    
    cocos2d::CCSize getMapSize();
    bool isAllowMove(const cocos2d::CCPoint &pt);
};

#endif // __HELLOWORLD_SCENE_H__
