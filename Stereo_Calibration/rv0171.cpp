#include <iostream>
#include <cmath>
#include <math.h>
#include <random>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "rv0171.h"

using namespace std;

// Read List of Points(x, y) from .txt file
vector<pair<double, double>> rv0171::PointList(string location)
{
    vector<pair<double, double>> pt;
    double x, y;

    ifstream fin;
    fin.open(location);
    while(!fin.eof())
    {
        fin >> x >> y;
        pt.push_back(make_pair(x, y));
    }
    if(fin.is_open()) fin.close();
    return pt;
}



//////////////////////////////
//// Zhang's Method (HW1) ////
//////////////////////////////
pair<double, double> rv0171::Normalization(double dX, double dY, KMatrix mat)
{
    //  dX, dX is x, y coordinate(double) of point, mat size is 2 x n
    // xmin, ymin, xmax, ymax(to make rectangle)
    double xmin = mat[0][0];
    double ymin = mat[1][0];
    double xmax = mat[0][0];
    double ymax = mat[1][0];
    for(unsigned int i = 1; i < mat.Col(); i++)
    {
        if(mat[0][i] < xmin) xmin = mat[0][i];
        if(mat[1][i] < ymin) ymin = mat[1][i];
        if(mat[0][i] > xmax) xmax = mat[0][i];
        if(mat[1][i] > ymax) ymax = mat[1][i];
    }

    // 1. shift-operation
    double xshift = dX - xmin;
    double yshift = dY - ymin;

    // 2. scale-operation
    double xlength = xmax - xmin;
    double ylength = ymax - ymin;

    double normalized_x = xshift / xlength;
    double normalized_y = yshift / ylength;

    return make_pair(normalized_x, normalized_y);
}

KMatrix rv0171::NormalizeCoordinates(KMatrix& M, KMatrix& normalized_M)
{
    KMatrix TM(3, 3);

    // 1. x_bar, y_bar (평균)
    double x_bar = 0;
    double y_bar = 0;
    for(unsigned int i = 0; i < M.Col(); i++)
    {
        x_bar += M[0][i];
        y_bar += M[1][i];
    }
    x_bar /= M.Col();
    y_bar /= M.Col();

    // 2. x', y'
    KMatrix temp_mM;
    for(unsigned int i=0; i < M.Col(); i++)
        temp_mM |= KVector(M[0][i] - x_bar, M[1][i] - y_bar); // 2 x 156

    // 3. s
    double s=0;
    double sum=0;
    for(unsigned int  i=0 ; i < temp_mM.Col(); i++)
        sum += sqrt(_SQR(temp_mM[0][i]) + _SQR(temp_mM[1][i]));
    s = sqrt(2) * temp_mM.Col() / sum;

    // 4. get T
    TM[0][0] =   s; TM[0][1] = 0.0; TM[0][2] = -s * x_bar;
    TM[1][0] = 0.0; TM[1][1] =   s; TM[1][2] = -s * y_bar;
    TM[2][0] = 0.0; TM[2][1] = 0.0; TM[2][2] =        1.0;

    normalized_M = TM * M;
    return TM;
}


KVector rv0171::MakevV(KMatrix mH, int i, int j)
{
    KVector vV(6);

    i -= 1;
    j -= 1;

    vV[0] = mH[0][i] * mH[0][j];
    vV[1] = mH[0][i] * mH[1][j] + mH[1][i] * mH[0][j];
    vV[2] = mH[1][i] * mH[1][j];
    vV[3] = mH[2][i] * mH[0][j] + mH[0][i] * mH[2][j];
    vV[4] = mH[2][i] * mH[1][j] + mH[1][i] * mH[2][j];
    vV[5] = mH[2][i] * mH[2][j];

    return vV;
}

