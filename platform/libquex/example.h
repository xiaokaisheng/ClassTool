#pragma once
#include <QPainterPath>
#include <qglobal.h>

#include "measurestatus.h"
#include "measuremanagerproxy.h"

QT_FORWARD_DECLARE_CLASS(QPoint);
QT_FORWARD_DECLARE_CLASS(QPainter);
QT_FORWARD_DECLARE_CLASS(QImage);
QT_FORWARD_DECLARE_CLASS(QEvent);

#if defined(CONF_PRODUCT_SERIES_US66)
#define DEFAULT_DISTANCE 100
#else
#define DEFAULT_DISTANCE 225
#endif // DEBUG


class MeasureAction
{
public:
    enum Enum {
        MouseLeftButtonDown,
        MouseLeftButtonUp,
        MouseRightButtonDown,
        MouseRightButtonUp,
        TouchBegin,
        TouchEnd,
        ShortTouchReleaseEnd
    };
};

class RulerView;
class MeasureImageDescriptor;
class MeasureCineData;
class UltrasoundMachine;
class MeasureMethodView
{
public:
    MeasureMethodView();
    virtual ~MeasureMethodView() {}

    virtual void enter(RulerView* view) = 0;
    virtual void exit() = 0;
    virtual void paint(QPainter* painter) = 0;
    virtual QRegion getClipRegion() const;
	bool isTouchOn() const
	{
		return touchOn;
	}
    void setTouchOn(bool touch_on);
    virtual bool processCustomEvent(QEvent* event);
    virtual bool onMouseMove(int x, int y) = 0;
    virtual bool onMeasureAction(MeasureAction::Enum action) = 0;
    virtual bool onMeasureAction(MeasureAction::Enum action, int x, int y)
    {
        Q_UNUSED(x);
        Q_UNUSED(y);
        Q_UNUSED(action);
        return false;
    };
    virtual bool onTouchMove(int x, int y)
    {
        Q_UNUSED(x);
        Q_UNUSED(y);
        return false;
    }
	virtual bool onTouchMove(int x1, int y1, int x2, int y2)
	{
		Q_UNUSED(x1);
		Q_UNUSED(y1);
		Q_UNUSED(x2);
		Q_UNUSED(y2);
		return false;
	}
    virtual bool hasResult() = 0;
    virtual bool getResult(const char* result_id, float& value) = 0;
    virtual void cursorTouchMove(int x, int y);
    QPoint getOffsetPoint() { return offsetPoint; }

    //状态的保存和加载
    virtual bool saveCine(const char* path, MeasureCineData& data) = 0;
    virtual bool loadCine(const char* path, const MeasureCineData& data) = 0;
    virtual bool autoCompleteRuler();
	virtual void setRulerViewByPoints(QVector<QPointF> points) {}
	virtual bool getCurPoint(QPoint&) { return false; }

    MeasureStatus::Enum getStatus();
	void setStatus(MeasureStatus::Enum state);
    MeasureImageDescriptor* getImageDescriptor();

protected:
    MeasureStatus::Enum measureStatus;
    RulerView* rulerView;
    MeasureImageDescriptor* imageDescriptor;
    QPoint offsetPoint;
    QPoint movePoint;
    MeasureManagerProxy* measMngPro;
    UltrasoundMachine* ultrasoundMachine;
private:
	bool touchOn = false;
};

class MeasureMethod;
class MeasureMethodViewFactory
{
public:
    static MeasureMethodView* create(MeasureMethod* method, bool using_touch_screen);
};

#define MAutoMeaColor QColor("#E0BF1B")
#define MAutoMeaActiveColor QColor("#4EF56A")