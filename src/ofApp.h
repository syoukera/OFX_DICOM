#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    void loadNpyFile(string filename);
    
    // 3Dテクスチャ用の変数
    GLuint textureID;
    int volWidth, volHeight, volDepth;
    vector<double> volumeData_double;
    vector<float> volumeData_float;

    // シェーダー用
    ofShader shader;
    ofEasyCam cam; // カメラ制御用
};
