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
    m_winSize = CCDirector::sharedDirector()->getWinSize();
    this->setTouchEnabled(1);
    m_pTileMap = CCTMXTiledMap::create("tilemap.tmx");
    this->addChild(m_pTileMap);
    m_pTileMap->setAnchorPoint(CCPoint(0.5,0.5));
    m_pTileMap->setScale(MINSCALE);
    m_pTileMap->setPosition(CCPoint(m_winSize.width/2, m_winSize.height/2));
    CCLog("pos:%f,%f,anchor:%f,%f",m_pTileMap->getPositionX(),m_pTileMap->getPositionY(),m_pTileMap->getAnchorPoint().x,m_pTileMap->getAnchorPoint().y);
    CCLog("%f,%f--",m_pTileMap->getMapSize().width*m_pTileMap->getTileSize().width,m_pTileMap->getMapSize().height*m_pTileMap->getTileSize().height);
//    m_pTileMap->setPosition(0, 0);
//    m_pTileMap->setAnchorPoint(CCPointZero);
    
    
    
    return true;
}

#pragma mark Touch
void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    switch (pTouches->count()) {
        case 1:
            this->m_lastPoint = this->m_pTileMap->getPosition();
            this->m_beganPoint = ((CCTouch *)pTouches->anyObject())->getLocation();
            break;
        case 2:
        {
            this->m_lastScale = m_pTileMap->getScale();
            
            CCSetIterator iter;
            CCPoint pt[2];
            int i = 0;
            for (iter = pTouches->begin(); iter != pTouches->end(); iter++) {
                CCTouch *touch = (CCTouch *)*iter;
                pt[i++] = touch->getLocation();
            }
            
            this->m_beganDistance = ccpDistance(pt[0],pt[1]);
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
            CCPoint translation = ccpSub(curPoint,this->m_beganPoint);
            CCPoint newPos = ccpAdd(this->m_lastPoint, translation);
            if (this->isAllowMove(newPos)) {
                m_pTileMap->setPosition(newPos);
            }else{
                this->m_lastPoint = this->m_pTileMap->getPosition();
                this->m_beganPoint = ((CCTouch *)pTouches->anyObject())->getLocation();
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
            
            this->m_curDistance = ccpDistance(pt[0],pt[1]);
            
            float changedScale = this->m_curDistance/this->m_beganDistance;
            float nowScale = this->m_lastScale - 1 + changedScale;

            nowScale = MAX(nowScale,MINSCALE);
            nowScale = MIN(nowScale,MAXSCALE);
            
            if (this->m_lastScale > nowScale) {
                CCPoint newPos = ccpSub(m_pTileMap->getPosition(), ccpMult(ccpNormalize(m_pTileMap->getPosition()), ccpLength(m_pTileMap->getPosition())*(this->m_lastScale - nowScale)/(this->m_lastScale -MINSCALE)));
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
    float x = m_winSize.width - m_pTileMap->getTileSize().width*m_pTileMap->getMapSize().width*m_pTileMap->getScale();
    float y = m_winSize.height - m_pTileMap->getTileSize().height*m_pTileMap->getMapSize().height*m_pTileMap->getScale();
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
