/*
 ******************************************************************************
 Project:      函数计算
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
**	功能：获取管网数据
**	输入：txt文件/Cnetwork
**	输出：none
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
**	功能：获取管道数据
**	输入：txt文件/Cpipe/Cnetwork
**	输出：none
-------------------------------------------------------
*/
{
	Cpipe* p = new Cpipe[net->Npipes];							//先申请管道数内存空间
	ifstream IN("pipe.txt");
	IN.ignore(1024, '\n');
	for (int i = 0; i < net->Npipes; i++)						//给管网内的所有管道的所有数据赋值
	{
		IN>>p[i].ID>>p[i].Len>>p[i].D>>p[i].a>>p[i].f>>p[i].seg_num>>p[i].N1.ID>>p[i].N2.ID;
		net->Pipe.emplace_back(p[i]);
	}
	delete[] p;
}
void GetJunctionFile( Cnetwork* net)
/*
------------------------------------------------------
**	功能：获取管网内所有连接件的数据
**	输入：txt文件Cnetwork
**	输出：none
-------------------------------------------------------
*/
{
	ifstream IN;
	Cpipe* p = new Cpipe[10];									//假设每个连接件最多支持十根上游管道连接和十根下游管道连接 
	Cpipe* q = new Cpipe[10];
	Cjunction* junc = new Cjunction[net->Njuncs];
	IN.open("junction.txt");
	for(int n=0;n<net->Njuncs;n++)
	{	
		IN.ignore(1024, '\n');
		IN >> junc[n].ID;
		IN.ignore(1024, '\n');
		IN.ignore(1024, '\n');
		int i = 0;												//存储上游管道编号
		while (IN.peek() != '\n')
		{														//统计非零数的个数
			IN >> p[i].ID;
			i++;
		}
		for (int j = 0; j < i; j++)
		{
			junc[n].uppipe.emplace_back(p[j]);
		}
	/*******************************************************/	//存储下游管道编号
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
	/*******************************************************/	//获取管网内所有连接件数据
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
**	功能：获取管网内所有水库的数据
**	输入：txt文件Creservior
**	输出：none
-------------------------------------------------------
*/
{
Creservior* r = new Creservior[net->Nreservoirs];			//申请水库类型内存空间用来存放临时的水库管道数据
Cpipe p;													//申请一个用来处理水库连接管道的临时对象
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
**	功能：获取节点数据
**	输入：txt文件Cnode
**	输出：none
-------------------------------------------------------
*/
{
	Cnode n;
	ifstream IN("node.txt");
	IN.ignore(1024, '\n');
	for (int i = 0; i < net->Nnodes; i++)
	{
		while (IN.peek() != '\n')								//peek()返回的是刚刚peek的那个字符，所以在循环中还是要用>>来进行下一步操作
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
**	功能：获取管网阀门数据
**	输入：Cnet
**	输出：none
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
**	功能：初始化稳态水力数据，本程序有待拓展完善
	假设阀门处于关闭状态，管道内的流量为0，管道内部所有的水头均为100
**	输入：none
**	输出：none
-------------------------------------------------------
*/
{
	for (int i = 0; i < net->Npipes; i++)
	{
		net->Pipe[i].H.clear();										//在Cpipe类的构造函数中已经定义了H容器并开拓内存空间，这里清理一下内存方便后续赋值（主要是因为原来写好的代码不想重新改了）
		for (int j = 0; j <= net->Pipe[i].seg_num; j++)
		{
			net->Pipe.at(i).H.emplace_back(100);					//假设管网内每一个点都是100m的水头高度，这里都是可以推翻重写的
			net->Pipe.at(i).Q.emplace_back(0);						//假设管网内每一个点都是流量为0
		}
	}
}
void InitPipeSeg(Cpipe* pipe)
/*
------------------------------------------------------
**	功能： 给管道内点开拓内存空间
**	输入：Cpipe
**	输出：none
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
**	功能：计算管道内部点的下一时刻水头和流量，管道内点的情况用vector容器进行表示
**	输入：Cpipe
**	输出：none
-------------------------------------------------------
*/
{
	InitPipeSeg(pipe);												//初始化管道的内节点容器
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
	pipe->CP.back()= H[i - 1] + pipe->B() * Q[i - 1] - pipe->R() * Q[i - 1] * fabs(Q[i - 1]);			//把CP和CM容器填满
	pipe->CP.front()= H[ 1] - pipe->B() * Q[1] - pipe->R() * Q[1] * fabs(Q[1]);
	for (int i = 1; i < pipe->seg_num; i++)							//分两个循环这样写很重要
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
**	功能：把所有连接件的管道添加到Cjunction的上下游管道容器中
**	输入：Cjunction
**	输出：none
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
**	功能：把所有Cjunction的上下游管道容器计算得到的数值添加到Cpipe中
**	输入：Cjunction
**	输出：none
-------------------------------------------------------
*/
{
	Cpipe* pipe = &*net->Pipe.begin();
	for (auto it = net->Junc.begin(); it != net->Junc.end(); it++)						//遍历所有的连接件
	{
		for (auto iter = (*it).uppipe.begin(); iter != (*it).uppipe.end(); iter++)		//遍历单个连接件的流入连接管道（即连接件在管道下游）
		{
			pipe[(*iter).ID - 1].H.back() = (*iter).H.back();												//将连接件对应的管道赋予管网内的管道
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
**	功能：把所有与连接件相连的Cpipe管道添加到Creservior的上下游管道容器中
**	输入：Creservior
**	输出：none
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
**	功能：把所有与连接件相连的Cpipe管道添加到Creservior的下游管道容器中
**	输入：Creservior
**	输出：none
-------------------------------------------------------
*/
{
	for (auto it = net->Reser.begin(); it != net->Reser.end(); it++)					//遍历所有水库
	{
		for (auto iter = (*it).pipe.begin(); iter != (*it).pipe.begin(); iter++)		//遍历每个水库的所有下游管道
		{
			net->Pipe.at((*iter).ID - 1).H.front() = (*it).H;							//将所有水库的下游管道的第一个内节点的水头值赋予Cpipe下的管道（根据索引）
			net->Pipe.at((*iter).ID - 1).Q.front() = (*it).Q;
		}
	}
}
void PipeNodeLink(Cnetwork* net)
/*
------------------------------------------------------
**	功能：把所有管道上下游的节点存入管道的上下游节点容器中
**	输入：Cnetwork
**	输出：none
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
**	功能：计算水库的节点流量值（因为节点水头是不变的，在之后的更新状态就可以了，无需计算
*	输入：Cnetwork
**	输出：none
-------------------------------------------------------
*/
{
	for (auto it = net->Reser.begin(); it != net->Reser.end(); it++)				//遍历所有水库
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
**功能;计算连接件的水头值
**输入：in_account：流入管道数目
		out_account：流出管道数目
		* Cp：流入Cp值地址
		* Cm：流出Cm值地址
		* B：存储各个管道的B值，存储策略为先存储流入管道的B值，再存储流出管道的B值
**输出：连接件水头
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
**	功能：计算连接件的水头值
**	输入：Cjunc
**	输出：none
-------------------------------------------------------
*/
{
	double temp;														//申请一个临时变量用来存计算出的连接件节点水头值
	vector<double>CP;
	vector<double>CM;
	vector<double>B;
	for (auto it = junc->uppipe.begin(); it != junc->uppipe.end(); it++)
	{
		CP.push_back(*((*it).CP.end() - 1));							//在CacuHyd中，CP、CM只在中间内节点进行计算
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
**	功能：连接阀门和上游管道的数据，把管道的数据赋予阀门
**	输入：Cnet
**	输出：none
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
**	功能：计算阀门的节点水头（该阀门设定为只在管网末端，以后可以拓展，此处设定为截止阀,即：阀门开度和流量系数呈线性关系，参考黄源论文）
**	输入：
* t0：	开阀时刻
* t：	当前时刻
* Tmax:	最大开阀历时
*		阀门开阀开阀度随时间变化曲线
**	输出：none
-------------------------------------------------------
*/
{
	Cvalve* valve = &*net->Valve.begin();
	Cpipe* p = &valve->uppipe;
	if (t > t0 and t-t0 <= Tmax)															//阀门是否在允许的时间内
	{
		for (auto it = net->Valve.begin(); it != net->Valve.end(); it++)					//阀门开度和时间的变化曲线关系
		{
			(*it).m_opening = (t - t0) * 0.1;
			(*it).CV = (*it).m_opening;														//阀门开度和CV值相等，采用的是截止阀

			double m_B = (*it).uppipe.B();													//定义一些变量，方便看
			double m_CV = (*it).CV;
			double m_CP = (*it).uppipe.CP.back();

			(*it).uppipe.Q.back() = -m_B * m_CV + sqrt((m_B * m_CV) * (m_B * m_CV) + 2 * m_CV * m_CP);
			(*it).uppipe.H.back() = m_CP - m_B * (*it).uppipe.Q.back();
		}
	}
}
void NewValvePipeLink(Cnetwork* net)															//定义：new代表组件向管道赋值，Link代表管道像组件赋值
/*
------------------------------------------------------
**	功能：更新阀门连接管道的水头和流量
**	输入：
* t0：	开阀时刻
* t：	当前时刻
* Tmax:	最大开阀历时
*		阀门开阀开发度随时间变化曲线
**	输出：none
-------------------------------------------------------
*/
{	
	
	for (auto it = net->Valve.begin(); it != net->Valve.end(); it++)						//遍历每个阀门
	{
		int temp = (*it).uppipe.ID - 1;
		net->Pipe.at(temp).H.back() = (*it).uppipe.H.back();								//每个阀门相连的管道的所有值都赋予对应索引号的管道
		net->Pipe.at(temp).Q.back() = (*it).uppipe.Q.back();
	}
}
void OutputData(Cnetwork* net)
/*
------------------------------------------------------
**	功能：把计算数据读出，腾出内存空间
**	输入：Cnet
**	输出：none
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
**	功能：计算管网内所有管道内点和连接件的下一时刻的H和Q值
**	输入：Cnet
*	t0:起始时刻
*	dt： 时间间隔
*	Tmax：最大时刻
**	输出：none
-------------------------------------------------------
*/
{
	Cpipe* pipe = &*net->Pipe.begin();
	Cjunction* junc = &*net->Junc.begin();
	Creservior* reser = &*net->Reser.begin();
	int m_n=0;																						//迭代次数
	double t = 0;																					//当前时刻，起始时刻为零
	JuncPipeLink(net);
	PipeValveLink(net);
	ReservoirPipeLink(net);
	while (t < Tmax)
	{
		for (auto it = net->Pipe.begin(); it != net->Pipe.end(); it++)
			{
				CacuPipeHyd(&*it);
			}
		JuncPipeLink(net);																			//把计算好的管道数据导入连接件
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
