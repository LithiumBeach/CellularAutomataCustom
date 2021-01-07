//RuleBehavior.cs
//No data should be stored in this class besides scene references

using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace ca
{
    public class RuleBehavior : MonoBehaviour
    {
        #region scene references
        //"Rule {X}: concerning cells of color:"
        public TextMeshProUGUI txtRuleX;

        //color picker text labels: should be "ANY" when transparent
        public RawImage imgThisColor;
        public TextMeshProUGUI txtThisColor;

        public RawImage imgIfColor;
        public TextMeshProUGUI txtIfColor;

        public RawImage imgThenColor;
        public TextMeshProUGUI txtThenColor;

        public TextMeshProUGUI txtMinNeighbors;
        public TextMeshProUGUI txtMaxNeighbors;
        public static string c_RuleXString = "Rule {0}: concerning cells of color:";


        //TODO: color button refs to change their color on click

        #endregion

        #region event handlers
        //dir: left click = +1 : right click = -1
        public void OnThisColorChange(int direction)
        {
            int newColor = WindowManager.Instance.OnThisColorChange(this, direction);
            imgThisColor.color = CAColor.colors[newColor];
            txtThisColor.text = CAColor.GetColorText(newColor);
        }
        public void OnIfColorChange(int direction)
        {
            int newColor = WindowManager.Instance.OnIfColorChange(this, direction);
            imgIfColor.color = CAColor.colors[newColor];
            txtIfColor.text = CAColor.GetColorText(newColor);
        }
        public void OnThenColorChange(int direction)
        {
            int newColor = WindowManager.Instance.OnThenColorChange(this, direction);
            imgThenColor.color = CAColor.colors[newColor];
            txtThenColor.text = CAColor.GetColorText(newColor);
        }
        public void OnMinNeighborsChange(int direction)
        {
            int newMinNeighbors = WindowManager.Instance.OnMinNeighborsChange(this, direction);
            txtMinNeighbors.text = newMinNeighbors.ToString();
        }
        public void OnMaxNeighborsChange(int direction)
        {
            int newMaxNeighbors = WindowManager.Instance.OnMaxNeighborsChange(this, direction);
            txtMaxNeighbors.text = newMaxNeighbors.ToString();
        }

        public void UpdateAllUI(RuleData rd)
        {
            if (rd != null)
            {
                OnThisColorChange(0);
                OnIfColorChange(0);
                OnThenColorChange(0);
                OnMinNeighborsChange(0);
                OnMaxNeighborsChange(0); 
            }
        }


        public void OnDeleteRule()
        {
            WindowManager.Instance.DeleteRule(this);
        }
        #endregion
    }
}