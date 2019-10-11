#include "PCLFunc.h"
using namespace std;
double LastLaserPose[4] = { -1, 0, 0, 0 }; //上一次自身位置信息 
boost::mutex m_ViewMutex;
vector<int> SetLandMark_ID;   //记录设置的靶标的ID
pcl::PointCloud<pcl::PointXYZRGB>::Ptr Laser_cloud_ptr(new pcl::PointCloud<pcl::PointXYZRGB>);
pcl::PointCloud<pcl::PointXYZ>::Ptr LandMark_cloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);       //实时获取的靶标、填充显示
pcl::PointCloud<pcl::PointXYZ>::Ptr SetLandMark_cloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);    //设置靶标、镂空显示
vtkSmartPointer<vtkRenderWindow> win;
vtkSmartPointer<vtkRenderWindowInteractor> iren;
vtkSmartPointer<vtkLegendScaleActor> grid_actor_;
int lastLMType = 0;   //靶标显示类型
typedef struct _ChangeShapesInfo
{
	bool ChangeShapesFlg;    //0不需要修改，1需要修改
	unsigned int ChangeSizeType;    //0大尺寸，1中尺寸，2小尺寸
}ChangeShapesInfo;
ChangeShapesInfo g_ChangeShapesInfo;

PCLFunc::PCLFunc()
{

}

#pragma region 鼠标和滚轮事件
void ClickPointCallBack(const pcl::visualization::PointPickingEvent& event, void* viewer_void)
{
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer> *> (viewer_void);
	int index = event.getPointIndex();
	if (index == -1)
		return;
	uint8_t r(255), g(15), b(15);
	pcl::PointXYZRGB current_point;
	event.getPoint(current_point.x, current_point.y, current_point.z);
	int PX = int(current_point.x * 1000);
	int PY = int(current_point.y * 1000);
	int PZ = int(current_point.z * 1000);
	pcl::PointXYZRGB Origin_point;
	Origin_point.x = LastLaserPose[2];
	Origin_point.y = LastLaserPose[3];
	Origin_point.z = current_point.z;
	float s_xcha = pow((current_point.x - Origin_point.x), 2);
	float s_ycha = pow((current_point.y - Origin_point.y), 2);
	float s_zcha = pow((current_point.z - Origin_point.z), 2);
	float sum = s_xcha + s_ycha + s_zcha;
	int PDistance = (int)(sqrt(sum) * 1000);
	std::stringstream ss;
	ss << "  No. : " << (index + 1) << "\nX(mm): " << PX << "\nY(mm): " << PY << "\nZ(mm): " << PZ << "\nD(mm): " << PDistance;
	viewer->removeShape("text", 0);
	viewer->addText(ss.str(), 1, 20, 13, 0, 0, 0, "text", 0);
}
void wheelMouseCallback(const pcl::visualization::MouseEvent& event, void* viewer_void)
{
	g_ChangeShapesInfo.ChangeShapesFlg = true;
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer> *> (viewer_void);
	if (event.getType() == 4 || (event.getType() == 5))    //4是滚轮向后，5是滚轮向前
	{
		double ScaleValue = win->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->GetParallelScale() / 3;
		if (ScaleValue >= 45)
		{
			if (g_ChangeShapesInfo.ChangeSizeType != 0)
			{
				g_ChangeShapesInfo.ChangeSizeType = 0;
				g_ChangeShapesInfo.ChangeShapesFlg = true;
			}
		}
		else if (ScaleValue >= 20)
		{
			if (g_ChangeShapesInfo.ChangeSizeType != 1)
			{
				g_ChangeShapesInfo.ChangeSizeType = 1;
				g_ChangeShapesInfo.ChangeShapesFlg = true;
			}
		}
		else
		{
			if (g_ChangeShapesInfo.ChangeSizeType != 2)
			{
				g_ChangeShapesInfo.ChangeSizeType = 2;
				g_ChangeShapesInfo.ChangeShapesFlg = true;
			}
		}
		if (g_ChangeShapesInfo.ChangeShapesFlg == true)         //图中的xy的缩放对应为  圆柱体：x,z  正方体：x,y  面：x,y
		{
			string shapeName = "";
			if (g_ChangeShapesInfo.ChangeSizeType == 0)  //大尺寸
			{
				for (int i = 0; i < LandMark_cloud_ptr->size(); i++)
				{
					shapeName = "landmark" + to_string((_Longlong)(i));
					if (lastLMType == 0)    //0：圆柱体 
					{
						viewer->updateMyShapesScale(1.8, 1, 1.8, shapeName);
					}
					else if (lastLMType == 1 || (lastLMType == 2))   //1：正方体、面
					{
						viewer->updateMyShapesScale(1.8, 1.8, 1, shapeName);
					}
				}
				for (int i = 0; i < SetLandMark_cloud_ptr->size(); i++)
				{
					shapeName = "Setlandmark" + to_string((_Longlong)(SetLandMark_ID.at(i)));
					if (lastLMType == 0)    //0：圆柱体 
					{
						viewer->updateMyShapesScale(1.8, 1, 1.8, shapeName);
					}
					else if (lastLMType == 1 || (lastLMType == 2))   //1：正方体、面
					{
						viewer->updateMyShapesScale(1.8, 1.8, 1, shapeName);
					}
				}
				shapeName = "LaserBody";
				viewer->updateMyShapesScale(1.8, 1.8, 1.8, shapeName);
				shapeName = "LaserAngle";
				viewer->updateMyLaserAngle(1.8, LastLaserPose[1], LastLaserPose[2], LastLaserPose[3], shapeName);
			}
			else if (g_ChangeShapesInfo.ChangeSizeType == 1)   //中尺寸
			{
				for (int i = 0; i < LandMark_cloud_ptr->size(); i++)
				{
					shapeName = "landmark" + to_string((_Longlong)(i));
					if (lastLMType == 0)    //0：圆柱体 
					{
						viewer->updateMyShapesScale(1, 1, 1, shapeName);
					}
					else if (lastLMType == 1 || (lastLMType == 2))   //1：正方体、面
					{
						viewer->updateMyShapesScale(1, 1, 1, shapeName);
					}
				}
				for (int i = 0; i < SetLandMark_cloud_ptr->size(); i++)
				{
					shapeName = "Setlandmark" + to_string((_Longlong)(SetLandMark_ID.at(i)));
					if (lastLMType == 0)    //0：圆柱体 
					{
						viewer->updateMyShapesScale(1, 1, 1, shapeName);
					}
					else if (lastLMType == 1 || (lastLMType == 2))   //1：正方体、面
					{
						viewer->updateMyShapesScale(1, 1, 1, shapeName);
					}
				}
				shapeName = "LaserBody";
				viewer->updateMyShapesScale(1, 1, 1, shapeName);
				shapeName = "LaserAngle";
				viewer->updateMyLaserAngle(1, LastLaserPose[1], LastLaserPose[2], LastLaserPose[3], shapeName);
			}
			else if (g_ChangeShapesInfo.ChangeSizeType == 2)   //小尺寸
			{
				for (int i = 0; i < LandMark_cloud_ptr->size(); i++)
				{
					shapeName = "landmark" + to_string((_Longlong)(i));
					if (lastLMType == 0)    //0：圆柱体 
					{
						viewer->updateMyShapesScale(0.5, 1, 0.5, shapeName);
					}
					else if (lastLMType == 1 || (lastLMType == 2))   //1：正方体、面
					{
						viewer->updateMyShapesScale(0.5, 0.5, 1, shapeName);
					}
				}
				for (int i = 0; i < SetLandMark_cloud_ptr->size(); i++)
				{
					shapeName = "Setlandmark" + to_string((_Longlong)(SetLandMark_ID.at(i)));
					if (lastLMType == 0)    //0：圆柱体 
					{
						viewer->updateMyShapesScale(0.5, 1, 0.5, shapeName);
					}
					else if (lastLMType == 1 || (lastLMType == 2))   //1：正方体、面
					{
						viewer->updateMyShapesScale(0.5, 0.5, 1, shapeName);
					}
				}
				shapeName = "LaserBody";
				viewer->updateMyShapesScale(0.5, 0.5, 0.5, shapeName);
				shapeName = "LaserAngle";
				viewer->updateMyLaserAngle(0.5, LastLaserPose[1], LastLaserPose[2], LastLaserPose[3], shapeName);
			}
			g_ChangeShapesInfo.ChangeShapesFlg = false;
		}
	}
}
#pragma endregion

