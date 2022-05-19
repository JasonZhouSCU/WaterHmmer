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
#define MAX	10000			//������������ܵ������Ӽ��ȣ�
#define DEN 1000			//ˮ���ܶ�
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
	int ID;					//�ڵ�����
	int m_Ntype;			//�ڵ�����
	double X;				//�ڵ�����
	double Y;
	double Z;

};
class Ctank
{
public:
	int     ID;				 // �ڵ�����
	double  A;               // ˮ�����
	double  Hmin;            // ��С�߶�
	double  Hmax;            // ���߶�
	double  H0;              // ��ʼ�߶�
	double  Vmin;            // ��С���
	double  Vmax;            // ������
	double  V0;              // ��ʼ�����
	double  V;               // ˮ�����
};


class Cpipe
{
public:
	int      ID;				 //�ܵ����
	int		 seg_num;			 //�ܵ��ֶ���Ŀ
	double   D;					 //ֱ��
	double   Len;				 //����
	double   a;					 //����
	double   Kc;				 //�ֲڶ�
	double   f;					 //����ϵ��

	Cnode			N1;			//��ʼ�ڵ�
	Cnode			N2;
	vector<double>	H;			//ÿ���ܵ�����Ӧ�Ľڵ��ˮͷ/����ֵ
	vector<double>	Q;
	vector<double>	CP;			//ÿ���ܵ�����Ӧ�Ľڵ��CP/CMֵ�����壺�ܵ���ÿ���ڵ㶼��Ӧһ��CP/CMֵ���׶�ֻ��CM��ĩ��ֻ��CPֵ��
	vector<double>	CM;

	Cpipe();
	~Cpipe();
	virtual double B()			//�����麯��������Ϊ�˺�������չ��Ŀǰû��ʲôӦ��
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
	int		 ID;				//ˮ������
	double   H;					//ˮͷֵ
	double	 Q;					//����ֵ
	vector<Cpipe> pipe;			//ˮ�����ӹܵ�
};
class Cpump
{
	int     ID;					 // ˮ������
	int     Ptype;				 // ˮ����������
	double  Q0;					 // ��ʼ����
	double  Qmax;				 // �������
	double  Hmax;				 // ���ˮͷ
	double  H0;					 // �ط�ˮͷ
	double  R;					 // ����ϵ��
	int     Hcurve;				 // ˮ����������
	int     Epat;				 // �����������
	double  Ecost;				 // ��λ�������
};
class Cvalve
{
public:
	int		ID;					//��������
	
	Cpipe	uppipe;				//���ιܵ�
	double  m_opening;		//���㷧�ŵĿ�����
	double	CV;				//���㷧�ŵ�CVֵ
};
class Cjunction
{
public:
	int		 ID;					//�ڵ�ID
	double	 Q;						//�ڵ�����
	double	 H;						//�ڵ�ˮͷ
	vector<Cpipe> uppipe;	        //���ιܵ�����
	vector<Cpipe> downpipe;		    //���ιܵ�����
};
class Ctime							//ʱ�������
{
public:
	int		n;						//��������
	double	dt;						//ʱ�䲽��
	double	Tmax;					//���ʱ��
	friend	Cvalve;
};
class Cnetwork
{
	public:
		int
			Nnodes,					 // �ڵ���Ŀ
			Ntanks,					 // ˮ����Ŀ
			Njuncs,					 // ���Ӽ���Ŀ
			Npipes,					 // �ܵ���Ŀ
			Npumps,					 // ˮ����Ŀ
			Nvalves,				 // ������Ŀ
			Nreservoirs;			 // ˮ����

	vector<Cnode>		 Node;				 // �ڵ�����
	vector<Cpipe>		 Pipe;				 // �ܵ�����
	vector<Ctank>		 Tank;				 // ˮ������
	vector<Cpump>		 Pump;				 // ������
	vector<Cvalve>		 Valve;				 // ��������
	vector<Cjunction>	 Junc;				 // ���Ӽ�����
	vector<Creservior>	 Reser;				 // ˮ������
	Ctime				 t;					 // ʱ�������
}; 
