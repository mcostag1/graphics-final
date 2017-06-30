#ifndef MYPANELOPENGL_H
#define MYPANELOPENGL_H

#include <QtWidgets>
#include <QtOpenGL>
#include <QtMultimedia>
#include "kiss_fft130/kiss_fft.h"
#include "kiss_fft130/tools/kiss_fftr.h"

struct bar{
	QVector4D vdisp;
	float hdisp;
	QVector4D color = QVector4D(0, 0, 0, 0);

};

class MyPanelOpenGL : public QOpenGLWidget
{
	Q_OBJECT

	protected:
		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();

	public:
		explicit MyPanelOpenGL(QWidget *parent = 0);
		virtual ~MyPanelOpenGL();

	private:

		void createVBOs();
		void destroyVBOs();
		void createShaders();
		void destroyShaders();


		unsigned int numVertices;
		QVector4D *vertices;
		QVector4D displacement;
		int numBars = 40;
		bar bars[40];
		QVector4D displacements[240];
		float spacing = .4/numBars;
		float barSize = 2.0/numBars;
		int one_seventh = numBars/7;
		int nframes = 1152;
		float buf[2000];

		QTimer* timer;
		float phase;

		QMediaPlayer* m_play;
		QAudioProbe* m_probe;


		/* VAO required in core profile */
		QOpenGLVertexArrayObject *vao;
		QOpenGLBuffer *vboVertices;

		QOpenGLShader *vertexShader;
		QOpenGLShader *fragmentShader;
		QOpenGLShaderProgram *shaderProgram;

signals:

		public slots:
			/* update scene */
			void step();
			void processBuffer(QAudioBuffer buff);

};

#endif // MYPANELOPENGL_H
