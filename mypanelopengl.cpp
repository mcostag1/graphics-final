#include "mypanelopengl.h"
#include <cmath>
#include "RtAudio.h"
#include <iostream>
#include <string>
using namespace std;



MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
	QOpenGLWidget(parent) {

    timer = NULL;
    m_play = NULL;
		m_probe = NULL;
		phase = 0.;


		shaderProgram=NULL;
		vertexShader=NULL;
		fragmentShader=NULL;
		vboVertices = NULL;
		vao = NULL;

		numVertices = 4;
		vertices = new QVector4D[4];

		vertices[0] = QVector4D(-1, -20,0., 1.);
		vertices[1] = QVector4D(-1, .01, 0., 1.);
		vertices[2] = QVector4D(-.8, -20, 0., 1.);
		vertices[3] = QVector4D(-.8, .01, 0., 1.);
		float inc = -1;
		float r = 1.0;
		float g = 0.0;
		float b = 0.0;
		for (int i = 0; i <numBars; i++) {
			bars[i].vdisp = QVector4D(0, -1., 0, 0);
			bars[i].hdisp = inc;
			inc+= barSize;
			if (i < (3*one_seventh)){
				g += .333/one_seventh;
			}
			if((i >= (3*one_seventh)) && (i < (4*one_seventh))){
				r -= 1.0/one_seventh;
			}
			if((i >= (4*one_seventh)) && (i < (5*one_seventh))){
				g -= 1.0/one_seventh;
				b += 1.0/one_seventh;
				r += .45/one_seventh; //can take this line out for more blue
			}
			if((i >= (5*one_seventh)) && (i < (6*one_seventh))){
				r += .29/one_seventh;
				b -= .50/one_seventh;
			}
			if((i >= (6*one_seventh)) && (i <= (7*one_seventh))){
				r += .64/one_seventh;
				g += .50/one_seventh;
				b += .42/one_seventh;
			}
			bars[i].color = QVector4D(r,g,b,0);
		}
	}

MyPanelOpenGL::~MyPanelOpenGL(){
	destroyVBOs();
	destroyShaders();
	delete timer; timer=NULL;
}

void MyPanelOpenGL::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	QFileInfo song_name;
	int song_num;

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "##############################################" << std::endl;
	std::cout << "################ Song Menu: ##################" << std::endl;
	std::cout << "##############################################" << std::endl;
	std::cout << "*type the number of the song you wish to play*" << std::endl;
	std::cout << "	1. 	Suit & Tie - Tori Kelly" << std::endl;
	std::cout << "	2. 	Scale" << std::endl;
	std::cout << "	3. 	Frequency" << std::endl;
	std::cout << "	4. 	All I want for Christmas is You - Mariah Carey" << std::endl;
	std::cout << "	5. 	Sandstorm - Darude" << std::endl;
	std::cout << "	6.	Brother - Matt Corby" << std::endl;
	std::cout << "	7. 	Up in the Woods - Bon Iver" << std::endl;

	std::cin >> song_num;


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	createShaders();
	createVBOs();
	int i;
	for (i = 0; i < numBars; i++) {
		displacements[i] = bars[i].vdisp;
	}

	// AUDIO Processing

	timer = new QTimer(this);
	m_play = new QMediaPlayer();
	m_probe = new QAudioProbe();
	QUrl local = QUrl::fromLocalFile(QFileInfo("suitandtie.mp3").absoluteFilePath());
	if (song_num == 1) {
		local = QUrl::fromLocalFile(QFileInfo("suitandtie.mp3").absoluteFilePath());
	}else if(song_num==2){
		local = QUrl::fromLocalFile(QFileInfo("example.mp3").absoluteFilePath());
	}else if(song_num==3){
	local = QUrl::fromLocalFile(QFileInfo("freq.mp3").absoluteFilePath());
	}else if(song_num==4){
	local = QUrl::fromLocalFile(QFileInfo("mariah.mp3").absoluteFilePath());
	}else if(song_num==5){
	local = QUrl::fromLocalFile(QFileInfo("sandstorm.mp3").absoluteFilePath());
	}else if(song_num==6){
	local = QUrl::fromLocalFile(QFileInfo("brother.mp3").absoluteFilePath());
	}else if(song_num==7){
	local = QUrl::fromLocalFile(QFileInfo("woods.mp3").absoluteFilePath());
	}
	m_play->setMedia(local);
	m_play->setVolume(50);
	if(!m_probe->setSource(m_play)){
		std::cout << "Unable to set probe" << std::endl;
	}

	phase = 0;
	connect(m_probe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processBuffer(QAudioBuffer)));
	m_play->play();
	connect(timer, SIGNAL(timeout()), this, SLOT(step()));
	timer->start(20); //generate a timeout every 30 miliseconds
}

void MyPanelOpenGL::step(){
	phase += 0.05;
	if (phase> 2*M_PI){
		phase -= 2*M_PI;
	}
	int i;

	for (i = 1; i < numBars; i=i+1) {
		displacements[i].setY(buf[i]/45);
	}
	update();
}

void MyPanelOpenGL::resizeGL(int w, int h)
{
	glViewport(0,0,w, h);
}

