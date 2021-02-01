using Sirenix.OdinInspector;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

namespace ca
{
    //attached to each tutorial cursor prefab: self-manages each tutorial stage
    public class TutorialStageBehavior : MonoBehaviour
    {
        //informative text to be faded into the scene
        [Required]
        public TextMeshProUGUI m_InfoText=null;
        public float m_FadeTime = .6f;
        private float t = 0f;

        //used only if no button to press, not required
        public TextMeshProUGUI m_ClickToContinueText=null;
        public float m_ClickToContinueTime = 5f;
        private float readT = 0f;


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
                BeginFadeOut(); 
            }
        }

        private void Start()
        {
            BeginFadeIn();
        }

        private void BeginFadeIn()
        {
            m_State = EState.FADING_IN;
            t = readT = 0f;
            m_InfoText.alpha = m_ClickToContinueText.alpha = 0;
        }
        private void BeginFadeOut()
        {
            m_State = EState.FADING_OUT;
            t = readT = 0f;
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
                    m_InfoText.alpha = Mathf.Lerp(TutorialManager.MIN_ALPHA, 1f, CAMath.SmoothStep(t / m_FadeTime));

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
                    m_ClickToContinueText.alpha = Mathf.Lerp(0f, 1f, Mathf.Min(1.0f, CAMath.SmoothStep((readT - m_ClickToContinueTime) / m_FadeTime)));
                }
                break;
                case EState.FADING_OUT:
                t += Time.deltaTime;
                if (t >= m_FadeTime)
                {
                    t = readT = 0f;
                    OnFadeOut();
                }
                else
                {
                    //fade out focus objects
                    m_ClickToContinueText.alpha = Mathf.Lerp(1f, 0f, Mathf.Min(1.0f, CAMath.SmoothStep(t / m_FadeTime)));
                    m_InfoText.alpha = Mathf.Lerp(1f, 0f, Mathf.Min(1.0f, CAMath.SmoothStep(t / m_FadeTime)));

                    //fade in everything else
                    //TutorialManager.Instance.SetNonTutorialAlpha(
                    //    Mathf.Lerp(1f, TutorialManager.MIN_ALPHA, CAMath.SmoothStep(t / m_FadeTime))
                    //);
                }
                break;
                default:
                break;
            }
        }

        private void OnFadeOut()
        {
            TutorialManager.Instance.AdvanceTutorialStage();

            Destroy(this.gameObject);
        }
    }
}