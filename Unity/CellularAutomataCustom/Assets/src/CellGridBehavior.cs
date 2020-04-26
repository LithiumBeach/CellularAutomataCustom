using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ca
{
    [RequireComponent(typeof(RectTransform))]
    [RequireComponent(typeof(RawImage))]
    public class CellGridBehavior : MonoBehaviour
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

        #region Zoom
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

        private void SetZoom(int zoomLevel)
        {
            m_Zoom = Math.Max(0, zoomLevel);
            GetComponent<RawImage>().texture = m_ZoomLevels[m_Zoom].m_Tex;
            UpdateCellPixelSize();

            //TODO: m_CellGrid.Resize(m_ZoomLevels[m_Zoom].size)

            //TODO: CopyCellGridToTexture()
        }
        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Zoom In")]
        private void ZoomIn() { SetZoom(m_Zoom - 1); }
        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Zoom Out")]
        private void ZoomOut() { SetZoom(m_Zoom + 1); }
        #endregion

        public CellGrid m_CellGrid;

        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Reset Grid")]
        private void ResetGrid()
        {
            for (int i = 0; i < MAX_ZOOM + 1; i++)
            {
                m_ZoomLevels[i].m_Tex = null;
            }
            m_CellGrid = new CellGrid(m_ZoomLevels[m_Zoom].m_Size, m_ZoomLevels[m_Zoom].m_Size, Color.blue);
            InitializeZoomLevels(Color.blue);

            SetZoom(m_Zoom);
        }

        private void InitializeZoomLevels(Color color)
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
                        m_ZoomLevels[i].m_Tex.SetPixel(x, y, color);
                    }
                }
                m_ZoomLevels[i].m_Tex.Apply();
                m_ZoomLevels[i].m_Tex.filterMode = FilterMode.Point;
            }
        }

        private void SyncZoomTexture()
        {
            for (int y = 0; y < CurZoomLevel.m_Tex.height; y++)
            {
                for (int x = 0; x < CurZoomLevel.m_Tex.width; x++)
                {
                    //re-invert y-axis.
                    //also subtract one, because unity's texture space apparently starts at -1
                    CurZoomLevel.m_Tex.SetPixel(x, -y - 1, m_CellGrid.At(x, y));
                }
            }
        }

        #region Events
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
            //get mouse pos in local CellGrid pixel space
            Vector2 localMousePos = m_RectTransform.InverseTransformPoint(pos);
            //Floor since cells are aligned top left
            Vector2Int cellIndex = new Vector2Int(
                //x-space starts at 0 and goes up.
                Mathf.FloorToInt(localMousePos.x / m_CellPixelSize.x),
                //inverted y-axis
                (Mathf.FloorToInt(-localMousePos.y / m_CellPixelSize.y))
            );
            //modulo (including negatives) with board size
            //toroidal wraparound asteroids.
            cellIndex = new Vector2Int(
                CAMath.Mod(cellIndex.x, CurZoomLevel.m_Size),
                CAMath.Mod(cellIndex.y, CurZoomLevel.m_Size)
            );

            //debug
            //CurZoomLevel.m_Tex.SetPixel(cellIndex.x, cellIndex.y, Color.black);

            m_CellGrid.SetColor(cellIndex, Color.green);

            SyncZoomTexture();


            CurZoomLevel.m_Tex.Apply();

            //TODO: is this necessary? Can we tell the raw image to reference this texture with a pointer?
            m_RawImage.texture = m_ZoomLevels[m_Zoom].m_Tex;
        }
        #endregion
    }
}