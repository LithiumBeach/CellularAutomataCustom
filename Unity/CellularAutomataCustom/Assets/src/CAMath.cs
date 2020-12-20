using System;
using UnityEngine;

namespace ca
{
    public class CAMath
    {
        //C# cannot assign preprocessor directives to values
        public static int LEFT = 1;
        public static int RIGHT = -1;


        //integer modulo that works with negative numbers
        public static int Mod(int a, int b)
        {
            //Debug.Log(a.ToString() + " % " + b.ToString() + " = " + (((a % b) + b) % b).ToString());
            return ((a % b) + b) % b;
        }

        public static float SmoothStep(float t)
        {
            return t * t * (3f - 2f * t);
        }
        public static float EaseIn(float t)
        {
            return 1f - Mathf.Cos(t * Mathf.PI * 0.5f);
        }
        public static float EaseOut(float t)
        {
            return Mathf.Sin(t * Mathf.PI * 0.5f);
        }
    }
}