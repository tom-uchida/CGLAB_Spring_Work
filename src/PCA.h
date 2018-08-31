//
//  PCA.h
//  CGLAB_Work
//
//  Created by Tomomasa Uchida on 2018/03/16.
//

#ifndef PCA_h
#define PCA_h

#include "ofMain.h"
#include "../libs/Eigen/include/Eigen/Core"
#include "../libs/Eigen/include/Eigen/Eigen"
#include <algorithm> // sort
#include <map> // pair

using namespace Eigen;

class PCA {
public :
    PCA();
    ~PCA();
    
    void calcEigenValueAndVector(const ofVboMesh& PointCloud);
    void drawEigenvectors(const ofVec3f& targetPos);
    void calcNormalVector();
    void calcContributionRatios();
    void calcCumulativeContributionRatio();
    void calcCurvature();
    void calcDimentionFeature();
    
    vector<float> getEigenValues() const;
    vector<ofVec3f> getEigenVectors() const;
    ofVec3f getCenterPos() const;
    ofVec3f getNormalVector() const;
    vector<float> getContributionRatios() const;
    float getCumulativeContributionRatio() const;
    float getCurvature() const;
    float getDimentionFeature() const;
    void clear();
    
private :
    Matrix3f Cov; // 分散共分散行列
    ofVec3f centerPos; // 対象点群の重心
    vector<float> eigenValues; // 固有値
    vector<ofVec3f> eigenVectors; // 固有ベクトル(３次元ベクトルが３つ)
    ofVec3f normalVector; // 法線ベクトル
    vector<float> contributionRatios; // 寄与率
    float cumulativeContributionRatio; // 累積寄与率
    float curvature; // 曲率
    float dimentionFeature; // 次元特徴
};

#endif /* PCA_h */
