#include <QFileDialog>
#include <QmessageBox>
#include <QPainter>
#include <sstream>
#include <fstream>
#include <QString>
#include <QImageReader>
#include <random>

#include "mainframe.h"
#include "ui_mainframe.h"
#include "imageform.h"
#include "rv0171.h"

using namespace std;
namespace fs = std::filesystem;

string rvdir = "C:/Stereo_Calibration/Stereo_Calibration/data/";

MainFrame::MainFrame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainFrame)
{
    ui->setupUi(this);

    _q_pFormFocused     = 0;

    //객체 맴버의 초기화


    //리스트 출력창을 안보이게    
    ui->listWidget->setVisible(false);
    this->adjustSize();

    //UI 활성화 갱신
    UpdateUI();
}

MainFrame::~MainFrame()
{ 
    delete ui;         

    for(auto& item : _lImageForm)
        delete item;

}

void MainFrame::CloseImageForm(ImageForm *pForm)
{
    //ImageForm 포인터 삭제
    unsigned int idx = std::find(_lImageForm.begin(), _lImageForm.end(), pForm) - _lImageForm.begin();
    if(idx != _lImageForm.size())
    {
        delete _lImageForm[idx];
        _lImageForm.erase(_lImageForm.begin() + idx);
    }

    //활성화 ImageForm 초기화
    _q_pFormFocused     = 0;

    //관련 객체 삭제

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::UpdateUI()
{    
    if(ui->tabWidget->currentIndex() == 0)
    {
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {  
    }
    else if(ui->tabWidget->currentIndex() == 2)
    {        
    }
    else if(ui->tabWidget->currentIndex() == 3)
    {
    }
}

void MainFrame::OnMousePos(const int &nX, const int &nY, ImageForm* q_pForm)
{
}

void MainFrame::focusInEvent(QFocusEvent * event)
{
    Q_UNUSED(event) ;

    UpdateUI();
}

void MainFrame::closeEvent(QCloseEvent* event)
{
    //생성된 ImageForm을 닫는다.
    for(unsigned int i=0; i< _lImageForm.size(); i++)
        delete _lImageForm[i];

    //리스트에서 삭제한다.
    _lImageForm.clear();
}


void MainFrame::on_buttonOpen_clicked()
{
    //이미지 파일 선택
    QFileDialog::Options    q_Options   =  QFileDialog::DontResolveSymlinks  | QFileDialog::DontUseNativeDialog; // | QFileDialog::ShowDirsOnly
    QString                 q_stFile    =  QFileDialog::getOpenFileName(this, tr("Select a Image File"),  "./data", "Image Files(*.bmp *.ppm *.pgm *.png)",0, q_Options);

    if(q_stFile.length() == 0)
        return;

    //이미지 출력을 위한 ImageForm 생성    
    ImageForm*              q_pForm   = new ImageForm(q_stFile, "OPEN", this);

    _lImageForm.push_back(q_pForm);
    q_pForm->show();

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_buttonDeleteContents_clicked()
{
    //생성된 ImageForm을 닫는다.
    for(int i=_lImageForm.size()-1; i>=0; i--)
        delete _lImageForm[i];

    //리스트에서 삭제한다.
    _lImageForm.clear();

    ui->listWidget->clear();
}

void MainFrame::on_tabWidget_currentChanged(int index)
{
    static int nOld = -1;

    if(nOld == 0)
    {

    }
    else if(nOld == 1)
    {

    }
    nOld = index;

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_buttonShowList_clicked()
{
    static int nWidthOld = ui->tabWidget->width();

    if(ui->listWidget->isVisible())
    {
        nWidthOld = ui->listWidget->width();
        ui->listWidget->hide();
        this->adjustSize();
    }
    else
    {        
        ui->listWidget->show();
        QRect q_rcWin = this->geometry();

        this->setGeometry(q_rcWin.left(), q_rcWin.top(), q_rcWin.width()+nWidthOld, q_rcWin.height());
    }
}



//////////////////////////////
//// Zhang's Method (HW1) ////
//////////////////////////////
void MainFrame::on_zhang_clicked()
{
    int nImg = 7;
    int nFeature = 156;
    int nItr = 1000;

    // Load Image & Model Points to vector
    vector<vector<pair<double, double>>> pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "Mono_Calibration_data/Image";
            dir += to_string(i);
            dir += ".txt";
            pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "Mono_Calibration_data/Model.txt";
            pointzip.push_back(rv0171::PointList(dir));
        }
    }

    KVector vX;

    vX = rv0171::ZhangsCalibration(pointzip, nImg, nFeature, nItr);

    // Reprojection
    KMatrix mA(3, 3);
    mA[0][0] = vX[0];   // alpha
    mA[0][1] = 0.0;
    mA[1][1] = vX[1];   // beta
    mA[0][2] = vX[2];   // u0
    mA[1][2] = vX[3];   // v0
    mA[2][2] = 1.0;     // scale
    double dK1 = vX[4]; // k1
    double dK2 = vX[5]; // k2
    double dError = vX[vX.Size() - 1];  // vX's last element is Err

    //  Print calibration parameters
    QString temp_str;
    temp_str = QString::fromStdString(std::to_string(mA[0][0]));
    ui->textAlpha->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA[0][1]));
    ui->textSkew->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA[0][2]));
    ui->textU0->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA[1][1]));
    ui->textBeta->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA[1][2]));
    ui->textV0->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK1));
    ui->textK1->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK2));
    ui->textK2->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dError));
    ui->textError->setText(temp_str);

    //UI 활성화 갱신
    UpdateUI();
}

