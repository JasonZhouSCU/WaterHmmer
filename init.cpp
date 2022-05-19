#include"classes.h"
Cpipe:: Cpipe()
{
	a = 0;
	ID = 0;
	Len = 0;
	D = 0;
	f = 0;
	seg_num = 0;
	Kc = 0;
	CM.resize(seg_num);
	CP.resize(seg_num);
	H.resize(seg_num);
	Q.resize(seg_num);
}
Cpipe::~Cpipe()
{
	CM.clear();
	CP.clear();
	H.clear();
	Q.clear();
}
