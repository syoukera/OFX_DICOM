#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
    
#ifdef OF_TARGET_OPENGLES
	ofGLESWindowSettings settings;
	settings.glesVersion=2;
#else
	ofGLWindowSettings settings;
	settings.setGLVersion(3,2);
#endif

	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();

    // ofSetupOpenGL(1024, 768, OF_WINDOW); // 画面サイズ
    // ofRunApp(new ofApp());
}
