#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <CL/opencl.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

namespace OpenCLApp {

    // clang-format off
    template <typename T>
    const std::string getTypeName ();

    template <> inline const std::string getTypeName<int> ()                {   return "int"; }
    template <> inline const std::string getTypeName<float> ()              {   return "float"; }
    template <> inline const std::string getTypeName<double> ()             {   return "double"; }
    template <> inline const std::string getTypeName<short> ()              {   return "short"; }
    template <> inline const std::string getTypeName<char> ()               {   return "char"; }
    template <> inline const std::string getTypeName<long long int> ()      {   return "long int"; }
    template <> inline const std::string getTypeName<unsigned char> ()      {   return "unsigned char"; }
    template <> inline const std::string getTypeName<unsigned short> ()     {   return "unsigned short"; }
    template <> inline const std::string getTypeName<unsigned int> ()       {   return "unsigned int"; }
    template <> inline const std::string getTypeName<unsigned long long> () {   return "unsigned long"; }
    
    template<typename T> inline const char* getKernelExtension() { return ""; }

    template<> inline const char* getKernelExtension<double>() {
        return "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n";
    }

    // clang-format on

    template <typename T>
    class BitonicSort {
    private:
        cl::Platform platform_;
        cl::Context context_;
        cl::CommandQueue queue_;
        std::string kernel_;

        static cl::Platform selectGPUplatform ();
        static cl::Context getGPUcontext (cl_platform_id p);
        static cl::CommandQueue getCommandQueue (cl::Context &ctx);

        using sort_t = cl::KernelFunctor<cl::Buffer, cl_int, cl_int>;
        void GPUBitonicSort (cl::vector<T> &vec);

    public:
        BitonicSort (cl::vector<T> &vec = {})
            : platform_ (selectGPUplatform ()), context_ (getGPUcontext (platform_ ())), queue_ (getCommandQueue (context_))
        {
            kernel_ = "#define KERNEL_TYPE " + getTypeName<T> () +
                      R"(
                        __kernel void bitonicSort(__global KERNEL_TYPE *A, int subPower, int power)
                        {
                            int i = get_global_id(0);
                            i = (i / subPower) * (subPower << 1) + i % subPower;
                            int swapIndex = i | subPower;
                            KERNEL_TYPE first = A[i];
                            KERNEL_TYPE second = A[swapIndex];
                            if (!(i & power) == (first > second)) {
                                A[i] = second;
                                A[swapIndex] = first;
                            }
                        }
                        )";

            GPUBitonicSort (vec);
        }
    };

    template <typename T>
    cl::Platform BitonicSort<T>::selectGPUplatform ()
    {
        cl::vector<cl::Platform> platforms;

        cl::Platform::get (&platforms);

        for (auto curIt = platforms.rbegin (), endIt = platforms.rend (); curIt != endIt; ++curIt) {
            cl_uint numDevices = 0;
            ::clGetDeviceIDs ((*curIt) (), CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
            std::vector<cl::Device> devices;
            (*curIt).getDevices (CL_DEVICE_TYPE_ALL, &devices);

            if ((*curIt).getInfo<CL_PLATFORM_NAME> () == "NVIDIA CUDA")  // TODO only for me
                // if (numDevices > 0)
                return *curIt;
        }
        throw std::runtime_error ("No platform with GPU devices(((");
    }

    template <typename T>
    cl::Context BitonicSort<T>::getGPUcontext (cl_platform_id p)
    {
        cl_context_properties cprops[] = {
            CL_CONTEXT_PLATFORM,
            reinterpret_cast<cl_context_properties> (p),
            0};

        return cl::Context (CL_DEVICE_TYPE_GPU, cprops);
    }

    template <typename T>
    cl::CommandQueue BitonicSort<T>::getCommandQueue (cl::Context &ctx)
    {
        std::vector<cl::Device> devices = ctx.getInfo<CL_CONTEXT_DEVICES> ();
        std::cout << (devices[0]).getInfo<CL_DEVICE_NAME> () << std::endl;
        return cl::CommandQueue (ctx, devices[0]);
    }

    template <typename T>
    void BitonicSort<T>::GPUBitonicSort (cl::vector<T> &vec)
    {
        int vecSize = vec.size ();

        int size = 1;
        while (size < vecSize)
            size <<= 1;

        int bufSize = size * sizeof (T);

        T maxNum = std::numeric_limits<T>::max ();
        vec.insert (vec.end (), size - vecSize, maxNum);

        cl::Buffer clData (context_, CL_MEM_READ_WRITE, bufSize);
        cl::copy (queue_, vec.begin (), vec.begin () + size, clData);

        try {
            cl::Program program (context_, kernel_, true);
            sort_t kernel (program, "bitonicSort");

            cl::NDRange GlobalRange (size / 2);
            cl::EnqueueArgs Args (queue_, GlobalRange);
            cl::Event evt = kernel (Args, clData, 1, 2);

            for (int power = 4; power <= size; power <<= 1) {
                for (int subPower = power; subPower > 1; subPower >>= 1) {
                    cl::EnqueueArgs locArgs (queue_, evt, GlobalRange);

                    evt = kernel (locArgs, clData, subPower >> 1, power);
                }
            }
            evt.wait ();
        }
        catch (cl::BuildError &err) {
            std::cerr << "OCL BUILD ERROR: " << err.err () << ":" << err.what ()
                      << std::endl;
            std::cerr << "-- Log --\n";
            for (auto e : err.getBuildLog ())
                std::cerr << e.second;
            std::cerr << "-- End log --\n";
            return;
        }
        catch (cl::Error &e) {
            std::cerr << "OCL ERROR: " << e.err () << ":" << e.what ()
                      << std::endl;
            return;
        }

        cl::copy (queue_, clData, vec.begin (), vec.begin () + size);
        vec.erase (vec.begin () + vecSize, vec.end ());
    }
}  // namespace OpenCLApp