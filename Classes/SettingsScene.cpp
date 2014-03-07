#include "HomeScene.h"   
#include "SimpleAudioEngine.h"
#include "SettingsScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* Settings::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Settings *layer = Settings::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void Settings::update(float dx)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    if( (_ball->getPosition().x+_ball->getContentSize().width/2) >= winSize.width){ //right edge
        _ball_x_direction = -1;
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("paddle.wav"); 
    }

    if( (_ball->getPosition().x-_ball->getContentSize().width/2) <= 0){ //left edge
        _ball_x_direction = 1;
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("paddle.wav"); 
    }

    if( (_ball->getPosition().y+_ball->getContentSize().height/2) >= winSize.height){ //top edge
        _ball_y_direction = -1;
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("paddle.wav"); 
    }

    if((_ball->getPosition().y-_ball->getContentSize().height/2) <= 0){ //bottom edge
        _ball_y_direction = 1;
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("paddle.wav"); 
    }

    _ball->setPosition(CCPoint(_ball->getPosition().x + (10*_ball_x_direction) , _ball->getPosition().y + (10*_ball_y_direction) ) ); //DEBUG MOVE BALL


}


void Settings::sfxCallback(CCObject* pSender)
{
    //
    if(CocosDenshion::SimpleAudioEngine::sharedEngine()->getEffectsVolume() == 0.0){
     CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0);
    } else {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0);
    }
 }

void Settings::musicCallback(CCObject* pSender)
{
    //
    if(CocosDenshion::SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() == 0.0){
     CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.8);
    } else {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0);
    }

}

void Settings::backCallback(CCObject* pSender)
{
    //
        CCDirector::sharedDirector()->replaceScene(Home::scene());

}

// on "init" you need to initialize your instance
bool Settings::init()
{


    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
     }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    /////////////////////////////
    // BCY3 code

    //Init sprites
    _background = CCSprite::create("background.png");
    _background->setPosition(ccp(winSize.width/2, winSize.height/2)); //center
   // _background->setSize();
    this->addChild(_background,-2);

    //Init Text
    CCLabelTTF *title = CCLabelTTF::create("Settings", "Silom.ttf", 80, CCSizeMake(winSize.width, 150), kCCTextAlignmentCenter);
    title->setAnchorPoint(ccp(0.5f,0.5f)); 
    title->setPosition(ccp(winSize.width/2, winSize.height-100));
    title->setColor(ccc3(80,80,80)); //Grey
    this->addChild(title, 4);
    

    //Init ball
        _ball_x_direction = 1;
    _ball_y_direction = 1;
        _ball = CCSprite::create("ball.png");
    _ball->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(_ball, -1);



    CCLabelTTF *music = CCLabelTTF::create("Music Courtesy of Bit Flipper", "Silom.ttf", 20, CCSizeMake(winSize.width, 150), kCCTextAlignmentCenter);
    music->setAnchorPoint(ccp(0.5f,0.5f)); 
    music->setPosition(ccp(winSize.width/2, 5));
    music->setColor(ccc3(80,80,80)); //Grey
    this->addChild(music, 4);




    //Init Menus
    CCMenuItem *new_game = CCMenuItemImage::create(
                                        "sfx.png",
                                        "sfx.png",
                                        this,
                                        menu_selector(Settings::sfxCallback) );

    new_game->setAnchorPoint(ccp(0.5f,0.5f)); 
    new_game->setPosition(winSize.width/2, title->getPositionY()-150);

    CCMenuItem *settings = CCMenuItemImage::create(
                                        "music.png",
                                        "music.png",
                                        this,
                                        menu_selector(Settings::musicCallback) );

            settings->setAnchorPoint(ccp(0.5f,0.5f)); 
        settings->setPosition(winSize.width/2, new_game->getPositionY()-140);

            CCMenuItem *back = CCMenuItemImage::create(
                                        "back.png",
                                        "back.png",
                                        this,
                                        menu_selector(Settings::backCallback) );

            back->setAnchorPoint(ccp(0.5f,0.5f)); 
        back->setPosition(winSize.width/2, settings->getPositionY()-140);


        CCMenu* main_menu = CCMenu::create(new_game, settings, back, NULL);

                    main_menu->setAnchorPoint(ccp(0.5f,0.5f)); 
        
        main_menu->setPosition(0,0);

   main_menu->setScale(0.5f); 

         this->addChild(main_menu, 10);






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

void Settings::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


