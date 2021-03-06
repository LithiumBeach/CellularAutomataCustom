﻿using System.Collections.Generic;
using UnityEngine;

namespace ca
{
    public static class CAColor
    {
        //these colors will change according to user selection, but only the indices are stored in data,
        //so when a color changes, the board and rules can update to the new color.
        public static List<Color> colors = new List<Color>
        {
        };
        public static List<Color> defaultColors = new List<Color>
        {
            Color.clear, //All Colors (ie. concerning cells of color "all")
            Color.white,
            Color.black,
            Color.red,
            Color.green,
            Color.blue
        };

        public static int ChangeColorInt(int currentColor, int direction, bool b_includeClear=false)
        {
            if(!b_includeClear && currentColor == 1 && direction < 0) { return CAColor.colors.Count-1; }
            return CAMath.Mod(currentColor + direction, b_includeClear ? 0 : 1, CAColor.colors.Count);
        }

        //if Color.Invisible, label color box "ANY"
        public static string GetColorText(int colorIndex)
        {
            return colorIndex == 0 ? "ANY" : "";
        }

    }
}