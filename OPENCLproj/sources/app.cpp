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
        return cl::Context (device);
    }

    void App::GPUBitonicSort (cl_int *data, size_t size)
    {
        size_t bufSize = size * sizeof (cl_int);
        cl::Buffer clData (context_, CL_MEM_READ_WRITE, bufSize);
        cl::copy(queue_, data, data + size, clData);

        try {
            cl::Program program (context_, program_, true);
            sort_t kernel(program, "bitonicSort");

            for (size_t power = 1; power < size;) {
                power <<= 1;
                for (size_t subPower = power; subPower > 1; subPower >>= 1) {
                    cl::NDRange GlobalRange (size / subPower, subPower >> 1);
                    cl::EnqueueArgs Args(queue_, GlobalRange);

                    cl::Event evt = kernel(Args, clData, subPower, power);
                    evt.wait();
                }
            }
        } catch (cl::BuildError &err) {
            std::cerr << "OCL BUILD ERROR: " << err.err() << ":" << err.what()
                        << std::endl;
            std::cerr << "-- Log --\n";
            for (auto e : err.getBuildLog())
                std::cerr << e.second;
            std::cerr << "-- End log --\n";
            return;
        }

        cl::copy(queue_, clData, data, data + size);
    }

    void App::CPUBitonicSort (cl_int *data, int size)
    {
        for (int power = 1; power < size;) {
            power <<= 1;
            for (int subPower = power; subPower > 1; subPower >>= 1) {
                
                
                for (int indexPow = 0; indexPow < size; indexPow += subPower) { //can parallel
                    int half = subPower >> 1;
                    bool up = indexPow / power % 2;
                    for (int i = 0; i < half; ++i) {  //can parallel
                        if (up ^ (data[indexPow + i] > data[indexPow + half + i]))
                            std::swap (data[indexPow + i], data[indexPow + half + i]);
                    }
                }   //barier


            }   //barier
        }
    }
}  // namespace OpenCLApp