#pragma region 初始化Vtk
void PCLFunc::initialVtkWidget(QGroupBox* dock)
{
	cloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));//CREATE_INTERACTOR:TRUE 弹框
	win = viewer->getRenderWindow();
	win->SetBorders(0);//设置边界 0无边界
	win->SetSize(1, 1);
	win->SetPosition(0, 0);
	viewer->createInteractor();//弹出交互界面
	viewer->addPointCloud(cloud, "cloud");
	viewer->setBackgroundColor(0.94118, 0.97255, 1);
	viewer->addCoordinateSystem(1.0);
	memset(&g_ChangeShapesInfo, 0, sizeof(g_ChangeShapesInfo));
	//RenderWindow 渲染窗口
	SetUpGrid(80, 1);//绘制网格
	qvtk = spawnSubwidget(dock);
	qvtk->SetRenderWindow(win);
	setViewTo(2, -1);//设置默认视角为Z-
	viewer->registerPointPickingCallback(ClickPointCallBack, (void*)&viewer);
	//viewer->registerMouseCallback(wheelMouseCallback, (void*)&viewer);
	g_ChangeShapesInfo.ChangeSizeType = 2;//默认小尺寸
}
#pragma endregion

QVTKWidget* PCLFunc::spawnSubwidget(QGroupBox *dock, QVTKWidget *qvtk)
{
	if (!qvtk)
		qvtk = new QVTKWidget();
	qvtk->setParent(dock);
	return qvtk;
}

