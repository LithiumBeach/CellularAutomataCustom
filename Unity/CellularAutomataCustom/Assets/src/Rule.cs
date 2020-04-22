using UnityEngine;

namespace ca
{
    public class Rule
    {
        //Conway's Game of Life Rules:
        /*
         * Any live cell with fewer than two live neighbours dies (referred to as underpopulation or exposure[1]).
         * Any live cell with more than three live neighbours dies(referred to as overpopulation or overcrowding).
         * Any live cell with two or three live neighbours lives, unchanged, to the next generation.
         * Any dead cell with exactly three live neighbours will come to life.
         */

        //CA Custom Rule
        /*
         * Concerning cells of color: <COLOR:ThisColorIndex>,
         * If between <INCLUSIVE_MIN: MinNumNeighbors> and <INCLUSIVE_MAX: MaxNumNeighbors> neighboring cells are <COLOR: IfColor>,
         * This cell will become <COLOR: ThenColor>
         */


    }

}