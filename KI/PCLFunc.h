#pragma once
#include "QVTKWidget.h"
#include <boost/thread/thread.hpp>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/common/common_headers.h>
#include <pcl/common/transforms.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>
#include "vtkCylinderSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkLegendScaleActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkAxisActor2D.h"
#include <QGroupBox>
class PCLFunc
{
public:
	PCLFunc();
	~PCLFunc();
	void initialVtkWidget(QGroupBox* dock);
	void TransView(vtkRenderer* render, float xTrans, float yTrans);
	void Chibiao(bool grid_enabled_);
	void SetLMArrayDisplay(float x[12000], float y[12000], int LMID[12000], int LMCount);
	void RedLMDisplay(int ExeType, int LMNO);
	void RedSetLMDisplay(int LMID);
	void SetUpGrid(int grid_size, float grid_step);
	void RotateViewTo(void);
	void setViewTo(int axis, int sign);
	void AddLaserLoPose(float *Pose);
	void AddCloudPointArray(float x[7200], float y[7200], float z[7200], float LandmarkData[121], float LaserPose[4], int ScanPointCount, int LmCount);
	void AddLandMarkArray(float *LMData, int LMCount);
	void DeleteOneSetLMDisplay(int LMID);
	void AddOneSetLMDisplay(int x, int y, int LMID);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
	pcl::visualization::PCLVisualizer::Ptr viewer=NULL;
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>::Ptr cloud_color_handler;
	bool FlagPointDisplay = true;  //true是点  false是线
	void PointOrPolyline(bool f_pointOrLine);
	void DeleteCurrLM();
	QVTKWidget *qvtk = NULL;
	QVTKWidget* spawnSubwidget(QGroupBox *dock, QVTKWidget *qvtk=NULL);
private:
	//点云数据存储
	std::string CloudId;
	int CurrentRedSetLMID = -1;
	int CurrentRedLMNO = -1;
	int EveryLMFlag[40];
};