int countview = 0;
//视图整体平移
void PCLFunc::TransView(vtkRenderer* render, float xTrans, float yTrans)
{
	countview++;
	if (countview == 10)
	{
		countview = 0;
	if (render == NULL)
	{
		return;
	}
	double viewFocus[4], focalDepth, viewPoint[3];
	double newPickPoint[4], oldPickPoint[4], motionVector[3];

	// Calculate the focal depth since we'll be using it a lot

	vtkCamera *camera = render->GetActiveCamera();
	camera->GetFocalPoint(viewFocus);
	render->SetWorldPoint(viewFocus[0], viewFocus[1], viewFocus[2], viewFocus[3]);
	render->WorldToDisplay();
	viewFocus[0] = render->GetDisplayPoint()[0];
	viewFocus[1] = render->GetDisplayPoint()[1];
	viewFocus[2] = render->GetDisplayPoint()[2];
	viewFocus[3] = render->GetDisplayPoint()[3];
	focalDepth = viewFocus[2];

	double x[2];
	x[0] = win->GetSize()[0];
	x[1] = win->GetSize()[1];
	double x1 = x[0] / 2, y1 = x[1] / 2;
	// Has to recalc old mouse point since the viewport has moved,
	// so can't move it outside the loop
	render->SetDisplayPoint(x1, y1, focalDepth);
	render->DisplayToWorld();
	oldPickPoint[0] = render->GetWorldPoint()[0];
	oldPickPoint[1] = render->GetWorldPoint()[1];
	oldPickPoint[2] = render->GetWorldPoint()[2];
	oldPickPoint[3] = render->GetWorldPoint()[3];

	// Camera motion is reversed
	motionVector[0] = xTrans;
	motionVector[1] = yTrans;
	motionVector[2] = 0;

	camera->GetFocalPoint(viewFocus);
	camera->GetPosition(viewPoint);
	camera->SetFocalPoint(motionVector[0] + viewFocus[0],
		motionVector[1] + viewFocus[1],
		motionVector[2] + viewFocus[2]);

	camera->SetPosition(motionVector[0] + viewPoint[0],
		motionVector[1] + viewPoint[1],
		motionVector[2] + viewPoint[2]);

	if (render->GetLightFollowCamera())
	{
		render->UpdateLightsGeometryToFollowCamera();
	}
	win->Render();
	}
}

#pragma region 尺标
void PCLFunc::Chibiao(bool grid_enabled_)//尺标
{
	vtkRenderer *CurrentRenderer = vtkRenderer::New();
	vtkAxisActor2D *m_AxisActor;
	CurrentRenderer = iren->FindPokedRenderer(NULL, NULL);
	if (!grid_enabled_)
	{
		grid_actor_->GetLegendLabelProperty()->SetColor(1, 0, 0);
		grid_actor_->GetLegendTitleProperty()->SetColor(1, 0, 0);

		//设置底部轴线
		m_AxisActor = grid_actor_->GetBottomAxis();
		m_AxisActor->GetProperty()->SetColor(1, 0, 0);
		m_AxisActor->GetLabelTextProperty()->SetColor(1, 0, 0);

		//设置顶部轴线
		m_AxisActor = grid_actor_->GetTopAxis();
		m_AxisActor->GetProperty()->SetColor(1, 0, 0);
		m_AxisActor->GetLabelTextProperty()->SetColor(1, 0, 0);

		//设置左部轴线
		m_AxisActor = grid_actor_->GetLeftAxis();
		m_AxisActor->GetProperty()->SetColor(1, 0, 0);
		m_AxisActor->GetLabelTextProperty()->SetColor(1, 0, 0);

		//设置右部轴线
		m_AxisActor = grid_actor_->GetRightAxis();
		m_AxisActor->GetProperty()->SetColor(1, 0, 0);
		m_AxisActor->GetLabelTextProperty()->SetColor(1, 0, 0);

		CurrentRenderer->AddViewProp(grid_actor_);
		grid_enabled_ = true;
	}
	else
	{
		CurrentRenderer->RemoveViewProp(grid_actor_);
		grid_enabled_ = false;
	}
	win->Render();
}
#pragma endregion

