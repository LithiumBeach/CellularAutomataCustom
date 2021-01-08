using UnityEngine;

namespace ca
{
    [System.Serializable]
    public class RuleData
    {
        //Conway's Game of Life Rules:
        /*
         * Any live cell with fewer than two live neighbours dies (referred to as underpopulation or exposure[1]).
         * Any live cell with more than three live neighbours dies(referred to as overpopulation or overcrowding).
         * (USELESS) Any live cell with two or three live neighbours lives, unchanged, to the next generation. (USELESS)
         * Any dead cell with exactly three live neighbours will come to life.
         */

        //CA Custom Rule
        /*
         * Concerning cells of color: <COLOR:ThisColorIndex>,
         * If between <INCLUSIVE_MIN: MinNumNeighbors> and <INCLUSIVE_MAX: MaxNumNeighbors> neighboring cells are <COLOR: IfColor>,
         * This cell will become <COLOR: ThenColor>
         */

        //index of CAColor.colors
        //"Concerning cells of this color: <>"
        public int m_ThisColor = 0;

        //"If between <min> and <max> neighbors" are <color>
        public int m_MinNumNeighbors = 0; //inclusive
        public int m_MaxNumNeighbors = 8; //inclusive
        public int m_IfColor = 1;

        //"Then this Cell will change to <color>
        public int m_ThenColor = 2;
    }

}