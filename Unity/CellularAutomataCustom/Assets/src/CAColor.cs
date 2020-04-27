using UnityEngine;

namespace ca
{
    public static class CAColor
    {
        //these colors will change according to user selection, but only the indices are stored in data,
        //so when a color changes, the board and rules can update to the new color.
        public static Color[] colors = new Color[]
        {
            Color.clear, //All Colors (ie. concerning cells of color "all")
            Color.white,
            Color.black,
            Color.red,
            Color.green,
            Color.blue
        };

    }
}