#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFullscreen(true);
    ofSetBackgroundAuto(true);
    ofBackground(0);
    //ofSetFrameRate(60);
    ofSetVerticalSync(true);
    //ofEnableAlphaBlending();
    //ofSetSmoothLighting(true);
    
    // PLYファイルの読み込み
    vboMesh.load("ply/bunny900.ply");
    //vboMesh.load("ply/saved/saved_0.ply");
    
    // 前処理
    calcPlySize(vboMesh);
    setKDTree(vboMesh);
    
    // フォント
    ofTrueTypeFont::setGlobalDpi(72);
    verdana20.load("font/verdana.ttf", 20, false);
    verdana30.load("font/verdana.ttf", 30, false);
    verdana40.load("font/verdana.ttf", 40, false);
    verdana20.setLineHeight(24); // 行間
    verdana30.setLineHeight(24);
    verdana40.setLineHeight(24);
    verdana20.setLetterSpacing(1.0); // 文字間
    verdana30.setLetterSpacing(1.0);
    verdana40.setLetterSpacing(1.0);
    
    // 光源・マテリアル
    setLight();
    setMaterial();
    
    // GUI
    setGUI();
}

void ofApp::setGUI() {
    gui.setup();
    gui.add(changeModel.setup("Model change ", "'a' key"));
    gui.add(InterPointDistMode.setup("Inter-Point-Dist Mode ('1' key)"));
    gui.add(RemovePointsMode.setup("Range-Select Mode ('2' key)"));
    gui.add(ExtractCrossSectionMode.setup("Cut-Cross-Section Mode ('3' key)"));
    gui.add(InterpolatePointsMode.setup("Interpolate-Points Mode ('4' key)"));
    gui.add(TargetRangePCAMode.setup("Target-Range-PCA Mode ('5' key)"));
    gui.add(isSearchWithKDTree.setup("Search with kdTree ('k' key)", false));
    gui.add(downRatio.setup("Down Sampling Ratio", 1.0, 0.0, 1.0));
    gui.add(DownSampling.setup("Down Sampling ('d' key)"));
    gui.add(isDrawAxis.setup("Draw Axis", true));
    gui.add(isDrawFrame.setup("Draw Frame", true));
    gui.add(changeDrawMode.setup("Change Draw Mode ", "'0' key"));
    gui.add(disableMouseInput.setup("Disable Mouse Input ", "'c' key"));
    gui.add(screenShot.setup("Screen Shot ", "'s' key"));
    gui.add(ResetPLY.setup("Reset PLY"));
    gui.add(savePLY.setup("Save PLY data ", "'v' key"));
    gui.add(fullScreen.setup("Full Screen ", "'f' key"));
    
    // モード1
    gui1.setup();
    gui1.setPosition(10, ofGetHeight()*0.5);
    
    // モード2
    gui2.setup();
    gui2.setPosition(10, ofGetHeight()*0.5);
    gui2.add(RemovePointsInRect.setup("Remove points ('r' key)"));
    
    // モード3
    gui3.setup();
    gui3.setPosition(10, ofGetHeight()*0.5);
    gui3.add(offset.setup("offset : ", 5, 0, (int)sideLength*0.25));
    gui3.add(DrawXAxis.setup("X-Axis"));
    gui3.add(DrawYAxis.setup("Y-Axis"));
    gui3.add(DrawZAxis.setup("Z-Axis"));
    gui3.add(CreateCrossSection.setup("Create cross section ('g' key)"));
    
    gui3X.setup();
    gui3X.setPosition(10, ofGetHeight()*0.5+150);
    gui3X.add(xAxisValue.setup("X : ", 0, -sideLength*0.5, sideLength*0.5));
    
    gui3Y.setup();
    gui3Y.setPosition(10, ofGetHeight()*0.5+150);
    gui3Y.add(yAxisValue.setup("Y : ", 0, -sideLength*0.5, sideLength*0.5));
    
    gui3Z.setup();
    gui3Z.setPosition(10, ofGetHeight()*0.5+150);
    gui3Z.add(zAxisValue.setup("Z : ", 0, -sideLength*0.5, sideLength*0.5));
    
    // モード4
    gui4.setup();
    gui4.setPosition(10, ofGetHeight()*0.5);
    gui4.add(countSphereRadius.setup("Sphere Radius", 1, 0, sideLength*0.5));
    gui4.add(certainRange_4.setup("Certain Range", 1, 0, sideLength*0.1));
    gui4.add(InterpolatePoints.setup("Interpolate points ('i' key)"));
    gui4.add(isDrawLineInSphere_4.setup("Draw lines in sphere", true));
    gui4.add(isDrawCandidatePointsInSphere_4.setup("Draw candidate points", true));
    gui4.add(isDrawSphere_4.setup("Draw count sphere", true));
    
    // モード5
    gui5.setup();
    gui5.setPosition(10, ofGetHeight()*0.5);
    gui5.add(countSphereRadius.setup("Sphere Radius", 1, 0, sideLength*0.5));
    gui5.add(PrincipalComponentAnalysis.setup("PCA ('p' key)"));
    gui5.add(isDrawSphere_5.setup("Draw count sphere", true));
    gui5.add(isDrawLineInSphere_5.setup("Draw lines in sphere", true));
    gui5.add(isDrawEigenvectors_5.setup("Draw eigenvectors", true));
    gui5.add(isDrawPCAresult_5.setup("Draw PCA result", true));

    // フォント
    gui.loadFont("font/verdana.ttf", 14);
    gui1.loadFont("font/verdana.ttf", 14);
    gui2.loadFont("font/verdana.ttf", 14);
    gui3.loadFont("font/verdana.ttf", 14);
    gui4.loadFont("font/verdana.ttf", 14);
    gui5.loadFont("font/verdana.ttf", 14);
}

void ofApp::setLight() {
    light.setup();
//    light.setSpotlight();
    light.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
    light.setAmbientColor(ofFloatColor(1.0, 0.0, 0.0, 1.0));
}

void ofApp::setMaterial() {
    material.setSpecularColor(ofFloatColor(1.0, 1.0, 0.0, 1.0));
    material.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0, 0.5)); // 半透明
    material.setAmbientColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
    material.setEmissiveColor(ofFloatColor(1.0, 0.0, 0.0, 1.0));
    material.setShininess(100);
}

// KDTreeに点群を追加する関数（最初に１回構成すれば良い）（ワールド座標系）
void ofApp::setKDTree(const ofVboMesh& PointCloud) {
    points.reserve(PointCloud.getNumVertices());
    for (int i = 0; i < PointCloud.getNumVertices(); i++) {
        vector<double> samplePoint(3); // 3次元
        samplePoint[0] = PointCloud.getVertex(i).x;
        samplePoint[1] = PointCloud.getVertex(i).y;
        samplePoint[2] = PointCloud.getVertex(i).z;
        
        points.push_back(samplePoint);
        kdTree.addPoint(samplePoint);
    }
    
    kdTree.constructKDTree();
}

