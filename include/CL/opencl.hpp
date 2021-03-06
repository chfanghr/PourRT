bplist00�_WebMainResource�	
^WebResourceURL_WebResourceFrameName_WebResourceData_WebResourceMIMEType_WebResourceTextEncodingName_Xhttps://raw.githubusercontent.com/KhronosGroup/OpenCL-CLHPP/master/include/CL/opencl.hppPO 6<html><head></head><body><pre style="word-wrap: break-word; white-space: pre-wrap;">//
// Copyright (c) 2008-2020 The Khronos Group Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

/*! \file
 *
 *   \brief C++ bindings for OpenCL 1.0, OpenCL 1.1, OpenCL 1.2,
 *       OpenCL 2.0, OpenCL 2.1, OpenCL 2.2, and OpenCL 3.0.
 *   \author Lee Howes and Bruce Merry
 *
 *   Derived from the OpenCL 1.x C++ bindings written by
 *   Benedict R. Gaster, Laurent Morichetti and Lee Howes
 *   With additions and fixes from:
 *       Brian Cole, March 3rd 2010 and April 2012
 *       Matt Gruenke, April 2012.
 *       Bruce Merry, February 2013.
 *       Tom Deakin and Simon McIntosh-Smith, July 2013
 *       James Price, 2015-
 *   \version 2.2.0
 *   \date 2019-09-18
 *
 *   Optional extension support
 *
 *         cl_ext_device_fission
 *         #define CL_HPP_USE_CL_DEVICE_FISSION
 *         cl_khr_d3d10_sharing
 *         #define CL_HPP_USE_DX_INTEROP
 *         cl_khr_sub_groups
 *         #define CL_HPP_USE_CL_SUB_GROUPS_KHR
 *         cl_khr_image2d_from_buffer
 *         #define CL_HPP_USE_CL_IMAGE2D_FROM_BUFFER_KHR
 *
 *   Doxygen documentation for this header is available here:
 *
 *       http://khronosgroup.github.io/OpenCL-CLHPP/
 *
 *   The latest version of this header can be found on the GitHub releases page:
 *
 *       https://github.com/KhronosGroup/OpenCL-CLHPP/releases
 *
 *   Bugs and patches can be submitted to the GitHub repository:
 *
 *       https://github.com/KhronosGroup/OpenCL-CLHPP
 */

/*! \mainpage
 * \section intro Introduction
 * For many large applications C++ is the language of choice and so it seems
 * reasonable to define C++ bindings for OpenCL.
 *
 * The interface is contained with a single C++ header file \em opencl.hpp and all
 * definitions are contained within the namespace \em cl. There is no additional
 * requirement to include \em cl.h and to use either the C++ or original C
 * bindings; it is enough to simply include \em opencl.hpp.
 *
 * The bindings themselves are lightweight and correspond closely to the
 * underlying C API. Using the C++ bindings introduces no additional execution
 * overhead.
 *
 * There are numerous compatibility, portability and memory management
 * fixes in the new header as well as additional OpenCL 2.0 features.
 * As a result the header is not directly backward compatible and for this
 * reason we release it as opencl.hpp rather than a new version of cl.hpp.
 * 
 *
 * \section compatibility Compatibility
 * Due to the evolution of the underlying OpenCL API the 2.0 C++ bindings
 * include an updated approach to defining supported feature versions
 * and the range of valid underlying OpenCL runtime versions supported.
 *
 * The combination of preprocessor macros CL_HPP_TARGET_OPENCL_VERSION and 
 * CL_HPP_MINIMUM_OPENCL_VERSION control this range. These are three digit
 * decimal values representing OpenCL runime versions. The default for 
 * the target is 200, representing OpenCL 2.0 and the minimum is also 
 * defined as 200. These settings would use 2.0 API calls only.
 * If backward compatibility with a 1.2 runtime is required, the minimum
 * version may be set to 120.
 *
 * Note that this is a compile-time setting, and so affects linking against
 * a particular SDK version rather than the versioning of the loaded runtime.
 *
 * The earlier versions of the header included basic vector and string 
 * classes based loosely on STL versions. These were difficult to 
 * maintain and very rarely used. For the 2.0 header we now assume
 * the presence of the standard library unless requested otherwise.
 * We use std::array, std::vector, std::shared_ptr and std::string 
 * throughout to safely manage memory and reduce the chance of a 
 * recurrance of earlier memory management bugs.
 *
 * These classes are used through typedefs in the cl namespace: 
 * cl::array, cl::vector, cl::pointer and cl::string.
 * In addition cl::allocate_pointer forwards to std::allocate_shared
 * by default.
 * In all cases these standard library classes can be replaced with 
 * custom interface-compatible versions using the CL_HPP_NO_STD_ARRAY, 
 * CL_HPP_NO_STD_VECTOR, CL_HPP_NO_STD_UNIQUE_PTR and 
 * CL_HPP_NO_STD_STRING macros.
 *
 * The OpenCL 1.x versions of the C++ bindings included a size_t wrapper
 * class to interface with kernel enqueue. This caused unpleasant interactions
 * with the standard size_t declaration and led to namespacing bugs.
 * In the 2.0 version we have replaced this with a std::array-based interface.
 * However, the old behaviour can be regained for backward compatibility
 * using the CL_HPP_ENABLE_SIZE_T_COMPATIBILITY macro.
 *
 * Finally, the program construction interface used a clumsy vector-of-pairs
 * design in the earlier versions. We have replaced that with a cleaner 
 * vector-of-vectors and vector-of-strings design. However, for backward 
 * compatibility old behaviour can be regained with the
 * CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY macro.
 * 
 * In OpenCL 2.0 OpenCL C is not entirely backward compatibility with 
 * earlier versions. As a result a flag must be passed to the OpenCL C
 * compiled to request OpenCL 2.0 compilation of kernels with 1.2 as
 * the default in the absence of the flag.
 * In some cases the C++ bindings automatically compile code for ease.
 * For those cases the compilation defaults to OpenCL C 2.0.
 * If this is not wanted, the CL_HPP_CL_1_2_DEFAULT_BUILD macro may
 * be specified to assume 1.2 compilation.
 * If more fine-grained decisions on a per-kernel bases are required
 * then explicit build operations that take the flag should be used.
 *
 *
 * \section parameterization Parameters
 * This header may be parameterized by a set of preprocessor macros.
 *
 * - CL_HPP_TARGET_OPENCL_VERSION
 *
 *   Defines the target OpenCL runtime version to build the header
 *   against. Defaults to 200, representing OpenCL 2.0.
 *
 * - CL_HPP_NO_STD_STRING
 *
 *   Do not use the standard library string class. cl::string is not
 *   defined and may be defined by the user before opencl.hpp is
 *   included.
 *
 * - CL_HPP_NO_STD_VECTOR
 *
 *   Do not use the standard library vector class. cl::vector is not
 *   defined and may be defined by the user before opencl.hpp is
 *   included.
 *
 * - CL_HPP_NO_STD_ARRAY
 *
 *   Do not use the standard library array class. cl::array is not
 *   defined and may be defined by the user before opencl.hpp is
 *   included.
 *
 * - CL_HPP_NO_STD_UNIQUE_PTR
 *
 *   Do not use the standard library unique_ptr class. cl::pointer and
 *   the cl::allocate_pointer functions are not defined and may be
 *   defined by the user before opencl.hpp is included.
 *
 * - CL_HPP_ENABLE_DEVICE_FISSION
 *
 *   Enables device fission for OpenCL 1.2 platforms.
 *
 * - CL_HPP_ENABLE_EXCEPTIONS
 *
 *   Enable exceptions for use in the C++ bindings header. This is the
 *   preferred error handling mechanism but is not required.
 *
 * - CL_HPP_ENABLE_SIZE_T_COMPATIBILITY
 *
 *   Backward compatibility option to support cl.hpp-style size_t
 *   class.  Replaces the updated std::array derived version and
 *   removal of size_t from the namespace. Note that in this case the
 *   new size_t class is placed in the cl::compatibility namespace and
 *   thus requires an additional using declaration for direct backward
 *   compatibility.
 *
 * - CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY
 *
 *   Enable older vector of pairs interface for construction of
 *   programs.
 *
 * - CL_HPP_CL_1_2_DEFAULT_BUILD
 *
 *   Default to OpenCL C 1.2 compilation rather than OpenCL C 2.0
 *   applies to use of cl::Program construction and other program
 *   build variants.
 *
 * - CL_HPP_USE_CL_SUB_GROUPS_KHR
 *
 *   Enable the cl_khr_subgroups extension.
 *
 * - CL_HPP_USE_IL_KHR
 *
 *   Enable the cl_khr_il_program extension.
 *
 *
 * \section example Example
 *
 * The following example shows a general use case for the C++
 * bindings, including support for the optional exception feature and
 * also the supplied vector and string classes, see following sections for
 * decriptions of these features.
 *
 * \code
    #define CL_HPP_ENABLE_EXCEPTIONS
    #define CL_HPP_TARGET_OPENCL_VERSION 200

    #include &lt;CL/opencl.hpp&gt;
    #include &lt;iostream&gt;
    #include &lt;vector&gt;
    #include &lt;memory&gt;
    #include &lt;algorithm&gt;

    const int numElements = 32;

    int main(void)
    {
        // Filter for a 2.0 platform and set it as the default
        std::vector&lt;cl::Platform&gt; platforms;
        cl::Platform::get(&amp;platforms);
        cl::Platform plat;
        for (auto &amp;p : platforms) {
            std::string platver = p.getInfo&lt;CL_PLATFORM_VERSION&gt;();
            if (platver.find("OpenCL 2.") != std::string::npos) {
                plat = p;
            }
        }
        if (plat() == 0)  {
            std::cout &lt;&lt; "No OpenCL 2.0 platform found.";
            return -1;
        }

        cl::Platform newP = cl::Platform::setDefault(plat);
        if (newP != plat) {
            std::cout &lt;&lt; "Error setting default platform.";
            return -1;
        }

        // Use C++11 raw string literals for kernel source code
        std::string kernel1{R"CLC(
            global int globalA;
            kernel void updateGlobal()
            {
              globalA = 75;
            }
        )CLC"};
        std::string kernel2{R"CLC(
            typedef struct { global int *bar; } Foo;
            kernel void vectorAdd(global const Foo* aNum, global const int *inputA, global const int *inputB,
                                  global int *output, int val, write_only pipe int outPipe, queue_t childQueue)
            {
              output[get_global_id(0)] = inputA[get_global_id(0)] + inputB[get_global_id(0)] + val + *(aNum-&gt;bar);
              write_pipe(outPipe, &amp;val);
              queue_t default_queue = get_default_queue();
              ndrange_t ndrange = ndrange_1D(get_global_size(0)/2, get_global_size(0)/2);

              // Have a child kernel write into third quarter of output
              enqueue_kernel(default_queue, CLK_ENQUEUE_FLAGS_WAIT_KERNEL, ndrange,
                ^{
                    output[get_global_size(0)*2 + get_global_id(0)] =
                      inputA[get_global_size(0)*2 + get_global_id(0)] + inputB[get_global_size(0)*2 + get_global_id(0)] + globalA;
                });

              // Have a child kernel write into last quarter of output
              enqueue_kernel(childQueue, CLK_ENQUEUE_FLAGS_WAIT_KERNEL, ndrange,
                ^{
                    output[get_global_size(0)*3 + get_global_id(0)] =
                      inputA[get_global_size(0)*3 + get_global_id(0)] + inputB[get_global_size(0)*3 + get_global_id(0)] + globalA + 2;
                });
            }
        )CLC"};

        // New simpler string interface style
        std::vector&lt;std::string&gt; programStrings {kernel1, kernel2};

        cl::Program vectorAddProgram(programStrings);
        try {
            vectorAddProgram.build("-cl-std=CL2.0");
        }
        catch (...) {
            // Print build info for all devices
            cl_int buildErr = CL_SUCCESS;
            auto buildInfo = vectorAddProgram.getBuildInfo&lt;CL_PROGRAM_BUILD_LOG&gt;(&amp;buildErr);
            for (auto &amp;pair : buildInfo) {
                std::cerr &lt;&lt; pair.second &lt;&lt; std::endl &lt;&lt; std::endl;
            }

            return 1;
        }

        typedef struct { int *bar; } Foo;

        // Get and run kernel that initializes the program-scope global
        // A test for kernels that take no arguments
        auto program2Kernel =
            cl::KernelFunctor&lt;&gt;(vectorAddProgram, "updateGlobal");
        program2Kernel(
            cl::EnqueueArgs(
            cl::NDRange(1)));

        //////////////////
        // SVM allocations

        auto anSVMInt = cl::allocate_svm&lt;int, cl::SVMTraitCoarse&lt;&gt;&gt;();
        *anSVMInt = 5;
        cl::SVMAllocator&lt;Foo, cl::SVMTraitCoarse&lt;cl::SVMTraitReadOnly&lt;&gt;&gt;&gt; svmAllocReadOnly;
        auto fooPointer = cl::allocate_pointer&lt;Foo&gt;(svmAllocReadOnly);
        fooPointer-&gt;bar = anSVMInt.get();
        cl::SVMAllocator&lt;int, cl::SVMTraitCoarse&lt;&gt;&gt; svmAlloc;
        std::vector&lt;int, cl::SVMAllocator&lt;int, cl::SVMTraitCoarse&lt;&gt;&gt;&gt; inputA(numElements, 1, svmAlloc);
        cl::coarse_svm_vector&lt;int&gt; inputB(numElements, 2, svmAlloc);

        //
        //////////////

        // Traditional cl_mem allocations
        std::vector&lt;int&gt; output(numElements, 0xdeadbeef);
        cl::Buffer outputBuffer(begin(output), end(output), false);
        cl::Pipe aPipe(sizeof(cl_int), numElements / 2);

        // Default command queue, also passed in as a parameter
        cl::DeviceCommandQueue defaultDeviceQueue = cl::DeviceCommandQueue::makeDefault(
            cl::Context::getDefault(), cl::Device::getDefault());

        auto vectorAddKernel =
            cl::KernelFunctor&lt;
                decltype(fooPointer)&amp;,
                int*,
                cl::coarse_svm_vector&lt;int&gt;&amp;,
                cl::Buffer,
                int,
                cl::Pipe&amp;,
                cl::DeviceCommandQueue
                &gt;(vectorAddProgram, "vectorAdd");

        // Ensure that the additional SVM pointer is available to the kernel
        // This one was not passed as a parameter
        vectorAddKernel.setSVMPointers(anSVMInt);

        // Hand control of coarse allocations to runtime
        cl::enqueueUnmapSVM(anSVMInt);
        cl::enqueueUnmapSVM(fooPointer);
        cl::unmapSVM(inputB);
        cl::unmapSVM(output2);

	    cl_int error;
	    vectorAddKernel(
            cl::EnqueueArgs(
                cl::NDRange(numElements/2),
                cl::NDRange(numElements/2)),
            fooPointer,
            inputA.data(),
            inputB,
            outputBuffer,
            3,
            aPipe,
            defaultDeviceQueue,
		    error
            );

        cl::copy(outputBuffer, begin(output), end(output));
        // Grab the SVM output vector using a map
        cl::mapSVM(output2);

        cl::Device d = cl::Device::getDefault();

        std::cout &lt;&lt; "Output:\n";
        for (int i = 1; i &lt; numElements; ++i) {
            std::cout &lt;&lt; "\t" &lt;&lt; output[i] &lt;&lt; "\n";
        }
        std::cout &lt;&lt; "\n\n";

        return 0;
    }
 *
 * \endcode
 *
 */
#ifndef CL_HPP_
#define CL_HPP_

/* Handle deprecated preprocessor definitions. In each case, we only check for
 * the old name if the new name is not defined, so that user code can define
 * both and hence work with either version of the bindings.
 */
#if !defined(CL_HPP_USE_DX_INTEROP) &amp;&amp; defined(USE_DX_INTEROP)
# pragma message("opencl.hpp: USE_DX_INTEROP is deprecated. Define CL_HPP_USE_DX_INTEROP instead")
# define CL_HPP_USE_DX_INTEROP
#endif
#if !defined(CL_HPP_USE_CL_DEVICE_FISSION) &amp;&amp; defined(USE_CL_DEVICE_FISSION)
# pragma message("opencl.hpp: USE_CL_DEVICE_FISSION is deprecated. Define CL_HPP_USE_CL_DEVICE_FISSION instead")
# define CL_HPP_USE_CL_DEVICE_FISSION
#endif
#if !defined(CL_HPP_ENABLE_EXCEPTIONS) &amp;&amp; defined(__CL_ENABLE_EXCEPTIONS)
# pragma message("opencl.hpp: __CL_ENABLE_EXCEPTIONS is deprecated. Define CL_HPP_ENABLE_EXCEPTIONS instead")
# define CL_HPP_ENABLE_EXCEPTIONS
#endif
#if !defined(CL_HPP_NO_STD_VECTOR) &amp;&amp; defined(__NO_STD_VECTOR)
# pragma message("opencl.hpp: __NO_STD_VECTOR is deprecated. Define CL_HPP_NO_STD_VECTOR instead")
# define CL_HPP_NO_STD_VECTOR
#endif
#if !defined(CL_HPP_NO_STD_STRING) &amp;&amp; defined(__NO_STD_STRING)
# pragma message("opencl.hpp: __NO_STD_STRING is deprecated. Define CL_HPP_NO_STD_STRING instead")
# define CL_HPP_NO_STD_STRING
#endif
#if defined(VECTOR_CLASS)
# pragma message("opencl.hpp: VECTOR_CLASS is deprecated. Alias cl::vector instead")
#endif
#if defined(STRING_CLASS)
# pragma message("opencl.hpp: STRING_CLASS is deprecated. Alias cl::string instead.")
#endif
#if !defined(CL_HPP_USER_OVERRIDE_ERROR_STRINGS) &amp;&amp; defined(__CL_USER_OVERRIDE_ERROR_STRINGS)
# pragma message("opencl.hpp: __CL_USER_OVERRIDE_ERROR_STRINGS is deprecated. Define CL_HPP_USER_OVERRIDE_ERROR_STRINGS instead")
# define CL_HPP_USER_OVERRIDE_ERROR_STRINGS
#endif

/* Warn about features that are no longer supported
 */
#if defined(__USE_DEV_VECTOR)
# pragma message("opencl.hpp: __USE_DEV_VECTOR is no longer supported. Expect compilation errors")
#endif
#if defined(__USE_DEV_STRING)
# pragma message("opencl.hpp: __USE_DEV_STRING is no longer supported. Expect compilation errors")
#endif

/* Detect which version to target */
#if !defined(CL_HPP_TARGET_OPENCL_VERSION)
# pragma message("opencl.hpp: CL_HPP_TARGET_OPENCL_VERSION is not defined. It will default to 300 (OpenCL 3.0)")
# define CL_HPP_TARGET_OPENCL_VERSION 300
#endif
#if CL_HPP_TARGET_OPENCL_VERSION != 100 &amp;&amp; \
    CL_HPP_TARGET_OPENCL_VERSION != 110 &amp;&amp; \
    CL_HPP_TARGET_OPENCL_VERSION != 120 &amp;&amp; \
    CL_HPP_TARGET_OPENCL_VERSION != 200 &amp;&amp; \
    CL_HPP_TARGET_OPENCL_VERSION != 210 &amp;&amp; \
    CL_HPP_TARGET_OPENCL_VERSION != 220 &amp;&amp; \
    CL_HPP_TARGET_OPENCL_VERSION != 300
# pragma message("opencl.hpp: CL_HPP_TARGET_OPENCL_VERSION is not a valid value (100, 110, 120, 200, 210, 220 or 300). It will be set to 300 (OpenCL 3.0).")
# undef CL_HPP_TARGET_OPENCL_VERSION
# define CL_HPP_TARGET_OPENCL_VERSION 300
#endif

/* Forward target OpenCL version to C headers if necessary */
#if defined(CL_TARGET_OPENCL_VERSION)
/* Warn if prior definition of CL_TARGET_OPENCL_VERSION is lower than
 * requested C++ bindings version */
#if CL_TARGET_OPENCL_VERSION &lt; CL_HPP_TARGET_OPENCL_VERSION
# pragma message("CL_TARGET_OPENCL_VERSION is already defined as is lower than CL_HPP_TARGET_OPENCL_VERSION")
#endif
#else
# define CL_TARGET_OPENCL_VERSION CL_HPP_TARGET_OPENCL_VERSION
#endif

#if !defined(CL_HPP_MINIMUM_OPENCL_VERSION)
# define CL_HPP_MINIMUM_OPENCL_VERSION 200
#endif
#if CL_HPP_MINIMUM_OPENCL_VERSION != 100 &amp;&amp; \
    CL_HPP_MINIMUM_OPENCL_VERSION != 110 &amp;&amp; \
    CL_HPP_MINIMUM_OPENCL_VERSION != 120 &amp;&amp; \
    CL_HPP_MINIMUM_OPENCL_VERSION != 200 &amp;&amp; \
    CL_HPP_MINIMUM_OPENCL_VERSION != 210 &amp;&amp; \
    CL_HPP_MINIMUM_OPENCL_VERSION != 220 &amp;&amp; \
    CL_HPP_MINIMUM_OPENCL_VERSION != 300
# pragma message("opencl.hpp: CL_HPP_MINIMUM_OPENCL_VERSION is not a valid value (100, 110, 120, 200, 210, 220 or 300). It will be set to 100")
# undef CL_HPP_MINIMUM_OPENCL_VERSION
# define CL_HPP_MINIMUM_OPENCL_VERSION 100
#endif
#if CL_HPP_MINIMUM_OPENCL_VERSION &gt; CL_HPP_TARGET_OPENCL_VERSION
# error "CL_HPP_MINIMUM_OPENCL_VERSION must not be greater than CL_HPP_TARGET_OPENCL_VERSION"
#endif

#if CL_HPP_MINIMUM_OPENCL_VERSION &lt;= 100 &amp;&amp; !defined(CL_USE_DEPRECATED_OPENCL_1_0_APIS)
# define CL_USE_DEPRECATED_OPENCL_1_0_APIS
#endif
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt;= 110 &amp;&amp; !defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
# define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#endif
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt;= 120 &amp;&amp; !defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
# define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#endif
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt;= 200 &amp;&amp; !defined(CL_USE_DEPRECATED_OPENCL_2_0_APIS)
# define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#endif
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt;= 210 &amp;&amp; !defined(CL_USE_DEPRECATED_OPENCL_2_1_APIS)
# define CL_USE_DEPRECATED_OPENCL_2_1_APIS
#endif
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt;= 220 &amp;&amp; !defined(CL_USE_DEPRECATED_OPENCL_2_2_APIS)
# define CL_USE_DEPRECATED_OPENCL_2_2_APIS
#endif

#ifdef _WIN32

#include &lt;malloc.h&gt;

#if defined(CL_HPP_USE_DX_INTEROP)
#include &lt;CL/cl_d3d10.h&gt;
#include &lt;CL/cl_dx9_media_sharing.h&gt;
#endif
#endif // _WIN32

#if defined(_MSC_VER)
#include &lt;intrin.h&gt;
#endif // _MSC_VER 
 
 // Check for a valid C++ version

// Need to do both tests here because for some reason __cplusplus is not 
// updated in visual studio
#if (!defined(_MSC_VER) &amp;&amp; __cplusplus &lt; 201103L) || (defined(_MSC_VER) &amp;&amp; _MSC_VER &lt; 1700)
#error Visual studio 2013 or another C++11-supporting compiler required
#endif

// 
#if defined(CL_HPP_USE_CL_DEVICE_FISSION) || defined(CL_HPP_USE_CL_SUB_GROUPS_KHR)
#include &lt;CL/cl_ext.h&gt;
#endif

#if defined(__APPLE__) || defined(__MACOSX)
#include &lt;OpenCL/opencl.h&gt;
#else
#include &lt;CL/opencl.h&gt;
#endif // !__APPLE__

#if (__cplusplus &gt;= 201103L || _MSVC_LANG &gt;= 201103L )
#define CL_HPP_NOEXCEPT_ noexcept
#else
#define CL_HPP_NOEXCEPT_
#endif

#if __cplusplus &gt;= 201703L
# define CL_HPP_DEFINE_STATIC_MEMBER_ inline
#elif defined(_MSC_VER)
# define CL_HPP_DEFINE_STATIC_MEMBER_ __declspec(selectany)
#elif defined(__MINGW32__)
# define CL_HPP_DEFINE_STATIC_MEMBER_ __attribute__((selectany))
#else
# define CL_HPP_DEFINE_STATIC_MEMBER_ __attribute__((weak))
#endif // !_MSC_VER

// Define deprecated prefixes and suffixes to ensure compilation
// in case they are not pre-defined
#if !defined(CL_EXT_PREFIX__VERSION_1_1_DEPRECATED)
#define CL_EXT_PREFIX__VERSION_1_1_DEPRECATED  
#endif // #if !defined(CL_EXT_PREFIX__VERSION_1_1_DEPRECATED)
#if !defined(CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED)
#define CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED
#endif // #if !defined(CL_EXT_PREFIX__VERSION_1_1_DEPRECATED)

#if !defined(CL_EXT_PREFIX__VERSION_1_2_DEPRECATED)
#define CL_EXT_PREFIX__VERSION_1_2_DEPRECATED  
#endif // #if !defined(CL_EXT_PREFIX__VERSION_1_2_DEPRECATED)
#if !defined(CL_EXT_SUFFIX__VERSION_1_2_DEPRECATED)
#define CL_EXT_SUFFIX__VERSION_1_2_DEPRECATED
#endif // #if !defined(CL_EXT_PREFIX__VERSION_1_2_DEPRECATED)

#if !defined(CL_CALLBACK)
#define CL_CALLBACK
#endif //CL_CALLBACK

#include &lt;utility&gt;
#include &lt;limits&gt;
#include &lt;iterator&gt;
#include &lt;mutex&gt;
#include &lt;cstring&gt;
#include &lt;functional&gt;


// Define a size_type to represent a correctly resolved size_t
#if defined(CL_HPP_ENABLE_SIZE_T_COMPATIBILITY)
namespace cl {
    using size_type = ::size_t;
} // namespace cl
#else // #if defined(CL_HPP_ENABLE_SIZE_T_COMPATIBILITY)
namespace cl {
    using size_type = size_t;
} // namespace cl
#endif // #if defined(CL_HPP_ENABLE_SIZE_T_COMPATIBILITY)


#if defined(CL_HPP_ENABLE_EXCEPTIONS)
#include &lt;exception&gt;
#endif // #if defined(CL_HPP_ENABLE_EXCEPTIONS)

#if !defined(CL_HPP_NO_STD_VECTOR)
#include &lt;vector&gt;
namespace cl {
    template &lt; class T, class Alloc = std::allocator&lt;T&gt; &gt;
    using vector = std::vector&lt;T, Alloc&gt;;
} // namespace cl
#endif // #if !defined(CL_HPP_NO_STD_VECTOR)

#if !defined(CL_HPP_NO_STD_STRING)
#include &lt;string&gt;
namespace cl {
    using string = std::string;
} // namespace cl
#endif // #if !defined(CL_HPP_NO_STD_STRING)

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200

#if !defined(CL_HPP_NO_STD_UNIQUE_PTR)
#include &lt;memory&gt;
namespace cl {
    // Replace unique_ptr and allocate_pointer for internal use
    // to allow user to replace them
    template&lt;class T, class D&gt;
    using pointer = std::unique_ptr&lt;T, D&gt;;
} // namespace cl
#endif 
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#if !defined(CL_HPP_NO_STD_ARRAY)
#include &lt;array&gt;
namespace cl {
    template &lt; class T, size_type N &gt;
    using array = std::array&lt;T, N&gt;;
} // namespace cl
#endif // #if !defined(CL_HPP_NO_STD_ARRAY)

// Define size_type appropriately to allow backward-compatibility
// use of the old size_t interface class
#if defined(CL_HPP_ENABLE_SIZE_T_COMPATIBILITY)
namespace cl {
    namespace compatibility {
        /*! \brief class used to interface between C++ and
        *  OpenCL C calls that require arrays of size_t values, whose
        *  size is known statically.
        */
        template &lt;int N&gt;
        class size_t
        {
        private:
            size_type data_[N];

        public:
            //! \brief Initialize size_t to all 0s
            size_t()
            {
                for (int i = 0; i &lt; N; ++i) {
                    data_[i] = 0;
                }
            }

            size_t(const array&lt;size_type, N&gt; &amp;rhs)
            {
                for (int i = 0; i &lt; N; ++i) {
                    data_[i] = rhs[i];
                }
            }

            size_type&amp; operator[](int index)
            {
                return data_[index];
            }

            const size_type&amp; operator[](int index) const
            {
                return data_[index];
            }

            //! \brief Conversion operator to T*.
            operator size_type* ()             { return data_; }

            //! \brief Conversion operator to const T*.
            operator const size_type* () const { return data_; }

            operator array&lt;size_type, N&gt;() const
            {
                array&lt;size_type, N&gt; ret;

                for (int i = 0; i &lt; N; ++i) {
                    ret[i] = data_[i];
                }
                return ret;
            }
        };
    } // namespace compatibility

    template&lt;int N&gt;
    using size_t = compatibility::size_t&lt;N&gt;;
} // namespace cl
#endif // #if defined(CL_HPP_ENABLE_SIZE_T_COMPATIBILITY)

// Helper alias to avoid confusing the macros
namespace cl {
    namespace detail {
        using size_t_array = array&lt;size_type, 3&gt;;
    } // namespace detail
} // namespace cl


/*! \namespace cl
 *
 * \brief The OpenCL C++ bindings are defined within this namespace.
 *
 */
