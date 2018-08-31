#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxKDTree.h"
#include "PCA.h"
//#define DISPLAY_1440_900

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void setGUI();
        void setLight();
        void setMaterial();
        void calcPlySize(ofVboMesh& PointCloud);
        void changeModeCheck();
        void changeAxisCheck();
        void removePointsInRect(const ofVec2f& startPos, const ofVec2f& endPos);
        void extractPlane(ofVboMesh& PointCloud);
        void calcPlaneEquation(const ofVboMesh& PointCloud, const ofVec3f& normalVector, const ofVec3f& centerPos);
        void displayPCAresult();
        void searchNearestPointWithKDTree(const ofVboMesh& PointCloud);
        void setKDTree(const ofVboMesh& PointCloud);
        void getkNNWithKDTree(const ofVec3f& queryPointPos, int k);
        void searchNearestPoint(const ofVboMesh& PointCloud);
        void searchPointsInSphere(const ofVec3f& queryPointPos, int mode);
        void drawLinesInSphere(const ofVec3f& queryPointPos, vector<ofVec3f>& points);
        void drawPointsInSphere(vector<ofVec3f>& points);
        void interpolatePointsInSphere();
        void drawExtractRange();
        void downSampling();
        void drawAxis();
        void drawFrame();
        void resetMode();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // Addon
    ofxPanel gui, gui1, gui2, gui3, gui3X, gui3Y, gui3Z, gui4, gui5;
    ofxButton InterPointDistMode, RemovePointsMode, ExtractCrossSectionMode, InterpolatePointsMode, TargetRangePCAMode, DownSampling, DrawXAxis, DrawYAxis, DrawZAxis, CreateCrossSection, InterpolatePoints, PrincipalComponentAnalysis, RemovePointsInRect, ResetPLY;
    ofxLabel changeModel, changeDrawMode, disableMouseInput, screenShot, fullScreen, savePLY;
    ofxToggle isDrawAxis, isDrawFrame, isDrawSphere_4, isDrawLineInSphere_4, isDrawCandidatePointsInSphere_4, isDrawSphere_5, isDrawLineInSphere_5, isDrawEigenvectors_5, isDrawPCAresult_5, isSearchWithKDTree;
    ofxFloatSlider downRatio, countSphereRadius, certainRange_4;
    ofxIntSlider offset, xAxisValue, yAxisValue, zAxisValue;
    ofxKDTree kdTree;
    
    
    ofVboMesh vboMesh, crossSectionPoints;
    ofEasyCam cam;
    ofImage img;
    
    ofVec2f mousePos;
    ofVec3f centerPos;
    ofVec3f nearestPointPos;
    vector<ofVec3f> interPoints;
    vector<ofVec2f> rangePoints;
    vector<vector<double>> points;
    vector<size_t> indexes;
    vector<double> dists;
    vector<ofVec3f> candidatePointsInSphere;
    vector<ofVec3f> pointsInSphere;
    vector<ofVec3f> interpolationPoints;
    class PCA pca;
    vector<float> eigenValues;
    vector<float> contributionRatios;
    ofVec3f firstEigenvector;
    ofVec3f secondEigenvector;
    ofVec3f thirdEigenvector;
    float dimentionFeature;
    int nearestIndex = 0;
    int clickCount = 0;
    int shotCounter = 0;
    int saveCounter = 0;
    float sphereRadius;
    float xMax = 0.0;
    float xMin = 0.0;
    float yMax = 0.0;
    float yMin = 0.0;
    float zMax = 0.0;
    float zMin = 0.0;
    float xSize, ySize, zSize, sideLength;
    
    bool isPLY1 = true;
    bool isPLY2 = false;
    bool isChangeModel = false;
    bool isInterPointDistMode = true;
    bool isRemovePointsMode = false;
    bool isCreateCrossSectionMode = false;
    bool isInterpolatePointsMode = false;
    bool isTargetRangePCAMode = false;
    bool isDrawXAxis_3 = false;
    bool isDrawYAxis_3 = true;
    bool isDrawZAxis_3 = false;
    bool isSelectAxisValue_3 = true;
    bool isCreateCrossSection_3 = false;
    bool isSelectRadiusRange_4 = false;
    bool isSelectCertainRange_4 = false;
    bool isAfterInterpolate_4 = false;
    bool isSelectRadiusRange_5 = false;
    bool isAfterPCA_5 = false;
    bool isInit = true;
    bool isWire = false;
    bool isPoint = true;
    bool isScreenshot = false;
    bool isSavePLY = false;
    
    ofTrueTypeFont verdana20, verdana30, verdana40;
    ofLight light;
    ofMaterial material;
    
    ofxFloatColorSlider specular_light, diffuse_light, ambient_light, specular_material, diffuse_material, ambient_material, emmisive_material;
    ofxIntSlider shiness_material;
    ofColor sphereColor = ofColor(255, 0, 0, 100);
};