//--------------------------------------------------------------
void ofApp::update(){
    // モードの切替
    changeModeCheck();
    
    // ダウンサンプリング
    if (DownSampling) downSampling();
    
    // モード2
    if (isRemovePointsMode) {
        if (RemovePointsInRect && clickCount == 2) {
            // ユーザが指定した範囲の点を削除
            removePointsInRect(rangePoints[0], rangePoints[1]);
            clickCount = 0;
            
            // kdTreeをリセット
            kdTree.clear();
            indexes.clear();
            points.clear();
            setKDTree(vboMesh);
        }
    }
    
    // モード3
    if (isCreateCrossSectionMode) {
        changeAxisCheck();
        
        if (CreateCrossSection && isSelectAxisValue_3) {
            isSelectAxisValue_3 = false;
            isCreateCrossSection_3 = true;
            extractPlane(vboMesh);
        }
        
    } else {
        isCreateCrossSection_3 = false;
    }
    
    // モード4
    if (isInterpolatePointsMode) {
        // 補間
        if (InterpolatePoints) {
            if (isSelectCertainRange_4) {
                isSelectCertainRange_4 = false;
                interpolatePointsInSphere();
                isAfterInterpolate_4 = true;
            }
        }
    }

    // モード5
    if (isTargetRangePCAMode) {
        // 主成分分析
        if (PrincipalComponentAnalysis) {
            if (isSelectRadiusRange_5 && !isAfterPCA_5) {
                ofVboMesh targetPointCloud;
                for (int i = 0; i < pointsInSphere.size(); i++) {
                    targetPointCloud.addVertex( pointsInSphere[i] );
                }
                
                pca.calcEigenValueAndVector(targetPointCloud);
                eigenValues = pca.getEigenValues();
                firstEigenvector = pca.getEigenVectors()[0];
                secondEigenvector = pca.getEigenVectors()[1];
                thirdEigenvector = pca.getEigenVectors()[2];
                contributionRatios = pca.getContributionRatios();
                dimentionFeature = pca.getDimentionFeature();
                
                isAfterPCA_5 = true;
                isSelectRadiusRange_5 = false;
                sphereRadius = countSphereRadius; // 半径を保存
            }
        }
        
    } else {
        isSelectRadiusRange_5 = false;
        isAfterPCA_5 = false;
    }
    
    // マウスの座標を更新
    mousePos.x = mouseX;
    mousePos.y = mouseY;
    
    // スクリーンショット
    if (isScreenshot){
        img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        string fileName = "screenshot_"+ofToString(shotCounter)+".png";
        img.save(fileName);
        shotCounter++;
        isScreenshot = false;
    }
    
    // PLYをリセット
    if (ResetPLY) {
        clickCount = 0;
        kdTree.clear();
        indexes.clear();
        points.clear();
        vboMesh.clear();
        
        if (isPLY1) vboMesh.load("ply/bunny900.ply");
        if (isPLY2) vboMesh.load("ply/heritage.ply");
        
        //calcPlySize(vboMesh);
        setKDTree(vboMesh);
    }
    
    // PLY出力
    if (isSavePLY) {
        string fileName = "saved_"+ofToString(saveCounter)+".ply";
        vboMesh.save("ply/saved/"+fileName);
        saveCounter++;
        isSavePLY = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    
    cam.begin();
        // 軸の描画
        if (isDrawAxis) drawAxis();
    
        // 枠の描画
        if (isDrawFrame) drawFrame();
    
        // 点の描画の仕方
        ofSetColor(255);
        ofSetLineWidth(1);
        glPointSize(1);
        if (isWire) vboMesh.drawWireframe();
        //if (isPoint) vboMesh.drawVertices();
        if (isPoint && !isCreateCrossSection_3) vboMesh.drawVertices();
    
        // モード1
        if (isInterPointDistMode && !isRemovePointsMode && !isCreateCrossSectionMode && !isInterpolatePointsMode && !isTargetRangePCAMode) {
            // １点目と２点目を線で結ぶ
            if (clickCount == 2) {
                ofSetColor(255, 0, 0);
                ofSetLineWidth(5);
                ofDrawLine(interPoints[0], interPoints[1]);
            }
        }
    
        // モード3
        if (isCreateCrossSectionMode && !isInterPointDistMode && !isRemovePointsMode && !isInterpolatePointsMode && !isTargetRangePCAMode) {
            // ユーザが抽出する範囲を選択する
            if (isSelectAxisValue_3) {
                // 抽出する範囲を描画
                drawExtractRange();
            }
            
            if (isCreateCrossSection_3) {
                // 固有ベクトルの描画（重心を始点として）
                pca.drawEigenvectors(pca.getCenterPos());
                
                // 生成した平面の点群を描画
                crossSectionPoints.drawVertices();
            }
        }
    
        // モード4
        if (isInterpolatePointsMode && !isInterPointDistMode && !isRemovePointsMode && !isCreateCrossSectionMode && !isTargetRangePCAMode) {
            // 計数球の半径を選択中
            if (isSelectRadiusRange_4) {
                // 計数球の描画(リアルタイム)
                if (isDrawSphere_4) {
                    ofEnableLighting();
                    light.enable();
                    material.begin();
                        ofFill();
                        ofSetColor(sphereColor);
                        ofDrawSphere(nearestPointPos, countSphereRadius);
                    material.end();
                    light.disable();
                    ofDisableLighting();
                }
                
                // 計数球内の点群を探索
                searchPointsInSphere(nearestPointPos, 4);
                
                // 計数球内の点群を描画
                if (isDrawLineInSphere_4) {
                    drawLinesInSphere(nearestPointPos, pointsInSphere);
                }
                
                // 補間候補点の描画
                if (isDrawCandidatePointsInSphere_4) {
                    drawPointsInSphere(candidatePointsInSphere);
                }
            }
            
            // 計数球の半径確定後
            if (isSelectCertainRange_4) {
                // 計数球の描画
                if (isDrawSphere_4) {
                    ofEnableLighting();
                    light.enable();
                    material.begin();
                        ofFill();
                        ofSetColor(sphereColor);
                        ofDrawSphere(candidatePointsInSphere[1], certainRange_4);
                    material.end();
                    light.disable();
                    ofDisableLighting();
                }
                
                // 計数球内の点群を描画
                if (isDrawLineInSphere_4) {
                    drawLinesInSphere(nearestPointPos, pointsInSphere);
                }
                
                // 候補点の描画（青色の点）
                if (isDrawCandidatePointsInSphere_4) {
                    drawPointsInSphere(candidatePointsInSphere);
                }
            }
            
            // 補間後
            if (isAfterInterpolate_4) {
                // 計数球内の点群を描画
                if (isDrawLineInSphere_4) {
                    drawLinesInSphere(nearestPointPos, pointsInSphere);
                }
                
                // 候補点の描画（青色の点）
                if (isDrawCandidatePointsInSphere_4) {
                    drawPointsInSphere(candidatePointsInSphere);
                }
            }
        }
    
        // モード5
        if (isTargetRangePCAMode && !isInterPointDistMode && !isRemovePointsMode && !isCreateCrossSectionMode && !isInterpolatePointsMode) {
            // 計数球の半径を選択中
            if (isSelectRadiusRange_5) {
                // 計数球の描画(リアルタイム)
                if (isDrawSphere_5) {
                    ofEnableLighting();
                    light.enable();
                    material.begin();
                        ofFill();
                        ofSetColor(sphereColor);
                        ofDrawSphere(nearestPointPos, countSphereRadius);
                    material.end();
                    light.disable();
                    ofDisableLighting();
                }
                
                // 計数球内の点群を探索
                searchPointsInSphere(nearestPointPos, 5);
                
                // 探索結果の描画
                drawLinesInSphere(nearestPointPos, pointsInSphere);
                
            // 計数球の半径確定後
            } else {
                // 計数球の描画
                if (isDrawSphere_5) {
                    ofEnableLighting();
                    light.enable();
                    material.begin();
                        ofFill();
                        ofSetColor(sphereColor);
                        ofDrawSphere(nearestPointPos, sphereRadius);
                    material.end();
                    light.disable();
                    ofDisableLighting();
                }
                
                // 探索結果の描画
                if (isDrawLineInSphere_5 && isAfterPCA_5) {
                    drawLinesInSphere(nearestPointPos, pointsInSphere);
                }
                
                // 固有ベクトルの描画
                if (isDrawEigenvectors_5 && isAfterPCA_5) {
                    pca.drawEigenvectors(nearestPointPos);
                }
            }
        }
        //ofPopMatrix();
    cam.end();
    
    
    ////////////////////////////////////////
    //////////// 以下，カメラ範囲外////////////
    ////////////////////////////////////////
    
    
    //////////// モード1 ////////////
    if (isInterPointDistMode && !isRemovePointsMode && !isCreateCrossSectionMode && !isInterpolatePointsMode && !isTargetRangePCAMode) {
        ofSetColor(255);
#ifdef DISPLAY_1440_900
        verdana30.drawString("<Inter-Point Distance Mode>", ofGetWidth()*0.67, 50);
#else
        verdana40.drawString("<Inter-Point Distance Mode>", ofGetWidth()*0.67, 50);
#endif
        // マウスに最も近い点を探索
        if (!isInit) {
            if (!isSearchWithKDTree) searchNearestPoint(vboMesh); // 計算量多い
            else searchNearestPointWithKDTree(vboMesh); // KDTree
        }
        
        if (clickCount == 0) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Select first point. (Right-click)", 300, 50);
        }
        
        if (clickCount == 1) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Select second point. (Right-click)", 300, 50);
        }
        
        // １点目を描画
        if (clickCount == 1 || clickCount == 2) {
            ofFill();
            ofSetColor(255, 0, 0);
            ofVec2f point1Screen = cam.worldToScreen(interPoints[0]);
            ofDrawCircle(point1Screen, 5);
            
            // 表示
            verdana30.drawString("Point1 : ("+ofToString(interPoints[0])+")", point1Screen.x+20, point1Screen.y);
        }
        
        // ２点目を描画
        if (clickCount == 2) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Reset points. (Right-click)", 300, 50);
            
            ofFill();
            ofSetColor(255, 0, 0);
            ofVec2f point2Screen = cam.worldToScreen(interPoints[1]);
            ofDrawCircle(point2Screen, 5);
            
            // 表示
            verdana30.drawString("Point2 : ("+ofToString(interPoints[1])+")", point2Screen.x+20, point2Screen.y);
            
            // ２点間の距離を表示
            float dist = interPoints[0].distance(interPoints[1]);
            verdana30.drawString("Distance : "+ofToString(dist), mousePos.x+30, mousePos.y+110);
        }
        
        gui1.draw();
    }
    
    //////////// モード2 ////////////
    if (isRemovePointsMode && !isInterPointDistMode && !isCreateCrossSectionMode && !isInterpolatePointsMode && !isTargetRangePCAMode) {
        ofSetColor(255);
#ifdef DISPLAY_1440_900
        verdana30.drawString("<Remove Points Mode>", ofGetWidth()*0.67, 50);
#else
        verdana40.drawString("<Remove Points Mode>", ofGetWidth()*0.67, 50);
#endif
        
        if (clickCount == 0) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Decide start point. (Right-click)", 300, 50);
        }
        
        // 始点を描画
        if (clickCount == 1) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Decide end point. (Right-click)", 300, 50);
            
            ofNoFill();
            ofSetColor(255, 0, 0);
            ofSetLineWidth(3);
            ofVec2f tmp = mousePos-rangePoints[0];
            ofDrawRectangle(rangePoints[0].x, rangePoints[0].y, tmp.x, tmp.y);
        }
        
        // 範囲四角形を描画
        if (clickCount == 2) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Press 'r' key, remove points.", 300, 50);
            verdana30.drawString("- Reset range. (Right-click).", 300, 90);
            
            ofFill();
            ofSetColor(255, 0, 0, 100);
            ofSetLineWidth(5);
            ofVec2f sideLength = rangePoints[1]-rangePoints[0];
            ofDrawRectangle(rangePoints[0].x, rangePoints[0].y, sideLength.x, sideLength.y);
            ofNoFill();
            ofDrawRectangle(rangePoints[0].x, rangePoints[0].y, sideLength.x, sideLength.y);
        }
        
        gui2.draw();
    }
    
    //////////// モード3 ////////////
    if (isCreateCrossSectionMode && !isInterPointDistMode && !isRemovePointsMode && !isInterpolatePointsMode && !isTargetRangePCAMode) {
        ofSetColor(255);
#ifdef DISPLAY_1440_900
        verdana30.drawString("<Create Cross-Section Mode>", ofGetWidth()*0.67-50, 50);
#else
        verdana40.drawString("<Create Cross-Section Mode>", ofGetWidth()*0.67-50, 50);
#endif
        
        // ユーザが動かす軸と抽出する範囲を指定する
        if (isSelectAxisValue_3) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Step1. Select x or y or z axis. (Button)", 300, 50);
            verdana30.drawString("- Step2. Select extract range. (Slider)", 300, 90);
            verdana30.drawString("- Step3. Press 'g' key, create cross section.", 300, 130);
        }
        
        if (isCreateCrossSection_3) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Reset extract range. (Right-click)", 300, 50);
        }
        
        if (isDrawXAxis_3) gui3X.draw();
        if (isDrawYAxis_3) gui3Y.draw();
        if (isDrawZAxis_3) gui3Z.draw();
    
        gui3.draw();
    }
    
    //////////// モード4 ////////////
    if (isInterpolatePointsMode && !isInterPointDistMode && !isRemovePointsMode && !isCreateCrossSectionMode && !isTargetRangePCAMode ) {
        ofSetColor(255);
#ifdef DISPLAY_1440_900
        verdana30.drawString("<Interpolate Points Mode>", ofGetWidth()*0.67, 50);
#else
        verdana40.drawString("<Interpolate Points Mode>", ofGetWidth()*0.67, 50);
#endif
        
        // ユーザが点を1つ指定する
        if (clickCount == 0) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Select one query point. (Right-click)", 300, 50);
            
            if (!isInit) {
                if (!isSearchWithKDTree) searchNearestPoint(vboMesh); // 計算量多い
                else searchNearestPointWithKDTree(vboMesh); // kdTree
            }
            
        // ユーザが指定した点の描画
        } else {
            ofFill();
            ofSetColor(255, 0, 0);
            ofVec2f nearestPointPos2D = cam.worldToScreen(nearestPointPos);
            ofDrawCircle(nearestPointPos2D, 5);
        }
        
        // 選択した点を描画
        if (clickCount == 1) {
            // ユーザが選択した点の座標を表示
            ofSetColor(255, 0, 0);
            if (mousePos.x < ofGetWidth()*0.5) {
                verdana30.drawString("Query Point : ("+ofToString(nearestPointPos)+")", mousePos.x+30, mousePos.y+30);
            } else {
                verdana30.drawString("Query Point : ("+ofToString(nearestPointPos)+")", mousePos.x-700, mousePos.y+30);
            }
        }
        
        // ユーザが計数球の半径を選択する
        if (isSelectRadiusRange_4) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Step1. Select the radius of the count sphere. (Slider)", 300, 50);
            verdana30.drawString("- Step2. Decide the radius. (Right-click)", 300, 90);
        }
        
        // 補間のための範囲を選択し，補間を実行する
        if (isSelectCertainRange_4) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Step1. Select the certain range. (Slider)", 300, 50);
            verdana30.drawString("- Step2. Press 'i' key, interpolate points", 300, 90);
        }
        
        // 補間後
        if (isAfterInterpolate_4) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Reset query point. (Right-click)", 300, 50);
        }
        
        gui4.draw();
    }
    
    //////////// モード5 ////////////
    if (isTargetRangePCAMode && !isInterPointDistMode && !isRemovePointsMode && !isCreateCrossSectionMode && !isInterpolatePointsMode ) {
        ofSetColor(255);
#ifdef DISPLAY_1440_900
        verdana30.drawString("<Target Range PCA Mode>", ofGetWidth()*0.67, 50);
#else
        verdana40.drawString("<Target Range PCA Mode>", ofGetWidth()*0.67, 50);
#endif
        
        // ユーザが点を1つ指定する
        if (clickCount == 0) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Select one query point. (Right-click)", 300, 50);
            
            if (!isInit) {
                if (!isSearchWithKDTree) searchNearestPoint(vboMesh); // 計算量多い
                else searchNearestPointWithKDTree(vboMesh); // kdTree
            }
            
        // ユーザが指定した点の描画
        } else {
            ofFill();
            ofSetColor(255, 0, 0);
            ofVec2f nearestPointPos2D = cam.worldToScreen(nearestPointPos);
            ofDrawCircle(nearestPointPos2D, 5);
        }
        
        // 選択した点を描画
        if (clickCount == 1) {
            // ユーザが選択した点の座標を表示
            ofSetColor(255, 0, 0);
            if (mousePos.x < ofGetWidth()*0.5) {
                verdana30.drawString("Query Point : ("+ofToString(nearestPointPos)+")", mousePos.x+30, mousePos.y+30);
            } else {
                verdana30.drawString("Query Point : ("+ofToString(nearestPointPos)+")", mousePos.x-700, mousePos.y+30);
            }
        }
        
        // ユーザが計数球の半径を選択し，主成分分析を実行する
        if (isSelectRadiusRange_5) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Step1. Select the radius of the count sphere. (Slider)", 300, 50);
            verdana30.drawString("- Step2. Press 'p' key, perform principal component analysis", 300, 90);
        }
        
        // 主成分分析の結果を表示
        if (isAfterPCA_5) {
            ofSetColor(255, 255, 0);
            verdana30.drawString("- Reset query point. (Right-click)", 300, 50);
            
            // 主成分分析の結果を表示
            if (isDrawPCAresult_5) displayPCAresult(); // 重い
        }
            
        gui5.draw();
    }
    
    
    // 常に表示
    ofDisableDepthTest();
    ofSetColor(255);
    //verdana30.drawString("clickCount : "+ofToString(clickCount), 100, ofGetHeight()-50);
    verdana40.drawString(ofToString(ofGetFrameRate())+" fps", ofGetWidth()*0.8, ofGetHeight()-50);
    if (isPLY1) verdana40.drawString(ofToString(vboMesh.getNumVertices())+" Points", ofGetWidth()*0.45, ofGetHeight()-50);
    if (isPLY2) verdana40.drawString(ofToString(vboMesh.getNumVertices())+" Points", ofGetWidth()*0.45, ofGetHeight()-50);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // モード1
    if (key == '1') {
        isRemovePointsMode = false;
        isCreateCrossSectionMode = false;
        isInterpolatePointsMode = false;
        isTargetRangePCAMode = false;
        isInterPointDistMode = true;
        
        clickCount = 0;
        vboMesh.clearColors();
    }
    
    // モード2
    if (key == '2') {
        isInterPointDistMode = false;
        isCreateCrossSectionMode = false;
        isInterpolatePointsMode = false;
        isTargetRangePCAMode = false;
        isRemovePointsMode = true;
        
        clickCount = 0;
        vboMesh.clearColors();
    }
    
    // モード3
    if (key == '3') {
        isInterPointDistMode = false;
        isRemovePointsMode = false;
        isInterpolatePointsMode = false;
        isTargetRangePCAMode = false;
        isCreateCrossSectionMode = true;
        
        clickCount = 0;
        pca.clear();
        vboMesh.clearColors();
    }
    
    // モード4
    if (key == '4') {
        isInterPointDistMode = false;
        isRemovePointsMode = false;
        isCreateCrossSectionMode = false;
        isTargetRangePCAMode = false;
        isInterpolatePointsMode = true;
        
        clickCount = 0;
        vboMesh.clearColors();
    }
    
    // モード5
    if (key == '5') {
        isInterPointDistMode = false;
        isRemovePointsMode = false;
        isCreateCrossSectionMode = false;
        isInterpolatePointsMode = false;
        isTargetRangePCAMode = true;
        
        clickCount = 0;
        pca.clear();
        vboMesh.clearColors();
    }
    
    // Switch Wire & Vert
    if (key == '0') {
        if (isWire) {
            isPoint = true;
            isWire = false;
            
        } else if (isPoint) {
            isWire = true;
            isPoint = false;
        }
    }
    
    // モデルの変更
    if (key == 'a') {
        clickCount = 0;
        kdTree.clear();
        indexes.clear();
        points.clear();
        vboMesh.clear();
        resetMode();
        
        if (isPLY1) {
            isPLY1 = false;
            //vboMesh.load("ply/dragon250.ply");
            vboMesh.load("ply/heritage.ply");
            isPLY2 = true;
            
        } else if (isPLY2) {
            isPLY2 = false;
            vboMesh.load("ply/bunny900.ply");
            isPLY1 = true;
        }
        
        calcPlySize(vboMesh);
        setKDTree(vboMesh);
        setGUI();
    }
    
    // マウスの入力を無効化
    if (key == 'c') {
        if(cam.getMouseInputEnabled()) cam.disableMouseInput();
        else cam.enableMouseInput();
    }
    
    // ダウンサンプリング
    if (key == 'd') {
        downSampling();
    }

    if (key == 'f') {
        ofToggleFullscreen();
    }
    
    // 断面図の生成
    if (key == 'g') {
        if (isSelectAxisValue_3) {
            isSelectAxisValue_3 = false;
            isCreateCrossSection_3 = true;
            extractPlane(vboMesh);
        }
    }
    
    // 補間
    if (key == 'i') {
        if (isInterpolatePointsMode && isSelectCertainRange_4) {
            isSelectCertainRange_4 = false;
            interpolatePointsInSphere();
            isAfterInterpolate_4 = true;
        }
    }
    
    // kdTreeを有効にするか無効にするか
    if (key == 'k') {
        isSearchWithKDTree = !isSearchWithKDTree;
    }
    
    // 主成分分析
    if (key == 'p') {
        if (isSelectRadiusRange_5 && !isAfterPCA_5) {
            ofVboMesh targetPointCloud;
            for (int i = 0; i < pointsInSphere.size(); i++) {
                targetPointCloud.addVertex( pointsInSphere[i] );
            }
            
            pca.calcEigenValueAndVector(targetPointCloud); // 各種計算
            eigenValues = pca.getEigenValues();
            firstEigenvector = pca.getEigenVectors()[0];
            secondEigenvector = pca.getEigenVectors()[1];
            thirdEigenvector = pca.getEigenVectors()[2];
            contributionRatios = pca.getContributionRatios();
            dimentionFeature = pca.getDimentionFeature();
            
            isAfterPCA_5 = true;
            isSelectRadiusRange_5 = false;
            sphereRadius = countSphereRadius; // 半径を保存
        }
    }
    
    // 指定範囲内の点を削除
    if (key == 'r') {
        if (clickCount == 2) {
            removePointsInRect(rangePoints[0], rangePoints[1]);
            clickCount = 0;
            
            // kdTreeをリセット
            kdTree.clear();
            indexes.clear();
            points.clear();
            setKDTree(vboMesh);
        }
    }
    
    // スクリーンショット
    if (key == 's') {
        isScreenshot = true;
    }
    
    // PLY出力
    if (key == 'v') {
        isSavePLY = true;
    }
}