namespace cl {
    class Memory;

#define CL_HPP_INIT_CL_EXT_FCN_PTR_(name) \
    if (!pfn_##name) {    \
    pfn_##name = (PFN_##name) \
    clGetExtensionFunctionAddress(#name); \
    if (!pfn_##name) {    \
    } \
    }

#define CL_HPP_INIT_CL_EXT_FCN_PTR_PLATFORM_(platform, name) \
    if (!pfn_##name) {    \
    pfn_##name = (PFN_##name) \
    clGetExtensionFunctionAddressForPlatform(platform, #name); \
    if (!pfn_##name) {    \
    } \
    }

    class Program;
    class Device;
    class Context;
    class CommandQueue;
    class DeviceCommandQueue;
    class Memory;
    class Buffer;
    class Pipe;

#if defined(CL_HPP_ENABLE_EXCEPTIONS)
    /*! \brief Exception class 
     * 
     *  This may be thrown by API functions when CL_HPP_ENABLE_EXCEPTIONS is defined.
     */
    class Error : public std::exception
    {
    private:
        cl_int err_;
        const char * errStr_;
    public:
        /*! \brief Create a new CL error exception for a given error code
         *  and corresponding message.
         * 
         *  \param err error code value.
         *
         *  \param errStr a descriptive string that must remain in scope until
         *                handling of the exception has concluded.  If set, it
         *                will be returned by what().
         */
        Error(cl_int err, const char * errStr = NULL) : err_(err), errStr_(errStr)
        {}

        ~Error() throw() {}

        /*! \brief Get error string associated with exception
         *
         * \return A memory pointer to the error message string.
         */
        virtual const char * what() const throw ()
        {
            if (errStr_ == NULL) {
                return "empty";
            }
            else {
                return errStr_;
            }
        }

        /*! \brief Get error code associated with exception
         *
         *  \return The error code.
         */
        cl_int err(void) const { return err_; }
    };
#define CL_HPP_ERR_STR_(x) #x
#else
#define CL_HPP_ERR_STR_(x) NULL
#endif // CL_HPP_ENABLE_EXCEPTIONS


namespace detail
{
#if defined(CL_HPP_ENABLE_EXCEPTIONS)
static inline cl_int errHandler (
    cl_int err,
    const char * errStr = NULL)
{
    if (err != CL_SUCCESS) {
        throw Error(err, errStr);
    }
    return err;
}
#else
static inline cl_int errHandler (cl_int err, const char * errStr = NULL)
{
    (void) errStr; // suppress unused variable warning
    return err;
}
#endif // CL_HPP_ENABLE_EXCEPTIONS
}



//! \cond DOXYGEN_DETAIL
#if !defined(CL_HPP_USER_OVERRIDE_ERROR_STRINGS)
#define __GET_DEVICE_INFO_ERR               CL_HPP_ERR_STR_(clGetDeviceInfo)
#define __GET_PLATFORM_INFO_ERR             CL_HPP_ERR_STR_(clGetPlatformInfo)
#define __GET_DEVICE_IDS_ERR                CL_HPP_ERR_STR_(clGetDeviceIDs)
#define __GET_PLATFORM_IDS_ERR              CL_HPP_ERR_STR_(clGetPlatformIDs)
#define __GET_CONTEXT_INFO_ERR              CL_HPP_ERR_STR_(clGetContextInfo)
#define __GET_EVENT_INFO_ERR                CL_HPP_ERR_STR_(clGetEventInfo)
#define __GET_EVENT_PROFILE_INFO_ERR        CL_HPP_ERR_STR_(clGetEventProfileInfo)
#define __GET_MEM_OBJECT_INFO_ERR           CL_HPP_ERR_STR_(clGetMemObjectInfo)
#define __GET_IMAGE_INFO_ERR                CL_HPP_ERR_STR_(clGetImageInfo)
#define __GET_SAMPLER_INFO_ERR              CL_HPP_ERR_STR_(clGetSamplerInfo)
#define __GET_KERNEL_INFO_ERR               CL_HPP_ERR_STR_(clGetKernelInfo)
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#define __GET_KERNEL_ARG_INFO_ERR           CL_HPP_ERR_STR_(clGetKernelArgInfo)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#define __GET_KERNEL_SUB_GROUP_INFO_ERR     CL_HPP_ERR_STR_(clGetKernelSubGroupInfo)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#define __GET_KERNEL_WORK_GROUP_INFO_ERR    CL_HPP_ERR_STR_(clGetKernelWorkGroupInfo)
#define __GET_PROGRAM_INFO_ERR              CL_HPP_ERR_STR_(clGetProgramInfo)
#define __GET_PROGRAM_BUILD_INFO_ERR        CL_HPP_ERR_STR_(clGetProgramBuildInfo)
#define __GET_COMMAND_QUEUE_INFO_ERR        CL_HPP_ERR_STR_(clGetCommandQueueInfo)

#define __CREATE_CONTEXT_ERR                CL_HPP_ERR_STR_(clCreateContext)
#define __CREATE_CONTEXT_FROM_TYPE_ERR      CL_HPP_ERR_STR_(clCreateContextFromType)
#define __GET_SUPPORTED_IMAGE_FORMATS_ERR   CL_HPP_ERR_STR_(clGetSupportedImageFormats)

#define __CREATE_BUFFER_ERR                 CL_HPP_ERR_STR_(clCreateBuffer)
#define __COPY_ERR                          CL_HPP_ERR_STR_(cl::copy)
#define __CREATE_SUBBUFFER_ERR              CL_HPP_ERR_STR_(clCreateSubBuffer)
#define __CREATE_GL_BUFFER_ERR              CL_HPP_ERR_STR_(clCreateFromGLBuffer)
#define __CREATE_GL_RENDER_BUFFER_ERR       CL_HPP_ERR_STR_(clCreateFromGLBuffer)
#define __GET_GL_OBJECT_INFO_ERR            CL_HPP_ERR_STR_(clGetGLObjectInfo)
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#define __CREATE_IMAGE_ERR                  CL_HPP_ERR_STR_(clCreateImage)
#define __CREATE_GL_TEXTURE_ERR             CL_HPP_ERR_STR_(clCreateFromGLTexture)
#define __IMAGE_DIMENSION_ERR               CL_HPP_ERR_STR_(Incorrect image dimensions)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#define __SET_MEM_OBJECT_DESTRUCTOR_CALLBACK_ERR CL_HPP_ERR_STR_(clSetMemObjectDestructorCallback)

#define __CREATE_USER_EVENT_ERR             CL_HPP_ERR_STR_(clCreateUserEvent)
#define __SET_USER_EVENT_STATUS_ERR         CL_HPP_ERR_STR_(clSetUserEventStatus)
#define __SET_EVENT_CALLBACK_ERR            CL_HPP_ERR_STR_(clSetEventCallback)
#define __WAIT_FOR_EVENTS_ERR               CL_HPP_ERR_STR_(clWaitForEvents)

#define __CREATE_KERNEL_ERR                 CL_HPP_ERR_STR_(clCreateKernel)
#define __SET_KERNEL_ARGS_ERR               CL_HPP_ERR_STR_(clSetKernelArg)
#define __CREATE_PROGRAM_WITH_SOURCE_ERR    CL_HPP_ERR_STR_(clCreateProgramWithSource)
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#define __CREATE_PROGRAM_WITH_IL_ERR        CL_HPP_ERR_STR_(clCreateProgramWithIL)
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#define __CREATE_PROGRAM_WITH_BINARY_ERR    CL_HPP_ERR_STR_(clCreateProgramWithBinary)
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
#define __CREATE_PROGRAM_WITH_IL_ERR        CL_HPP_ERR_STR_(clCreateProgramWithIL)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#define __CREATE_PROGRAM_WITH_BUILT_IN_KERNELS_ERR    CL_HPP_ERR_STR_(clCreateProgramWithBuiltInKernels)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#define __BUILD_PROGRAM_ERR                 CL_HPP_ERR_STR_(clBuildProgram)
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#define __COMPILE_PROGRAM_ERR               CL_HPP_ERR_STR_(clCompileProgram)
#define __LINK_PROGRAM_ERR                  CL_HPP_ERR_STR_(clLinkProgram)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#define __CREATE_KERNELS_IN_PROGRAM_ERR     CL_HPP_ERR_STR_(clCreateKernelsInProgram)

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#define __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR          CL_HPP_ERR_STR_(clCreateCommandQueueWithProperties)
#define __CREATE_SAMPLER_WITH_PROPERTIES_ERR                CL_HPP_ERR_STR_(clCreateSamplerWithProperties)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#define __SET_COMMAND_QUEUE_PROPERTY_ERR    CL_HPP_ERR_STR_(clSetCommandQueueProperty)
#define __ENQUEUE_READ_BUFFER_ERR           CL_HPP_ERR_STR_(clEnqueueReadBuffer)
#define __ENQUEUE_READ_BUFFER_RECT_ERR      CL_HPP_ERR_STR_(clEnqueueReadBufferRect)
#define __ENQUEUE_WRITE_BUFFER_ERR          CL_HPP_ERR_STR_(clEnqueueWriteBuffer)
#define __ENQUEUE_WRITE_BUFFER_RECT_ERR     CL_HPP_ERR_STR_(clEnqueueWriteBufferRect)
#define __ENQEUE_COPY_BUFFER_ERR            CL_HPP_ERR_STR_(clEnqueueCopyBuffer)
#define __ENQEUE_COPY_BUFFER_RECT_ERR       CL_HPP_ERR_STR_(clEnqueueCopyBufferRect)
#define __ENQUEUE_FILL_BUFFER_ERR           CL_HPP_ERR_STR_(clEnqueueFillBuffer)
#define __ENQUEUE_READ_IMAGE_ERR            CL_HPP_ERR_STR_(clEnqueueReadImage)
#define __ENQUEUE_WRITE_IMAGE_ERR           CL_HPP_ERR_STR_(clEnqueueWriteImage)
#define __ENQUEUE_COPY_IMAGE_ERR            CL_HPP_ERR_STR_(clEnqueueCopyImage)
#define __ENQUEUE_FILL_IMAGE_ERR            CL_HPP_ERR_STR_(clEnqueueFillImage)
#define __ENQUEUE_COPY_IMAGE_TO_BUFFER_ERR  CL_HPP_ERR_STR_(clEnqueueCopyImageToBuffer)
#define __ENQUEUE_COPY_BUFFER_TO_IMAGE_ERR  CL_HPP_ERR_STR_(clEnqueueCopyBufferToImage)
#define __ENQUEUE_MAP_BUFFER_ERR            CL_HPP_ERR_STR_(clEnqueueMapBuffer)
#define __ENQUEUE_MAP_IMAGE_ERR             CL_HPP_ERR_STR_(clEnqueueMapImage)
#define __ENQUEUE_UNMAP_MEM_OBJECT_ERR      CL_HPP_ERR_STR_(clEnqueueUnMapMemObject)
#define __ENQUEUE_NDRANGE_KERNEL_ERR        CL_HPP_ERR_STR_(clEnqueueNDRangeKernel)
#define __ENQUEUE_NATIVE_KERNEL             CL_HPP_ERR_STR_(clEnqueueNativeKernel)
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#define __ENQUEUE_MIGRATE_MEM_OBJECTS_ERR   CL_HPP_ERR_STR_(clEnqueueMigrateMemObjects)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
#define __ENQUEUE_MIGRATE_SVM_ERR   CL_HPP_ERR_STR_(clEnqueueSVMMigrateMem)
#define __SET_DEFAULT_DEVICE_COMMAND_QUEUE_ERR   CL_HPP_ERR_STR_(clSetDefaultDeviceCommandQueue)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 210


#define __ENQUEUE_ACQUIRE_GL_ERR            CL_HPP_ERR_STR_(clEnqueueAcquireGLObjects)
#define __ENQUEUE_RELEASE_GL_ERR            CL_HPP_ERR_STR_(clEnqueueReleaseGLObjects)

#define __CREATE_PIPE_ERR             CL_HPP_ERR_STR_(clCreatePipe)
#define __GET_PIPE_INFO_ERR           CL_HPP_ERR_STR_(clGetPipeInfo)


#define __RETAIN_ERR                        CL_HPP_ERR_STR_(Retain Object)
#define __RELEASE_ERR                       CL_HPP_ERR_STR_(Release Object)
#define __FLUSH_ERR                         CL_HPP_ERR_STR_(clFlush)
#define __FINISH_ERR                        CL_HPP_ERR_STR_(clFinish)
#define __VECTOR_CAPACITY_ERR               CL_HPP_ERR_STR_(Vector capacity error)

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
#define __GET_HOST_TIMER_ERR           CL_HPP_ERR_STR_(clGetHostTimer)
#define __GET_DEVICE_AND_HOST_TIMER_ERR           CL_HPP_ERR_STR_(clGetDeviceAndHostTimer)
#endif
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 220
#define __SET_PROGRAM_RELEASE_CALLBACK_ERR          CL_HPP_ERR_STR_(clSetProgramReleaseCallback)
#define __SET_PROGRAM_SPECIALIZATION_CONSTANT_ERR   CL_HPP_ERR_STR_(clSetProgramSpecializationConstant)
#endif


/**
 * CL 1.2 version that uses device fission.
 */
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#define __CREATE_SUB_DEVICES_ERR            CL_HPP_ERR_STR_(clCreateSubDevices)
#else
#define __CREATE_SUB_DEVICES_ERR            CL_HPP_ERR_STR_(clCreateSubDevicesEXT)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120

/**
 * Deprecated APIs for 1.2
 */
#if defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
#define __ENQUEUE_MARKER_ERR                CL_HPP_ERR_STR_(clEnqueueMarker)
#define __ENQUEUE_WAIT_FOR_EVENTS_ERR       CL_HPP_ERR_STR_(clEnqueueWaitForEvents)
#define __ENQUEUE_BARRIER_ERR               CL_HPP_ERR_STR_(clEnqueueBarrier)
#define __UNLOAD_COMPILER_ERR               CL_HPP_ERR_STR_(clUnloadCompiler)
#define __CREATE_GL_TEXTURE_2D_ERR          CL_HPP_ERR_STR_(clCreateFromGLTexture2D)
#define __CREATE_GL_TEXTURE_3D_ERR          CL_HPP_ERR_STR_(clCreateFromGLTexture3D)
#define __CREATE_IMAGE2D_ERR                CL_HPP_ERR_STR_(clCreateImage2D)
#define __CREATE_IMAGE3D_ERR                CL_HPP_ERR_STR_(clCreateImage3D)
#endif // #if defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)

/**
 * Deprecated APIs for 2.0
 */
#if defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
#define __CREATE_COMMAND_QUEUE_ERR          CL_HPP_ERR_STR_(clCreateCommandQueue)
#define __ENQUEUE_TASK_ERR                  CL_HPP_ERR_STR_(clEnqueueTask)
#define __CREATE_SAMPLER_ERR                CL_HPP_ERR_STR_(clCreateSampler)
#endif // #if defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)

/**
 * CL 1.2 marker and barrier commands
 */
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#define __ENQUEUE_MARKER_WAIT_LIST_ERR                CL_HPP_ERR_STR_(clEnqueueMarkerWithWaitList)
#define __ENQUEUE_BARRIER_WAIT_LIST_ERR               CL_HPP_ERR_STR_(clEnqueueBarrierWithWaitList)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
#define __CLONE_KERNEL_ERR     CL_HPP_ERR_STR_(clCloneKernel)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 210

#endif // CL_HPP_USER_OVERRIDE_ERROR_STRINGS
//! \endcond


namespace detail {

// Generic getInfoHelper. The final parameter is used to guide overload
// resolution: the actual parameter passed is an int, which makes this
// a worse conversion sequence than a specialization that declares the
// parameter as an int.
template&lt;typename Functor, typename T&gt;
inline cl_int getInfoHelper(Functor f, cl_uint name, T* param, long)
{
    return f(name, sizeof(T), param, NULL);
}

// Specialized for getInfo&lt;CL_PROGRAM_BINARIES&gt;
// Assumes that the output vector was correctly resized on the way in
template &lt;typename Func&gt;
inline cl_int getInfoHelper(Func f, cl_uint name, vector&lt;vector&lt;unsigned char&gt;&gt;* param, int)
{
    if (name != CL_PROGRAM_BINARIES) {
        return CL_INVALID_VALUE;
    }
    if (param) {
        // Create array of pointers, calculate total size and pass pointer array in
        size_type numBinaries = param-&gt;size();
        vector&lt;unsigned char*&gt; binariesPointers(numBinaries);

        for (size_type i = 0; i &lt; numBinaries; ++i)
        {
            binariesPointers[i] = (*param)[i].data();
        }

        cl_int err = f(name, numBinaries * sizeof(unsigned char*), binariesPointers.data(), NULL);

        if (err != CL_SUCCESS) {
            return err;
        }
    }


    return CL_SUCCESS;
}

// Specialized getInfoHelper for vector params
template &lt;typename Func, typename T&gt;
inline cl_int getInfoHelper(Func f, cl_uint name, vector&lt;T&gt;* param, long)
{
    size_type required;
    cl_int err = f(name, 0, NULL, &amp;required);
    if (err != CL_SUCCESS) {
        return err;
    }
    const size_type elements = required / sizeof(T);

    // Temporary to avoid changing param on an error
    vector&lt;T&gt; localData(elements);
    err = f(name, required, localData.data(), NULL);
    if (err != CL_SUCCESS) {
        return err;
    }
    if (param) {
        *param = std::move(localData);
    }

    return CL_SUCCESS;
}

/* Specialization for reference-counted types. This depends on the
 * existence of Wrapper&lt;T&gt;::cl_type, and none of the other types having the
 * cl_type member. Note that simplify specifying the parameter as Wrapper&lt;T&gt;
 * does not work, because when using a derived type (e.g. Context) the generic
 * template will provide a better match.
 */
template &lt;typename Func, typename T&gt;
inline cl_int getInfoHelper(
    Func f, cl_uint name, vector&lt;T&gt;* param, int, typename T::cl_type = 0)
{
    size_type required;
    cl_int err = f(name, 0, NULL, &amp;required);
    if (err != CL_SUCCESS) {
        return err;
    }

    const size_type elements = required / sizeof(typename T::cl_type);

    vector&lt;typename T::cl_type&gt; value(elements);
    err = f(name, required, value.data(), NULL);
    if (err != CL_SUCCESS) {
        return err;
    }

    if (param) {
        // Assign to convert CL type to T for each element
        param-&gt;resize(elements);

        // Assign to param, constructing with retain behaviour
        // to correctly capture each underlying CL object
        for (size_type i = 0; i &lt; elements; i++) {
            (*param)[i] = T(value[i], true);
        }
    }
    return CL_SUCCESS;
}

// Specialized GetInfoHelper for string params
template &lt;typename Func&gt;
inline cl_int getInfoHelper(Func f, cl_uint name, string* param, long)
{
    size_type required;
    cl_int err = f(name, 0, NULL, &amp;required);
    if (err != CL_SUCCESS) {
        return err;
    }

    // std::string has a constant data member
    // a char vector does not
    if (required &gt; 0) {
        vector&lt;char&gt; value(required);
        err = f(name, required, value.data(), NULL);
        if (err != CL_SUCCESS) {
            return err;
        }
        if (param) {
            param-&gt;assign(begin(value), prev(end(value)));
        }
    }
    else if (param) {
        param-&gt;assign("");
    }
    return CL_SUCCESS;
}

// Specialized GetInfoHelper for clsize_t params
template &lt;typename Func, size_type N&gt;
inline cl_int getInfoHelper(Func f, cl_uint name, array&lt;size_type, N&gt;* param, long)
{
    size_type required;
    cl_int err = f(name, 0, NULL, &amp;required);
    if (err != CL_SUCCESS) {
        return err;
    }

    size_type elements = required / sizeof(size_type);
    vector&lt;size_type&gt; value(elements, 0);

    err = f(name, required, value.data(), NULL);
    if (err != CL_SUCCESS) {
        return err;
    }
    
    // Bound the copy with N to prevent overruns
    // if passed N &gt; than the amount copied
    if (elements &gt; N) {
        elements = N;
    }
    for (size_type i = 0; i &lt; elements; ++i) {
        (*param)[i] = value[i];
    }

    return CL_SUCCESS;
}

template&lt;typename T&gt; struct ReferenceHandler;

/* Specialization for reference-counted types. This depends on the
 * existence of Wrapper&lt;T&gt;::cl_type, and none of the other types having the
 * cl_type member. Note that simplify specifying the parameter as Wrapper&lt;T&gt;
 * does not work, because when using a derived type (e.g. Context) the generic
 * template will provide a better match.
 */
template&lt;typename Func, typename T&gt;
inline cl_int getInfoHelper(Func f, cl_uint name, T* param, int, typename T::cl_type = 0)
{
    typename T::cl_type value;
    cl_int err = f(name, sizeof(value), &amp;value, NULL);
    if (err != CL_SUCCESS) {
        return err;
    }
    *param = value;
    if (value != NULL)
    {
        err = param-&gt;retain();
        if (err != CL_SUCCESS) {
            return err;
        }
    }
    return CL_SUCCESS;
}

#define CL_HPP_PARAM_NAME_INFO_1_0_(F) \
    F(cl_platform_info, CL_PLATFORM_PROFILE, string) \
    F(cl_platform_info, CL_PLATFORM_VERSION, string) \
    F(cl_platform_info, CL_PLATFORM_NAME, string) \
    F(cl_platform_info, CL_PLATFORM_VENDOR, string) \
    F(cl_platform_info, CL_PLATFORM_EXTENSIONS, string) \
    \
    F(cl_device_info, CL_DEVICE_TYPE, cl_device_type) \
    F(cl_device_info, CL_DEVICE_VENDOR_ID, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_COMPUTE_UNITS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_WORK_GROUP_SIZE, size_type) \
    F(cl_device_info, CL_DEVICE_MAX_WORK_ITEM_SIZES, cl::vector&lt;size_type&gt;) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_CLOCK_FREQUENCY, cl_uint) \
    F(cl_device_info, CL_DEVICE_ADDRESS_BITS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_READ_IMAGE_ARGS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_MEM_ALLOC_SIZE, cl_ulong) \
    F(cl_device_info, CL_DEVICE_IMAGE2D_MAX_WIDTH, size_type) \
    F(cl_device_info, CL_DEVICE_IMAGE2D_MAX_HEIGHT, size_type) \
    F(cl_device_info, CL_DEVICE_IMAGE3D_MAX_WIDTH, size_type) \
    F(cl_device_info, CL_DEVICE_IMAGE3D_MAX_HEIGHT, size_type) \
    F(cl_device_info, CL_DEVICE_IMAGE3D_MAX_DEPTH, size_type) \
    F(cl_device_info, CL_DEVICE_IMAGE_SUPPORT, cl_bool) \
    F(cl_device_info, CL_DEVICE_MAX_PARAMETER_SIZE, size_type) \
    F(cl_device_info, CL_DEVICE_MAX_SAMPLERS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MEM_BASE_ADDR_ALIGN, cl_uint) \
    F(cl_device_info, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, cl_uint) \
    F(cl_device_info, CL_DEVICE_SINGLE_FP_CONFIG, cl_device_fp_config) \
    F(cl_device_info, CL_DEVICE_DOUBLE_FP_CONFIG, cl_device_fp_config) \
    F(cl_device_info, CL_DEVICE_HALF_FP_CONFIG, cl_device_fp_config) \
    F(cl_device_info, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, cl_device_mem_cache_type) \
    F(cl_device_info, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, cl_uint)\
    F(cl_device_info, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, cl_ulong) \
    F(cl_device_info, CL_DEVICE_GLOBAL_MEM_SIZE, cl_ulong) \
    F(cl_device_info, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, cl_ulong) \
    F(cl_device_info, CL_DEVICE_MAX_CONSTANT_ARGS, cl_uint) \
    F(cl_device_info, CL_DEVICE_LOCAL_MEM_TYPE, cl_device_local_mem_type) \
    F(cl_device_info, CL_DEVICE_LOCAL_MEM_SIZE, cl_ulong) \
    F(cl_device_info, CL_DEVICE_ERROR_CORRECTION_SUPPORT, cl_bool) \
    F(cl_device_info, CL_DEVICE_PROFILING_TIMER_RESOLUTION, size_type) \
    F(cl_device_info, CL_DEVICE_ENDIAN_LITTLE, cl_bool) \
    F(cl_device_info, CL_DEVICE_AVAILABLE, cl_bool) \
    F(cl_device_info, CL_DEVICE_COMPILER_AVAILABLE, cl_bool) \
    F(cl_device_info, CL_DEVICE_EXECUTION_CAPABILITIES, cl_device_exec_capabilities) \
    F(cl_device_info, CL_DEVICE_PLATFORM, cl_platform_id) \
    F(cl_device_info, CL_DEVICE_NAME, string) \
    F(cl_device_info, CL_DEVICE_VENDOR, string) \
    F(cl_device_info, CL_DRIVER_VERSION, string) \
    F(cl_device_info, CL_DEVICE_PROFILE, string) \
    F(cl_device_info, CL_DEVICE_VERSION, string) \
    F(cl_device_info, CL_DEVICE_EXTENSIONS, string) \
    \
    F(cl_context_info, CL_CONTEXT_REFERENCE_COUNT, cl_uint) \
    F(cl_context_info, CL_CONTEXT_DEVICES, cl::vector&lt;Device&gt;) \
    F(cl_context_info, CL_CONTEXT_PROPERTIES, cl::vector&lt;cl_context_properties&gt;) \
    \
    F(cl_event_info, CL_EVENT_COMMAND_QUEUE, cl::CommandQueue) \
    F(cl_event_info, CL_EVENT_COMMAND_TYPE, cl_command_type) \
    F(cl_event_info, CL_EVENT_REFERENCE_COUNT, cl_uint) \
    F(cl_event_info, CL_EVENT_COMMAND_EXECUTION_STATUS, cl_int) \
    \
    F(cl_profiling_info, CL_PROFILING_COMMAND_QUEUED, cl_ulong) \
    F(cl_profiling_info, CL_PROFILING_COMMAND_SUBMIT, cl_ulong) \
    F(cl_profiling_info, CL_PROFILING_COMMAND_START, cl_ulong) \
    F(cl_profiling_info, CL_PROFILING_COMMAND_END, cl_ulong) \
    \
    F(cl_mem_info, CL_MEM_TYPE, cl_mem_object_type) \
    F(cl_mem_info, CL_MEM_FLAGS, cl_mem_flags) \
    F(cl_mem_info, CL_MEM_SIZE, size_type) \
    F(cl_mem_info, CL_MEM_HOST_PTR, void*) \
    F(cl_mem_info, CL_MEM_MAP_COUNT, cl_uint) \
    F(cl_mem_info, CL_MEM_REFERENCE_COUNT, cl_uint) \
    F(cl_mem_info, CL_MEM_CONTEXT, cl::Context) \
    \
    F(cl_image_info, CL_IMAGE_FORMAT, cl_image_format) \
    F(cl_image_info, CL_IMAGE_ELEMENT_SIZE, size_type) \
    F(cl_image_info, CL_IMAGE_ROW_PITCH, size_type) \
    F(cl_image_info, CL_IMAGE_SLICE_PITCH, size_type) \
    F(cl_image_info, CL_IMAGE_WIDTH, size_type) \
    F(cl_image_info, CL_IMAGE_HEIGHT, size_type) \
    F(cl_image_info, CL_IMAGE_DEPTH, size_type) \
    \
    F(cl_sampler_info, CL_SAMPLER_REFERENCE_COUNT, cl_uint) \
    F(cl_sampler_info, CL_SAMPLER_CONTEXT, cl::Context) \
    F(cl_sampler_info, CL_SAMPLER_NORMALIZED_COORDS, cl_bool) \
    F(cl_sampler_info, CL_SAMPLER_ADDRESSING_MODE, cl_addressing_mode) \
    F(cl_sampler_info, CL_SAMPLER_FILTER_MODE, cl_filter_mode) \
    \
    F(cl_program_info, CL_PROGRAM_REFERENCE_COUNT, cl_uint) \
    F(cl_program_info, CL_PROGRAM_CONTEXT, cl::Context) \
    F(cl_program_info, CL_PROGRAM_NUM_DEVICES, cl_uint) \
    F(cl_program_info, CL_PROGRAM_DEVICES, cl::vector&lt;Device&gt;) \
    F(cl_program_info, CL_PROGRAM_SOURCE, string) \
    F(cl_program_info, CL_PROGRAM_BINARY_SIZES, cl::vector&lt;size_type&gt;) \
    F(cl_program_info, CL_PROGRAM_BINARIES, cl::vector&lt;cl::vector&lt;unsigned char&gt;&gt;) \
    \
    F(cl_program_build_info, CL_PROGRAM_BUILD_STATUS, cl_build_status) \
    F(cl_program_build_info, CL_PROGRAM_BUILD_OPTIONS, string) \
    F(cl_program_build_info, CL_PROGRAM_BUILD_LOG, string) \
    \
    F(cl_kernel_info, CL_KERNEL_FUNCTION_NAME, string) \
    F(cl_kernel_info, CL_KERNEL_NUM_ARGS, cl_uint) \
    F(cl_kernel_info, CL_KERNEL_REFERENCE_COUNT, cl_uint) \
    F(cl_kernel_info, CL_KERNEL_CONTEXT, cl::Context) \
    F(cl_kernel_info, CL_KERNEL_PROGRAM, cl::Program) \
    \
    F(cl_kernel_work_group_info, CL_KERNEL_WORK_GROUP_SIZE, size_type) \
    F(cl_kernel_work_group_info, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, cl::detail::size_t_array) \
    F(cl_kernel_work_group_info, CL_KERNEL_LOCAL_MEM_SIZE, cl_ulong) \
    \
    F(cl_command_queue_info, CL_QUEUE_CONTEXT, cl::Context) \
    F(cl_command_queue_info, CL_QUEUE_DEVICE, cl::Device) \
    F(cl_command_queue_info, CL_QUEUE_REFERENCE_COUNT, cl_uint) \
    F(cl_command_queue_info, CL_QUEUE_PROPERTIES, cl_command_queue_properties)


#define CL_HPP_PARAM_NAME_INFO_1_1_(F) \
    F(cl_context_info, CL_CONTEXT_NUM_DEVICES, cl_uint)\
    F(cl_device_info, CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, cl_uint) \
    F(cl_device_info, CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF, cl_uint) \
    F(cl_device_info, CL_DEVICE_OPENCL_C_VERSION, string) \
    \
    F(cl_mem_info, CL_MEM_ASSOCIATED_MEMOBJECT, cl::Memory) \
    F(cl_mem_info, CL_MEM_OFFSET, size_type) \
    \
    F(cl_kernel_work_group_info, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, size_type) \
    F(cl_kernel_work_group_info, CL_KERNEL_PRIVATE_MEM_SIZE, cl_ulong) \
    \
    F(cl_event_info, CL_EVENT_CONTEXT, cl::Context)

#define CL_HPP_PARAM_NAME_INFO_1_2_(F) \
    F(cl_program_info, CL_PROGRAM_NUM_KERNELS, size_type) \
    F(cl_program_info, CL_PROGRAM_KERNEL_NAMES, string) \
    \
    F(cl_program_build_info, CL_PROGRAM_BINARY_TYPE, cl_program_binary_type) \
    \
    F(cl_kernel_info, CL_KERNEL_ATTRIBUTES, string) \
    \
    F(cl_kernel_arg_info, CL_KERNEL_ARG_ADDRESS_QUALIFIER, cl_kernel_arg_address_qualifier) \
    F(cl_kernel_arg_info, CL_KERNEL_ARG_ACCESS_QUALIFIER, cl_kernel_arg_access_qualifier) \
    F(cl_kernel_arg_info, CL_KERNEL_ARG_TYPE_NAME, string) \
    F(cl_kernel_arg_info, CL_KERNEL_ARG_NAME, string) \
    F(cl_kernel_arg_info, CL_KERNEL_ARG_TYPE_QUALIFIER, cl_kernel_arg_type_qualifier) \
    \
    F(cl_device_info, CL_DEVICE_PARENT_DEVICE, cl::Device) \
    F(cl_device_info, CL_DEVICE_PARTITION_MAX_SUB_DEVICES, cl_uint) \
    F(cl_device_info, CL_DEVICE_PARTITION_PROPERTIES, cl::vector&lt;cl_device_partition_property&gt;) \
    F(cl_device_info, CL_DEVICE_PARTITION_TYPE, cl::vector&lt;cl_device_partition_property&gt;)  \
    F(cl_device_info, CL_DEVICE_REFERENCE_COUNT, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_INTEROP_USER_SYNC, size_type) \
    F(cl_device_info, CL_DEVICE_PARTITION_AFFINITY_DOMAIN, cl_device_affinity_domain) \
    F(cl_device_info, CL_DEVICE_BUILT_IN_KERNELS, string) \
    \
    F(cl_image_info, CL_IMAGE_ARRAY_SIZE, size_type) \
    F(cl_image_info, CL_IMAGE_NUM_MIP_LEVELS, cl_uint) \
    F(cl_image_info, CL_IMAGE_NUM_SAMPLES, cl_uint)

#define CL_HPP_PARAM_NAME_INFO_2_0_(F) \
    F(cl_device_info, CL_DEVICE_QUEUE_ON_HOST_PROPERTIES, cl_command_queue_properties) \
    F(cl_device_info, CL_DEVICE_QUEUE_ON_DEVICE_PROPERTIES, cl_command_queue_properties) \
    F(cl_device_info, CL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE, cl_uint) \
    F(cl_device_info, CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_ON_DEVICE_QUEUES, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_ON_DEVICE_EVENTS, cl_uint) \
    F(cl_device_info, CL_DEVICE_MAX_PIPE_ARGS, cl_uint) \
    F(cl_device_info, CL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS, cl_uint) \
    F(cl_device_info, CL_DEVICE_PIPE_MAX_PACKET_SIZE, cl_uint) \
    F(cl_device_info, CL_DEVICE_SVM_CAPABILITIES, cl_device_svm_capabilities) \
    F(cl_device_info, CL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT, cl_uint) \
    F(cl_device_info, CL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT, cl_uint) \
    F(cl_profiling_info, CL_PROFILING_COMMAND_COMPLETE, cl_ulong) \
    F(cl_kernel_exec_info, CL_KERNEL_EXEC_INFO_SVM_FINE_GRAIN_SYSTEM, cl_bool) \
    F(cl_kernel_exec_info, CL_KERNEL_EXEC_INFO_SVM_PTRS, void**) \
    F(cl_command_queue_info, CL_QUEUE_SIZE, cl_uint) \
    F(cl_mem_info, CL_MEM_USES_SVM_POINTER, cl_bool) \
    F(cl_program_build_info, CL_PROGRAM_BUILD_GLOBAL_VARIABLE_TOTAL_SIZE, size_type) \
    F(cl_pipe_info, CL_PIPE_PACKET_SIZE, cl_uint) \
    F(cl_pipe_info, CL_PIPE_MAX_PACKETS, cl_uint)

#define CL_HPP_PARAM_NAME_INFO_SUBGROUP_KHR_(F) \
    F(cl_kernel_sub_group_info, CL_KERNEL_MAX_SUB_GROUP_SIZE_FOR_NDRANGE_KHR, size_type) \
    F(cl_kernel_sub_group_info, CL_KERNEL_SUB_GROUP_COUNT_FOR_NDRANGE_KHR, size_type)

#define CL_HPP_PARAM_NAME_INFO_IL_KHR_(F) \
    F(cl_device_info, CL_DEVICE_IL_VERSION_KHR, string) \
    F(cl_program_info, CL_PROGRAM_IL_KHR, cl::vector&lt;unsigned char&gt;)

#define CL_HPP_PARAM_NAME_INFO_2_1_(F) \
    F(cl_platform_info, CL_PLATFORM_HOST_TIMER_RESOLUTION, size_type) \
    F(cl_program_info, CL_PROGRAM_IL, cl::vector&lt;unsigned char&gt;) \
    F(cl_kernel_info, CL_KERNEL_MAX_NUM_SUB_GROUPS, size_type) \
    F(cl_kernel_info, CL_KERNEL_COMPILE_NUM_SUB_GROUPS, size_type) \
    F(cl_device_info, CL_DEVICE_MAX_NUM_SUB_GROUPS, cl_uint) \
    F(cl_device_info, CL_DEVICE_IL_VERSION, string) \
    F(cl_device_info, CL_DEVICE_SUB_GROUP_INDEPENDENT_FORWARD_PROGRESS, cl_bool) \
    F(cl_command_queue_info, CL_QUEUE_DEVICE_DEFAULT, cl::DeviceCommandQueue) \
    F(cl_kernel_sub_group_info, CL_KERNEL_MAX_SUB_GROUP_SIZE_FOR_NDRANGE, size_type) \
    F(cl_kernel_sub_group_info, CL_KERNEL_SUB_GROUP_COUNT_FOR_NDRANGE, size_type) \
    F(cl_kernel_sub_group_info, CL_KERNEL_LOCAL_SIZE_FOR_SUB_GROUP_COUNT, cl::detail::size_t_array)

#define CL_HPP_PARAM_NAME_INFO_2_2_(F) \
    F(cl_program_info, CL_PROGRAM_SCOPE_GLOBAL_CTORS_PRESENT, cl_bool) \
    F(cl_program_info, CL_PROGRAM_SCOPE_GLOBAL_DTORS_PRESENT, cl_bool)

#define CL_HPP_PARAM_NAME_DEVICE_FISSION_(F) \
    F(cl_device_info, CL_DEVICE_PARENT_DEVICE_EXT, cl_device_id) \
    F(cl_device_info, CL_DEVICE_PARTITION_TYPES_EXT, cl::vector&lt;cl_device_partition_property_ext&gt;) \
    F(cl_device_info, CL_DEVICE_AFFINITY_DOMAINS_EXT, cl::vector&lt;cl_device_partition_property_ext&gt;) \
    F(cl_device_info, CL_DEVICE_REFERENCE_COUNT_EXT , cl_uint) \
    F(cl_device_info, CL_DEVICE_PARTITION_STYLE_EXT, cl::vector&lt;cl_device_partition_property_ext&gt;)

#define CL_HPP_PARAM_NAME_CL_KHR_EXTENDED_VERSIONING_CL3_SHARED_(F) \
    F(cl_platform_info, CL_PLATFORM_NUMERIC_VERSION_KHR, cl_version_khr) \
    F(cl_platform_info, CL_PLATFORM_EXTENSIONS_WITH_VERSION_KHR, cl::vector&lt;cl_name_version_khr&gt;) \
    \
    F(cl_device_info, CL_DEVICE_NUMERIC_VERSION_KHR, cl_version_khr) \
    F(cl_device_info, CL_DEVICE_EXTENSIONS_WITH_VERSION_KHR, cl::vector&lt;cl_name_version_khr&gt;) \
    F(cl_device_info, CL_DEVICE_ILS_WITH_VERSION_KHR, cl::vector&lt;cl_name_version_khr&gt;) \
    F(cl_device_info, CL_DEVICE_BUILT_IN_KERNELS_WITH_VERSION_KHR, cl::vector&lt;cl_name_version_khr&gt;)

#define CL_HPP_PARAM_NAME_CL_KHR_EXTENDED_VERSIONING_KHRONLY_(F) \
    F(cl_device_info, CL_DEVICE_OPENCL_C_NUMERIC_VERSION_KHR, cl_version_khr)

#define CL_HPP_PARAM_NAME_INFO_3_0_(F) \
    F(cl_platform_info, CL_PLATFORM_NUMERIC_VERSION, cl_version) \
    F(cl_platform_info, CL_PLATFORM_EXTENSIONS_WITH_VERSION, cl::vector&lt;cl_name_version&gt;) \
    \
    F(cl_device_info, CL_DEVICE_NUMERIC_VERSION, cl_version) \
    F(cl_device_info, CL_DEVICE_EXTENSIONS_WITH_VERSION, cl::vector&lt;cl_name_version&gt;) \
    F(cl_device_info, CL_DEVICE_ILS_WITH_VERSION, cl::vector&lt;cl_name_version&gt;) \
    F(cl_device_info, CL_DEVICE_BUILT_IN_KERNELS_WITH_VERSION, cl::vector&lt;cl_name_version&gt;) \
    F(cl_device_info, CL_DEVICE_ATOMIC_MEMORY_CAPABILITIES, cl_device_atomic_capabilities) \
    F(cl_device_info, CL_DEVICE_ATOMIC_FENCE_CAPABILITIES, cl_device_atomic_capabilities) \
    F(cl_device_info, CL_DEVICE_NON_UNIFORM_WORK_GROUP_SUPPORT, cl_bool) \
    F(cl_device_info, CL_DEVICE_OPENCL_C_ALL_VERSIONS, cl::vector&lt;cl_name_version&gt;) \
    F(cl_device_info, CL_DEVICE_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, size_type) \
    F(cl_device_info, CL_DEVICE_WORK_GROUP_COLLECTIVE_FUNCTIONS_SUPPORT, cl_bool) \
    F(cl_device_info, CL_DEVICE_GENERIC_ADDRESS_SPACE_SUPPORT, cl_bool) \
    F(cl_device_info, CL_DEVICE_OPENCL_C_FEATURES, cl::vector&lt;cl_name_version&gt;) \
    F(cl_device_info, CL_DEVICE_DEVICE_ENQUEUE_CAPABILITIES, cl_device_device_enqueue_capabilities) \
    F(cl_device_info, CL_DEVICE_PIPE_SUPPORT, cl_bool) \
    F(cl_device_info, CL_DEVICE_LATEST_CONFORMANCE_VERSION_PASSED, string) \
    \
    F(cl_command_queue_info, CL_QUEUE_PROPERTIES_ARRAY, cl::vector&lt;cl_queue_properties&gt;) \
    F(cl_mem_info, CL_MEM_PROPERTIES, cl::vector&lt;cl_mem_properties&gt;) \
    F(cl_pipe_info, CL_PIPE_PROPERTIES, cl::vector&lt;cl_pipe_properties&gt;) \
    F(cl_sampler_info, CL_SAMPLER_PROPERTIES, cl::vector&lt;cl_sampler_properties&gt;)

template &lt;typename enum_type, cl_int Name&gt;
struct param_traits {};

#define CL_HPP_DECLARE_PARAM_TRAITS_(token, param_name, T) \
struct token;                                        \
template&lt;&gt;                                           \
struct param_traits&lt;detail:: token,param_name&gt;       \
{                                                    \
    enum { value = param_name };                     \
    typedef T param_type;                            \
};

CL_HPP_PARAM_NAME_INFO_1_0_(CL_HPP_DECLARE_PARAM_TRAITS_)
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
CL_HPP_PARAM_NAME_INFO_1_1_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
CL_HPP_PARAM_NAME_INFO_1_2_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
CL_HPP_PARAM_NAME_INFO_2_0_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
CL_HPP_PARAM_NAME_INFO_2_1_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 220
CL_HPP_PARAM_NAME_INFO_2_2_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 220
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 300
CL_HPP_PARAM_NAME_INFO_3_0_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 300

#if defined(CL_HPP_USE_CL_SUB_GROUPS_KHR) &amp;&amp; CL_HPP_TARGET_OPENCL_VERSION &lt; 210
CL_HPP_PARAM_NAME_INFO_SUBGROUP_KHR_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // #if defined(CL_HPP_USE_CL_SUB_GROUPS_KHR) &amp;&amp; CL_HPP_TARGET_OPENCL_VERSION &lt; 210

#if defined(CL_HPP_USE_IL_KHR)
CL_HPP_PARAM_NAME_INFO_IL_KHR_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // #if defined(CL_HPP_USE_IL_KHR)


// Flags deprecated in OpenCL 2.0
#define CL_HPP_PARAM_NAME_INFO_1_0_DEPRECATED_IN_2_0_(F) \
    F(cl_device_info, CL_DEVICE_QUEUE_PROPERTIES, cl_command_queue_properties)

#define CL_HPP_PARAM_NAME_INFO_1_1_DEPRECATED_IN_2_0_(F) \
    F(cl_device_info, CL_DEVICE_HOST_UNIFIED_MEMORY, cl_bool)

#define CL_HPP_PARAM_NAME_INFO_1_2_DEPRECATED_IN_2_0_(F) \
    F(cl_image_info, CL_IMAGE_BUFFER, cl::Buffer)

// Include deprecated query flags based on versions
// Only include deprecated 1.0 flags if 2.0 not active as there is an enum clash
#if CL_HPP_TARGET_OPENCL_VERSION &gt; 100 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200 &amp;&amp; CL_HPP_TARGET_OPENCL_VERSION &lt; 200
CL_HPP_PARAM_NAME_INFO_1_0_DEPRECATED_IN_2_0_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 110
#if CL_HPP_TARGET_OPENCL_VERSION &gt; 110 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
CL_HPP_PARAM_NAME_INFO_1_1_DEPRECATED_IN_2_0_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120
#if CL_HPP_TARGET_OPENCL_VERSION &gt; 120 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
CL_HPP_PARAM_NAME_INFO_1_2_DEPRECATED_IN_2_0_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200

#if defined(CL_HPP_USE_CL_DEVICE_FISSION)
CL_HPP_PARAM_NAME_DEVICE_FISSION_(CL_HPP_DECLARE_PARAM_TRAITS_);
#endif // CL_HPP_USE_CL_DEVICE_FISSION

#if defined(cl_khr_extended_versioning)
#if CL_HPP_TARGET_OPENCL_VERSION &lt; 300
CL_HPP_PARAM_NAME_CL_KHR_EXTENDED_VERSIONING_CL3_SHARED_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // CL_HPP_TARGET_OPENCL_VERSION &lt; 300
CL_HPP_PARAM_NAME_CL_KHR_EXTENDED_VERSIONING_KHRONLY_(CL_HPP_DECLARE_PARAM_TRAITS_)
#endif // cl_khr_extended_versioning

#ifdef CL_PLATFORM_ICD_SUFFIX_KHR
CL_HPP_DECLARE_PARAM_TRAITS_(cl_platform_info, CL_PLATFORM_ICD_SUFFIX_KHR, string)
#endif

#ifdef CL_DEVICE_PROFILING_TIMER_OFFSET_AMD
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_PROFILING_TIMER_OFFSET_AMD, cl_ulong)
#endif

#ifdef CL_DEVICE_GLOBAL_FREE_MEMORY_AMD
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_GLOBAL_FREE_MEMORY_AMD, vector&lt;size_type&gt;)
#endif
#ifdef CL_DEVICE_SIMD_PER_COMPUTE_UNIT_AMD
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_SIMD_PER_COMPUTE_UNIT_AMD, cl_uint)
#endif
#ifdef CL_DEVICE_SIMD_WIDTH_AMD
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_SIMD_WIDTH_AMD, cl_uint)
#endif
#ifdef CL_DEVICE_SIMD_INSTRUCTION_WIDTH_AMD
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_SIMD_INSTRUCTION_WIDTH_AMD, cl_uint)
#endif
#ifdef CL_DEVICE_WAVEFRONT_WIDTH_AMD
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_WAVEFRONT_WIDTH_AMD, cl_uint)
#endif
#ifdef CL_DEVICE_GLOBAL_MEM_CHANNELS_AMD
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_GLOBAL_MEM_CHANNELS_AMD, cl_uint)
#endif
#ifdef CL_DEVICE_GLOBAL_MEM_CHANNEL_BANKS_AMD
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_GLOBAL_MEM_CHANNEL_BANKS_AMD, cl_uint)
#endif
#ifdef CL_DEVICE_GLOBAL_MEM_CHANNEL_BANK_WIDTH_AMD
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_GLOBAL_MEM_CHANNEL_BANK_WIDTH_AMD, cl_uint)
#endif
#ifdef CL_DEVICE_LOCAL_MEM_SIZE_PER_COMPUTE_UNIT_AMD
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_LOCAL_MEM_SIZE_PER_COMPUTE_UNIT_AMD, cl_uint)
#endif
#ifdef CL_DEVICE_LOCAL_MEM_BANKS_AMD
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_LOCAL_MEM_BANKS_AMD, cl_uint)
#endif

