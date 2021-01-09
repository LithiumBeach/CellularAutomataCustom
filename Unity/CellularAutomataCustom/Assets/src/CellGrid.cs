using System;
using System.Collections.Generic;
using UnityEngine;

namespace ca
{
    //CellGrid
    //numerical properties of 2D grids of cells
    //map containing numerical representation of cells
    //
    //TODO: is general enough for (numerically) managing any 2D grid of things
    //ie: lookup integers or enums for sprite atlas
    //TODO: remove 'color' specifications
    //TODO: IEvaluate(object)
    public class CellGrid
    {
        //TODO: update this comment
        //grid of integers representing, ie: colors indexed at CAColor.colors[int]
        private int[] m_Cells;
        //all rule evaluations are saved to the cache
        //the cache is moved to m_Cells only when instructed to Apply().
        private int[] m_CellsCache = null;
        private int m_Width;
        private int m_Height;
        private int m_Area;

        public int GetArea() { return m_Area; }
        public int GetWidth() { return m_Width; }
        public int GetHeight() { return m_Height; }

        private Vector2Int Get2D(int i, int rowLength)
        {
            return new Vector2Int(i % rowLength, i / rowLength);
        }
        private int Get1D(Vector2Int i, int rowLength)
        {
            return Get1D(i.x, i.y, rowLength);
        }
        private int Get1D(int x, int y, int rowLength)
        {
            return rowLength * y + x;
        }


        public CellGrid(int w, int h, int color)
        {
            m_Width = w; m_Height = h; m_Area = w * h;

            ClearTo(color);
        }

        //param: defaultValue= if the array has to expand, what value should we put for the new indices?
        public void Resize(Vector2Int size, int defaultValue = 0) { Resize(size.x, size.y, defaultValue); }
        public void Resize(int size, int defaultValue = 0) { Resize(size, size, defaultValue); }
        public void Resize(int w, int h, int defaultValue = 0)
        {
            int prevW = m_Width; int prevH = m_Height; int prevArea = m_Area;
            m_Width = w; m_Height = h; m_Area = w * h;

            if (m_Cells == null || m_Cells.Length < 1)
            {
                m_Cells = new int[m_Area];
            }
            else
            {
                //expand or contract m_Cells, keeping values intact
                int[] m_CellsCopy = new int[prevArea];
                Array.ConstrainedCopy(m_Cells, 0, m_CellsCopy, 0, Math.Min(m_Cells.Length, m_CellsCopy.Length));
                m_Cells = new int[m_Area];

                for (int y = 0; y < m_Height; y++)
                {
                    for (int x = 0; x < m_Width; x++)
                    {
                        if (x < prevW && y < prevH)
                        {
                            m_Cells[Get1D(x, y, m_Width)] = m_CellsCopy[Get1D(x, y, prevW)];
                        }
                        else
                        {
                            m_Cells[Get1D(x, y, m_Width)] = defaultValue;
                        }
                    }
                }

                m_CellsCopy = null;
            }
            //TODO: clear cache?
            m_CellsCache = null;
        }

        public int At(int x, int y) { return m_Cells[Get1D(x, y, m_Width)]; }
        public int At(Vector2Int i) { return m_Cells[Get1D(i, m_Width)]; }

        public int[] GetNeighbors(int i1D, bool b_Toroidal = true) { return GetNeighbors(Get2D(i1D, m_Width), b_Toroidal); }
        public int[] GetNeighbors(Vector2Int i2D, bool b_Toroidal = true)
        {
            //TODO:
            if (b_Toroidal == false) { throw new NotImplementedException(); }

            int[] n = new int[8];
            int c = 0;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    //skip self
                    if (j == 0 && i == 0) { continue; }

                    n[c] = m_Cells[Get1D(
                        CAMath.Mod((i2D.x + i), m_Width),
                        CAMath.Mod((i2D.y + j), m_Height),
                        m_Width)
                        ];
                    c++;
                }
            }

            return n;
        }

        internal void SetColor(int x, int y, int color)
        {
            m_Cells[Get1D(x, y, m_Width)] = color;
        }
        internal void SetColor(Vector2Int cellIndex, int color)
        {
            m_Cells[Get1D(cellIndex, m_Width)] = color;
        }

        //TODO: Evaluate can be generic, object param cast to RuleData ?
        internal virtual void Evaluate(RuleData rule)
        {
            //cache m_Cells only on the first evaluation since we wrote the cache
            if (m_CellsCache == null)
            {
                //m_CellsCache = m_Cells; 
                m_CellsCache = new int[m_Area];
                for (int i = 0; i < m_Area; i++)
                {
                    m_CellsCache[i] = m_Cells[i];
                }
            }
            //for each cell
            for (int i = 0; i < m_Area; i++)
            {
                int iColor = m_Cells[i];

                //if concerning cells of color: (0=all)
                if (rule.m_ThisColor == 0 ||
                    rule.m_ThisColor == iColor)
                {
                    int[] neighbors = GetNeighbors(i);

                    //find all neighbors matching ifcolor
                    int relevantNeighborCount = 0;
                    for (int j = 0; j < neighbors.Length; j++)
                    {
                        if (neighbors[j] == rule.m_IfColor)
                        {
                            relevantNeighborCount++;
                        }
                    }

                    //if between <MIN (inclusive)> and <MAX (inclusive)> neighbors are <COLOR>
                    if ((rule.m_MinNumNeighbors <= relevantNeighborCount &&
                        rule.m_MaxNumNeighbors >= relevantNeighborCount) ||
                        rule.m_IfColor == 0) //evaluate to true if if color is ANY
                    {
                        //Then this cell will change to <COLOR>
                        m_CellsCache[i] = rule.m_ThenColor;
                    }
                }
                //if no change in color is determined, remain unchanged into the next state
            }
        }

        internal void Apply()
        {
            //m_Cells = Array.Copy(m_CellsCache);
            for (int i = 0; i < m_Area; i++)
            {
                m_Cells[i] = m_CellsCache[i];
            }
            m_CellsCache = null;
        }

        internal void ClearTo(int caColor)
        {
            if (m_Cells == null)
            {
                m_Cells = new int[m_Area];
            }
            m_CellsCache = null;

            for (int i = 0; i < m_Area; i++)
            {
                m_Cells[i] = caColor;
            }
        }
    }
}