//WindowManager.cs
//Keep this as lean as possible: manages all buttons, rulesets, and a cell grid
//Buttons trigger functions here
//Manages ONLY displayed portion of rulesets (current ruleset). Call into SaveLoadManager.

using Sirenix.OdinInspector;
using Sirenix.Serialization;
using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

namespace ca
{
    public class WindowManager : patterns.SingletonBehavior<WindowManager>, patterns.IStart
    {
        [Required]
        public CellGridBehavior m_CellGrid;
        [Required]
        public TextMeshProUGUI m_FPSText;
        [Required]
        public TextMeshProUGUI m_SimulateButtonText;
        [Required]
        public TextMeshProUGUI m_ZoomButtonText;

        [Required]
        public Transform m_RulesetParent;
        [Required]
        public RuleBehavior m_RuleBehaviorPrefab;

        private readonly float[] m_FPSOptions = new float[]
        {
            1f,
            2f,
            4f,
            8f,
            10f,
            12f,
            16f,
            24f,
            32f,
            48f,
            64f,
            96f,//TODO: something is wrong after about this rate.
            128f,
            256f,
        };
        private float[] m_OneDivFPSOptions;

        //Left Mouse Button
        public Action<Vector2> OnLeftMouseDown;
        public Action<Vector2> WhileLeftMouseDown;
        public Action<Vector2> OnLeftMouseUp;
        //Right Mouse Button
        public Action<Vector2> OnRightMouseDown;
        public Action<Vector2> WhileRightMouseDown;
        public Action<Vector2> OnRightMouseUp;


        private bool m_IsSimulating = false;

        private int m_FPSIndex = 0;
        public float FPS
        { get { return m_FPSOptions[m_FPSIndex]; } }
        public float OneDivFPS
        { get { return m_OneDivFPSOptions[m_FPSIndex]; } }

        private float m_FPSCount = 0.0f;


        public void IStart()
        {
            //unrestrict Unity framerate
            Application.targetFrameRate = -1;

            //cache 1/m_FPSOptions
            m_OneDivFPSOptions = new float[m_FPSOptions.Length];
            for (int i = 0; i < m_FPSOptions.Length; i++)
            { m_OneDivFPSOptions[i] = 1.0f / m_FPSOptions[i]; }

            //initialize UI
            ChangeFPS(0);
            SetZoom(0);

            LoadCurrentRulesetUI();
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
            if (Input.GetKeyDown(KeyCode.Mouse1))
            {
                OnRightMouseDown?.Invoke(mousePos2D);
            }
            if (Input.GetKey(KeyCode.Mouse1))
            {
                WhileRightMouseDown?.Invoke(mousePos2D);
            }
            if (Input.GetKeyUp(KeyCode.Mouse1))
            {
                OnRightMouseUp?.Invoke(mousePos2D);
            }
            #endregion

            if (m_IsSimulating)
            {
                m_FPSCount += Time.deltaTime;
                if (m_FPSCount > OneDivFPS)
                {
                    SimulateStep();

                    while (m_FPSCount > OneDivFPS)
                    {
                        m_FPSCount -= OneDivFPS;
                    }
                }
            }
        }

        #region Rulesets

        private void LoadCurrentRulesetUI()
        {
            //destroy any leftover children from the previous ruleset
            foreach (Transform t in m_RulesetParent)
            {
                Destroy(t.gameObject);
            }

            //instantiate all rules
            foreach (RuleData rd in SaveLoadManager.Instance.CurrentRuleset.m_Rules)
            {
                //do not add data, only add rule ui's.
                AddRuleUI(rd);
            }
        }

        public void OnAddRulesetButtonPressed()
        {
            //update Data
            SaveLoadManager.Instance.AddNewRuleset();
            SaveLoadManager.Instance.SetCurrentRuleset(SaveLoadManager.Instance.NumRulesets - 1);

            //update UI
            LoadCurrentRulesetUI();
        }

        public void AddRule() { AddRule(new RuleData()); }
        private void AddRule(RuleData rd)
        {
            //update data
            SaveLoadManager.Instance.AddNewRule(rd);

            AddRuleUI(rd);
        }

