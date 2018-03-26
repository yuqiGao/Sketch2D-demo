#include "HelloWorldScene.h"
#include "GOScene.h"
#include "geometry/recognizer/GeometricRecognizerNode.h"
//#include "scene/SceneManager.h"
#include <fstream>
using namespace DollarRecognizer;
#include "stdlib.h"

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    //auto scene = Scene::create();
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
	layer-> setPhyWorld(scene->getPhysicsWorld());
    // add layer as a child to scene
    scene->addChild(layer,1);

	layer->_canvasLayer = GameCanvasLayer::create();
	//_canvasLayer->switchToNewDrawNode();
	//auto layerdraw = TrainingCanvasLayer::create();
	//auto _geoRecognizer = GeometricRecognizerNode::create();
	//layerdraw ->addChild(_geoRecognizer);
	//layerdraw->getDrawNode()->recognize();
	//layerdraw->_drawNodeList.push_back(drawNode);
	//auto _gameLayer = layerdraw->createGameLayer();
	//_gameLayer->setVisible(true);
	//layerdraw->startGameSimulation();
	//layerdraw->stopGameSimulation();
	//layerdraw->setVisible(false);
	//layerdraw->stopAllActions();
	scene->addChild(layer->_canvasLayer, 2);
	//scene->addChild(_gameLayer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//this->setKeyboardEnabled(true);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

	//physics world init


    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    //auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    //
    //// position the label on the center of the screen
    //label->setPosition(Vec2(origin.x + visibleSize.width/2,
    //                        origin.y + visibleSize.height - label->getContentSize().height));

    //// add the label as a child to this layer
    //this->addChild(label, 1);
	 
    // add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");

    //// position the sprite on the center of the screen
    //sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));


	//New Sprite Support Hit & Control
	this->board = Sprite::create("board.png");
	board->setPosition(Vec2(visibleSize.width / 2 + origin.x, 13.5));
	board->setScaleX(1.5);//
	//board->setScaleY(0.1);//

	//physics body
	auto physicsBody = PhysicsBody::createBox(Size(board->getContentSize().width, board->getContentSize().height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	board->addComponent(physicsBody);

    // add the sprite as a child to this layer
    //this->addChild(sprite, 0);
	this->addChild(board, 0);
    

	//keyboard control
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// Implementation of the keyboard event callback function prototype



	ballOne = Sprite::create("Ball.jpg");
	ballOne->setPosition(200, 200);
	ballOne->setScaleX(0.5);//100
	ballOne->setScaleY(0.5);//100
	PhysicsBody* ballBodyOne = PhysicsBody::createCircle(ballOne->getContentSize().width / 2, PHYSICSBODY_MATERIAL_DEFAULT);
	//ballBodyOne->setDynamic(false);
	ballBodyOne->getShape(0)->setRestitution(1.0f);
	ballBodyOne->getShape(0)->setFriction(0.0f);
	ballBodyOne->getShape(0)->setDensity(1.0f);
	//ballBodyOne->getShape(0)->setMass(5000);
	ballBodyOne->setGravityEnable(false);
	//ballBodyOne->setCategoryBitmask(0x0001);
	//ballBodyOne->setCollisionBitmask(0x0001);
	//ballBodyOne->setContactTestBitmask(0x0001);
	ballBodyOne->setContactTestBitmask(0x0001);
	//设置物体的冲力
	Vect force = Vect(500000.0f, 500000.0f);

	//把物体添加到精灵中
	ballOne->setPhysicsBody(ballBodyOne);
	//设置标志
	ballOne->setTag(1);
	this->addChild(ballOne,1);
	ballBodyOne->applyImpulse(force);
	//ballOne->runAction(RepeatForever::create(
	//	                 Sequence::create(MoveTo::create(1.0f,Vec2(300,200)),
	//									MoveTo::create(1.0f,Vec2(300,400)),
	//									MoveTo::create(1.0f,Vec2(200,400)),
	//									MoveTo::create(1.0f,Vec2(200,200)),
	//									NULL)));

	//创建一个盒子，用来碰撞
	Sprite* edgeSpace = Sprite::create();
	//PhysicsBody* boundBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(0.0f, 1.0f, 0.0f), 3);
	boundBody->getShape(0)->setFriction(0.0f);
	boundBody->getShape(0)->setRestitution(1.0f);
	//boundBody->setCategoryBitmask(0x0011);
	//boundBody->setCollisionBitmask(0x0011);
	//boundBody->setContactTestBitmask(0x0);
	edgeSpace->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	edgeSpace->setPhysicsBody(boundBody);
	edgeSpace->getPhysicsBody()->setContactTestBitmask(0x0001);
	
	this->addChild(edgeSpace);
	edgeSpace->setTag(10);

	createbricks();

	//GeometricRecognizer* w = new GeometricRecognizer();
	//w->loadTemplates();
	//vector<string> gesturesList;
	//gesturesList.push_back("Circle");
	//gesturesList.push_back("Delete");
	//gesturesList.push_back("Arrow");
	//gesturesList.push_back("0");
	//w->activateTemplates(gesturesList);
	//RecognitionResult result = w->recognize(path);

	//log("Recognized gesture: %s, Score: %f", result.name.c_str(), result.score);

    return true;
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	//log("Key with keycode %d pressed", keyCode);

	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW )
	{
		Speed* action = Speed::create(MoveBy::create(2,Vec2(-1000, 0)), 1);
		action->setTag(4);
		//auto moveBy = MoveBy::create(2, Vec2(-100, 0));
		//auto seq = Sequence::create(moveBy, nullptr);
		this->board->runAction(action);
		
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		Speed* action = Speed::create(MoveBy::create(2, Vec2(1000, 0)), 1);
		action->setTag(6);
		this->board->runAction(action);
		
	}
	if (EventKeyboard::KeyCode::KEY_ENTER == keyCode){
		this->_canvasLayer->recognize();
		string name = "";
		int x = 0;
		int y = 0;
		int index = 0;
		ifstream file;
		file.open("D://fish.txt");
		string strLine;
		while (getline(file, strLine))
		{
			if (index == 0){
				name = strLine;
			}
			if (index == 1){
				x = atoi(strLine.c_str());
			}
			if (index == 2){
				y = atoi(strLine.c_str());
			}
			index++;
			log("%s", strLine.c_str());
		}
		int a = name.find(":");
		name = name.substr(0, a);
		generateObstacle(x,y,name,100);
		file.close();
	}


}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	//log("Key with keycode %d released", keyCode);
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		this->board->stopActionByTag(4);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		this->board->stopActionByTag(6);
	}
}

