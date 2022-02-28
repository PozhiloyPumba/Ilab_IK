#include "app.hpp"

namespace OpenCLApp {

    cl::Platform App::selectGPUplatform ()
    {
        cl::vector<cl::Platform> platforms;

        cl::Platform::get (&platforms);

        for (auto curIt = platforms.rbegin (), endIt = platforms.rend ();
             curIt != endIt;
             ++curIt) {  // from end to begin
            cl_uint numDevices = 0;
            ::clGetDeviceIDs ((*curIt) (), CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
            // std::vector<cl::Device> devices;
            // (*curIt).getDevices(CL_DEVICE_TYPE_ALL, &devices);
            // for (auto device: devices) {
            //     std::cout << device.getInfo<CL_DEVICE_NAME>();
            // }
            if ((*curIt).getInfo<CL_PLATFORM_NAME> () == "NVIDIA CUDA")  // TODO only for me
                return *curIt;
        }
        throw std::runtime_error ("No platform with GPU devices(((");
    }

    cl::Device App::getGPUdevice (cl::Platform &platform)
    {
        std::vector<cl::Device> devices;
        platform.getDevices (CL_DEVICE_TYPE_GPU, &devices);
        // for (auto device : devices)
        //     std::cout << device.getInfo<CL_DEVICE_NAME>();

        return devices[0];
    }

    cl::Context App::getGPUcontext (cl::Device &device)
    {
        cl::vector<cl::Device> contextDevices;
        contextDevices.push_back (device);

        return cl::Context (contextDevices);
    }

    void App::BitonicSort (cl_int *data, size_t size)
    {
        size_t bufSize = size * sizeof (cl_int);
        cl::Buffer clData (context_, CL_MEM_READ_WRITE, bufSize);
        cl::copy(queue_, data, data + size, clData);

        cl::Program program (context_, program_, true);

        sort_t kernel(program, "bitonicSort");

        cl::NDRange GlobalRange (size);
        cl::NDRange LocalRange (1); //TODO!!!
        cl::EnqueueArgs Args(queue_, GlobalRange, LocalRange);

        cl::Event evt = kernel(Args, clData);
        evt.wait();

        cl::copy(queue_, clData, data, data + size);
    }

}  // namespace OpenCLApp
