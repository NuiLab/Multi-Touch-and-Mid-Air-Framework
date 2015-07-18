#ifndef __QFLIGHTINSTRUMENTS_H__
#define __QFLIGHTINSTRUMENTS_H__

#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QMap>
#include <QTableWidget>
#include <QtOpenGL>
#include <QGLWidget>

////////////////////////////////////////////////////////////////////////////////
// 
//	github.com/bushuhui/qFlightInstruments
//
////////////////////////////////////////////////////////////////////////////////

///
/// The Attitude indicator class
///
class QADI : public QOpenGLWidget
{
    Q_OBJECT

public:
    QADI(QWidget *parent = 0);
    ~QADI();

    ///
    /// Set roll & pitch values (in degree)
    /// r - roll
    /// p - pitch
    ///
    void setData(double r, double p) {
        m_roll = r;
        m_pitch = p;
        if( m_roll < -180 ) m_roll = -180;
        if( m_roll > 180  ) m_roll =  180;
        if( m_pitch < -90 ) m_pitch = -90;
        if( m_pitch > 90  ) m_pitch =  90;

        emit canvasReplot();
    }

    ///
    /// Set roll angle (in degree)
    /// val - roll
    ///
    void setRoll(double val) {
        m_roll  = val;
        if( m_roll < -180 ) m_roll = -180;
        if( m_roll > 180  ) m_roll =  180;

        emit canvasReplot();
    }

    ///
    /// Set pitch value (in degree)
    /// val - pitch
    ///
    void setPitch(double val) {
        m_pitch = val;
        if( m_pitch < -90 ) m_pitch = -90;
        if( m_pitch > 90  ) m_pitch =  90;

        emit canvasReplot();
    }

    ///
    /// Get roll angle (in degree)
    /// roll angle
    ///
    double getRoll() {return m_roll;}

    ///
    /// Get pitch angle (in degree)
    /// pitch angle
    ///
    double getPitch(){return m_pitch;}


signals:
    void canvasReplot(void);

protected slots:
    void canvasReplot_slot(void);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
	void initializeGL();
    void keyPressEvent(QKeyEvent *event);

protected:
    int     m_sizeMin, m_sizeMax;           ///< widget's min/max size (in pixel)
    int     m_size, m_offset;               ///< current size & offset

    double  m_roll;                         ///< roll angle (in degree)
    double  m_pitch;                        ///< pitch angle (in degree)
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///
/// The Compass & altitude display class
///
class QCompass : public QWidget
{
    Q_OBJECT

public:
    QCompass(QWidget *parent = 0);
    ~QCompass();

    ///
    /// Set all data (yaw, alt, height)
    ///
    /// y - yaw ( in degree)
    /// a - altitude ( in m)
    /// h - height from ground (in m)
    ///
    void setData(double y, double a, double h) {
        m_yaw = y;
        m_alt = a;
        m_h   = h;

        if( m_yaw < 0   ) m_yaw = 360 + m_yaw;
        if( m_yaw > 360 ) m_yaw = m_yaw - 360;

        emit canvasReplot();
    }

    ///
    /// Set yaw angle (in degree)
    /// val - yaw angle (in degree)
    ///
    void setYaw(double val) {
        m_yaw  = val;
        if( m_yaw < 0   ) m_yaw = 360 + m_yaw;
        if( m_yaw > 360 ) m_yaw = m_yaw - 360;

        emit canvasReplot();
    }

    ///
    /// Set altitude value
    /// val - altitude (in m)
    ///
    void setAlt(double val) {
        m_alt = val;

        emit canvasReplot();
    }

    ///
    /// Set height from ground
    /// val - height (in m)
    ///
    void setH(double val) {
        m_h = val;

        emit canvasReplot();
    }

    ///
    /// Get yaw angle
    /// yaw angle (in degree)
    ///
    double getYaw() {return m_yaw;}

    ///
    /// Get altitude value
    /// altitude (in m)
    ///
    double getAlt() {return m_alt;}

    ///
    /// Get height from ground
    /// height from ground (in m)
    ///
    double getH()   {return m_h;}

signals:
    void canvasReplot(void);

protected slots:
    void canvasReplot_slot(void);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

protected:
    int     m_sizeMin, m_sizeMax;               ///< widget min/max size (in pixel)
    int     m_size, m_offset;                   ///< widget size and offset size

    double  m_yaw;                              ///< yaw angle (in degree)
    double  m_alt;                              ///< altitude (in m)
    double  m_h;                                ///< height from ground (in m)
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///
/// The accelerator gauge class
///
class QAccel : public QWidget
{
	Q_OBJECT

public:
	QAccel(QWidget *parent = 0);
	~QAccel();

	///
	/// Set acceleration data
	///
	/// accel - acceleration in m/s^2
	///
	void setData(double accel) {
		m_accel = accel;

		if (m_accel < -4) m_accel = -4; 
		if (m_accel > 4) m_accel = 4;

		emit canvasReplot();
	}

	void setAxisAccel(double Ax, double Ay, double Az)
	{
		m_ax = Ax; 
		m_ay = Ay; 
		m_az = Az; 

		emit canvasReplot();
	}

	///
	/// Get acceleration
	/// acceleration and acceleration components
	///
	double getAccel() { return m_accel; }

	double getAxisX() { return m_ax; }

	double getAxisY() { return m_ay; }

	double getAxisZ() { return m_az; }

signals:
	void canvasReplot(void);

protected slots:
	void canvasReplot_slot(void);

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void keyPressEvent(QKeyEvent *event);

protected:
	int     m_sizeMin, m_sizeMax;               ///< widget min/max size (in pixel)
	int     m_size, m_offset;                   ///< widget size and offset size

	double  m_accel;							///< acceleration (in m/s^2)
	double	m_ax = 0.0; 
	double	m_ay = 0.0;
	double	m_az = 0.0;
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef QMap<QString, QString> ListMap;

///
/// The List view class, it will display key-value pair in lines
///
class QKeyValueListView : public QTableWidget
{
public:
    Q_OBJECT

public:
    QKeyValueListView(QWidget *parent = 0);
    virtual ~QKeyValueListView();

    ///
    /// \Set list data
    /// d - list data
    ///
    void setData(ListMap &d) {
        m_data = d;
        emit listUpdate();
    }

    ///
    /// Get list data
    /// d - list data obj
    ///
    ListMap& getData(void) {
        return m_data;
    }

    void beginSetData(void) {
        m_mutex->lock();
    }

    void endSetData(void) {
        m_mutex->unlock();
    }

    ///
    /// Reload data to table widget
    ///
    void listReload(void) {
        emit listUpdate();
    }

signals:
    void listUpdate(void);

protected slots:
    void listUpdate_slot(void);

protected:
    ListMap         m_data;
    QMutex          *m_mutex;
};

#endif
