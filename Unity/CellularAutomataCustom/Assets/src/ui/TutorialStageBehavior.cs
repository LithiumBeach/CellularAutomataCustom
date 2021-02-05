using Sirenix.OdinInspector;
using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

namespace ca
{
    //attached to each tutorial cursor prefab: self-manages each tutorial stage
    public class TutorialStageBehavior : MonoBehaviour
    {
        public int m_TutorialNum=0;
        public bool b_FadeIn = false;
        public bool b_FadeOut = false;
        public bool b_IsCellGridInputAllowed = false;
        public bool b_CanAdvanceWithoutEventTrigger = false;
        public bool b_ShuffleBoardIfClear = false;

        //informative text to be faded into the scene
        [Required]
        public TextMeshProUGUI m_InfoText = null;
        private const float m_FadeTime = 1.2f;
        private float t = 0f; //general-use timer

        //used only if no button to press, not required
        public TextMeshProUGUI m_ClickToContinueText = null;
        private const float c_ClickToContinueAlphaMax = 0.4f;
        public float m_ClickToContinueTime = 4f;
        private const float MAX_ALLOWED_TIME_IN_ANY_STAGE = 180f;//something has gone wrong.

        #region Tutorial State Checking
        //based on the level, call a function
        private TutorialStateCheck m_TSC;
        private void InitTutorialStateCheck()
        {
            switch (m_TutorialNum)
            {
                case 1:
                m_TSC = new TutorialSC1();
                break;
                case 2:
                m_TSC = new TutorialSC2();
                break;
                case 3:
                m_TSC = new TutorialSC3();
                break;
                case 4:
                m_TSC = new TutorialSC4();
                break;
                case 5:
                m_TSC = new TutorialSC5();
                break;
                case 6:
                m_TSC = new TutorialSC6();
                break;
                case 7:
                m_TSC = new TutorialSC7();
                break;
                case 8:
                m_TSC = new TutorialSC8();
                break;
                case 10:
                m_TSC = new TutorialSC10();
                break;
                //no special checks
                default:
                m_TSC = new TutorialStateCheck();
                break;
            }
        }
        #endregion


        private enum EState
        {
            FADING_IN,
            WAITING,
            FADING_OUT
        }
        private EState m_State;



        public void AdvanceTutorialStage()
        {
            if (t >= m_ClickToContinueTime)
            {
                //this MAX_TIME case is for any failure of communication in the tutorial. worst case.
                if (m_TSC.CanAdvanceStage(this) || t >= MAX_ALLOWED_TIME_IN_ANY_STAGE)
                {
                    //if the board is clear and it should not be clear now
                    if (b_ShuffleBoardIfClear &&
                        WindowManager.Instance.m_CellGrid.m_CellGrid.IsBoardOneColor())
                    {
                        //shuffle board
                        WindowManager.Instance.m_CellGrid.ClearBoard(0);
                    }

                    BeginFadeOut();
                }
            }
        }

        private void Start()
        {
            InitTutorialStateCheck();

            if (b_FadeIn)
            {
                m_State = EState.FADING_IN;
                BeginFadeIn(0f);
            }
            else
            {
                m_State = EState.WAITING;
                BeginFadeIn(1f);
            }

            //allow cell grid input?
            WindowManager.Instance.m_CellGrid.b_IsInputActive = b_IsCellGridInputAllowed;
        }

        private void BeginFadeIn(float a)
        {
            t = 0f;
            m_InfoText.alpha = a;
            if (m_ClickToContinueText != null)
            {
                m_ClickToContinueText.alpha = a * c_ClickToContinueAlphaMax;
            }
            TutorialManager.Instance.UpdateFocusObjects(a, true);
        }
        private void BeginFadeOut()
        {
            m_State = EState.FADING_OUT;
            t = 0f;
            m_InfoText.alpha = 1f;
            if (m_ClickToContinueText != null)
            {
                m_ClickToContinueText.alpha = c_ClickToContinueAlphaMax;
            }
            TutorialManager.Instance.UpdateFocusObjects(1f, true);
        }

        private void Update()
        {
            switch (m_State)
            {
                case EState.FADING_IN:
                t += Time.deltaTime;
                if (t >= m_FadeTime)
                {
                    m_State = EState.WAITING;
                    t = 0f;
                    TutorialManager.Instance.UpdateFocusObjects(1f, true);
                }
                else
                {
                    //fade in focus objects
                    float a = Mathf.Lerp(TutorialManager.MIN_ALPHA, 1f, CAMath.SmoothStep(t / m_FadeTime));
                    m_InfoText.alpha = a;
                    TutorialManager.Instance.UpdateFocusObjects(a, true);
                }
                break;
                case EState.WAITING:
                t += Time.deltaTime;
                if (t > (m_ClickToContinueTime) && t < (m_ClickToContinueTime + m_FadeTime)) //if timer still counting
                {
                    //fade in click anywhere to continue text
                    if (m_ClickToContinueText != null)
                    {
                        m_ClickToContinueText.alpha = Mathf.Lerp(0f, c_ClickToContinueAlphaMax,
                            Mathf.Min(1.0f, CAMath.SmoothStep((t - m_ClickToContinueTime) / m_FadeTime)));
                    }
                }
                else if (b_CanAdvanceWithoutEventTrigger)
                {
                    AdvanceTutorialStage();
                }
                break;
                case EState.FADING_OUT:
                t += Time.deltaTime;
                if (b_FadeOut || t >= m_FadeTime)
                {
                    t = 0f;
                    OnFadeOut();
                }
                else
                {
                    //fade out focus objects
                    if (m_ClickToContinueText != null)
                    {
                        m_ClickToContinueText.alpha = 0f;
                    }
                    float a = Mathf.Lerp(1f, 0f, Mathf.Min(1.0f, CAMath.SmoothStep(t / m_FadeTime)));
                    m_InfoText.alpha = a;
                }
                break;
                default:
                break;
            }
        }

        //<button ID , press count>
        public Dictionary<int, int> m_ButtonPressCounts;
        public void HandleButtonPress(int buttonNumber)
        {
            if(m_ButtonPressCounts == null) { m_ButtonPressCounts = new Dictionary<int, int>(); }
            if (!m_ButtonPressCounts.ContainsKey(buttonNumber))
            {
                m_ButtonPressCounts[buttonNumber] = 0;
            }
            m_ButtonPressCounts[buttonNumber] += 1;
        }

        private void OnFadeOut()
        {
            //allow cell grid input ? always at the end, even if it gets set back immediately
            WindowManager.Instance.m_CellGrid.b_IsInputActive = true;

            TutorialManager.Instance.AdvanceTutorialStage();

            Destroy(this.gameObject); //should destroy m_TSC, etc
        }
    }
}