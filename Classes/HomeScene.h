#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "cocos2d.h"

class Home : public cocos2d::CCLayer
{

private: 

	cocos2d::CCSprite *_background;
	

public:

	void update(float dx);
	cocos2d::CCSprite *_ball;
	int _ball_x_direction; // + = right | - = left
	int _ball_y_direction; // + = up | - = down
    


    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void newGameCallback(CCObject* pSender);
    void settingsCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(Home);
};

#endif // __HOME_SCENE_H__
