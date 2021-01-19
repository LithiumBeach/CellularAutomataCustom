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

        private bool b_DeleteNextClickedColor=false;

        //called from UI buttons
        public void OnLeftMouseClick(CAGlobalColorSelectorButton colorSquareRef)
        {
            Debug.Log("left clicked child #" + colorSquareRef.transform.parent.GetSiblingIndex().ToString());

            if (b_DeleteNextClickedColor)
            {
                DeleteColorState(colorSquareRef.transform.parent.GetSiblingIndex());
            }
        }
        public void OnRightMouseClick(CAGlobalColorSelectorButton colorSquareRef)
        {
            Debug.Log("right clicked child #" + colorSquareRef.transform.parent.GetSiblingIndex().ToString());

            if (b_DeleteNextClickedColor)
            {
                DeleteColorState(colorSquareRef.transform.parent.GetSiblingIndex());
            }
        }

        public void OnAddNewColorStatePressed()
        {
            //TODO: prompt color selector, use selected color instead of Color.white
            SaveLoadManager.Instance.AddColor(Color.white);
            AddNewColorState(Color.white);
        }

        private void AddNewColorState(Color c)
        {
            //instantiate as child of color square parent
            GameObject newGO = Instantiate(m_ColorSquarePrefab, m_ColorSquareParent);

            //get button reference from children (there should only be one)
            CAGlobalColorSelectorButton newBtn = newGO.transform.GetComponentInChildren<CAGlobalColorSelectorButton>();

            //update background color
            newBtn.m_Background.color = c;
        }

        //if we press once, we are in delete mode
        //if we press again before pressing any colors, we cancel the delete
        public void OnDeleteColorStatePressed()
        {
            b_DeleteNextClickedColor = !b_DeleteNextClickedColor;
        }
        private void DeleteColorState(int childIndex)
        {
            //destroy UI gameobject
            Destroy(m_ColorSquareParent.GetChild(childIndex).gameObject);

            //add 1 to child index so we ignore transparent
            SaveLoadManager.Instance.RemoveColor(childIndex + 1);

            b_DeleteNextClickedColor = false;
        }
        private void ChangeColorState(RawImage colorSquareRef, int dir)
        {

        }

        public void OnResetAllColorsToDefaultPressed()
        {
            //reset data to default
            SaveLoadManager.Instance.Colors = CAColor.defaultColors;

            //reset UI to default
            ResetTo(CAColor.defaultColors);
        }
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
                AddNewColorState(newColors[i]);
            }

            //additionally, any time we reset (ie when we open the canvas), dont delete the next thing
            b_DeleteNextClickedColor = false;
        }
    }
}