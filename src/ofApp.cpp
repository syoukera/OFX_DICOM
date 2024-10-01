#include "ofApp.h"
#include <fstream>

// NPYファイルの読み込み
void ofApp::loadNpyFile(string filename) {
    string path = ofToDataPath(filename, true);
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        ofLogError() << "Failed to open file: " << path;
        return;
    }

    // NPYファイルのヘッダ情報を読み飛ばす（適宜調整が必要）
    char buffer[256];
    file.read(buffer, 256);

    // データの読み込み
    volWidth = 666;  // 適切な幅を設定
    volHeight = 666; // 適切な高さを設定
    volDepth = 666;  // 適切な奥行きを設定
    volumeData_double.resize(volWidth * volHeight * volDepth);
    file.read(reinterpret_cast<char*>(volumeData_double.data()), volumeData_double.size() * sizeof(double));
    file.close();

    ofLogNotice() << "NPY file loaded successfully!";

    volumeData_float.resize(volWidth * volHeight * volDepth);
    for (size_t i = 0; i < volumeData_float.size(); ++i) {
        volumeData_float[i] = static_cast<float>(volumeData_double[i]);
    }
}

void ofApp::setup() {
    // NPYファイルを読み込む
    loadNpyFile("volumeData_norm.npy");

    // デバッグ用に一部データを表示
    ofLogNotice() << "First data points for read: " << volumeData_double[0] << ", " << volumeData_double[1] << ", " << volumeData_double[2];
    ofLogNotice() << "First data points for load: " << volumeData_float[0] << ", " << volumeData_float[1] << ", " << volumeData_float[2];


    // OpenGL で3Dテクスチャを設定
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);

    // テクスチャのパラメータを設定
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // 3Dテクスチャにデータをアップロード
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, volWidth, volHeight, volDepth, 0, GL_RED, GL_FLOAT, volumeData_float.data());

    // シェーダーのロード
    shader.load("shadersGL3/vertex_shader.glsl", "shadersGL3/fragment_shader.glsl");

    // カメラの初期設定
    // cam.setDistance(300);
    cam.setPosition(0, 0, 300);
    cam.lookAt(glm::vec3(0, 0, 0)); // カメラの向きを設定

    ofEnableDepthTest(); // OpenFrameworks での深度テストの有効化
    // glDisable(GL_CULL_FACE); // カリングを無効化
}

void ofApp::update() {
    // 必要ならアップデート処理を追加
}

void ofApp::draw() {
    ofBackground(110);
    cam.begin();
    

    shader.begin();

    // // カメラの位置を取得し、シェーダーに渡す
    // glm::vec3 camPosition = cam.getPosition();
    // shader.setUniform3f("rayOrigin", camPosition);

    shader.setUniform1i("volumeTexture", 0); // テクスチャユニット0を設定
    shader.setUniform3f("volumeDimensions", volWidth, volHeight, volDepth);

    // 画面の解像度をシェーダーに渡す
    shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());

    if (!shader.isLoaded()) {
        ofLogError() << "Shader failed to load!";
    }

    // テクスチャのバインド
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, textureID);

    // shader.end(); // シェーダーを一旦無効にする

    // // 立方体の描画
    // ofSetColor(0, 255, 0);
    // ofFill();
    // ofDrawBox(0, 0, 0, 100);

    // 四角形を描画して全画面を覆う（シェーダーを適用するため）
    ofDrawRectangle(-ofGetWidth()/2, -ofGetHeight()/2, ofGetWidth(), ofGetHeight());

    shader.end();
    cam.end();
}