// 체커보드 점을 띄운 검은 이미지 출력
void MainFrame::on_plot_point_clicked()
{
    int nImg = 7;
    int nFeature = 156;
    int row = 1300;
    int col = 1300;

    // Load Image & Model Points to vector
    vector<vector<pair<double, double>>> pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "Mono_Calibration_data/Image";
            dir += to_string(i);
            dir += ".txt";
            pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "Mono_Calibration_data/Model.txt";
            pointzip.push_back(rv0171::PointList(dir));
        }
    }

    KImageColor Image[nImg + 1];
    ImageForm* q_pForm[nImg + 1];
    for(int i = 0; i < nImg + 1; i++){
        string f_name = "";
        Image[i].Create(row, col);
        if(i != nImg) {
            f_name += "Image";
            f_name += to_string(i + 1);
            q_pForm[i] = new ImageForm(Image[i], QString::fromStdString(f_name), this);
        }
        else{
            f_name += "Model";
            q_pForm[i] = new ImageForm(Image[i], QString::fromStdString(f_name), this);
        }
    }

    for(int i = 0; i < nImg + 1; i++){
        for(int j = 0; j < nFeature; j++){
            q_pForm[i]->DrawLine(pointzip[i][j].first, pointzip[i][j].second, pointzip[i][j].first, pointzip[i][j].second, QColor(255, 0, 0), 2);
        }
    }

    for(int i = 0; i < nImg + 1; i++){
        _lImageForm.push_back(q_pForm[i]);
        q_pForm[i]->show();
    }

    //UI 활성화 갱신
    UpdateUI();
}