KVector rv0171::ZhangsCalibration(vector<vector<pair<double, double>>> pointzip, const int nImg, const int nFeature, const int nItr)
{
    // 1. Estimate the Homography H
    KList<KMatrix> lmF;
    KMatrix mFn, mMn;
    KMatrix mTf, mTm;

    for(int i = 0; i < nImg; i++)
    {
        KMatrix mF;
        for(int j = 0; j < nFeature; j++)
            mF |= KVector(pointzip[i][j].first, pointzip[i][j].second, 1.0);    // Feature matrix
        lmF.Add(mF);
    }

    KMatrix mM;
    for(int i = 0; i < nFeature; i++)
        mM |= KVector(pointzip[nImg][i].first, pointzip[nImg][i].second, 1.0);  // Model matrix

    double mFn_x[nFeature];
    double mFn_y[nFeature];
    double mMn_x[nFeature];
    double mMn_y[nFeature];

    // Mn
    for(int i = 0; i < nFeature; i++)
    {
        mMn_x[i] = rv0171::Normalization(mM[0][i], mM[1][i], mM).first;
        mMn_y[i] = rv0171::Normalization(mM[0][i], mM[1][i], mM).second;
    }

    for(int i = 0; i < nFeature; i++)
    {
        mMn |= KVector(mMn_x[i], mMn_y[i], 1.0);    // Normalized model matrix
    }

    // Tm
    mTm = rv0171::NormalizeCoordinates(mM, mMn);    // Normalized model transform matrix

    // F(iterative)
    KList<KMatrix> lmH;
    KMatrix mV;

    for(int i = 0; i < nImg; i++)
    {
        for(int j = 0; j < nFeature; j++)
        {
            // refresh mFn_x, mFn_y for every image
            mFn_x[j] = rv0171::Normalization(lmF[i][0][j], lmF[i][1][j], lmF[i]).first;
            mFn_y[j] = rv0171::Normalization(lmF[i][0][j], lmF[i][1][j], lmF[i]).second;
        }

        for(int j = 0; j < nFeature; j++)
            mFn |= KVector(mFn_x[j], mFn_y[j], 1.0);        // Normalized feature matrix

        // Tf
        mTf = rv0171::NormalizeCoordinates(lmF[i], mFn);    // Normalized feature transform matrix

        // Form the matrix A such that ||Ah||=0, ||h||=1, (A is not intrinsic)
        KMatrix mA; // 3x9 matrix
        for(int j = 0; j < nFeature; j++)
        {
            KVector vM;
            // mMn: 3 x 156
            vM = KVector(mMn[0][j], mMn[1][j], 1.0); // Transpose in Advance
            // ^= 행단위로 추가
            mA ^= KVector(0.0, 0.0, 0.0).Tail(-vM).Tail(vM * mFn[1][j]).Tr();                  // 1 x 9, ^= 열단위 추가: (0^T, -mi^T, vi * mi^T)
            mA ^= vM.Tail(KVector(0.0, 0.0, 0.0)).Tail(vM * (-mFn[0][j])).Tr();                // ^= 열단위 추가: (mi^T, 0^T, -ui * mi^T)
            mA ^= (vM * (-mFn[1][j])).Tail(vM * mFn[0][j]).Tail(KVector(0.0, 0.0, 0.0)).Tr();  // ^= 열단위 추가: (-vi * mi^T, ui * mi^T, 0^T)
        }

        KMatrix _mU, _mV, mHn, mH;
        KVector _vD, _vH;

        // Apply SVD
        mA.SVD(_mU, _vD, _mV);

        // last col composes H
        _vH = _mV.Column(_mV.Col() - 1);

        // make H(normalized)
        mHn = (_vH.Cut(0, 2).Tr() ^ _vH.Cut(3, 5).Tr() ^ _vH.Cut(6, 8).Tr());

        // undo the normalization
        mH = ~mTf * mHn * mTm;
        // scale to make mH[2][2]=1.0
        mH /= mH[2][2];

        lmH.Add(mH);

        // 2. Form the matrix V as shown in the previous lecture
        KVector vV11, vV12, vV22;
        vV11 = rv0171::MakevV(mH, 1, 1);
        vV12 = rv0171::MakevV(mH, 1, 2);
        vV22 = rv0171::MakevV(mH, 2, 2);

        mV ^= vV12.Tr();                        // 1x6
        mV ^= (vV11-vV22).Tr();                 // 1x6
    }

    // 3. Solve Vb=0, then form the matrix B
    KMatrix mU, mW;
    KVector vD, vB;

    mV.SVD(mU, vD, mW);
    vB = mW.Column(mW.Col() - 1);

    KMatrix mB(3, 3);
    mB[0][0] = vB[0]; mB[0][1] = vB[1]; mB[0][2] = vB[3];
    mB[1][0] = vB[1]; mB[1][1] = vB[2]; mB[1][2] = vB[4];
    mB[2][0] = vB[3]; mB[2][1] = vB[4]; mB[2][2] = vB[5];

    // 4. Obtain the intrinsic parameters from B=r(A^-T)(A^-1)
    KMatrix _mA(3, 3);   // 3x3 calibration matrix
    double dLambda;
    _mA[1][2] = (mB[0][1]*mB[0][2] -  mB[0][0]*mB[1][2]) / (mB[0][0]*mB[1][1] - mB[0][1]*mB[0][1]);             // v0
    dLambda = mB[2][2] - (_SQR(mB[0][2]) + _mA[1][2] * (mB[0][1]*mB[0][2] - mB[0][0]*mB[1][2])) / mB[0][0];     // Lambda
    _mA[0][0] = sqrt(dLambda / mB[0][0]);                                                                       // alpha
    _mA[1][1] = sqrt(dLambda * mB[0][0] / (mB[0][0]*mB[1][1] - _SQR(mB[0][1])));                                // beta
    _mA[0][2] = -mB[0][2] * _SQR(_mA[0][0]) / dLambda;                                                          // u0
    _mA[2][2] = 1.0;

    // 5. Obtain the extrinsic parameters from H=[h1 h2 h3]=n*A[r1 r2 t]
    KList<KMatrix> lmR;
    KList<KVector> lvT;
    for(int i= 0; i < nImg; i++)
    {
        double dSize;
        KVector vR1, vR2, vR3, _vT;

        vR1 = _mA.Iv() * lmH[i].Column(0); vR1.Normalized(_SIZE_NORMALIZE, &dSize);
        vR2 = _mA.Iv() * lmH[i].Column(1); vR2.Normalized(_SIZE_NORMALIZE);
        vR3 = vR1.Skew() * vR2;
        _vT = 1 / dSize * _mA.Iv() * lmH[i].Column(2);

        //orthogonalization
        KMatrix _mR = (vR1 | vR2 | vR3);
        _mR.SVD(mU, vD, mW);
        _mR = mU * mW.Tr();

        lmR.Add(_mR);
        lvT.Add(_vT);
    }

    // 6. Perform the nonlinear optimization for fine-tuning of the parameters including the lens distortion parameters
    // vX: 6x1 vector (alpha, beta, u0, v0, k1, k2)
    KVector vX;

    vX.Tailed(_mA[0][0]); vX.Tailed(_mA[1][1]); // alpha, beta
    vX.Tailed(_mA[0][2]); vX.Tailed(_mA[1][2]); // u0, v0

    vX.Tailed(KVector(0.0, 0.0));   // k1, k2

    for (int i = 0; i < nImg; i++)
    {
        KRotation rR(lmR[i]);
        KHomogeneous hP(rR, lvT[i]);
        //vX.Tailed(hP.EulerTranslation());
        vX.Tailed(hP.R().Rodrigues());              // R1, R2, R3
        vX.Tailed(lvT[i]);                          // Tx, Ty, Tz
    }
    // vX: 48x1

    // optimization
    KCalibrationZhang zhang;
    zhang.mM = mM;
    for(int i = 0; i < nImg; i++) zhang.mF[i] = lmF[i];
    zhang.nImg = nImg;
    zhang.nFeature = nFeature;
    zhang.Powell(vX, 0.05, nItr);

    vX.Tailed(zhang.dError);
    // vX: (alpha, beta, u0, v0, k1, k2) + (R1, R2, R3, Tx, Ty, Tz)*7 + dError => 49x1

    return vX;
}