#ifdef CL_DEVICE_COMPUTE_UNITS_BITFIELD_ARM
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_COMPUTE_UNITS_BITFIELD_ARM, cl_ulong)
#endif
#ifdef CL_DEVICE_JOB_SLOTS_ARM
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_JOB_SLOTS_ARM, cl_uint)
#endif
#ifdef CL_DEVICE_SCHEDULING_CONTROLS_CAPABILITIES_ARM
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_SCHEDULING_CONTROLS_CAPABILITIES_ARM, cl_bitfield)
#endif
#ifdef CL_KERNEL_EXEC_INFO_WORKGROUP_BATCH_SIZE_ARM
CL_HPP_DECLARE_PARAM_TRAITS_(cl_kernel_exec_info, CL_KERNEL_EXEC_INFO_WORKGROUP_BATCH_SIZE_ARM, cl_uint)
#endif
#ifdef CL_KERNEL_EXEC_INFO_WORKGROUP_BATCH_SIZE_MODIFIER_ARM
CL_HPP_DECLARE_PARAM_TRAITS_(cl_kernel_exec_info, CL_KERNEL_EXEC_INFO_WORKGROUP_BATCH_SIZE_MODIFIER_ARM, cl_int)
#endif

#ifdef CL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV, cl_uint)
#endif
#ifdef CL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV, cl_uint)
#endif
#ifdef CL_DEVICE_REGISTERS_PER_BLOCK_NV
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_REGISTERS_PER_BLOCK_NV, cl_uint)
#endif
#ifdef CL_DEVICE_WARP_SIZE_NV
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_WARP_SIZE_NV, cl_uint)
#endif
#ifdef CL_DEVICE_GPU_OVERLAP_NV
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_GPU_OVERLAP_NV, cl_bool)
#endif
#ifdef CL_DEVICE_KERNEL_EXEC_TIMEOUT_NV
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_KERNEL_EXEC_TIMEOUT_NV, cl_bool)
#endif
#ifdef CL_DEVICE_INTEGRATED_MEMORY_NV
CL_HPP_DECLARE_PARAM_TRAITS_(cl_device_info, CL_DEVICE_INTEGRATED_MEMORY_NV, cl_bool)
#endif

// Convenience functions

template &lt;typename Func, typename T&gt;
inline cl_int
getInfo(Func f, cl_uint name, T* param)
{
    return getInfoHelper(f, name, param, 0);
}

template &lt;typename Func, typename Arg0&gt;
struct GetInfoFunctor0
{
    Func f_; const Arg0&amp; arg0_;
    cl_int operator ()(
        cl_uint param, size_type size, void* value, size_type* size_ret)
    { return f_(arg0_, param, size, value, size_ret); }
};

template &lt;typename Func, typename Arg0, typename Arg1&gt;
struct GetInfoFunctor1
{
    Func f_; const Arg0&amp; arg0_; const Arg1&amp; arg1_;
    cl_int operator ()(
        cl_uint param, size_type size, void* value, size_type* size_ret)
    { return f_(arg0_, arg1_, param, size, value, size_ret); }
};

template &lt;typename Func, typename Arg0, typename T&gt;
inline cl_int
getInfo(Func f, const Arg0&amp; arg0, cl_uint name, T* param)
{
    GetInfoFunctor0&lt;Func, Arg0&gt; f0 = { f, arg0 };
    return getInfoHelper(f0, name, param, 0);
}

template &lt;typename Func, typename Arg0, typename Arg1, typename T&gt;
inline cl_int
getInfo(Func f, const Arg0&amp; arg0, const Arg1&amp; arg1, cl_uint name, T* param)
{
    GetInfoFunctor1&lt;Func, Arg0, Arg1&gt; f0 = { f, arg0, arg1 };
    return getInfoHelper(f0, name, param, 0);
}


template&lt;typename T&gt;
struct ReferenceHandler
{ };

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
/**
 * OpenCL 1.2 devices do have retain/release.
 */
template &lt;&gt;
struct ReferenceHandler&lt;cl_device_id&gt;
{
    /**
     * Retain the device.
     * \param device A valid device created using createSubDevices
     * \return 
     *   CL_SUCCESS if the function executed successfully.
     *   CL_INVALID_DEVICE if device was not a valid subdevice
     *   CL_OUT_OF_RESOURCES
     *   CL_OUT_OF_HOST_MEMORY
     */
    static cl_int retain(cl_device_id device)
    { return ::clRetainDevice(device); }
    /**
     * Retain the device.
     * \param device A valid device created using createSubDevices
     * \return 
     *   CL_SUCCESS if the function executed successfully.
     *   CL_INVALID_DEVICE if device was not a valid subdevice
     *   CL_OUT_OF_RESOURCES
     *   CL_OUT_OF_HOST_MEMORY
     */
    static cl_int release(cl_device_id device)
    { return ::clReleaseDevice(device); }
};
#else // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
/**
 * OpenCL 1.1 devices do not have retain/release.
 */
template &lt;&gt;
struct ReferenceHandler&lt;cl_device_id&gt;
{
    // cl_device_id does not have retain().
    static cl_int retain(cl_device_id)
    { return CL_SUCCESS; }
    // cl_device_id does not have release().
    static cl_int release(cl_device_id)
    { return CL_SUCCESS; }
};
#endif // ! (CL_HPP_TARGET_OPENCL_VERSION &gt;= 120)

template &lt;&gt;
struct ReferenceHandler&lt;cl_platform_id&gt;
{
    // cl_platform_id does not have retain().
    static cl_int retain(cl_platform_id)
    { return CL_SUCCESS; }
    // cl_platform_id does not have release().
    static cl_int release(cl_platform_id)
    { return CL_SUCCESS; }
};

template &lt;&gt;
struct ReferenceHandler&lt;cl_context&gt;
{
    static cl_int retain(cl_context context)
    { return ::clRetainContext(context); }
    static cl_int release(cl_context context)
    { return ::clReleaseContext(context); }
};

template &lt;&gt;
struct ReferenceHandler&lt;cl_command_queue&gt;
{
    static cl_int retain(cl_command_queue queue)
    { return ::clRetainCommandQueue(queue); }
    static cl_int release(cl_command_queue queue)
    { return ::clReleaseCommandQueue(queue); }
};

template &lt;&gt;
struct ReferenceHandler&lt;cl_mem&gt;
{
    static cl_int retain(cl_mem memory)
    { return ::clRetainMemObject(memory); }
    static cl_int release(cl_mem memory)
    { return ::clReleaseMemObject(memory); }
};

template &lt;&gt;
struct ReferenceHandler&lt;cl_sampler&gt;
{
    static cl_int retain(cl_sampler sampler)
    { return ::clRetainSampler(sampler); }
    static cl_int release(cl_sampler sampler)
    { return ::clReleaseSampler(sampler); }
};

template &lt;&gt;
struct ReferenceHandler&lt;cl_program&gt;
{
    static cl_int retain(cl_program program)
    { return ::clRetainProgram(program); }
    static cl_int release(cl_program program)
    { return ::clReleaseProgram(program); }
};

template &lt;&gt;
struct ReferenceHandler&lt;cl_kernel&gt;
{
    static cl_int retain(cl_kernel kernel)
    { return ::clRetainKernel(kernel); }
    static cl_int release(cl_kernel kernel)
    { return ::clReleaseKernel(kernel); }
};

template &lt;&gt;
struct ReferenceHandler&lt;cl_event&gt;
{
    static cl_int retain(cl_event event)
    { return ::clRetainEvent(event); }
    static cl_int release(cl_event event)
    { return ::clReleaseEvent(event); }
};


#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120
// Extracts version number with major in the upper 16 bits, minor in the lower 16
static cl_uint getVersion(const vector&lt;char&gt; &amp;versionInfo)
{
    int highVersion = 0;
    int lowVersion = 0;
    int index = 7;
    while(versionInfo[index] != '.' ) {
        highVersion *= 10;
        highVersion += versionInfo[index]-'0';
        ++index;
    }
    ++index;
    while(versionInfo[index] != ' ' &amp;&amp;  versionInfo[index] != '\0') {
        lowVersion *= 10;
        lowVersion += versionInfo[index]-'0';
        ++index;
    }
    return (highVersion &lt;&lt; 16) | lowVersion;
}

static cl_uint getPlatformVersion(cl_platform_id platform)
{
    size_type size = 0;
    clGetPlatformInfo(platform, CL_PLATFORM_VERSION, 0, NULL, &amp;size);

    vector&lt;char&gt; versionInfo(size);
    clGetPlatformInfo(platform, CL_PLATFORM_VERSION, size, versionInfo.data(), &amp;size);
    return getVersion(versionInfo);
}

static cl_uint getDevicePlatformVersion(cl_device_id device)
{
    cl_platform_id platform;
    clGetDeviceInfo(device, CL_DEVICE_PLATFORM, sizeof(platform), &amp;platform, NULL);
    return getPlatformVersion(platform);
}

static cl_uint getContextPlatformVersion(cl_context context)
{
    // The platform cannot be queried directly, so we first have to grab a
    // device and obtain its context
    size_type size = 0;
    clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &amp;size);
    if (size == 0)
        return 0;
    vector&lt;cl_device_id&gt; devices(size/sizeof(cl_device_id));
    clGetContextInfo(context, CL_CONTEXT_DEVICES, size, devices.data(), NULL);
    return getDevicePlatformVersion(devices[0]);
}
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120

template &lt;typename T&gt;
class Wrapper
{
public:
    typedef T cl_type;

protected:
    cl_type object_;

public:
    Wrapper() : object_(NULL) { }
    
    Wrapper(const cl_type &amp;obj, bool retainObject) : object_(obj) 
    {
        if (retainObject) { 
            detail::errHandler(retain(), __RETAIN_ERR); 
        }
    }

    ~Wrapper()
    {
        if (object_ != NULL) { release(); }
    }

    Wrapper(const Wrapper&lt;cl_type&gt;&amp; rhs)
    {
        object_ = rhs.object_;
        detail::errHandler(retain(), __RETAIN_ERR);
    }

    Wrapper(Wrapper&lt;cl_type&gt;&amp;&amp; rhs) CL_HPP_NOEXCEPT_
    {
        object_ = rhs.object_;
        rhs.object_ = NULL;
    }

    Wrapper&lt;cl_type&gt;&amp; operator = (const Wrapper&lt;cl_type&gt;&amp; rhs)
    {
        if (this != &amp;rhs) {
            detail::errHandler(release(), __RELEASE_ERR);
            object_ = rhs.object_;
            detail::errHandler(retain(), __RETAIN_ERR);
        }
        return *this;
    }

    Wrapper&lt;cl_type&gt;&amp; operator = (Wrapper&lt;cl_type&gt;&amp;&amp; rhs)
    {
        if (this != &amp;rhs) {
            detail::errHandler(release(), __RELEASE_ERR);
            object_ = rhs.object_;
            rhs.object_ = NULL;
        }
        return *this;
    }

    Wrapper&lt;cl_type&gt;&amp; operator = (const cl_type &amp;rhs)
    {
        detail::errHandler(release(), __RELEASE_ERR);
        object_ = rhs;
        return *this;
    }

    const cl_type&amp; operator ()() const { return object_; }

    cl_type&amp; operator ()() { return object_; }

    cl_type get() const { return object_; }

protected:
    template&lt;typename Func, typename U&gt;
    friend inline cl_int getInfoHelper(Func, cl_uint, U*, int, typename U::cl_type);

    cl_int retain() const
    {
        if (object_ != nullptr) {
            return ReferenceHandler&lt;cl_type&gt;::retain(object_);
        }
        else {
            return CL_SUCCESS;
        }
    }

    cl_int release() const
    {
        if (object_ != nullptr) {
            return ReferenceHandler&lt;cl_type&gt;::release(object_);
        }
        else {
            return CL_SUCCESS;
        }
    }
};

template &lt;&gt;
class Wrapper&lt;cl_device_id&gt;
{
public:
    typedef cl_device_id cl_type;

protected:
    cl_type object_;
    bool referenceCountable_;

    static bool isReferenceCountable(cl_device_id device)
    {
        bool retVal = false;
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120
        if (device != NULL) {
            int version = getDevicePlatformVersion(device);
            if(version &gt; ((1 &lt;&lt; 16) + 1)) {
                retVal = true;
            }
        }
#else // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120
        retVal = true;
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
        return retVal;
    }

public:
    Wrapper() : object_(NULL), referenceCountable_(false) 
    { 
    }
    
    Wrapper(const cl_type &amp;obj, bool retainObject) : 
        object_(obj), 
        referenceCountable_(false) 
    {
        referenceCountable_ = isReferenceCountable(obj); 

        if (retainObject) {
            detail::errHandler(retain(), __RETAIN_ERR);
        }
    }

    ~Wrapper()
    {
        release();
    }
    
    Wrapper(const Wrapper&lt;cl_type&gt;&amp; rhs)
    {
        object_ = rhs.object_;
        referenceCountable_ = isReferenceCountable(object_); 
        detail::errHandler(retain(), __RETAIN_ERR);
    }

    Wrapper(Wrapper&lt;cl_type&gt;&amp;&amp; rhs) CL_HPP_NOEXCEPT_
    {
        object_ = rhs.object_;
        referenceCountable_ = rhs.referenceCountable_;
        rhs.object_ = NULL;
        rhs.referenceCountable_ = false;
    }

    Wrapper&lt;cl_type&gt;&amp; operator = (const Wrapper&lt;cl_type&gt;&amp; rhs)
    {
        if (this != &amp;rhs) {
            detail::errHandler(release(), __RELEASE_ERR);
            object_ = rhs.object_;
            referenceCountable_ = rhs.referenceCountable_;
            detail::errHandler(retain(), __RETAIN_ERR);
        }
        return *this;
    }

    Wrapper&lt;cl_type&gt;&amp; operator = (Wrapper&lt;cl_type&gt;&amp;&amp; rhs)
    {
        if (this != &amp;rhs) {
            detail::errHandler(release(), __RELEASE_ERR);
            object_ = rhs.object_;
            referenceCountable_ = rhs.referenceCountable_;
            rhs.object_ = NULL;
            rhs.referenceCountable_ = false;
        }
        return *this;
    }

    Wrapper&lt;cl_type&gt;&amp; operator = (const cl_type &amp;rhs)
    {
        detail::errHandler(release(), __RELEASE_ERR);
        object_ = rhs;
        referenceCountable_ = isReferenceCountable(object_); 
        return *this;
    }

    const cl_type&amp; operator ()() const { return object_; }

    cl_type&amp; operator ()() { return object_; }

    cl_type get() const { return object_; }

protected:
    template&lt;typename Func, typename U&gt;
    friend inline cl_int getInfoHelper(Func, cl_uint, U*, int, typename U::cl_type);

    template&lt;typename Func, typename U&gt;
    friend inline cl_int getInfoHelper(Func, cl_uint, vector&lt;U&gt;*, int, typename U::cl_type);

    cl_int retain() const
    {
        if( object_ != nullptr &amp;&amp; referenceCountable_ ) {
            return ReferenceHandler&lt;cl_type&gt;::retain(object_);
        }
        else {
            return CL_SUCCESS;
        }
    }

    cl_int release() const
    {
        if (object_ != nullptr &amp;&amp; referenceCountable_) {
            return ReferenceHandler&lt;cl_type&gt;::release(object_);
        }
        else {
            return CL_SUCCESS;
        }
    }
};

template &lt;typename T&gt;
inline bool operator==(const Wrapper&lt;T&gt; &amp;lhs, const Wrapper&lt;T&gt; &amp;rhs)
{
    return lhs() == rhs();
}

template &lt;typename T&gt;
inline bool operator!=(const Wrapper&lt;T&gt; &amp;lhs, const Wrapper&lt;T&gt; &amp;rhs)
{
    return !operator==(lhs, rhs);
}

} // namespace detail
//! \endcond


using BuildLogType = vector&lt;std::pair&lt;cl::Device, typename detail::param_traits&lt;detail::cl_program_build_info, CL_PROGRAM_BUILD_LOG&gt;::param_type&gt;&gt;;
#if defined(CL_HPP_ENABLE_EXCEPTIONS)
/**
* Exception class for build errors to carry build info
*/
class BuildError : public Error
{
private:
    BuildLogType buildLogs;
public:
    BuildError(cl_int err, const char * errStr, const BuildLogType &amp;vec) : Error(err, errStr), buildLogs(vec)
    {
    }

    BuildLogType getBuildLog() const
    {
        return buildLogs;
    }
};
namespace detail {
    static inline cl_int buildErrHandler(
        cl_int err,
        const char * errStr,
        const BuildLogType &amp;buildLogs)
    {
        if (err != CL_SUCCESS) {
            throw BuildError(err, errStr, buildLogs);
        }
        return err;
    }
} // namespace detail

#else
namespace detail {
    static inline cl_int buildErrHandler(
        cl_int err,
        const char * errStr,
        const BuildLogType &amp;buildLogs)
    {
        (void)buildLogs; // suppress unused variable warning
        (void)errStr;
        return err;
    }
} // namespace detail
#endif // #if defined(CL_HPP_ENABLE_EXCEPTIONS)


/*! \stuct ImageFormat
 *  \brief Adds constructors and member functions for cl_image_format.
 *
 *  \see cl_image_format
 */
struct ImageFormat : public cl_image_format
{
    //! \brief Default constructor - performs no initialization.
    ImageFormat(){}

    //! \brief Initializing constructor.
    ImageFormat(cl_channel_order order, cl_channel_type type)
    {
        image_channel_order = order;
        image_channel_data_type = type;
    }

    //! \brief Assignment operator.
    ImageFormat&amp; operator = (const ImageFormat&amp; rhs)
    {
        if (this != &amp;rhs) {
            this-&gt;image_channel_data_type = rhs.image_channel_data_type;
            this-&gt;image_channel_order     = rhs.image_channel_order;
        }
        return *this;
    }
};

/*! \brief Class interface for cl_device_id.
 *
 *  \note Copies of these objects are inexpensive, since they don't 'own'
 *        any underlying resources or data structures.
 *
 *  \see cl_device_id
 */
class Device : public detail::Wrapper&lt;cl_device_id&gt;
{
private:
    static std::once_flag default_initialized_;
    static Device default_;
    static cl_int default_error_;

    /*! \brief Create the default context.
    *
    * This sets @c default_ and @c default_error_. It does not throw
    * @c cl::Error.
    */
    static void makeDefault();

    /*! \brief Create the default platform from a provided platform.
    *
    * This sets @c default_. It does not throw
    * @c cl::Error.
    */
    static void makeDefaultProvided(const Device &amp;p) {
        default_ = p;
    }

public:
#ifdef CL_HPP_UNIT_TEST_ENABLE
    /*! \brief Reset the default.
    *
    * This sets @c default_ to an empty value to support cleanup in
    * the unit test framework.
    * This function is not thread safe.
    */
    static void unitTestClearDefault() {
        default_ = Device();
    }
#endif // #ifdef CL_HPP_UNIT_TEST_ENABLE

    //! \brief Default constructor - initializes to NULL.
    Device() : detail::Wrapper&lt;cl_type&gt;() { }

    /*! \brief Constructor from cl_device_id.
     * 
     *  This simply copies the device ID value, which is an inexpensive operation.
     */
    explicit Device(const cl_device_id &amp;device, bool retainObject = false) : 
        detail::Wrapper&lt;cl_type&gt;(device, retainObject) { }

    /*! \brief Returns the first device on the default context.
     *
     *  \see Context::getDefault()
     */
    static Device getDefault(
        cl_int *errResult = NULL)
    {
        std::call_once(default_initialized_, makeDefault);
        detail::errHandler(default_error_);
        if (errResult != NULL) {
            *errResult = default_error_;
        }
        return default_;
    }

    /**
    * Modify the default device to be used by
    * subsequent operations.
    * Will only set the default if no default was previously created.
    * @return updated default device.
    *         Should be compared to the passed value to ensure that it was updated.
    */
    static Device setDefault(const Device &amp;default_device)
    {
        std::call_once(default_initialized_, makeDefaultProvided, std::cref(default_device));
        detail::errHandler(default_error_);
        return default_;
    }

    /*! \brief Assignment operator from cl_device_id.
     * 
     *  This simply copies the device ID value, which is an inexpensive operation.
     */
    Device&amp; operator = (const cl_device_id&amp; rhs)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
    * Required for MSVC.
    */
    Device(const Device&amp; dev) : detail::Wrapper&lt;cl_type&gt;(dev) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
    * Required for MSVC.
    */
    Device&amp; operator = (const Device &amp;dev)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(dev);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
    * Required for MSVC.
    */
    Device(Device&amp;&amp; dev) CL_HPP_NOEXCEPT_ : detail::Wrapper&lt;cl_type&gt;(std::move(dev)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
    * Required for MSVC.
    */
    Device&amp; operator = (Device &amp;&amp;dev)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(std::move(dev));
        return *this;
    }

    //! \brief Wrapper for clGetDeviceInfo().
    template &lt;typename T&gt;
    cl_int getInfo(cl_device_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(&amp;::clGetDeviceInfo, object_, name, param),
            __GET_DEVICE_INFO_ERR);
    }

    //! \brief Wrapper for clGetDeviceInfo() that returns by value.
    template &lt;cl_device_info name&gt; typename
    detail::param_traits&lt;detail::cl_device_info, name&gt;::param_type
    getInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_device_info, name&gt;::param_type param;
        cl_int result = getInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }


#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
    /**
     * Return the current value of the host clock as seen by the device.
     * The resolution of the device timer may be queried with the
     * CL_DEVICE_PROFILING_TIMER_RESOLUTION query.
     * @return The host timer value.
     */
    cl_ulong getHostTimer(cl_int *error = nullptr)
    {
        cl_ulong retVal = 0;
        cl_int err = 
            clGetHostTimer(this-&gt;get(), &amp;retVal);
        detail::errHandler(
            err,
            __GET_HOST_TIMER_ERR);
        if (error) {
            *error = err;
        }
        return retVal;
    }

    /**
     * Return a synchronized pair of host and device timestamps as seen by device.
     * Use to correlate the clocks and get the host timer only using getHostTimer
     * as a lower cost mechanism in between calls.
     * The resolution of the host timer may be queried with the 
     * CL_PLATFORM_HOST_TIMER_RESOLUTION query.
     * The resolution of the device timer may be queried with the
     * CL_DEVICE_PROFILING_TIMER_RESOLUTION query.
     * @return A pair of (device timer, host timer) timer values.
     */
    std::pair&lt;cl_ulong, cl_ulong&gt; getDeviceAndHostTimer(cl_int *error = nullptr)
    {
        std::pair&lt;cl_ulong, cl_ulong&gt; retVal;
        cl_int err =
            clGetDeviceAndHostTimer(this-&gt;get(), &amp;(retVal.first), &amp;(retVal.second));
        detail::errHandler(
            err,
            __GET_DEVICE_AND_HOST_TIMER_ERR);
        if (error) {
            *error = err;
        }
        return retVal;
    }
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210

    /**
     * CL 1.2 version
     */
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
    //! \brief Wrapper for clCreateSubDevices().
    cl_int createSubDevices(
        const cl_device_partition_property * properties,
        vector&lt;Device&gt;* devices)
    {
        cl_uint n = 0;
        cl_int err = clCreateSubDevices(object_, properties, 0, NULL, &amp;n);
        if (err != CL_SUCCESS) {
            return detail::errHandler(err, __CREATE_SUB_DEVICES_ERR);
        }

        vector&lt;cl_device_id&gt; ids(n);
        err = clCreateSubDevices(object_, properties, n, ids.data(), NULL);
        if (err != CL_SUCCESS) {
            return detail::errHandler(err, __CREATE_SUB_DEVICES_ERR);
        }

        // Cannot trivially assign because we need to capture intermediates 
        // with safe construction
        if (devices) {
            devices-&gt;resize(ids.size());

            // Assign to param, constructing with retain behaviour
            // to correctly capture each underlying CL object
            for (size_type i = 0; i &lt; ids.size(); i++) {
                // We do not need to retain because this device is being created 
                // by the runtime
                (*devices)[i] = Device(ids[i], false);
            }
        }

        return CL_SUCCESS;
    }
#elif defined(CL_HPP_USE_CL_DEVICE_FISSION)

/**
 * CL 1.1 version that uses device fission extension.
 */
    cl_int createSubDevices(
        const cl_device_partition_property_ext * properties,
        vector&lt;Device&gt;* devices)
    {
        typedef CL_API_ENTRY cl_int 
            ( CL_API_CALL * PFN_clCreateSubDevicesEXT)(
                cl_device_id /*in_device*/,
                const cl_device_partition_property_ext * /* properties */,
                cl_uint /*num_entries*/,
                cl_device_id * /*out_devices*/,
                cl_uint * /*num_devices*/ ) CL_EXT_SUFFIX__VERSION_1_1;

        static PFN_clCreateSubDevicesEXT pfn_clCreateSubDevicesEXT = NULL;
        CL_HPP_INIT_CL_EXT_FCN_PTR_(clCreateSubDevicesEXT);

        cl_uint n = 0;
        cl_int err = pfn_clCreateSubDevicesEXT(object_, properties, 0, NULL, &amp;n);
        if (err != CL_SUCCESS) {
            return detail::errHandler(err, __CREATE_SUB_DEVICES_ERR);
        }

        vector&lt;cl_device_id&gt; ids(n);
        err = pfn_clCreateSubDevicesEXT(object_, properties, n, ids.data(), NULL);
        if (err != CL_SUCCESS) {
            return detail::errHandler(err, __CREATE_SUB_DEVICES_ERR);
        }
        // Cannot trivially assign because we need to capture intermediates 
        // with safe construction
        if (devices) {
            devices-&gt;resize(ids.size());

            // Assign to param, constructing with retain behaviour
            // to correctly capture each underlying CL object
            for (size_type i = 0; i &lt; ids.size(); i++) {
                // We do not need to retain because this device is being created 
                // by the runtime
                (*devices)[i] = Device(ids[i], false);
            }
        }
        return CL_SUCCESS;
    }
#endif // defined(CL_HPP_USE_CL_DEVICE_FISSION)
};

CL_HPP_DEFINE_STATIC_MEMBER_ std::once_flag Device::default_initialized_;
CL_HPP_DEFINE_STATIC_MEMBER_ Device Device::default_;
CL_HPP_DEFINE_STATIC_MEMBER_ cl_int Device::default_error_ = CL_SUCCESS;

/*! \brief Class interface for cl_platform_id.
 *
 *  \note Copies of these objects are inexpensive, since they don't 'own'
 *        any underlying resources or data structures.
 *
 *  \see cl_platform_id
 */
class Platform : public detail::Wrapper&lt;cl_platform_id&gt;
{
private:
    static std::once_flag default_initialized_;
    static Platform default_;
    static cl_int default_error_;

    /*! \brief Create the default context.
    *
    * This sets @c default_ and @c default_error_. It does not throw
    * @c cl::Error.
    */
    static void makeDefault() {
        /* Throwing an exception from a call_once invocation does not do
        * what we wish, so we catch it and save the error.
        */
#if defined(CL_HPP_ENABLE_EXCEPTIONS)
        try
#endif
        {
            // If default wasn't passed ,generate one
            // Otherwise set it
            cl_uint n = 0;

            cl_int err = ::clGetPlatformIDs(0, NULL, &amp;n);
            if (err != CL_SUCCESS) {
                default_error_ = err;
                return;
            }
            if (n == 0) {
                default_error_ = CL_INVALID_PLATFORM;
                return;
            }

            vector&lt;cl_platform_id&gt; ids(n);
            err = ::clGetPlatformIDs(n, ids.data(), NULL);
            if (err != CL_SUCCESS) {
                default_error_ = err;
                return;
            }

            default_ = Platform(ids[0]);
        }
#if defined(CL_HPP_ENABLE_EXCEPTIONS)
        catch (cl::Error &amp;e) {
            default_error_ = e.err();
        }
#endif
    }

    /*! \brief Create the default platform from a provided platform.
     *
     * This sets @c default_. It does not throw
     * @c cl::Error.
     */
    static void makeDefaultProvided(const Platform &amp;p) {
       default_ = p;
    }
    
public:
#ifdef CL_HPP_UNIT_TEST_ENABLE
    /*! \brief Reset the default.
    *
    * This sets @c default_ to an empty value to support cleanup in
    * the unit test framework.
    * This function is not thread safe.
    */
    static void unitTestClearDefault() {
        default_ = Platform();
    }
#endif // #ifdef CL_HPP_UNIT_TEST_ENABLE

    //! \brief Default constructor - initializes to NULL.
    Platform() : detail::Wrapper&lt;cl_type&gt;()  { }

    /*! \brief Constructor from cl_platform_id.
     * 
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  This simply copies the platform ID value, which is an inexpensive operation.
     */
    explicit Platform(const cl_platform_id &amp;platform, bool retainObject = false) : 
        detail::Wrapper&lt;cl_type&gt;(platform, retainObject) { }

    /*! \brief Assignment operator from cl_platform_id.
     * 
     *  This simply copies the platform ID value, which is an inexpensive operation.
     */
    Platform&amp; operator = (const cl_platform_id&amp; rhs)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(rhs);
        return *this;
    }

    static Platform getDefault(
        cl_int *errResult = NULL)
    {
        std::call_once(default_initialized_, makeDefault);
        detail::errHandler(default_error_);
        if (errResult != NULL) {
            *errResult = default_error_;
        }
        return default_;
    }

    /**
     * Modify the default platform to be used by 
     * subsequent operations.
     * Will only set the default if no default was previously created.
     * @return updated default platform. 
     *         Should be compared to the passed value to ensure that it was updated.
     */
    static Platform setDefault(const Platform &amp;default_platform)
    {
        std::call_once(default_initialized_, makeDefaultProvided, std::cref(default_platform));
        detail::errHandler(default_error_);
        return default_;
    }

    //! \brief Wrapper for clGetPlatformInfo().
    template &lt;typename T&gt;
    cl_int getInfo(cl_platform_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(&amp;::clGetPlatformInfo, object_, name, param),
            __GET_PLATFORM_INFO_ERR);
    }

    //! \brief Wrapper for clGetPlatformInfo() that returns by value.
    template &lt;cl_platform_info name&gt; typename
    detail::param_traits&lt;detail::cl_platform_info, name&gt;::param_type
    getInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_platform_info, name&gt;::param_type param;
        cl_int result = getInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }

    /*! \brief Gets a list of devices for this platform.
     * 
     *  Wraps clGetDeviceIDs().
     */
    cl_int getDevices(
        cl_device_type type,
        vector&lt;Device&gt;* devices) const
    {
        cl_uint n = 0;
        if( devices == NULL ) {
            return detail::errHandler(CL_INVALID_ARG_VALUE, __GET_DEVICE_IDS_ERR);
        }
        cl_int err = ::clGetDeviceIDs(object_, type, 0, NULL, &amp;n);
        if (err != CL_SUCCESS  &amp;&amp; err != CL_DEVICE_NOT_FOUND) {
            return detail::errHandler(err, __GET_DEVICE_IDS_ERR);
        }

        vector&lt;cl_device_id&gt; ids(n);
        if (n&gt;0) {
            err = ::clGetDeviceIDs(object_, type, n, ids.data(), NULL);
            if (err != CL_SUCCESS) {
                return detail::errHandler(err, __GET_DEVICE_IDS_ERR);
            }
        }

        // Cannot trivially assign because we need to capture intermediates 
        // with safe construction
        // We must retain things we obtain from the API to avoid releasing
        // API-owned objects.
        if (devices) {
            devices-&gt;resize(ids.size());

            // Assign to param, constructing with retain behaviour
            // to correctly capture each underlying CL object
            for (size_type i = 0; i &lt; ids.size(); i++) {
                (*devices)[i] = Device(ids[i], true);
            }
        }
        return CL_SUCCESS;
    }

#if defined(CL_HPP_USE_DX_INTEROP)
   /*! \brief Get the list of available D3D10 devices.
     *
     *  \param d3d_device_source.
     *
     *  \param d3d_object.
     *
     *  \param d3d_device_set.
     *
     *  \param devices returns a vector of OpenCL D3D10 devices found. The cl::Device
     *  values returned in devices can be used to identify a specific OpenCL
     *  device. If \a devices argument is NULL, this argument is ignored.
     *
     *  \return One of the following values:
     *    - CL_SUCCESS if the function is executed successfully.
     *
     *  The application can query specific capabilities of the OpenCL device(s)
     *  returned by cl::getDevices. This can be used by the application to
     *  determine which device(s) to use.
     *
     * \note In the case that exceptions are enabled and a return value
     * other than CL_SUCCESS is generated, then cl::Error exception is
     * generated.
     */
    cl_int getDevices(
        cl_d3d10_device_source_khr d3d_device_source,
        void *                     d3d_object,
        cl_d3d10_device_set_khr    d3d_device_set,
        vector&lt;Device&gt;* devices) const
    {
        typedef CL_API_ENTRY cl_int (CL_API_CALL *PFN_clGetDeviceIDsFromD3D10KHR)(
            cl_platform_id platform, 
            cl_d3d10_device_source_khr d3d_device_source, 
            void * d3d_object,
            cl_d3d10_device_set_khr d3d_device_set,
            cl_uint num_entries,
            cl_device_id * devices,
            cl_uint* num_devices);

        if( devices == NULL ) {
            return detail::errHandler(CL_INVALID_ARG_VALUE, __GET_DEVICE_IDS_ERR);
        }

        static PFN_clGetDeviceIDsFromD3D10KHR pfn_clGetDeviceIDsFromD3D10KHR = NULL;
        CL_HPP_INIT_CL_EXT_FCN_PTR_PLATFORM_(object_, clGetDeviceIDsFromD3D10KHR);

        cl_uint n = 0;
        cl_int err = pfn_clGetDeviceIDsFromD3D10KHR(
            object_, 
            d3d_device_source, 
            d3d_object,
            d3d_device_set, 
            0, 
            NULL, 
            &amp;n);
        if (err != CL_SUCCESS) {
            return detail::errHandler(err, __GET_DEVICE_IDS_ERR);
        }

        vector&lt;cl_device_id&gt; ids(n);
        err = pfn_clGetDeviceIDsFromD3D10KHR(
            object_, 
            d3d_device_source, 
            d3d_object,
            d3d_device_set,
            n, 
            ids.data(), 
            NULL);
        if (err != CL_SUCCESS) {
            return detail::errHandler(err, __GET_DEVICE_IDS_ERR);
        }

        // Cannot trivially assign because we need to capture intermediates 
        // with safe construction
        // We must retain things we obtain from the API to avoid releasing
        // API-owned objects.
        if (devices) {
            devices-&gt;resize(ids.size());

            // Assign to param, constructing with retain behaviour
            // to correctly capture each underlying CL object
            for (size_type i = 0; i &lt; ids.size(); i++) {
                (*devices)[i] = Device(ids[i], true);
            }
        }
        return CL_SUCCESS;
    }
#endif

    /*! \brief Gets a list of available platforms.
     * 
     *  Wraps clGetPlatformIDs().
     */
    static cl_int get(
        vector&lt;Platform&gt;* platforms)
    {
        cl_uint n = 0;

        if( platforms == NULL ) {
            return detail::errHandler(CL_INVALID_ARG_VALUE, __GET_PLATFORM_IDS_ERR);
        }

        cl_int err = ::clGetPlatformIDs(0, NULL, &amp;n);
        if (err != CL_SUCCESS) {
            return detail::errHandler(err, __GET_PLATFORM_IDS_ERR);
        }

        vector&lt;cl_platform_id&gt; ids(n);
        err = ::clGetPlatformIDs(n, ids.data(), NULL);
        if (err != CL_SUCCESS) {
            return detail::errHandler(err, __GET_PLATFORM_IDS_ERR);
        }

        if (platforms) {
            platforms-&gt;resize(ids.size());

            // Platforms don't reference count
            for (size_type i = 0; i &lt; ids.size(); i++) {
                (*platforms)[i] = Platform(ids[i]);
            }
        }
        return CL_SUCCESS;
    }

    /*! \brief Gets the first available platform.
     * 
     *  Wraps clGetPlatformIDs(), returning the first result.
     */
    static cl_int get(
        Platform * platform)
    {
        cl_int err;
        Platform default_platform = Platform::getDefault(&amp;err);
        if (platform) {
            *platform = default_platform;
        }
        return err;
    }

    /*! \brief Gets the first available platform, returning it by value.
     *
     * \return Returns a valid platform if one is available.
     *         If no platform is available will return a null platform.
     * Throws an exception if no platforms are available
     * or an error condition occurs.
     * Wraps clGetPlatformIDs(), returning the first result.
     */
    static Platform get(
        cl_int * errResult = NULL)
    {
        cl_int err;
        Platform default_platform = Platform::getDefault(&amp;err);
        if (errResult) {
            *errResult = err;
        }
        return default_platform;
    }    
    
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
    //! \brief Wrapper for clUnloadCompiler().
    cl_int
    unloadCompiler()
    {
        return ::clUnloadPlatformCompiler(object_);
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
}; // class Platform

CL_HPP_DEFINE_STATIC_MEMBER_ std::once_flag Platform::default_initialized_;
CL_HPP_DEFINE_STATIC_MEMBER_ Platform Platform::default_;
CL_HPP_DEFINE_STATIC_MEMBER_ cl_int Platform::default_error_ = CL_SUCCESS;


/**
 * Deprecated APIs for 1.2
 */
#if defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
/**
 * Unload the OpenCL compiler.
 * \note Deprecated for OpenCL 1.2. Use Platform::unloadCompiler instead.
 */
inline CL_EXT_PREFIX__VERSION_1_1_DEPRECATED cl_int
UnloadCompiler() CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;
inline cl_int
UnloadCompiler()
{
    return ::clUnloadCompiler();
}
#endif // #if defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)

/*! \brief Class interface for cl_context.
 *
 *  \note Copies of these objects are shallow, meaning that the copy will refer
 *        to the same underlying cl_context as the original.  For details, see
 *        clRetainContext() and clReleaseContext().
 *
 *  \see cl_context
 */
