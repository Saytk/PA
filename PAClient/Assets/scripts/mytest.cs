using System;
using System.Runtime.InteropServices;
using UnityEngine;
using fts;


[StructLayout(LayoutKind.Sequential)]
public struct SimpleStruct
{
    public IntPtr weights;
    public IntPtr architecture; // Pointer to architecture array
                                // Pointer to weights array
    public int weightsSize; // Size of weights array
    public int architectureSize; // Size of architecture array
    public double learningRate;
    // Add more fields as needed
}

public class MLP
{
    SimpleStruct s;
    double[] weights;
    int[] architecture;

    public MLP(int num_inputs, int num_outputs, int num_hidden_layers,
       int num_neurons_per_hidden_layer, double learning_rate)
    {
        s = MyDll.createMlp(num_inputs, num_outputs, num_hidden_layers, num_neurons_per_hidden_layer, learning_rate);
    }

    public double[] GetWeights()
    {
        weights = new double[s.weightsSize];
        Marshal.Copy(s.weights, weights, 0, s.weightsSize);
        return weights;
    }

    public int[] GetArchitecture()
    {
        architecture = new int[s.architectureSize];
        Marshal.Copy(s.architecture, architecture, 0, s.architectureSize);
        return architecture;
    }

    // Rest of the MLP class...
}
[PluginAttr("DllTest")]
public static class MyDll
{

    
    [PluginFunctionAttr("getTrace")]
    public static GetTraceDelegate getTrace;
    public delegate float GetTraceDelegate();

  

    [PluginFunctionAttr("create_mlp")]
    public static CreateMlp createMlp = null;
    public delegate SimpleStruct CreateMlp(int num_inputs, int num_outputs, int num_hidden_layers,
       int num_neurons_per_hidden_layer, double learning_rate);

}




public class MyTest : MonoBehaviour
{
    void Start()
    {

        MLP m = new MLP(3, 3, 3, 4, 0.01);
        Debug.Log("Start method was called." +m.GetWeights()[0]);
        if (MyDll.getTrace == null)
        {
            Debug.LogError("Failed to load the DLL");
            return;
        }

        Debug.Log("The trace value is: " + MyDll.getTrace());
    }
}
