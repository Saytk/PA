// SimpleDLL.h

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

extern "C"
{
DLL_EXPORT int Add(int a, int b);
}


