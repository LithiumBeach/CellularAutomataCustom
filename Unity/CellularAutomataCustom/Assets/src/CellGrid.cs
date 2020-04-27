using System;
using System.Collections.Generic;
using UnityEngine;

namespace ca
{
    public class CellGrid
    {
        //ints represent colors indexed at CAColor.colors[int]
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

        private Vector2Int Get2D(int i)
        {
            return new Vector2Int(i % m_Width, i / m_Width);
        }
        private int Get1D(Vector2Int i)
        {
            return m_Width * i.y + i.x;
        }
        private int Get1D(int x, int y)
        {
            return m_Width * y + x;
        }


        public CellGrid(int w, int h)
        {
            m_Width = w; m_Height = h; m_Area = w * h;
            m_Cells = new int[w * h];
        }
        public CellGrid(int w, int h, int color)
        {
            m_Width = w; m_Height = h; m_Area = w * h;
            int size = w * h;

            m_Cells = new int[size];
            for (int i = 0; i < size; i++)
            {
                m_Cells[i] = color;
            }
        }


        public int At(int x, int y) { return m_Cells[Get1D(x, y)]; }
        public int At(Vector2Int i) { return m_Cells[Get1D(i)]; }

        public int[] GetNeighbors(int i1D, bool b_Toroidal = true) { return GetNeighbors(Get2D(i1D), b_Toroidal); }
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
                        CAMath.Mod((i2D.y + j), m_Height))
                        ];
                    c++;
                }
            }

            return n;
        }

        internal void SetColor(Vector2Int cellIndex, int color)
        {
            m_Cells[Get1D(cellIndex)] = color;
        }

        internal void Evaluate(Rule rule)
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
                    for(int j = 0; j < neighbors.Length; j++)
                    {
                        if(neighbors[j] == rule.m_IfColor)
                        {
                            relevantNeighborCount++;
                        }
                    }

                    //if between <MIN (inclusive)> and <MAX (inclusive)> neighbors are <COLOR>
                    if (rule.m_MinNumNeighbors <= relevantNeighborCount &&
                        rule.m_MaxNumNeighbors >= relevantNeighborCount)
                    {
                        //Then this cell will change to <COLOR>
                        m_CellsCache[i] = rule.m_ThenColor;
                    }
                }
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
    }
}