#pragma region SetLMArrayDisplay
void PCLFunc::SetLMArrayDisplay(float x[12000], float y[12000], int LMID[12000], int LMCount)
{
	int LastSetLandmarkCount = SetLandMark_cloud_ptr->size();
	string shapeName = "";
	int CurrentLMFlag = 0;
	double XTranslation = 0;
	double YTranslation = 0;
	pcl::PointXYZ PCylinderDowm, PCylinderUp;
	//清除点云
	Laser_cloud_ptr->clear();
	viewer->updatePointCloud(Laser_cloud_ptr, "cloud");
	//刷新当前指定的 id 对应点云的数据以及显示 。
	float m_LaserPose[4];
	m_LaserPose[0] = 0;
	m_LaserPose[1] = 0;
	m_LaserPose[2] = 0;
	m_LaserPose[3] = 0;
	//boost::lock_guard<boost::mutex> lock(m_ViewMutex);
	AddLaserLoPose(m_LaserPose);
	//删除当前实时获取的靶标
	for (int num = 0; num < LandMark_cloud_ptr->size(); num++)
	{
		shapeName = "landmark" + to_string((_Longlong)(num));
		viewer->removeShape(shapeName);
	}
	LandMark_cloud_ptr->clear();
	//显示靶标
	int i = 0;
	int j = 0;
	for (; i < LMCount; i++)
	{
		int a = LMID[i];
		shapeName = "Setlandmark" + to_string((_Longlong)(LMID[i]));
		PCylinderDowm.x = x[i] / 1000;
		PCylinderDowm.y = y[i] / 1000;
		PCylinderDowm.z = 0;
		if (j >= LastSetLandmarkCount)
		{
			PCylinderUp.x = PCylinderDowm.x;
			PCylinderUp.y = PCylinderDowm.y;
			PCylinderUp.z = 5;
			viewer->addLMCylinder(PCylinderDowm, 0.6, shapeName, 0);
			if (g_ChangeShapesInfo.ChangeSizeType == 0)   //大尺寸
			{
				viewer->updateMyShapesScale(1.8, 1, 1.8, shapeName);
			}
			else if (g_ChangeShapesInfo.ChangeSizeType == 2)   //小尺寸
			{
				viewer->updateMyShapesScale(0.5, 1, 0.5, shapeName);
			}
			SetLandMark_cloud_ptr->push_back(PCylinderDowm);
			SetLandMark_ID.push_back(LMID[i]);
		}
		else
		{
			for (; j < LastSetLandmarkCount;)
			{
				if (SetLandMark_ID.at(j) == LMID[i])
				{
					XTranslation = PCylinderDowm.x - SetLandMark_cloud_ptr->at(j).x;
					YTranslation = PCylinderDowm.y - SetLandMark_cloud_ptr->at(j).y;
					if (XTranslation != 0 || (YTranslation != 0))
					{
						viewer->updateMyShapes(XTranslation*50, YTranslation*50, shapeName);
						SetLandMark_cloud_ptr->at(j).x = PCylinderDowm.x;
						SetLandMark_cloud_ptr->at(j).y = PCylinderDowm.y;
						SetLandMark_cloud_ptr->at(j).z = 0;
					}
					j++;
					break;
				}
				else if (SetLandMark_ID.at(j) > LMID[i])
				{
					PCylinderUp.x = PCylinderDowm.x;
					PCylinderUp.y = PCylinderDowm.y;
					PCylinderUp.z = 5;
					viewer->addLMCylinder(PCylinderDowm, 0.6, shapeName, 0);
					if (g_ChangeShapesInfo.ChangeSizeType == 0)   //大尺寸
					{
						viewer->updateMyShapesScale(1.8, 1, 1.8, shapeName);
					}
					else if (g_ChangeShapesInfo.ChangeSizeType == 2)   //小尺寸
					{
						viewer->updateMyShapesScale(0.5, 1, 0.5, shapeName);
					}
					SetLandMark_cloud_ptr->insert(SetLandMark_cloud_ptr->begin() + j, PCylinderDowm);
					SetLandMark_ID.insert(SetLandMark_ID.begin() + j, LMID[i]);
					j++;
					LastSetLandmarkCount++;
					break;
				}
				else
				{
					//删除
					string deleteshapeName = "Setlandmark" + to_string((_Longlong)(SetLandMark_ID[j]));
					viewer->removeShape(deleteshapeName);
					SetLandMark_cloud_ptr->erase(SetLandMark_cloud_ptr->begin() + j);
					SetLandMark_ID.erase(SetLandMark_ID.begin() + j);
					LastSetLandmarkCount--;
					if (j == LastSetLandmarkCount)
					{
						PCylinderUp.x = PCylinderDowm.x;
						PCylinderUp.y = PCylinderDowm.y;
						PCylinderUp.z = 5;
						viewer->addLMCylinder(PCylinderDowm, 0.6, shapeName, 0);
						if (g_ChangeShapesInfo.ChangeSizeType == 0)   //大尺寸
						{
							viewer->updateMyShapesScale(1.8, 1, 1.8, shapeName);
						}
						else if (g_ChangeShapesInfo.ChangeSizeType == 2)   //小尺寸
						{
							viewer->updateMyShapesScale(0.5, 1, 0.5, shapeName);
						}
						SetLandMark_cloud_ptr->push_back(PCylinderDowm);
						SetLandMark_ID.push_back(LMID[i]);
					}
				}
			}
		}
	}
	//删除多余的靶标
	if (j < LastSetLandmarkCount)
	{
		for (; j < LastSetLandmarkCount; j++)
		{
			shapeName = "Setlandmark" + to_string((_Longlong)(SetLandMark_ID.at(SetLandMark_ID.size() - 1)));
			viewer->removeShape(shapeName);
			SetLandMark_cloud_ptr->erase(SetLandMark_cloud_ptr->end() - 1);
			SetLandMark_ID.pop_back();
		}
	}
	qvtk->update();//不能用spinonce，用了就会卡死
}
#pragma endregion

#pragma region 将(靶标探测)选中靶标改为红色
void PCLFunc::RedLMDisplay(int ExeType, int LMNO)   //ExeType使用类型，0为下次显示更新，1为立即更新
{
	string shapeName = "";
	if (ExeType == 0)
	{
		CurrentRedLMNO = LMNO;
	}
	else
	{
		if (LMNO != -1)
		{
			if (CurrentRedLMNO != LMNO)
			{
				if (CurrentRedLMNO != -1)
				{
					shapeName = "landmark" + to_string((_Longlong)(CurrentRedLMNO));
					if (EveryLMFlag[CurrentRedLMNO] - 10 == 0)
					{
						viewer->updateMyShapeColor(0.176, 0.867, 0.867, shapeName);//浅蓝
					}
					else
					{
						viewer->updateMyShapeColor(0.176, 0.867, 0.176, shapeName);//绿
					}
				}
				shapeName = "landmark" + to_string((_Longlong)(LMNO));
				viewer->updateMyShapeColor(0.867, 0.176, 0.176, shapeName);//红
				CurrentRedLMNO = LMNO;
			}
		}
		else
		{
			if (CurrentRedLMNO != -1)
			{
				shapeName = "landmark" + to_string((_Longlong)(CurrentRedLMNO));
				if (EveryLMFlag[CurrentRedLMNO] - 10 == 0)
				{
					viewer->updateMyShapeColor(0.176, 0.867, 0.867, shapeName);
				}
				else
				{
					viewer->updateMyShapeColor(0.176, 0.867, 0.176, shapeName);
				}
			}
			CurrentRedLMNO = -1;
		}
		qvtk->update();
	}
}
#pragma endregion