// ユーザが指定した範囲内の点群を削除する関数
void ofApp::removePointsInRect(const ofVec2f& startPos, const ofVec2f& endPos) {
    for (int i = 0; i < vboMesh.getNumVertices(); i++) {
        ofVec2f pointPosScreen = cam.worldToScreen(vboMesh.getVertex(i));
        
        // 指定範囲内の点であれば削除
        if (pointPosScreen.x > startPos.x && pointPosScreen.y > startPos.y &&
            pointPosScreen.x < endPos.x && pointPosScreen.y < endPos.y) {
            
            vboMesh.removeVertex(i);
        }
    }
}

// 平面抽出（断面図の生成）
void ofApp::extractPlane(ofVboMesh& PointCloud) {
    ofVboMesh targetPointCloud;
    
    // ユーザが選択した範囲の点群の色を変える
    PointCloud.getColors().resize( PointCloud.getNumVertices() ); // 点群の色情報をリセット
    for (int i = 0; i < PointCloud.getNumVertices(); i++) {
        // X軸方向
        if (isDrawXAxis_3 && xAxisValue-offset < PointCloud.getVertex(i).x && PointCloud.getVertex(i).x < xAxisValue+offset) {
            PointCloud.setColor(i, ofFloatColor(1.0, 0.0, 0.0));
            targetPointCloud.addVertex(PointCloud.getVertex(i));
        }
        
        // Y軸方向
        if (isDrawYAxis_3 && yAxisValue-offset < PointCloud.getVertex(i).y && PointCloud.getVertex(i).y < yAxisValue+offset) {
            PointCloud.setColor(i, ofFloatColor(0.0, 1.0, 0.0));
            targetPointCloud.addVertex(PointCloud.getVertex(i));
        }
        
        // Z軸方向
        if (isDrawZAxis_3 && zAxisValue-offset < PointCloud.getVertex(i).z && PointCloud.getVertex(i).z < zAxisValue+offset) {
            PointCloud.setColor(i, ofFloatColor(0.0, 0.0, 1.0));
            targetPointCloud.addVertex(PointCloud.getVertex(i));
        }
    }
    
    // 対象範囲の点群の重心と法線ベクトルを計算
    pca.clear();
    pca.calcEigenValueAndVector(targetPointCloud);
    
    // 平面の方程式に代入して３次元座標を２次元平面上に落とす
    calcPlaneEquation(targetPointCloud, pca.getNormalVector(), pca.getCenterPos());
}