double KCalibrationZhang::Erf(const KVector& vX)
{
    double dError = 0;

    KMatrix mA(3, 3);   // intrinsic matrix
    mA[0][0] = vX[0];   // alpha
    mA[1][1] = vX[1];   // beta
    mA[0][2] = vX[2];   // u0
    mA[1][2] = vX[3];   // v0
    mA[2][2] = 1.0;     // scale factor
    double dK1 = vX[4]; // distortion parameter k1
    double dK2 = vX[5]; // distortion parameter k2

    KHomogeneous lP[nImg];
    for (int i = 0; i < nImg; i++)
    {
        KRotation rR;
        rR.FromRodrigues(vX[6*(i+1)], vX[6*(i+1)+1], vX[6*(i+1)+2]);
        KVector vT(vX[6*(i+1)+3], vX[6*(i+1)+4], vX[6*(i+1)+5]);
        lP[i] = KHomogeneous(rR, vT);
    }

    double dR2;
    KVector vXc, vXn, vU, vDr;
    // double sigma
    for(int i = 0; i < nImg; i++)
    {
        for(int j = 0; j < nFeature; j++)
        {
            vXc = (lP[i].R() * mM.Column(j) + lP[i].t());       // 3x1 matrix
            vXn = (vXc / vXc[2]).To(1);                         // 2x1 matrix

            // Radial distortion
            dR2 = _SQR(vXn[0]) + _SQR(vXn[1]);                  // double
            vDr = vXn*(1.0 + dK1 * dR2 + dK2 * dR2 * dR2);      // 2x1 matrix

            // Final distortion
            vXc = vDr.Tailed(1.0);                              // 3x1 matrix

            // image projection
            vU = mA * vXc; vU /= vU[2];                         // 3x1 matrix

            //error computation
            dError += (_SQR(mF[i][0][j] - vU[0]) + _SQR(mF[i][1][j] - vU[1]));
        }
    }
    //dError /= nImg * nFeature;
    this->dError = dError;

    cout << "Epoch:" << cnt++ \
         << " | Alpha:" << vX[0] << " Beta:" << vX[1] \
         << " u0:" << vX[2] << " v0:" << vX[3] \
         << " k1:" << vX[4] << " k2:" << vX[5] \
         << " Error:" << dError << endl;
    return dError;
}



