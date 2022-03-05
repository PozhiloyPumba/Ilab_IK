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
        cl::Context context_;
        cl::CommandQueue queue_;
        std::string kernel_;

        static cl::Platform selectGPUplatform ();
        static cl::Context getGPUcontext (cl_platform_id p);
        static cl::CommandQueue getCommandQueue (cl::Context &ctx);

        using sort_t = cl::KernelFunctor<cl::Buffer, cl_int, cl_int>;
        using sort_rec_t = cl::KernelFunctor<cl::Buffer, cl_int, cl_int, cl_int>;

        void BitonicSort (cl::vector<T> &vec, cl_int begin, cl_int end, cl_int direction);
        void BitonicMerge (cl::vector<T> &vec, cl_int begin, cl_int end, cl_int direction);

    public:
        App (const std::string &sourceFileName)
            : platform_ (selectGPUplatform ()), context_ (getGPUcontext (platform_())), queue_ (getCommandQueue (context_))
        {   
            std::fstream in (sourceFileName, std::ios_base::in);

            while (in) {
                std::string newLine;
                std::getline (in, newLine);
                kernel_ += newLine + "\n";
            }
        }

        void GPUBitonicSort (cl::vector<T> &vec);

        void BitonicSort (cl::vector<T> &vec);

        void recBitonicSort (cl::vector<T> &vec);

        void GPUrecBitonicSort (cl::vector<T> &vec);
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
            std::vector<cl::Device> devices;
            (*curIt).getDevices(CL_DEVICE_TYPE_ALL, &devices);
            // for (auto device: devices) {
            //     std::cout << device.getInfo<CL_DEVICE_NAME>();
            // }
            if ((*curIt).getInfo<CL_PLATFORM_NAME> () == "NVIDIA CUDA")  // TODO only for me
            // if (numDevices > 0)
                return *curIt;
        }
        throw std::runtime_error ("No platform with GPU devices(((");
    }

    template <typename T>
    cl::Context App<T>::getGPUcontext (cl_platform_id p)
    {
        cl_context_properties cprops[] = {
            CL_CONTEXT_PLATFORM,
            reinterpret_cast<cl_context_properties>(p),
            0
        };

        return cl::Context(CL_DEVICE_TYPE_GPU, cprops);
    }

    template <typename T>
    cl::CommandQueue App<T>::getCommandQueue (cl::Context &ctx)
    {
         std::vector<cl::Device> devices = ctx.getInfo<CL_CONTEXT_DEVICES>();
         std::cout << (devices[0]).getInfo<CL_DEVICE_NAME> () << std::endl;
         return cl::CommandQueue (ctx, devices[0]);
    }

    template <typename T>
    void App<T>::GPUBitonicSort (cl::vector<T> &vec)
    {
        int vecSize = vec.size();
        int size = std::pow (2, int (std::log2 (vec.size())) + 1);
        int bufSize = size * sizeof (cl_int);

        vec.insert(vec.end(), size - vecSize, INT32_MAX);

        cl::Buffer clData (context_, CL_MEM_READ_WRITE, bufSize);
        cl::copy(queue_, vec.begin(), vec.end(), clData);

        try {
            cl::Program program (context_, kernel_, true);
            sort_t kernel(program, "bitonicSort");
            
            cl::NDRange GlobalRange (size / 2, 1);
            cl::EnqueueArgs Args(queue_, GlobalRange);
            cl::Event evt = kernel(Args, clData, 2, 2);
            cl_ulong time = 0;

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
        catch (cl::Error &e) {
            std::cerr << "OCL ERROR: " << e.err() << ":" << e.what()
                        << std::endl;
            return;
        }

        cl::copy(queue_, clData, vec.begin(), vec.begin() + vecSize);
        vec.erase (vec.begin() + vecSize, vec.end());
    }

    template <typename T>
    void App<T>::BitonicSort (cl::vector<T> &vec)
    {
        int vecSize = vec.size();
        int newVecSize = std::pow (2, int(std::log2 (vec.size())) + 1);

        vec.insert(vec.end(), newVecSize - vecSize, INT32_MAX);

        size_t size = vec.size();

        std::cout << "size = " << size << std::endl;
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

    template <typename T>
    void App<T>::GPUrecBitonicSort (cl::vector<T> &vec)
    {
        int vecSize = vec.size();
        int size = std::pow (2, int(std::log2 (vec.size())) + 1);
        int bufSize = size * sizeof (cl_int);

        vec.insert(vec.end(), size - vecSize, INT32_MAX);
        cl::Buffer clData (context_, CL_MEM_READ_WRITE, bufSize);

        try {
            cl::copy(queue_, vec.begin(), vec.end(), clData);
            
            cl::Program program (context_, kernel_, true);
            sort_rec_t kernel(program, "bitonicSort");
            
            cl::NDRange GlobalRange (size, size / 2);
            cl::NDRange LocalRange (2);
            cl::EnqueueArgs Args(queue_, GlobalRange, LocalRange);
            cl::Event evt = kernel(Args, clData, 0, size, 1);

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
        catch (cl::Error &e) {
            std::cerr << "OCL ERROR: " << e.err() << ":" << e.what()
                        << std::endl;
            return;
        }

        cl::copy(queue_, clData, vec.begin(), vec.begin() + vecSize);

        vec.erase (vec.begin() + vecSize, vec.end());
    }

    template <typename T>
    void App<T>::recBitonicSort (cl::vector<T> &vec)
    {
        int vecSize = vec.size();
        int newVecSize = std::pow (2, int(std::log2 (vec.size())) + 1);

        vec.insert(vec.end(), newVecSize - vecSize, INT32_MAX);

        BitonicSort (vec, 0, newVecSize, 1);

        vec.erase (vec.begin() + vecSize, vec.end());
    }

    template <typename T>
    void App<T>::BitonicSort (cl::vector<T> &vec, cl_int begin, cl_int end, cl_int direction)
    {
        if (end - begin < 2)
            return;

        cl_int half = (begin + end) / 2;
        BitonicSort (vec, begin, half, 1);
        BitonicSort (vec, half, end, 0);

        BitonicMerge (vec, begin, end, direction);
    }

    template <typename T>
    void App<T>::BitonicMerge (cl::vector<T> &vec, cl_int begin, cl_int end, cl_int direction)
    {
        for (int subPower = end - begin; subPower > 1; subPower >>= 1) {
            int half = subPower >> 1;
            for (int indexPow = begin; indexPow < end; indexPow += subPower) {
                for (int i = 0; i < half; ++i) {
                    if (direction == (vec[indexPow + i] > vec[indexPow + half + i]))
                        std::swap (vec[indexPow + i], vec[indexPow + half + i]);
                }
            }
        }
    }
}  // namespace OpenCLApp