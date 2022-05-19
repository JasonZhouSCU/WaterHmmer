/*
 ******************************************************************************
 Project:      ��������
 Version:      1.0
 Module:       funcs.h
 Description:  prototypes of external functions called by various modules
 Authors:      Jason
 Last Updated: 04/28/2022
 ******************************************************************************
*/
#include"classes.h"
#include<fstream>
#include<unordered_map>

void GetNetworkFile(Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ���ȡ��������
**	���룺txt�ļ�/Cnetwork
**	�����none
-------------------------------------------------------
*/
{
	ifstream IN("network.txt");
	IN.ignore(1024,'\n');
	IN >> net->Nnodes >> net->Npipes >> net->Njuncs >> net->Ntanks >> net->Nvalves >> net->Npumps >> net->Nreservoirs;
}
void GetPipeFile(Cnetwork *net)
/*
------------------------------------------------------
**	���ܣ���ȡ�ܵ�����
**	���룺txt�ļ�/Cpipe/Cnetwork
**	�����none
-------------------------------------------------------
*/
{
	Cpipe* p = new Cpipe[net->Npipes];							//������ܵ����ڴ�ռ�
	ifstream IN("pipe.txt");
	IN.ignore(1024, '\n');
	for (int i = 0; i < net->Npipes; i++)						//�������ڵ����йܵ����������ݸ�ֵ
	{
		IN>>p[i].ID>>p[i].Len>>p[i].D>>p[i].a>>p[i].f>>p[i].seg_num>>p[i].N1.ID>>p[i].N2.ID;
		net->Pipe.emplace_back(p[i]);
	}
	delete[] p;
}
void GetJunctionFile( Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ���ȡ�������������Ӽ�������
**	���룺txt�ļ�Cnetwork
**	�����none
-------------------------------------------------------
*/
{
	ifstream IN;
	Cpipe* p = new Cpipe[10];									//����ÿ�����Ӽ����֧��ʮ�����ιܵ����Ӻ�ʮ�����ιܵ����� 
	Cpipe* q = new Cpipe[10];
	Cjunction* junc = new Cjunction[net->Njuncs];
	IN.open("junction.txt");
	for(int n=0;n<net->Njuncs;n++)
	{	
		IN.ignore(1024, '\n');
		IN >> junc[n].ID;
		IN.ignore(1024, '\n');
		IN.ignore(1024, '\n');
		int i = 0;												//�洢���ιܵ����
		while (IN.peek() != '\n')
		{														//ͳ�Ʒ������ĸ���
			IN >> p[i].ID;
			i++;
		}
		for (int j = 0; j < i; j++)
		{
			junc[n].uppipe.emplace_back(p[j]);
		}
	/*******************************************************/	//�洢���ιܵ����
		IN.ignore(1024, '\n');
		IN.ignore(1024, '\n');
		i = 0;	
		while( IN.peek() != '\n')
		{
			IN >> q[i].ID;
			i++;
		}
		for (int j = 0; j < i; j++)
		{
			junc[n].downpipe.emplace_back(q[j]);
		}
	/*******************************************************/	//��ȡ�������������Ӽ�����
		net->Junc.push_back(junc[n]);
		IN.ignore(1024, '\n');
		IN.ignore(1024, '\n');
	}
	delete[]p;
	delete[]q;
	delete[]junc;
}
void GetReservoirFile(Cnetwork *net)
/*
------------------------------------------------------
**	���ܣ���ȡ����������ˮ�������
**	���룺txt�ļ�Creservior
**	�����none
-------------------------------------------------------
*/
{
Creservior* r = new Creservior[net->Nreservoirs];			//����ˮ�������ڴ�ռ����������ʱ��ˮ��ܵ�����
Cpipe p;													//����һ����������ˮ�����ӹܵ�����ʱ����
ifstream IN("reservior.txt");
for (int i = 0; i < net->Nreservoirs; i++)
{
	IN.ignore(1024, '\n');
	IN >> r[i].ID;
	net->Reser.push_back(r[i]);
	IN.ignore(1024, '\n');
	IN.ignore(1024, '\n');
	while (IN.peek() != '\n')
	{
		IN >> p.ID;
		net->Reser.at(i).pipe.emplace_back(p);
	}
	IN.ignore(1024, '\n');
	IN.ignore(1024, '\n');
	IN >> net->Reser.at(i).H;
	IN.ignore(1024, '\n');
	IN.ignore(1024, '\n');
}
delete[]r;
}
void GetNodeFile(Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ���ȡ�ڵ�����
**	���룺txt�ļ�Cnode
**	�����none
-------------------------------------------------------
*/
{
	Cnode n;
	ifstream IN("node.txt");
	IN.ignore(1024, '\n');
	for (int i = 0; i < net->Nnodes; i++)
	{
		while (IN.peek() != '\n')								//peek()���ص��Ǹո�peek���Ǹ��ַ���������ѭ���л���Ҫ��>>��������һ������
		{
			IN >> n.ID >> n.m_Ntype;
			net->Node.emplace_back(n);
		}
		IN.ignore(1024, '\n');
	}
}
void GetValveFile(Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ���ȡ������������
**	���룺Cnet
**	�����none
-------------------------------------------------------
*/
{
	ifstream IN("valve.txt");
	net->Valve.resize(net->Nvalves);
	IN.ignore(1024, '\n');
	for (int i = 0; i < net->Nvalves; i++)
	{
		while (IN.peek() != '\n')
		{
			IN >> net->Valve[i].ID >> net->Valve[i].uppipe.ID;
		}
	}
}
void InitHyd(Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ���ʼ����̬ˮ�����ݣ��������д���չ����
	���跧�Ŵ��ڹر�״̬���ܵ��ڵ�����Ϊ0���ܵ��ڲ����е�ˮͷ��Ϊ100
**	���룺none
**	�����none
-------------------------------------------------------
*/
{
	for (int i = 0; i < net->Npipes; i++)
	{
		net->Pipe[i].H.clear();										//��Cpipe��Ĺ��캯�����Ѿ�������H�����������ڴ�ռ䣬��������һ���ڴ淽�������ֵ����Ҫ����Ϊԭ��д�õĴ��벻�����¸��ˣ�
		for (int j = 0; j <= net->Pipe[i].seg_num; j++)
		{
			net->Pipe.at(i).H.emplace_back(100);					//���������ÿһ���㶼��100m��ˮͷ�߶ȣ����ﶼ�ǿ����Ʒ���д��
			net->Pipe.at(i).Q.emplace_back(0);						//���������ÿһ���㶼������Ϊ0
		}
	}
}
void InitPipeSeg(Cpipe* pipe)
/*
------------------------------------------------------
**	���ܣ� ���ܵ��ڵ㿪���ڴ�ռ�
**	���룺Cpipe
**	�����none
-------------------------------------------------------
*/
{
	for (int i = 0; i < pipe->seg_num; i++)
	{
		pipe->CM.emplace_back(0);
		pipe->CP.emplace_back(0);
	}
}
void CacuPipeHyd(Cpipe* pipe)
/*
------------------------------------------------------
**	���ܣ�����ܵ��ڲ������һʱ��ˮͷ���������ܵ��ڵ�������vector�������б�ʾ
**	���룺Cpipe
**	�����none
-------------------------------------------------------
*/
{
	InitPipeSeg(pipe);												//��ʼ���ܵ����ڽڵ�����
	double* CP = &pipe->CP[0];
	double* CM = &pipe->CM[0];
	double* H = &pipe->H[0];
	double* Q = &pipe->Q[0];
	double* HP = new double[pipe->seg_num];
	double* QP = new double[pipe->seg_num];
	for (int i = 1; i < pipe->seg_num; i++)
	{
		CP[i] = H[i - 1] + pipe->B() * Q[i - 1] - pipe->R() * Q[i - 1] * fabs(Q[i - 1]);
		CM[i] = H[i + 1] - pipe->B() * Q[i + 1] - pipe->R() * Q[i + 1] * fabs(Q[i + 1]);
		HP[i] = 0.5 * (CP[i] + CM[i]);
		QP[i] = (CP[i] - CM[i]) / pipe->B();
	}
	int i = pipe->seg_num;
	pipe->CP.back()= H[i - 1] + pipe->B() * Q[i - 1] - pipe->R() * Q[i - 1] * fabs(Q[i - 1]);			//��CP��CM��������
	pipe->CP.front()= H[ 1] - pipe->B() * Q[1] - pipe->R() * Q[1] * fabs(Q[1]);
	for (int i = 1; i < pipe->seg_num; i++)							//������ѭ������д����Ҫ
	{
		H[i] = HP[i];
		Q[i] = QP[i];
	}
	delete[]HP;
	delete[]QP;
}
void JuncPipeLink(Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ����������Ӽ��Ĺܵ���ӵ�Cjunction�������ιܵ�������
**	���룺Cjunction
**	�����none
-------------------------------------------------------
*/
{
	Cpipe* pipe = &net->Pipe[0];
	for (auto it = net->Junc.begin(); it != net->Junc.end(); it++)
	{
		for (auto iter = (*it).uppipe.begin(); iter != (*it).uppipe.end(); iter++)
		{
			(*iter) = pipe[(*iter).ID - 1];
			cout << (*iter).H.back() << endl;
		}
	}
	for (auto it = net->Junc.begin(); it != net->Junc.end(); it++)
	{
		for (auto iter = (*it).downpipe.begin(); iter != (*it).downpipe.end(); iter++)
		{
			(*iter) = pipe[(*iter).ID - 1];
		}
	}
}
void NewJuncPipeLink(Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ�������Cjunction�������ιܵ���������õ�����ֵ��ӵ�Cpipe��
**	���룺Cjunction
**	�����none
-------------------------------------------------------
*/
{
	Cpipe* pipe = &*net->Pipe.begin();
	for (auto it = net->Junc.begin(); it != net->Junc.end(); it++)						//�������е����Ӽ�
	{
		for (auto iter = (*it).uppipe.begin(); iter != (*it).uppipe.end(); iter++)		//�����������Ӽ����������ӹܵ��������Ӽ��ڹܵ����Σ�
		{
			pipe[(*iter).ID - 1].H.back() = (*iter).H.back();												//�����Ӽ���Ӧ�Ĺܵ���������ڵĹܵ�
			pipe[(*iter).ID - 1].Q.back() = (*iter).Q.back();
		}
	}
	for (auto it = net->Junc.begin(); it != net->Junc.end(); it++)
	{
		for (auto iter = (*it).downpipe.begin(); iter != (*it).downpipe.end(); iter++)
		{
			 pipe[(*iter).ID - 1].H.front() = (*iter).H.front();
			 pipe[(*iter).ID - 1].Q.front() = (*iter).Q.front();
		}
	}
}
void ReservoirPipeLink(Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ������������Ӽ�������Cpipe�ܵ���ӵ�Creservior�������ιܵ�������
**	���룺Creservior
**	�����none
-------------------------------------------------------
*/
{
	for (auto it = net->Reser.begin(); it != net->Reser.end(); it++)
	{
		for (auto iter = (*it).pipe.begin(); iter != (*it).pipe.end(); iter++)
		{
			(*iter) = net->Pipe.at((*iter).ID - 1);
		}
	}
}
void NewReservoirPipeLink(Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ������������Ӽ�������Cpipe�ܵ���ӵ�Creservior�����ιܵ�������
**	���룺Creservior
**	�����none
-------------------------------------------------------
*/
{
	for (auto it = net->Reser.begin(); it != net->Reser.end(); it++)					//��������ˮ��
	{
		for (auto iter = (*it).pipe.begin(); iter != (*it).pipe.begin(); iter++)		//����ÿ��ˮ����������ιܵ�
		{
			net->Pipe.at((*iter).ID - 1).H.front() = (*it).H;							//������ˮ������ιܵ��ĵ�һ���ڽڵ��ˮͷֵ����Cpipe�µĹܵ�������������
			net->Pipe.at((*iter).ID - 1).Q.front() = (*it).Q;
		}
	}
}
void PipeNodeLink(Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ������йܵ������εĽڵ����ܵ��������νڵ�������
**	���룺Cnetwork
**	�����none
-------------------------------------------------------
*/
{
	Cnode* node = &net->Node.at(0);
	Cpipe* pipe = &net->Pipe.at(0);
	for (auto it = net->Pipe.begin(); it != net->Pipe.end(); it++)
	{
		(*it).N1 = node[(*it).N1.ID - 1];
		(*it).N2 = node[(*it).N2.ID - 1];
	}
}
void CacuReserviorHyd(Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ�����ˮ��Ľڵ�����ֵ����Ϊ�ڵ�ˮͷ�ǲ���ģ���֮��ĸ���״̬�Ϳ����ˣ��������
*	���룺Cnetwork
**	�����none
-------------------------------------------------------
*/
{
	for (auto it = net->Reser.begin(); it != net->Reser.end(); it++)				//��������ˮ��
	{
		for (auto iter = (*it).pipe.begin(); iter != (*it).pipe.begin(); iter++)
		{
			(*it).Q = ((*iter).CP.front() - (*it).H) / (*iter).B();
		}
	}
}
double CacuHP(int in_account, int out_account, double* Cp, double* Cm, double* B)
/*
-------------------------------------------------------------
**����;�������Ӽ���ˮͷֵ
**���룺in_account������ܵ���Ŀ
		out_account�������ܵ���Ŀ
		* Cp������Cpֵ��ַ
		* Cm������Cmֵ��ַ
		* B���洢�����ܵ���Bֵ���洢����Ϊ�ȴ洢����ܵ���Bֵ���ٴ洢�����ܵ���Bֵ
**��������Ӽ�ˮͷ
-----------------------------------------------------------------
*/
{
	double SumB = 0;
	double CPB = 0, CMB = 0;

	for (int i = 0; i < in_account + out_account; i++)
	{
		if (B[i] != 0)
		{
			SumB += 1 / B[i];
		}
		else
		{
			std::cout << "error!" << std::endl;
		}
	}
	for (int i = 0; i < in_account; i++)
	{
		CPB += Cp[i] / B[i];
	}
	for (int i = 0; i < out_account; i++)
	{
		CMB += Cm[i] / B[i + in_account];
	}
	return (CPB + CMB) / SumB;
}
void CacuJunctionHyd(Cjunction* junc)
/*
------------------------------------------------------
**	���ܣ��������Ӽ���ˮͷֵ
**	���룺Cjunc
**	�����none
-------------------------------------------------------
*/
{
	double temp;														//����һ����ʱ�������������������Ӽ��ڵ�ˮͷֵ
	vector<double>CP;
	vector<double>CM;
	vector<double>B;
	for (auto it = junc->uppipe.begin(); it != junc->uppipe.end(); it++)
	{
		CP.push_back(*((*it).CP.end() - 1));							//��CacuHyd�У�CP��CMֻ���м��ڽڵ���м���
	}
	for (auto it = junc->downpipe.begin(); it != junc->downpipe.end(); it++)
	{
		CM.push_back(*((*it).CM.begin() + 1));
	}
	for (auto it = junc->uppipe.begin(); it != junc->uppipe.end(); it++)
	{
		B.push_back((*it).B());
	}
	for (auto it = junc->downpipe.begin(); it != junc->downpipe.end(); it++)
	{
		B.push_back((*it).B());
	}
	temp = CacuHP(junc->uppipe.size(), junc->downpipe.size(), &*CP.begin(), &*CM.begin(), &*B.begin());
	junc->H = temp;
}
void PipeValveLink(Cnetwork *net)
/*
------------------------------------------------------
**	���ܣ����ӷ��ź����ιܵ������ݣ��ѹܵ������ݸ��跧��
**	���룺Cnet
**	�����none
-------------------------------------------------------
*/
{
	Cvalve*	 valve	=	&*net->Valve.begin();
	Cpipe*	 pipe	=	&*net->Pipe.begin();
	valve->uppipe=pipe[valve->uppipe.ID - 1];
}
void CacuValve(Cnetwork *net,double t0,double t,double Tmax )
/*
------------------------------------------------------
**	���ܣ����㷧�ŵĽڵ�ˮͷ���÷����趨Ϊֻ�ڹ���ĩ�ˣ��Ժ������չ���˴��趨Ϊ��ֹ��,�������ſ��Ⱥ�����ϵ�������Թ�ϵ���ο���Դ���ģ�
**	���룺
* t0��	����ʱ��
* t��	��ǰʱ��
* Tmax:	��󿪷���ʱ
*		���ſ�����������ʱ��仯����
**	�����none
-------------------------------------------------------
*/
{
	Cvalve* valve = &*net->Valve.begin();
	Cpipe* p = &valve->uppipe;
	if (t > t0 and t-t0 <= Tmax)															//�����Ƿ��������ʱ����
	{
		for (auto it = net->Valve.begin(); it != net->Valve.end(); it++)					//���ſ��Ⱥ�ʱ��ı仯���߹�ϵ
		{
			(*it).m_opening = (t - t0) * 0.1;
			(*it).CV = (*it).m_opening;														//���ſ��Ⱥ�CVֵ��ȣ����õ��ǽ�ֹ��

			double m_B = (*it).uppipe.B();													//����һЩ���������㿴
			double m_CV = (*it).CV;
			double m_CP = (*it).uppipe.CP.back();

			(*it).uppipe.Q.back() = -m_B * m_CV + sqrt((m_B * m_CV) * (m_B * m_CV) + 2 * m_CV * m_CP);
			(*it).uppipe.H.back() = m_CP - m_B * (*it).uppipe.Q.back();
		}
	}
}
void NewValvePipeLink(Cnetwork* net)															//���壺new���������ܵ���ֵ��Link����ܵ��������ֵ
/*
------------------------------------------------------
**	���ܣ����·������ӹܵ���ˮͷ������
**	���룺
* t0��	����ʱ��
* t��	��ǰʱ��
* Tmax:	��󿪷���ʱ
*		���ſ�����������ʱ��仯����
**	�����none
-------------------------------------------------------
*/
{	
	
	for (auto it = net->Valve.begin(); it != net->Valve.end(); it++)						//����ÿ������
	{
		int temp = (*it).uppipe.ID - 1;
		net->Pipe.at(temp).H.back() = (*it).uppipe.H.back();								//ÿ�����������Ĺܵ�������ֵ�������Ӧ�����ŵĹܵ�
		net->Pipe.at(temp).Q.back() = (*it).uppipe.Q.back();
	}
}
void OutputData(Cnetwork* net)
/*
------------------------------------------------------
**	���ܣ��Ѽ������ݶ������ڳ��ڴ�ռ�
**	���룺Cnet
**	�����none
-------------------------------------------------------
*/
{
	ofstream OUT("output.txt");
	for (auto it = net->Pipe.begin(); it != net->Pipe.end(); it++)
	{
		for (auto iter = (*it).H.begin(); iter != (*it).H.end(); iter++)
		{
			OUT << (*iter) << "	";
		}
		OUT << endl;
	}
}
void Assemble(Cnetwork* net, double t0,double dt,double Tmax )
/*
------------------------------------------------------
**	���ܣ�������������йܵ��ڵ�����Ӽ�����һʱ�̵�H��Qֵ
**	���룺Cnet
*	t0:��ʼʱ��
*	dt�� ʱ����
*	Tmax�����ʱ��
**	�����none
-------------------------------------------------------
*/
{
	Cpipe* pipe = &*net->Pipe.begin();
	Cjunction* junc = &*net->Junc.begin();
	Creservior* reser = &*net->Reser.begin();
	int m_n=0;																						//��������
	double t = 0;																					//��ǰʱ�̣���ʼʱ��Ϊ��
	JuncPipeLink(net);
	PipeValveLink(net);
	ReservoirPipeLink(net);
	while (t < Tmax)
	{
		for (auto it = net->Pipe.begin(); it != net->Pipe.end(); it++)
			{
				CacuPipeHyd(&*it);
			}
		JuncPipeLink(net);																			//�Ѽ���õĹܵ����ݵ������Ӽ�
		ReservoirPipeLink(net);
		PipeValveLink(net);

		for (auto it = net->Junc.begin(); it != net->Junc.end(); it++)
		{
			CacuJunctionHyd(&*it);
		}
		NewJuncPipeLink(net);
		CacuReserviorHyd(net);
		NewReservoirPipeLink(net);
		CacuValve(net, t0, t, Tmax);
		NewValvePipeLink(net);
		t += dt;
		m_n++;
		cout << net->Pipe.at(4).Q.back() << endl;
	}
		
}
