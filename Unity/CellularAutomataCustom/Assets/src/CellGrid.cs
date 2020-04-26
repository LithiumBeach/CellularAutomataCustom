using System;
using UnityEngine;

namespace ca
{
    public class CellGrid
    {
        private Color[] m_Cells;
        private int m_Width;
        private int m_Height;

        public CellGrid(int w, int h)
        {
            m_Width = w; m_Height = h;
            m_Cells = new Color[w * h];
        }
        public CellGrid(int w, int h, Color color)
        {
            m_Width = w; m_Height = h;
            int size = w * h;

            m_Cells = new Color[size];
            for (int i = 0; i < size; i++)
            {
                m_Cells[i] = color;
            }
        }

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

        public Color At(int x, int y) { return m_Cells[Get1D(x, y)]; }
        public Color At(Vector2Int i) { return m_Cells[Get1D(i)]; }

        public Color[] GetNeighbors(int i1D, bool b_Toroidal = true) { return GetNeighbors(Get2D(i1D), b_Toroidal); }
        public Color[] GetNeighbors(Vector2Int i2D, bool b_Toroidal = true)
        {
            //TODO:
            if (b_Toroidal == false) { throw new NotImplementedException(); }

            Color[] n = new Color[8];
            int c = 0;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    //skip self
                    if (j == 0 && i == 0) { continue; }

                    n[c] = m_Cells[Get1D(CAMath.Mod((i2D.x + i), m_Width), CAMath.Mod((i2D.y + j), m_Height))];
                    c++;
                }
            }

            return n;
        }

        internal void SetColor(Vector2Int cellIndex, Color color)
        {
            m_Cells[Get1D(cellIndex)] = color;
        }
    }
}