        private void AddRuleUI(RuleData rd)
        {
            //create new rule UI
            RuleBehavior rb = Instantiate(m_RuleBehaviorPrefab);
            rb.transform.SetParent(m_RulesetParent);

            //Initialize all ui elements in from ruledata
            rb.UpdateAllUI(rd);
        }

        public void DeleteRule(RuleBehavior ruleBehavior)
        {
            throw new NotImplementedException();
        }

        #endregion

        #region Ruleset Button Press Callbacks
        //returns new color index
        public int OnThisColorChange(RuleBehavior ruleBehavior, int direction)
        {
            //GetSiblingIndex
            int rbIndex = ruleBehavior.transform.GetSiblingIndex();
            SaveLoadManager.Instance.SetThisColor(rbIndex, CAMath.Mod(rbIndex + direction, CAColor.colors.Length));
            return SaveLoadManager.Instance.CurrentRuleset.m_Rules[rbIndex].m_ThisColor;
        }
        public int OnIfColorChange(RuleBehavior ruleBehavior, int direction)
        {
            int rbIndex = ruleBehavior.transform.GetSiblingIndex();
            SaveLoadManager.Instance.SetIfColor(rbIndex, CAMath.Mod(rbIndex + direction, CAColor.colors.Length));
            return SaveLoadManager.Instance.CurrentRuleset.m_Rules[rbIndex].m_IfColor;
        }
        public int OnThenColorChange(RuleBehavior ruleBehavior, int direction)
        {
            int rbIndex = ruleBehavior.transform.GetSiblingIndex();
            SaveLoadManager.Instance.SetThenColor(rbIndex, CAMath.Mod(rbIndex + direction, CAColor.colors.Length));
            return SaveLoadManager.Instance.CurrentRuleset.m_Rules[rbIndex].m_ThenColor;
        }
        public int OnMinNeighborsChange(RuleBehavior ruleBehavior, int direction)
        {
            int rbIndex = ruleBehavior.transform.GetSiblingIndex();
            SaveLoadManager.Instance.SetMinNeighbors(rbIndex, CAMath.Mod(rbIndex + direction, CAColor.colors.Length));
            return SaveLoadManager.Instance.CurrentRuleset.m_Rules[rbIndex].m_MinNumNeighbors;
        }
        public int OnMaxNeighborsChange(RuleBehavior ruleBehavior, int direction)
        {
            int rbIndex = ruleBehavior.transform.GetSiblingIndex();
            SaveLoadManager.Instance.SetMaxNeighbors(rbIndex, CAMath.Mod(rbIndex + direction, CAColor.colors.Length));
            return SaveLoadManager.Instance.CurrentRuleset.m_Rules[rbIndex].m_MaxNumNeighbors;
        }
        #endregion

        #region Zoom


        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Zoom In")]
        public void ZoomIn() { SetZoom(m_CellGrid.Zoom - 1); }
        [Sirenix.OdinInspector.Button(Sirenix.OdinInspector.ButtonSizes.Medium, Name = "Zoom Out")]
        public void ZoomOut() { SetZoom(m_CellGrid.Zoom + 1); }

        public void SetZoom(int zoomLevel)
        {
            m_CellGrid.SetZoom(zoomLevel);
            m_ZoomButtonText.text = m_CellGrid.Zoom.ToString();
        }

        #endregion

        #region Simulating

        public void SimulateStep()
        {
            m_CellGrid.EvaluateNextState(SaveLoadManager.Instance.CurrentRuleset.m_Rules);
        }

        //-1 = slower, +1 = faster, 0 = no change
        public void ChangeFPS(int direction)
        {
            //update fps index
            m_FPSIndex += direction;
            m_FPSIndex = CAMath.Mod(m_FPSIndex, m_FPSOptions.Length);

            //update ui
            m_FPSText.text = FPS.ToString();
        }

        public void ToggleSimulating()
        {
            m_IsSimulating = !m_IsSimulating;
            m_SimulateButtonText.text = m_IsSimulating ? "STOP" : "SIMULATE";
        }

        #endregion

        //cleanup
        private void OnDestroy()
        {
            OnLeftMouseDown = null;
            WhileLeftMouseDown = null;
            OnLeftMouseUp = null;
            OnRightMouseDown = null;
            WhileRightMouseDown = null;
            OnRightMouseUp = null;
        }
    }
}