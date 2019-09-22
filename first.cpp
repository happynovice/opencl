// opencl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include <CL/cl.h>
#include <CL/cl.hpp>
#include<vector>
#include<string>
int main()
{
	cl_int ret;
	cl_uint num=0;
	std::vector<cl::Platform> platforms_cpp;
	cl::Platform::get(&platforms_cpp);
	std::vector<cl::Device> devices_cpp;
	platforms_cpp[0].getDevices(CL_DEVICE_TYPE_GPU,&devices_cpp);
	auto str=devices_cpp[0].getInfo<CL_DEVICE_NAME>();
	platforms_cpp[1].getDevices(CL_DEVICE_TYPE_GPU, &devices_cpp);
	auto str2 = devices_cpp[0].getInfo<CL_DEVICE_NAME>();
	std::cout << "device1:"<< str<<" device2:"<< str2 <<"\n";
	std::cout << "platforms_cpp.size():" << platforms_cpp.size() <<" devices_cpp.size():"<< devices_cpp.size() << "\n";
	ret = clGetPlatformIDs(0, 0, &num);
	std::cout << "clGetPlatformIDs:"<<num<<"\n";
	std::vector<cl_platform_id> platforms(num);
	cl_platform_id *platforms_data = (cl_platform_id *)platforms.data();
	
	for (int i = 0; i < num; i++) {
		
		ret = clGetPlatformIDs(num, platforms_data, NULL);
		if (ret != CL_SUCCESS) {
			std::cout << "Unable to get platform ID\n";
			return 0;
		}

		cl_context_properties prop[] = { CL_CONTEXT_PLATFORM,reinterpret_cast<cl_context_properties>(platforms[i]),0 };
		cl_context context = clCreateContextFromType(prop, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL);
		if (context == 0) {
			std::cerr << "Unable to create opencl context \n";
			return 0;
		}
		size_t cb;
		clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &cb);

		std::vector<cl_device_id> devices(cb / sizeof(cl_device_id));
		std::cout << "devices.size():" << devices.size() << " cb:" << cb << "\n";
		for (int i = 0; i < devices.size(); i++)
		{
			clGetContextInfo(context, CL_CONTEXT_DEVICES, cb, &devices[i], 0);

			clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 0, NULL, &cb);

			std::string dev_name;
			dev_name.resize(cb);
			clGetDeviceInfo(devices[i], CL_DEVICE_NAME, cb, &dev_name[0], 0);
			std::cout << "Device: " << dev_name.c_str() << "\n";

		}
		clReleaseContext(context);
	}


	
	std::cout << "hello opencl \n";
    return 0;
}

