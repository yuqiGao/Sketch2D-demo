#include "CanvasLayer.h"
#include "util/VisibleRect.h"
#include "gesture/GeometricRecognizer.h"

USING_NS_CC;

void CanvasLayer::onEnter()
{
	Layer::onEnter();

	_canvasSize = VisibleRect::size();

	_mouseListener = EventListenerMouse::create();
	_mouseListener->onMouseDown = CC_CALLBACK_1(CanvasLayer::onMouseDown, this);
	_mouseListener->onMouseMove = CC_CALLBACK_1(CanvasLayer::onMouseMove, this);
	_mouseListener->onMouseUp = CC_CALLBACK_1(CanvasLayer::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

	_isDrawing = false;
}

void CanvasLayer::onMouseDown(EventMouse* event)
{
	// if current status is not drawing, mark status as drawing
	// save first location as current mouse location in view
	if (!_isDrawing)
	{
		_startDrawLocation = convertToNodeSpace(event->getLocationInView());
		_isDrawing = true;
	}
	//int X = (int)event->getLocationInView().x;
	//int Y = (int)event->getLocationInView().y;
	//cocos2d::log("X: %d, Y: %d\n",X ,Y );
	//this->path.push_back(Point2D((int)event->getLocationInView().x, (int)event->getLocationInView().y));
	
}

void CanvasLayer::onMouseMove(EventMouse* event)
{
	if (_isDrawing)
	{
		auto currentLocation = convertToNodeSpace(event->getLocationInView());
		_currentDrawNode->drawLine(_startDrawLocation, currentLocation, _currentDrawNode->getBrushColor());
		_currentDrawNode->addToPath(_startDrawLocation, currentLocation);
		_startDrawLocation = currentLocation;
		//int X = (int)event->getLocationInView().x;
		//int Y = (int)event->getLocationInView().y;
		//cocos2d::log("X: %d, Y: %d\n", X, Y);
		//cocos2d::log("X: %f, Y: %f\n", event->getLocationInView().x, event->getLocationInView().y);

	}
}

void CanvasLayer::onMouseUp(EventMouse* event)
{
	// mark status is not drawing when mouse up
	//This by YQ 2018/1/3
	//All push_back is modified
	//log("s");
	//auto _geoRecognizer = new GeometricRecognizer();
	//_currentDrawNode->getPath()
	//RecognitionResult result = _geoRecognizer->recognize();
	//log("%s", result.name);
	//_currentDrawNode->clear();
	_isDrawing = false;
}