// 平面の方程式に代入して平面上に落とす
void ofApp::calcPlaneEquation(const ofVboMesh& targetPointCloud, const ofVec3f& normalVector, const ofVec3f& centerPos) {
    crossSectionPoints.clear(); // 点群情報をリセット
    crossSectionPoints.getColors().resize( targetPointCloud.getNumVertices() ); // 点群の色情報をリセット
    
    double x, y, z;
    
    if (isDrawXAxis_3) {
        for (int i = 0; i < targetPointCloud.getNumVertices(); i++) {
            y = targetPointCloud.getVertex(i).y;
            z = targetPointCloud.getVertex(i).z;
            x = ((normalVector.y*(centerPos.y-y) + normalVector.z*(centerPos.z-z)) / normalVector.x) + centerPos.x;
            
            crossSectionPoints.addVertex(ofVec3f(x, y, z));
            crossSectionPoints.setColor(i, ofFloatColor(1.0, 0, 0));
        }
    }

    if (isDrawYAxis_3) {
        for (int i = 0; i < targetPointCloud.getNumVertices(); i++) {
            x = targetPointCloud.getVertex(i).x;
            z = targetPointCloud.getVertex(i).z;
            y = ((normalVector.x*(centerPos.x-x) + normalVector.z*(centerPos.z-z)) / normalVector.y) + centerPos.y;
            
            crossSectionPoints.addVertex(ofVec3f(x, y, z));
            crossSectionPoints.setColor(i, ofFloatColor(0, 1.0, 0));
        }
    }
    
    if (isDrawZAxis_3) {
        for (int i = 0; i < targetPointCloud.getNumVertices(); i++) {
            x = targetPointCloud.getVertex(i).x;
            y = targetPointCloud.getVertex(i).y;
            z = ((normalVector.x*(centerPos.x-x) + normalVector.y*(centerPos.y-y)) / normalVector.z) + centerPos.z;
            
            crossSectionPoints.addVertex(ofVec3f(x, y, z));
            crossSectionPoints.setColor(i, ofFloatColor(0, 0, 1.0));
        }
    }
}

