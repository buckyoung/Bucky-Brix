#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{

private:

	cocos2d::CCSprite *_background;
	cocos2d::CCSprite *_ball;
	cocos2d::CCSprite *_paddle;
	//cocos2d::CCSprite *_savior;
	cocos2d::CCArray *_bricks;

	int _ball_x_direction; // + = right | - = left
	int _ball_y_direction; // + = up | - = down
	int _ball_speed;
	int _speed_index;
	int _speeds[4];
	int _angle_multiplier;

	cocos2d::CCPoint _touchOffset;

	void update(float dx);
	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);


	


public:

	

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
