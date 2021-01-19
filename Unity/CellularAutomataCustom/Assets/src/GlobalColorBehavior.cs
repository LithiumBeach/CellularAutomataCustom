using Sirenix.OdinInspector;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ca
{
    public class GlobalColorBehavior : patterns.SingletonBehavior<GlobalColorBehavior>
    {
        [Required]
        public GameObject m_ColorSquarePrefab;
        [Required]
        public Transform m_ColorSquareParent;

        //called from UI buttons
        public void OnLeftMouseClick(CAGlobalColorSelectorButton colorSquareRef)
        {
            Debug.Log("left clicked child #" + colorSquareRef.transform.parent.GetSiblingIndex().ToString());
        }
        public void OnRightMouseClick(CAGlobalColorSelectorButton colorSquareRef)
        {
            Debug.Log("right clicked child #" + colorSquareRef.transform.parent.GetSiblingIndex().ToString());
        }
        public void AddNewColorState()
        {
            Debug.Log("add new colorstate");
        }

        //called indirectly after click events
        private void ChangeColorState(RawImage colorSquareRef, int dir)
        {

        }
        private void DeleteColorState(RawImage colorSquareRef)
        {

        }

        //
        public void ResetTo(List<Color> newColors)
        {
            //the first color must always be transparent
            Debug.Assert(newColors[0] == Color.clear);

            //cleanup any existing squares before adding
            if (m_ColorSquareParent.childCount > 0)
            {
                foreach (Transform t in m_ColorSquareParent)
                {
                    Destroy(t.gameObject);
                }
            }

            //add 1 square per color
            for (int i = 1; i < newColors.Count; i++)
            {
                //instantiate as child of color square parent
                GameObject newGO = Instantiate(m_ColorSquarePrefab, m_ColorSquareParent);

                //get button reference from children (there should only be one)
                CAGlobalColorSelectorButton newBtn = newGO.transform.GetComponentInChildren<CAGlobalColorSelectorButton>();

                //update background color
                newBtn.m_Background.color = newColors[i];
            }
        }
    }
}