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
//How oftern is update run? every tick?
void HelloWorld::update(float dx)
{
    CCObject* brick;
    CCARRAY_FOREACH(_bricks, brick){
        if (! ((CCSprite *) brick)->isVisible() ){ //skip if the brick is already broken
            continue;
        }

        //if mole->boundingBox().containsPoint(touchLocation)) {

        if (_ball->boundingBox().intersectsRect(((CCSprite *)brick)->boundingBox())){
            ((CCSprite *)brick)->setVisible(false);
        }
    }

    _ball->setPosition(CCPoint(_ball->getPosition().x+1, _ball->getPosition().y+1)); //DEBUG MOVE BALL


}

void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
     
    // Create a new touch object that holds the touch event.
    CCTouch *touch = (CCTouch *)touches->anyObject();
 
    //get the location of the touch event.
    CCPoint location = touch->getLocationInView();
 
    //Convert
    CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);

    convertedLocation.y = 50;
 
    // stop all current actions on the sprite.
    _paddle->stopAllActions();
 
    // run a new action which is to move the sprite to the new location with a 1 second delay.
    _paddle->runAction(CCMoveTo::create(1.0, convertedLocation));

}

void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
    

}


// Method is called after the user is finished touching the screen or they have lifted their finger off the screen.
void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event) {
 
 
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

    this->setTouchEnabled(true); //Enable touches on the scene!
    _bricks = new CCArray();//Init array of _bricks

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

    CCSprite *brick;   
    //Top Row
    for(int i = 0; i < 7; i++) {

        brick = CCSprite::create("brick_blue.png");

        static int padding= (winSize.width-((brick->getContentSize().width)*7))/8;

        int xOffset = padding+brick->getContentSize().width/2+((brick->getContentSize().width+padding)*i);
        brick->setPosition(ccp(xOffset, winSize.height - brick->getContentSize().height * .5) );
        this->addChild(brick); //Add to screen
        _bricks->addObject(brick); //Add to brick array
        
    }

    //Middle Row
    for(int i = 0; i < 5; i++) {

        brick = CCSprite::create("brick_orange.png");

        static int padding= (winSize.width-((brick->getContentSize().width)*5))/6;

        int xOffset = padding+brick->getContentSize().width/2+((brick->getContentSize().width+padding)*i);
        brick->setPosition(ccp(xOffset, winSize.height - brick->getContentSize().height * 1.5) ); //set row in relation to top row
        this->addChild(brick); //Add to screen
        _bricks->addObject(brick); //Add to brick array
        
    }

    //Bottom Row
    for(int i = 0; i < 6; i++) {
        
        brick = CCSprite::create("brick_purple.png");

        static int padding= (winSize.width-((brick->getContentSize().width)*6))/7;

        int xOffset = padding+brick->getContentSize().width/2+((brick->getContentSize().width+padding)*i);
        brick->setPosition(ccp(xOffset, winSize.height - brick->getContentSize().height * 2.5)); //set row in relation to the top of the screen and the size of the _bricks
        this->addChild(brick); //Add to screen
        _bricks->addObject(brick); //Add to brick array
        
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


