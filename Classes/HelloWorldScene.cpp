#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "HomeScene.h"

#define SPEED1 2
#define SPEED2 5
#define SPEED3 7
#define SPEED4 9
#define SETY 70.0
#define SAVIOR_AMOUNT 500 //not used anymore -- savior now divides score by 2
#define PADDLEPIX 30 //number of pixels that the angled part of the paddle is

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

void HelloWorld::inc_score(int tag){
    _score = _score + (_speeds[_speed_index] * tag * _angle_multiplier * 10);
    char string[12] = { 0 };
    sprintf(string, "%d", _score);
    _score_label->setString(string);

}

void HelloWorld::dec_score(int amt){
    _score = _score /2 ; //NOT USING AMOUNT ANYMORE! ((( = _score - amt )))
    _score = (_score < 0)?0:_score;
    char string[12] = { 0 };
    sprintf(string, "%d", _score);
    _score_label->setString(string);

}

void HelloWorld::newGameCallback(CCObject* pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CCDirector::sharedDirector()->replaceScene(Home::scene());

}

//this->scheduleUpdate() calls HelloWorld::update(float FRAMERATE:dt) --- scale velocity by dt factor
//How oftern is update run? every tick?
void HelloWorld::update(float dx)
{
        CCObject* brick;
      CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    if(_running){

        CCARRAY_FOREACH(_bricks, brick){

            if (! ((CCSprite *) brick)->isVisible() ){ //skip if the brick is already broken
                continue;
            }

            //if mole->boundingBox().containsPoint(touchLocation)) {

            if (_ball->boundingBox().intersectsRect(((CCSprite *)brick)->boundingBox())){ //COLLISION: ball/brick
                ((CCSprite *)brick)->setVisible(false);
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("brick.wav"); 
                inc_score( ((CCSprite *)brick)->getTag() );
                _total_bricks--;

                //CHECK FOR WIN!
                if (_total_bricks == 0){
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("winsound.wav"); 
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
                    _won = true;
                    _running = false;
                    _once = true;
                }
                
                if(_ball->boundingBox().getMaxX() < ((CCSprite *)brick)->boundingBox().getMinX()+SPEED4 ){ //Hit left side of brick
                    _ball_x_direction = -1;
                } else if (_ball->boundingBox().getMinX() > ((CCSprite *)brick)->boundingBox().getMaxX()-SPEED4){ //hit right side of brick
                    _ball_x_direction = 1;
                } else {
                    _ball_y_direction = -1;
                }

                //see if we need to increase the speed
                if (_speed_index < ((CCSprite *)brick)->getTag()){
                    _speed_index++;
                }
            }

        }

        if (_ball->boundingBox().intersectsRect(_paddle->boundingBox() ) ){ //COLLISION: ball/paddle

                if(_ball->boundingBox().getMinY() < _paddle->boundingBox().getMaxY()/2){ //hit side of paddle -- give some lee-way
                    //do nothing
                } else { //Hit top of paddle

                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("paddle.wav"); 

                    _ball_speed = _speeds[_speed_index]; //update SPEED
                    _ball_y_direction = 1;

                    if(_ball->getPosition().x < (_paddle->getPosition().x-_paddle->getContentSize().width/2)+ PADDLEPIX   ) { //if it hits the left 35 pixels of the paddle
                        _angle_multiplier = 1.4;
                        _ball_x_direction = -1;
                    } else if(_ball->getPosition().x > (_paddle->getPosition().x+_paddle->getContentSize().width/2)-PADDLEPIX ){ //if it hits right
                        _angle_multiplier = 1.4;
                        _ball_x_direction = 1;
                    } else {
                        _angle_multiplier = 1;
                    }
                }
        }

        if ( _savior->isVisible() && _ball->boundingBox().intersectsRect(_savior->boundingBox() ) ){ //COLLISION: ball/savior!
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("savior.wav");
            dec_score(SAVIOR_AMOUNT); 
            _speed_index = 0; //RESET
            _ball_speed = _speeds[_speed_index]; //reset
            _ball_y_direction = 1;
            _savior->setVisible(false);

            CCActionInterval* effect = CCBlink::create(1, 10);
            _ball->runAction( effect );

        }

       

        if( (_ball->getPosition().x+_ball->getContentSize().width/2) >= winSize.width){ //right edge
            _ball_x_direction = -1;
                   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("paddle.wav"); 
        }

        if( (_ball->getPosition().x-_ball->getContentSize().width/2) <= 0){ //left edge
            _ball_x_direction = 1;
                   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("paddle.wav"); 
        }

        if( (_ball->getPosition().y+_ball->getContentSize().height/2) >= winSize.height){ //top edge
            if(_speed_index < 3){
                _speed_index++;
                _ball_speed = _speeds[_speed_index]; //Reset ball speed special
            }
            _ball_y_direction = -1;
                   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("paddle.wav"); 
        }

        if( !_savior->isVisible()  && (_ball->getPosition().y-_ball->getContentSize().height/2) <= 0){ //bottom edge
            //_ball_y_direction = 1;
            //_speed_index = 0;
            //_ball_speed = _speeds[_speed_index]; //Reset ball speed
            //_ball->setVisible(false);

            //GAME OVER LOSE
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("losesound.wav"); 
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
                        _won = false;
                        _running = false;
                        _once = true;

        }
            _ball->setPosition(CCPoint(_ball->getPosition().x + (_ball_speed*_ball_x_direction * _angle_multiplier), _ball->getPosition().y + (_ball_speed*_ball_y_direction) ) ); //DEBUG MOVE BALL
     
     }//END IF RUNNING

     if (!_running && _once){
        _once = false;

        _ball->setVisible(false);
        _paddle->setVisible(false);
        _savior->setVisible(false);

        CCARRAY_FOREACH(_bricks, brick){
            ((CCSprite *) brick)->setVisible(false);
        }

        //Won
        if(_won){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("winsong.wav", false);
                    CCLabelTTF *title = CCLabelTTF::create("You WON!", "Silom.ttf", 80, CCSizeMake(winSize.width, 150), kCCTextAlignmentCenter);
            title->setAnchorPoint(ccp(0.5f,0.5f)); 
            title->setPosition(ccp(winSize.width/2, winSize.height-100));
            title->setColor(ccc3(80,80,80)); //Grey
            this->addChild(title, 4);

        } else {
                    CCLabelTTF *title = CCLabelTTF::create("You LOST!", "Silom.ttf", 80, CCSizeMake(winSize.width, 150), kCCTextAlignmentCenter);
            title->setAnchorPoint(ccp(0.5f,0.5f)); 
            title->setPosition(ccp(winSize.width/2, winSize.height-100));
            title->setColor(ccc3(80,80,80)); //Grey
            this->addChild(title, 4);

        }

        CCLabelTTF *gameby = CCLabelTTF::create("Score:", "Silom.ttf", 50, CCSizeMake(winSize.width, 150), kCCTextAlignmentCenter);
        gameby->setAnchorPoint(ccp(0.5f,0.5f)); 
        gameby->setPosition(ccp(winSize.width/2, winSize.height/2 + 50));
        gameby->setColor(ccc3(80,80,80)); //Grey
        this->addChild(gameby, 4);

        _score_label->setColor(ccc3(0,0,0)); //black


        CCUserDefault::sharedUserDefault()->flush ();  //flush before every get
        int highscore = CCUserDefault::sharedUserDefault()->getIntegerForKey("hiscore", -1); //-1 is default val if not exist
        
        if(highscore < _score){
            CCUserDefault::sharedUserDefault()->setIntegerForKey("hiscore", _score); 
            highscore = _score;
        }

        char string[20] = { 0 };
        sprintf(string, "High Score: %d", highscore);

        CCLabelTTF *hiscore = CCLabelTTF::create(string, "Silom.ttf", 50, CCSizeMake(winSize.width, 150), kCCTextAlignmentCenter);
        hiscore->setAnchorPoint(ccp(0.5f,0.5f)); 
        hiscore->setPosition(ccp(winSize.width/2, winSize.height/2 - 100));
        hiscore->setColor(ccc3(80,80,80)); //Grey
        this->addChild(hiscore, 4);

            CCMenuItem *new_game = CCMenuItemImage::create(
                                        "newgame.png",
                                        "newgame.png",
                                        this,
                                        menu_selector(HelloWorld::newGameCallback) );

                new_game->setAnchorPoint(ccp(0.5f,0.5f)); 
                new_game->setPosition(winSize.width/2, 100);
                new_game->setScale(0.5f);

                CCMenu* main_menu = CCMenu::create(new_game, NULL);

                main_menu->setAnchorPoint(ccp(0.5f,0.5f)); 
                main_menu->setPosition(0,0);
                this->addChild(main_menu, 10);
     }


}


