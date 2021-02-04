using Sirenix.OdinInspector;
using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using Random = UnityEngine.Random;

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
            m_Zoom = CAMath.Mod(zoomLevel, MAX_ZOOM_LEVEL + 1);
            GetComponent<RawImage>().texture = m_ZoomLevels[m_Zoom].m_Tex;
            UpdateCellPixelSize();

            if (m_CellGrid != null)
            {
                //resize cellgrid, if it is expading, new cells will be the clear to color
                m_CellGrid.Resize(m_ZoomLevels[m_Zoom].m_Size, m_ClearToColor == 0 ? 1 : m_ClearToColor);
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
        [Required]
        public TextMeshProUGUI m_ClearToColorButtonText;
        private int m_ClearToColor = 1;
        public int ClearToColor { get { return m_ClearToColor; } }

        [Range(0.0f, 1.0f)]
        public float m_ShuffleBgProbability;


        #endregion

        //cache the cellindex the user clicks down on, to compare with the click up cellindex
        private Vector2Int m_OnLeftMouseDownCellIndex;
        private Vector2Int m_OnRightMouseDownCellIndex;
        private static Vector2Int c_InvalidIndex = new Vector2Int(-1, -1);

        //pause and unpause grid interaction
        public bool b_IsInputActive = true;


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

        public void SyncZoomTexture()
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
            if (rules != null && rules.Count > 0)
            {
                foreach (RuleData rule in rules)
                {
                    m_CellGrid.Evaluate(rule);
                }

                m_CellGrid.Apply();
            }

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
            WindowManager.Instance.OnLeftMouseUp += HandleLeftMouseUp;
            WindowManager.Instance.OnRightMouseUp += HandleRightMouseUp;
            WindowManager.Instance.WhileLeftMouseDown +=  HandleWhileLeftMouse;
            WindowManager.Instance.WhileRightMouseDown += HandleWhileRightMouse;

            ResetGrid();

            //update ui
            ChangeClearToColor(0);

            //initialize to zoom level 2
            WindowManager.Instance.SetZoom(2);
        }

        #region mouse event handlers
        private void HandleLeftMouseDown(Vector2 pos)
        {
            if (WindowManager.Instance.IsMainCanvasActive())
            {
                Vector2Int cellIndex = GetCellIndexFromScreenPos(pos);
                if (cellIndex == c_InvalidIndex) { return; } //exit if invalid position
                m_OnLeftMouseDownCellIndex = cellIndex;
            }
        }
        private void HandleRightMouseDown(Vector2 pos)
        {
            if (b_IsInputActive && WindowManager.Instance.IsMainCanvasActive())
            {
                Vector2Int cellIndex = GetCellIndexFromScreenPos(pos);
                if (cellIndex == c_InvalidIndex) { return; } //exit if invalid position
                m_OnRightMouseDownCellIndex = cellIndex;
            }
        }

        private void HandleWhileLeftMouse(Vector2 pos)
        {
            if (b_IsInputActive && WindowManager.Instance.IsMainCanvasActive())
            {
                Vector2Int cellIndex = GetCellIndexFromScreenPos(pos);
                if (cellIndex == c_InvalidIndex) { return; } //exit if invalid position

                //if this cellindex is not the same as the down cellindex, draw
                if (cellIndex != m_OnLeftMouseDownCellIndex)
                {
                    //get next/previous color int
                    int newColor = CAColor.ChangeColorInt(m_CellGrid.At(m_OnLeftMouseDownCellIndex), 0, b_includeClear: false);

                    //set int color id in data
                    m_CellGrid.SetColor(cellIndex, newColor);

                    SyncZoomTexture();
                }
            }
        }
        private void HandleWhileRightMouse(Vector2 pos)
        {
            //stub -- do nothing -- no drawing with right clicks
        }

        private void HandleLeftMouseUp(Vector2 pos)
        {
            HandleMouseUp(pos, CAMath.LEFT);
        }
        private void HandleRightMouseUp(Vector2 pos)
        {
            HandleMouseUp(pos, CAMath.RIGHT);
        }

        private void HandleMouseUp(Vector2 pos, int dir)
        {
            if (b_IsInputActive && WindowManager.Instance.IsMainCanvasActive())
            {
                Vector2Int cellIndex = GetCellIndexFromScreenPos(pos);
                if (cellIndex == c_InvalidIndex) { return; } //exit if invalid position

                //if the click down is in the same cell as the click up
                if ((m_OnLeftMouseDownCellIndex == cellIndex && dir == CAMath.LEFT) ||
                    (m_OnRightMouseDownCellIndex == cellIndex && dir == CAMath.RIGHT))
                {
                    //get next/previous color int
                    int newColor = CAColor.ChangeColorInt(m_CellGrid.At(cellIndex), dir, b_includeClear: false);

                    //set int color id in data
                    m_CellGrid.SetColor(cellIndex, newColor);

                    SyncZoomTexture();
                }
                //otherwise nothing needs to be done,
            }
        }
        #endregion

        private Vector2Int GetCellIndexFromScreenPos(Vector2 pos)
        {
            //get mouse pos in local CellGrid pixel space
            Vector2 localMousePos = m_RectTransform.InverseTransformPoint(pos);

            //if mouse was not clicked inside cell grid, return
            if (!m_RectTransform.rect.Contains(localMousePos))
            {
                return c_InvalidIndex;
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
                return c_InvalidIndex;
            }
            return cellIndex;
        }


        #region Event Trigger Callbacks
        public void ClearBoard()
        {
            ClearBoard(m_ClearToColor, m_Zoom);
        }
        public void ClearBoard(int caColor)
        {
            ClearBoard(caColor, m_Zoom);
        }
        public void ClearBoard(int caColor, int zoomLevel)
        {
            //if caColor = "ANY"
            if (caColor == 0)
            {
                ShuffleBoard();
                TutorialManager.Instance.b_HasEverClearedToAny = true;
            }
            else
            {
                //clear data board (and Apply)
                m_CellGrid.ClearTo(caColor);
            }

            //clear texture
            for (int y = 0; y < CurZoomLevel.m_Tex.height; y++)
            {
                for (int x = 0; x < CurZoomLevel.m_Tex.width; x++)
                {
                    m_ZoomLevels[zoomLevel].m_Tex.SetPixel(x, -y - 1, CAColor.colors[m_CellGrid.At(x, y)]);
                }
            }

            m_ZoomLevels[zoomLevel].m_Tex.Apply();
            m_ZoomLevels[zoomLevel].m_Tex.filterMode = FilterMode.Point;
        }

        private void ShuffleBoard()
        {
            List<int> allColorsInRuleset = SaveLoadManager.Instance.GetCurrentRulesetColors();
            if(allColorsInRuleset.Count < 2) { return; }

            //ALL should not be in this list
            Debug.Assert(!allColorsInRuleset.Contains(0));

            //foreach cell in grid
            for (int i = 0; i < m_CellGrid.GetArea(); i++)
            {
                //roll normalized dice (0-1)
                float nDice = Random.Range(0.0f, 1.0f);

                if (nDice < m_ShuffleBgProbability)
                {
                    //cell = bgColor = 1
                    m_CellGrid.SetColor(i, 1);
                }
                //if not bg, pick random color
                else
                {
                    //cell = roll integer dice range [2, CAColors.colors.Length]
                    m_CellGrid.SetColor(i, allColorsInRuleset[Random.Range(0, allColorsInRuleset.Count)]);
                }
            }
        }

        //1 = increase, -1 = decrease, 0 = unchanged
        public void ChangeClearToColor(int direction)
        {
            m_ClearToColor = CAColor.ChangeColorInt(m_ClearToColor, direction, true);

            //update rule button background
            m_ClearToColorButton.texture = m_ClearToColor == 0 ? WindowManager.Instance.m_AnyColorBgTex : null;
            m_ClearToColorButton.color = m_ClearToColor == 0 ? Color.white : CAColor.colors[m_ClearToColor];

            m_ClearToColorButtonText.text = CAColor.GetColorText(m_ClearToColor);
        }
        #endregion
    }
}