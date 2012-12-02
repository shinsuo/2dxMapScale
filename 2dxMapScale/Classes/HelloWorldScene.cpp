#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

static float MINSCALE = 0.5;
static float MAXSCALE = 2;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    /*
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(size.width/2, size.height/2) );

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    //*/
    this->setTouchEnabled(1);
    
    m_pTileMap = CCTMXTiledMap::create("tilemap.tmx");
    this->addChild(m_pTileMap);
    CCLog("pos:%f,%f,anchor:%f,%f",m_pTileMap->getPositionX(),m_pTileMap->getPositionY(),m_pTileMap->getAnchorPoint().x,m_pTileMap->getAnchorPoint().y);
    CCLog("%f,%f--",m_pTileMap->getMapSize().width*m_pTileMap->getTileSize().width,m_pTileMap->getMapSize().height*m_pTileMap->getTileSize().height);
//    m_pTileMap->setPosition(0, 0);
//    m_pTileMap->setAnchorPoint(CCPointZero);
    m_WinSize = CCDirector::sharedDirector()->getWinSize();
    
    
    
    return true;
}

#pragma mark Touch
void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    
    
    switch (pTouches->count()) {
        case 1:
            this->m_LastPoint = ((CCTouch *)pTouches->anyObject())->getLocation();
            break;
        case 2:
        {
            this->m_LastScale = m_pTileMap->getScale();
            
            CCSetIterator iter;
            CCPoint pt[2];
            int i = 0;
            for (iter = pTouches->begin(); iter != pTouches->end(); iter++) {
                CCTouch *touch = (CCTouch *)*iter;
                pt[i++] = touch->getLocation();
            }
            
            this->m_BeganDistance = ccpDistance(pt[0],pt[1]);
        }
            
            break;
        default:
            break;
    }
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    switch (pTouches->count()) {
        case 1:
        {
            CCPoint curPoint = ((CCTouch *)pTouches->anyObject())->getLocation();
            CCPoint translation = ccpSub(this->m_LastPoint,curPoint);
            CCPoint newPos = ccpSub(m_pTileMap->getPosition(), translation);
            if (this->isAllowMove(newPos)) {
                m_pTileMap->setPosition(newPos);
            }
            
            
        }
            break;
        case 2:
        {
            CCSetIterator iter;
            CCPoint pt[2];
            int i = 0;
            for (iter = pTouches->begin(); iter != pTouches->end(); iter++) {
                CCTouch *touch = (CCTouch *)*iter;
                pt[i++] = touch->getLocation();
            }
            
            this->m_CurDistance = ccpDistance(pt[0],pt[1]);
            
            float changedScale = this->m_CurDistance/this->m_BeganDistance;
            float nowScale = this->m_LastScale - 1 + changedScale;
            nowScale = MIN(nowScale,MINSCALE);
            nowScale = MAX(nowScale,MAXSCALE);
            
            if (this->m_LastScale > nowScale) {
                CCPoint newPos = ccpSub(m_pTileMap->getPosition(), ccpMult(ccpNormalize(m_pTileMap->getPosition()), ccpLength(m_pTileMap->getPosition())*(this->m_LastScale - nowScale)/(this->m_LastScale -1)));
                if (this->isAllowMove(newPos)) {
                    m_pTileMap->setPosition(newPos);
                }
            }
            m_pTileMap->setScale(nowScale);
            
        }
            break;
        default:
            break;
    }
}

bool HelloWorld::isAllowMove(const CCPoint &pt)
{
    float x = m_WinSize.width - m_pTileMap->getTileSize().width*m_pTileMap->getMapSize().width*m_pTileMap->getScale();
    float y = m_WinSize.height - m_pTileMap->getTileSize().height*m_pTileMap->getMapSize().height*m_pTileMap->getScale();
    float width = abs(x);
    float height = abs(y);
    
    
    CCRect rect = CCRectMake(x, y, width, height);
    return (pt.x > rect.origin.x && pt.x < rect.origin.x+rect.size.width &&
            pt.y > rect.origin.y && pt.y < rect.origin.y+rect.size.height);
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
