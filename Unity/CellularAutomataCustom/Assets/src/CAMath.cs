using System;
using UnityEngine;

namespace ca
{
    public class CAMath
    {
        //C# cannot assign preprocessor directives to values
        public static int LEFT = 1;
        public static int RIGHT = -1;


        //integer modulo between a range of values high and low
        //low is inclusive, high is exclusive
        public static int Mod(int a, int b) { return Mod(a, 0, b); }
        public static int Mod(int a, int low, int high)
        {
            //quick fix for negative a values
            while (a < low) { a += high; }

            return ((a - low) % (high - low)) + low;
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