//////////////////////////////
// Stereo Calibration (HW2) //
//////////////////////////////
KVector rv0171::PointProjection(KMatrix mA, KVector vK, KHomogeneous hP, KVector vM)
{
    // mA: Intrinsic parameter, vK: Distortion parameter, hP: Homogeneous matrix, vM: Model feature
    // Projection
    double dR2;
    KVector vXc, vXn, vU, vDr;
    vXc = hP.R() * vM + hP.t();     // 3x1 matrix
    vXn = (vXc / vXc[2]).To(1);     // 2x1 matrix
    // Radial distortion
    dR2 = _SQR(vXn[0]) + _SQR(vXn[1]);
    vDr = vXn * (1.0 + vK[0] * dR2 + vK[1] * pow(dR2, 2));  // 2x1 matrix
    // Final distortion
    vXc = vDr.Tailed(1.0);                              // 3x1 matrix
    // Image projection
    vU = mA * vXc;      vU /= vU[2];                    // 3x1 matrix, {ud, vd, 1.0}

    return vU;
}

KMatrix rv0171::ImageProjection(KMatrix mA, KVector vK, KHomogeneous hP, KMatrix mM)
{
    // mA: Intrinsic parameter, vK: Distortion parameter, hP: homogeneous matrix, mM:Model features
    int nFeature = mM.Size() / 3;
    KMatrix mF; // Projected points

    for(int i = 0; i < nFeature; i++)
    {
        // Projection
        double dR2;
        KVector vXc, vXn, vU, vDr;
        vXc = hP.R() * mM.Column(i) + hP.t();     // 3x1 matrix
        vXn = (vXc / vXc[2]).To(1);     // 2x1 matrix

        // Radial distortion
        dR2 = _SQR(vXn[0]) + _SQR(vXn[1]);
        vDr = vXn*(1.0 + vK[0] * dR2 + vK[1] * pow(dR2, 2));    // 2x1 matrix

        // Final distortion
        vXc = vDr.Tailed(1.0);          // 3x1 matrix

        // Image Projection
        vU = mA * vXc; vU /= vU[2];     // 3x1 matrix(scaled)

        mF |= vU;   // 3xnFeature matrix, add col
    }

    return mF;
}

