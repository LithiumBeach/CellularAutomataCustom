using Sirenix.OdinInspector;
using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

//CellGridBehavior.cs
//The parent of this gameobject must have a Canvas component
//Manages all cells in a grid
//Manages grid zoom

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
        private Vector2 m_CellPixelSize;
        private void UpdateCellPixelSize()
        {
            m_CellPixelSize = new Vector2(
                m_RectTransform.rect.width / (float)CurZoomLevel.m_Size,
                m_RectTransform.rect.height / (float)CurZoomLevel.m_Size
            );
        }

        public class ZoomLevel
        {
            public Texture2D m_Tex = null;
            public int m_Size = 0;
            public ZoomLevel(int size) { m_Size = size; m_Tex = null; }
        }

        #region Zoom
        [Range(0, MAX_ZOOM_LEVEL)]
        private int m_Zoom;
        public int Zoom { get { return m_Zoom; } }

        private Dictionary<int, ZoomLevel> m_ZoomLevels = new Dictionary<int, ZoomLevel>
        {
            {0,     new ZoomLevel(Mathf.RoundToInt(Mathf.Lerp(MIN_ZOOM, MAX_ZOOM, CAMath.EaseIn(0.0f))))},
            {1,     new ZoomLevel(Mathf.RoundToInt(Mathf.Lerp(MIN_ZOOM, MAX_ZOOM, CAMath.EaseIn(0.1f))))},
            {2,     new ZoomLevel(Mathf.RoundToInt(Mathf.Lerp(MIN_ZOOM, MAX_ZOOM, CAMath.EaseIn(0.2f))))},
            {3,     new ZoomLevel(Mathf.RoundToInt(Mathf.Lerp(MIN_ZOOM, MAX_ZOOM, CAMath.EaseIn(0.3f))))},
            {4,     new ZoomLevel(Mathf.RoundToInt(Mathf.Lerp(MIN_ZOOM, MAX_ZOOM, CAMath.EaseIn(0.4f))))},
            {5,     new ZoomLevel(Mathf.RoundToInt(Mathf.Lerp(MIN_ZOOM, MAX_ZOOM, CAMath.EaseIn(0.5f))))},
            {6,     new ZoomLevel(Mathf.RoundToInt(Mathf.Lerp(MIN_ZOOM, MAX_ZOOM, CAMath.EaseIn(0.6f))))},
            {7,     new ZoomLevel(Mathf.RoundToInt(Mathf.Lerp(MIN_ZOOM, MAX_ZOOM, CAMath.EaseIn(0.7f))))},
            {8,     new ZoomLevel(Mathf.RoundToInt(Mathf.Lerp(MIN_ZOOM, MAX_ZOOM, CAMath.EaseIn(0.8f))))},
            {9,     new ZoomLevel(Mathf.RoundToInt(Mathf.Lerp(MIN_ZOOM, MAX_ZOOM, CAMath.EaseIn(0.9f))))},
            {10,    new ZoomLevel(Mathf.RoundToInt(Mathf.Lerp(MIN_ZOOM, MAX_ZOOM, CAMath.EaseIn(1.0f))))},
        };
        private const int MIN_ZOOM = 16;
        private const int MAX_ZOOM = 512;
        private const int MAX_ZOOM_LEVEL = 8;
        public ZoomLevel CurZoomLevel { get { return m_ZoomLevels[m_Zoom]; } }

        public void SetZoom(int zoomLevel)
        {
            m_Zoom = CAMath.Mod(zoomLevel, MAX_ZOOM_LEVEL+1);
            GetComponent<RawImage>().texture = m_ZoomLevels[m_Zoom].m_Tex;
            UpdateCellPixelSize();

            if (m_CellGrid != null)
            {
                //resize cellgrid, if it is expading, new cells will be the clear to color
                m_CellGrid.Resize(m_ZoomLevels[m_Zoom].m_Size, m_ClearToColor); 
            }
            else
            {
                ResetGrid();
            }

            //copy cell grid to texture
            SyncZoomTexture();
        }
        #endregion

        public CellGrid m_CellGrid;


        #region Interface Values

        //the image whose color we will always set to cleartocolor
        [Required]
        public RawImage m_ClearToColorButton;
        private int m_ClearToColor = 1;


        #endregion

        private void ResetGrid()
        {
            for (int i = 0; i < MAX_ZOOM_LEVEL + 1; i++)
            {
                m_ZoomLevels[i].m_Tex = null;
            }
            m_CellGrid = new CellGrid(m_ZoomLevels[m_Zoom].m_Size, m_ZoomLevels[m_Zoom].m_Size, 1);
            InitializeZoomLevels();

            SetZoom(m_Zoom);
            SyncZoomTexture();
        }

        private void InitializeZoomLevels()
        {
            //for each zoom level, construct a unique texture and store it in a collection
            for (int i = 0; i < MAX_ZOOM_LEVEL + 1; i++)
            {
                //create a square texture, sizes defined in zoom levels dictionary
                m_ZoomLevels[i].m_Tex = new Texture2D(m_ZoomLevels[i].m_Size, m_ZoomLevels[i].m_Size);

                ClearBoard(m_ClearToColor, i);
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
                    CurZoomLevel.m_Tex.SetPixel(x, -y - 1, CAColor.colors[m_CellGrid.At(x, y)]);
                }
            }

            CurZoomLevel.m_Tex.Apply();
        }


        public void EvaluateNextState(List<RuleData> rules)
        {
            foreach (RuleData rule in rules)
            {
                m_CellGrid.Evaluate(rule);
            }
            m_CellGrid.Apply();

            SyncZoomTexture();
        }


        public void Start()
        {
            //find components
            m_RectTransform = GetComponent<RectTransform>();
            m_RawImage = GetComponent<RawImage>();

            //register input callbacks
            WindowManager.Instance.OnLeftMouseDown += HandleLeftMouseDown;
            WindowManager.Instance.OnRightMouseDown += HandleRightMouseDown;

            ResetGrid();

            //update ui
            ChangeClearToColor(0);
        }

        private void HandleLeftMouseDown(Vector2 pos)
        {
            HandleMouseDown(pos, CAMath.LEFT);
        }
        private void HandleRightMouseDown(Vector2 pos)
        {
            HandleMouseDown(pos, CAMath.RIGHT);
        }

        private void HandleMouseDown(Vector2 pos, int ebutton)
        {
            //get mouse pos in local CellGrid pixel space
            Vector2 localMousePos = m_RectTransform.InverseTransformPoint(pos);

            //if mouse was not clicked inside cell grid, return
            if (!m_RectTransform.rect.Contains(localMousePos))
            {
                return;
            }

            //convert unity local coordinates into positive-valued y
            localMousePos.y = -localMousePos.y;

            //Floor since cells are aligned top left
            Vector2Int cellIndex = new Vector2Int(
                //x-space starts at 0 and goes up.
                Mathf.FloorToInt(localMousePos.x / m_CellPixelSize.x),
                //inverted y-axis
                Mathf.FloorToInt(localMousePos.y / m_CellPixelSize.y)
            );

            //ignore clicks outside the cell grid
            if (cellIndex.x < 0 || cellIndex.x >= CurZoomLevel.m_Size ||
                cellIndex.y < 0 || cellIndex.y >= CurZoomLevel.m_Size)
            {
                return;
            }

            //set int color id in data
            int newColor = m_CellGrid.At(cellIndex);

            //add or subtract color int if left+ : right-
            newColor += ebutton;
            if (ebutton == CAMath.LEFT && newColor >= CAColor.colors.Length)
            {
                //0 is transparent.
                newColor = 1;
            }
            else if (ebutton == CAMath.RIGHT && newColor <= 0)
            {
                newColor = CAColor.colors.Length - 1;
            }
            m_CellGrid.SetColor(cellIndex, newColor);

            SyncZoomTexture();
        }


        #region Event Trigger Callbacks
        public void ClearBoard()
        {
            ClearBoard(m_ClearToColor, m_Zoom);
        }
        public void ClearBoard(int caColor, int zoomLevel)
        {
            //clear data board (and Apply)
            m_CellGrid.ClearTo(caColor);

            //clear texture
            for (int y = 0; y < m_ZoomLevels[zoomLevel].m_Tex.height; y++)
            {
                for (int x = 0; x < m_ZoomLevels[zoomLevel].m_Tex.width; x++)
                {
                    m_ZoomLevels[zoomLevel].m_Tex.SetPixel(x, y, CAColor.colors[caColor]);
                }
            }
            m_ZoomLevels[zoomLevel].m_Tex.Apply();
            m_ZoomLevels[zoomLevel].m_Tex.filterMode = FilterMode.Point;
        }

        //1 = increase, -1 = decrease, 0 = unchanged
        public void ChangeClearToColor(int direction)
        {
            m_ClearToColor = CAColor.ChangeColorInt(m_ClearToColor, direction, false);
            m_ClearToColorButton.color = CAColor.colors[m_ClearToColor];
        }
        #endregion
    }
}