// KDTreeを用いて，計数球内の点群を探索する関数(探索するだけ．探索結果の可視化は別の関数．)
void ofApp::searchPointsInSphere(const ofVec3f& queryPointPos, int mode) {
    // STEP1 k=1としてKDTreeで最近傍点を探索
    // STEP2 最近傍点との距離と球の半径とを比較
    // STEP3 超えていれば，終了．
    int k = 1; // k=1は自分自身
    float dist = 0.0;
    do {
        getkNNWithKDTree(queryPointPos, k);
        dist = dists[k-1]; // 最も遠い距離を格納（distsの中身は近い順）
        
        k++;
        
    } while (dist < countSphereRadius*countSphereRadius);
    
    // 探索した点を保存
    if (mode == 4) {
        candidatePointsInSphere.clear();
        candidatePointsInSphere.reserve(k);
        for (int i = 1; i < k-1; i++) {
            // 「中点」を補完候補点とする
            ofVec3f tmpInterPoint;
            tmpInterPoint.x = (queryPointPos.x + points[indexes[i]][0]) * 0.5;
            tmpInterPoint.y = (queryPointPos.y + points[indexes[i]][1]) * 0.5;
            tmpInterPoint.z = (queryPointPos.z + points[indexes[i]][2]) * 0.5;
            
            candidatePointsInSphere.push_back(tmpInterPoint);
        }
        
        pointsInSphere.clear();
        pointsInSphere.reserve(k);
        for (int i = 1; i < k-1; i++) {
            ofVec3f tmp;
            tmp.x = points[indexes[i]][0];
            tmp.y = points[indexes[i]][1];
            tmp.z = points[indexes[i]][2];
            pointsInSphere.push_back(tmp);
        }
        
    } else if (mode == 5) {
        pointsInSphere.clear();
        pointsInSphere.reserve(k);
        for (int i = 1; i < k-1; i++) {
            ofVec3f tmp;
            tmp.x = points[indexes[i]][0];
            tmp.y = points[indexes[i]][1];
            tmp.z = points[indexes[i]][2];
            pointsInSphere.push_back(tmp);
        }
    }
}

