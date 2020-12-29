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

        public static int ChangeColorInt(int currentColor, int direction, bool b_includeClear=false)
        {
            Debug.Assert(direction == -1 || direction == 1);
            currentColor += direction;

            if (currentColor <= 0)
            {
                currentColor = CAColor.colors.Length - 1;
            }
            else if (currentColor >= CAColor.colors.Length)
            {
                //0 is transparent (ALL)
                currentColor = b_includeClear ? 0 : 1;
            }
            return currentColor;
        }

    }
}