class Context 
    : public detail::Wrapper&lt;cl_context&gt;
{
private:
    static std::once_flag default_initialized_;
    static Context default_;
    static cl_int default_error_;

    /*! \brief Create the default context from the default device type in the default platform.
     *
     * This sets @c default_ and @c default_error_. It does not throw
     * @c cl::Error.
     */
    static void makeDefault() {
        /* Throwing an exception from a call_once invocation does not do
         * what we wish, so we catch it and save the error.
         */
#if defined(CL_HPP_ENABLE_EXCEPTIONS)
        try
#endif
        {
#if !defined(__APPLE__) &amp;&amp; !defined(__MACOS)
            const Platform &amp;p = Platform::getDefault();
            cl_platform_id defaultPlatform = p();
            cl_context_properties properties[3] = {
                CL_CONTEXT_PLATFORM, (cl_context_properties)defaultPlatform, 0
            };
#else // #if !defined(__APPLE__) &amp;&amp; !defined(__MACOS)
            cl_context_properties *properties = nullptr;
#endif // #if !defined(__APPLE__) &amp;&amp; !defined(__MACOS)

            default_ = Context(
                CL_DEVICE_TYPE_DEFAULT,
                properties,
                NULL,
                NULL,
                &amp;default_error_);
        }
#if defined(CL_HPP_ENABLE_EXCEPTIONS)
        catch (cl::Error &amp;e) {
            default_error_ = e.err();
        }
#endif
    }


    /*! \brief Create the default context from a provided Context.
     *
     * This sets @c default_. It does not throw
     * @c cl::Error.
     */
    static void makeDefaultProvided(const Context &amp;c) {
        default_ = c;
    }
    
public:
#ifdef CL_HPP_UNIT_TEST_ENABLE
    /*! \brief Reset the default.
    *
    * This sets @c default_ to an empty value to support cleanup in
    * the unit test framework.
    * This function is not thread safe.
    */
    static void unitTestClearDefault() {
        default_ = Context();
    }
#endif // #ifdef CL_HPP_UNIT_TEST_ENABLE

    /*! \brief Constructs a context including a list of specified devices.
     *
     *  Wraps clCreateContext().
     */
    Context(
        const vector&lt;Device&gt;&amp; devices,
        cl_context_properties* properties = NULL,
        void (CL_CALLBACK * notifyFptr)(
            const char *,
            const void *,
            size_type,
            void *) = NULL,
        void* data = NULL,
        cl_int* err = NULL)
    {
        cl_int error;

        size_type numDevices = devices.size();
        vector&lt;cl_device_id&gt; deviceIDs(numDevices);

        for( size_type deviceIndex = 0; deviceIndex &lt; numDevices; ++deviceIndex ) {
            deviceIDs[deviceIndex] = (devices[deviceIndex])();
        }

        object_ = ::clCreateContext(
            properties, (cl_uint) numDevices,
            deviceIDs.data(),
            notifyFptr, data, &amp;error);

        detail::errHandler(error, __CREATE_CONTEXT_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    /*! \brief Constructs a context including a specific device.
     *
     *  Wraps clCreateContext().
     */
    Context(
        const Device&amp; device,
        cl_context_properties* properties = NULL,
        void (CL_CALLBACK * notifyFptr)(
            const char *,
            const void *,
            size_type,
            void *) = NULL,
        void* data = NULL,
        cl_int* err = NULL)
    {
        cl_int error;

        cl_device_id deviceID = device();

        object_ = ::clCreateContext(
            properties, 1,
            &amp;deviceID,
            notifyFptr, data, &amp;error);

        detail::errHandler(error, __CREATE_CONTEXT_ERR);
        if (err != NULL) {
            *err = error;
        }
    }
    
    /*! \brief Constructs a context including all or a subset of devices of a specified type.
     *
     *  Wraps clCreateContextFromType().
     */
    Context(
        cl_device_type type,
        cl_context_properties* properties = NULL,
        void (CL_CALLBACK * notifyFptr)(
            const char *,
            const void *,
            size_type,
            void *) = NULL,
        void* data = NULL,
        cl_int* err = NULL)
    {
        cl_int error;

#if !defined(__APPLE__) &amp;&amp; !defined(__MACOS)
        cl_context_properties prop[4] = {CL_CONTEXT_PLATFORM, 0, 0, 0 };

        if (properties == NULL) {
            // Get a valid platform ID as we cannot send in a blank one
            vector&lt;Platform&gt; platforms;
            error = Platform::get(&amp;platforms);
            if (error != CL_SUCCESS) {
                detail::errHandler(error, __CREATE_CONTEXT_FROM_TYPE_ERR);
                if (err != NULL) {
                    *err = error;
                }
                return;
            }

            // Check the platforms we found for a device of our specified type
            cl_context_properties platform_id = 0;
            for (unsigned int i = 0; i &lt; platforms.size(); i++) {

                vector&lt;Device&gt; devices;

#if defined(CL_HPP_ENABLE_EXCEPTIONS)
                try {
#endif

                    error = platforms[i].getDevices(type, &amp;devices);

#if defined(CL_HPP_ENABLE_EXCEPTIONS)
                } catch (cl::Error&amp; e) {
                    error = e.err();
                }
    // Catch if exceptions are enabled as we don't want to exit if first platform has no devices of type
    // We do error checking next anyway, and can throw there if needed
#endif

                // Only squash CL_SUCCESS and CL_DEVICE_NOT_FOUND
                if (error != CL_SUCCESS &amp;&amp; error != CL_DEVICE_NOT_FOUND) {
                    detail::errHandler(error, __CREATE_CONTEXT_FROM_TYPE_ERR);
                    if (err != NULL) {
                        *err = error;
                    }
                }

                if (devices.size() &gt; 0) {
                    platform_id = (cl_context_properties)platforms[i]();
                    break;
                }
            }

            if (platform_id == 0) {
                detail::errHandler(CL_DEVICE_NOT_FOUND, __CREATE_CONTEXT_FROM_TYPE_ERR);
                if (err != NULL) {
                    *err = CL_DEVICE_NOT_FOUND;
                }
                return;
            }

            prop[1] = platform_id;
            properties = &amp;prop[0];
        }
#endif
        object_ = ::clCreateContextFromType(
            properties, type, notifyFptr, data, &amp;error);

        detail::errHandler(error, __CREATE_CONTEXT_FROM_TYPE_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Context(const Context&amp; ctx) : detail::Wrapper&lt;cl_type&gt;(ctx) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Context&amp; operator = (const Context &amp;ctx)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(ctx);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Context(Context&amp;&amp; ctx) CL_HPP_NOEXCEPT_ : detail::Wrapper&lt;cl_type&gt;(std::move(ctx)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Context&amp; operator = (Context &amp;&amp;ctx)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(std::move(ctx));
        return *this;
    }


    /*! \brief Returns a singleton context including all devices of CL_DEVICE_TYPE_DEFAULT.
     *
     *  \note All calls to this function return the same cl_context as the first.
     */
    static Context getDefault(cl_int * err = NULL) 
    {
        std::call_once(default_initialized_, makeDefault);
        detail::errHandler(default_error_);
        if (err != NULL) {
            *err = default_error_;
        }
        return default_;
    }

    /**
     * Modify the default context to be used by
     * subsequent operations.
     * Will only set the default if no default was previously created.
     * @return updated default context.
     *         Should be compared to the passed value to ensure that it was updated.
     */
    static Context setDefault(const Context &amp;default_context)
    {
        std::call_once(default_initialized_, makeDefaultProvided, std::cref(default_context));
        detail::errHandler(default_error_);
        return default_;
    }

    //! \brief Default constructor - initializes to NULL.
    Context() : detail::Wrapper&lt;cl_type&gt;() { }

    /*! \brief Constructor from cl_context - takes ownership.
     * 
     *  This effectively transfers ownership of a refcount on the cl_context
     *  into the new Context object.
     */
    explicit Context(const cl_context&amp; context, bool retainObject = false) : 
        detail::Wrapper&lt;cl_type&gt;(context, retainObject) { }

    /*! \brief Assignment operator from cl_context - takes ownership.
     * 
     *  This effectively transfers ownership of a refcount on the rhs and calls
     *  clReleaseContext() on the value previously held by this instance.
     */
    Context&amp; operator = (const cl_context&amp; rhs)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(rhs);
        return *this;
    }

    //! \brief Wrapper for clGetContextInfo().
    template &lt;typename T&gt;
    cl_int getInfo(cl_context_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(&amp;::clGetContextInfo, object_, name, param),
            __GET_CONTEXT_INFO_ERR);
    }

    //! \brief Wrapper for clGetContextInfo() that returns by value.
    template &lt;cl_context_info name&gt; typename
    detail::param_traits&lt;detail::cl_context_info, name&gt;::param_type
    getInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_context_info, name&gt;::param_type param;
        cl_int result = getInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }

    /*! \brief Gets a list of supported image formats.
     *  
     *  Wraps clGetSupportedImageFormats().
     */
    cl_int getSupportedImageFormats(
        cl_mem_flags flags,
        cl_mem_object_type type,
        vector&lt;ImageFormat&gt;* formats) const
    {
        cl_uint numEntries;
        
        if (!formats) {
            return CL_SUCCESS;
        }

        cl_int err = ::clGetSupportedImageFormats(
           object_, 
           flags,
           type, 
           0, 
           NULL, 
           &amp;numEntries);
        if (err != CL_SUCCESS) {
            return detail::errHandler(err, __GET_SUPPORTED_IMAGE_FORMATS_ERR);
        }

        if (numEntries &gt; 0) {
            vector&lt;ImageFormat&gt; value(numEntries);
            err = ::clGetSupportedImageFormats(
                object_,
                flags,
                type,
                numEntries,
                (cl_image_format*)value.data(),
                NULL);
            if (err != CL_SUCCESS) {
                return detail::errHandler(err, __GET_SUPPORTED_IMAGE_FORMATS_ERR);
            }

            formats-&gt;assign(begin(value), end(value));
        }
        else {
            // If no values are being returned, ensure an empty vector comes back
            formats-&gt;clear();
        }

        return CL_SUCCESS;
    }
};

inline void Device::makeDefault()
{
    /* Throwing an exception from a call_once invocation does not do
    * what we wish, so we catch it and save the error.
    */
#if defined(CL_HPP_ENABLE_EXCEPTIONS)
    try
#endif
    {
        cl_int error = 0;

        Context context = Context::getDefault(&amp;error);
        detail::errHandler(error, __CREATE_CONTEXT_ERR);

        if (error != CL_SUCCESS) {
            default_error_ = error;
        }
        else {
            default_ = context.getInfo&lt;CL_CONTEXT_DEVICES&gt;()[0];
            default_error_ = CL_SUCCESS;
        }
    }
#if defined(CL_HPP_ENABLE_EXCEPTIONS)
    catch (cl::Error &amp;e) {
        default_error_ = e.err();
    }
#endif
}

CL_HPP_DEFINE_STATIC_MEMBER_ std::once_flag Context::default_initialized_;
CL_HPP_DEFINE_STATIC_MEMBER_ Context Context::default_;
CL_HPP_DEFINE_STATIC_MEMBER_ cl_int Context::default_error_ = CL_SUCCESS;

/*! \brief Class interface for cl_event.
 *
 *  \note Copies of these objects are shallow, meaning that the copy will refer
 *        to the same underlying cl_event as the original.  For details, see
 *        clRetainEvent() and clReleaseEvent().
 *
 *  \see cl_event
 */
class Event : public detail::Wrapper&lt;cl_event&gt;
{
public:
    //! \brief Default constructor - initializes to NULL.
    Event() : detail::Wrapper&lt;cl_type&gt;() { }

    /*! \brief Constructor from cl_event - takes ownership.
     * 
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  This effectively transfers ownership of a refcount on the cl_event
     *  into the new Event object.
     */
    explicit Event(const cl_event&amp; event, bool retainObject = false) : 
        detail::Wrapper&lt;cl_type&gt;(event, retainObject) { }

    /*! \brief Assignment operator from cl_event - takes ownership.
     *
     *  This effectively transfers ownership of a refcount on the rhs and calls
     *  clReleaseEvent() on the value previously held by this instance.
     */
    Event&amp; operator = (const cl_event&amp; rhs)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(rhs);
        return *this;
    }

    //! \brief Wrapper for clGetEventInfo().
    template &lt;typename T&gt;
    cl_int getInfo(cl_event_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(&amp;::clGetEventInfo, object_, name, param),
            __GET_EVENT_INFO_ERR);
    }

    //! \brief Wrapper for clGetEventInfo() that returns by value.
    template &lt;cl_event_info name&gt; typename
    detail::param_traits&lt;detail::cl_event_info, name&gt;::param_type
    getInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_event_info, name&gt;::param_type param;
        cl_int result = getInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }

    //! \brief Wrapper for clGetEventProfilingInfo().
    template &lt;typename T&gt;
    cl_int getProfilingInfo(cl_profiling_info name, T* param) const
    {
        return detail::errHandler(detail::getInfo(
            &amp;::clGetEventProfilingInfo, object_, name, param),
            __GET_EVENT_PROFILE_INFO_ERR);
    }

    //! \brief Wrapper for clGetEventProfilingInfo() that returns by value.
    template &lt;cl_profiling_info name&gt; typename
    detail::param_traits&lt;detail::cl_profiling_info, name&gt;::param_type
    getProfilingInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_profiling_info, name&gt;::param_type param;
        cl_int result = getProfilingInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }

    /*! \brief Blocks the calling thread until this event completes.
     * 
     *  Wraps clWaitForEvents().
     */
    cl_int wait() const
    {
        return detail::errHandler(
            ::clWaitForEvents(1, &amp;object_),
            __WAIT_FOR_EVENTS_ERR);
    }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
    /*! \brief Registers a user callback function for a specific command execution status.
     *
     *  Wraps clSetEventCallback().
     */
    cl_int setCallback(
        cl_int type,
        void (CL_CALLBACK * pfn_notify)(cl_event, cl_int, void *),
        void * user_data = NULL)
    {
        return detail::errHandler(
            ::clSetEventCallback(
                object_,
                type,
                pfn_notify,
                user_data), 
            __SET_EVENT_CALLBACK_ERR);
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 110

    /*! \brief Blocks the calling thread until every event specified is complete.
     * 
     *  Wraps clWaitForEvents().
     */
    static cl_int
    waitForEvents(const vector&lt;Event&gt;&amp; events)
    {
        return detail::errHandler(
            ::clWaitForEvents(
                (cl_uint) events.size(), (events.size() &gt; 0) ? (cl_event*)&amp;events.front() : NULL),
            __WAIT_FOR_EVENTS_ERR);
    }
};

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
/*! \brief Class interface for user events (a subset of cl_event's).
 * 
 *  See Event for details about copy semantics, etc.
 */
