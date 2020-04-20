using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ca
{
    [RequireComponent(typeof(GridLayoutGroup))]
    public class TileGridBehavior : MonoBehaviour
    {
        private GridLayoutGroup m_Grid;
        private RectTransform m_RectTransform;
        private Dictionary<Vector2Int, TileBehavior> m_Tiles = new Dictionary<Vector2Int, TileBehavior>();

        [Sirenix.OdinInspector.Required]
        public GameObject m_TilePrefab = null;

        #region Width/Height
        //width & height in # of tiles
        [Range(2,1024)]
        public int m_TileSize;
        #endregion

        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Refresh Grid")]
        private void ResetGrid()
        {
            if (m_Tiles == null)
            {
                m_Tiles = new Dictionary<Vector2Int, TileBehavior>();
            }
            if (m_Grid == null)
            {
                m_Grid = GetComponent<GridLayoutGroup>();
            }
            if (m_RectTransform == null)
            {
                m_RectTransform = GetComponent<RectTransform>();
            }
            DestroyTiles();
            InitializeTiles();
        }

        private void DestroyTiles()
        {
            transform.DestroyAllChildren();
            m_Tiles.Clear();
            StartCoroutine(WaitAFrame());
        }
        IEnumerator WaitAFrame()
        {
            yield return 0;
        }

        private void InitializeTiles()
        {
            //set size of each cell as ratio of (single tile size) / (# tiles)
            m_Grid.cellSize = new Vector2(
                m_RectTransform.rect.width / (float)m_TileSize,
                m_RectTransform.rect.height / (float)m_TileSize
            );

            for (int y = 0; y < m_TileSize; y++)
            {
                for (int x = 0; x < m_TileSize; x++)
                {
                    GameObject go = Instantiate(m_TilePrefab);
                    Vector2Int index = new Vector2Int(x, y);
                    m_Tiles[index] = go.GetComponent<TileBehavior>();
                    go.transform.SetParent(transform);
                }
            }
        }



        public void Awake()
        {
            m_Grid = GetComponent<GridLayoutGroup>();
            Debug.Assert(m_Grid != null);
        }


    }
}