bool HelloWorld::onContactBegin(const PhysicsContact& contact)
{
	
	Sprite* spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	Sprite* spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
	if (!spriteA || !spriteA)
		return true;
	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();
	if (tagA == 1 && tagB == 10 || tagA == 10 && tagB == 1)
	{
		//log("Ball at %f or %f", spriteA->getPositionY(), spriteB->getPositionY());
		if (spriteA->getPositionY()<(float)5 || spriteB->getPositionY()<(float)5)
		{
			//Director::getInstance()->end();
			Scene* scene = GOScene::createScene();
			//Scene* scene = SceneManager::GetMenuScene();
			this->_canvasLayer->stopAllActions();
			log("GO");
			this->cleanup();
			Director::getInstance()->pushScene(scene);
		}
	}
	

	
	return true;
}

void HelloWorld::onContactSeparate(const PhysicsContact& contact)
{

	Sprite* spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	Sprite* spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
	if (!spriteA || !spriteB)
		return;
	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();
	
	if (tagA == 13|| tagA == 100){
		spriteA->removeFromParentAndCleanup(true);
		//log("Ball Clear Brick at %f", spriteA->getPositionY());
	}
	
	if (tagB == 13||tagB == 100){
		spriteB->removeFromParentAndCleanup(true);
		//log("Ball Clear Brick at %f", spriteB->getPositionY());
	}



	//return true;
}

void HelloWorld::createbricks(){

	for (size_t line = 1; line <= 3; line++){
		for (size_t i = 0; i < 100; i++)
		{

			int index = (rand() % (4)) + 1;
			Size visibleSize = Director::getInstance()->getVisibleSize();
			Sprite* brick = NULL;
			switch (index)
			{
			case 1: brick = Sprite::create("yellow.png"); break;
			case 2: brick = Sprite::create("red.png"); break;
			case 3: brick = Sprite::create("green.png"); break;
			case 4: brick = Sprite::create("blue.png"); break;
			default:
				break;
			}
			if ((i + line)*brick->getContentSize().width>visibleSize.width - brick->getContentSize().width*line){
				break;
			}
			PhysicsBody* body = PhysicsBody::createEdgeBox(brick->getContentSize(), PhysicsMaterial(1.0f, 1.0f, 0.0f));
			brick->setTag(13);
			body->setContactTestBitmask(0xFFFFFFFF);
			brick->setPosition((i + line)*brick->getContentSize().width, visibleSize.height - brick->getContentSize().height*line);
			brick->setPhysicsBody(body);
			this->addChild(brick);

		}
	}
	
}

void HelloWorld::onEnter()
{
	log("Enter physics world");
	Layer::onEnter();
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(HelloWorld::onContactSeparate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

}


void HelloWorld::generateObstacle(double x, double y, string name, int tagNumber)
{
	name += ".jpg";
	Sprite* add_item = Sprite::create(name);
	add_item->setPosition(x, y);
	PhysicsBody* ballBodyOne = PhysicsBody::createCircle(add_item->getContentSize().width / 2, PHYSICSBODY_MATERIAL_DEFAULT);

	ballBodyOne->setDynamic(true);
	ballBodyOne->getShape(0)->setRestitution(1.0f);
	ballBodyOne->getShape(0)->setFriction(0.0f);
	ballBodyOne->getShape(0)->setDensity(1.0f);
	ballBodyOne->setGravityEnable(true);
	ballBodyOne->setCategoryBitmask(0x0001);
	ballBodyOne->setCollisionBitmask(0x0001);
	ballBodyOne->setContactTestBitmask(0x0001);
	Vect force = Vect(1000000.0f, 1000000.0f);
	add_item->setPhysicsBody(ballBodyOne);
	add_item->setTag(tagNumber);
	this->addChild(add_item);
	ballBodyOne->applyImpulse(force);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