void MyPanelOpenGL::paintGL(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	if(!vboVertices){
		return;
	}
		shaderProgram->bind();
		vboVertices->bind();
		shaderProgram->enableAttributeArray("vPosition");
		shaderProgram->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4, 0);
		int i;
		for (i = 0; i < numBars; i++) {
			shaderProgram->setUniformValue("color", bars[i].color);
			shaderProgram->setUniformValue("disp", displacements[i]+ bars[i].vdisp);
			glDrawArrays(GL_TRIANGLE_STRIP, i*numVertices, numVertices);
		}
		glFlush();
		vboVertices->release();
		shaderProgram->release();
}

void MyPanelOpenGL::createShaders(){

	destroyShaders();

	vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
	if (!vertexShader->compileSourceFile("shaders/vshader.glsl")){
		qWarning() << vertexShader->log();
	}

	fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
	if(!fragmentShader->compileSourceFile("shaders/fshader.glsl")){
		qWarning() << fragmentShader->log();
	}

	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShader(vertexShader);
	shaderProgram->addShader(fragmentShader);

	if(!shaderProgram->link()){
		qWarning() << shaderProgram->log() << endl;
	}
}

void MyPanelOpenGL::destroyShaders(){

	delete vertexShader; vertexShader=NULL;
	delete fragmentShader; fragmentShader=NULL;

	if(shaderProgram){
		shaderProgram->release();
		delete shaderProgram; shaderProgram=NULL;
	}
}

void MyPanelOpenGL::createVBOs(){
	destroyVBOs(); //get rid of any old buffers

	vao = new QOpenGLVertexArrayObject(this);
	vao->create();
	vao->bind();
	vboVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vboVertices->create();
	vboVertices->bind();
	vboVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);
	// vboVertices->allocate(vertices, numVertices*sizeof(QVector4D));
	QVector4D *all_vertices;
	int temp = numBars*4;
	all_vertices = new QVector4D[temp];
	int i;
	for ( i = 0; i < numBars; i++) {
		all_vertices[4*i] = QVector4D(bars[i].hdisp, -20,0., 1.);
		all_vertices[4*i+1] = QVector4D(bars[i].hdisp, .01, 0., 1.);
		all_vertices[4*i+2] = QVector4D(bars[i].hdisp+barSize-spacing, -20, 0., 1.);
		all_vertices[4*i+3] = QVector4D(bars[i].hdisp+barSize-spacing, .01, 0., 1.);
	}
	vboVertices->allocate(all_vertices, temp*sizeof(QVector4D));

	delete [] all_vertices; all_vertices=NULL;
}

void MyPanelOpenGL::destroyVBOs(){
	if (vao){
		vao->release();
		delete vao; vao = NULL;
	}

	if (vboVertices){
		vboVertices->release();
		delete vboVertices; vboVertices=NULL;
	}
}


void MyPanelOpenGL::processBuffer(QAudioBuffer buff){

	QVector<float> max_vals;

	if (!buff.format().isValid() ||
			buff.format().byteOrder() != QAudioFormat::LittleEndian){
		printf("oops\n"); return;
	}

	if (buff.format().codec() != "audio/pcm"){
		printf("oops2\n"); return;
	}

	if(buff.format().sampleType() != QAudioFormat::SignedInt ||
			buff.format().sampleSize() != 16){
		printf("oops3\n"); return;
	}

	nframes = buff.frameCount();
	int nchannels = buff.format().channelCount();
	float val =0;
	const qint16* vals = buff.constData<qint16>();
	max_vals.fill(0, nframes);

	for(int f=0; f<nframes; ++f){
		for(int c=0; c<nchannels; ++c){
			val = fabs(float(vals[f*nchannels+c]))/float(SHRT_MAX);
			if (val > max_vals.at(f)){
				max_vals.replace(f, val);
			}
		}
	}


	// Begin FFT data prep
	float max_val_array[nframes];
	kiss_fft_cpx out_cpx[nframes];
	kiss_fft_cpx *cpx_buf;

	// conver to proper float * type for FFT
	for (size_t i = 0; i < nframes; i++) {
		max_val_array[i] = max_vals[i];
	}

	// allocate space for FFT
	kiss_fft_cfg fft = kiss_fft_alloc(nframes,0, 0, 0);

	int i;
	kiss_fft_cpx *mat2;
	mat2=(kiss_fft_cpx*)KISS_FFT_MALLOC(sizeof(kiss_fft_cpx)*nframes);
				kiss_fft_scalar zero;
				memset(&zero,0,sizeof(zero) );
	for(i=0; i<nframes ; i++)
	{
		mat2[i].r = max_val_array[i];
		mat2[i].i = zero;
	}

	cpx_buf = mat2;

	// perform FFT on samples of audio data
	kiss_fft(fft,cpx_buf, out_cpx);

	// convert FFT into appropriate format to use within our visualization
	for (size_t i = 0; i < nframes; i++) {
		buf[i] = sqrt((out_cpx[i].r*out_cpx[i].r)+(out_cpx[i].r*out_cpx[i].r));
	}
}
