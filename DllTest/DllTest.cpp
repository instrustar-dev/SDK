// DllTest.cpp: 定义应用程序的入口点。
//

#include "DllTest.h"
#include "VdsoLib.h"
#include <thread>

bool runing = true;
int sample_num = 0;
unsigned int* sample = NULL;
unsigned int mem_length = 0;
double* buffer = NULL;

void CALLBACK DevDataReadyCallBack(void* ppara)
{
	std::cout << "DevDataReadyCallBack\n";
}

void CALLBACK DevNoticeAddCallBack(void* ppara)
{
    std::cout << "DevNoticeAddCallBack\n";
	//sample
	int sample_num = GetOscSupportSampleNum();
	if (sample != NULL)
	{
		delete[]sample;
		sample = NULL;
	}
	sample = new unsigned int[sample_num];
	if (GetOscSupportSamples(sample, sample_num))
	{
		for (int i = 0; i < sample_num; i++)
			std::cout << sample[i] << '\n';
		std::cout << std::endl;
	}
	SetOscSample(sample[sample_num-2]);

	//
	mem_length =  GetMemoryLength()*1024;  //KB
	buffer = new double[mem_length];

	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	Capture(mem_length/1024, 0);
}

void CALLBACK DevNoticeRemoveCallBack(void* ppara)
{
     std::cout << "DevNoticeRemoveCallBack\n";
	 runing = false;
	sample_num = 0;
	if (sample != NULL)
	{
		delete[]sample;
		sample = NULL;
	}

	if (buffer != NULL)
	{
		delete[]buffer;
		buffer = NULL;
	}
}

int main()
{
	std::cout << "Vdso Test..." << std::endl;

	InitDll();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	SetDevNoticeCallBack(NULL, DevNoticeAddCallBack, DevNoticeRemoveCallBack);
	SetDataReadyCallBack(NULL, DevDataReadyCallBack);
	
	while (runing)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if(IsDevAvailable())
        {
            if (IsDataReady())
            {
                unsigned int len = ReadVoltageDatas(0, buffer, mem_length);
                
				double minv=buffer[0];
				double maxv=buffer[0];
				for(unsigned int i=0; i<len; i++)
				{
					minv = buffer[i]<minv? buffer[i]:minv;
					maxv = buffer[i]>maxv? buffer[i]:maxv;
				}
				std::cout << "ReadVoltageDatas " << len <<" minv " << minv << " maxv " << maxv << '\n';

                Capture(mem_length / 1024, 0);
            }
        }
	};

	FinishDll();
	std::cout << "...Vdso Test" << std::endl;
	return 0;
}
