//ColorList.cs
//Serializable List<Color>
using System.Collections.Generic;
using UnityEngine;

namespace ca
{
    [System.Serializable]
    public class ColorList
    {
        public ColorList() { list = new List<Color>(); }
        public ColorList(List<Color> l) { list = new List<Color>(l); }
        public ColorList(Color[] a) { list = new List<Color>(a); }

        public List<Color> list;
    }
}