class UserEvent : public Event
{
public:
    /*! \brief Constructs a user event on a given context.
     *
     *  Wraps clCreateUserEvent().
     */
    UserEvent(
        const Context&amp; context,
        cl_int * err = NULL)
    {
        cl_int error;
        object_ = ::clCreateUserEvent(
            context(),
            &amp;error);

        detail::errHandler(error, __CREATE_USER_EVENT_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    //! \brief Default constructor - initializes to NULL.
    UserEvent() : Event() { }

    /*! \brief Sets the execution status of a user event object.
     *
     *  Wraps clSetUserEventStatus().
     */
    cl_int setStatus(cl_int status)
    {
        return detail::errHandler(
            ::clSetUserEventStatus(object_,status), 
            __SET_USER_EVENT_STATUS_ERR);
    }
};
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 110

/*! \brief Blocks the calling thread until every event specified is complete.
 * 
 *  Wraps clWaitForEvents().
 */
inline static cl_int
WaitForEvents(const vector&lt;Event&gt;&amp; events)
{
    return detail::errHandler(
        ::clWaitForEvents(
            (cl_uint) events.size(), (events.size() &gt; 0) ? (cl_event*)&amp;events.front() : NULL),
        __WAIT_FOR_EVENTS_ERR);
}

/*! \brief Class interface for cl_mem.
 *
 *  \note Copies of these objects are shallow, meaning that the copy will refer
 *        to the same underlying cl_mem as the original.  For details, see
 *        clRetainMemObject() and clReleaseMemObject().
 *
 *  \see cl_mem
 */
class Memory : public detail::Wrapper&lt;cl_mem&gt;
{
public:
    //! \brief Default constructor - initializes to NULL.
    Memory() : detail::Wrapper&lt;cl_type&gt;() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     *  Optionally transfer ownership of a refcount on the cl_mem
     *  into the new Memory object.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *
     *  See Memory for further details.
     */
    explicit Memory(const cl_mem&amp; memory, bool retainObject) :
        detail::Wrapper&lt;cl_type&gt;(memory, retainObject) { }

    /*! \brief Assignment operator from cl_mem - takes ownership.
     *
     *  This effectively transfers ownership of a refcount on the rhs and calls
     *  clReleaseMemObject() on the value previously held by this instance.
     */
    Memory&amp; operator = (const cl_mem&amp; rhs)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Memory(const Memory&amp; mem) : detail::Wrapper&lt;cl_type&gt;(mem) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Memory&amp; operator = (const Memory &amp;mem)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(mem);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Memory(Memory&amp;&amp; mem) CL_HPP_NOEXCEPT_ : detail::Wrapper&lt;cl_type&gt;(std::move(mem)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Memory&amp; operator = (Memory &amp;&amp;mem)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(std::move(mem));
        return *this;
    }


    //! \brief Wrapper for clGetMemObjectInfo().
    template &lt;typename T&gt;
    cl_int getInfo(cl_mem_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(&amp;::clGetMemObjectInfo, object_, name, param),
            __GET_MEM_OBJECT_INFO_ERR);
    }

    //! \brief Wrapper for clGetMemObjectInfo() that returns by value.
    template &lt;cl_mem_info name&gt; typename
    detail::param_traits&lt;detail::cl_mem_info, name&gt;::param_type
    getInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_mem_info, name&gt;::param_type param;
        cl_int result = getInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
    /*! \brief Registers a callback function to be called when the memory object
     *         is no longer needed.
     *
     *  Wraps clSetMemObjectDestructorCallback().
     *
     *  Repeated calls to this function, for a given cl_mem value, will append
     *  to the list of functions called (in reverse order) when memory object's
     *  resources are freed and the memory object is deleted.
     *
     *  \note
     *  The registered callbacks are associated with the underlying cl_mem
     *  value - not the Memory class instance.
     */
    cl_int setDestructorCallback(
        void (CL_CALLBACK * pfn_notify)(cl_mem, void *),
        void * user_data = NULL)
    {
        return detail::errHandler(
            ::clSetMemObjectDestructorCallback(
                object_,
                pfn_notify,
                user_data), 
            __SET_MEM_OBJECT_DESTRUCTOR_CALLBACK_ERR);
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 110

};

// Pre-declare copy functions
class Buffer;
template&lt; typename IteratorType &gt;
cl_int copy( IteratorType startIterator, IteratorType endIterator, cl::Buffer &amp;buffer );
template&lt; typename IteratorType &gt;
cl_int copy( const cl::Buffer &amp;buffer, IteratorType startIterator, IteratorType endIterator );
template&lt; typename IteratorType &gt;
cl_int copy( const CommandQueue &amp;queue, IteratorType startIterator, IteratorType endIterator, cl::Buffer &amp;buffer );
template&lt; typename IteratorType &gt;
cl_int copy( const CommandQueue &amp;queue, const cl::Buffer &amp;buffer, IteratorType startIterator, IteratorType endIterator );


#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
namespace detail
{
    class SVMTraitNull
    {
    public:
        static cl_svm_mem_flags getSVMMemFlags()
        {
            return 0;
        }
    };
} // namespace detail

template&lt;class Trait = detail::SVMTraitNull&gt;
class SVMTraitReadWrite
{
public:
    static cl_svm_mem_flags getSVMMemFlags()
    {
        return CL_MEM_READ_WRITE |
            Trait::getSVMMemFlags();
    }
};

template&lt;class Trait = detail::SVMTraitNull&gt;
class SVMTraitReadOnly
{
public:
    static cl_svm_mem_flags getSVMMemFlags()
    {
        return CL_MEM_READ_ONLY |
            Trait::getSVMMemFlags();
    }
};

template&lt;class Trait = detail::SVMTraitNull&gt;
class SVMTraitWriteOnly
{
public:
    static cl_svm_mem_flags getSVMMemFlags()
    {
        return CL_MEM_WRITE_ONLY |
            Trait::getSVMMemFlags();
    }
};

template&lt;class Trait = SVMTraitReadWrite&lt;&gt;&gt;
class SVMTraitCoarse
{
public:
    static cl_svm_mem_flags getSVMMemFlags()
    {
        return Trait::getSVMMemFlags();
    }
};

template&lt;class Trait = SVMTraitReadWrite&lt;&gt;&gt;
class SVMTraitFine
{
public:
    static cl_svm_mem_flags getSVMMemFlags()
    {
        return CL_MEM_SVM_FINE_GRAIN_BUFFER |
            Trait::getSVMMemFlags();
    }
};

template&lt;class Trait = SVMTraitReadWrite&lt;&gt;&gt;
class SVMTraitAtomic
{
public:
    static cl_svm_mem_flags getSVMMemFlags()
    {
        return
            CL_MEM_SVM_FINE_GRAIN_BUFFER |
            CL_MEM_SVM_ATOMICS |
            Trait::getSVMMemFlags();
    }
};

// Pre-declare SVM map function
template&lt;typename T&gt;
inline cl_int enqueueMapSVM(
    T* ptr,
    cl_bool blocking,
    cl_map_flags flags,
    size_type size,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL);

/**
 * STL-like allocator class for managing SVM objects provided for convenience.
 *
 * Note that while this behaves like an allocator for the purposes of constructing vectors and similar objects,
 * care must be taken when using with smart pointers.
 * The allocator should not be used to construct a unique_ptr if we are using coarse-grained SVM mode because
 * the coarse-grained management behaviour would behave incorrectly with respect to reference counting.
 *
 * Instead the allocator embeds a Deleter which may be used with unique_ptr and is used
 * with the allocate_shared and allocate_ptr supplied operations.
 */
template&lt;typename T, class SVMTrait&gt;
class SVMAllocator {
private:
    Context context_;

public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type&amp; reference;
    typedef const value_type&amp; const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    template&lt;typename U&gt;
    struct rebind
    {
        typedef SVMAllocator&lt;U, SVMTrait&gt; other;
    };

    template&lt;typename U, typename V&gt;
    friend class SVMAllocator;

    SVMAllocator() :
        context_(Context::getDefault())
    {
    }

    explicit SVMAllocator(cl::Context context) :
        context_(context)
    {
    }


    SVMAllocator(const SVMAllocator &amp;other) :
        context_(other.context_)
    {
    }

    template&lt;typename U&gt;
    SVMAllocator(const SVMAllocator&lt;U, SVMTrait&gt; &amp;other) :
        context_(other.context_)
    {
    }

    ~SVMAllocator()
    {
    }

    pointer address(reference r) CL_HPP_NOEXCEPT_
    {
        return std::addressof(r);
    }

    const_pointer address(const_reference r) CL_HPP_NOEXCEPT_
    {
        return std::addressof(r);
    }

    /**
     * Allocate an SVM pointer.
     *
     * If the allocator is coarse-grained, this will take ownership to allow
     * containers to correctly construct data in place. 
     */
    pointer allocate(
        size_type size,
        typename cl::SVMAllocator&lt;void, SVMTrait&gt;::const_pointer = 0)
    {
        // Allocate memory with default alignment matching the size of the type
        void* voidPointer =
            clSVMAlloc(
            context_(),
            SVMTrait::getSVMMemFlags(),
            size*sizeof(T),
            0);
        pointer retValue = reinterpret_cast&lt;pointer&gt;(
            voidPointer);
#if defined(CL_HPP_ENABLE_EXCEPTIONS)
        if (!retValue) {
            std::bad_alloc excep;
            throw excep;
        }
#endif // #if defined(CL_HPP_ENABLE_EXCEPTIONS)

        // If allocation was coarse-grained then map it
        if (!(SVMTrait::getSVMMemFlags() &amp; CL_MEM_SVM_FINE_GRAIN_BUFFER)) {
            cl_int err = enqueueMapSVM(retValue, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, size*sizeof(T));
            if (err != CL_SUCCESS) {
                std::bad_alloc excep;
                throw excep;
            }
        }

        // If exceptions disabled, return null pointer from allocator
        return retValue;
    }

    void deallocate(pointer p, size_type)
    {
        clSVMFree(context_(), p);
    }

    /**
     * Return the maximum possible allocation size.
     * This is the minimum of the maximum sizes of all devices in the context.
     */
    size_type max_size() const CL_HPP_NOEXCEPT_
    {
        size_type maxSize = std::numeric_limits&lt;size_type&gt;::max() / sizeof(T);

        for (const Device &amp;d : context_.getInfo&lt;CL_CONTEXT_DEVICES&gt;()) {
            maxSize = std::min(
                maxSize, 
                static_cast&lt;size_type&gt;(d.getInfo&lt;CL_DEVICE_MAX_MEM_ALLOC_SIZE&gt;()));
        }

        return maxSize;
    }

    template&lt; class U, class... Args &gt;
    void construct(U* p, Args&amp;&amp;... args)
    {
        new(p)T(args...);
    }

    template&lt; class U &gt;
    void destroy(U* p)
    {
        p-&gt;~U();
    }

    /**
     * Returns true if the contexts match.
     */
    inline bool operator==(SVMAllocator const&amp; rhs)
    {
        return (context_==rhs.context_);
    }

    inline bool operator!=(SVMAllocator const&amp; a)
    {
        return !operator==(a);
    }
}; // class SVMAllocator        return cl::pointer&lt;T&gt;(tmp, detail::Deleter&lt;T, Alloc&gt;{alloc, copies});


template&lt;class SVMTrait&gt;
class SVMAllocator&lt;void, SVMTrait&gt; {
public:
    typedef void value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    template&lt;typename U&gt;
    struct rebind
    {
        typedef SVMAllocator&lt;U, SVMTrait&gt; other;
    };

    template&lt;typename U, typename V&gt;
    friend class SVMAllocator;
};

#if !defined(CL_HPP_NO_STD_UNIQUE_PTR)
namespace detail
{
    template&lt;class Alloc&gt;
    class Deleter {
    private:
        Alloc alloc_;
        size_type copies_;

    public:
        typedef typename std::allocator_traits&lt;Alloc&gt;::pointer pointer;

        Deleter(const Alloc &amp;alloc, size_type copies) : alloc_{ alloc }, copies_{ copies }
        {
        }

        void operator()(pointer ptr) const {
            Alloc tmpAlloc{ alloc_ };
            std::allocator_traits&lt;Alloc&gt;::destroy(tmpAlloc, std::addressof(*ptr));
            std::allocator_traits&lt;Alloc&gt;::deallocate(tmpAlloc, ptr, copies_);
        }
    };
} // namespace detail

/**
 * Allocation operation compatible with std::allocate_ptr.
 * Creates a unique_ptr&lt;T&gt; by default.
 * This requirement is to ensure that the control block is not
 * allocated in memory inaccessible to the host.
 */
template &lt;class T, class Alloc, class... Args&gt;
cl::pointer&lt;T, detail::Deleter&lt;Alloc&gt;&gt; allocate_pointer(const Alloc &amp;alloc_, Args&amp;&amp;... args)
{
    Alloc alloc(alloc_);
    static const size_type copies = 1;

    // Ensure that creation of the management block and the
    // object are dealt with separately such that we only provide a deleter

    T* tmp = std::allocator_traits&lt;Alloc&gt;::allocate(alloc, copies);
    if (!tmp) {
        std::bad_alloc excep;
        throw excep;
    }
    try {
        std::allocator_traits&lt;Alloc&gt;::construct(
            alloc,
            std::addressof(*tmp),
            std::forward&lt;Args&gt;(args)...);

        return cl::pointer&lt;T, detail::Deleter&lt;Alloc&gt;&gt;(tmp, detail::Deleter&lt;Alloc&gt;{alloc, copies});
    }
    catch (std::bad_alloc&amp; b)
    {
        std::allocator_traits&lt;Alloc&gt;::deallocate(alloc, tmp, copies);
        throw;
    }
}

template&lt; class T, class SVMTrait, class... Args &gt;
cl::pointer&lt;T, detail::Deleter&lt;SVMAllocator&lt;T, SVMTrait&gt;&gt;&gt; allocate_svm(Args... args)
{
    SVMAllocator&lt;T, SVMTrait&gt; alloc;
    return cl::allocate_pointer&lt;T&gt;(alloc, args...);
}

template&lt; class T, class SVMTrait, class... Args &gt;
cl::pointer&lt;T, detail::Deleter&lt;SVMAllocator&lt;T, SVMTrait&gt;&gt;&gt; allocate_svm(const cl::Context &amp;c, Args... args)
{
    SVMAllocator&lt;T, SVMTrait&gt; alloc(c);
    return cl::allocate_pointer&lt;T&gt;(alloc, args...);
}
#endif // #if !defined(CL_HPP_NO_STD_UNIQUE_PTR)

/*! \brief Vector alias to simplify contruction of coarse-grained SVM containers.
 * 
 */
template &lt; class T &gt;
using coarse_svm_vector = vector&lt;T, cl::SVMAllocator&lt;int, cl::SVMTraitCoarse&lt;&gt;&gt;&gt;;

/*! \brief Vector alias to simplify contruction of fine-grained SVM containers.
*
*/
template &lt; class T &gt;
using fine_svm_vector = vector&lt;T, cl::SVMAllocator&lt;int, cl::SVMTraitFine&lt;&gt;&gt;&gt;;

/*! \brief Vector alias to simplify contruction of fine-grained SVM containers that support platform atomics.
*
*/
template &lt; class T &gt;
using atomic_svm_vector = vector&lt;T, cl::SVMAllocator&lt;int, cl::SVMTraitAtomic&lt;&gt;&gt;&gt;;

#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200


/*! \brief Class interface for Buffer Memory Objects.
 * 
 *  See Memory for details about copy semantics, etc.
 *
 *  \see Memory
 */
class Buffer : public Memory
{
public:

    /*! \brief Constructs a Buffer in a specified context.
     *
     *  Wraps clCreateBuffer().
     *
     *  \param host_ptr Storage to be used if the CL_MEM_USE_HOST_PTR flag was
     *                  specified.  Note alignment &amp; exclusivity requirements.
     */
    Buffer(
        const Context&amp; context,
        cl_mem_flags flags,
        size_type size,
        void* host_ptr = NULL,
        cl_int* err = NULL)
    {
        cl_int error;
        object_ = ::clCreateBuffer(context(), flags, size, host_ptr, &amp;error);

        detail::errHandler(error, __CREATE_BUFFER_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    /*! \brief Constructs a Buffer in the default context.
     *
     *  Wraps clCreateBuffer().
     *
     *  \param host_ptr Storage to be used if the CL_MEM_USE_HOST_PTR flag was
     *                  specified.  Note alignment &amp; exclusivity requirements.
     *
     *  \see Context::getDefault()
     */
    Buffer(
         cl_mem_flags flags,
        size_type size,
        void* host_ptr = NULL,
        cl_int* err = NULL)
    {
        cl_int error;

        Context context = Context::getDefault(err);

        object_ = ::clCreateBuffer(context(), flags, size, host_ptr, &amp;error);

        detail::errHandler(error, __CREATE_BUFFER_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    /*!
     * \brief Construct a Buffer from a host container via iterators.
     * IteratorType must be random access.
     * If useHostPtr is specified iterators must represent contiguous data.
     */
    template&lt; typename IteratorType &gt;
    Buffer(
        IteratorType startIterator,
        IteratorType endIterator,
        bool readOnly,
        bool useHostPtr = false,
        cl_int* err = NULL)
    {
        typedef typename std::iterator_traits&lt;IteratorType&gt;::value_type DataType;
        cl_int error;

        cl_mem_flags flags = 0;
        if( readOnly ) {
            flags |= CL_MEM_READ_ONLY;
        }
        else {
            flags |= CL_MEM_READ_WRITE;
        }
        if( useHostPtr ) {
            flags |= CL_MEM_USE_HOST_PTR;
        }
        
        size_type size = sizeof(DataType)*(endIterator - startIterator);

        Context context = Context::getDefault(err);

        if( useHostPtr ) {
            object_ = ::clCreateBuffer(context(), flags, size, static_cast&lt;DataType*&gt;(&amp;*startIterator), &amp;error);
        } else {
            object_ = ::clCreateBuffer(context(), flags, size, 0, &amp;error);
        }

        detail::errHandler(error, __CREATE_BUFFER_ERR);
        if (err != NULL) {
            *err = error;
        }

        if( !useHostPtr ) {
            error = cl::copy(startIterator, endIterator, *this);
            detail::errHandler(error, __CREATE_BUFFER_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
    }

    /*!
     * \brief Construct a Buffer from a host container via iterators using a specified context.
     * IteratorType must be random access.
     * If useHostPtr is specified iterators must represent contiguous data.
     */
    template&lt; typename IteratorType &gt;
    Buffer(const Context &amp;context, IteratorType startIterator, IteratorType endIterator,
        bool readOnly, bool useHostPtr = false, cl_int* err = NULL);
    
    /*!
    * \brief Construct a Buffer from a host container via iterators using a specified queue.
    * If useHostPtr is specified iterators must be random access.
    */
    template&lt; typename IteratorType &gt;
    Buffer(const CommandQueue &amp;queue, IteratorType startIterator, IteratorType endIterator,
        bool readOnly, bool useHostPtr = false, cl_int* err = NULL);

    //! \brief Default constructor - initializes to NULL.
    Buffer() : Memory() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with earlier versions.
     *
     *  See Memory for further details.
     */
    explicit Buffer(const cl_mem&amp; buffer, bool retainObject = false) :
        Memory(buffer, retainObject) { }

    /*! \brief Assignment from cl_mem - performs shallow copy.
    *
    *  See Memory for further details.
    */
    Buffer&amp; operator = (const cl_mem&amp; rhs)
    {
        Memory::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Buffer(const Buffer&amp; buf) : Memory(buf) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Buffer&amp; operator = (const Buffer &amp;buf)
    {
        Memory::operator=(buf);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Buffer(Buffer&amp;&amp; buf) CL_HPP_NOEXCEPT_ : Memory(std::move(buf)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Buffer&amp; operator = (Buffer &amp;&amp;buf)
    {
        Memory::operator=(std::move(buf));
        return *this;
    }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
    /*! \brief Creates a new buffer object from this.
     *
     *  Wraps clCreateSubBuffer().
     */
    Buffer createSubBuffer(
        cl_mem_flags flags,
        cl_buffer_create_type buffer_create_type,
        const void * buffer_create_info,
        cl_int * err = NULL)
    {
        Buffer result;
        cl_int error;
        result.object_ = ::clCreateSubBuffer(
            object_, 
            flags, 
            buffer_create_type, 
            buffer_create_info, 
            &amp;error);

        detail::errHandler(error, __CREATE_SUBBUFFER_ERR);
        if (err != NULL) {
            *err = error;
        }

        return result;
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
};

#if defined (CL_HPP_USE_DX_INTEROP)
/*! \brief Class interface for creating OpenCL buffers from ID3D10Buffer's.
 *
 *  This is provided to facilitate interoperability with Direct3D.
 * 
 *  See Memory for details about copy semantics, etc.
 *
 *  \see Memory
 */
class BufferD3D10 : public Buffer
{
public:
   

    /*! \brief Constructs a BufferD3D10, in a specified context, from a
     *         given ID3D10Buffer.
     *
     *  Wraps clCreateFromD3D10BufferKHR().
     */
    BufferD3D10(
        const Context&amp; context,
        cl_mem_flags flags,
        ID3D10Buffer* bufobj,
        cl_int * err = NULL) : pfn_clCreateFromD3D10BufferKHR(nullptr)
    {
        typedef CL_API_ENTRY cl_mem (CL_API_CALL *PFN_clCreateFromD3D10BufferKHR)(
            cl_context context, cl_mem_flags flags, ID3D10Buffer*  buffer,
            cl_int* errcode_ret);
        PFN_clCreateFromD3D10BufferKHR pfn_clCreateFromD3D10BufferKHR;
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
        vector&lt;cl_context_properties&gt; props = context.getInfo&lt;CL_CONTEXT_PROPERTIES&gt;();
        cl_platform platform = -1;
        for( int i = 0; i &lt; props.size(); ++i ) {
            if( props[i] == CL_CONTEXT_PLATFORM ) {
                platform = props[i+1];
            }
        }
        CL_HPP_INIT_CL_EXT_FCN_PTR_PLATFORM_(platform, clCreateFromD3D10BufferKHR);
#elif CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
        CL_HPP_INIT_CL_EXT_FCN_PTR_(clCreateFromD3D10BufferKHR);
#endif

        cl_int error;
        object_ = pfn_clCreateFromD3D10BufferKHR(
            context(),
            flags,
            bufobj,
            &amp;error);

        detail::errHandler(error, __CREATE_GL_BUFFER_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    //! \brief Default constructor - initializes to NULL.
    BufferD3D10() : Buffer() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with 
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit BufferD3D10(const cl_mem&amp; buffer, bool retainObject = false) : 
        Buffer(buffer, retainObject) { }

    /*! \brief Assignment from cl_mem - performs shallow copy.
     *
     *  See Memory for further details.
     */
    BufferD3D10&amp; operator = (const cl_mem&amp; rhs)
    {
        Buffer::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    BufferD3D10(const BufferD3D10&amp; buf) : 
        Buffer(buf) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    BufferD3D10&amp; operator = (const BufferD3D10 &amp;buf)
    {
        Buffer::operator=(buf);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    BufferD3D10(BufferD3D10&amp;&amp; buf) CL_HPP_NOEXCEPT_ : Buffer(std::move(buf)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    BufferD3D10&amp; operator = (BufferD3D10 &amp;&amp;buf)
    {
        Buffer::operator=(std::move(buf));
        return *this;
    }
};
#endif

/*! \brief Class interface for GL Buffer Memory Objects.
 *
 *  This is provided to facilitate interoperability with OpenGL.
 * 
 *  See Memory for details about copy semantics, etc.
 * 
 *  \see Memory
 */
class BufferGL : public Buffer
{
public:
    /*! \brief Constructs a BufferGL in a specified context, from a given
     *         GL buffer.
     *
     *  Wraps clCreateFromGLBuffer().
     */
    BufferGL(
        const Context&amp; context,
        cl_mem_flags flags,
        cl_GLuint bufobj,
        cl_int * err = NULL)
    {
        cl_int error;
        object_ = ::clCreateFromGLBuffer(
            context(),
            flags,
            bufobj,
            &amp;error);

        detail::errHandler(error, __CREATE_GL_BUFFER_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    //! \brief Default constructor - initializes to NULL.
    BufferGL() : Buffer() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit BufferGL(const cl_mem&amp; buffer, bool retainObject = false) :
        Buffer(buffer, retainObject) { }

    /*! \brief Assignment from cl_mem - performs shallow copy.
     *
     *  See Memory for further details.
     */
    BufferGL&amp; operator = (const cl_mem&amp; rhs)
    {
        Buffer::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    BufferGL(const BufferGL&amp; buf) : Buffer(buf) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    BufferGL&amp; operator = (const BufferGL &amp;buf)
    {
        Buffer::operator=(buf);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    BufferGL(BufferGL&amp;&amp; buf) CL_HPP_NOEXCEPT_ : Buffer(std::move(buf)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    BufferGL&amp; operator = (BufferGL &amp;&amp;buf)
    {
        Buffer::operator=(std::move(buf));
        return *this;
    }

    //! \brief Wrapper for clGetGLObjectInfo().
    cl_int getObjectInfo(
        cl_gl_object_type *type,
        cl_GLuint * gl_object_name)
    {
        return detail::errHandler(
            ::clGetGLObjectInfo(object_,type,gl_object_name),
            __GET_GL_OBJECT_INFO_ERR);
    }
};

/*! \brief Class interface for GL Render Buffer Memory Objects.
 *
 *  This is provided to facilitate interoperability with OpenGL.
 * 
 *  See Memory for details about copy semantics, etc.
 * 
 *  \see Memory
 */
class BufferRenderGL : public Buffer
{
public:
    /*! \brief Constructs a BufferRenderGL in a specified context, from a given
     *         GL Renderbuffer.
     *
     *  Wraps clCreateFromGLRenderbuffer().
     */
    BufferRenderGL(
        const Context&amp; context,
        cl_mem_flags flags,
        cl_GLuint bufobj,
        cl_int * err = NULL)
    {
        cl_int error;
        object_ = ::clCreateFromGLRenderbuffer(
            context(),
            flags,
            bufobj,
            &amp;error);

        detail::errHandler(error, __CREATE_GL_RENDER_BUFFER_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    //! \brief Default constructor - initializes to NULL.
    BufferRenderGL() : Buffer() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with 
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit BufferRenderGL(const cl_mem&amp; buffer, bool retainObject = false) :
        Buffer(buffer, retainObject) { }

    /*! \brief Assignment from cl_mem - performs shallow copy.
     *
     *  See Memory for further details.
     */
    BufferRenderGL&amp; operator = (const cl_mem&amp; rhs)
    {
        Buffer::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    BufferRenderGL(const BufferRenderGL&amp; buf) : Buffer(buf) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    BufferRenderGL&amp; operator = (const BufferRenderGL &amp;buf)
    {
        Buffer::operator=(buf);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    BufferRenderGL(BufferRenderGL&amp;&amp; buf) CL_HPP_NOEXCEPT_ : Buffer(std::move(buf)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    BufferRenderGL&amp; operator = (BufferRenderGL &amp;&amp;buf)
    {
        Buffer::operator=(std::move(buf));
        return *this;
    }

    //! \brief Wrapper for clGetGLObjectInfo().
    cl_int getObjectInfo(
        cl_gl_object_type *type,
        cl_GLuint * gl_object_name)
    {
        return detail::errHandler(
            ::clGetGLObjectInfo(object_,type,gl_object_name),
            __GET_GL_OBJECT_INFO_ERR);
    }
};

/*! \brief C++ base class for Image Memory objects.
 *
 *  See Memory for details about copy semantics, etc.
 * 
 *  \see Memory
 */
class Image : public Memory
{
protected:
    //! \brief Default constructor - initializes to NULL.
    Image() : Memory() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit Image(const cl_mem&amp; image, bool retainObject = false) :
        Memory(image, retainObject) { }

    /*! \brief Assignment from cl_mem - performs shallow copy.
     *
     *  See Memory for further details.
     */
    Image&amp; operator = (const cl_mem&amp; rhs)
    {
        Memory::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image(const Image&amp; img) : Memory(img) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image&amp; operator = (const Image &amp;img)
    {
        Memory::operator=(img);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image(Image&amp;&amp; img) CL_HPP_NOEXCEPT_ : Memory(std::move(img)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image&amp; operator = (Image &amp;&amp;img)
    {
        Memory::operator=(std::move(img));
        return *this;
    }


public:
    //! \brief Wrapper for clGetImageInfo().
    template &lt;typename T&gt;
    cl_int getImageInfo(cl_image_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(&amp;::clGetImageInfo, object_, name, param),
            __GET_IMAGE_INFO_ERR);
    }
    
    //! \brief Wrapper for clGetImageInfo() that returns by value.
    template &lt;cl_image_info name&gt; typename
    detail::param_traits&lt;detail::cl_image_info, name&gt;::param_type
    getImageInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_image_info, name&gt;::param_type param;
        cl_int result = getImageInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }
};

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
/*! \brief Class interface for 1D Image Memory objects.
 *
 *  See Memory for details about copy semantics, etc.
 * 
 *  \see Memory
 */
class Image1D : public Image
{
public:
    /*! \brief Constructs a 1D Image in a specified context.
     *
     *  Wraps clCreateImage().
     */
    Image1D(
        const Context&amp; context,
        cl_mem_flags flags,
        ImageFormat format,
        size_type width,
        void* host_ptr = NULL,
        cl_int* err = NULL)
    {
        cl_int error;
        cl_image_desc desc =
        {
            CL_MEM_OBJECT_IMAGE1D,
            width,
            0, 0, 0, 0, 0, 0, 0, 0
        };
        object_ = ::clCreateImage(
            context(), 
            flags, 
            &amp;format, 
            &amp;desc, 
            host_ptr, 
            &amp;error);

        detail::errHandler(error, __CREATE_IMAGE_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    //! \brief Default constructor - initializes to NULL.
    Image1D() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit Image1D(const cl_mem&amp; image1D, bool retainObject = false) :
        Image(image1D, retainObject) { }

    /*! \brief Assignment from cl_mem - performs shallow copy.
     *
     *  See Memory for further details.
     */
    Image1D&amp; operator = (const cl_mem&amp; rhs)
    {
        Image::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image1D(const Image1D&amp; img) : Image(img) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image1D&amp; operator = (const Image1D &amp;img)
    {
        Image::operator=(img);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image1D(Image1D&amp;&amp; img) CL_HPP_NOEXCEPT_ : Image(std::move(img)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image1D&amp; operator = (Image1D &amp;&amp;img)
    {
        Image::operator=(std::move(img));
        return *this;
    }

};

/*! \class Image1DBuffer
 * \brief Image interface for 1D buffer images.
 */
class Image1DBuffer : public Image
{
public:
    Image1DBuffer(
        const Context&amp; context,
        cl_mem_flags flags,
        ImageFormat format,
        size_type width,
        const Buffer &amp;buffer,
        cl_int* err = NULL)
    {
        cl_int error;
        cl_image_desc desc =
        {
            CL_MEM_OBJECT_IMAGE1D_BUFFER,
            width,
            0, 0, 0, 0, 0, 0, 0,
            buffer()
        };
        object_ = ::clCreateImage(
            context(), 
            flags, 
            &amp;format, 
            &amp;desc, 
            NULL, 
            &amp;error);

        detail::errHandler(error, __CREATE_IMAGE_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    Image1DBuffer() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit Image1DBuffer(const cl_mem&amp; image1D, bool retainObject = false) :
        Image(image1D, retainObject) { }

    Image1DBuffer&amp; operator = (const cl_mem&amp; rhs)
    {
        Image::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image1DBuffer(const Image1DBuffer&amp; img) : Image(img) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image1DBuffer&amp; operator = (const Image1DBuffer &amp;img)
    {
        Image::operator=(img);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image1DBuffer(Image1DBuffer&amp;&amp; img) CL_HPP_NOEXCEPT_ : Image(std::move(img)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image1DBuffer&amp; operator = (Image1DBuffer &amp;&amp;img)
    {
        Image::operator=(std::move(img));
        return *this;
    }

};

/*! \class Image1DArray
 * \brief Image interface for arrays of 1D images.
 */
class Image1DArray : public Image
{
public:
    Image1DArray(
        const Context&amp; context,
        cl_mem_flags flags,
        ImageFormat format,
        size_type arraySize,
        size_type width,
        size_type rowPitch,
        void* host_ptr = NULL,
        cl_int* err = NULL)
    {
        cl_int error;
        cl_image_desc desc =
        {
            CL_MEM_OBJECT_IMAGE1D_ARRAY,
            width,
            0, 0,  // height, depth (unused)
            arraySize,
            rowPitch,
            0, 0, 0, 0
        };
        object_ = ::clCreateImage(
            context(), 
            flags, 
            &amp;format, 
            &amp;desc, 
            host_ptr, 
            &amp;error);

        detail::errHandler(error, __CREATE_IMAGE_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    Image1DArray() { }
  
    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit Image1DArray(const cl_mem&amp; imageArray, bool retainObject = false) :
        Image(imageArray, retainObject) { }


    Image1DArray&amp; operator = (const cl_mem&amp; rhs)
    {
        Image::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image1DArray(const Image1DArray&amp; img) : Image(img) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image1DArray&amp; operator = (const Image1DArray &amp;img)
    {
        Image::operator=(img);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image1DArray(Image1DArray&amp;&amp; img) CL_HPP_NOEXCEPT_ : Image(std::move(img)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image1DArray&amp; operator = (Image1DArray &amp;&amp;img)
    {
        Image::operator=(std::move(img));
        return *this;
    }

};
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120


/*! \brief Class interface for 2D Image Memory objects.
 *
 *  See Memory for details about copy semantics, etc.
 * 
 *  \see Memory
 */
class Image2D : public Image
{
public:
    /*! \brief Constructs a 2D Image in a specified context.
     *
     *  Wraps clCreateImage().
     */
    Image2D(
        const Context&amp; context,
        cl_mem_flags flags,
        ImageFormat format,
        size_type width,
        size_type height,
        size_type row_pitch = 0,
        void* host_ptr = NULL,
        cl_int* err = NULL)
    {
        cl_int error;
        bool useCreateImage;

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120
        // Run-time decision based on the actual platform
        {
            cl_uint version = detail::getContextPlatformVersion(context());
            useCreateImage = (version &gt;= 0x10002); // OpenCL 1.2 or above
        }
#elif CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
        useCreateImage = true;
#else
        useCreateImage = false;
#endif

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
        if (useCreateImage)
        {
            cl_image_desc desc =
            {
                CL_MEM_OBJECT_IMAGE2D,
                width,
                height,
                0, 0, // depth, array size (unused)
                row_pitch,
                0, 0, 0, 0
            };
            object_ = ::clCreateImage(
                context(),
                flags,
                &amp;format,
                &amp;desc,
                host_ptr,
                &amp;error);

            detail::errHandler(error, __CREATE_IMAGE_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120
        if (!useCreateImage)
        {
            object_ = ::clCreateImage2D(
                context(), flags,&amp;format, width, height, row_pitch, host_ptr, &amp;error);

            detail::errHandler(error, __CREATE_IMAGE2D_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120
    }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200 || defined(CL_HPP_USE_CL_IMAGE2D_FROM_BUFFER_KHR)
    /*! \brief Constructs a 2D Image from a buffer.
    * \note This will share storage with the underlying buffer.
    *
    *  Wraps clCreateImage().
    */
    Image2D(
        const Context&amp; context,
        ImageFormat format,
        const Buffer &amp;sourceBuffer,
        size_type width,
        size_type height,
        size_type row_pitch = 0,
        cl_int* err = nullptr)
    {
        cl_int error;

        cl_image_desc desc =
        {
            CL_MEM_OBJECT_IMAGE2D,
            width,
            height,
            0, 0, // depth, array size (unused)
            row_pitch,
            0, 0, 0,
            // Use buffer as input to image
            sourceBuffer()
        };
        object_ = ::clCreateImage(
            context(),
            0, // flags inherited from buffer
            &amp;format,
            &amp;desc,
            nullptr,
            &amp;error);

        detail::errHandler(error, __CREATE_IMAGE_ERR);
        if (err != nullptr) {
            *err = error;
        }
    }
#endif //#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200 || defined(CL_HPP_USE_CL_IMAGE2D_FROM_BUFFER_KHR)

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
    /*! \brief Constructs a 2D Image from an image.
    * \note This will share storage with the underlying image but may
    *       reinterpret the channel order and type.
    *
    * The image will be created matching with a descriptor matching the source. 
    *
    * \param order is the channel order to reinterpret the image data as.
    *              The channel order may differ as described in the OpenCL 
    *              2.0 API specification.
    *
    * Wraps clCreateImage().
    */
    Image2D(
        const Context&amp; context,
        cl_channel_order order,
        const Image &amp;sourceImage,
        cl_int* err = nullptr)
    {
        cl_int error;

        // Descriptor fields have to match source image
        size_type sourceWidth = 
            sourceImage.getImageInfo&lt;CL_IMAGE_WIDTH&gt;();
        size_type sourceHeight = 
            sourceImage.getImageInfo&lt;CL_IMAGE_HEIGHT&gt;();
        size_type sourceRowPitch =
            sourceImage.getImageInfo&lt;CL_IMAGE_ROW_PITCH&gt;();
        cl_uint sourceNumMIPLevels =
            sourceImage.getImageInfo&lt;CL_IMAGE_NUM_MIP_LEVELS&gt;();
        cl_uint sourceNumSamples =
            sourceImage.getImageInfo&lt;CL_IMAGE_NUM_SAMPLES&gt;();
        cl_image_format sourceFormat =
            sourceImage.getImageInfo&lt;CL_IMAGE_FORMAT&gt;();

        // Update only the channel order. 
        // Channel format inherited from source.
        sourceFormat.image_channel_order = order;
        cl_image_desc desc =
        {
            CL_MEM_OBJECT_IMAGE2D,
            sourceWidth,
            sourceHeight,
            0, 0, // depth (unused), array size (unused)
            sourceRowPitch,
            0, // slice pitch (unused)
            sourceNumMIPLevels,
            sourceNumSamples,
            // Use buffer as input to image
            sourceImage()
        };
        object_ = ::clCreateImage(
            context(),
            0, // flags should be inherited from mem_object
            &amp;sourceFormat,
            &amp;desc,
            nullptr,
            &amp;error);

        detail::errHandler(error, __CREATE_IMAGE_ERR);
        if (err != nullptr) {
            *err = error;
        }
    }
#endif //#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200

    //! \brief Default constructor - initializes to NULL.
    Image2D() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit Image2D(const cl_mem&amp; image2D, bool retainObject = false) :
        Image(image2D, retainObject) { }

    /*! \brief Assignment from cl_mem - performs shallow copy.
     *
     *  See Memory for further details.
     */
    Image2D&amp; operator = (const cl_mem&amp; rhs)
    {
        Image::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image2D(const Image2D&amp; img) : Image(img) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image2D&amp; operator = (const Image2D &amp;img)
    {
        Image::operator=(img);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image2D(Image2D&amp;&amp; img) CL_HPP_NOEXCEPT_ : Image(std::move(img)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image2D&amp; operator = (Image2D &amp;&amp;img)
    {
        Image::operator=(std::move(img));
        return *this;
    }

};


#if defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
/*! \brief Class interface for GL 2D Image Memory objects.
 *
 *  This is provided to facilitate interoperability with OpenGL.
 * 
 *  See Memory for details about copy semantics, etc.
 * 
 *  \see Memory
 *  \note Deprecated for OpenCL 1.2. Please use ImageGL instead.
 */
class CL_EXT_PREFIX__VERSION_1_1_DEPRECATED Image2DGL : public Image2D 
{
public:
    /*! \brief Constructs an Image2DGL in a specified context, from a given
     *         GL Texture.
     *
     *  Wraps clCreateFromGLTexture2D().
     */
    Image2DGL(
        const Context&amp; context,
        cl_mem_flags flags,
        cl_GLenum target,
        cl_GLint  miplevel,
        cl_GLuint texobj,
        cl_int * err = NULL)
    {
        cl_int error;
        object_ = ::clCreateFromGLTexture2D(
            context(),
            flags,
            target,
            miplevel,
            texobj,
            &amp;error);

        detail::errHandler(error, __CREATE_GL_TEXTURE_2D_ERR);
        if (err != NULL) {
            *err = error;
        }

    }
    
    //! \brief Default constructor - initializes to NULL.
    Image2DGL() : Image2D() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit Image2DGL(const cl_mem&amp; image, bool retainObject = false) : 
        Image2D(image, retainObject) { }

    /*! \brief Assignment from cl_mem - performs shallow copy.
     *c
     *  See Memory for further details.
     */
    Image2DGL&amp; operator = (const cl_mem&amp; rhs)
    {
        Image2D::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image2DGL(const Image2DGL&amp; img) : Image2D(img) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image2DGL&amp; operator = (const Image2DGL &amp;img)
    {
        Image2D::operator=(img);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image2DGL(Image2DGL&amp;&amp; img) CL_HPP_NOEXCEPT_ : Image2D(std::move(img)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image2DGL&amp; operator = (Image2DGL &amp;&amp;img)
    {
        Image2D::operator=(std::move(img));
        return *this;
    }

} CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED;
#endif // CL_USE_DEPRECATED_OPENCL_1_1_APIS

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
/*! \class Image2DArray
 * \brief Image interface for arrays of 2D images.
 */
class Image2DArray : public Image
{
public:
    Image2DArray(
        const Context&amp; context,
        cl_mem_flags flags,
        ImageFormat format,
        size_type arraySize,
        size_type width,
        size_type height,
        size_type rowPitch,
        size_type slicePitch,
        void* host_ptr = NULL,
        cl_int* err = NULL)
    {
        cl_int error;
        cl_image_desc desc =
        {
            CL_MEM_OBJECT_IMAGE2D_ARRAY,
            width,
            height,
            0,       // depth (unused)
            arraySize,
            rowPitch,
            slicePitch,
            0, 0, 0
        };
        object_ = ::clCreateImage(
            context(), 
            flags, 
            &amp;format, 
            &amp;desc, 
            host_ptr, 
            &amp;error);

        detail::errHandler(error, __CREATE_IMAGE_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    Image2DArray() { }
    
    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit Image2DArray(const cl_mem&amp; imageArray, bool retainObject = false) : Image(imageArray, retainObject) { }

    Image2DArray&amp; operator = (const cl_mem&amp; rhs)
    {
        Image::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image2DArray(const Image2DArray&amp; img) : Image(img) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image2DArray&amp; operator = (const Image2DArray &amp;img)
    {
        Image::operator=(img);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image2DArray(Image2DArray&amp;&amp; img) CL_HPP_NOEXCEPT_ : Image(std::move(img)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image2DArray&amp; operator = (Image2DArray &amp;&amp;img)
    {
        Image::operator=(std::move(img));
        return *this;
    }
};
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120

/*! \brief Class interface for 3D Image Memory objects.
 *
 *  See Memory for details about copy semantics, etc.
 * 
 *  \see Memory
 */
class Image3D : public Image
{
public:
    /*! \brief Constructs a 3D Image in a specified context.
     *
     *  Wraps clCreateImage().
     */
    Image3D(
        const Context&amp; context,
        cl_mem_flags flags,
        ImageFormat format,
        size_type width,
        size_type height,
        size_type depth,
        size_type row_pitch = 0,
        size_type slice_pitch = 0,
        void* host_ptr = NULL,
        cl_int* err = NULL)
    {
        cl_int error;
        bool useCreateImage;

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120
        // Run-time decision based on the actual platform
        {
            cl_uint version = detail::getContextPlatformVersion(context());
            useCreateImage = (version &gt;= 0x10002); // OpenCL 1.2 or above
        }
#elif CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
        useCreateImage = true;
#else
        useCreateImage = false;
#endif

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
        if (useCreateImage)
        {
            cl_image_desc desc =
            {
                CL_MEM_OBJECT_IMAGE3D,
                width,
                height,
                depth,
                0,      // array size (unused)
                row_pitch,
                slice_pitch,
                0, 0, 0
            };
            object_ = ::clCreateImage(
                context(), 
                flags, 
                &amp;format, 
                &amp;desc, 
                host_ptr, 
                &amp;error);

            detail::errHandler(error, __CREATE_IMAGE_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif  // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120
        if (!useCreateImage)
        {
            object_ = ::clCreateImage3D(
                context(), flags, &amp;format, width, height, depth, row_pitch,
                slice_pitch, host_ptr, &amp;error);

            detail::errHandler(error, __CREATE_IMAGE3D_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 120
    }

    //! \brief Default constructor - initializes to NULL.
    Image3D() : Image() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit Image3D(const cl_mem&amp; image3D, bool retainObject = false) : 
        Image(image3D, retainObject) { }

    /*! \brief Assignment from cl_mem - performs shallow copy.
     *
     *  See Memory for further details.
     */
    Image3D&amp; operator = (const cl_mem&amp; rhs)
    {
        Image::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image3D(const Image3D&amp; img) : Image(img) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image3D&amp; operator = (const Image3D &amp;img)
    {
        Image::operator=(img);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image3D(Image3D&amp;&amp; img) CL_HPP_NOEXCEPT_ : Image(std::move(img)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image3D&amp; operator = (Image3D &amp;&amp;img)
    {
        Image::operator=(std::move(img));
        return *this;
    }
};

#if defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
/*! \brief Class interface for GL 3D Image Memory objects.
 *
 *  This is provided to facilitate interoperability with OpenGL.
 * 
 *  See Memory for details about copy semantics, etc.
 * 
 *  \see Memory
 */
class Image3DGL : public Image3D
{
public:
    /*! \brief Constructs an Image3DGL in a specified context, from a given
     *         GL Texture.
     *
     *  Wraps clCreateFromGLTexture3D().
     */
    Image3DGL(
        const Context&amp; context,
        cl_mem_flags flags,
        cl_GLenum target,
        cl_GLint  miplevel,
        cl_GLuint texobj,
        cl_int * err = NULL)
    {
        cl_int error;
        object_ = ::clCreateFromGLTexture3D(
            context(),
            flags,
            target,
            miplevel,
            texobj,
            &amp;error);

        detail::errHandler(error, __CREATE_GL_TEXTURE_3D_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    //! \brief Default constructor - initializes to NULL.
    Image3DGL() : Image3D() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit Image3DGL(const cl_mem&amp; image, bool retainObject = false) : 
        Image3D(image, retainObject) { }

    /*! \brief Assignment from cl_mem - performs shallow copy.
     *
     *  See Memory for further details.
     */
    Image3DGL&amp; operator = (const cl_mem&amp; rhs)
    {
        Image3D::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image3DGL(const Image3DGL&amp; img) : Image3D(img) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Image3DGL&amp; operator = (const Image3DGL &amp;img)
    {
        Image3D::operator=(img);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image3DGL(Image3DGL&amp;&amp; img) CL_HPP_NOEXCEPT_ : Image3D(std::move(img)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Image3DGL&amp; operator = (Image3DGL &amp;&amp;img)
    {
        Image3D::operator=(std::move(img));
        return *this;
    }
};
#endif // CL_USE_DEPRECATED_OPENCL_1_1_APIS

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
/*! \class ImageGL
 * \brief general image interface for GL interop.
 * We abstract the 2D and 3D GL images into a single instance here
 * that wraps all GL sourced images on the grounds that setup information
 * was performed by OpenCL anyway.
 */
class ImageGL : public Image
{
public:
    ImageGL(
        const Context&amp; context,
        cl_mem_flags flags,
        cl_GLenum target,
        cl_GLint  miplevel,
        cl_GLuint texobj,
        cl_int * err = NULL)
    {
        cl_int error;
        object_ = ::clCreateFromGLTexture(
            context(), 
            flags, 
            target,
            miplevel,
            texobj,
            &amp;error);

        detail::errHandler(error, __CREATE_GL_TEXTURE_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    ImageGL() : Image() { }
    
    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  See Memory for further details.
     */
    explicit ImageGL(const cl_mem&amp; image, bool retainObject = false) : 
        Image(image, retainObject) { }

    ImageGL&amp; operator = (const cl_mem&amp; rhs)
    {
        Image::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    ImageGL(const ImageGL&amp; img) : Image(img) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    ImageGL&amp; operator = (const ImageGL &amp;img)
    {
        Image::operator=(img);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    ImageGL(ImageGL&amp;&amp; img) CL_HPP_NOEXCEPT_ : Image(std::move(img)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    ImageGL&amp; operator = (ImageGL &amp;&amp;img)
    {
        Image::operator=(std::move(img));
        return *this;
    }
};
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120



#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
/*! \brief Class interface for Pipe Memory Objects.
*
*  See Memory for details about copy semantics, etc.
*
*  \see Memory
*/
class Pipe : public Memory
{
public:

    /*! \brief Constructs a Pipe in a specified context.
     *
     * Wraps clCreatePipe().
     * @param context Context in which to create the pipe.
     * @param flags Bitfield. Only CL_MEM_READ_WRITE and CL_MEM_HOST_NO_ACCESS are valid.
     * @param packet_size Size in bytes of a single packet of the pipe.
     * @param max_packets Number of packets that may be stored in the pipe.
     *
     */
    Pipe(
        const Context&amp; context,
        cl_uint packet_size,
        cl_uint max_packets,
        cl_int* err = NULL)
    {
        cl_int error;

        cl_mem_flags flags = CL_MEM_READ_WRITE | CL_MEM_HOST_NO_ACCESS;
        object_ = ::clCreatePipe(context(), flags, packet_size, max_packets, nullptr, &amp;error);

        detail::errHandler(error, __CREATE_PIPE_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    /*! \brief Constructs a Pipe in a the default context.
     *
     * Wraps clCreatePipe().
     * @param flags Bitfield. Only CL_MEM_READ_WRITE and CL_MEM_HOST_NO_ACCESS are valid.
     * @param packet_size Size in bytes of a single packet of the pipe.
     * @param max_packets Number of packets that may be stored in the pipe.
     *
     */
    Pipe(
        cl_uint packet_size,
        cl_uint max_packets,
        cl_int* err = NULL)
    {
        cl_int error;

        Context context = Context::getDefault(err);

        cl_mem_flags flags = CL_MEM_READ_WRITE | CL_MEM_HOST_NO_ACCESS;
        object_ = ::clCreatePipe(context(), flags, packet_size, max_packets, nullptr, &amp;error);

        detail::errHandler(error, __CREATE_PIPE_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    //! \brief Default constructor - initializes to NULL.
    Pipe() : Memory() { }

    /*! \brief Constructor from cl_mem - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with earlier versions.
     *
     *  See Memory for further details.
     */
    explicit Pipe(const cl_mem&amp; pipe, bool retainObject = false) :
        Memory(pipe, retainObject) { }

    /*! \brief Assignment from cl_mem - performs shallow copy.
     *
     *  See Memory for further details.
     */
    Pipe&amp; operator = (const cl_mem&amp; rhs)
    {
        Memory::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Pipe(const Pipe&amp; pipe) : Memory(pipe) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Pipe&amp; operator = (const Pipe &amp;pipe)
    {
        Memory::operator=(pipe);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Pipe(Pipe&amp;&amp; pipe) CL_HPP_NOEXCEPT_ : Memory(std::move(pipe)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Pipe&amp; operator = (Pipe &amp;&amp;pipe)
    {
        Memory::operator=(std::move(pipe));
        return *this;
    }

    //! \brief Wrapper for clGetMemObjectInfo().
    template &lt;typename T&gt;
    cl_int getInfo(cl_pipe_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(&amp;::clGetPipeInfo, object_, name, param),
            __GET_PIPE_INFO_ERR);
    }

    //! \brief Wrapper for clGetMemObjectInfo() that returns by value.
    template &lt;cl_pipe_info name&gt; typename
        detail::param_traits&lt;detail::cl_pipe_info, name&gt;::param_type
        getInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_pipe_info, name&gt;::param_type param;
        cl_int result = getInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }
}; // class Pipe
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200


/*! \brief Class interface for cl_sampler.
 *
 *  \note Copies of these objects are shallow, meaning that the copy will refer
 *        to the same underlying cl_sampler as the original.  For details, see
 *        clRetainSampler() and clReleaseSampler().
 *
 *  \see cl_sampler 
 */
class Sampler : public detail::Wrapper&lt;cl_sampler&gt;
{
public:
    //! \brief Default constructor - initializes to NULL.
    Sampler() { }

    /*! \brief Constructs a Sampler in a specified context.
     *
     *  Wraps clCreateSampler().
     */
    Sampler(
        const Context&amp; context,
        cl_bool normalized_coords,
        cl_addressing_mode addressing_mode,
        cl_filter_mode filter_mode,
        cl_int* err = NULL)
    {
        cl_int error;

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        cl_sampler_properties sampler_properties[] = {
            CL_SAMPLER_NORMALIZED_COORDS, normalized_coords,
            CL_SAMPLER_ADDRESSING_MODE, addressing_mode,
            CL_SAMPLER_FILTER_MODE, filter_mode,
            0 };
        object_ = ::clCreateSamplerWithProperties(
            context(),
            sampler_properties,
            &amp;error);

        detail::errHandler(error, __CREATE_SAMPLER_WITH_PROPERTIES_ERR);
        if (err != NULL) {
            *err = error;
        }
#else
        object_ = ::clCreateSampler(
            context(),
            normalized_coords,
            addressing_mode,
            filter_mode,
            &amp;error);

        detail::errHandler(error, __CREATE_SAMPLER_ERR);
        if (err != NULL) {
            *err = error;
        }
#endif        
    }

    /*! \brief Constructor from cl_sampler - takes ownership.
     * 
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  This effectively transfers ownership of a refcount on the cl_sampler
     *  into the new Sampler object.
     */
    explicit Sampler(const cl_sampler&amp; sampler, bool retainObject = false) : 
        detail::Wrapper&lt;cl_type&gt;(sampler, retainObject) { }

    /*! \brief Assignment operator from cl_sampler - takes ownership.
     *
     *  This effectively transfers ownership of a refcount on the rhs and calls
     *  clReleaseSampler() on the value previously held by this instance.
     */
    Sampler&amp; operator = (const cl_sampler&amp; rhs)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Sampler(const Sampler&amp; sam) : detail::Wrapper&lt;cl_type&gt;(sam) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Sampler&amp; operator = (const Sampler &amp;sam)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(sam);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Sampler(Sampler&amp;&amp; sam) CL_HPP_NOEXCEPT_ : detail::Wrapper&lt;cl_type&gt;(std::move(sam)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Sampler&amp; operator = (Sampler &amp;&amp;sam)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(std::move(sam));
        return *this;
    }

    //! \brief Wrapper for clGetSamplerInfo().
    template &lt;typename T&gt;
    cl_int getInfo(cl_sampler_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(&amp;::clGetSamplerInfo, object_, name, param),
            __GET_SAMPLER_INFO_ERR);
    }

    //! \brief Wrapper for clGetSamplerInfo() that returns by value.
    template &lt;cl_sampler_info name&gt; typename
    detail::param_traits&lt;detail::cl_sampler_info, name&gt;::param_type
    getInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_sampler_info, name&gt;::param_type param;
        cl_int result = getInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }
};

class Program;
class CommandQueue;
class DeviceCommandQueue;
class Kernel;

//! \brief Class interface for specifying NDRange values.
class NDRange
{
private:
    size_type sizes_[3];
    cl_uint dimensions_;

public:
    //! \brief Default constructor - resulting range has zero dimensions.
    NDRange()
        : dimensions_(0)
    {
        sizes_[0] = 0;
        sizes_[1] = 0;
        sizes_[2] = 0;
    }

    //! \brief Constructs one-dimensional range.
    NDRange(size_type size0)
        : dimensions_(1)
    {
        sizes_[0] = size0;
        sizes_[1] = 1;
        sizes_[2] = 1;
    }

    //! \brief Constructs two-dimensional range.
    NDRange(size_type size0, size_type size1)
        : dimensions_(2)
    {
        sizes_[0] = size0;
        sizes_[1] = size1;
        sizes_[2] = 1;
    }

    //! \brief Constructs three-dimensional range.
    NDRange(size_type size0, size_type size1, size_type size2)
        : dimensions_(3)
    {
        sizes_[0] = size0;
        sizes_[1] = size1;
        sizes_[2] = size2;
    }

    /*! \brief Conversion operator to const size_type *.
     *  
     *  \returns a pointer to the size of the first dimension.
     */
    operator const size_type*() const { 
        return sizes_; 
    }

    //! \brief Queries the number of dimensions in the range.
    size_type dimensions() const 
    { 
        return dimensions_; 
    }

    //! \brief Returns the size of the object in bytes based on the
    // runtime number of dimensions
    size_type size() const
    {
        return dimensions_*sizeof(size_type);
    }

    size_type* get()
    {
        return sizes_;
    }
    
    const size_type* get() const
    {
        return sizes_;
    }
};

//! \brief A zero-dimensional range.
static const NDRange NullRange;

//! \brief Local address wrapper for use with Kernel::setArg
struct LocalSpaceArg
{
    size_type size_;
};

namespace detail {

template &lt;typename T, class Enable = void&gt;
struct KernelArgumentHandler;

// Enable for objects that are not subclasses of memory
// Pointers, constants etc
template &lt;typename T&gt;
struct KernelArgumentHandler&lt;T, typename std::enable_if&lt;!std::is_base_of&lt;cl::Memory, T&gt;::value&gt;::type&gt;
{
    static size_type size(const T&amp;) { return sizeof(T); }
    static const T* ptr(const T&amp; value) { return &amp;value; }
};

// Enable for subclasses of memory where we want to get a reference to the cl_mem out
// and pass that in for safety
template &lt;typename T&gt;
struct KernelArgumentHandler&lt;T, typename std::enable_if&lt;std::is_base_of&lt;cl::Memory, T&gt;::value&gt;::type&gt;
{
    static size_type size(const T&amp;) { return sizeof(cl_mem); }
    static const cl_mem* ptr(const T&amp; value) { return &amp;(value()); }
};

// Specialization for DeviceCommandQueue defined later

template &lt;&gt;
struct KernelArgumentHandler&lt;LocalSpaceArg, void&gt;
{
    static size_type size(const LocalSpaceArg&amp; value) { return value.size_; }
    static const void* ptr(const LocalSpaceArg&amp;) { return NULL; }
};

} 
//! \endcond

/*! Local
 * \brief Helper function for generating LocalSpaceArg objects.
 */
inline LocalSpaceArg
Local(size_type size)
{
    LocalSpaceArg ret = { size };
    return ret;
}

/*! \brief Class interface for cl_kernel.
 *
 *  \note Copies of these objects are shallow, meaning that the copy will refer
 *        to the same underlying cl_kernel as the original.  For details, see
 *        clRetainKernel() and clReleaseKernel().
 *
 *  \see cl_kernel
 */
class Kernel : public detail::Wrapper&lt;cl_kernel&gt;
{
public:
    inline Kernel(const Program&amp; program, const char* name, cl_int* err = NULL);

    //! \brief Default constructor - initializes to NULL.
    Kernel() { }

    /*! \brief Constructor from cl_kernel - takes ownership.
     * 
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     *  This effectively transfers ownership of a refcount on the cl_kernel
     *  into the new Kernel object.
     */
    explicit Kernel(const cl_kernel&amp; kernel, bool retainObject = false) : 
        detail::Wrapper&lt;cl_type&gt;(kernel, retainObject) { }

    /*! \brief Assignment operator from cl_kernel - takes ownership.
     *
     *  This effectively transfers ownership of a refcount on the rhs and calls
     *  clReleaseKernel() on the value previously held by this instance.
     */
    Kernel&amp; operator = (const cl_kernel&amp; rhs)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Kernel(const Kernel&amp; kernel) : detail::Wrapper&lt;cl_type&gt;(kernel) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Kernel&amp; operator = (const Kernel &amp;kernel)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(kernel);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Kernel(Kernel&amp;&amp; kernel) CL_HPP_NOEXCEPT_ : detail::Wrapper&lt;cl_type&gt;(std::move(kernel)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Kernel&amp; operator = (Kernel &amp;&amp;kernel)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(std::move(kernel));
        return *this;
    }

    template &lt;typename T&gt;
    cl_int getInfo(cl_kernel_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(&amp;::clGetKernelInfo, object_, name, param),
            __GET_KERNEL_INFO_ERR);
    }

    template &lt;cl_kernel_info name&gt; typename
    detail::param_traits&lt;detail::cl_kernel_info, name&gt;::param_type
    getInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_kernel_info, name&gt;::param_type param;
        cl_int result = getInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
    template &lt;typename T&gt;
    cl_int getArgInfo(cl_uint argIndex, cl_kernel_arg_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(&amp;::clGetKernelArgInfo, object_, argIndex, name, param),
            __GET_KERNEL_ARG_INFO_ERR);
    }

    template &lt;cl_kernel_arg_info name&gt; typename
    detail::param_traits&lt;detail::cl_kernel_arg_info, name&gt;::param_type
    getArgInfo(cl_uint argIndex, cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_kernel_arg_info, name&gt;::param_type param;
        cl_int result = getArgInfo(argIndex, name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120

    template &lt;typename T&gt;
    cl_int getWorkGroupInfo(
        const Device&amp; device, cl_kernel_work_group_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(
                &amp;::clGetKernelWorkGroupInfo, object_, device(), name, param),
                __GET_KERNEL_WORK_GROUP_INFO_ERR);
    }

    template &lt;cl_kernel_work_group_info name&gt; typename
    detail::param_traits&lt;detail::cl_kernel_work_group_info, name&gt;::param_type
        getWorkGroupInfo(const Device&amp; device, cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
        detail::cl_kernel_work_group_info, name&gt;::param_type param;
        cl_int result = getWorkGroupInfo(device, name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }
    
#if (CL_HPP_TARGET_OPENCL_VERSION &gt;= 200 &amp;&amp; defined(CL_HPP_USE_CL_SUB_GROUPS_KHR)) || CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
    cl_int getSubGroupInfo(const cl::Device &amp;dev, cl_kernel_sub_group_info name, const cl::NDRange &amp;range, size_type* param) const
    {
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210

        return detail::errHandler(
            clGetKernelSubGroupInfo(object_, dev(), name, range.size(), range.get(), sizeof(size_type), param, nullptr),
            __GET_KERNEL_SUB_GROUP_INFO_ERR);

#else // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210

        typedef clGetKernelSubGroupInfoKHR_fn PFN_clGetKernelSubGroupInfoKHR;
        static PFN_clGetKernelSubGroupInfoKHR pfn_clGetKernelSubGroupInfoKHR = NULL;
        CL_HPP_INIT_CL_EXT_FCN_PTR_(clGetKernelSubGroupInfoKHR);

        return detail::errHandler(
            pfn_clGetKernelSubGroupInfoKHR(object_, dev(), name, range.size(), range.get(), sizeof(size_type), param, nullptr),
            __GET_KERNEL_SUB_GROUP_INFO_ERR);

#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
    }

    template &lt;cl_kernel_sub_group_info name&gt;
        size_type getSubGroupInfo(const cl::Device &amp;dev, const cl::NDRange &amp;range, cl_int* err = NULL) const
    {
        size_type param;
        cl_int result = getSubGroupInfo(dev, name, range, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
    /*! \brief setArg overload taking a shared_ptr type
     */
    template&lt;typename T, class D&gt;
    cl_int setArg(cl_uint index, const cl::pointer&lt;T, D&gt; &amp;argPtr)
    {
        return detail::errHandler(
            ::clSetKernelArgSVMPointer(object_, index, argPtr.get()),
            __SET_KERNEL_ARGS_ERR);
    }

    /*! \brief setArg overload taking a vector type.
     */
    template&lt;typename T, class Alloc&gt;
    cl_int setArg(cl_uint index, const cl::vector&lt;T, Alloc&gt; &amp;argPtr)
    {
        return detail::errHandler(
            ::clSetKernelArgSVMPointer(object_, index, argPtr.data()),
            __SET_KERNEL_ARGS_ERR);
    }

    /*! \brief setArg overload taking a pointer type
     */
    template&lt;typename T&gt;
    typename std::enable_if&lt;std::is_pointer&lt;T&gt;::value, cl_int&gt;::type
        setArg(cl_uint index, const T argPtr)
    {
        return detail::errHandler(
            ::clSetKernelArgSVMPointer(object_, index, argPtr),
            __SET_KERNEL_ARGS_ERR);
    }
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200

    /*! \brief setArg overload taking a POD type
     */
    template &lt;typename T&gt;
    typename std::enable_if&lt;!std::is_pointer&lt;T&gt;::value, cl_int&gt;::type
        setArg(cl_uint index, const T &amp;value)
    {
        return detail::errHandler(
            ::clSetKernelArg(
                object_,
                index,
                detail::KernelArgumentHandler&lt;T&gt;::size(value),
                detail::KernelArgumentHandler&lt;T&gt;::ptr(value)),
            __SET_KERNEL_ARGS_ERR);
    }

    cl_int setArg(cl_uint index, size_type size, const void* argPtr)
    {
        return detail::errHandler(
            ::clSetKernelArg(object_, index, size, argPtr),
            __SET_KERNEL_ARGS_ERR);
    }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
    /*!
     * Specify a vector of SVM pointers that the kernel may access in 
     * addition to its arguments.
     */
    cl_int setSVMPointers(const vector&lt;void*&gt; &amp;pointerList)
    {
        return detail::errHandler(
            ::clSetKernelExecInfo(
                object_,
                CL_KERNEL_EXEC_INFO_SVM_PTRS,
                sizeof(void*)*pointerList.size(),
                pointerList.data()));
    }

    /*!
     * Specify a std::array of SVM pointers that the kernel may access in
     * addition to its arguments.
     */
    template&lt;int ArrayLength&gt;
    cl_int setSVMPointers(const std::array&lt;void*, ArrayLength&gt; &amp;pointerList)
    {
        return detail::errHandler(
            ::clSetKernelExecInfo(
                object_,
                CL_KERNEL_EXEC_INFO_SVM_PTRS,
                sizeof(void*)*pointerList.size(),
                pointerList.data()));
    }

    /*! \brief Enable fine-grained system SVM.
     *
     * \note It is only possible to enable fine-grained system SVM if all devices
     *       in the context associated with kernel support it.
     * 
     * \param svmEnabled True if fine-grained system SVM is requested. False otherwise.
     * \return CL_SUCCESS if the function was executed succesfully. CL_INVALID_OPERATION
     *         if no devices in the context support fine-grained system SVM.
     *
     * \see clSetKernelExecInfo
     */
    cl_int enableFineGrainedSystemSVM(bool svmEnabled)
    {
        cl_bool svmEnabled_ = svmEnabled ? CL_TRUE : CL_FALSE;
        return detail::errHandler(
            ::clSetKernelExecInfo(
                object_,
                CL_KERNEL_EXEC_INFO_SVM_FINE_GRAIN_SYSTEM,
                sizeof(cl_bool),
                &amp;svmEnabled_
                )
            );
    }
    
    template&lt;int index, int ArrayLength, class D, typename T0, typename T1, typename... Ts&gt;
    void setSVMPointersHelper(std::array&lt;void*, ArrayLength&gt; &amp;pointerList, const pointer&lt;T0, D&gt; &amp;t0, const pointer&lt;T1, D&gt; &amp;t1, Ts &amp; ... ts)
    {
        pointerList[index] = static_cast&lt;void*&gt;(t0.get());
        setSVMPointersHelper&lt;index + 1, ArrayLength&gt;(pointerList, t1, ts...);
    }

    template&lt;int index, int ArrayLength, typename T0, typename T1, typename... Ts&gt;
    typename std::enable_if&lt;std::is_pointer&lt;T0&gt;::value, void&gt;::type
    setSVMPointersHelper(std::array&lt;void*, ArrayLength&gt; &amp;pointerList, T0 t0, T1 t1, Ts... ts)
    {
        pointerList[index] = static_cast&lt;void*&gt;(t0);
        setSVMPointersHelper&lt;index + 1, ArrayLength&gt;(pointerList, t1, ts...);
    }

    template&lt;int index, int ArrayLength, typename T0, class D&gt;
    void setSVMPointersHelper(std::array&lt;void*, ArrayLength&gt; &amp;pointerList, const pointer&lt;T0, D&gt; &amp;t0)
    {
        pointerList[index] = static_cast&lt;void*&gt;(t0.get());
    }


    template&lt;int index, int ArrayLength, typename T0&gt;
    typename std::enable_if&lt;std::is_pointer&lt;T0&gt;::value, void&gt;::type
    setSVMPointersHelper(std::array&lt;void*, ArrayLength&gt; &amp;pointerList, T0 t0)
    {
        pointerList[index] = static_cast&lt;void*&gt;(t0);
    }

    template&lt;typename T0, typename... Ts&gt;
    cl_int setSVMPointers(const T0 &amp;t0, Ts &amp; ... ts)
    {
        std::array&lt;void*, 1 + sizeof...(Ts)&gt; pointerList;

        setSVMPointersHelper&lt;0, 1 + sizeof...(Ts)&gt;(pointerList, t0, ts...);
        return detail::errHandler(
            ::clSetKernelExecInfo(
            object_,
            CL_KERNEL_EXEC_INFO_SVM_PTRS,
            sizeof(void*)*(1 + sizeof...(Ts)),
            pointerList.data()));
    }

    template&lt;typename T&gt;
    cl_int setExecInfo(cl_kernel_exec_info param_name, const T&amp; val)
    {
        return detail::errHandler(
            ::clSetKernelExecInfo(
            object_,
            param_name,
            sizeof(T),
            &amp;val));
    }

    template&lt;cl_kernel_exec_info name&gt;
    cl_int setExecInfo(typename detail::param_traits&lt;detail::cl_kernel_exec_info, name&gt;::param_type&amp; val)
    {
        return setExecInfo(name, val);
    }
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
    /**
     * Make a deep copy of the kernel object including its arguments.
     * @return A new kernel object with internal state entirely separate from that
     *         of the original but with any arguments set on the original intact.
     */
    Kernel clone()
    {
        cl_int error;
        Kernel retValue(clCloneKernel(this-&gt;get(), &amp;error));

        detail::errHandler(error, __CLONE_KERNEL_ERR);
        return retValue;
    }
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
};

/*! \class Program
 * \brief Program interface that implements cl_program.
 */
class Program : public detail::Wrapper&lt;cl_program&gt;
{
public:
#if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
    typedef vector&lt;vector&lt;unsigned char&gt;&gt; Binaries;
    typedef vector&lt;string&gt; Sources;
#else // #if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
    typedef vector&lt;std::pair&lt;const void*, size_type&gt; &gt; Binaries;
    typedef vector&lt;std::pair&lt;const char*, size_type&gt; &gt; Sources;
#endif // #if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
    
    Program(
        const string&amp; source,
        bool build = false,
        cl_int* err = NULL)
    {
        cl_int error;

        const char * strings = source.c_str();
        const size_type length  = source.size();

        Context context = Context::getDefault(err);

        object_ = ::clCreateProgramWithSource(
            context(), (cl_uint)1, &amp;strings, &amp;length, &amp;error);

        detail::errHandler(error, __CREATE_PROGRAM_WITH_SOURCE_ERR);

        if (error == CL_SUCCESS &amp;&amp; build) {

            error = ::clBuildProgram(
                object_,
                0,
                NULL,
#if !defined(CL_HPP_CL_1_2_DEFAULT_BUILD)
                "-cl-std=CL2.0",
#else
                "",
#endif // #if !defined(CL_HPP_CL_1_2_DEFAULT_BUILD)
                NULL,
                NULL);

            detail::buildErrHandler(error, __BUILD_PROGRAM_ERR, getBuildInfo&lt;CL_PROGRAM_BUILD_LOG&gt;());
        }

        if (err != NULL) {
            *err = error;
        }
    }

    Program(
        const Context&amp; context,
        const string&amp; source,
        bool build = false,
        cl_int* err = NULL)
    {
        cl_int error;

        const char * strings = source.c_str();
        const size_type length  = source.size();

        object_ = ::clCreateProgramWithSource(
            context(), (cl_uint)1, &amp;strings, &amp;length, &amp;error);

        detail::errHandler(error, __CREATE_PROGRAM_WITH_SOURCE_ERR);

        if (error == CL_SUCCESS &amp;&amp; build) {
            error = ::clBuildProgram(
                object_,
                0,
                NULL,
#if !defined(CL_HPP_CL_1_2_DEFAULT_BUILD)
                "-cl-std=CL2.0",
#else
                "",
#endif // #if !defined(CL_HPP_CL_1_2_DEFAULT_BUILD)
                NULL,
                NULL);
            
            detail::buildErrHandler(error, __BUILD_PROGRAM_ERR, getBuildInfo&lt;CL_PROGRAM_BUILD_LOG&gt;());
        }

        if (err != NULL) {
            *err = error;
        }
    }

    /**
     * Create a program from a vector of source strings and the default context.
     * Does not compile or link the program.
     */
    Program(
        const Sources&amp; sources,
        cl_int* err = NULL)
    {
        cl_int error;
        Context context = Context::getDefault(err);

        const size_type n = (size_type)sources.size();

        vector&lt;size_type&gt; lengths(n);
        vector&lt;const char*&gt; strings(n);

        for (size_type i = 0; i &lt; n; ++i) {
#if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
            strings[i] = sources[(int)i].data();
            lengths[i] = sources[(int)i].length();
#else // #if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
            strings[i] = sources[(int)i].first;
            lengths[i] = sources[(int)i].second;
#endif // #if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
        }

        object_ = ::clCreateProgramWithSource(
            context(), (cl_uint)n, strings.data(), lengths.data(), &amp;error);

        detail::errHandler(error, __CREATE_PROGRAM_WITH_SOURCE_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    /**
     * Create a program from a vector of source strings and a provided context.
     * Does not compile or link the program.
     */
    Program(
        const Context&amp; context,
        const Sources&amp; sources,
        cl_int* err = NULL)
    {
        cl_int error;

        const size_type n = (size_type)sources.size();

        vector&lt;size_type&gt; lengths(n);
        vector&lt;const char*&gt; strings(n);

        for (size_type i = 0; i &lt; n; ++i) {
#if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
            strings[i] = sources[(int)i].data();
            lengths[i] = sources[(int)i].length();
#else // #if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
            strings[i] = sources[(int)i].first;
            lengths[i] = sources[(int)i].second;
#endif // #if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
        }

        object_ = ::clCreateProgramWithSource(
            context(), (cl_uint)n, strings.data(), lengths.data(), &amp;error);

        detail::errHandler(error, __CREATE_PROGRAM_WITH_SOURCE_ERR);
        if (err != NULL) {
            *err = error;
        }
    }


#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210 || (CL_HPP_TARGET_OPENCL_VERSION==200 &amp;&amp; defined(CL_HPP_USE_IL_KHR))
    /**
     * Program constructor to allow construction of program from SPIR-V or another IL.
     * Valid for either OpenCL &gt;= 2.1 or when CL_HPP_USE_IL_KHR is defined.
     */
    Program(
        const vector&lt;char&gt;&amp; IL,
        bool build = false,
        cl_int* err = NULL)
    {
        cl_int error;

        Context context = Context::getDefault(err);

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210

        object_ = ::clCreateProgramWithIL(
            context(), static_cast&lt;const void*&gt;(IL.data()), IL.size(), &amp;error);

#else // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210

        typedef clCreateProgramWithILKHR_fn PFN_clCreateProgramWithILKHR;
        static PFN_clCreateProgramWithILKHR pfn_clCreateProgramWithILKHR = NULL;
        CL_HPP_INIT_CL_EXT_FCN_PTR_(clCreateProgramWithILKHR);

        return detail::errHandler(
            pfn_clCreateProgramWithILKHR(
                context(), static_cast&lt;const void*&gt;(IL.data()), IL.size(), &amp;error);

#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210

        detail::errHandler(error, __CREATE_PROGRAM_WITH_IL_ERR);

        if (error == CL_SUCCESS &amp;&amp; build) {

            error = ::clBuildProgram(
                object_,
                0,
                NULL,
#if !defined(CL_HPP_CL_1_2_DEFAULT_BUILD)
                "-cl-std=CL2.0",
#else
                "",
#endif // #if !defined(CL_HPP_CL_1_2_DEFAULT_BUILD)
                NULL,
                NULL);

            detail::buildErrHandler(error, __BUILD_PROGRAM_ERR, getBuildInfo&lt;CL_PROGRAM_BUILD_LOG&gt;());
        }

        if (err != NULL) {
            *err = error;
        }
    }

    /**
     * Program constructor to allow construction of program from SPIR-V or another IL
     * for a specific context.
     * Valid for either OpenCL &gt;= 2.1 or when CL_HPP_USE_IL_KHR is defined.
     */
    Program(
        const Context&amp; context,
        const vector&lt;char&gt;&amp; IL,
        bool build = false,
        cl_int* err = NULL)
    {
        cl_int error;

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210

        object_ = ::clCreateProgramWithIL(
            context(), static_cast&lt;const void*&gt;(IL.data()), IL.size(), &amp;error);

#else // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210

        typedef clCreateProgramWithILKHR_fn PFN_clCreateProgramWithILKHR;
        static PFN_clCreateProgramWithILKHR pfn_clCreateProgramWithILKHR = NULL;
        CL_HPP_INIT_CL_EXT_FCN_PTR_(clCreateProgramWithILKHR);

        return detail::errHandler(
            pfn_clCreateProgramWithILKHR(
            context(), static_cast&lt;const void*&gt;(IL.data()), IL.size(), &amp;error);

#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210

        detail::errHandler(error, __CREATE_PROGRAM_WITH_IL_ERR);

        if (error == CL_SUCCESS &amp;&amp; build) {
            error = ::clBuildProgram(
                object_,
                0,
                NULL,
#if !defined(CL_HPP_CL_1_2_DEFAULT_BUILD)
                "-cl-std=CL2.0",
#else
                "",
#endif // #if !defined(CL_HPP_CL_1_2_DEFAULT_BUILD)
                NULL,
                NULL);

            detail::buildErrHandler(error, __BUILD_PROGRAM_ERR, getBuildInfo&lt;CL_PROGRAM_BUILD_LOG&gt;());
        }

        if (err != NULL) {
            *err = error;
        }
    }
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210

    /**
     * Construct a program object from a list of devices and a per-device list of binaries.
     * \param context A valid OpenCL context in which to construct the program.
     * \param devices A vector of OpenCL device objects for which the program will be created.
     * \param binaries A vector of pairs of a pointer to a binary object and its length.
     * \param binaryStatus An optional vector that on completion will be resized to
     *   match the size of binaries and filled with values to specify if each binary
     *   was successfully loaded.
     *   Set to CL_SUCCESS if the binary was successfully loaded.
     *   Set to CL_INVALID_VALUE if the length is 0 or the binary pointer is NULL.
     *   Set to CL_INVALID_BINARY if the binary provided is not valid for the matching device.
     * \param err if non-NULL will be set to CL_SUCCESS on successful operation or one of the following errors:
     *   CL_INVALID_CONTEXT if context is not a valid context.
     *   CL_INVALID_VALUE if the length of devices is zero; or if the length of binaries does not match the length of devices; 
     *     or if any entry in binaries is NULL or has length 0.
     *   CL_INVALID_DEVICE if OpenCL devices listed in devices are not in the list of devices associated with context.
     *   CL_INVALID_BINARY if an invalid program binary was encountered for any device. binaryStatus will return specific status for each device.
     *   CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources required by the OpenCL implementation on the host.
     */
    Program(
        const Context&amp; context,
        const vector&lt;Device&gt;&amp; devices,
        const Binaries&amp; binaries,
        vector&lt;cl_int&gt;* binaryStatus = NULL,
        cl_int* err = NULL)
    {
        cl_int error;
        
        const size_type numDevices = devices.size();
        
        // Catch size mismatch early and return
        if(binaries.size() != numDevices) {
            error = CL_INVALID_VALUE;
            detail::errHandler(error, __CREATE_PROGRAM_WITH_BINARY_ERR);
            if (err != NULL) {
                *err = error;
            }
            return;
        }


        vector&lt;size_type&gt; lengths(numDevices);
        vector&lt;const unsigned char*&gt; images(numDevices);
#if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
        for (size_type i = 0; i &lt; numDevices; ++i) {
            images[i] = binaries[i].data();
            lengths[i] = binaries[(int)i].size();
        }
#else // #if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
        for (size_type i = 0; i &lt; numDevices; ++i) {
            images[i] = (const unsigned char*)binaries[i].first;
            lengths[i] = binaries[(int)i].second;
        }
#endif // #if !defined(CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY)
        
        vector&lt;cl_device_id&gt; deviceIDs(numDevices);
        for( size_type deviceIndex = 0; deviceIndex &lt; numDevices; ++deviceIndex ) {
            deviceIDs[deviceIndex] = (devices[deviceIndex])();
        }

        if(binaryStatus) {
            binaryStatus-&gt;resize(numDevices);
        }
        
        object_ = ::clCreateProgramWithBinary(
            context(), (cl_uint) devices.size(),
            deviceIDs.data(),
            lengths.data(), images.data(), (binaryStatus != NULL &amp;&amp; numDevices &gt; 0)
               ? &amp;binaryStatus-&gt;front()
               : NULL, &amp;error);

        detail::errHandler(error, __CREATE_PROGRAM_WITH_BINARY_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
    /**
     * Create program using builtin kernels.
     * \param kernelNames Semi-colon separated list of builtin kernel names
     */
    Program(
        const Context&amp; context,
        const vector&lt;Device&gt;&amp; devices,
        const string&amp; kernelNames,
        cl_int* err = NULL)
    {
        cl_int error;


        size_type numDevices = devices.size();
        vector&lt;cl_device_id&gt; deviceIDs(numDevices);
        for( size_type deviceIndex = 0; deviceIndex &lt; numDevices; ++deviceIndex ) {
            deviceIDs[deviceIndex] = (devices[deviceIndex])();
        }
        
        object_ = ::clCreateProgramWithBuiltInKernels(
            context(), 
            (cl_uint) devices.size(),
            deviceIDs.data(),
            kernelNames.c_str(), 
            &amp;error);

        detail::errHandler(error, __CREATE_PROGRAM_WITH_BUILT_IN_KERNELS_ERR);
        if (err != NULL) {
            *err = error;
        }
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120

    Program() { }
    

    /*! \brief Constructor from cl_program - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     */
    explicit Program(const cl_program&amp; program, bool retainObject = false) : 
        detail::Wrapper&lt;cl_type&gt;(program, retainObject) { }

    Program&amp; operator = (const cl_program&amp; rhs)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Program(const Program&amp; program) : detail::Wrapper&lt;cl_type&gt;(program) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    Program&amp; operator = (const Program &amp;program)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(program);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Program(Program&amp;&amp; program) CL_HPP_NOEXCEPT_ : detail::Wrapper&lt;cl_type&gt;(std::move(program)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    Program&amp; operator = (Program &amp;&amp;program)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(std::move(program));
        return *this;
    }

    cl_int build(
        const vector&lt;Device&gt;&amp; devices,
        const char* options = NULL,
        void (CL_CALLBACK * notifyFptr)(cl_program, void *) = NULL,
        void* data = NULL) const
    {
        size_type numDevices = devices.size();
        vector&lt;cl_device_id&gt; deviceIDs(numDevices);
        
        for( size_type deviceIndex = 0; deviceIndex &lt; numDevices; ++deviceIndex ) {
            deviceIDs[deviceIndex] = (devices[deviceIndex])();
        }

        cl_int buildError = ::clBuildProgram(
            object_,
            (cl_uint)
            devices.size(),
            deviceIDs.data(),
            options,
            notifyFptr,
            data);

        return detail::buildErrHandler(buildError, __BUILD_PROGRAM_ERR, getBuildInfo&lt;CL_PROGRAM_BUILD_LOG&gt;());
    }

    cl_int build(
        const Device&amp; device,
        const char* options = NULL,
        void (CL_CALLBACK * notifyFptr)(cl_program, void *) = NULL,
        void* data = NULL) const
    {
        cl_device_id deviceID = device();

        cl_int buildError = ::clBuildProgram(
            object_,
            1,
            &amp;deviceID,
            options,
            notifyFptr,
            data);

        BuildLogType buildLog(1);
        buildLog.push_back(std::make_pair(device, getBuildInfo&lt;CL_PROGRAM_BUILD_LOG&gt;(device)));
        return detail::buildErrHandler(buildError, __BUILD_PROGRAM_ERR, buildLog);
    }

    cl_int build(
        const char* options = NULL,
        void (CL_CALLBACK * notifyFptr)(cl_program, void *) = NULL,
        void* data = NULL) const
    {
        cl_int buildError = ::clBuildProgram(
            object_,
            0,
            NULL,
            options,
            notifyFptr,
            data);

        return detail::buildErrHandler(buildError, __BUILD_PROGRAM_ERR, getBuildInfo&lt;CL_PROGRAM_BUILD_LOG&gt;());
    }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
    cl_int compile(
        const char* options = NULL,
        void (CL_CALLBACK * notifyFptr)(cl_program, void *) = NULL,
        void* data = NULL) const
    {
        cl_int error = ::clCompileProgram(
            object_,
            0,
            NULL,
            options,
            0,
            NULL,
            NULL,
            notifyFptr,
            data);
        return detail::buildErrHandler(error, __COMPILE_PROGRAM_ERR, getBuildInfo&lt;CL_PROGRAM_BUILD_LOG&gt;());
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120

    template &lt;typename T&gt;
    cl_int getInfo(cl_program_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(&amp;::clGetProgramInfo, object_, name, param),
            __GET_PROGRAM_INFO_ERR);
    }

    template &lt;cl_program_info name&gt; typename
    detail::param_traits&lt;detail::cl_program_info, name&gt;::param_type
    getInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_program_info, name&gt;::param_type param;
        cl_int result = getInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }

    template &lt;typename T&gt;
    cl_int getBuildInfo(
        const Device&amp; device, cl_program_build_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(
                &amp;::clGetProgramBuildInfo, object_, device(), name, param),
                __GET_PROGRAM_BUILD_INFO_ERR);
    }

    template &lt;cl_program_build_info name&gt; typename
    detail::param_traits&lt;detail::cl_program_build_info, name&gt;::param_type
    getBuildInfo(const Device&amp; device, cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_program_build_info, name&gt;::param_type param;
        cl_int result = getBuildInfo(device, name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }
    
    /**
     * Build info function that returns a vector of device/info pairs for the specified 
     * info type and for all devices in the program.
     * On an error reading the info for any device, an empty vector of info will be returned.
     */
    template &lt;cl_program_build_info name&gt;
    vector&lt;std::pair&lt;cl::Device, typename detail::param_traits&lt;detail::cl_program_build_info, name&gt;::param_type&gt;&gt;
        getBuildInfo(cl_int *err = NULL) const
    {
        cl_int result = CL_SUCCESS;

        auto devs = getInfo&lt;CL_PROGRAM_DEVICES&gt;(&amp;result);
        vector&lt;std::pair&lt;cl::Device, typename detail::param_traits&lt;detail::cl_program_build_info, name&gt;::param_type&gt;&gt;
            devInfo;

        // If there was an initial error from getInfo return the error
        if (result != CL_SUCCESS) {
            if (err != NULL) {
                *err = result;
            }
            return devInfo;
        }

        for (const cl::Device &amp;d : devs) {
            typename detail::param_traits&lt;
                detail::cl_program_build_info, name&gt;::param_type param;
            result = getBuildInfo(d, name, &amp;param);
            devInfo.push_back(
                std::pair&lt;cl::Device, typename detail::param_traits&lt;detail::cl_program_build_info, name&gt;::param_type&gt;
                (d, param));
            if (result != CL_SUCCESS) {
                // On error, leave the loop and return the error code
                break;
            }
        }
        if (err != NULL) {
            *err = result;
        }
        if (result != CL_SUCCESS) {
            devInfo.clear();
        }
        return devInfo;
    }

    cl_int createKernels(vector&lt;Kernel&gt;* kernels)
    {
        cl_uint numKernels;
        cl_int err = ::clCreateKernelsInProgram(object_, 0, NULL, &amp;numKernels);
        if (err != CL_SUCCESS) {
            return detail::errHandler(err, __CREATE_KERNELS_IN_PROGRAM_ERR);
        }

        vector&lt;cl_kernel&gt; value(numKernels);
        
        err = ::clCreateKernelsInProgram(
            object_, numKernels, value.data(), NULL);
        if (err != CL_SUCCESS) {
            return detail::errHandler(err, __CREATE_KERNELS_IN_PROGRAM_ERR);
        }

        if (kernels) {
            kernels-&gt;resize(value.size());

            // Assign to param, constructing with retain behaviour
            // to correctly capture each underlying CL object
            for (size_type i = 0; i &lt; value.size(); i++) {
                // We do not need to retain because this kernel is being created 
                // by the runtime
                (*kernels)[i] = Kernel(value[i], false);
            }
        }
        return CL_SUCCESS;
    }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 220
#if defined(CL_USE_DEPRECATED_OPENCL_2_2_APIS)
    /*! \brief Registers a callback function to be called when destructors for
     *         program scope global variables are complete and before the
     *         program is released.
     *
     *  Wraps clSetProgramReleaseCallback().
     *
     *  Each call to this function registers the specified user callback function
     *  on a callback stack associated with program. The registered user callback
     *  functions are called in the reverse order in which they were registered.
     */
    CL_EXT_PREFIX__VERSION_2_2_DEPRECATED cl_int setReleaseCallback(
        void (CL_CALLBACK * pfn_notify)(cl_program program, void * user_data),
        void * user_data = NULL) CL_EXT_SUFFIX__VERSION_2_2_DEPRECATED
    {
        return detail::errHandler(
            ::clSetProgramReleaseCallback(
                object_,
                pfn_notify,
                user_data),
            __SET_PROGRAM_RELEASE_CALLBACK_ERR);
    }
#endif // #if defined(CL_USE_DEPRECATED_OPENCL_2_2_APIS)

    /*! \brief Sets a SPIR-V specialization constant.
     *
     *  Wraps clSetProgramSpecializationConstant().
     */
    template &lt;typename T&gt;
    typename std::enable_if&lt;!std::is_pointer&lt;T&gt;::value, cl_int&gt;::type
        setSpecializationConstant(cl_uint index, const T &amp;value)
    {
        return detail::errHandler(
            ::clSetProgramSpecializationConstant(
                object_,
                index,
                sizeof(value),
                &amp;value),
            __SET_PROGRAM_SPECIALIZATION_CONSTANT_ERR);
    }

    /*! \brief Sets a SPIR-V specialization constant.
     *
     *  Wraps clSetProgramSpecializationConstant().
     */
    cl_int setSpecializationConstant(cl_uint index, size_type size, const void* value)
    {
        return detail::errHandler(
            ::clSetProgramSpecializationConstant(
                object_,
                index,
                size,
                value),
            __SET_PROGRAM_SPECIALIZATION_CONSTANT_ERR);
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 220
};

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
inline Program linkProgram(
    Program input1,
    Program input2,
    const char* options = NULL,
    void (CL_CALLBACK * notifyFptr)(cl_program, void *) = NULL,
    void* data = NULL,
    cl_int* err = NULL) 
{
    cl_int error_local = CL_SUCCESS;

    cl_program programs[2] = { input1(), input2() };

    Context ctx = input1.getInfo&lt;CL_PROGRAM_CONTEXT&gt;(&amp;error_local);
    if(error_local!=CL_SUCCESS) {
        detail::errHandler(error_local, __LINK_PROGRAM_ERR);
    }

    cl_program prog = ::clLinkProgram(
        ctx(),
        0,
        NULL,
        options,
        2,
        programs,
        notifyFptr,
        data,
        &amp;error_local);

    detail::errHandler(error_local,__COMPILE_PROGRAM_ERR);
    if (err != NULL) {
        *err = error_local;
    }

    return Program(prog);
}

inline Program linkProgram(
    vector&lt;Program&gt; inputPrograms,
    const char* options = NULL,
    void (CL_CALLBACK * notifyFptr)(cl_program, void *) = NULL,
    void* data = NULL,
    cl_int* err = NULL) 
{
    cl_int error_local = CL_SUCCESS;

    vector&lt;cl_program&gt; programs(inputPrograms.size());

    for (unsigned int i = 0; i &lt; inputPrograms.size(); i++) {
        programs[i] = inputPrograms[i]();
    }
    
    Context ctx;
    if(inputPrograms.size() &gt; 0) {
        ctx = inputPrograms[0].getInfo&lt;CL_PROGRAM_CONTEXT&gt;(&amp;error_local);
        if(error_local!=CL_SUCCESS) {
            detail::errHandler(error_local, __LINK_PROGRAM_ERR);
        }
    }
    cl_program prog = ::clLinkProgram(
        ctx(),
        0,
        NULL,
        options,
        (cl_uint)inputPrograms.size(),
        programs.data(),
        notifyFptr,
        data,
        &amp;error_local);

    detail::errHandler(error_local,__COMPILE_PROGRAM_ERR);
    if (err != NULL) {
        *err = error_local;
    }

    return Program(prog, false);
}
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120

// Template specialization for CL_PROGRAM_BINARIES
template &lt;&gt;
inline cl_int cl::Program::getInfo(cl_program_info name, vector&lt;vector&lt;unsigned char&gt;&gt;* param) const
{
    if (name != CL_PROGRAM_BINARIES) {
        return CL_INVALID_VALUE;
    }
    if (param) {
        // Resize the parameter array appropriately for each allocation
        // and pass down to the helper

        vector&lt;size_type&gt; sizes = getInfo&lt;CL_PROGRAM_BINARY_SIZES&gt;();
        size_type numBinaries = sizes.size();

        // Resize the parameter array and constituent arrays
        param-&gt;resize(numBinaries);
        for (size_type i = 0; i &lt; numBinaries; ++i) {
            (*param)[i].resize(sizes[i]);
        }

        return detail::errHandler(
            detail::getInfo(&amp;::clGetProgramInfo, object_, name, param),
            __GET_PROGRAM_INFO_ERR);
    }

    return CL_SUCCESS;
}

template&lt;&gt;
inline vector&lt;vector&lt;unsigned char&gt;&gt; cl::Program::getInfo&lt;CL_PROGRAM_BINARIES&gt;(cl_int* err) const
{
    vector&lt;vector&lt;unsigned char&gt;&gt; binariesVectors;

    cl_int result = getInfo(CL_PROGRAM_BINARIES, &amp;binariesVectors);
    if (err != NULL) {
        *err = result;
    }
    return binariesVectors;
}

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 220
// Template specialization for clSetProgramSpecializationConstant
template &lt;&gt;
inline cl_int cl::Program::setSpecializationConstant(cl_uint index, const bool &amp;value)
{
    cl_uchar ucValue = value ? CL_UCHAR_MAX : 0;
    return detail::errHandler(
        ::clSetProgramSpecializationConstant(
            object_,
            index,
            sizeof(ucValue),
            &amp;ucValue),
        __SET_PROGRAM_SPECIALIZATION_CONSTANT_ERR);
}
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 220

inline Kernel::Kernel(const Program&amp; program, const char* name, cl_int* err)
{
    cl_int error;

    object_ = ::clCreateKernel(program(), name, &amp;error);
    detail::errHandler(error, __CREATE_KERNEL_ERR);

    if (err != NULL) {
        *err = error;
    }

}

enum class QueueProperties : cl_command_queue_properties
{
    None = 0,
    Profiling = CL_QUEUE_PROFILING_ENABLE,
    OutOfOrder = CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
};

inline QueueProperties operator|(QueueProperties lhs, QueueProperties rhs)
{
    return static_cast&lt;QueueProperties&gt;(static_cast&lt;cl_command_queue_properties&gt;(lhs) | static_cast&lt;cl_command_queue_properties&gt;(rhs));
}

/*! \class CommandQueue
 * \brief CommandQueue interface for cl_command_queue.
 */
class CommandQueue : public detail::Wrapper&lt;cl_command_queue&gt;
{
private:
    static std::once_flag default_initialized_;
    static CommandQueue default_;
    static cl_int default_error_;

    /*! \brief Create the default command queue returned by @ref getDefault.
     *
     * It sets default_error_ to indicate success or failure. It does not throw
     * @c cl::Error.
     */
    static void makeDefault()
    {
        /* We don't want to throw an error from this function, so we have to
         * catch and set the error flag.
         */
#if defined(CL_HPP_ENABLE_EXCEPTIONS)
        try
#endif
        {
            int error;
            Context context = Context::getDefault(&amp;error);

            if (error != CL_SUCCESS) {
                default_error_ = error;
            }
            else {
                Device device = Device::getDefault();
                default_ = CommandQueue(context, device, 0, &amp;default_error_);
            }
        }
#if defined(CL_HPP_ENABLE_EXCEPTIONS)
        catch (cl::Error &amp;e) {
            default_error_ = e.err();
        }
#endif
    }

    /*! \brief Create the default command queue.
     *
     * This sets @c default_. It does not throw
     * @c cl::Error.
     */
    static void makeDefaultProvided(const CommandQueue &amp;c) {
        default_ = c;
    }

public:
#ifdef CL_HPP_UNIT_TEST_ENABLE
    /*! \brief Reset the default.
    *
    * This sets @c default_ to an empty value to support cleanup in
    * the unit test framework.
    * This function is not thread safe.
    */
    static void unitTestClearDefault() {
        default_ = CommandQueue();
    }
#endif // #ifdef CL_HPP_UNIT_TEST_ENABLE
        

    /*!
     * \brief Constructs a CommandQueue based on passed properties.
     * Will return an CL_INVALID_QUEUE_PROPERTIES error if CL_QUEUE_ON_DEVICE is specified.
     */
   CommandQueue(
        cl_command_queue_properties properties,
        cl_int* err = NULL)
    {
        cl_int error;

        Context context = Context::getDefault(&amp;error);
        detail::errHandler(error, __CREATE_CONTEXT_ERR);

        if (error != CL_SUCCESS) {
            if (err != NULL) {
                *err = error;
            }
        }
        else {
            Device device = context.getInfo&lt;CL_CONTEXT_DEVICES&gt;()[0];
            bool useWithProperties;

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
            // Run-time decision based on the actual platform
            {
                cl_uint version = detail::getContextPlatformVersion(context());
                useWithProperties = (version &gt;= 0x20000); // OpenCL 2.0 or above
            }
#elif CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
            useWithProperties = true;
#else
            useWithProperties = false;
#endif

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
            if (useWithProperties) {
                cl_queue_properties queue_properties[] = {
                    CL_QUEUE_PROPERTIES, properties, 0 };
                if ((properties &amp; CL_QUEUE_ON_DEVICE) == 0) {
                    object_ = ::clCreateCommandQueueWithProperties(
                        context(), device(), queue_properties, &amp;error);
                }
                else {
                    error = CL_INVALID_QUEUE_PROPERTIES;
                }

                detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
                if (err != NULL) {
                    *err = error;
                }
            }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
            if (!useWithProperties) {
                object_ = ::clCreateCommandQueue(
                    context(), device(), properties, &amp;error);

                detail::errHandler(error, __CREATE_COMMAND_QUEUE_ERR);
                if (err != NULL) {
                    *err = error;
                }
            }
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
        }
    }

   /*!
    * \brief Constructs a CommandQueue based on passed properties.
    * Will return an CL_INVALID_QUEUE_PROPERTIES error if CL_QUEUE_ON_DEVICE is specified.
    */
   CommandQueue(
       QueueProperties properties,
       cl_int* err = NULL)
   {
       cl_int error;

       Context context = Context::getDefault(&amp;error);
       detail::errHandler(error, __CREATE_CONTEXT_ERR);

       if (error != CL_SUCCESS) {
           if (err != NULL) {
               *err = error;
           }
       }
       else {
           Device device = context.getInfo&lt;CL_CONTEXT_DEVICES&gt;()[0];
           bool useWithProperties;

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
           // Run-time decision based on the actual platform
           {
               cl_uint version = detail::getContextPlatformVersion(context());
               useWithProperties = (version &gt;= 0x20000); // OpenCL 2.0 or above
           }
#elif CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
           useWithProperties = true;
#else
           useWithProperties = false;
#endif

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
           if (useWithProperties) {
               cl_queue_properties queue_properties[] = {
                   CL_QUEUE_PROPERTIES, static_cast&lt;cl_queue_properties&gt;(properties), 0 };

               object_ = ::clCreateCommandQueueWithProperties(
                   context(), device(), queue_properties, &amp;error);

               detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
               if (err != NULL) {
                   *err = error;
               }
           }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
           if (!useWithProperties) {
               object_ = ::clCreateCommandQueue(
                   context(), device(), static_cast&lt;cl_command_queue_properties&gt;(properties), &amp;error);

               detail::errHandler(error, __CREATE_COMMAND_QUEUE_ERR);
               if (err != NULL) {
                   *err = error;
               }
           }
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200

       }
   }

    /*!
     * \brief Constructs a CommandQueue for an implementation defined device in the given context
     * Will return an CL_INVALID_QUEUE_PROPERTIES error if CL_QUEUE_ON_DEVICE is specified.
     */
    explicit CommandQueue(
        const Context&amp; context,
        cl_command_queue_properties properties = 0,
        cl_int* err = NULL)
    {
        cl_int error;
        bool useWithProperties;
        vector&lt;cl::Device&gt; devices;
        error = context.getInfo(CL_CONTEXT_DEVICES, &amp;devices);

        detail::errHandler(error, __CREATE_CONTEXT_ERR);

        if (error != CL_SUCCESS)
        {
            if (err != NULL) {
                *err = error;
            }
            return;
        }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
        // Run-time decision based on the actual platform
        {
            cl_uint version = detail::getContextPlatformVersion(context());
            useWithProperties = (version &gt;= 0x20000); // OpenCL 2.0 or above
        }
#elif CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        useWithProperties = true;
#else
        useWithProperties = false;
#endif

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        if (useWithProperties) {
            cl_queue_properties queue_properties[] = {
                CL_QUEUE_PROPERTIES, properties, 0 };
            if ((properties &amp; CL_QUEUE_ON_DEVICE) == 0) {
                object_ = ::clCreateCommandQueueWithProperties(
                    context(), devices[0](), queue_properties, &amp;error);
            }
            else {
                error = CL_INVALID_QUEUE_PROPERTIES;
            }

            detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
        if (!useWithProperties) {
            object_ = ::clCreateCommandQueue(
                context(), devices[0](), properties, &amp;error);

            detail::errHandler(error, __CREATE_COMMAND_QUEUE_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
    }

    /*!
    * \brief Constructs a CommandQueue for an implementation defined device in the given context
    * Will return an CL_INVALID_QUEUE_PROPERTIES error if CL_QUEUE_ON_DEVICE is specified.
    */
    explicit CommandQueue(
        const Context&amp; context,
        QueueProperties properties,
        cl_int* err = NULL)
    {
        cl_int error;
        bool useWithProperties;
        vector&lt;cl::Device&gt; devices;
        error = context.getInfo(CL_CONTEXT_DEVICES, &amp;devices);

        detail::errHandler(error, __CREATE_CONTEXT_ERR);

        if (error != CL_SUCCESS)
        {
            if (err != NULL) {
                *err = error;
            }
            return;
        }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
        // Run-time decision based on the actual platform
        {
            cl_uint version = detail::getContextPlatformVersion(context());
            useWithProperties = (version &gt;= 0x20000); // OpenCL 2.0 or above
        }
#elif CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        useWithProperties = true;
#else
        useWithProperties = false;
#endif

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        if (useWithProperties) {
            cl_queue_properties queue_properties[] = {
                CL_QUEUE_PROPERTIES, static_cast&lt;cl_queue_properties&gt;(properties), 0 };
            object_ = ::clCreateCommandQueueWithProperties(
                context(), devices[0](), queue_properties, &amp;error);

            detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
        if (!useWithProperties) {
            object_ = ::clCreateCommandQueue(
                context(), devices[0](), static_cast&lt;cl_command_queue_properties&gt;(properties), &amp;error);

            detail::errHandler(error, __CREATE_COMMAND_QUEUE_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
    }

    /*!
     * \brief Constructs a CommandQueue for a passed device and context
     * Will return an CL_INVALID_QUEUE_PROPERTIES error if CL_QUEUE_ON_DEVICE is specified.
     */
    CommandQueue(
        const Context&amp; context,
        const Device&amp; device,
        cl_command_queue_properties properties = 0,
        cl_int* err = NULL)
    {
        cl_int error;
        bool useWithProperties;

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
        // Run-time decision based on the actual platform
        {
            cl_uint version = detail::getContextPlatformVersion(context());
            useWithProperties = (version &gt;= 0x20000); // OpenCL 2.0 or above
        }
#elif CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        useWithProperties = true;
#else
        useWithProperties = false;
#endif

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        if (useWithProperties) {
            cl_queue_properties queue_properties[] = {
                CL_QUEUE_PROPERTIES, properties, 0 };
            object_ = ::clCreateCommandQueueWithProperties(
                context(), device(), queue_properties, &amp;error);

            detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
        if (!useWithProperties) {
            object_ = ::clCreateCommandQueue(
                context(), device(), properties, &amp;error);

            detail::errHandler(error, __CREATE_COMMAND_QUEUE_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
    }

    /*!
     * \brief Constructs a CommandQueue for a passed device and context
     * Will return an CL_INVALID_QUEUE_PROPERTIES error if CL_QUEUE_ON_DEVICE is specified.
     */
    CommandQueue(
        const Context&amp; context,
        const Device&amp; device,
        QueueProperties properties,
        cl_int* err = NULL)
    {
        cl_int error;
        bool useWithProperties;

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200 &amp;&amp; CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
        // Run-time decision based on the actual platform
        {
            cl_uint version = detail::getContextPlatformVersion(context());
            useWithProperties = (version &gt;= 0x20000); // OpenCL 2.0 or above
        }
#elif CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        useWithProperties = true;
#else
        useWithProperties = false;
#endif

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        if (useWithProperties) {
            cl_queue_properties queue_properties[] = {
                CL_QUEUE_PROPERTIES, static_cast&lt;cl_queue_properties&gt;(properties), 0 };
            object_ = ::clCreateCommandQueueWithProperties(
                context(), device(), queue_properties, &amp;error);

            detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
#if CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
        if (!useWithProperties) {
            object_ = ::clCreateCommandQueue(
                context(), device(), static_cast&lt;cl_command_queue_properties&gt;(properties), &amp;error);

            detail::errHandler(error, __CREATE_COMMAND_QUEUE_ERR);
            if (err != NULL) {
                *err = error;
            }
        }
#endif // CL_HPP_MINIMUM_OPENCL_VERSION &lt; 200
    }

    static CommandQueue getDefault(cl_int * err = NULL) 
    {
        std::call_once(default_initialized_, makeDefault);
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        detail::errHandler(default_error_, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
#else // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        detail::errHandler(default_error_, __CREATE_COMMAND_QUEUE_ERR);
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
        if (err != NULL) {
            *err = default_error_;
        }
        return default_;
    }

    /**
     * Modify the default command queue to be used by
     * subsequent operations.
     * Will only set the default if no default was previously created.
     * @return updated default command queue.
     *         Should be compared to the passed value to ensure that it was updated.
     */
    static CommandQueue setDefault(const CommandQueue &amp;default_queue)
    {
        std::call_once(default_initialized_, makeDefaultProvided, std::cref(default_queue));
        detail::errHandler(default_error_);
        return default_;
    }

    CommandQueue() { }


    /*! \brief Constructor from cl_command_queue - takes ownership.
     *
     * \param retainObject will cause the constructor to retain its cl object.
     *                     Defaults to false to maintain compatibility with
     *                     earlier versions.
     */
    explicit CommandQueue(const cl_command_queue&amp; commandQueue, bool retainObject = false) : 
        detail::Wrapper&lt;cl_type&gt;(commandQueue, retainObject) { }

    CommandQueue&amp; operator = (const cl_command_queue&amp; rhs)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    CommandQueue(const CommandQueue&amp; queue) : detail::Wrapper&lt;cl_type&gt;(queue) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    CommandQueue&amp; operator = (const CommandQueue &amp;queue)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(queue);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    CommandQueue(CommandQueue&amp;&amp; queue) CL_HPP_NOEXCEPT_ : detail::Wrapper&lt;cl_type&gt;(std::move(queue)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    CommandQueue&amp; operator = (CommandQueue &amp;&amp;queue)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(std::move(queue));
        return *this;
    }

    template &lt;typename T&gt;
    cl_int getInfo(cl_command_queue_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(
                &amp;::clGetCommandQueueInfo, object_, name, param),
                __GET_COMMAND_QUEUE_INFO_ERR);
    }

    template &lt;cl_command_queue_info name&gt; typename
    detail::param_traits&lt;detail::cl_command_queue_info, name&gt;::param_type
    getInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_command_queue_info, name&gt;::param_type param;
        cl_int result = getInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }

    cl_int enqueueReadBuffer(
        const Buffer&amp; buffer,
        cl_bool blocking,
        size_type offset,
        size_type size,
        void* ptr,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueReadBuffer(
                object_, buffer(), blocking, offset, size,
                ptr,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_READ_BUFFER_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    cl_int enqueueWriteBuffer(
        const Buffer&amp; buffer,
        cl_bool blocking,
        size_type offset,
        size_type size,
        const void* ptr,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueWriteBuffer(
                object_, buffer(), blocking, offset, size,
                ptr,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
                __ENQUEUE_WRITE_BUFFER_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    cl_int enqueueCopyBuffer(
        const Buffer&amp; src,
        const Buffer&amp; dst,
        size_type src_offset,
        size_type dst_offset,
        size_type size,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueCopyBuffer(
                object_, src(), dst(), src_offset, dst_offset, size,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQEUE_COPY_BUFFER_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
    cl_int enqueueReadBufferRect(
        const Buffer&amp; buffer,
        cl_bool blocking,
        const array&lt;size_type, 3&gt;&amp; buffer_offset,
        const array&lt;size_type, 3&gt;&amp; host_offset,
        const array&lt;size_type, 3&gt;&amp; region,
        size_type buffer_row_pitch,
        size_type buffer_slice_pitch,
        size_type host_row_pitch,
        size_type host_slice_pitch,
        void *ptr,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueReadBufferRect(
                object_, 
                buffer(), 
                blocking,
                buffer_offset.data(),
                host_offset.data(),
                region.data(),
                buffer_row_pitch,
                buffer_slice_pitch,
                host_row_pitch,
                host_slice_pitch,
                ptr,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
                __ENQUEUE_READ_BUFFER_RECT_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    cl_int enqueueWriteBufferRect(
        const Buffer&amp; buffer,
        cl_bool blocking,
        const array&lt;size_type, 3&gt;&amp; buffer_offset,
        const array&lt;size_type, 3&gt;&amp; host_offset,
        const array&lt;size_type, 3&gt;&amp; region,
        size_type buffer_row_pitch,
        size_type buffer_slice_pitch,
        size_type host_row_pitch,
        size_type host_slice_pitch,
        const void *ptr,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueWriteBufferRect(
                object_, 
                buffer(), 
                blocking,
                buffer_offset.data(),
                host_offset.data(),
                region.data(),
                buffer_row_pitch,
                buffer_slice_pitch,
                host_row_pitch,
                host_slice_pitch,
                ptr,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
                __ENQUEUE_WRITE_BUFFER_RECT_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    cl_int enqueueCopyBufferRect(
        const Buffer&amp; src,
        const Buffer&amp; dst,
        const array&lt;size_type, 3&gt;&amp; src_origin,
        const array&lt;size_type, 3&gt;&amp; dst_origin,
        const array&lt;size_type, 3&gt;&amp; region,
        size_type src_row_pitch,
        size_type src_slice_pitch,
        size_type dst_row_pitch,
        size_type dst_slice_pitch,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueCopyBufferRect(
                object_, 
                src(), 
                dst(), 
                src_origin.data(),
                dst_origin.data(),
                region.data(),
                src_row_pitch,
                src_slice_pitch,
                dst_row_pitch,
                dst_slice_pitch,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQEUE_COPY_BUFFER_RECT_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
    /**
     * Enqueue a command to fill a buffer object with a pattern
     * of a given size. The pattern is specified as a vector type.
     * \tparam PatternType The datatype of the pattern field. 
     *     The pattern type must be an accepted OpenCL data type.
     * \tparam offset Is the offset in bytes into the buffer at 
     *     which to start filling. This must be a multiple of 
     *     the pattern size.
     * \tparam size Is the size in bytes of the region to fill.
     *     This must be a multiple of the pattern size.
     */
    template&lt;typename PatternType&gt;
    cl_int enqueueFillBuffer(
        const Buffer&amp; buffer,
        PatternType pattern,
        size_type offset,
        size_type size,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueFillBuffer(
                object_, 
                buffer(),
                static_cast&lt;void*&gt;(&amp;pattern),
                sizeof(PatternType), 
                offset, 
                size,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
                __ENQUEUE_FILL_BUFFER_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120

    cl_int enqueueReadImage(
        const Image&amp; image,
        cl_bool blocking,
        const array&lt;size_type, 3&gt;&amp; origin,
        const array&lt;size_type, 3&gt;&amp; region,
        size_type row_pitch,
        size_type slice_pitch,
        void* ptr,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueReadImage(
                object_, 
                image(), 
                blocking, 
                origin.data(),
                region.data(), 
                row_pitch, 
                slice_pitch, 
                ptr,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_READ_IMAGE_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    cl_int enqueueWriteImage(
        const Image&amp; image,
        cl_bool blocking,
        const array&lt;size_type, 3&gt;&amp; origin,
        const array&lt;size_type, 3&gt;&amp; region,
        size_type row_pitch,
        size_type slice_pitch,
        const void* ptr,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueWriteImage(
                object_, 
                image(), 
                blocking, 
                origin.data(),
                region.data(), 
                row_pitch, 
                slice_pitch, 
                ptr,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_WRITE_IMAGE_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    cl_int enqueueCopyImage(
        const Image&amp; src,
        const Image&amp; dst,
        const array&lt;size_type, 3&gt;&amp; src_origin,
        const array&lt;size_type, 3&gt;&amp; dst_origin,
        const array&lt;size_type, 3&gt;&amp; region,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueCopyImage(
                object_, 
                src(), 
                dst(), 
                src_origin.data(),
                dst_origin.data(), 
                region.data(),
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_COPY_IMAGE_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
    /**
     * Enqueue a command to fill an image object with a specified color.
     * \param fillColor is the color to use to fill the image.
     *     This is a four component RGBA floating-point color value if
     *     the image channel data type is not an unnormalized signed or
     *     unsigned data type.
     */
    cl_int enqueueFillImage(
        const Image&amp; image,
        cl_float4 fillColor,
        const array&lt;size_type, 3&gt;&amp; origin,
        const array&lt;size_type, 3&gt;&amp; region,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueFillImage(
                object_, 
                image(),
                static_cast&lt;void*&gt;(&amp;fillColor), 
                origin.data(),
                region.data(),
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
                __ENQUEUE_FILL_IMAGE_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    /**
     * Enqueue a command to fill an image object with a specified color.
     * \param fillColor is the color to use to fill the image.
     *     This is a four component RGBA signed integer color value if
     *     the image channel data type is an unnormalized signed integer
     *     type.
     */
    cl_int enqueueFillImage(
        const Image&amp; image,
        cl_int4 fillColor,
        const array&lt;size_type, 3&gt;&amp; origin,
        const array&lt;size_type, 3&gt;&amp; region,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueFillImage(
                object_, 
                image(),
                static_cast&lt;void*&gt;(&amp;fillColor), 
                origin.data(),
                region.data(),
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
                __ENQUEUE_FILL_IMAGE_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    /**
     * Enqueue a command to fill an image object with a specified color.
     * \param fillColor is the color to use to fill the image.
     *     This is a four component RGBA unsigned integer color value if
     *     the image channel data type is an unnormalized unsigned integer
     *     type.
     */
    cl_int enqueueFillImage(
        const Image&amp; image,
        cl_uint4 fillColor,
        const array&lt;size_type, 3&gt;&amp; origin,
        const array&lt;size_type, 3&gt;&amp; region,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueFillImage(
                object_, 
                image(),
                static_cast&lt;void*&gt;(&amp;fillColor), 
                origin.data(),
                region.data(),
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
                __ENQUEUE_FILL_IMAGE_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120

    cl_int enqueueCopyImageToBuffer(
        const Image&amp; src,
        const Buffer&amp; dst,
        const array&lt;size_type, 3&gt;&amp; src_origin,
        const array&lt;size_type, 3&gt;&amp; region,
        size_type dst_offset,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueCopyImageToBuffer(
                object_, 
                src(), 
                dst(), 
                src_origin.data(),
                region.data(), 
                dst_offset,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_COPY_IMAGE_TO_BUFFER_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    cl_int enqueueCopyBufferToImage(
        const Buffer&amp; src,
        const Image&amp; dst,
        size_type src_offset,
        const array&lt;size_type, 3&gt;&amp; dst_origin,
        const array&lt;size_type, 3&gt;&amp; region,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueCopyBufferToImage(
                object_, 
                src(), 
                dst(), 
                src_offset,
                dst_origin.data(), 
                region.data(),
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_COPY_BUFFER_TO_IMAGE_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    void* enqueueMapBuffer(
        const Buffer&amp; buffer,
        cl_bool blocking,
        cl_map_flags flags,
        size_type offset,
        size_type size,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL,
        cl_int* err = NULL) const
    {
        cl_event tmp;
        cl_int error;
        void * result = ::clEnqueueMapBuffer(
            object_, buffer(), blocking, flags, offset, size,
            (events != NULL) ? (cl_uint) events-&gt;size() : 0,
            (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
            (event != NULL) ? &amp;tmp : NULL,
            &amp;error);

        detail::errHandler(error, __ENQUEUE_MAP_BUFFER_ERR);
        if (err != NULL) {
            *err = error;
        }
        if (event != NULL &amp;&amp; error == CL_SUCCESS)
            *event = tmp;

        return result;
    }

    void* enqueueMapImage(
        const Image&amp; buffer,
        cl_bool blocking,
        cl_map_flags flags,
        const array&lt;size_type, 3&gt;&amp; origin,
        const array&lt;size_type, 3&gt;&amp; region,
        size_type * row_pitch,
        size_type * slice_pitch,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL,
        cl_int* err = NULL) const
    {
        cl_event tmp;
        cl_int error;
        void * result = ::clEnqueueMapImage(
            object_, buffer(), blocking, flags,
            origin.data(), 
            region.data(),
            row_pitch, slice_pitch,
            (events != NULL) ? (cl_uint) events-&gt;size() : 0,
            (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
            (event != NULL) ? &amp;tmp : NULL,
            &amp;error);

        detail::errHandler(error, __ENQUEUE_MAP_IMAGE_ERR);
        if (err != NULL) {
              *err = error;
        }
        if (event != NULL &amp;&amp; error == CL_SUCCESS)
            *event = tmp;
        return result;
    }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
    /**
     * Enqueues a command that will allow the host to update a region of a coarse-grained SVM buffer.
     * This variant takes a raw SVM pointer.
     */
    template&lt;typename T&gt;
    cl_int enqueueMapSVM(
        T* ptr,
        cl_bool blocking,
        cl_map_flags flags,
        size_type size,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(::clEnqueueSVMMap(
            object_, blocking, flags, static_cast&lt;void*&gt;(ptr), size,
            (events != NULL) ? (cl_uint)events-&gt;size() : 0,
            (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*)&amp;events-&gt;front() : NULL,
            (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_MAP_BUFFER_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }


    /**
     * Enqueues a command that will allow the host to update a region of a coarse-grained SVM buffer.
     * This variant takes a cl::pointer instance.
     */
    template&lt;typename T, class D&gt;
    cl_int enqueueMapSVM(
        cl::pointer&lt;T, D&gt; &amp;ptr,
        cl_bool blocking,
        cl_map_flags flags,
        size_type size,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(::clEnqueueSVMMap(
            object_, blocking, flags, static_cast&lt;void*&gt;(ptr.get()), size,
            (events != NULL) ? (cl_uint)events-&gt;size() : 0,
            (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*)&amp;events-&gt;front() : NULL,
            (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_MAP_BUFFER_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    /**
     * Enqueues a command that will allow the host to update a region of a coarse-grained SVM buffer.
     * This variant takes a cl::vector instance.
     */
    template&lt;typename T, class Alloc&gt;
    cl_int enqueueMapSVM(
        cl::vector&lt;T, Alloc&gt; &amp;container,
        cl_bool blocking,
        cl_map_flags flags,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(::clEnqueueSVMMap(
            object_, blocking, flags, static_cast&lt;void*&gt;(container.data()), container.size()*sizeof(T),
            (events != NULL) ? (cl_uint)events-&gt;size() : 0,
            (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*)&amp;events-&gt;front() : NULL,
            (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_MAP_BUFFER_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200

    cl_int enqueueUnmapMemObject(
        const Memory&amp; memory,
        void* mapped_ptr,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueUnmapMemObject(
                object_, memory(), mapped_ptr,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_UNMAP_MEM_OBJECT_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }


#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
    /**
     * Enqueues a command that will release a coarse-grained SVM buffer back to the OpenCL runtime.
     * This variant takes a raw SVM pointer.
     */
    template&lt;typename T&gt;
    cl_int enqueueUnmapSVM(
        T* ptr,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueSVMUnmap(
            object_, static_cast&lt;void*&gt;(ptr),
            (events != NULL) ? (cl_uint)events-&gt;size() : 0,
            (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*)&amp;events-&gt;front() : NULL,
            (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_UNMAP_MEM_OBJECT_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    /**
     * Enqueues a command that will release a coarse-grained SVM buffer back to the OpenCL runtime.
     * This variant takes a cl::pointer instance.
     */
    template&lt;typename T, class D&gt;
    cl_int enqueueUnmapSVM(
        cl::pointer&lt;T, D&gt; &amp;ptr,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueSVMUnmap(
            object_, static_cast&lt;void*&gt;(ptr.get()),
            (events != NULL) ? (cl_uint)events-&gt;size() : 0,
            (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*)&amp;events-&gt;front() : NULL,
            (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_UNMAP_MEM_OBJECT_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    /**
     * Enqueues a command that will release a coarse-grained SVM buffer back to the OpenCL runtime.
     * This variant takes a cl::vector instance.
     */
    template&lt;typename T, class Alloc&gt;
    cl_int enqueueUnmapSVM(
        cl::vector&lt;T, Alloc&gt; &amp;container,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueSVMUnmap(
            object_, static_cast&lt;void*&gt;(container.data()),
            (events != NULL) ? (cl_uint)events-&gt;size() : 0,
            (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*)&amp;events-&gt;front() : NULL,
            (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_UNMAP_MEM_OBJECT_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
    /**
     * Enqueues a marker command which waits for either a list of events to complete, 
     * or all previously enqueued commands to complete.
     *
     * Enqueues a marker command which waits for either a list of events to complete, 
     * or if the list is empty it waits for all commands previously enqueued in command_queue 
     * to complete before it completes. This command returns an event which can be waited on, 
     * i.e. this event can be waited on to insure that all events either in the event_wait_list 
     * or all previously enqueued commands, queued before this command to command_queue, 
     * have completed.
     */
    cl_int enqueueMarkerWithWaitList(
        const vector&lt;Event&gt; *events = 0,
        Event *event = 0) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueMarkerWithWaitList(
                object_,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_MARKER_WAIT_LIST_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    /**
     * A synchronization point that enqueues a barrier operation.
     *
     * Enqueues a barrier command which waits for either a list of events to complete, 
     * or if the list is empty it waits for all commands previously enqueued in command_queue 
     * to complete before it completes. This command blocks command execution, that is, any 
     * following commands enqueued after it do not execute until it completes. This command 
     * returns an event which can be waited on, i.e. this event can be waited on to insure that 
     * all events either in the event_wait_list or all previously enqueued commands, queued 
     * before this command to command_queue, have completed.
     */
    cl_int enqueueBarrierWithWaitList(
        const vector&lt;Event&gt; *events = 0,
        Event *event = 0) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueBarrierWithWaitList(
                object_,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_BARRIER_WAIT_LIST_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }
    
    /**
     * Enqueues a command to indicate with which device a set of memory objects
     * should be associated.
     */
    cl_int enqueueMigrateMemObjects(
        const vector&lt;Memory&gt; &amp;memObjects,
        cl_mem_migration_flags flags,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL
        ) const
    {
        cl_event tmp;
        
        vector&lt;cl_mem&gt; localMemObjects(memObjects.size());

        for( int i = 0; i &lt; (int)memObjects.size(); ++i ) {
            localMemObjects[i] = memObjects[i]();
        }
        
        cl_int err = detail::errHandler(
            ::clEnqueueMigrateMemObjects(
                object_, 
                (cl_uint)memObjects.size(), 
                localMemObjects.data(),
                flags,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_UNMAP_MEM_OBJECT_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120


#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
    /**
     * Enqueues a command that will allow the host associate ranges within a set of
     * SVM allocations with a device.
     * @param sizes - The length from each pointer to migrate.
     */
    template&lt;typename T&gt;
    cl_int enqueueMigrateSVM(
        const cl::vector&lt;T*&gt; &amp;svmRawPointers,
        const cl::vector&lt;size_type&gt; &amp;sizes,
        cl_mem_migration_flags flags = 0,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(::clEnqueueSVMMigrateMem(
            object_,
            svmRawPointers.size(), static_cast&lt;void**&gt;(svmRawPointers.data()),
            sizes.data(), // array of sizes not passed
            flags,
            (events != NULL) ? (cl_uint)events-&gt;size() : 0,
            (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*)&amp;events-&gt;front() : NULL,
            (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_MIGRATE_SVM_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    /**
     * Enqueues a command that will allow the host associate a set of SVM allocations with
     * a device.
     */
    template&lt;typename T&gt;
    cl_int enqueueMigrateSVM(
        const cl::vector&lt;T*&gt; &amp;svmRawPointers,
        cl_mem_migration_flags flags = 0,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        return enqueueMigrateSVM(svmRawPointers, cl::vector&lt;size_type&gt;(svmRawPointers.size()), flags, events, event);
    }


    /**
     * Enqueues a command that will allow the host associate ranges within a set of
     * SVM allocations with a device.
     * @param sizes - The length from each pointer to migrate.
     */
    template&lt;typename T, class D&gt;
    cl_int enqueueMigrateSVM(
        const cl::vector&lt;cl::pointer&lt;T, D&gt;&gt; &amp;svmPointers,
        const cl::vector&lt;size_type&gt; &amp;sizes,
        cl_mem_migration_flags flags = 0,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl::vector&lt;void*&gt; svmRawPointers;
        svmRawPointers.reserve(svmPointers.size());
        for (auto p : svmPointers) {
            svmRawPointers.push_back(static_cast&lt;void*&gt;(p.get()));
        }

        return enqueueMigrateSVM(svmRawPointers, sizes, flags, events, event);
    }


    /**
     * Enqueues a command that will allow the host associate a set of SVM allocations with
     * a device.
     */
    template&lt;typename T, class D&gt;
    cl_int enqueueMigrateSVM(
        const cl::vector&lt;cl::pointer&lt;T, D&gt;&gt; &amp;svmPointers,
        cl_mem_migration_flags flags = 0,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        return enqueueMigrateSVM(svmPointers, cl::vector&lt;size_type&gt;(svmPointers.size()), flags, events, event);
    }

    /**
     * Enqueues a command that will allow the host associate ranges within a set of
     * SVM allocations with a device.
     * @param sizes - The length from the beginning of each container to migrate.
     */
    template&lt;typename T, class Alloc&gt;
    cl_int enqueueMigrateSVM(
        const cl::vector&lt;cl::vector&lt;T, Alloc&gt;&gt; &amp;svmContainers,
        const cl::vector&lt;size_type&gt; &amp;sizes,
        cl_mem_migration_flags flags = 0,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl::vector&lt;void*&gt; svmRawPointers;
        svmRawPointers.reserve(svmContainers.size());
        for (auto p : svmContainers) {
            svmRawPointers.push_back(static_cast&lt;void*&gt;(p.data()));
        }

        return enqueueMigrateSVM(svmRawPointers, sizes, flags, events, event);
    }

    /**
     * Enqueues a command that will allow the host associate a set of SVM allocations with
     * a device.
     */
    template&lt;typename T, class Alloc&gt;
    cl_int enqueueMigrateSVM(
        const cl::vector&lt;cl::vector&lt;T, Alloc&gt;&gt; &amp;svmContainers,
        cl_mem_migration_flags flags = 0,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        return enqueueMigrateSVM(svmContainers, cl::vector&lt;size_type&gt;(svmContainers.size()), flags, events, event);
    }

#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
    
    cl_int enqueueNDRangeKernel(
        const Kernel&amp; kernel,
        const NDRange&amp; offset,
        const NDRange&amp; global,
        const NDRange&amp; local = NullRange,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueNDRangeKernel(
                object_, kernel(), (cl_uint) global.dimensions(),
                offset.dimensions() != 0 ? (const size_type*) offset : NULL,
                (const size_type*) global,
                local.dimensions() != 0 ? (const size_type*) local : NULL,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_NDRANGE_KERNEL_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

#if defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)
    CL_EXT_PREFIX__VERSION_1_2_DEPRECATED cl_int enqueueTask(
        const Kernel&amp; kernel,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const CL_EXT_SUFFIX__VERSION_1_2_DEPRECATED
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueTask(
                object_, kernel(),
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_TASK_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }
#endif // #if defined(CL_USE_DEPRECATED_OPENCL_1_2_APIS)

    cl_int enqueueNativeKernel(
        void (CL_CALLBACK *userFptr)(void *),
        std::pair&lt;void*, size_type&gt; args,
        const vector&lt;Memory&gt;* mem_objects = NULL,
        const vector&lt;const void*&gt;* mem_locs = NULL,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL) const
    {
        size_type elements = 0;
        if (mem_objects != NULL) {
            elements = mem_objects-&gt;size();
        }
        vector&lt;cl_mem&gt; mems(elements);
        for (unsigned int i = 0; i &lt; elements; i++) {
            mems[i] = ((*mem_objects)[i])();
        }
        
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueNativeKernel(
                object_, userFptr, args.first, args.second,
                (mem_objects != NULL) ? (cl_uint) mem_objects-&gt;size() : 0,
                mems.data(),
                (mem_locs != NULL &amp;&amp; mem_locs-&gt;size() &gt; 0) ? (const void **) &amp;mem_locs-&gt;front() : NULL,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_NATIVE_KERNEL);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

/**
 * Deprecated APIs for 1.2
 */
#if defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
    CL_EXT_PREFIX__VERSION_1_1_DEPRECATED 
    cl_int enqueueMarker(Event* event = NULL) const CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED
    {
        cl_event tmp;
        cl_int err = detail::errHandler(
            ::clEnqueueMarker(
                object_, 
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_MARKER_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }

    CL_EXT_PREFIX__VERSION_1_1_DEPRECATED
    cl_int enqueueWaitForEvents(const vector&lt;Event&gt;&amp; events) const CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED
    {
        return detail::errHandler(
            ::clEnqueueWaitForEvents(
                object_,
                (cl_uint) events.size(),
                events.size() &gt; 0 ? (const cl_event*) &amp;events.front() : NULL),
            __ENQUEUE_WAIT_FOR_EVENTS_ERR);
    }
#endif // defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)

    cl_int enqueueAcquireGLObjects(
         const vector&lt;Memory&gt;* mem_objects = NULL,
         const vector&lt;Event&gt;* events = NULL,
         Event* event = NULL) const
     {
        cl_event tmp;
        cl_int err = detail::errHandler(
             ::clEnqueueAcquireGLObjects(
                 object_,
                 (mem_objects != NULL) ? (cl_uint) mem_objects-&gt;size() : 0,
                 (mem_objects != NULL &amp;&amp; mem_objects-&gt;size() &gt; 0) ? (const cl_mem *) &amp;mem_objects-&gt;front(): NULL,
                 (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                 (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                 (event != NULL) ? &amp;tmp : NULL),
             __ENQUEUE_ACQUIRE_GL_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
     }

    cl_int enqueueReleaseGLObjects(
         const vector&lt;Memory&gt;* mem_objects = NULL,
         const vector&lt;Event&gt;* events = NULL,
         Event* event = NULL) const
     {
        cl_event tmp;
        cl_int err = detail::errHandler(
             ::clEnqueueReleaseGLObjects(
                 object_,
                 (mem_objects != NULL) ? (cl_uint) mem_objects-&gt;size() : 0,
                 (mem_objects != NULL &amp;&amp; mem_objects-&gt;size() &gt; 0) ? (const cl_mem *) &amp;mem_objects-&gt;front(): NULL,
                 (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                 (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                 (event != NULL) ? &amp;tmp : NULL),
             __ENQUEUE_RELEASE_GL_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
     }

#if defined (CL_HPP_USE_DX_INTEROP)
typedef CL_API_ENTRY cl_int (CL_API_CALL *PFN_clEnqueueAcquireD3D10ObjectsKHR)(
    cl_command_queue command_queue, cl_uint num_objects,
    const cl_mem* mem_objects, cl_uint num_events_in_wait_list,
    const cl_event* event_wait_list, cl_event* event);
typedef CL_API_ENTRY cl_int (CL_API_CALL *PFN_clEnqueueReleaseD3D10ObjectsKHR)(
    cl_command_queue command_queue, cl_uint num_objects,
    const cl_mem* mem_objects,  cl_uint num_events_in_wait_list,
    const cl_event* event_wait_list, cl_event* event);

    cl_int enqueueAcquireD3D10Objects(
         const vector&lt;Memory&gt;* mem_objects = NULL,
         const vector&lt;Event&gt;* events = NULL,
         Event* event = NULL) const
    {
        static PFN_clEnqueueAcquireD3D10ObjectsKHR pfn_clEnqueueAcquireD3D10ObjectsKHR = NULL;
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
        cl_context context = getInfo&lt;CL_QUEUE_CONTEXT&gt;();
        cl::Device device(getInfo&lt;CL_QUEUE_DEVICE&gt;());
        cl_platform_id platform = device.getInfo&lt;CL_DEVICE_PLATFORM&gt;();
        CL_HPP_INIT_CL_EXT_FCN_PTR_PLATFORM_(platform, clEnqueueAcquireD3D10ObjectsKHR);
#endif
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
        CL_HPP_INIT_CL_EXT_FCN_PTR_(clEnqueueAcquireD3D10ObjectsKHR);
#endif
        
        cl_event tmp;
        cl_int err = detail::errHandler(
             pfn_clEnqueueAcquireD3D10ObjectsKHR(
                 object_,
                 (mem_objects != NULL) ? (cl_uint) mem_objects-&gt;size() : 0,
                 (mem_objects != NULL &amp;&amp; mem_objects-&gt;size() &gt; 0) ? (const cl_mem *) &amp;mem_objects-&gt;front(): NULL,
                 (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                 (events != NULL) ? (cl_event*) &amp;events-&gt;front() : NULL,
                 (event != NULL) ? &amp;tmp : NULL),
             __ENQUEUE_ACQUIRE_GL_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
     }

    cl_int enqueueReleaseD3D10Objects(
         const vector&lt;Memory&gt;* mem_objects = NULL,
         const vector&lt;Event&gt;* events = NULL,
         Event* event = NULL) const
    {
        static PFN_clEnqueueReleaseD3D10ObjectsKHR pfn_clEnqueueReleaseD3D10ObjectsKHR = NULL;
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
        cl_context context = getInfo&lt;CL_QUEUE_CONTEXT&gt;();
        cl::Device device(getInfo&lt;CL_QUEUE_DEVICE&gt;());
        cl_platform_id platform = device.getInfo&lt;CL_DEVICE_PLATFORM&gt;();
        CL_HPP_INIT_CL_EXT_FCN_PTR_PLATFORM_(platform, clEnqueueReleaseD3D10ObjectsKHR);
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 120
#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
        CL_HPP_INIT_CL_EXT_FCN_PTR_(clEnqueueReleaseD3D10ObjectsKHR);
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 110

        cl_event tmp;
        cl_int err = detail::errHandler(
            pfn_clEnqueueReleaseD3D10ObjectsKHR(
                object_,
                (mem_objects != NULL) ? (cl_uint) mem_objects-&gt;size() : 0,
                (mem_objects != NULL &amp;&amp; mem_objects-&gt;size() &gt; 0) ? (const cl_mem *) &amp;mem_objects-&gt;front(): NULL,
                (events != NULL) ? (cl_uint) events-&gt;size() : 0,
                (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
                (event != NULL) ? &amp;tmp : NULL),
            __ENQUEUE_RELEASE_GL_ERR);

        if (event != NULL &amp;&amp; err == CL_SUCCESS)
            *event = tmp;

        return err;
    }
#endif

/**
 * Deprecated APIs for 1.2
 */
#if defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
    CL_EXT_PREFIX__VERSION_1_1_DEPRECATED
    cl_int enqueueBarrier() const CL_EXT_SUFFIX__VERSION_1_1_DEPRECATED
    {
        return detail::errHandler(
            ::clEnqueueBarrier(object_),
            __ENQUEUE_BARRIER_ERR);
    }
#endif // CL_USE_DEPRECATED_OPENCL_1_1_APIS

    cl_int flush() const
    {
        return detail::errHandler(::clFlush(object_), __FLUSH_ERR);
    }

    cl_int finish() const
    {
        return detail::errHandler(::clFinish(object_), __FINISH_ERR);
    }
}; // CommandQueue

CL_HPP_DEFINE_STATIC_MEMBER_ std::once_flag CommandQueue::default_initialized_;
CL_HPP_DEFINE_STATIC_MEMBER_ CommandQueue CommandQueue::default_;
CL_HPP_DEFINE_STATIC_MEMBER_ cl_int CommandQueue::default_error_ = CL_SUCCESS;


#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
enum class DeviceQueueProperties : cl_command_queue_properties
{
    None = 0,
    Profiling = CL_QUEUE_PROFILING_ENABLE,
};

inline DeviceQueueProperties operator|(DeviceQueueProperties lhs, DeviceQueueProperties rhs)
{
    return static_cast&lt;DeviceQueueProperties&gt;(static_cast&lt;cl_command_queue_properties&gt;(lhs) | static_cast&lt;cl_command_queue_properties&gt;(rhs));
}

/*! \class DeviceCommandQueue
 * \brief DeviceCommandQueue interface for device cl_command_queues.
 */
class DeviceCommandQueue : public detail::Wrapper&lt;cl_command_queue&gt;
{
public:

    /*!
     * Trivial empty constructor to create a null queue.
     */
    DeviceCommandQueue() { }

    /*!
     * Default construct device command queue on default context and device
     */
    DeviceCommandQueue(DeviceQueueProperties properties, cl_int* err = NULL)
    {
        cl_int error;
        cl::Context context = cl::Context::getDefault();
        cl::Device device = cl::Device::getDefault();

        cl_command_queue_properties mergedProperties =
            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_ON_DEVICE | static_cast&lt;cl_command_queue_properties&gt;(properties);

        cl_queue_properties queue_properties[] = {
            CL_QUEUE_PROPERTIES, mergedProperties, 0 };
        object_ = ::clCreateCommandQueueWithProperties(
            context(), device(), queue_properties, &amp;error);

        detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    /*!
     * Create a device command queue for a specified device in the passed context.
     */
    DeviceCommandQueue(
        const Context&amp; context,
        const Device&amp; device,
        DeviceQueueProperties properties = DeviceQueueProperties::None,
        cl_int* err = NULL)
    {
        cl_int error;

        cl_command_queue_properties mergedProperties =
            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_ON_DEVICE | static_cast&lt;cl_command_queue_properties&gt;(properties);
        cl_queue_properties queue_properties[] = {
            CL_QUEUE_PROPERTIES, mergedProperties, 0 };
        object_ = ::clCreateCommandQueueWithProperties(
            context(), device(), queue_properties, &amp;error);

        detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    /*!
     * Create a device command queue for a specified device in the passed context.
     */
    DeviceCommandQueue(
        const Context&amp; context,
        const Device&amp; device,
        cl_uint queueSize,
        DeviceQueueProperties properties = DeviceQueueProperties::None,
        cl_int* err = NULL)
    {
        cl_int error;

        cl_command_queue_properties mergedProperties =
            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_ON_DEVICE | static_cast&lt;cl_command_queue_properties&gt;(properties);
        cl_queue_properties queue_properties[] = {
            CL_QUEUE_PROPERTIES, mergedProperties,
            CL_QUEUE_SIZE, queueSize, 
            0 };
        object_ = ::clCreateCommandQueueWithProperties(
            context(), device(), queue_properties, &amp;error);

        detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
        if (err != NULL) {
            *err = error;
        }
    }

    /*! \brief Constructor from cl_command_queue - takes ownership.
    *
    * \param retainObject will cause the constructor to retain its cl object.
    *                     Defaults to false to maintain compatibility with
    *                     earlier versions.
    */
    explicit DeviceCommandQueue(const cl_command_queue&amp; commandQueue, bool retainObject = false) :
        detail::Wrapper&lt;cl_type&gt;(commandQueue, retainObject) { }

    DeviceCommandQueue&amp; operator = (const cl_command_queue&amp; rhs)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(rhs);
        return *this;
    }

    /*! \brief Copy constructor to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    DeviceCommandQueue(const DeviceCommandQueue&amp; queue) : detail::Wrapper&lt;cl_type&gt;(queue) {}

    /*! \brief Copy assignment to forward copy to the superclass correctly.
     * Required for MSVC.
     */
    DeviceCommandQueue&amp; operator = (const DeviceCommandQueue &amp;queue)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(queue);
        return *this;
    }

    /*! \brief Move constructor to forward move to the superclass correctly.
     * Required for MSVC.
     */
    DeviceCommandQueue(DeviceCommandQueue&amp;&amp; queue) CL_HPP_NOEXCEPT_ : detail::Wrapper&lt;cl_type&gt;(std::move(queue)) {}

    /*! \brief Move assignment to forward move to the superclass correctly.
     * Required for MSVC.
     */
    DeviceCommandQueue&amp; operator = (DeviceCommandQueue &amp;&amp;queue)
    {
        detail::Wrapper&lt;cl_type&gt;::operator=(std::move(queue));
        return *this;
    }

    template &lt;typename T&gt;
    cl_int getInfo(cl_command_queue_info name, T* param) const
    {
        return detail::errHandler(
            detail::getInfo(
            &amp;::clGetCommandQueueInfo, object_, name, param),
            __GET_COMMAND_QUEUE_INFO_ERR);
    }

    template &lt;cl_command_queue_info name&gt; typename
        detail::param_traits&lt;detail::cl_command_queue_info, name&gt;::param_type
        getInfo(cl_int* err = NULL) const
    {
        typename detail::param_traits&lt;
            detail::cl_command_queue_info, name&gt;::param_type param;
        cl_int result = getInfo(name, &amp;param);
        if (err != NULL) {
            *err = result;
        }
        return param;
    }

    /*!
     * Create a new default device command queue for the default device,
     * in the default context and of the default size.
     * If there is already a default queue for the specified device this
     * function will return the pre-existing queue.
     */
    static DeviceCommandQueue makeDefault(
        cl_int *err = nullptr)
    {
        cl_int error;
        cl::Context context = cl::Context::getDefault();
        cl::Device device = cl::Device::getDefault();

        cl_command_queue_properties properties =
            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_ON_DEVICE | CL_QUEUE_ON_DEVICE_DEFAULT;
        cl_queue_properties queue_properties[] = {
            CL_QUEUE_PROPERTIES, properties,
            0 };
        DeviceCommandQueue deviceQueue(
            ::clCreateCommandQueueWithProperties(
            context(), device(), queue_properties, &amp;error));

        detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
        if (err != NULL) {
            *err = error;
        }

        return deviceQueue;
    }

    /*!
     * Create a new default device command queue for the specified device
     * and of the default size.
     * If there is already a default queue for the specified device this
     * function will return the pre-existing queue.
     */
    static DeviceCommandQueue makeDefault(
        const Context &amp;context, const Device &amp;device, cl_int *err = nullptr)
    {
        cl_int error;

        cl_command_queue_properties properties =
            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_ON_DEVICE | CL_QUEUE_ON_DEVICE_DEFAULT;
        cl_queue_properties queue_properties[] = {
            CL_QUEUE_PROPERTIES, properties,
            0 };
        DeviceCommandQueue deviceQueue(
            ::clCreateCommandQueueWithProperties(
            context(), device(), queue_properties, &amp;error));

        detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
        if (err != NULL) {
            *err = error;
        }

        return deviceQueue;
    }

    /*!
     * Create a new default device command queue for the specified device 
     * and of the requested size in bytes.
     * If there is already a default queue for the specified device this
     * function will return the pre-existing queue.
     */
    static DeviceCommandQueue makeDefault(
        const Context &amp;context, const Device &amp;device, cl_uint queueSize, cl_int *err = nullptr)
    {
        cl_int error;

        cl_command_queue_properties properties =
            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_ON_DEVICE | CL_QUEUE_ON_DEVICE_DEFAULT;
        cl_queue_properties queue_properties[] = {
            CL_QUEUE_PROPERTIES, properties,
            CL_QUEUE_SIZE, queueSize,
            0 };
        DeviceCommandQueue deviceQueue(
            ::clCreateCommandQueueWithProperties(
                context(), device(), queue_properties, &amp;error));

        detail::errHandler(error, __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR);
        if (err != NULL) {
            *err = error;
        }

        return deviceQueue;
    }



#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
    /*!
     * Modify the default device command queue to be used for subsequent kernels.
     * This can update the default command queue for a device repeatedly to account
     * for kernels that rely on the default.
     * @return updated default device command queue.
     */
    static DeviceCommandQueue updateDefault(const Context &amp;context, const Device &amp;device, const DeviceCommandQueue &amp;default_queue, cl_int *err = nullptr)
    {
        cl_int error;
        error = clSetDefaultDeviceCommandQueue(context.get(), device.get(), default_queue.get());

        detail::errHandler(error, __SET_DEFAULT_DEVICE_COMMAND_QUEUE_ERR);
        if (err != NULL) {
            *err = error;
        }
        return default_queue;
    }

    /*!
     * Return the current default command queue for the specified command queue
     */
    static DeviceCommandQueue getDefault(const CommandQueue &amp;queue, cl_int * err = NULL)
    {
        return queue.getInfo&lt;CL_QUEUE_DEVICE_DEFAULT&gt;(err);
    }

#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 210
}; // DeviceCommandQueue

namespace detail
{
    // Specialization for device command queue
    template &lt;&gt;
    struct KernelArgumentHandler&lt;cl::DeviceCommandQueue, void&gt;
    {
        static size_type size(const cl::DeviceCommandQueue&amp;) { return sizeof(cl_command_queue); }
        static const cl_command_queue* ptr(const cl::DeviceCommandQueue&amp; value) { return &amp;(value()); }
    };
} // namespace detail

#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200


template&lt; typename IteratorType &gt;
Buffer::Buffer(
    const Context &amp;context,
    IteratorType startIterator,
    IteratorType endIterator,
    bool readOnly,
    bool useHostPtr,
    cl_int* err)
{
    typedef typename std::iterator_traits&lt;IteratorType&gt;::value_type DataType;
    cl_int error;

    cl_mem_flags flags = 0;
    if( readOnly ) {
        flags |= CL_MEM_READ_ONLY;
    }
    else {
        flags |= CL_MEM_READ_WRITE;
    }
    if( useHostPtr ) {
        flags |= CL_MEM_USE_HOST_PTR;
    }
    
    size_type size = sizeof(DataType)*(endIterator - startIterator);

    if( useHostPtr ) {
        object_ = ::clCreateBuffer(context(), flags, size, static_cast&lt;DataType*&gt;(&amp;*startIterator), &amp;error);
    } else {
        object_ = ::clCreateBuffer(context(), flags, size, 0, &amp;error);
    }

    detail::errHandler(error, __CREATE_BUFFER_ERR);
    if (err != NULL) {
        *err = error;
    }

    if( !useHostPtr ) {
        CommandQueue queue(context, 0, &amp;error);
        detail::errHandler(error, __CREATE_BUFFER_ERR);
        if (err != NULL) {
            *err = error;
        }

        error = cl::copy(queue, startIterator, endIterator, *this);
        detail::errHandler(error, __CREATE_BUFFER_ERR);
        if (err != NULL) {
            *err = error;
        }
    }
}

template&lt; typename IteratorType &gt;
Buffer::Buffer(
    const CommandQueue &amp;queue,
    IteratorType startIterator,
    IteratorType endIterator,
    bool readOnly,
    bool useHostPtr,
    cl_int* err)
{
    typedef typename std::iterator_traits&lt;IteratorType&gt;::value_type DataType;
    cl_int error;

    cl_mem_flags flags = 0;
    if (readOnly) {
        flags |= CL_MEM_READ_ONLY;
    }
    else {
        flags |= CL_MEM_READ_WRITE;
    }
    if (useHostPtr) {
        flags |= CL_MEM_USE_HOST_PTR;
    }

    size_type size = sizeof(DataType)*(endIterator - startIterator);

    Context context = queue.getInfo&lt;CL_QUEUE_CONTEXT&gt;();

    if (useHostPtr) {
        object_ = ::clCreateBuffer(context(), flags, size, static_cast&lt;DataType*&gt;(&amp;*startIterator), &amp;error);
    }
    else {
        object_ = ::clCreateBuffer(context(), flags, size, 0, &amp;error);
    }

    detail::errHandler(error, __CREATE_BUFFER_ERR);
    if (err != NULL) {
        *err = error;
    }

    if (!useHostPtr) {
        error = cl::copy(queue, startIterator, endIterator, *this);
        detail::errHandler(error, __CREATE_BUFFER_ERR);
        if (err != NULL) {
            *err = error;
        }
    }
}

inline cl_int enqueueReadBuffer(
    const Buffer&amp; buffer,
    cl_bool blocking,
    size_type offset,
    size_type size,
    void* ptr,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.enqueueReadBuffer(buffer, blocking, offset, size, ptr, events, event);
}

inline cl_int enqueueWriteBuffer(
        const Buffer&amp; buffer,
        cl_bool blocking,
        size_type offset,
        size_type size,
        const void* ptr,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.enqueueWriteBuffer(buffer, blocking, offset, size, ptr, events, event);
}

inline void* enqueueMapBuffer(
        const Buffer&amp; buffer,
        cl_bool blocking,
        cl_map_flags flags,
        size_type offset,
        size_type size,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL,
        cl_int* err = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);
    detail::errHandler(error, __ENQUEUE_MAP_BUFFER_ERR);
    if (err != NULL) {
        *err = error;
    }

    void * result = ::clEnqueueMapBuffer(
            queue(), buffer(), blocking, flags, offset, size,
            (events != NULL) ? (cl_uint) events-&gt;size() : 0,
            (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*) &amp;events-&gt;front() : NULL,
            (cl_event*) event,
            &amp;error);

    detail::errHandler(error, __ENQUEUE_MAP_BUFFER_ERR);
    if (err != NULL) {
        *err = error;
    }
    return result;
}


#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
/**
 * Enqueues to the default queue a command that will allow the host to
 * update a region of a coarse-grained SVM buffer.
 * This variant takes a raw SVM pointer.
 */
template&lt;typename T&gt;
inline cl_int enqueueMapSVM(
    T* ptr,
    cl_bool blocking,
    cl_map_flags flags,
    size_type size,
    const vector&lt;Event&gt;* events,
    Event* event)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);
    if (error != CL_SUCCESS) {
        return detail::errHandler(error, __ENQUEUE_MAP_BUFFER_ERR);
    }

    return queue.enqueueMapSVM(
        ptr, blocking, flags, size, events, event);
}

/**
 * Enqueues to the default queue a command that will allow the host to 
 * update a region of a coarse-grained SVM buffer.
 * This variant takes a cl::pointer instance.
 */
template&lt;typename T, class D&gt;
inline cl_int enqueueMapSVM(
    cl::pointer&lt;T, D&gt; ptr,
    cl_bool blocking,
    cl_map_flags flags,
    size_type size,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);
    if (error != CL_SUCCESS) {
        return detail::errHandler(error, __ENQUEUE_MAP_BUFFER_ERR);
    }

    return queue.enqueueMapSVM(
        ptr, blocking, flags, size, events, event);
}

/**
 * Enqueues to the default queue a command that will allow the host to
 * update a region of a coarse-grained SVM buffer.
 * This variant takes a cl::vector instance.
 */
template&lt;typename T, class Alloc&gt;
inline cl_int enqueueMapSVM(
    cl::vector&lt;T, Alloc&gt; container,
    cl_bool blocking,
    cl_map_flags flags,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);
    if (error != CL_SUCCESS) {
        return detail::errHandler(error, __ENQUEUE_MAP_BUFFER_ERR);
    }

    return queue.enqueueMapSVM(
        container, blocking, flags, events, event);
}

#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200

inline cl_int enqueueUnmapMemObject(
    const Memory&amp; memory,
    void* mapped_ptr,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);
    detail::errHandler(error, __ENQUEUE_MAP_BUFFER_ERR);
    if (error != CL_SUCCESS) {
        return error;
    }

    cl_event tmp;
    cl_int err = detail::errHandler(
        ::clEnqueueUnmapMemObject(
        queue(), memory(), mapped_ptr,
        (events != NULL) ? (cl_uint)events-&gt;size() : 0,
        (events != NULL &amp;&amp; events-&gt;size() &gt; 0) ? (cl_event*)&amp;events-&gt;front() : NULL,
        (event != NULL) ? &amp;tmp : NULL),
        __ENQUEUE_UNMAP_MEM_OBJECT_ERR);

    if (event != NULL &amp;&amp; err == CL_SUCCESS)
        *event = tmp;

    return err;
}

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
/**
 * Enqueues to the default queue a command that will release a coarse-grained 
 * SVM buffer back to the OpenCL runtime.
 * This variant takes a raw SVM pointer.
 */
template&lt;typename T&gt;
inline cl_int enqueueUnmapSVM(
    T* ptr,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);
    if (error != CL_SUCCESS) {
        return detail::errHandler(error, __ENQUEUE_UNMAP_MEM_OBJECT_ERR);
    }

    return detail::errHandler(queue.enqueueUnmapSVM(ptr, events, event), 
        __ENQUEUE_UNMAP_MEM_OBJECT_ERR);

}

/**
 * Enqueues to the default queue a command that will release a coarse-grained 
 * SVM buffer back to the OpenCL runtime.
 * This variant takes a cl::pointer instance.
 */
template&lt;typename T, class D&gt;
inline cl_int enqueueUnmapSVM(
    cl::pointer&lt;T, D&gt; &amp;ptr,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);
    if (error != CL_SUCCESS) {
        return detail::errHandler(error, __ENQUEUE_UNMAP_MEM_OBJECT_ERR);
    }

    return detail::errHandler(queue.enqueueUnmapSVM(ptr, events, event),
        __ENQUEUE_UNMAP_MEM_OBJECT_ERR);
}

/**
 * Enqueues to the default queue a command that will release a coarse-grained 
 * SVM buffer back to the OpenCL runtime.
 * This variant takes a cl::vector instance.
 */
template&lt;typename T, class Alloc&gt;
inline cl_int enqueueUnmapSVM(
    cl::vector&lt;T, Alloc&gt; &amp;container,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);
    if (error != CL_SUCCESS) {
        return detail::errHandler(error, __ENQUEUE_UNMAP_MEM_OBJECT_ERR);
    }

    return detail::errHandler(queue.enqueueUnmapSVM(container, events, event),
        __ENQUEUE_UNMAP_MEM_OBJECT_ERR);
}

#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200

inline cl_int enqueueCopyBuffer(
        const Buffer&amp; src,
        const Buffer&amp; dst,
        size_type src_offset,
        size_type dst_offset,
        size_type size,
        const vector&lt;Event&gt;* events = NULL,
        Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.enqueueCopyBuffer(src, dst, src_offset, dst_offset, size, events, event);
}

/**
 * Blocking copy operation between iterators and a buffer.
 * Host to Device.
 * Uses default command queue.
 */
template&lt; typename IteratorType &gt;
inline cl_int copy( IteratorType startIterator, IteratorType endIterator, cl::Buffer &amp;buffer )
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);
    if (error != CL_SUCCESS)
        return error;

    return cl::copy(queue, startIterator, endIterator, buffer);
}

/**
 * Blocking copy operation between iterators and a buffer.
 * Device to Host.
 * Uses default command queue.
 */
template&lt; typename IteratorType &gt;
inline cl_int copy( const cl::Buffer &amp;buffer, IteratorType startIterator, IteratorType endIterator )
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);
    if (error != CL_SUCCESS)
        return error;

    return cl::copy(queue, buffer, startIterator, endIterator);
}

/**
 * Blocking copy operation between iterators and a buffer.
 * Host to Device.
 * Uses specified queue.
 */
template&lt; typename IteratorType &gt;
inline cl_int copy( const CommandQueue &amp;queue, IteratorType startIterator, IteratorType endIterator, cl::Buffer &amp;buffer )
{
    typedef typename std::iterator_traits&lt;IteratorType&gt;::value_type DataType;
    cl_int error;
    
    size_type length = endIterator-startIterator;
    size_type byteLength = length*sizeof(DataType);

    DataType *pointer = 
        static_cast&lt;DataType*&gt;(queue.enqueueMapBuffer(buffer, CL_TRUE, CL_MAP_WRITE, 0, byteLength, 0, 0, &amp;error));
    // if exceptions enabled, enqueueMapBuffer will throw
    if( error != CL_SUCCESS ) {
        return error;
    }
#if defined(_MSC_VER)
    std::copy(
        startIterator, 
        endIterator, 
        stdext::checked_array_iterator&lt;DataType*&gt;(
            pointer, length));
#else
    std::copy(startIterator, endIterator, pointer);
#endif
    Event endEvent;
    error = queue.enqueueUnmapMemObject(buffer, pointer, 0, &amp;endEvent);
    // if exceptions enabled, enqueueUnmapMemObject will throw
    if( error != CL_SUCCESS ) { 
        return error;
    }
    endEvent.wait();
    return CL_SUCCESS;
}

/**
 * Blocking copy operation between iterators and a buffer.
 * Device to Host.
 * Uses specified queue.
 */
template&lt; typename IteratorType &gt;
inline cl_int copy( const CommandQueue &amp;queue, const cl::Buffer &amp;buffer, IteratorType startIterator, IteratorType endIterator )
{
    typedef typename std::iterator_traits&lt;IteratorType&gt;::value_type DataType;
    cl_int error;
        
    size_type length = endIterator-startIterator;
    size_type byteLength = length*sizeof(DataType);

    DataType *pointer = 
        static_cast&lt;DataType*&gt;(queue.enqueueMapBuffer(buffer, CL_TRUE, CL_MAP_READ, 0, byteLength, 0, 0, &amp;error));
    // if exceptions enabled, enqueueMapBuffer will throw
    if( error != CL_SUCCESS ) {
        return error;
    }
    std::copy(pointer, pointer + length, startIterator);
    Event endEvent;
    error = queue.enqueueUnmapMemObject(buffer, pointer, 0, &amp;endEvent);
    // if exceptions enabled, enqueueUnmapMemObject will throw
    if( error != CL_SUCCESS ) { 
        return error;
    }
    endEvent.wait();
    return CL_SUCCESS;
}


#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
/**
 * Blocking SVM map operation - performs a blocking map underneath.
 */
template&lt;typename T, class Alloc&gt;
inline cl_int mapSVM(cl::vector&lt;T, Alloc&gt; &amp;container)
{
    return enqueueMapSVM(container, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE);
}

/**
* Blocking SVM map operation - performs a blocking map underneath.
*/
template&lt;typename T, class Alloc&gt;
inline cl_int unmapSVM(cl::vector&lt;T, Alloc&gt; &amp;container)
{
    return enqueueUnmapSVM(container);
}

#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 110
inline cl_int enqueueReadBufferRect(
    const Buffer&amp; buffer,
    cl_bool blocking,
    const array&lt;size_type, 3&gt;&amp; buffer_offset,
    const array&lt;size_type, 3&gt;&amp; host_offset,
    const array&lt;size_type, 3&gt;&amp; region,
    size_type buffer_row_pitch,
    size_type buffer_slice_pitch,
    size_type host_row_pitch,
    size_type host_slice_pitch,
    void *ptr,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.enqueueReadBufferRect(
        buffer, 
        blocking, 
        buffer_offset, 
        host_offset,
        region,
        buffer_row_pitch,
        buffer_slice_pitch,
        host_row_pitch,
        host_slice_pitch,
        ptr, 
        events, 
        event);
}

inline cl_int enqueueWriteBufferRect(
    const Buffer&amp; buffer,
    cl_bool blocking,
    const array&lt;size_type, 3&gt;&amp; buffer_offset,
    const array&lt;size_type, 3&gt;&amp; host_offset,
    const array&lt;size_type, 3&gt;&amp; region,
    size_type buffer_row_pitch,
    size_type buffer_slice_pitch,
    size_type host_row_pitch,
    size_type host_slice_pitch,
    const void *ptr,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.enqueueWriteBufferRect(
        buffer, 
        blocking, 
        buffer_offset, 
        host_offset,
        region,
        buffer_row_pitch,
        buffer_slice_pitch,
        host_row_pitch,
        host_slice_pitch,
        ptr, 
        events, 
        event);
}

inline cl_int enqueueCopyBufferRect(
    const Buffer&amp; src,
    const Buffer&amp; dst,
    const array&lt;size_type, 3&gt;&amp; src_origin,
    const array&lt;size_type, 3&gt;&amp; dst_origin,
    const array&lt;size_type, 3&gt;&amp; region,
    size_type src_row_pitch,
    size_type src_slice_pitch,
    size_type dst_row_pitch,
    size_type dst_slice_pitch,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.enqueueCopyBufferRect(
        src,
        dst,
        src_origin,
        dst_origin,
        region,
        src_row_pitch,
        src_slice_pitch,
        dst_row_pitch,
        dst_slice_pitch,
        events, 
        event);
}
#endif // CL_HPP_TARGET_OPENCL_VERSION &gt;= 110

inline cl_int enqueueReadImage(
    const Image&amp; image,
    cl_bool blocking,
    const array&lt;size_type, 3&gt;&amp; origin,
    const array&lt;size_type, 3&gt;&amp; region,
    size_type row_pitch,
    size_type slice_pitch,
    void* ptr,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL) 
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.enqueueReadImage(
        image,
        blocking,
        origin,
        region,
        row_pitch,
        slice_pitch,
        ptr,
        events, 
        event);
}

inline cl_int enqueueWriteImage(
    const Image&amp; image,
    cl_bool blocking,
    const array&lt;size_type, 3&gt;&amp; origin,
    const array&lt;size_type, 3&gt;&amp; region,
    size_type row_pitch,
    size_type slice_pitch,
    const void* ptr,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.enqueueWriteImage(
        image,
        blocking,
        origin,
        region,
        row_pitch,
        slice_pitch,
        ptr,
        events, 
        event);
}

inline cl_int enqueueCopyImage(
    const Image&amp; src,
    const Image&amp; dst,
    const array&lt;size_type, 3&gt;&amp; src_origin,
    const array&lt;size_type, 3&gt;&amp; dst_origin,
    const array&lt;size_type, 3&gt;&amp; region,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.enqueueCopyImage(
        src,
        dst,
        src_origin,
        dst_origin,
        region,
        events,
        event);
}

inline cl_int enqueueCopyImageToBuffer(
    const Image&amp; src,
    const Buffer&amp; dst,
    const array&lt;size_type, 3&gt;&amp; src_origin,
    const array&lt;size_type, 3&gt;&amp; region,
    size_type dst_offset,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.enqueueCopyImageToBuffer(
        src,
        dst,
        src_origin,
        region,
        dst_offset,
        events,
        event);
}

inline cl_int enqueueCopyBufferToImage(
    const Buffer&amp; src,
    const Image&amp; dst,
    size_type src_offset,
    const array&lt;size_type, 3&gt;&amp; dst_origin,
    const array&lt;size_type, 3&gt;&amp; region,
    const vector&lt;Event&gt;* events = NULL,
    Event* event = NULL)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.enqueueCopyBufferToImage(
        src,
        dst,
        src_offset,
        dst_origin,
        region,
        events,
        event);
}


inline cl_int flush(void)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    }

    return queue.flush();
}

inline cl_int finish(void)
{
    cl_int error;
    CommandQueue queue = CommandQueue::getDefault(&amp;error);

    if (error != CL_SUCCESS) {
        return error;
    } 


    return queue.finish();
}

class EnqueueArgs
{
private:
    CommandQueue queue_;
    const NDRange offset_;
    const NDRange global_;
    const NDRange local_;
    vector&lt;Event&gt; events_;

    template&lt;typename... Ts&gt;
    friend class KernelFunctor;

public:
    EnqueueArgs(NDRange global) : 
      queue_(CommandQueue::getDefault()),
      offset_(NullRange), 
      global_(global),
      local_(NullRange)
    {

    }

    EnqueueArgs(NDRange global, NDRange local) : 
      queue_(CommandQueue::getDefault()),
      offset_(NullRange), 
      global_(global),
      local_(local)
    {

    }

    EnqueueArgs(NDRange offset, NDRange global, NDRange local) : 
      queue_(CommandQueue::getDefault()),
      offset_(offset), 
      global_(global),
      local_(local)
    {

    }

    EnqueueArgs(Event e, NDRange global) : 
      queue_(CommandQueue::getDefault()),
      offset_(NullRange), 
      global_(global),
      local_(NullRange)
    {
        events_.push_back(e);
    }

    EnqueueArgs(Event e, NDRange global, NDRange local) : 
      queue_(CommandQueue::getDefault()),
      offset_(NullRange), 
      global_(global),
      local_(local)
    {
        events_.push_back(e);
    }

    EnqueueArgs(Event e, NDRange offset, NDRange global, NDRange local) : 
      queue_(CommandQueue::getDefault()),
      offset_(offset), 
      global_(global),
      local_(local)
    {
        events_.push_back(e);
    }

    EnqueueArgs(const vector&lt;Event&gt; &amp;events, NDRange global) : 
      queue_(CommandQueue::getDefault()),
      offset_(NullRange), 
      global_(global),
      local_(NullRange),
      events_(events)
    {

    }

    EnqueueArgs(const vector&lt;Event&gt; &amp;events, NDRange global, NDRange local) : 
      queue_(CommandQueue::getDefault()),
      offset_(NullRange), 
      global_(global),
      local_(local),
      events_(events)
    {

    }

    EnqueueArgs(const vector&lt;Event&gt; &amp;events, NDRange offset, NDRange global, NDRange local) : 
      queue_(CommandQueue::getDefault()),
      offset_(offset), 
      global_(global),
      local_(local),
      events_(events)
    {

    }

    EnqueueArgs(CommandQueue &amp;queue, NDRange global) : 
      queue_(queue),
      offset_(NullRange), 
      global_(global),
      local_(NullRange)
    {

    }

    EnqueueArgs(CommandQueue &amp;queue, NDRange global, NDRange local) : 
      queue_(queue),
      offset_(NullRange), 
      global_(global),
      local_(local)
    {

    }

    EnqueueArgs(CommandQueue &amp;queue, NDRange offset, NDRange global, NDRange local) : 
      queue_(queue),
      offset_(offset), 
      global_(global),
      local_(local)
    {

    }

    EnqueueArgs(CommandQueue &amp;queue, Event e, NDRange global) : 
      queue_(queue),
      offset_(NullRange), 
      global_(global),
      local_(NullRange)
    {
        events_.push_back(e);
    }

    EnqueueArgs(CommandQueue &amp;queue, Event e, NDRange global, NDRange local) : 
      queue_(queue),
      offset_(NullRange), 
      global_(global),
      local_(local)
    {
        events_.push_back(e);
    }

    EnqueueArgs(CommandQueue &amp;queue, Event e, NDRange offset, NDRange global, NDRange local) : 
      queue_(queue),
      offset_(offset), 
      global_(global),
      local_(local)
    {
        events_.push_back(e);
    }

    EnqueueArgs(CommandQueue &amp;queue, const vector&lt;Event&gt; &amp;events, NDRange global) : 
      queue_(queue),
      offset_(NullRange), 
      global_(global),
      local_(NullRange),
      events_(events)
    {

    }

    EnqueueArgs(CommandQueue &amp;queue, const vector&lt;Event&gt; &amp;events, NDRange global, NDRange local) : 
      queue_(queue),
      offset_(NullRange), 
      global_(global),
      local_(local),
      events_(events)
    {

    }

    EnqueueArgs(CommandQueue &amp;queue, const vector&lt;Event&gt; &amp;events, NDRange offset, NDRange global, NDRange local) : 
      queue_(queue),
      offset_(offset), 
      global_(global),
      local_(local),
      events_(events)
    {

    }
};


//----------------------------------------------------------------------------------------------


/**
 * Type safe kernel functor.
 * 
 */
template&lt;typename... Ts&gt;
class KernelFunctor
{
private:
    Kernel kernel_;

    template&lt;int index, typename T0, typename... T1s&gt;
    void setArgs(T0&amp;&amp; t0, T1s&amp;&amp;... t1s)
    {
        kernel_.setArg(index, t0);
        setArgs&lt;index + 1, T1s...&gt;(std::forward&lt;T1s&gt;(t1s)...);
    }

    template&lt;int index, typename T0&gt;
    void setArgs(T0&amp;&amp; t0)
    {
        kernel_.setArg(index, t0);
    }

    template&lt;int index&gt;
    void setArgs()
    {
    }


public:
    KernelFunctor(Kernel kernel) : kernel_(kernel)
    {}

    KernelFunctor(
        const Program&amp; program,
        const string name,
        cl_int * err = NULL) :
        kernel_(program, name.c_str(), err)
    {}

    //! \brief Return type of the functor
    typedef Event result_type;

    /**
     * Enqueue kernel.
     * @param args Launch parameters of the kernel.
     * @param t0... List of kernel arguments based on the template type of the functor.
     */
    Event operator() (
        const EnqueueArgs&amp; args,
        Ts... ts)
    {
        Event event;
        setArgs&lt;0&gt;(std::forward&lt;Ts&gt;(ts)...);
        
        args.queue_.enqueueNDRangeKernel(
            kernel_,
            args.offset_,
            args.global_,
            args.local_,
            &amp;args.events_,
            &amp;event);

        return event;
    }

    /**
    * Enqueue kernel with support for error code.
    * @param args Launch parameters of the kernel.
    * @param t0... List of kernel arguments based on the template type of the functor.
    * @param error Out parameter returning the error code from the execution.
    */
    Event operator() (
        const EnqueueArgs&amp; args,
        Ts... ts,
        cl_int &amp;error)
    {
        Event event;
        setArgs&lt;0&gt;(std::forward&lt;Ts&gt;(ts)...);

        error = args.queue_.enqueueNDRangeKernel(
            kernel_,
            args.offset_,
            args.global_,
            args.local_,
            &amp;args.events_,
            &amp;event);
        
        return event;
    }

#if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200
    cl_int setSVMPointers(const vector&lt;void*&gt; &amp;pointerList)
    {
        return kernel_.setSVMPointers(pointerList);
    }

    template&lt;typename T0, typename... T1s&gt;
    cl_int setSVMPointers(const T0 &amp;t0, T1s &amp;... ts)
    {
        return kernel_.setSVMPointers(t0, ts...);
    }
#endif // #if CL_HPP_TARGET_OPENCL_VERSION &gt;= 200

    Kernel getKernel()
    {
        return kernel_;
    }
};

namespace compatibility {
    /**
     * Backward compatibility class to ensure that cl.hpp code works with opencl.hpp.
     * Please use KernelFunctor directly.
     */
    template&lt;typename... Ts&gt;
    struct make_kernel
    {
        typedef KernelFunctor&lt;Ts...&gt; FunctorType;

        FunctorType functor_;

        make_kernel(
            const Program&amp; program,
            const string name,
            cl_int * err = NULL) :
            functor_(FunctorType(program, name, err))
        {}

        make_kernel(
            const Kernel kernel) :
            functor_(FunctorType(kernel))
        {}

        //! \brief Return type of the functor
        typedef Event result_type;

        //! \brief Function signature of kernel functor with no event dependency.
        typedef Event type_(
            const EnqueueArgs&amp;,
            Ts...);

        Event operator()(
            const EnqueueArgs&amp; enqueueArgs,
            Ts... args)
        {
            return functor_(
                enqueueArgs, args...);
        }
    };
} // namespace compatibility


//----------------------------------------------------------------------------------------------------------------------

#undef CL_HPP_ERR_STR_
#if !defined(CL_HPP_USER_OVERRIDE_ERROR_STRINGS)
#undef __GET_DEVICE_INFO_ERR               
#undef __GET_PLATFORM_INFO_ERR             
#undef __GET_DEVICE_IDS_ERR                
#undef __GET_PLATFORM_IDS_ERR              
#undef __GET_CONTEXT_INFO_ERR              
#undef __GET_EVENT_INFO_ERR                
#undef __GET_EVENT_PROFILE_INFO_ERR        
#undef __GET_MEM_OBJECT_INFO_ERR           
#undef __GET_IMAGE_INFO_ERR                
#undef __GET_SAMPLER_INFO_ERR              
#undef __GET_KERNEL_INFO_ERR               
#undef __GET_KERNEL_ARG_INFO_ERR           
#undef __GET_KERNEL_SUB_GROUP_INFO_ERR     
#undef __GET_KERNEL_WORK_GROUP_INFO_ERR    
#undef __GET_PROGRAM_INFO_ERR              
#undef __GET_PROGRAM_BUILD_INFO_ERR        
#undef __GET_COMMAND_QUEUE_INFO_ERR        
#undef __CREATE_CONTEXT_ERR                
#undef __CREATE_CONTEXT_FROM_TYPE_ERR      
#undef __GET_SUPPORTED_IMAGE_FORMATS_ERR   
#undef __CREATE_BUFFER_ERR                 
#undef __COPY_ERR                          
#undef __CREATE_SUBBUFFER_ERR              
#undef __CREATE_GL_BUFFER_ERR              
#undef __CREATE_GL_RENDER_BUFFER_ERR       
#undef __GET_GL_OBJECT_INFO_ERR            
#undef __CREATE_IMAGE_ERR                  
#undef __CREATE_GL_TEXTURE_ERR             
#undef __IMAGE_DIMENSION_ERR               
#undef __SET_MEM_OBJECT_DESTRUCTOR_CALLBACK_ERR 
#undef __CREATE_USER_EVENT_ERR             
#undef __SET_USER_EVENT_STATUS_ERR         
#undef __SET_EVENT_CALLBACK_ERR            
#undef __WAIT_FOR_EVENTS_ERR               
#undef __CREATE_KERNEL_ERR                 
#undef __SET_KERNEL_ARGS_ERR               
#undef __CREATE_PROGRAM_WITH_SOURCE_ERR    
#undef __CREATE_PROGRAM_WITH_IL_ERR        
#undef __CREATE_PROGRAM_WITH_BINARY_ERR    
#undef __CREATE_PROGRAM_WITH_IL_ERR        
#undef __CREATE_PROGRAM_WITH_BUILT_IN_KERNELS_ERR    
#undef __BUILD_PROGRAM_ERR                 
#undef __COMPILE_PROGRAM_ERR               
#undef __LINK_PROGRAM_ERR                  
#undef __CREATE_KERNELS_IN_PROGRAM_ERR     
#undef __CREATE_COMMAND_QUEUE_WITH_PROPERTIES_ERR          
#undef __CREATE_SAMPLER_WITH_PROPERTIES_ERR                
#undef __SET_COMMAND_QUEUE_PROPERTY_ERR    
#undef __ENQUEUE_READ_BUFFER_ERR           
#undef __ENQUEUE_READ_BUFFER_RECT_ERR      
#undef __ENQUEUE_WRITE_BUFFER_ERR          
#undef __ENQUEUE_WRITE_BUFFER_RECT_ERR     
#undef __ENQEUE_COPY_BUFFER_ERR            
#undef __ENQEUE_COPY_BUFFER_RECT_ERR       
#undef __ENQUEUE_FILL_BUFFER_ERR           
#undef __ENQUEUE_READ_IMAGE_ERR            
#undef __ENQUEUE_WRITE_IMAGE_ERR           
#undef __ENQUEUE_COPY_IMAGE_ERR            
#undef __ENQUEUE_FILL_IMAGE_ERR            
#undef __ENQUEUE_COPY_IMAGE_TO_BUFFER_ERR  
#undef __ENQUEUE_COPY_BUFFER_TO_IMAGE_ERR  
#undef __ENQUEUE_MAP_BUFFER_ERR            
#undef __ENQUEUE_MAP_IMAGE_ERR             
#undef __ENQUEUE_UNMAP_MEM_OBJECT_ERR      
#undef __ENQUEUE_NDRANGE_KERNEL_ERR        
#undef __ENQUEUE_NATIVE_KERNEL             
#undef __ENQUEUE_MIGRATE_MEM_OBJECTS_ERR   
#undef __ENQUEUE_MIGRATE_SVM_ERR
#undef __ENQUEUE_ACQUIRE_GL_ERR            
#undef __ENQUEUE_RELEASE_GL_ERR            
#undef __CREATE_PIPE_ERR             
#undef __GET_PIPE_INFO_ERR           
#undef __RETAIN_ERR                        
#undef __RELEASE_ERR                       
#undef __FLUSH_ERR                         
#undef __FINISH_ERR                        
#undef __VECTOR_CAPACITY_ERR               
#undef __CREATE_SUB_DEVICES_ERR            
#undef __CREATE_SUB_DEVICES_ERR            
#undef __ENQUEUE_MARKER_ERR                
#undef __ENQUEUE_WAIT_FOR_EVENTS_ERR       
#undef __ENQUEUE_BARRIER_ERR               
#undef __UNLOAD_COMPILER_ERR               
#undef __CREATE_GL_TEXTURE_2D_ERR          
#undef __CREATE_GL_TEXTURE_3D_ERR          
#undef __CREATE_IMAGE2D_ERR                
#undef __CREATE_IMAGE3D_ERR                
#undef __CREATE_COMMAND_QUEUE_ERR          
#undef __ENQUEUE_TASK_ERR                  
#undef __CREATE_SAMPLER_ERR                
#undef __ENQUEUE_MARKER_WAIT_LIST_ERR                
#undef __ENQUEUE_BARRIER_WAIT_LIST_ERR               
#undef __CLONE_KERNEL_ERR     
#undef __GET_HOST_TIMER_ERR
#undef __GET_DEVICE_AND_HOST_TIMER_ERR

#endif //CL_HPP_USER_OVERRIDE_ERROR_STRINGS

// Extensions
#undef CL_HPP_INIT_CL_EXT_FCN_PTR_
#undef CL_HPP_INIT_CL_EXT_FCN_PTR_PLATFORM_

#if defined(CL_HPP_USE_CL_DEVICE_FISSION)
#undef CL_HPP_PARAM_NAME_DEVICE_FISSION_
#endif // CL_HPP_USE_CL_DEVICE_FISSION

#undef CL_HPP_NOEXCEPT_
#undef CL_HPP_DEFINE_STATIC_MEMBER_

} // namespace cl

#endif // CL_HPP_
</pre></body></html>Ztext/plainUUTF-8            (   7   N   `   v   �   �   � 7 7                          7 