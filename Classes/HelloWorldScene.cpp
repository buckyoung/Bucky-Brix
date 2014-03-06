#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#define PTM_RATIO 32

using namespace cocos2d;
using namespace CocosDenshion;

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

//this->scheduleUpdate() calls HelloWorld::update(float FRAMERATE:dt) --- scale velocity by dt factor

void HelloWorld::update(float dx)
{
    _ball->setPosition(CCPoint(200, 500));
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

    /////////////////////////////
    // BCY3 code
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //Sprites:
    _background = CCSprite::create("background.png");
    _background->setPosition(ccp(winSize.width/2, winSize.height/2)); //center
   // _background->setSize();
    this->addChild(_background,-1);

    _ball = CCSprite::create("ball.png");
    _ball->setPosition(ccp(100, 100));
    this->addChild(_ball);

    _paddle = CCSprite::create("paddle.png");
    _paddle->setPosition(ccp(winSize.width/2, 50));
    this->addChild(_paddle);

    CCSprite *_brick;   
    //Top Row
    for(int i = 0; i < 7; i++) {

        _brick = CCSprite::create("brick_blue.png");

        static int padding= (winSize.width-((_brick->getContentSize().width)*7))/8;

        int xOffset = padding+_brick->getContentSize().width/2+((_brick->getContentSize().width+padding)*i);
        _brick->setPosition(ccp(xOffset, winSize.height-_brick->getContentSize().height * .25) );
        this->addChild(_brick);
        
    }

    //Middle Row
    for(int i = 0; i < 5; i++) {

        _brick = CCSprite::create("brick_orange.png");

        static int padding= (winSize.width-((_brick->getContentSize().width)*5))/6;

        int xOffset = padding+_brick->getContentSize().width/2+((_brick->getContentSize().width+padding)*i);
        _brick->setPosition(ccp(xOffset, winSize.height-_brick->getContentSize().height * .75) ); //set row in relation to top row
        this->addChild(_brick);
        
    }

    //Bottom Row
    for(int i = 0; i < 6; i++) {
        
        _brick = CCSprite::create("brick_purple.png");

        static int padding= (winSize.width-((_brick->getContentSize().width)*6))/7;

        int xOffset = padding+_brick->getContentSize().width/2+((_brick->getContentSize().width+padding)*i);
        _brick->setPosition(ccp(xOffset, winSize.height-_brick->getContentSize().height * 1.25)); //set row in relation to the top of the screen and the size of the bricks
        this->addChild(_brick);
        
    }




    //CALL GAME LOGIC EVERY SECOND
    //this->schedule( schedule_selector(HelloWorld::gameLogic), 1.0 );

    this->scheduleUpdate();

    return true;


    /*
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
    
    return true;
    */
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


