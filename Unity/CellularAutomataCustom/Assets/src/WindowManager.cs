﻿using Sirenix.OdinInspector;
using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

namespace ca
{
    public class WindowManager : patterns.SingletonBehavior<WindowManager>
    {
        [Required]
        public CellGridBehavior m_CellGrid;
        [Required]
        public TextMeshProUGUI m_FPSText;
        [Required]
        public TextMeshProUGUI m_SimulateButtonText;
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

        [HideInInspector]
        public List<RulesetSO> m_Rulesets;
        public RulesetSO m_ActiveRuleset;

        //Left Mouse Button
        public Action<Vector2> OnLeftMouseDown;
        public Action<Vector2> WhileLeftMouseDown;
        public Action<Vector2> OnLeftMouseUp;


        private bool m_IsSimulating = false;
        private int m_FPSIndex = 0;

        public float FPS
        { get { return m_FPSOptions[m_FPSIndex]; } }
        public float OneDivFPS
        { get { return m_OneDivFPSOptions[m_FPSIndex]; } }

        private float m_FPSCount = 0.0f;

        private void Awake()
        {
            //unrestrict Unity framerate
            Application.targetFrameRate = -1;

            //cache 1/m_FPSOptions
            m_OneDivFPSOptions = new float[m_FPSOptions.Length];
            for (int i = 0; i < m_FPSOptions.Length; i++)
            { m_OneDivFPSOptions[i] = 1.0f / m_FPSOptions[i]; }

            //initialize UI
            ChangeFPS(0);
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

            if (m_IsSimulating)
            {
                m_FPSCount += Time.deltaTime;
                if (m_FPSCount > OneDivFPS)
                {
                    m_CellGrid.EvaluateNextState(m_ActiveRuleset.m_Rules);

                    while (m_FPSCount > OneDivFPS)
                    {
                        m_FPSCount -= OneDivFPS;
                    }
                }
            }
        }

        #region Events

        public void SimulateStep()
        {
            if (m_ActiveRuleset != null)
            {
                m_CellGrid.EvaluateNextState(m_ActiveRuleset.m_Rules);
            }
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
        }
    }
}