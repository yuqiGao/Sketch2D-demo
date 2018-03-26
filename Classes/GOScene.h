#ifndef __GO_SCENE_H__
#define __GO_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class GOScene : public cocos2d::Layer
{
public:

	virtual bool init();

	static cocos2d::Scene* createScene();
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(GOScene);

	//virtual void onEnter();
	bool onContactBegin(const PhysicsContact& contact);
};

#endif // __GO_SCENE_H__