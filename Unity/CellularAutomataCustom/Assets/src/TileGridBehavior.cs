using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ca
{
    [RequireComponent(typeof(RectTransform))]
    [RequireComponent(typeof(RawImage))]
    public class TileGridBehavior : MonoBehaviour
    {
        [Sirenix.OdinInspector.Required]
        public RectTransform m_RectTransform;

        private RawImage m_RawImage;

        //the size of one cell in px: this is cached when zoom level changes
        private Vector2Int m_CellPixelSize;
        private void UpdateCellPixelSize()
        {
            m_CellPixelSize = new Vector2Int(
                (int)m_RectTransform.rect.width / CurZoomLevel.m_Size,
                (int)m_RectTransform.rect.height / CurZoomLevel.m_Size
            );
        }

        public class ZoomLevel
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
        private int m_Zoom = 0;
        public ZoomLevel CurZoomLevel { get { return m_ZoomLevels[m_Zoom]; } }

        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Zoom In")]
        private void ZoomIn()
        {
            m_Zoom = Math.Max(0, m_Zoom - 1);
            GetComponent<RawImage>().texture = m_ZoomLevels[m_Zoom].m_Tex;
            UpdateCellPixelSize();
        }
        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Zoom Out")]
        private void ZoomOut()
        {
            m_Zoom = Math.Min(MAX_ZOOM, m_Zoom + 1);
            GetComponent<RawImage>().texture = m_ZoomLevels[m_Zoom].m_Tex;
            UpdateCellPixelSize();
        }

        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Refresh Grid")]
        private void ResetGrid()
        {
            for (int i = 0; i < MAX_ZOOM + 1; i++)
            {
                m_ZoomLevels[i].m_Tex = null;
            }
            InitializeTextures();

            m_Zoom = 0;
            UpdateCellPixelSize();
        }

        private void InitializeTextures()
        {
            //for each zoom level, construct a unique texture and store it in a collection
            for (int i = 0; i < MAX_ZOOM + 1; i++)
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
            //find components
            m_RectTransform = GetComponent<RectTransform>();
            m_RawImage = GetComponent<RawImage>();

            //register input callbacks
            WindowManager.Instance.OnLeftMouseDown += HandleLeftMouseDown;

            ResetGrid();
        }

        private void HandleLeftMouseDown(Vector2 pos)
        {
            //get mouse pos in local TileGrid pixel space
            Vector2 localMousePos = m_RectTransform.InverseTransformPoint(pos);

            //Floor since tiles are aligned top left
            Vector2Int tileIndex = new Vector2Int(
                //x-space starts at 0 and goes up.
                Mathf.FloorToInt(localMousePos.x) / m_CellPixelSize.x,
                //for whatever miserable reason, the top-left pixel is (0, -1)
                Mathf.FloorToInt(localMousePos.y) / m_CellPixelSize.y - 1
            );

            CurZoomLevel.m_Tex.SetPixel(tileIndex.x, tileIndex.y, Color.black);
            CurZoomLevel.m_Tex.Apply();
            m_RawImage.texture = m_ZoomLevels[m_Zoom].m_Tex;
            Debug.Log(tileIndex);
        }

    }
}