#pragma region 将(靶标获取或文件导入)的选中靶标改为红色(静态)
void PCLFunc::RedSetLMDisplay(int LMID)
{
	string shapeName = "";
	if (LMID != -1)
	{
		if (CurrentRedSetLMID != LMID)
		{
			if (CurrentRedSetLMID != -1)
			{
				shapeName = "Setlandmark" + to_string((_Longlong)(CurrentRedSetLMID));
				viewer->updateMyShapeColor(0, 0, 0, shapeName);
			}

			shapeName = "Setlandmark" + to_string((_Longlong)(LMID));
			viewer->updateMyShapeColor(0.867, 0.176, 0.176, shapeName);
			CurrentRedSetLMID = LMID;
		}
	}
	else
	{
		if (CurrentRedSetLMID != -1)
		{
			shapeName = "Setlandmark" + to_string((_Longlong)(CurrentRedSetLMID));
			viewer->updateMyShapeColor(0, 0, 0, shapeName);
		}
		CurrentRedSetLMID = -1;
	}
	qvtk->update();
}
#pragma endregion

#pragma region 网格
void PCLFunc::SetUpGrid(int grid_size, float grid_step)
{
	string name_x = string("line_x_");
	string name_y = string("line_y_");
	string name_line_x = string("");
	string name_line_y = string("");
	string name_xScale = string("Scale_x_");
	string name_Scale_x = string("");
	string name_yScale = string("Scale_y_");
	string name_Scale_y = string("");
	pcl::PointXYZ p1(0, 0, 0), p2(0, 0, 0);
	pcl::PointXYZ p3(p1), p4(p2);
	pcl::PointXYZ px(p1), py(p2);
	int startpos = grid_step*grid_size / 2;
	for (int j = 0; j <= grid_size; j++)
	{
		p1.x = -startpos + j*grid_step; p1.y = -startpos;
		p2.x = -startpos + j*grid_step; p2.y = startpos;
		name_line_x = name_x + to_string((_Longlong)j);
		name_Scale_x = name_xScale + to_string((_Longlong)j);
		if (j == grid_size / 2)
		{
			viewer->addLine(p1, p2, 0.25, 0.25, 0.25, name_line_x);//在指定视口 viewport 中添加一条线段,ptl 、 pt2 为线段起始和终点,
			//rgb 为颜色,id 为对象 ID 。
		}
		else
		{
			viewer->addLine(p1, p2, 0.827451, 0.827451, 0.827451, name_line_x);
			px.x = p1.x;
			viewer->addText3D(to_string((_Longlong)p1.x), px, (grid_step / 5.0), 0, 0, 0, name_Scale_x);
			if (p1.x == startpos)
			{
				px.x += 1;
				viewer->addText3D("X(m)", px, (grid_step / 5.0 + 1), 0, 0, 0, "Axis_x");
				//在 3D 空间添加 3D字符串， text 为字符串内容， position 为添加字符串位置，
				//textScale 为字符缩放大小， r 、 g, b 指定颜色， id 指定字符串的 ID 标志。
			}
		}
		p3.x = -startpos; p3.y = -startpos + j*grid_step;
		p4.x = startpos;  p4.y = -startpos + j*grid_step;
		name_line_y = name_y + to_string((_Longlong)j);
		name_Scale_y = name_yScale + to_string((_Longlong)j);
		if (j == grid_size / 2)
		{
			viewer->addLine(p3, p4, 0.25, 0.25, 0.25, name_line_y);
		}
		else
		{
			viewer->addLine(p3, p4, 0.827451, 0.827451, 0.827451, name_line_y);
			py.y = p3.y;
			viewer->addText3D(to_string((_Longlong)p3.y), py, (grid_step / 5.0), 0, 0, 0, name_Scale_y);
			if (p3.y == startpos)
			{
				py.y += 1;
				viewer->addText3D("Y(m)", py, (grid_step / 5.0 + 1), 0, 0, 0, "Axis_y");
			}
		}
	}
}
#pragma endregion

#pragma region 变换PCLvisualizer 视角，实质是变换VTK 相机参数
void PCLFunc::RotateViewTo(void)
{
	std::vector<pcl::visualization::Camera> camera;
	viewer->getCameras(camera);
	Eigen::RowVector3d OldUpVector(camera[0].view);
	Eigen::RowVector3d OldPosition(camera[0].pos);
	Eigen::Matrix3d RotationM;
	RotationM << 0, 1, 0,
		-1, 0, 0,
		0, 0, 1;
	Eigen::RowVector3d NewUpVector = OldUpVector*RotationM;
	Eigen::RowVector3d NewPosition = OldPosition*RotationM;
	camera[0].view[0] = NewUpVector(0);
	camera[0].view[1] = NewUpVector(1);
	camera[0].view[2] = NewUpVector(2);
	camera[0].pos[0] = NewPosition(0);
	camera[0].pos[1] = NewPosition(1);
	camera[0].pos[2] = NewPosition(2);
	viewer->setCameraPosition(camera[0].pos[0], camera[0].pos[1], camera[0].pos[2], camera[0].view[0]
		, camera[0].view[1], camera[0].view[2]);
	viewer->resetCamera();
	viewer->updateCamera();
}

