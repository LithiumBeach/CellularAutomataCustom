using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ca
{
    public class TileGridBehavior : MonoBehaviour
    {
        [Sirenix.OdinInspector.Required]
        public RectTransform m_RectTransform;
        //private Dictionary<Vector2Int, TileBehavior> m_Tiles = new Dictionary<Vector2Int, TileBehavior>();

        private class ZoomLevel
        {
            public Texture2D m_Tex = null;
            public int m_Size = 0;
            public ZoomLevel(int size) { m_Size = size; m_Tex = null; }
        }


        //s_Textures Index, pixel width
        private Dictionary<int, ZoomLevel> m_ZoomLevels = new Dictionary<int, ZoomLevel>
        {
            {0, new ZoomLevel(4)},
            {1, new ZoomLevel(8)},
            {2, new ZoomLevel(16)},
            {3, new ZoomLevel(32)},
            {4, new ZoomLevel(64)},
            {5, new ZoomLevel(128)},
            {6, new ZoomLevel(256)},
            {7, new ZoomLevel(512)},
            {8, new ZoomLevel(1024)}
        };
        private const int MAX_ZOOM = 8;
        [Range(0, MAX_ZOOM)]
        public int m_Zoom = 0;

        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Zoom In")]
        private void ZoomIn()
        {
            m_Zoom = Math.Max(0, m_Zoom - 1);

            GetComponent<RawImage>().texture = m_ZoomLevels[m_Zoom].m_Tex;
        }
        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Zoom Out")]
        private void ZoomOut()
        {
            m_Zoom = Math.Min(MAX_ZOOM, m_Zoom + 1);

            GetComponent<RawImage>().texture = m_ZoomLevels[m_Zoom].m_Tex;
        }

        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Refresh Grid")]
        private void ResetGrid()
        {
            for (int i = 0; i < MAX_ZOOM + 1; i++)
            {
                m_ZoomLevels[i].m_Tex = null;
            }
            InitializeTextures();
        }

        private void InitializeTextures()
        {
            //for each zoom level, construct a unique texture and store it in a collection
            for (int i = 0; i < MAX_ZOOM+1; i++)
            {
                //create a square texture, sizes defined in zoom levels dictionary
                m_ZoomLevels[i].m_Tex = new Texture2D(m_ZoomLevels[i].m_Size, m_ZoomLevels[i].m_Size);

                //initialize texture to white
                for (int y = 0; y < m_ZoomLevels[i].m_Tex.height; y++)
                {
                    for (int x = 0; x < m_ZoomLevels[i].m_Tex.width; x++)
                    {
                        m_ZoomLevels[i].m_Tex.SetPixel(x, y, x % 2 == 1 ? Color.white : Color.black);
                    }
                }
                m_ZoomLevels[i].m_Tex.Apply();
                m_ZoomLevels[i].m_Tex.filterMode = FilterMode.Point;
            }
        }

        public void Awake()
        {

        }


    }
}