KVector rv0171::StereoCalibration(KVector vXl, KVector vXr, vector<vector<pair<double, double>>> psFl, vector<vector<pair<double, double>>> psFr, const int nImg, const int nFeature, const int nItr)
{
    // Left image points
    KList<KMatrix> lpFl;    // Feature matrix for each image
    for (int i = 0; i < nImg; i++)
    {
        // For each image
        KMatrix mF;         // 3 x nFeature matrix
        for (int j = 0; j < nFeature; j++)
            mF |= KVector(psFl[i][j].first, psFl[i][j].second, 1.0); // Add a column
        lpFl.Add(mF);
    }

    // Right image points
    KList<KMatrix> lpFr;    // Feature matrix for each image
    for (int i = 0; i < nImg; i++)
    {
        // For each image
        KMatrix mF;         // 3 x nFeature matrix
        for (int j = 0; j < nFeature; j++)
            mF |= KVector(psFr[i][j].first, psFr[i][j].second, 1.0); // Add a column
        lpFr.Add(mF);
    }

    // Model points
    KMatrix mpM;             // 3 x mFeature matrix
    for (int i = 0; i < nFeature; i++)
        mpM |= KVector(psFr[nImg][i].first, psFr[nImg][i].second, 1.0);             // Add a column

    KCalibrationStereo stereo_calib;
    stereo_calib.vXl = vXl;
    stereo_calib.vXr = vXr;

    stereo_calib.nImg = nImg;
    stereo_calib.nFeature = nFeature;

    // update mAl, mAr, hRw, hLw, hLr, vKl, vKr

    KMatrix mAl(3, 3), mAr(3, 3);
    KVector vKl, vKr;

    // _mAl, _mAr, _vKl, _vKr
    mAl[0][0] = vXl[0];    // alpha
    mAl[1][1] = vXl[1];    // beta
    mAl[0][2] = vXl[2];    // u0
    mAl[1][2] = vXl[3];    // v0
    mAl[2][2] = 1.0;       // scale factor

    mAr[0][0] = vXr[0];    // alpha
    mAr[1][1] = vXr[1];    // beta
    mAr[0][2] = vXr[2];    // u0
    mAr[1][2] = vXr[3];    // v0
    mAr[2][2] = 1.0;       // scale factor

    stereo_calib._mAl = mAl;
    stereo_calib._mAr = mAr;

    // k1, k2
    stereo_calib._vKl = KVector(vXl[4], vXl[5]);
    stereo_calib._vKr = KVector(vXr[4], vXr[5]);

    KHomogeneous* hLw; // hLw
    hLw = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vXl[6*(i+1)], vXl[6*(i+1)+1], vXl[6*(i+1)+2]);
        KVector   vT(vXl[6*(i+1)+3], vXl[6*(i+1)+4], vXl[6*(i+1)+5]);
        hLw[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->TL
    }

    KHomogeneous* hRw; // hRw
    hRw = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vXr[6*(i+1)], vXr[6*(i+1)+1], vXr[6*(i+1)+2]);
        KVector   vT(vXr[6*(i+1)+3], vXr[6*(i+1)+4], vXr[6*(i+1)+5]);
        hRw[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->TR
    }

    KHomogeneous hLr;
    hLr = hLw[14] * ~hRw[14];

    stereo_calib._lpFl = &lpFl;
    stereo_calib._lpFr = &lpFr;
    stereo_calib._mpM = &mpM;

    stereo_calib._hLw = hLw;
    stereo_calib._hRw = hRw;
    stereo_calib._hLr = hLr;

    KVector vH;
    vH.Tailed(stereo_calib._hLr.R().Rodrigues());   // 3x1
    vH.Tailed(stereo_calib._hLr.t());               // 3x1

    stereo_calib.Powell(vH, 0.05, nItr);
    vH.Tailed(stereo_calib.dError);

    return vH;
}

double KCalibrationStereo::Erf(const KVector& vH)
{
    const KList<KMatrix>& lFl   = *_lpFl;   // list point Feature left
    const KList<KMatrix>& lFr   = *_lpFr;   // list point Feature right
    const KMatrix& mM           = *_mpM;    // matrix point Model

    KRotation rR;
    rR.FromRodrigues(vH[0], vH[1], vH[2]);
    KVector   vT(vH[3], vH[4], vH[5]);
    _hLr = KHomogeneous(rR, vT);
    _hRl = ~_hLr;

    double dError = 0.0;

    for (unsigned int i = 0; i < lFr.Count(); i++)  // // lFr.Count() = 15
    {
        // For each images
        // Right image projection
        KMatrix mFl = rv0171::ImageProjection(_mAl, _vKl, _hLr * _hRw[i], mM);     // 3 x nFeature
        KMatrix mFr = rv0171::ImageProjection(_mAr, _vKr, _hRl * _hLw[i], mM);     // 3 x nFeature
        // (--, --, 1.0)
        mFl /= KMatrix::RepMat(mFl.RowVec(2), 3, 1);
        mFr /= KMatrix::RepMat(mFr.RowVec(2), 3, 1);
        // Error
        KMatrix mEl = lFl[i] - mFl;
        KMatrix mEr = lFr[i] - mFr;
        // 3 x nFeature
        // Element-wise square
        mEl ^= 2.0;
        mEr ^= 2.0;
        // Norm
        dError += (mEl.Sum().Sum() + mEr.Sum().Sum());
    }
    cout << "Epoch:" << cnt++ \
         << " R1:" << vH[0] << " R2:" << vH[1] \
         << " R3:" << vH[2] << " t1:" << vH[3] \
         << " t2:" << vH[4] << " t3:" << vH[5] \
         << " Error:" << dError << endl;

    return dError;
}