//TOUCHES:
void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    //reset touch offset
    _touchOffset = CCPointZero;

    for( CCSetIterator it = touches->begin(); it != touches->end(); it++ ){
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

        convertedLocation.y = SETY;

        if( touch && ( ccpDistance(_paddle->getPosition(), convertedLocation ) < 100.0f) )
        {
            // calculate offset from sprite to touch point
            _touchOffset = ccpSub(_paddle->getPosition(), convertedLocation );
           // _touchOffset.y = 0.0;

            //CCLog("_touchOffset: x: %f | y: %f", _touchOffset.x, _touchOffset.y);
        }
    }

     /*BEFORE IMPLEMENTING SPRITE TOUCH X
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
    */
}

void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* event)
{

    for( CCSetIterator it = touches->begin(); it != touches->end(); it++) 
    {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
        convertedLocation.y = SETY;
        //_touchOffset.y = 0.0;
        //CCLog("_touchOffset: x: %f | y: %f", _touchOffset.x, _touchOffset.y);

        // set the new sprite position
        if( touch && _touchOffset.x ) //&& _touchOffset.y
            _paddle->setPosition( ccpAdd( convertedLocation , _touchOffset) ) ;
    }
    

}


// Method is called after the user is finished touching the screen or they have lifted their finger off the screen.
void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event) {
    
    for( CCSetIterator it = touches->begin(); it != touches->end(); it++) 
    {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
        convertedLocation.y = SETY;
        //_touchOffset.y = 0.0;

        if( touch && _touchOffset.x) //&& _touchOffset.y  
        {
            // set the new sprite position
            _paddle->setPosition( ccpAdd(convertedLocation , _touchOffset) );

            // stop any existing actions are reset the scale
            _paddle->stopAllActions();
            //this->sprite->setScale(1.0f);

            // animate letting go of the sprite
            //this->sprite->runAction(CCSequence::create(
              //  CCScaleBy::create(0.125f, 1.111f),
                //CCScaleBy::create(0.125f, 0.9f),
                //nullptr
                //));
        }
    }
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

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    /////////////////////////////
    // BCY3 code


    _running = true;
    _won = false;

    this->setTouchEnabled(true); //Enable touches on the scene!
    _bricks = new CCArray();//Init array of _bricks

   
    
    //Sprites:
    _background = CCSprite::create("background.png");
    _background->setPosition(ccp(winSize.width/2, winSize.height/2)); //center
   // _background->setSize();
    this->addChild(_background,-2);

    _ball = CCSprite::create("ball.png");
    _ball->setPosition(ccp(100, 100));
    this->addChild(_ball);

    _paddle = CCSprite::create("paddle.png");
    _paddle->setPosition(ccp(winSize.width/2, SETY));
    this->addChild(_paddle);

    _savior = CCSprite::create("savior.png");
    _savior->setPosition(CCPoint(winSize.width/2, 10));
    this->addChild(_savior);


    //INIT score 
    _score = 0;
    _score_label = CCLabelTTF::create("0", "Silom.ttf", 80, CCSizeMake(300, 150), kCCTextAlignmentCenter);
    _score_label->setAnchorPoint(ccp(0.5f,0.5f)); 
    _score_label->setPosition(ccp(winSize.width/2, winSize.height/2));
    _score_label->setColor(ccc3(180,180,180)); //Grey
    
    this->addChild(_score_label, -1);

    //init ball
    _ball_x_direction = 1;
    _ball_y_direction = 1;
    _speed_index = 0;
    _speeds[0] = SPEED1;
    _speeds[1] = SPEED2;
    _speeds[2] = SPEED3;
    _speeds[3] = SPEED4;
    _ball_speed = _speeds[_speed_index];
    _angle_multiplier = 1;

    CCSprite *brick;
    _total_bricks = 0;
    //Top Row
    for(int i = 0; i < 7; i++) {

        brick = CCSprite::create("brick_blue.png");
        brick->setTag(3);

        static int padding= (winSize.width-((brick->getContentSize().width)*7))/8;

        int xOffset = padding+brick->getContentSize().width/2+((brick->getContentSize().width+padding)*i);
        brick->setPosition(ccp(xOffset, winSize.height - brick->getContentSize().height * .5) );
        this->addChild(brick); //Add to screen
        _bricks->addObject(brick); //Add to brick array
        _total_bricks++;
        
    }

    //Middle Row
    for(int i = 0; i < 5; i++) {

        brick = CCSprite::create("brick_orange.png");
        brick->setTag(2);

        static int padding= (winSize.width-((brick->getContentSize().width)*5))/6;

        int xOffset = padding+brick->getContentSize().width/2+((brick->getContentSize().width+padding)*i);
        brick->setPosition(ccp(xOffset, winSize.height - brick->getContentSize().height * 1.5) ); //set row in relation to top row
        this->addChild(brick); //Add to screen
        _bricks->addObject(brick); //Add to brick array
        _total_bricks++;
        
    }

    //Bottom Row
    for(int i = 0; i < 6; i++) {
        
        brick = CCSprite::create("brick_purple.png");
        brick->setTag(1);

        static int padding= (winSize.width-((brick->getContentSize().width)*6))/7;

        int xOffset = padding+brick->getContentSize().width/2+((brick->getContentSize().width+padding)*i);
        brick->setPosition(ccp(xOffset, winSize.height - brick->getContentSize().height * 2.5)); //set row in relation to the top of the screen and the size of the _bricks
        this->addChild(brick); //Add to screen
        _bricks->addObject(brick); //Add to brick array
        _total_bricks++;
        
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


