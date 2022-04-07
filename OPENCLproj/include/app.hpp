#ifndef APP_HPP__
#define APP_HPP__

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_MINIMUM_OPENCL_VERSION 200
#define CL_HPP_TARGET_OPENCL_VERSION 200
#endif

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_QUEUE_PROFILING_ENABLE

#include <CL/opencl.hpp>

namespace OpenCLApp {
#ifndef KERNEL_SOURCE
#define KERNEL_SOURCE "../sources/kernels/simple.cl"
#endif

#ifndef LOCAL_SIZE
#define LOCAL_SIZE 64
#endif

    class Timer {
        std::chrono::high_resolution_clock::time_point start_;
        std::chrono::high_resolution_clock::time_point end_;

    public:
        void timerInit () { start_ = std::chrono::high_resolution_clock::now (); }
        void timerEnd () { end_ = std::chrono::high_resolution_clock::now (); }
        long getTimeNs ()
        {
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::nanoseconds> (end_ - start_);
            return elapsed_seconds.count ();
        }
        long getTimeMs ()
        {
            return getTimeNs () / 1000000;
        }
    };

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

        std::vector<cl::Event> profiling_;

        using sort_t = cl::KernelFunctor<cl::Buffer, cl_int, cl_int>;
        using SVM_sort_t = cl::KernelFunctor<T *, cl_int, cl_int>;

    public:
        BitonicSort ()
            : platform_ (Platform::selectGPUplatform ()), context_ (Context::getGPUcontext (platform_ ())), queue_ (Queue::getCommandQueue (context_))
        {
            kernel_ = getKernelExtension<T> () +
                      "#define KERNEL_TYPE " + getTypeName<T> () + "\n";

            std::ifstream in;
            in.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            in.open (KERNEL_SOURCE);

            while (!in.eof ()) {
                std::string newLine;
                std::getline (in, newLine);

                kernel_ += newLine + "\n";
            }

            in.close ();
        }

        void GPUBitonicSort (cl::vector<T> &vec);
        long getGPULastSortTimeNs ();
        long getGPULastSortTimeMs () { return getGPULastSortTimeNs () / 1000000; }
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
#ifdef DEBUG  // for print device name
        std::cout << (devices[0]).getInfo<CL_DEVICE_NAME> () << std::endl;
#endif
        queue_ = cl::CommandQueue (ctx, devices[0], CL_QUEUE_PROFILING_ENABLE);
        isCreated_ = true;

        return queue_;
    }

    template <typename T>
    void BitonicSort<T>::GPUBitonicSort (cl::vector<T> &vec)
    {
        profiling_.clear ();
        int vecSize = vec.size ();

        int size = 1;
        while (size < vecSize)
            size <<= 1;

        T maxNum = std::numeric_limits<T>::max ();
        vec.insert (vec.end (), size - vecSize, maxNum);

#ifndef SHARED
        int bufSize = size * sizeof (T);
        cl::Buffer clData (context_, CL_MEM_READ_WRITE, bufSize);
        cl::copy (queue_, vec.begin (), vec.begin () + size, clData);
#else
        T *clData = reinterpret_cast<T *> (::clSVMAlloc (context_ (), cl::SVMTraitReadWrite<>::getSVMMemFlags (), size * sizeof (T), 0));
        queue_.enqueueMapSVM (clData, CL_TRUE, CL_MAP_WRITE, size * sizeof (T));

        std::copy (vec.begin (), vec.end (), clData);
        queue_.enqueueUnmapSVM (clData);
#endif

        try {
            cl::Program program (context_, kernel_, true);
#ifndef SHARED
            sort_t kernel (program, "bitonicSort");
#else
            SVM_sort_t kernel (program, "bitonicSort");
#endif

            cl::NDRange GlobalRange (size / 2);
            cl::EnqueueArgs Args (queue_, GlobalRange, LOCAL_SIZE);
            cl::Event evt = kernel (Args, clData, 1, 2);

            for (int power = 4; power <= size; power <<= 1) {
                for (int subPower = power; subPower > 1; subPower >>= 1) {
                    cl::EnqueueArgs locArgs (queue_, evt, GlobalRange, LOCAL_SIZE);

                    evt = kernel (locArgs, clData, subPower >> 1, power);
                    profiling_.push_back (evt);
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

#ifndef SHARED
        cl::copy (queue_, clData, vec.begin (), vec.begin () + size);
        vec.erase (vec.begin () + vecSize, vec.end ());
#else
        vec.clear ();
        queue_.enqueueMapSVM (clData, CL_TRUE, CL_MAP_READ, size * sizeof (T));
        std::copy (clData, clData + vecSize, std::inserter (vec, vec.begin ()));
        queue_.enqueueUnmapSVM (clData);
        ::clSVMFree (context_ (), clData);
#endif
    }

    template <typename T>
    long BitonicSort<T>::getGPULastSortTimeNs ()
    {
        long GPUtime = 0;
        for (auto evt : profiling_)
            GPUtime += evt.getProfilingInfo<CL_PROFILING_COMMAND_END> () - evt.getProfilingInfo<CL_PROFILING_COMMAND_START> ();

        return GPUtime;
    }
}  // namespace OpenCLApp

#endif