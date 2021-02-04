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
        private float t = 0f;

        //used only if no button to press, not required
        public TextMeshProUGUI m_ClickToContinueText = null;
        public float m_ClickToContinueTime = 4f;
        private float readT = 0f;

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
            if (readT >= m_ClickToContinueTime)
            {
                if (m_TSC.CanAdvanceStage(this))
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
            t = readT = 0f;
            m_InfoText.alpha = a;
            if (m_ClickToContinueText != null)
            {
                m_ClickToContinueText.alpha = a;
            }
            TutorialManager.Instance.UpdateFocusObjects(a, true);
        }
        private void BeginFadeOut()
        {
            m_State = EState.FADING_OUT;
            t = readT = 0f;
            m_InfoText.alpha = 1f;
            if (m_ClickToContinueText != null)
            {
                m_ClickToContinueText.alpha = 1f;
            }
            TutorialManager.Instance.UpdateFocusObjects(1f, false);
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
                    t = readT = 0f;
                }
                else
                {
                    //fade out everything else
                    TutorialManager.Instance.SetNonTutorialAlpha(
                        Mathf.Lerp(1f, TutorialManager.MIN_ALPHA, CAMath.SmoothStep(t / m_FadeTime))
                    );

                    //fade in focus objects
                    float a = Mathf.Lerp(TutorialManager.MIN_ALPHA, 1f, CAMath.SmoothStep(t / m_FadeTime));
                    m_InfoText.alpha = a;
                    TutorialManager.Instance.UpdateFocusObjects(a, true);
                }
                break;
                case EState.WAITING:
                readT += Time.deltaTime;
                if (readT > (m_ClickToContinueTime) && readT < (m_ClickToContinueTime + m_FadeTime)) //if timer still counting
                {
                    //fade in click anywhere to continue text
                    if (m_ClickToContinueText != null)
                    {
                        m_ClickToContinueText.alpha = Mathf.Lerp(0f, 1f, Mathf.Min(1.0f, CAMath.SmoothStep((readT - m_ClickToContinueTime) / m_FadeTime)));
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
                    t = readT = 0f;
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
                    TutorialManager.Instance.UpdateFocusObjects(a, false);
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