void PCLFunc::setViewTo(int axis, int sign)
{
	std::vector<pcl::visualization::Camera> camera;
	viewer->getCameras(camera);
	Eigen::Vector3d position(camera[0].pos);
	double norm = position.norm();
	if (axis == 0)  //X
	{
		camera[0].view[0] = 0;
		camera[0].view[1] = 0;
		camera[0].view[2] = 1;
		camera[0].pos[0] = norm*sign*(-1);
		camera[0].pos[1] = norm*sign*(-0);
		camera[0].pos[2] = norm*sign*(-0);
	}
	else if (axis == 1)  //Y
	{
		camera[0].view[0] = 0;
		camera[0].view[1] = 0;
		camera[0].view[2] = 1;

		camera[0].pos[0] = norm*sign*(-0);
		camera[0].pos[1] = norm*sign*(-1);
		camera[0].pos[2] = norm*sign*(-0);
	}
	else if (axis == 2)  //Z
	{
		camera[0].view[0] = 0;
		camera[0].view[1] = 1;
		camera[0].view[2] = 0;
		camera[0].pos[0] = norm*sign*(-0);
		camera[0].pos[1] = norm*sign*(-0);
		camera[0].pos[2] = norm*sign*(-1);
	}
	viewer->setCameraPosition(camera[0].pos[0], camera[0].pos[1], camera[0].pos[2], camera[0].view[0]
		, camera[0].view[1], camera[0].view[2]);
	//viewer->camera_ = camera[0];
	viewer->resetCamera();
	viewer->updateCamera();
}
#pragma endregion

#pragma region 增加激光器位置
void PCLFunc::AddLaserLoPose(float *Pose)
{
	double XTranslation = 0;
	double YTranslation = 0;
	double Angle = 0;
	string shapeName = "";
	pcl::PointXYZ PCylinderDowm, PCylinderUp;

	if (int(*Pose) == 0)
	{
		Angle = 0;
		PCylinderDowm.x = 0;
		PCylinderDowm.y = 0;
	}
	else
	{
		Angle = *(Pose + 1);
		PCylinderDowm.x = *(Pose + 2) / 1000;
		PCylinderDowm.y = *(Pose + 3) / 1000;
	}
	PCylinderDowm.z = 0;

	if ((int)LastLaserPose[0] == -1)
	{
		shapeName = "LaserBody";
		viewer->addMyLaserBody(PCylinderDowm, shapeName, 0);
		shapeName = "LaserAngle";
		viewer->addMyLaserAngle(PCylinderDowm, Angle, shapeName, 0);
		LastLaserPose[0] = 0;
	}
	else
	{
		shapeName = "LaserBody";
		XTranslation = PCylinderDowm.x - LastLaserPose[2];
		YTranslation = PCylinderDowm.y - LastLaserPose[3];

		viewer->updateMyShapes(XTranslation, YTranslation, shapeName);
	}
	if (g_ChangeShapesInfo.ChangeSizeType == 0)  //大尺寸
	{
		viewer->updateMyShapesScale(1.8, 1.8, 1.8, shapeName);
	}
	else if (g_ChangeShapesInfo.ChangeSizeType == 1)   //中尺寸
	{
		viewer->updateMyShapesScale(1, 1, 1, shapeName);
	}
	else if (g_ChangeShapesInfo.ChangeSizeType == 2)   //小尺寸
	{
		viewer->updateMyShapesScale(0.5, 0.5, 0.5, shapeName);
	}
	shapeName = "LaserAngle";
	if (Angle != LastLaserPose[1])
	{
		//删除
		viewer->removeShape(shapeName);
		if (abs(XTranslation) > 1 || abs(YTranslation) > 1)//大于1m平移
		{
			vtkRenderer *ren = FindPokedRenderer((int)(*(Pose + 2) / 1000), (int)(*(Pose + 3) / 1000));
			TransView(ren, XTranslation, YTranslation);
		}
		//添加
		viewer->addMyLaserAngle(PCylinderDowm, Angle, shapeName, 0);

		if (g_ChangeShapesInfo.ChangeSizeType == 0)  //大尺寸
		{
			viewer->updateMyLaserAngle(1.8, Angle, PCylinderDowm.x, PCylinderDowm.y, shapeName);
		}
		if (g_ChangeShapesInfo.ChangeSizeType == 1)   //中尺寸
		{
			viewer->updateMyLaserAngle(1, Angle, PCylinderDowm.x, PCylinderDowm.y, shapeName);
		}
		else if (g_ChangeShapesInfo.ChangeSizeType == 2)   //小尺寸
		{
			viewer->updateMyLaserAngle(0.5, Angle, PCylinderDowm.x, PCylinderDowm.y, shapeName);
		}
	}
	LastLaserPose[1] = Angle;
	LastLaserPose[2] = PCylinderDowm.x;
	LastLaserPose[3] = PCylinderDowm.y;
}
#pragma endregion

