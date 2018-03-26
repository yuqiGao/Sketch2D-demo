#include "GOScene.h"
#include "HelloWorldScene.h"



Scene* GOScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	//auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	// 'layer' is an autorelease object
	auto layer = GOScene::create();
	//layer->setPhyWorld(scene->getPhysicsWorld());
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GOScene::init()
{
	//////////////////////////////


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->setKeyboardEnabled(true);

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	//physics world init


	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GOScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 200);
	//
	// position the label on the center of the screen
	//label->setPosition(Vec2(origin.x + visibleSize.width / 2,
	//	origin.y + visibleSize.height - label->getContentSize().height));
	label->setPosition(Vec2(visibleSize.width / 2,
		origin.y + visibleSize.height/2 + 50));

	auto label2 = Label::createWithTTF("Try again", "fonts/Marker Felt.ttf", 40);
	//
	// position the label on the center of the screen
	//label->setPosition(Vec2(origin.x + visibleSize.width / 2,
	//	origin.y + visibleSize.height - label->getContentSize().height));
	label2->setPosition(Vec2(visibleSize.width / 2,
		visibleSize.height / 2 - 50  ));

	// add the label as a child to this layer
	this->addChild(label, 1);
	this->addChild(label2, 1);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [label2](Touch *t, Event *e){
		if (label2->cocos2d::Node::getBoundingBox().containsPoint(t->getLocation()))
		{//cocos2d::Node::
			Scene* scene = HelloWorld::createScene();
			Director::getInstance()->replaceScene(scene);
		}
		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, label2);

	// add "HelloWorld" splash screen"
	//auto sprite = Sprite::create("HelloWorld.png");

	// position the sprite on the center of the screen
	//sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));


	//New Sprite Support Hit & Control

	// add the sprite as a child to this layer
	//this->addChild(sprite, 0);
	//this->addChild(board, 0);

	return true;
}


void GOScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
