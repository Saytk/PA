using fts;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestScript : MonoBehaviour
{

    [PluginAttr("libPAMLDLL")]
    public static class FooPlugin
    {
        [PluginFunctionAttr("Add")]
        public static Sum sum = null;
        public delegate float Sum(float a, float b);
    }

    // Start is called before the first frame update
    void Start() {
    }

}