//////////////////////////////
// Stereo Calibration (HW2) //
//////////////////////////////
void MainFrame::on_stereo_calibration_clicked()
{
    int nImg = 15;
    int nFeature = 100; // Feature점 개수: 100
    int nItr = 100;

    // 1. Capture stereo pairs
    vector<vector<pair<double, double>>> left_pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "Stereo_Calibration_data/calib_data/left";
            dir += to_string(i);
            dir += ".txt";
            left_pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "Stereo_Calibration_data/calib_data/model.txt";
            left_pointzip.push_back(rv0171::PointList(dir));
        }
    }

    vector<vector<pair<double, double>>> right_pointzip;
    for(int i = 1; i <= nImg + 1; i++){
        string dir = rvdir;
        if(i != nImg + 1) {
            dir += "Stereo_Calibration_data/calib_data/right";
            dir += to_string(i);
            dir += ".txt";
            right_pointzip.push_back(rv0171::PointList(dir));
        }
        else {
            dir += "Stereo_Calibration_data/calib_data/model.txt";
            right_pointzip.push_back(rv0171::PointList(dir));
        }
    }
    KVector vX_l, vX_r;

    // 2. Calibrate separately the two cameras using Zhang's method
    vX_l = rv0171::ZhangsCalibration(left_pointzip, nImg, nFeature, nItr);
    vX_r = rv0171::ZhangsCalibration(right_pointzip, nImg, nFeature, nItr);

    KMatrix mA_l(3, 3); // left camera intrinsic matrix
    mA_l[0][0] = vX_l[0];
    mA_l[1][1] = vX_l[1];
    mA_l[0][2] = vX_l[2];
    mA_l[1][2] = vX_l[3];
    mA_l[2][2] = 1.0;
    double dK1_l = vX_l[4];
    double dK2_l = vX_l[5];
    double dError_l = vX_l[vX_l.Size() - 1];  // vX's last element is Err

    //  Print left calibration parameters
    QString temp_str;
    temp_str = QString::fromStdString(std::to_string(mA_l[0][0]));
    ui->textAlphaCam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_l[0][1]));
    ui->textSkewCam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_l[0][2]));
    ui->textU0Cam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_l[1][1]));
    ui->textBetaCam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_l[1][2]));
    ui->textV0Cam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK1_l));
    ui->textK1Cam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK2_l));
    ui->textK2Cam01->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dError_l));
    ui->textErrorCam01->setText(temp_str);

    KMatrix mA_r(3, 3); // right camera intrinsic matrix
    mA_r[0][0] = vX_r[0];
    mA_r[1][1] = vX_r[1];
    mA_r[0][2] = vX_r[2];
    mA_r[1][2] = vX_r[3];
    mA_r[2][2] = 1.0;
    double dK1_r = vX_r[4];
    double dK2_r = vX_r[5];
    double dError_r = vX_r[vX_r.Size() - 1];  // vX's last element is Err

    KHomogeneous* hRw; // hRw
    hRw = new KHomogeneous[nImg];
    for (int i = 0; i < nImg; i++) {
        KRotation rR;
        rR.FromRodrigues(vX_r[6*(i+1)], vX_r[6*(i+1)+1], vX_r[6*(i+1)+2]);
        KVector   vT(vX_r[6*(i+1)+3], vX_r[6*(i+1)+4], vX_r[6*(i+1)+5]);
        hRw[i] = KHomogeneous(rR, vT);      // 4x4 matrix, W->TR
    }

    // Print right calibration parameters
    temp_str = QString::fromStdString(std::to_string(mA_r[0][0]));
    ui->textAlphaCam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_r[0][1]));
    ui->textSkewCam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_r[0][2]));
    ui->textU0Cam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_r[1][1]));
    ui->textBetaCam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(mA_r[1][2]));
    ui->textV0Cam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK1_r));
    ui->textK1Cam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dK2_r));
    ui->textK2Cam02->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(dError_r));
    ui->textErrorCam02->setText(temp_str);

    // 3. Stereo Calibration
    // vX: 6*1 matrix, {r1, r2, r3, t1, t2, t3}: 호모그래피를 평균 낸 것 사용
    KVector vX = rv0171::StereoCalibration(vX_l, vX_r, left_pointzip, right_pointzip, nImg, nFeature, nItr);    // hLr

    KRotation rR;
    rR.FromRodrigues(vX[0], vX[1], vX[2]);
    KVector vT(vX[3], vX[4], vX[5]);
    KHomogeneous hLr = KHomogeneous(rR, vT);

    // Print right calibration parameters
    temp_str = QString::fromStdString(std::to_string(vX[0]));
    ui->textR1->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(vX[1]));
    ui->textR2->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(vX[2]));
    ui->textR3->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(vX[3]));
    ui->textT1->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(vX[4]));
    ui->textT2->setText(temp_str);
    temp_str = QString::fromStdString(std::to_string(vX[5]));
    ui->textT3->setText(temp_str);

    // 4. Project Right points to Left points (for verification)
    // Model points
    KMatrix mM;             // 3 x mFeature matrix
    for (int i = 0; i < nFeature; i++) {
        mM |= KVector(left_pointzip[nImg][i].first, left_pointzip[nImg][i].second, 1.0);             // Add a column
    }

    // 출력을 위한 ImageForm 생성
    ImageForm* q_pForm[15] = {0,};
    string wName[] = {"left1", "left2", "left3", "left4", "left5", "left6", "left7", "left8", "left9", "left10", "left11", "left12", "left13", "left14", "left15"};
    for (int i = 0; i < nImg; i++) {
        string sImgName = rvdir;
        sImgName += "Stereo_Calibration_data/calib_data/left";
        sImgName += to_string(i + 1);
        sImgName += ".bmp";

        KImageColor icMain(480, 640);
        QImage* _img = new QImage();        //이미지를 로드하기 위한 QImage 선언
        if (_img->load(QString::fromStdString(sImgName))) {
            for (int ii = 0; ii < 480; ii++) {
                for (int jj = 0; jj < 640; jj++) {
                    QColor color = _img->pixelColor(jj,ii);
                    icMain[ii][jj].r = color.red();
                    icMain[ii][jj].g = color.green();
                    icMain[ii][jj].b = color.blue();
                }
            }
        }
        else {
            qDebug() << "Image load Error";
        }

        KHomogeneous hP = hLr * hRw[i];

        // Display window
        if (q_pForm[i]) {
            // 이미지 창이 이미 존재하면 아무것도 하지 않음
        }
        else {
            q_pForm[i] = new ImageForm(icMain, QString::fromStdString(wName[i]), this);

            for(int j = 0; j < nFeature; j++) {
                // Real point
                QPoint point;
                point.setX((int)(left_pointzip[i][j].first));
                point.setY((int)(left_pointzip[i][j].second));
                QColor color;
                color.setRed(255);
                color.setGreen(0);
                color.setBlue(0);
                q_pForm[i]->DrawEllipse(point, 3, 3, color);

                // Projection
                KVector vU = rv0171::PointProjection(mA_l, KVector(dK1_l, dK2_l), hP, mM.Column(j));

                // Reprojected point
                QPoint rePoint;
                rePoint.setX((int)(vU[__X]));
                rePoint.setY((int)(vU[__Y]));
                QColor reColor;
                reColor.setRed(0);
                reColor.setGreen(0);
                reColor.setBlue(255);
                q_pForm[i]->DrawEllipse(rePoint, 3, 3, reColor);
            }
            _lImageForm.push_back(q_pForm[i]);
            q_pForm[i]->show();
        }
    }

    cout << "\nStereo Calibration Process Finished!" << endl;

    // UI 활성화 갱신
    UpdateUI();

    delete[] hRw;
}
