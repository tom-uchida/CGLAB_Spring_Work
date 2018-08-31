//
//  PCA.cpp
//  CGLAB_Work
//
//  Created by Tomomasa Uchida on 2018/03/16.
//

#include <stdio.h>
#include "PCA.h"

PCA::PCA(){
    eigenValues.reserve(3);
    eigenVectors.reserve(3);
    
    contributionRatios.reserve(3);
    cumulativeContributionRatio = 0.0;
    curvature = 0.0;
    dimentionFeature = 0.0;
}

PCA::~PCA(){
    eigenValues.clear();
    eigenVectors.clear();
    contributionRatios.clear();
}

void PCA::calcEigenValueAndVector(const ofVboMesh& PointCloud){
    ofVec3f avgPos;
    float xMax = 0.0; float yMax = 0.0; float zMax = 0.0;
    float xMin = 0.0; float yMin = 0.0; float zMin = 0.0;
    float xSum = 0.0; float ySum = 0.0; float zSum = 0.0;
    float x2Sum = 0.0; float y2Sum = 0.0; float z2Sum = 0.0;
    
    for (int i = 0; i < PointCloud.getNumVertices(); i++) {
        // Update max value
        if ( xMax < PointCloud.getVertex(i).x )
            xMax = PointCloud.getVertex(i).x;
        
        if ( yMax < PointCloud.getVertex(i).y )
            yMax = PointCloud.getVertex(i).y;
        
        if ( zMax < PointCloud.getVertex(i).z )
            zMax = PointCloud.getVertex(i).z;
        
        // Update min value
        if ( PointCloud.getVertex(i).x < xMin)
            xMin = PointCloud.getVertex(i).x;
        
        if ( PointCloud.getVertex(i).y < yMin)
            yMin = PointCloud.getVertex(i).y;
        
        if ( PointCloud.getVertex(i).z < zMin)
            zMin = PointCloud.getVertex(i).z;
        
        // 和
        xSum += PointCloud.getVertex(i).x;
        ySum += PointCloud.getVertex(i).y;
        zSum += PointCloud.getVertex(i).z;
        
        // ２乗和
        x2Sum += PointCloud.getVertex(i).x * PointCloud.getVertex(i).x;
        y2Sum += PointCloud.getVertex(i).y * PointCloud.getVertex(i).y;
        z2Sum += PointCloud.getVertex(i).z * PointCloud.getVertex(i).z;
    }
    
    // 平均
    float xAvg = 0.0; float yAvg = 0.0; float zAvg = 0.0;
    xAvg = xSum / (float)PointCloud.getNumVertices();
    yAvg = ySum / (float)PointCloud.getNumVertices();
    zAvg = zSum / (float)PointCloud.getNumVertices();
    
    // 重心の座標
    centerPos = ofVec3f(xAvg, yAvg, zAvg);
    
    // 偏差平方和：Deviation sum of squares
    // 偏差積和：Deviation sum of products
    float xxDSS = 0.0; float yyDSS = 0.0; float zzDSS = 0.0;
    float xyDSP = 0.0; float yzDSP = 0.0; float xzDSP = 0.0;
    for (int i = 0; i < PointCloud.getNumVertices(); i++) {
        // 偏差
        float xDFA = PointCloud.getVertex(i).x - xAvg;
        float yDFA = PointCloud.getVertex(i).y - yAvg;
        float zDFA = PointCloud.getVertex(i).z - zAvg;
        
        // 偏差平方和
        xxDSS += xDFA * xDFA;
        yyDSS += yDFA * yDFA;
        zzDSS += zDFA * zDFA;
        
        // 偏差積和
        xyDSP += xDFA * yDFA;
        yzDSP += yDFA * zDFA;
        xzDSP += xDFA * zDFA;
    }
    
    // 分散（偏差平方和の平均）
    float Sxx = xxDSS / (float)PointCloud.getNumVertices();
    float Syy = yyDSS / (float)PointCloud.getNumVertices();
    float Szz = zzDSS / (float)PointCloud.getNumVertices();
    
    // 共分散（偏差積和の平均）
    float Sxy = xyDSP / (float)PointCloud.getNumVertices();
    float Syz = yzDSP / (float)PointCloud.getNumVertices();
    float Sxz = xzDSP / (float)PointCloud.getNumVertices();
    
    // 分散共分散行列
    Cov << Sxx, Sxy, Sxz,
           Sxy, Syy, Syz,
           Sxz, Syz, Szz;
    
    // 固有値を格納
    EigenSolver<Matrix3f> ES(Cov);
    eigenValues.push_back( ES.eigenvalues()(0).real() );
    eigenValues.push_back( ES.eigenvalues()(1).real() );
    eigenValues.push_back( ES.eigenvalues()(2).real() );
    
    
    ////////////////////////////////////////////
    ////////// 第1主成分〜第3主成分を判定 //////////
    ////////////////////////////////////////////
    // STEP1. 固有値を降順にソートしておく
    // STEP2. 最大固有値に対応する固有ベクトルを探す
    std::sort( eigenValues.begin(), eigenValues.end(), std::greater<float>() ); // 降順にソートしておく
    
    // 第1主成分(固有値が最も大きい固有ベクトル)の判定
    Vector3f firstEigenVector;
    if ( ES.eigenvalues()(0).real() == eigenValues[0] ) {
        firstEigenVector = ES.eigenvectors().col(0).real();
    } else if ( ES.eigenvalues()(1).real() == eigenValues[0] ) {
        firstEigenVector = ES.eigenvectors().col(1).real();
    } else if ( ES.eigenvalues()(2).real() == eigenValues[0] ) {
        firstEigenVector = ES.eigenvectors().col(2).real();
    }

    // 第2主成分(固有値が２番目に大きい固有ベクトル)の判定
    Vector3f secondEigenVector;
    if ( ES.eigenvalues()(0).real() == eigenValues[1] ) {
        secondEigenVector = ES.eigenvectors().col(0).real();
    } else if ( ES.eigenvalues()(1).real() == eigenValues[1] ) {
        secondEigenVector = ES.eigenvectors().col(1).real();
    } else if ( ES.eigenvalues()(2).real() == eigenValues[1] ) {
        secondEigenVector = ES.eigenvectors().col(2).real();
    }
            
    // 第3主成分(固有値が最も小さい固有ベクトル)の判定
    Vector3f thirdEigenVector;
    if ( ES.eigenvalues()(0).real() == eigenValues[2] ) {
        thirdEigenVector = ES.eigenvectors().col(0).real();
    } else if ( ES.eigenvalues()(1).real() == eigenValues[2] ) {
        thirdEigenVector = ES.eigenvectors().col(1).real();
    } else if ( ES.eigenvalues()(2).real() == eigenValues[2] ) {
        thirdEigenVector = ES.eigenvectors().col(2).real();
    }
    
    // 固有ベクトルを格納
    eigenVectors.push_back(ofVec3f(firstEigenVector[0], firstEigenVector[1], firstEigenVector[2]));
    eigenVectors.push_back(ofVec3f(secondEigenVector[0], secondEigenVector[1], secondEigenVector[2]));
    eigenVectors.push_back(ofVec3f(thirdEigenVector[0], thirdEigenVector[1], thirdEigenVector[2]));
    
    calcNormalVector(); // 法線ベクトル
    calcContributionRatios(); // 寄与率
    calcCumulativeContributionRatio(); // 累積寄与率
    calcCurvature(); // 曲率
    calcDimentionFeature(); // 次元特徴
}

