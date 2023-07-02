using System;
using System.Runtime.InteropServices;
using UnityEngine;
using fts;

[PluginAttr("DllTest")]
public static class MyDll
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate float GetTraceDelegate();

    [PluginFunctionAttr("getTrace")]
    public static GetTraceDelegate getTrace;
}

public class MyTest : MonoBehaviour
{
    void Start()
    {
        Debug.Log("Start method was called.");

        if (MyDll.getTrace == null)
        {
            Debug.LogError("Failed to load the DLL");
            return;
        }

        Debug.Log("The trace value is: " + MyDll.getTrace());
    }
}