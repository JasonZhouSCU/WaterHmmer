
#include"func.h"
#include"classes.h"

int main()
{
	double		t0 = 0;
	double		Tmax = 1;
	Cnetwork net;
	Cpipe pipe;
	Cjunction junc;
	double		dt = 100.00 / (1000.00* 2.00);
	


	GetNetworkFile(&net);					//获取管网数据
	GetPipeFile(&net);						//获取管道数据
	GetJunctionFile(&net);					//获取连接件数据
	GetReservoirFile(&net);					//获取水库数据
	GetNodeFile(&net);						//获取节点数据
	GetValveFile(&net);						//获取阀门数据

	
	InitHyd(&net);							//初始化稳态水力数据


	Assemble(&net,t0,dt,Tmax);
	OutputData(&net);


	return 0;
}