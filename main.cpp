
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
	


	GetNetworkFile(&net);					//��ȡ��������
	GetPipeFile(&net);						//��ȡ�ܵ�����
	GetJunctionFile(&net);					//��ȡ���Ӽ�����
	GetReservoirFile(&net);					//��ȡˮ������
	GetNodeFile(&net);						//��ȡ�ڵ�����
	GetValveFile(&net);						//��ȡ��������

	
	InitHyd(&net);							//��ʼ����̬ˮ������


	Assemble(&net,t0,dt,Tmax);
	OutputData(&net);


	return 0;
}