// 計数球内の点の探索結果の描画（候補点(青色)を描画）（モード4）
void ofApp::drawPointsInSphere(vector<ofVec3f>& points) {
    ofFill();
    ofSetColor(0, 0, 255);
    for (int i = 1; i < points.size(); i++) {
        ofDrawSphere(points[i], 1.0);
    }
}

// 点群に補間点を追加する関数
void ofApp::interpolatePointsInSphere() {
    // 候補点から実際の補間点を抽出
    interpolationPoints.clear();
    for (int i = 0; i < candidatePointsInSphere.size(); i++) {
        // Step.1 KDTreeで最近傍点を探索
        // Step.2 最近傍点との距離が，一定範囲内に"存在しない場合"，補間点とする
        // Step.3 これをすべての候補点に対してチェック
        
        // k=1は自分自身，dists[0]は常に0
        getkNNWithKDTree(candidatePointsInSphere[i], 2);
        if (dists[1] > certainRange_4*certainRange_4) {
            // 補間点を保存
            interpolationPoints.push_back(candidatePointsInSphere[i]);
        }
    }
    
    // 確定した補間点を点群に追加
    for (int i = 0; i < interpolationPoints.size(); i++) {
        vboMesh.addVertex(interpolationPoints[i]);
    }
}

// 計数球内の点の探索結果の描画（線で結ぶ）（モード4,5）
void ofApp::drawLinesInSphere(const ofVec3f& queryPointPos, vector<ofVec3f>& points) {
    ofFill();
    ofSetColor(0, 255, 0);
    ofSetLineWidth(2);
    
    for (int i = 1; i < points.size(); i++) {
        ofDrawSphere(points[i], 1.0);
        ofDrawLine(queryPointPos, points[i]);
    }
}

// KDTreeでk-NN
void ofApp::getkNNWithKDTree(const ofVec3f& queryPointPos, int k) {
    // クエリ点の準備
    vector<double> queryPoint(3); // 3次元
    queryPoint[0] = queryPointPos.x;
    queryPoint[1] = queryPointPos.y;
    queryPoint[2] = queryPointPos.z;
    
    // KDTreeを実行
    kdTree.getKNN(queryPoint, k, indexes, dists);
}

// マウス位置に最も近い点を探す関数（総当り/計算量多い）
void ofApp::searchNearestPoint(const ofVboMesh& PointCloud) {
    float nearestDistance = 0;
    ofVec2f nearestPoint;
    
    // 最も近い点を探す（総当り）
    // indexを考慮しているため，ワールド座標系での座標も取得可能
    for (int i = 0; i < PointCloud.getNumVertices(); i++) {
        ofVec3f pointPosScreen = cam.worldToScreen(PointCloud.getVertex(i)); // ワールド座標系 → スクリーン座標系
        float distance = pointPosScreen.distance(mousePos); // スクリーン上での距離
        if (i == 0 || distance < nearestDistance) {
            nearestDistance = distance;
            nearestPoint = pointPosScreen;
            nearestIndex = i; // indexを保存
        }
    }
    
    // 最も近い点とマウス位置を線で結ぶ
    ofSetColor(255, 0, 0);
    ofSetLineWidth(5);
    ofDrawLine(nearestPoint, mousePos);
    
    // 最も近い点を強調
    ofFill();
    ofSetColor(255, 255, 0);
    ofDrawCircle(nearestPoint, 3);
    ofSetLineWidth(1);
    
    // 表示
    nearestPointPos = PointCloud.getVertex(nearestIndex);
    verdana30.drawString("Index : "+ofToString(nearestIndex), mousePos.x+30, mousePos.y);
    verdana30.drawString("Point : ("+ofToString(PointCloud.getVertex(nearestIndex))+")", mousePos.x+30, mousePos.y+40);
}

// マウス位置に最も近い点をKDTreeで探す
void ofApp::searchNearestPointWithKDTree(const ofVboMesh& PointCloud) {
    // クエリ点の準備（マウスの位置）
    vector<double> queryPoint(3); // 3次元
    ofVec3f tmpMousePos = mousePos;
    queryPoint[0] = cam.screenToWorld(tmpMousePos).x;
    queryPoint[1] = cam.screenToWorld(tmpMousePos).y;
    queryPoint[2] = cam.screenToWorld(tmpMousePos).z;
    
    // kdTreeでマウスに最も近い点を探索（3次元）
    kdTree.getKNN(queryPoint, 1, indexes, dists);
    nearestIndex = (int)indexes[0];
    
    ofVec2f nearestPointScreen;
    nearestPointScreen = cam.worldToScreen(PointCloud.getVertex(nearestIndex));
    
    
//    // クエリ点の準備(マウスの位置)
//    vector<double> queryPoint(2); // 2次元
//    queryPoint[0] = mousePos.x;
//    queryPoint[1] = mousePos.y;
//
//    // 点群の座標をスクリーン座標系に変換
//    // indexを考慮しているため，ワールド座標系での座標も取得可能
//    // スクリーン座標で最近傍点を探索し，インデックスを求めることで
//    kdTree.clear();
//    points.reserve(PointCloud.getNumVertices());
//    for (int i = 0; i < PointCloud.getNumVertices(); i++) {
//        vector<double> samplePoint(2); // 2次元
//        samplePoint[0] = cam.worldToScreen(PointCloud.getVertex(i)).x; // ワールド座標系 → スクリーン座標系
//        samplePoint[1] = cam.worldToScreen(PointCloud.getVertex(i)).y;
//
//        points.push_back(samplePoint);
//        kdTree.addPoint(samplePoint);
//    }
//    kdTree.constructKDTree();
//
//    // kdTreeでマウスに最も近い点を探索（2次元）
//    kdTree.getKNN(queryPoint, 1, indexes, dists);
//    nearestIndex = (int)indexes[0];
//
//    ofVec2f nearestPointScreen;
//    nearestPointScreen = cam.worldToScreen(PointCloud.getVertex(nearestIndex));
//
    // 最も近い点とマウス位置を線で結ぶ
    ofSetColor(255, 0, 0);
    ofSetLineWidth(5);
    ofDrawLine(nearestPointScreen, mousePos);

    // 最も近い点を強調
    ofFill();
    ofSetColor(255, 255, 0);
    ofDrawCircle(nearestPointScreen, 3);
    ofSetLineWidth(1);

    // 表示
    verdana30.drawString("Index : "+ofToString(indexes[0]), mousePos.x+30, mousePos.y);
    verdana30.drawString("Point : ("+ofToString(PointCloud.getVertex(nearestIndex))+")", mousePos.x+30, mousePos.y+40);
}

