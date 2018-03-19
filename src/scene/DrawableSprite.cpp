#include "DrawableSprite.h"
#include "util/VisibleRect.h"
#include "geometry/recognizer/GeometricRecognizerNode.h"
USING_NS_CC;

using namespace DollarRecognizer;

#define SCALE 2
#define VEC2_TO_POINT2D(vec2, scale) Point2D(vec2.x/(scale), vec2.y/(scale))
#define NORMALIZE_VEC2_TO_POINT2D(vec2, scale) Point2D((vec2.x-_xMin)/(scale), (vec2.y-_yMin)/(scale))
#define POINT2D_TO_VEC2(p2d) Vec2((p2d).x, (p2d).y)

DrawableSprite::DrawableSprite()
:_xMin(VisibleRect::width())
, _xMax(0)
, _yMin(VisibleRect::height())
, _yMax(0)
, _reference(nullptr)
, _brushColor(Color4F::WHITE)
{
	_lineWidth = 3;
}

void DrawableSprite::addToPath(Vec2 from, Vec2 to)
{
	// add two new point to current path
	this->_path.push_back(from); this->_path.push_back(to);

	// calculate new bary center with 2 new points
	_baryCenter = (_baryCenter*(_path.size() / 2 - 1) + (from + to) / 2) / (_path.size() / 2);

	// calculate new content border rectangle
	_xMin = MIN(MIN(from.x, to.x), _xMin);
	_xMax = MAX(MAX(from.x, to.x), _xMax);
	_yMin = MIN(MIN(from.y, to.y), _yMin);
	_yMax = MAX(MAX(from.y, to.y), _yMax);
}

void DrawableSprite::redraw()
{
	for (auto p = _path.begin(); p != _path.end(); p+=2)
	{
		this->drawLine(*p, *(p + 1), _brushColor);
	}
}

DrawableSprite* DrawableSprite::createWithMultiStrokeGesture(const MultiStrokeGesture& multiStrokes)
{
	// convert MultiStrokeGesture to path
	// flatten stroke gesture, split joint point into 2 point
	DrawableSprite *pRet = new(std::nothrow) DrawableSprite();
	if (pRet && pRet->init())
	{
		for (auto path = multiStrokes.begin(); path != multiStrokes.end(); path++)
		{
			if (!path->empty())
			{
				auto startLoc = POINT2D_TO_VEC2(*(path->begin()));
				for (auto p = path->begin() + 1; p != path->end(); p++)
				{
					auto endLoc = POINT2D_TO_VEC2(*p);
					pRet->addToPath(startLoc, endLoc);
					startLoc = endLoc;
				}
			}
		}
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

MultiStrokeGesture& DrawableSprite::getMultiStrokeGesture(MultiStrokeGesture& multiStrokes)
{
	// convert path to MultiStrokeGesture
	// check current point is connect to next point in the path
	// true if point in current stroke, false make a new stroke
	Vec2 prev = *(_path.begin());
	Path2D path;
	path.push_back(NORMALIZE_VEC2_TO_POINT2D(prev, SCALE));
	for (auto p = _path.begin(); p != _path.end(); p += 2)
	{
		Vec2 start = *p, end = *(p + 1);
		if (start == prev)
		{
			path.push_back(NORMALIZE_VEC2_TO_POINT2D(prev, SCALE));
		}
		else
		{
			path.push_back(NORMALIZE_VEC2_TO_POINT2D(prev, SCALE));
			multiStrokes.push_back(path);
			path.clear();
			path.push_back(NORMALIZE_VEC2_TO_POINT2D(start, SCALE));
		}

		prev = end;
	}
	multiStrokes.push_back(path);
	return multiStrokes;
}

RecognitionResult DrawableSprite::recognize()
{
	
	if (!this->_path.empty())
	{
		log("Begin inner recognize");
		MultiStrokeGesture multiStrokes;
		getMultiStrokeGesture(multiStrokes);
		_geoRecognizer = new GeometricRecognizer();
		//GeometricRecognizer* w = new GeometricRecognizer();
		_geoRecognizer->loadTemplates();
		vector<string> gesturesList;
		gesturesList.push_back("Circle");
		gesturesList.push_back("Caret");
		gesturesList.push_back("CheckMark");
		gesturesList.push_back("Arrow");
		//gesturesList.push_back("Delete");
		gesturesList.push_back("Diamond");
		gesturesList.push_back("Pigtail");
		gesturesList.push_back("QuestionMark");
		gesturesList.push_back("Rectangle");
		gesturesList.push_back("Spiral");
		gesturesList.push_back("Triangle");
		gesturesList.push_back("Star");
		gesturesList.push_back("0");
		_geoRecognizer->activateTemplates(gesturesList);
		//_geoRecognizer->loadTemplates();
		Path2D GLpath;
		for (int s = 0; s < multiStrokes.size(); s++) {
			Path2D tmp = multiStrokes.at(s);
			for (int p = 0; p < tmp.size(); p++) {
				Point2D pt = tmp.at(p);
				/*log("asd");
				char str[256];
				sprintf(str, "%lf", pt.x);
				string result = str;
					log(str);*/
				GLpath.push_back(pt);
			}
		}
		
		//RecognitionResult result = _geoRecognizer->Multirecognize(multiStrokes, "normal");
		RecognitionResult result = _geoRecognizer->recognize(GLpath);
		log("Recognized gesture: %s, Score: %f", result.name.c_str(), result.score);
		return result;
	}
	return RecognitionResult("", 0);
}

Texture2D* DrawableSprite::createTexture()
{
	// use RenderTexture to generate Texture2D instance
	auto _target = RenderTexture::create(VisibleRect::width(), VisibleRect::height(), Texture2D::PixelFormat::RGBA8888);
	_target->begin();
	this->visit();
	_target->end();
	return _target->getSprite()->getTexture();
}

bool DrawableSprite::containsPoint(const Vec2& point)
{
	bool hasMin = false, hasMax = false;

	// compare (x, y) with all points in path
	// true if find min =< (x, y) <= max, false otherwise
	for (auto p = _path.begin(); p != _path.end(); p ++)
	{
		hasMin |= p->x <= point.x&&p->y <= point.y;
		hasMax |= p->x >= point.x&&p->y >= point.y;
		if (hasMin&&hasMax)return true;
	}
	return false;
}