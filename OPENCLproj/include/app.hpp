#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <CL/opencl.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

namespace OpenCLApp {
    template<typename T = int>
    class App {
    private:
        cl::Platform platform_;
        cl::Device device_;
        cl::Context context_;
        cl::CommandQueue queue_;
        std::string kernel_;

        static cl::Platform selectGPUplatform ();
        static cl::Device getGPUdevice (cl::Platform &platform);
        static cl::Context getGPUcontext (cl::Device &device);

        using sort_t = cl::KernelFunctor<cl::Buffer, cl_int, cl_int>;

    public:
        App (const std::string &sourceFileName)
            : platform_ (selectGPUplatform ()), device_ (getGPUdevice (platform_)), context_ (getGPUcontext (device_)), queue_ (context_)
        {
            std::cout << device_.getInfo<CL_DEVICE_NAME>() << std::endl;
            
            std::fstream in (sourceFileName, std::ios_base::in);

            while (in) {
                std::string newLine;
                std::getline (in, newLine);
                kernel_ += newLine + "\n";
            }
        }

        void GPUBitonicSort (cl::vector<T> &vec);

        void simpleBitonicSort (cl::vector<T> &vec);
    };


    template <typename T>
    cl::Platform App<T>::selectGPUplatform ()
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

    template <typename T>
    cl::Device App<T>::getGPUdevice (cl::Platform &platform)
    {
        std::vector<cl::Device> devices;
        platform.getDevices (CL_DEVICE_TYPE_GPU, &devices);
        // for (auto device : devices)
        //     std::cout << device.getInfo<CL_DEVICE_NAME>();

        return devices[0];
    }

    template <typename T>
    cl::Context App<T>::getGPUcontext (cl::Device &device)
    {
        return cl::Context (device);
    }

    template <typename T>
    void App<T>::GPUBitonicSort (cl::vector<T> &vec)
    {
        int vecSize = vec.size();
        int size = std::pow (2, int (std::log2 (vec.size())) + 1);
        int bufSize = size * sizeof (cl_int);

        for (int i = vecSize; i < size; ++i)
            vec.push_back (INT32_MAX);

        cl::Buffer clData (context_, CL_MEM_READ_WRITE, bufSize);
        cl::copy(queue_, vec.begin(), vec.end(), clData);

        try {
            cl::Program program (context_, kernel_, true);
            sort_t kernel(program, "bitonicSort");
            
            cl::NDRange GlobalRange (size / 2, 1);
            cl::EnqueueArgs Args(queue_, GlobalRange);
            cl::Event evt = kernel(Args, clData, 2, 2);

            for (int power = 2; power < size;) {
                power <<= 1;
                for (int subPower = power; subPower > 1; subPower >>= 1) {
                    cl::NDRange locGlobalRange (size / subPower, subPower >> 1);
                    cl::EnqueueArgs locArgs (queue_, evt, locGlobalRange);

                    evt = kernel(locArgs, clData, subPower, power);
                }
            }
            evt.wait();
            
        } catch (cl::BuildError &err) {
            std::cerr << "OCL BUILD ERROR: " << err.err() << ":" << err.what()
                        << std::endl;
            std::cerr << "-- Log --\n";
            for (auto e : err.getBuildLog())
                std::cerr << e.second;
            std::cerr << "-- End log --\n";
            return;
        }

        cl::copy(queue_, clData, vec.begin(), vec.begin() + vecSize);
        
        vec.erase (vec.begin() + vecSize, vec.end());
    }

    template <typename T>
    void App<T>::simpleBitonicSort (cl::vector<T> &vec)
    {
        int vecSize = vec.size();
        int newVecSize = std::pow (2, std::log2 (vec.size()) + 1);

        for (int i = vecSize; i < newVecSize; ++i) {
            vec.push_back (INT32_MAX);
        }
        size_t size = vec.size();

        for (size_t power = 1; power < size;) {
            power <<= 1;
            for (size_t subPower = power; subPower > 1; subPower >>= 1) {
                size_t half = subPower >> 1;
                for (size_t indexPow = 0; indexPow < size; indexPow += subPower) {
                    bool up = indexPow / power % 2;
                    for (size_t i = 0; i < half; ++i) {
                        if (up ^ (vec[indexPow + i] > vec[indexPow + half + i]))
                            std::swap (vec[indexPow + i], vec[indexPow + half + i]);
                    }
                }
            }
        }

        vec.erase (vec.begin() + vecSize, vec.end());
    }
}  // namespace OpenCLApp