// 主成分分析の結果表示
void ofApp::displayPCAresult(){
#ifdef DISPLAY_1440_900
    // 網掛け
    ofSetColor(255, 150);
    ofDrawRectangle(ofGetWidth()*0.7-10, 100, ofGetWidth()*0.3, 700);
    
    // 表示
    ofSetColor(0);
    verdana30.drawString("<PCA Result>", ofGetWidth()*0.7, 150 );
    
    // 固有値
    verdana20.drawString("<Eigenvalues>", ofGetWidth()*0.7, 200);
    verdana20.drawString(ofToString(eigenValues), ofGetWidth()*0.7, 230);
    
    // 固有ベクトル
    ofSetColor(255, 0, 0);
    verdana20.drawString("<First Eigenvector (" +ofToString(eigenValues[0])+")>", ofGetWidth()*0.7, 330);
    verdana20.drawString("("+ofToString(firstEigenvector)+")", ofGetWidth()*0.7, 360);
    ofSetColor(0, 255, 0);
    verdana20.drawString("<Second Eigenvector (" +ofToString(eigenValues[1])+")>", ofGetWidth()*0.7, 400);
    verdana20.drawString("("+ofToString(secondEigenvector)+")", ofGetWidth()*0.7, 430);
    ofSetColor(0, 0, 255);
    verdana20.drawString("<Third Eigenvector (" +ofToString(eigenValues[2])+")>", ofGetWidth()*0.7, 470);
    verdana20.drawString("("+ofToString(thirdEigenvector)+")", ofGetWidth()*0.7, 500);
    
    // 寄与率
    ofSetColor(0);
    verdana20.drawString("<Contribution Ratios>", ofGetWidth()*0.7, 600);
    verdana20.drawString(ofToString(contributionRatios), ofGetWidth()*0.7, 630);
    
    // 次元特徴
    verdana20.drawString("<Dimention Feature>", ofGetWidth()*0.7, 730);
    verdana20.drawString(ofToString(dimentionFeature), ofGetWidth()*0.7, 760);
    
#else
    // 網掛け
    ofSetColor(255, 150);
    ofDrawRectangle(ofGetWidth()*0.7-10, 100, ofGetWidth()*0.3, 800);
    
    // 表示
    ofSetColor(0);
    verdana40.drawString("<PCA Result>", ofGetWidth()*0.7, 150 );
    
    // 固有値
    verdana30.drawString("<Eigenvalues>", ofGetWidth()*0.7, 200);
    verdana30.drawString(ofToString(eigenValues), ofGetWidth()*0.7, 240);
    
    // 固有ベクトル
    ofSetColor(255, 0, 0);
    verdana30.drawString("<First Eigenvector (" +ofToString(eigenValues[0])+")>", ofGetWidth()*0.7, 340);
    verdana30.drawString("("+ofToString(firstEigenvector)+")", ofGetWidth()*0.7, 380);
    ofSetColor(0, 255, 0);
    verdana30.drawString("<Second Eigenvector (" +ofToString(eigenValues[1])+")>", ofGetWidth()*0.7, 430);
    verdana30.drawString("("+ofToString(secondEigenvector)+")", ofGetWidth()*0.7, 470);
    ofSetColor(0, 0, 255);
    verdana30.drawString("<Third Eigenvector (" +ofToString(eigenValues[2])+")>", ofGetWidth()*0.7, 530);
    verdana30.drawString("("+ofToString(thirdEigenvector)+")", ofGetWidth()*0.7, 570);
    
    // 寄与率
    ofSetColor(0);
    verdana30.drawString("<Contribution Ratios>", ofGetWidth()*0.7, 670);
    verdana30.drawString(ofToString(contributionRatios), ofGetWidth()*0.7, 710);
    
//    // 曲率
//    verdana30.drawString("<Curvature>", ofGetWidth()*0.7, 790);
//    verdana30.drawString(ofToString(pca.getCurvature()), ofGetWidth()*0.7, 830);
    
    // 次元特徴
    verdana30.drawString("<Dimention Feature>", ofGetWidth()*0.7, 810);
    verdana30.drawString(ofToString(dimentionFeature), ofGetWidth()*0.7, 850);
#endif
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (isInit) isInit = false;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

void ofApp::mousePressed(int x, int y, int button){
    // モード1
    if (isInterPointDistMode && button == 2) {
        // １回目
        if (clickCount == 0) {
            interPoints.clear();
            clickCount++;
            // マウス位置に最も近い点の座標を保存
            interPoints.push_back(vboMesh.getVertex(nearestIndex));
        }
        
        // ２回目
        else if (clickCount == 1) {
            clickCount++;
            interPoints.push_back(vboMesh.getVertex(nearestIndex));
        }
        
        // ３回目
        else clickCount = 0;
    }
    
    // モード2
    if (isRemovePointsMode && button == 2) {
        // 1回目（始点）
        if (clickCount == 0) {
            //cam.disableMouseInput(); // 範囲選択中はマウスインプットを無効
            rangePoints.clear();
            clickCount++;
            // マウスの座標を保存
            rangePoints.push_back(mousePos);
        }
        
        // 2回目（終点）
        else if (clickCount == 1) {
            clickCount++;
            rangePoints.push_back(mousePos);
        }
        
        // 3回目
        else {
            clickCount = 0;
            cam.enableMouseInput();
        }
    }
    
    // モード3
    if (isCreateCrossSectionMode && button == 2) {
        // 抽出する範囲の選択に戻る
        if (isCreateCrossSection_3) {
            isSelectAxisValue_3 = true;
            isCreateCrossSection_3 = false;
            vboMesh.clearColors(); // 元の点群の色情報をリセット
        }
    }
    
    // モード4
    if (isInterpolatePointsMode && button == 2) {
        // 1回目（計数球の半径選択モードへ）
        if (clickCount == 0) {
            clickCount++;
            isSelectRadiusRange_4 = true;
            
        // 2回目（補間候補点に対する一定範囲の選択モードへ）
        } else if (clickCount == 1) {
            clickCount++;
            isSelectRadiusRange_4 = false;
            isSelectCertainRange_4 = true;
            
        // 3回目（クエリ点選択に戻る）
        } else if (clickCount == 2 && isAfterInterpolate_4) {
            clickCount = 0;
            
            isSelectCertainRange_4 = false;
            isAfterInterpolate_4 = false;
            countSphereRadius = 1.0;
            certainRange_4 = 1.0;
        }
    }
    
    // モード5
    if (isTargetRangePCAMode && button == 2) {
        // 1回目（計数球の半径選択モードへ）
        if (clickCount == 0) {
            clickCount++;
            isSelectRadiusRange_5 = true;
            
        // 2回目（クエリ点選択に戻る）
        } else if (clickCount == 1) {
            clickCount = 0;
            pca.clear();
            
            isSelectRadiusRange_5 = false;
            isAfterPCA_5 = false;
            countSphereRadius = 0.0;
            sphereRadius = 0.0;
        }
    }
}

// ダウンサンプリングを行う関数
void ofApp::downSampling() {
    int newPointNum = ceil(vboMesh.getNumVertices() * downRatio);
    
    // 遅い??
    // ユーザが指定した比率になるように点を削除する
    while(1) {
        vboMesh.removeVertex(ofRandom(vboMesh.getNumVertices()));
        if (vboMesh.getNumVertices() <= newPointNum) break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::changeModeCheck() {
    if (InterPointDistMode) {
        isRemovePointsMode = false;
        isCreateCrossSectionMode = false;
        isInterpolatePointsMode = false;
        isTargetRangePCAMode = false;
        isInterPointDistMode = true;
        
        clickCount = 0;
        vboMesh.clearColors(); // 元の点群の色情報をリセット
    }
    
    if (RemovePointsMode) {
        isInterPointDistMode = false;
        isCreateCrossSectionMode = false;
        isInterpolatePointsMode = false;
        isTargetRangePCAMode = false;
        isRemovePointsMode = true;
        
        clickCount = 0;
        vboMesh.clearColors(); // 元の点群の色情報をリセット
    }
    
    if (ExtractCrossSectionMode) {
        isInterPointDistMode = false;
        isRemovePointsMode = false;
        isInterpolatePointsMode = false;
        isTargetRangePCAMode = false;
        isCreateCrossSectionMode = true;
        
        clickCount = 0;
        pca.clear();
        vboMesh.clearColors(); // 元の点群の色情報をリセット
    }
    
    if (InterpolatePointsMode) {
        isInterPointDistMode = false;
        isRemovePointsMode = false;
        isCreateCrossSectionMode = false;
        isTargetRangePCAMode = false;
        isInterpolatePointsMode = true;
        
        clickCount = 0;
        vboMesh.clearColors(); // 元の点群の色情報をリセット
    }
    
    if (TargetRangePCAMode) {
        isInterPointDistMode = false;
        isRemovePointsMode = false;
        isCreateCrossSectionMode = false;
        isInterpolatePointsMode = false;
        isTargetRangePCAMode = true;
        
        clickCount = 0;
        pca.clear();
        vboMesh.clearColors(); // 元の点群の色情報をリセット
    }
}

// 点群のサイズを計算する関数
void ofApp::calcPlySize(ofVboMesh& PointCloud) {
    for (int i = 0; i < PointCloud.getNumVertices(); i++) {
        // Update max value
        if ( xMax < PointCloud.getVertex(i).x )
            xMax = PointCloud.getVertex(i).x;
        
        if ( yMax < PointCloud.getVertex(i).y )
            yMax = PointCloud.getVertex(i).y;
        
        if ( zMax < PointCloud.getVertex(i).z )
            zMax = PointCloud.getVertex(i).z;
        
        
        // Update min value
        if ( xMin > PointCloud.getVertex(i).x )
            xMin = PointCloud.getVertex(i).x;
        
        if ( yMin > PointCloud.getVertex(i).y )
            yMin = PointCloud.getVertex(i).y;
        
        if ( zMin > PointCloud.getVertex(i).z )
            zMin = PointCloud.getVertex(i).z;
    }
    
    // 枠の一辺の長さを計算
    xSize = xMax - xMin;
    ySize = yMax - yMin;
    zSize = zMax - zMin;
    vector<float> tmp = {xSize, ySize, zSize};
    ofSort(tmp); // 昇順にソート
    sideLength = tmp[2];
}

// 軸を描画する関数
void ofApp::drawAxis() {
    ofSetLineWidth(3);
    
    ofSetColor(255, 0, 0);
    ofDrawLine(0, 0, 0, sideLength*0.5, 0, 0);
    
    ofSetColor(0, 255, 0);
    ofDrawLine(0, 0, 0, 0, sideLength*0.5, 0);
    
    ofSetColor(0, 0, 255);
    ofDrawLine(0, 0, 0, 0, 0, sideLength*0.5);
}

// 全体の枠を描画する関数
void ofApp::drawFrame() {
    ofNoFill();
    ofSetColor(255);
    ofSetLineWidth(1);
    ofDrawBox(ofVec3f(0), sideLength);
}

// 抽出範囲を描画する関数
void ofApp::drawExtractRange() {
    ofFill();
    
    // x方向
    if (isDrawXAxis_3) {
        ofSetColor(255, 0, 0, 100);
        ofPushMatrix();
            ofRotateY(90);
            ofDrawBox(0, 0, xAxisValue, sideLength, sideLength, offset*2);
        ofPopMatrix();
        
    // y方向
    } else if (isDrawYAxis_3) {
        ofSetColor(0, 255, 0, 100);
        ofPushMatrix();
            ofRotateX(-90);
            ofDrawBox(0, 0, yAxisValue, sideLength, sideLength, offset*2);
        ofPopMatrix();
        
    // z方向
    } else if (isDrawZAxis_3) {
        ofSetColor(0, 0, 255, 100);
        ofPushMatrix();
            ofDrawBox(0, 0, zAxisValue, sideLength, sideLength, offset*2);
        ofPopMatrix();
    }
}

void ofApp::changeAxisCheck() {
    if (DrawXAxis) {
        isDrawXAxis_3 = true;
        isDrawYAxis_3 = false;
        isDrawZAxis_3 = false;
    }
    
    if (DrawYAxis) {
        isDrawYAxis_3 = true;
        isDrawXAxis_3 = false;
        isDrawZAxis_3 = false;
    }
    
    if (DrawZAxis) {
        isDrawZAxis_3 = true;
        isDrawXAxis_3 = false;
        isDrawYAxis_3 = false;
    }
}

void ofApp::resetMode() {
    // モード1
    if (isInterPointDistMode) {
        
    }
    
    // モード2
    if (isRemovePointsMode) {
        
    }
    
    // モード3
    if (isCreateCrossSectionMode) {
        isDrawXAxis_3 = false;
        isDrawYAxis_3 = true;
        isDrawZAxis_3 = false;
        isSelectAxisValue_3 = true;
        isCreateCrossSection_3 = false;
    }
    
    // モード4
    if (isInterpolatePointsMode) {
        isSelectRadiusRange_4 = true;
        isSelectCertainRange_4 = false;
        isAfterInterpolate_4 = false;
    }
    
    // モード5
    if (isTargetRangePCAMode) {
        isSelectRadiusRange_5 = false;
        isAfterPCA_5 = false;
    }
}
