using Sirenix.OdinInspector;
using System;
using System.Collections.Generic;
using UnityEngine;

namespace ca
{
    public class WindowManager : patterns.SingletonBehavior<WindowManager>
    {
        [Required]
        public CellGridBehavior m_CellGrid;

        [HideInInspector]
        public List<RulesetSO> m_Rulesets;
        public RulesetSO m_ActiveRuleset;

        //Left Mouse Button
        public Action<Vector2> OnLeftMouseDown;
        public Action<Vector2> WhileLeftMouseDown;
        public Action<Vector2> OnLeftMouseUp;

        private void Awake()
        {
            
        }

        private void Update()
        {
            #region Event System: Invoke events
            Vector2 mousePos2D = Input.mousePosition;
            if (Input.GetKeyDown(KeyCode.Mouse0))
            {
                OnLeftMouseDown?.Invoke(mousePos2D);
            }
            if (Input.GetKey(KeyCode.Mouse0))
            {
                WhileLeftMouseDown?.Invoke(mousePos2D);
            }
            if (Input.GetKeyUp(KeyCode.Mouse0))
            {
                OnLeftMouseUp?.Invoke(mousePos2D);
            }
            #endregion

            //have the board evaluate the active Ruleset
            if (Input.GetKeyDown(KeyCode.Space))//TODO: temp space to simulate
            {
                if (m_ActiveRuleset != null)
                {
                    m_CellGrid.EvaluateNextState(m_ActiveRuleset.m_Rules);
                }
            }
        }

        //cleanup
        private void OnDestroy()
        {
            OnLeftMouseDown = null;
            WhileLeftMouseDown = null;
            OnLeftMouseUp = null;
        }
    }
}