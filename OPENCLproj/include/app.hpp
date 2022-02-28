#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <CL/opencl.hpp>
#include <iostream>
#include <fstream>

namespace OpenCLApp {
    class App {
    private:
        cl::Platform platform_;
        cl::Device device_;
        cl::Context context_;
        cl::CommandQueue queue_;
        std::string program_;

        static cl::Platform selectGPUplatform ();
        static cl::Device getGPUdevice (cl::Platform &platform);
        static cl::Context getGPUcontext (cl::Device &device);

    public:
        App (const std::string &sourceFileName)
            : platform_ (selectGPUplatform ()), device_ (getGPUdevice (platform_)), context_ (getGPUcontext (device_)), queue_ (context_, device_)
        {
            std::fstream in (sourceFileName, std::ios_base::in);
                std::cout  << sourceFileName;
            while (in) {
                std::string newLine;
                std::getline (in, newLine);
                program_ += newLine + "\n";
            }

            std::cout << program_;
        }

        void BitonicSort (cl_int const *data, size_t size);
    };
}  // namespace OpenCLApp