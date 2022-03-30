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
    
    template<typename T> inline const std::string getKernelExtension() { return ""; }

    template<> inline const std::string getKernelExtension<double>() {
        return "#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n";
    }

    // clang-format on

    class Platform {
        static inline cl::Platform platform_;
        static inline bool isCreated_ = false;

    public:
        static cl::Platform &selectGPUplatform ();
    };

    class Context {
        static inline cl::Context context_;
        static inline bool isCreated_ = false;

    public:
        static cl::Context &getGPUcontext (cl_platform_id p);
    };

    class Queue {
        static inline cl::CommandQueue queue_;
        static inline bool isCreated_ = false;

    public:
        static cl::CommandQueue &getCommandQueue (cl::Context &ctx);
    };

    template <typename T>
    class BitonicSort {
    private:
        cl::Platform &platform_;
        cl::Context &context_;
        cl::CommandQueue &queue_;
        std::string kernel_;

        using sort_t = cl::KernelFunctor<cl::Buffer, cl_int, cl_int>;
        void GPUBitonicSort (cl::vector<T> &vec);

    public:
        BitonicSort (cl::vector<T> &vec = {})
            : platform_ (Platform::selectGPUplatform ()), context_ (Context::getGPUcontext (platform_ ())), queue_ (Queue::getCommandQueue (context_))
        {
            kernel_ = getKernelExtension<T> () +
                      "#define KERNEL_TYPE " + getTypeName<T> () +
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

    cl::Platform &Platform::selectGPUplatform ()
    {
        if (isCreated_)
            return platform_;

        cl::vector<cl::Platform> platforms;

        cl::Platform::get (&platforms);

        for (auto curIt = platforms.begin (), endIt = platforms.end (); curIt != endIt; ++curIt) {
            cl_uint numDevices = 0;
            ::clGetDeviceIDs ((*curIt) (), CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
            std::vector<cl::Device> devices;
            (*curIt).getDevices (CL_DEVICE_TYPE_ALL, &devices);
            std::cout << (*curIt).getInfo<CL_PLATFORM_NAME> () << std::endl;
#ifdef NVIDIA
            if ((*curIt).getInfo<CL_PLATFORM_NAME> () == "NVIDIA CUDA") {
#else
            if (numDevices > 0) {
#endif
                platform_ = *curIt;
                isCreated_ = true;

                return platform_;
            }
        }
        throw std::runtime_error ("No platform with GPU devices(((");
    }

    cl::Context &Context::getGPUcontext (cl_platform_id p)
    {
        if (isCreated_)
            return context_;

        cl_context_properties cprops[] = {
            CL_CONTEXT_PLATFORM,
            reinterpret_cast<cl_context_properties> (p),
            0};

        context_ = cl::Context (CL_DEVICE_TYPE_GPU, cprops);
        isCreated_ = true;
        return context_;
    }

    cl::CommandQueue &Queue::getCommandQueue (cl::Context &ctx)
    {
        if (isCreated_)
            return queue_;

        std::vector<cl::Device> devices = ctx.getInfo<CL_CONTEXT_DEVICES> ();
#if 0  // for print device name
        std::cout << (devices[0]).getInfo<CL_DEVICE_NAME> () << std::endl;
#endif
        queue_ = cl::CommandQueue (ctx, devices[0]);
        isCreated_ = true;

        return queue_;
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