#pragma region 实时点云及靶标显示
void PCLFunc::AddCloudPointArray(float x[7200], float y[7200], float z[7200], float LandmarkData[121], float LaserPose[4], int ScanPointCount, int LmCount)
{
	uint8_t r(15), g(15), b(255);
	pcl::PointXYZRGB temp_point;
	uint32_t rgb = (static_cast<uint32_t>(r) << 16 |
		static_cast<uint32_t>(g) << 8 | static_cast<uint32_t>(b));
	float a = rgb;
	float *bb = &a;
	*bb = *reinterpret_cast<float*>(&rgb);//此处不能为bb =reinterpret_cast<float*>(&rgb)
	temp_point.rgb = a;
	boost::unique_lock<boost::mutex> lock(m_ViewMutex);
	Laser_cloud_ptr->clear();
	for (int i = 0; i < ScanPointCount; i++)
	{
		temp_point.x = x[i];
		temp_point.y = y[i];
		temp_point.z = z[i];
		Laser_cloud_ptr->points.push_back(temp_point);
	}
	Laser_cloud_ptr->width = (int)Laser_cloud_ptr->points.size();
	Laser_cloud_ptr->height = 1;
	//显示扫描数据
	if (FlagPointDisplay == true)
	{
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "cloud");
		viewer->updatePointCloud(Laser_cloud_ptr, "cloud");
	}
	else
	{
		viewer->removeShape("PolyLine");
		viewer->addPolyLine<pcl::PointXYZRGB>(Laser_cloud_ptr, 0.06, 0.06, 1, "PolyLine", 0);
	}
	//显示自身位置
	if (LaserPose != NULL)
	{
		AddLaserLoPose(LaserPose);
	}
	//显示靶标
	if (LandmarkData != NULL)
	{
		if (LandmarkData[0] == 0 || LandmarkData[0] == 1)
		{
			AddLandMarkArray(LandmarkData, LmCount);
		}
	}
	else
	{
		if (LandMark_cloud_ptr->size() > 0)
		{
			string shapeName = "";
			for (int num = 0; num < LandMark_cloud_ptr->size(); num++)
			{
				shapeName = "landmark" + to_string((_Longlong)(num));
				viewer->removeShape(shapeName);
			}
			LandMark_cloud_ptr->clear();
		}
	}
	qvtk->update();
}
void PCLFunc::AddLandMarkArray(float *LMData, int LMCount)
{
	int LastLandMarkCount = LandMark_cloud_ptr->size();
	int TempLMCount = max(LMCount, LastLandMarkCount);
	double XTranslation = 0;
	double YTranslation = 0;
	int CurrentLMFlag = 0;
	double Angle = 0;
	string shapeName = "";
	pcl::PointXYZ PCylinderDowm, PCylinderUp;
	for (int num = 0; num < TempLMCount; num++)
	{
		shapeName = "landmark" + to_string((_Longlong)(num));
		CurrentLMFlag = *(LMData + num * 3);	//当前靶标类型，0：未设置；1：已设置
		PCylinderDowm.x = *(LMData + 1 + num * 3);
		PCylinderDowm.y = *(LMData + 2 + num * 3);
		PCylinderDowm.z = 0;
		if (num <= (LastLandMarkCount - 1) && num <= (LMCount - 1))
		{
			XTranslation = PCylinderDowm.x - LandMark_cloud_ptr->at(num).x;
			YTranslation = PCylinderDowm.y - LandMark_cloud_ptr->at(num).y;
			if (XTranslation >= 0.05 || XTranslation <= -0.05 || YTranslation >= 0.05 || YTranslation <= -0.05)
			{
				viewer->updateMyShapes(XTranslation, YTranslation, shapeName);
				LandMark_cloud_ptr->at(num).x = PCylinderDowm.x;
				LandMark_cloud_ptr->at(num).y = PCylinderDowm.y;
				LandMark_cloud_ptr->at(num).z = 0;
			}
			//颜色改变
			if (CurrentRedLMNO == num)
			{
				viewer->updateMyShapeColor(0.867, 0.176, 0.176, shapeName);//红色
				EveryLMFlag[num] = CurrentLMFlag + 10;   //记录标红的靶标类型，为原类型+10
			}
			else
			{
				if (CurrentLMFlag != EveryLMFlag[num])
				{
					//改变颜色
					EveryLMFlag[num] = CurrentLMFlag;
					if (CurrentLMFlag == 0)   //0设置的靶标；1匹配上的靶标；2新添加的靶标
					{
						viewer->updateMyShapeColor(0.176, 0.867, 0.867, shapeName);//淡蓝
					}
					else if (CurrentLMFlag == 1)
					{
						viewer->updateMyShapeColor(0.176, 0.867, 0.176, shapeName);//绿色
					}
				}
			}
		}
		else
		{
			if (num >= LastLandMarkCount)
			{
				PCylinderUp.x = PCylinderDowm.x;
				PCylinderUp.y = PCylinderDowm.y;
				PCylinderUp.z = 5;
				//创建靶标
				if (g_ChangeShapesInfo.ChangeSizeType == 0)   //大尺寸
				{
					viewer->addMyCylinder(PCylinderDowm, 0.6, 0.176, 0.867, 0.867, shapeName, 0);
					viewer->updateMyShapesScale(1.8, 1, 1.8, shapeName);
				}
				else if (g_ChangeShapesInfo.ChangeSizeType == 1)                //中尺寸
				{
					viewer->addMyCylinder(PCylinderDowm, 0.6, 0.176, 0.867, 0.867, shapeName, 0);
				}
				else if (g_ChangeShapesInfo.ChangeSizeType == 2)               //小尺寸
				{
					viewer->addMyCylinder(PCylinderDowm, 0.6, 0.176, 0.867, 0.867, shapeName, 0);
					viewer->updateMyShapesScale(0.5, 1, 0.5, shapeName);
				}
				//改变颜色
				EveryLMFlag[num] = CurrentLMFlag;
				if (CurrentRedLMNO == num)
				{
					viewer->updateMyShapeColor(0.867, 0.176, 0.176, shapeName);
					EveryLMFlag[num] = CurrentLMFlag + 10;   //记录标红的靶标类型，为原类型+10
				}
				else
				{
					if (CurrentLMFlag == 1)   //改颜色
					{
						viewer->updateMyShapeColor(0.176, 0.867, 0.176, shapeName);
					}
				}
				LandMark_cloud_ptr->push_back(PCylinderDowm);
			}
			else
			{
				//删除
				viewer->removeShape(shapeName);
				LandMark_cloud_ptr->erase(LandMark_cloud_ptr->end() - 1);
			}
		}
	}
}
#pragma endregion

