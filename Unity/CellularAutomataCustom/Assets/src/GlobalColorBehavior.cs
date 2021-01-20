﻿using Sirenix.OdinInspector;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace ca
{
    public class GlobalColorBehavior : patterns.SingletonBehavior<GlobalColorBehavior>
    {
        [Required]
        public GameObject m_ColorSquarePrefab;
        [Required]
        public Transform m_ColorSquareParent;


        private EState m_State = EState.DEFAULT;
        private enum EState
        {
            DEFAULT,
            DELETE_NEXT_CLICKED_COLOR,
            CHANGE_NEXT_CLICKED_COLOR,
            CHANGE_TO_NEXT_CLICKED_PIXEL_COLOR
        }

        private int m_ChangeNextClickedColorSiblingIndex;

        //called from UI buttons
        public void OnLeftMouseClickColorSquare(CAGlobalColorSelectorButton colorSquareRef)
        {
            Debug.Log("left clicked child #" + colorSquareRef.transform.parent.GetSiblingIndex().ToString());

            HandleAnyMouseClickColorSquare(colorSquareRef);
        }
        public void OnRightMouseClickColorSquare(CAGlobalColorSelectorButton colorSquareRef)
        {
            Debug.Log("right clicked child #" + colorSquareRef.transform.parent.GetSiblingIndex().ToString());

            HandleAnyMouseClickColorSquare(colorSquareRef);
        }

        private void HandleAnyMouseClickColorSquare(CAGlobalColorSelectorButton colorSquareRef)
        {
            switch (m_State)
            {
                case EState.DEFAULT:
                m_ChangeNextClickedColorSiblingIndex = colorSquareRef.transform.parent.GetSiblingIndex();
                m_State = EState.CHANGE_TO_NEXT_CLICKED_PIXEL_COLOR;
                break;
                case EState.DELETE_NEXT_CLICKED_COLOR:
                DeleteColorState(colorSquareRef.transform.parent.GetSiblingIndex());
                break;
                case EState.CHANGE_NEXT_CLICKED_COLOR:
                m_ChangeNextClickedColorSiblingIndex = colorSquareRef.transform.parent.GetSiblingIndex();
                m_State = EState.CHANGE_TO_NEXT_CLICKED_PIXEL_COLOR;
                break;
                case EState.CHANGE_TO_NEXT_CLICKED_PIXEL_COLOR:
                break;
                default:
                break;
            }
        }

        //this should be one of the only unity Update methods in the code.
        //it is needed only to listen for clicks at any position on the screen.
        private void Update()
        {
            if ((Input.GetMouseButtonDown(0) || Input.GetMouseButtonDown(1)) &&
                m_State == EState.CHANGE_TO_NEXT_CLICKED_PIXEL_COLOR)
            {
                //create new rendertexture
                Camera.main.targetTexture = new RenderTexture(Screen.width, Screen.height, 24);
                RenderTexture.active = Camera.main.targetTexture;

                StartCoroutine("ChangeToNextClickedPixelColor");

                m_State = EState.DEFAULT;
            }
        }
        private System.Collections.IEnumerator ChangeToNextClickedPixelColor()
        {
            yield return new WaitForEndOfFrame();

            //take screenshot to texture2D
            Texture2D capture = new Texture2D(Screen.width, Screen.height, TextureFormat.RGB24, false);
            capture.ReadPixels(new Rect(0, 0, Screen.width, Screen.height), 0, 0);
            capture.Apply();

            //get pixel color at mouse position
            Color pixelColor = capture.GetPixel(
                Mathf.FloorToInt(Input.mousePosition.x),
                Mathf.FloorToInt(Input.mousePosition.y)
            );

            //change color of this state
            ChangeColorState(m_ChangeNextClickedColorSiblingIndex, pixelColor);

            //cleanup
            Destroy(Camera.main.targetTexture);
            Camera.main.targetTexture = null;
            RenderTexture.active = null;

            StopCoroutine("ChangeToNextClickedPixelColor");
        }

        public void OnAddNewColorStatePressed()
        {
            //TODO: prompt color selector, use selected color instead of Color.white
            SaveLoadManager.Instance.AddColor(Color.white);
            AddNewColorState(Color.white);
            m_State = EState.DEFAULT;
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
            if (m_State == EState.DELETE_NEXT_CLICKED_COLOR)
            {
                m_State = EState.DEFAULT;
            }
            else
            {
                m_State = EState.DELETE_NEXT_CLICKED_COLOR;
            }
        }
        private void DeleteColorState(int childIndex)
        {
            //destroy UI gameobject
            Destroy(m_ColorSquareParent.GetChild(childIndex).gameObject);

            //add 1 to child index so we ignore transparent
            SaveLoadManager.Instance.RemoveColor(childIndex + 1);

            m_State = EState.DEFAULT;
        }

        public void OnChangeColorStatePressed()
        {
            m_State = EState.CHANGE_NEXT_CLICKED_COLOR;
        }
        private void ChangeColorState(int childIndex, Color c)
        {
            SaveLoadManager.Instance.ChangeColor(childIndex + 1, c);

            //get button reference from children (there should only be one)
            CAGlobalColorSelectorButton thisBtn = m_ColorSquareParent.GetChild(childIndex).GetComponentInChildren<CAGlobalColorSelectorButton>();

            //update background color
            thisBtn.m_Background.color = c;
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

            //additionally, any time we reset (ie when we open the canvas), cancel any user states
            m_State = EState.DEFAULT;
        }
    }
}