#pragma once

#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/CompositeViewer>


class AuxiliaryViewUpdater : public osgGA::GUIEventHandler
{
public:
	AuxiliaryViewUpdater()
		: _distance(-1.0), _offsetX(0.0f), _offsetY(0.0f),
		_lastDragX(-1.0f), _lastDragY(-1.0f)
	{}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osgViewer::View* view = static_cast<osgViewer::View*>(&aa);
		if (view)
		{
			auto eventTyp = ea.getEventType();

			if (eventTyp != osgGA::GUIEventAdapter::FRAME)
			{
				int a = 2;
				a = a*a;


			}

			switch (ea.getEventType())
			{
			case osgGA::GUIEventAdapter::SCROLL:
			{

			}
			break;
			case osgGA::GUIEventAdapter::SCROLL_DOWN:
			{

			}
			break;
			case osgGA::GUIEventAdapter::SCROLL_UP:
			{

			}
			break;



			case osgGA::GUIEventAdapter::PUSH:
				_lastDragX = -1.0f;
				_lastDragY = -1.0f;
				break;
			case osgGA::GUIEventAdapter::DRAG:
				if (_lastDragX > 0.0f && _lastDragY > 0.0f)
				{
					if (ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
					{
						_offsetX += ea.getX() - _lastDragX;
						_offsetY += ea.getY() - _lastDragY;
					}
					else if (ea.getButtonMask() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
					{
						float dy = ea.getY() - _lastDragY;
						_distance *= 1.0 + dy / ea.getWindowHeight();
						if (_distance < 1.0) _distance = 1.0;
					}
				}
				_lastDragX = ea.getX();
				_lastDragY = ea.getY();
				break;
			case osgGA::GUIEventAdapter::FRAME:
				if (view->getCamera())
				{
					osg::Vec3d eye, center, up;
					view->getCamera()->getViewMatrixAsLookAt(eye, center, up);

					osg::Vec3d lookDir = center - eye; lookDir.normalize();
					osg::Vec3d side = lookDir ^ up; side.normalize();

					const osg::BoundingSphere& bs = view->getSceneData()->getBound();
					if (_distance < 0.0) _distance = bs.radius() * 3.0;
					center = bs.center();

					center -= (side * _offsetX + up * _offsetY) * 0.1;
					view->getCamera()->setViewMatrixAsLookAt(center - lookDir*_distance, center, up);
				}
				break;
			}
		}
		return false;
	}

protected:
	double _distance;
	float _offsetX, _offsetY;
	float _lastDragX, _lastDragY;
};


