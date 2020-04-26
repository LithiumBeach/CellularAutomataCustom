using UnityEngine;

namespace ca
{
    public class CAMath
    {
        //integer modulo that works with negative numbers
        public static int Mod(int a, int b)
        {
            //Debug.Log(a.ToString() + " % " + b.ToString() + " = " + (((a % b) + b) % b).ToString());
            return ((a % b) + b) % b;
        }
    }
}