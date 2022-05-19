/*
 ******************************************************************************
 Project:      SMART CITY
 Version:      1.0
 Module:       classes.h
 Description:  symbolic constants and data types used throughout SMART CITY
 Authors:      Jason
 Last Updated: 04/28/2022
 ******************************************************************************
*/
#pragma once
#include<iostream>
#include<stdio.h>
#include<vector>
#define G	9.8
#define PI	3.14159
#define MAX	10000			//组件最大数（如管道、连接件等）
#define DEN 1000			//水的密度
using namespace std;

typedef enum
{
	JUNCTION,
	RESERVOIR,
	TANK,
	VALVE
} NodeType;

class Cnode
{
public:
	int ID;					//节点索引
	int m_Ntype;			//节点类型
	double X;				//节点坐标
	double Y;
	double Z;

};
class Ctank
{
public:
	int     ID;				 // 节点索引
	double  A;               // 水箱面积
	double  Hmin;            // 最小高度
	double  Hmax;            // 最大高度
	double  H0;              // 初始高度
	double  Vmin;            // 最小体积
	double  Vmax;            // 最大体积
	double  V0;              // 初始化体积
	double  V;               // 水箱体积
};


class Cpipe
{
public:
	int      ID;				 //管道编号
	int		 seg_num;			 //管道分段数目
	double   D;					 //直径
	double   Len;				 //长度
	double   a;					 //波速
	double   Kc;				 //粗糙度
	double   f;					 //阻力系数

	Cnode			N1;			//开始节点
	Cnode			N2;
	vector<double>	H;			//每根管道所对应的节点的水头/流量值
	vector<double>	Q;
	vector<double>	CP;			//每根管道所对应的节点的CP/CM值，定义：管道内每个节点都对应一个CP/CM值，首端只有CM，末端只有CP值；
	vector<double>	CM;

	Cpipe();
	~Cpipe();
	virtual double B()			//设置虚函数纯粹是为了后续的拓展，目前没有什么应用
	{
		return a / (9.8 * 0.25 * PI * D * D);
	}
	virtual double R()
	{
		return f * (Len / seg_num) / (2 * 9.8 * D * (0.25 * PI * D * D) * (0.25 * PI * D * D));
	}

};
class Creservior
{
public:
	int		 ID;				//水库索引
	double   H;					//水头值
	double	 Q;					//流量值
	vector<Cpipe> pipe;			//水库连接管道
};
class Cpump
{
	int     ID;					 // 水泵索引
	int     Ptype;				 // 水泵曲线类型
	double  Q0;					 // 初始流量
	double  Qmax;				 // 最大流量
	double  Hmax;				 // 最大水头
	double  H0;					 // 关阀水头
	double  R;					 // 流量系数
	int     Hcurve;				 // 水力曲线索引
	int     Epat;				 // 能量损耗索引
	double  Ecost;				 // 单位能量损耗
};
class Cvalve
{
public:
	int		ID;					//阀门索引
	
	Cpipe	uppipe;				//上游管道
	double  m_opening;		//计算阀门的开发度
	double	CV;				//计算阀门的CV值
};
class Cjunction
{
public:
	int		 ID;					//节点ID
	double	 Q;						//节点流量
	double	 H;						//节点水头
	vector<Cpipe> uppipe;	        //上游管道容器
	vector<Cpipe> downpipe;		    //下游管道容器
};
class Ctime							//时间控制器
{
public:
	int		n;						//迭代次数
	double	dt;						//时间步长
	double	Tmax;					//最大时长
	friend	Cvalve;
};
class Cnetwork
{
	public:
		int
			Nnodes,					 // 节点数目
			Ntanks,					 // 水箱数目
			Njuncs,					 // 连接件数目
			Npipes,					 // 管道数目
			Npumps,					 // 水泵数目
			Nvalves,				 // 阀门数目
			Nreservoirs;			 // 水库数

	vector<Cnode>		 Node;				 // 节点数组
	vector<Cpipe>		 Pipe;				 // 管道数组
	vector<Ctank>		 Tank;				 // 水箱数组
	vector<Cpump>		 Pump;				 // 泵数组
	vector<Cvalve>		 Valve;				 // 阀门数组
	vector<Cjunction>	 Junc;				 // 连接件数组
	vector<Creservior>	 Reser;				 // 水库数组
	Ctime				 t;					 // 时间控制器
}; 
