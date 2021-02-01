using Sirenix.OdinInspector;
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

        //informative text to be faded into the scene
        [Required]
        public TextMeshProUGUI m_InfoText = null;
        public float m_FadeTime = .6f;
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
                case 0:
                m_TSC = new TutorialStateCheck();
                break;
                case 1:
                m_TSC = new TutorialSC1();
                break;
                case 2:
                m_TSC = new TutorialSC2();
                break;
                default:
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
                if (m_TSC.CanAdvanceStage())
                {
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
            TutorialManager.Instance.UpdateFocusObjectAlphas(a);
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
            TutorialManager.Instance.UpdateFocusObjectAlphas(1f);
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
                    //fade in m_InfoText
                    float a = Mathf.Lerp(TutorialManager.MIN_ALPHA, 1f, CAMath.SmoothStep(t / m_FadeTime));
                    m_InfoText.alpha = a;
                    TutorialManager.Instance.UpdateFocusObjectAlphas(a);

                    //fade out everything else
                    TutorialManager.Instance.SetNonTutorialAlpha(
                        Mathf.Lerp(1f, TutorialManager.MIN_ALPHA, CAMath.SmoothStep(t / m_FadeTime))
                    );
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
                    TutorialManager.Instance.UpdateFocusObjectAlphas(a);
                }
                break;
                default:
                break;
            }
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