void PCA::drawEigenvectors(const ofVec3f& targetPos) {
    ofSetLineWidth(5);
    
    ofSetColor(255, 0, 0);
    ofDrawLine(targetPos, targetPos+eigenVectors[0]*100);
    
    ofSetColor(0, 255, 0);
    ofDrawLine(targetPos, targetPos+eigenVectors[1]*100);
    
    ofSetColor(0, 0, 255);
    ofDrawLine(targetPos, targetPos+eigenVectors[2]*100);
}

// 法線ベクトル
void PCA::calcNormalVector() {
    normalVector = eigenVectors[2]; // 第３主成分ベクトル
}

// 寄与率
void PCA::calcContributionRatios() {
    // 寄与率は、観測データの全情報量に対してどの程度の情報量を個々の主成分が集めたかを示す比率。
    // 寄与率は、この主成分だけで元のデータの何割を説明することができているかを表した数字。
    for (int i = 0; i < 3; i++) {
        float tmp = eigenValues[i] / (eigenValues[0]+eigenValues[1]+eigenValues[2]);
        contributionRatios.push_back(tmp);
    }
}

// 累積寄与率
void PCA::calcCumulativeContributionRatio() {
    for (int i = 0; i < 3; i++) {
        cumulativeContributionRatio += contributionRatios[i];
    }
}

// 曲率
void PCA::calcCurvature() {
    // 最小固有値λ3の寄与率
    curvature = eigenValues[3] / (eigenValues[0]+eigenValues[1]+eigenValues[2]);
}

// 次元特徴
void PCA::calcDimentionFeature() {
    // pairを作成
    vector<pair<float, int>> pairs = {
        {eigenValues[0]-eigenValues[1], 1},
        {eigenValues[1]-eigenValues[2], 2},
        {eigenValues[2]               , 3}
    };
    
    // first昇順、second昇順の順番でソート
    std::sort(pairs.begin(), pairs.end());
    
    // 最大を与えるkの値
    dimentionFeature = pairs[2].second; // k = 1 or 2 or 3
}

vector<float> PCA::getEigenValues() const{
    return eigenValues;
}

vector<ofVec3f> PCA::getEigenVectors() const{
    return eigenVectors;
}

ofVec3f PCA::getCenterPos() const{
    return centerPos;
}

ofVec3f PCA::getNormalVector() const{
    return normalVector;
}

vector<float> PCA::getContributionRatios() const{
    return contributionRatios;
}

float PCA::getCumulativeContributionRatio() const{
    return cumulativeContributionRatio;
}

float PCA::getCurvature() const{
    return curvature;
}

float PCA::getDimentionFeature() const{
    return dimentionFeature;
}

void PCA::clear() {
    eigenValues.clear();
    eigenVectors.clear();
    contributionRatios.clear();
}
