using System.Collections.Generic;
using TMPro;
using UnityEngine;

namespace ca
{
    public class RuleBehavior : MonoBehaviour
    {
        public RuleData m_Data;

        #region scene references
        //"Rule {X}: concerning cells of color:"
        public TextMeshProUGUI txtRuleX;
        //color picker text labels: should be "ANY" when transparent
        public TextMeshProUGUI txtThisColor;
        public TextMeshProUGUI txtIfColor;
        public TextMeshProUGUI txtThenColor;

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