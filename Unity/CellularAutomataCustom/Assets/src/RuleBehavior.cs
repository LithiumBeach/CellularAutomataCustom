using System.Collections.Generic;
using TMPro;
using UnityEngine;

namespace ca
{
    public class RuleBehavior : MonoBehaviour
    {
        public RuleData m_Data;

        #region scene references
        public TextMeshProUGUI txtThisColor;
        public TextMeshProUGUI txtIfColor; // should be "ANY" when transparent
        public TextMeshProUGUI txtThenColor; // should be "ANY" when transparent

        #endregion

        #region event handlers
        //dir: left click = +1 : right click = -1
        public void OnThisColorChange(int direction)
        {
            //TODO:
            //txtConcerningColorText
        }
        public void OnDeleteRule()
        {
            //TODO:
        }
        public void OnMinNeighborsChange(int direction)
        {
            //TODO:
        }
        public void OnMaxNeighborsChange(int direction)
        {
            //TODO:
        }
        public void OnIfColorChange(int direction)
        {
            //TODO:
            //txtIfColor
        }
        public void OnThenColorChange(int direction)
        {
            //TODO:
            //txtThenColor
        }
        #endregion
    }
}