#pragma region 删除一个显示的靶标(靶标获取)
void PCLFunc::DeleteOneSetLMDisplay(int LMID)
{
	int LastSetLandmarkCount = SetLandMark_cloud_ptr->size();
	string shapeName = "Setlandmark" + to_string((_Longlong)(LMID));
	viewer->removeShape(shapeName);
	for (int i = 0; i < LastSetLandmarkCount; i++)
	{
		if (SetLandMark_ID.at(i) == LMID)
		{
			SetLandMark_cloud_ptr->erase(SetLandMark_cloud_ptr->begin() + i);
			SetLandMark_ID.erase(SetLandMark_ID.begin() + i);
			break;
		}
	}
	qvtk->update();
}
#pragma endregion 

#pragma region 增加一个靶标显示
void PCLFunc::AddOneSetLMDisplay(int x, int y, int LMID)
{
	int LastSetLandmarkCount = SetLandMark_cloud_ptr->size();
	string shapeName = "";
	double XTranslation = 0;
	double YTranslation = 0;
	pcl::PointXYZ PCylinderDowm, PCylinderUp;

	shapeName = "Setlandmark" + to_string((_Longlong)(LMID));
	PCylinderDowm.x = ((float)x) / 1000;
	PCylinderDowm.y = ((float)y) / 1000;
	PCylinderDowm.z = 0;
	int endItemID = 0;
	if (SetLandMark_ID.empty())
	{
		endItemID = -1;
	}
	else
	{
		endItemID = SetLandMark_ID.back();
	}

	if (endItemID < LMID)
	{
		PCylinderUp.x = PCylinderDowm.x;
		PCylinderUp.y = PCylinderDowm.y;
		PCylinderUp.z = 5;
		viewer->addLMCylinder(PCylinderDowm, 0.6, shapeName, 0);
		if (g_ChangeShapesInfo.ChangeSizeType == 0)   //大尺寸
		{
			viewer->updateMyShapesScale(1.8, 1, 1.8, shapeName);
		}
		else if (g_ChangeShapesInfo.ChangeSizeType == 2)   //小尺寸
		{
			viewer->updateMyShapesScale(0.5, 1, 0.5, shapeName);
		}
		SetLandMark_cloud_ptr->push_back(PCylinderDowm);
		SetLandMark_ID.push_back(LMID);
	}
	else
	{
		for (int i = 0; i<LastSetLandmarkCount; i++)
		{
			if (SetLandMark_ID.at(i)>LMID)
			{
				PCylinderUp.x = PCylinderDowm.x;
				PCylinderUp.y = PCylinderDowm.y;
				PCylinderUp.z = 5;
				viewer->addLMCylinder(PCylinderDowm, 0.6, shapeName, 0);
				if (g_ChangeShapesInfo.ChangeSizeType == 0)   //大尺寸
				{
					viewer->updateMyShapesScale(1.8, 1, 1.8, shapeName);
				}
				else if (g_ChangeShapesInfo.ChangeSizeType == 2)   //小尺寸
				{
					viewer->updateMyShapesScale(0.5, 1, 0.5, shapeName);
				}
				SetLandMark_cloud_ptr->insert(SetLandMark_cloud_ptr->begin() + i, PCylinderDowm);
				SetLandMark_ID.insert(SetLandMark_ID.begin() + i, LMID);
				break;
			}
			else if (SetLandMark_ID.at(i) == LMID)
			{
				XTranslation = PCylinderDowm.x - SetLandMark_cloud_ptr->at(i).x;
				YTranslation = PCylinderDowm.y - SetLandMark_cloud_ptr->at(i).y;
				if (XTranslation != 0 || (YTranslation != 0))
				{
					viewer->updateMyShapes(XTranslation, YTranslation, shapeName);
					SetLandMark_cloud_ptr->at(i).x = PCylinderDowm.x;
					SetLandMark_cloud_ptr->at(i).y = PCylinderDowm.y;
					SetLandMark_cloud_ptr->at(i).z = 0;
				}
				break;
			}
			else
			{
				continue;
			}
		}
	}
	qvtk->update();
}
#pragma endregion

#pragma region 点线切换
void PCLFunc::PointOrPolyline(bool f_pointOrLine)
{
	if (f_pointOrLine)
	{
		viewer->addPolyLine<pcl::PointXYZRGB>(Laser_cloud_ptr, 0.06, 0.06, 1, "PolyLine");
		viewer->removePointCloud("cloud");
		FlagPointDisplay = false;
	}
	else
	{
		pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(Laser_cloud_ptr);
		viewer->addPointCloud<pcl::PointXYZRGB>(Laser_cloud_ptr, rgb, "cloud");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "cloud");
		viewer->removeShape("PolyLine");
		FlagPointDisplay = true;
	}
}
#pragma endregion

#pragma region 	删除当前实时获取的靶标
void PCLFunc::DeleteCurrLM()
{
	for (int i = 0; i < SetLandMark_cloud_ptr->size(); i++)
	{
		viewer->removeShape("Setlandmark" + to_string((_Longlong)(SetLandMark_ID.at(i))));
	}
	SetLandMark_cloud_ptr->clear();
}
#pragma endregion

PCLFunc::~PCLFunc()
{
}
