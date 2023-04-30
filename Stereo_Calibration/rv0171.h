#ifndef RV0171_H
#define RV0171_H

#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <fstream>

#include "kfc.h"
#include "QDebug"

#define __X 0
#define __Y 1

#define __PI 3.141592
#define __2PI 6.283184

#define __MonitorWidth 1920
#define __MonitorHeight 1080

using namespace std;

class KFeatureHaarlike;

namespace rv0171
{
    // Read List of Points(x, y) from .txt file
    vector<pair<double, double>> PointList(string location);

    // Zhang's Method (HW1)
    pair<double, double> Normalization(double dX, double dY, KMatrix mat);  // 2D Normalization of each pts in mat
    KMatrix NormalizeCoordinates(KMatrix& M, KMatrix& normalized_M);
    KVector MakevV(KMatrix mH, int i, int j);
    KVector ZhangsCalibration(vector<vector<pair<double, double>>> pointzip, const int nImg, const int nFeature, const int iter = 500);

    // Stereo Camera Calibration (HW2)
    KVector StereoCalibration(KVector vXl, KVector vXr, vector<vector<pair<double, double>>> psFl, vector<vector<pair<double, double>>> psFr, const int nImg, const int nFeature, const int nItr=500);
    KVector PointProjection(KMatrix mA, KVector vK, KHomogeneous hP, KVector vM);
    KMatrix ImageProjection(KMatrix mA, KVector vK, KHomogeneous hP, KMatrix mM);
}



// Camera Calibration(HW1)
class KCalibrationZhang : public KOptima
{
public:
    KMatrix mM;         // model world coordinate
    KMatrix mF[15];      // feature image coordinate

    int nImg;
    int nFeature;

    int cnt = 0;
    double dError = 0;

protected:
    virtual double Erf(const KVector& vX);
};



// Stereo Camera Calibration(HW2)
class KCalibrationStereo : public KOptima
{
public:
    KVector         vXl, vXr;
    KMatrix         _mAl, _mAr;         // 4 Dim
    KHomogeneous*   _hLw;
    KHomogeneous*   _hRw;
    KHomogeneous    _hLr, _hRl;   // Xr = hRw * Xw, Xl = hLr * Xl
    KVector         _vKl, _vKr;

    int nImg, nFeature;
    int cnt = 0;
    double dError;

    //temporal
    KList<KMatrix>*     _lpFl;  // list point Feature left
    KList<KMatrix>*     _lpFr;  // list point Feature right
    KMatrix*            _mpM;   // matrix point Model

public:
    KCalibrationStereo(){ }

protected:
//    void UpdateParameters(const int nImg);

    virtual double Erf(const KVector& vH);
};

#endif // RV0171_H
