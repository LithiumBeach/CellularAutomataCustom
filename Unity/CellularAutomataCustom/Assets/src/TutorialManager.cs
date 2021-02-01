using Sirenix.OdinInspector;
using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace ca
{
    public class TutorialManager : patterns.SingletonBehavior<TutorialManager>, patterns.IStart
    {
        [PropertyTooltip("in-order prefabs to spawn at each tutorial stage.")]
        public List<GameObject> m_TutorialStagePrefabs;
        //private GameObject m_ActiveTutorialStage;

        //cache all Images, RawImages, and TextMeshProUGUIS
        private List<Image> m_NonTutorialImages;
        private List<bool> m_NonTutorialImagesWereRaycast;
        private List<RawImage> m_NonTutorialRawImages;
        private List<bool> m_NonTutorialRawImagesWereRaycast;
        private List<TextMeshProUGUI> m_NonTutorialTexts;
        private List<bool> m_NonTutorialTextsWereRaycast;

        //this should reset every time the game is run. only completing the tutorial will set it to not repeat on startup
        public int m_CurrentStage = 0;



        public const float MIN_ALPHA = 0.015f;

        public void IStart()
        {
            if (SaveLoadManager.Instance.ShouldShowTutorial)
            {
                StartCoroutine("DelayedTutorialStart");
            }
        }
        private System.Collections.IEnumerator DelayedTutorialStart()
        {
            //really make sure everything's been loaded by now
            yield return null;
            yield return null;

            PopulateAllNonTutorialReferences();
            BeginTutorial();

            StopCoroutine("DelayedTutorialStart");
        }

        // before spawning any tutorial objects, store references to all relevant objects for
        // fade in/ fade out, and raycast disabling
        private void PopulateAllNonTutorialReferences()
        {
            m_NonTutorialImages = GameObjectUtility.FindObjectsOfTypeAll<Image>();
            m_NonTutorialRawImages = GameObjectUtility.FindObjectsOfTypeAll<RawImage>();
            m_NonTutorialTexts = GameObjectUtility.FindObjectsOfTypeAll<TextMeshProUGUI>();

            //cache which ones were raycast target and which weren't
            m_NonTutorialImagesWereRaycast = new List<bool>(m_NonTutorialImages.Count);
            for (int i = 0; i < m_NonTutorialImages.Count; i++)
            {
                m_NonTutorialImagesWereRaycast.Add(m_NonTutorialImages[i].raycastTarget);
            }
            m_NonTutorialRawImagesWereRaycast = new List<bool>(m_NonTutorialRawImages.Count);
            for (int i = 0; i < m_NonTutorialRawImages.Count; i++)
            {
                m_NonTutorialRawImagesWereRaycast.Add(m_NonTutorialRawImages[i].raycastTarget);
            }
            m_NonTutorialTextsWereRaycast = new List<bool>(m_NonTutorialTexts.Count);
            for (int i = 0; i < m_NonTutorialTexts.Count; i++)
            {
                m_NonTutorialTextsWereRaycast.Add(m_NonTutorialTexts[i].raycastTarget);
            }
        }

        public void BeginTutorial()
        {
            //turn off all raycast detectors
            UpdateTutorialRaycastTarget(false);

            //instance first tutorial stage
            Instantiate(m_TutorialStagePrefabs[0], gameObject.transform);
        }

        public void AdvanceTutorialStage()
        {
            StartCoroutine("AdvanceTutorialStageAfterFrame");
        }

        private System.Collections.IEnumerator AdvanceTutorialStageAfterFrame()
        {
            yield return null;

            m_CurrentStage += 1;

            if (m_CurrentStage < m_TutorialStagePrefabs.Count)
            {
                //instance current tutorial stage
                Instantiate(m_TutorialStagePrefabs[m_CurrentStage], gameObject.transform);
            }
            else
            {
                //end tutorial
                EndTutorial();
            }
            StopCoroutine("AdvanceTutorialStageAfterFrame");
        }

        public void EndTutorial()
        {
            //turn everything back on
            UpdateTutorialRaycastTarget(true);

            //tutorial complete, do not show again
            SaveLoadManager.Instance.ShouldShowTutorial = false;

            //show photosensitivity warning ALWAYS after tutorial
            SaveLoadManager.Instance.ShouldShowPhotosensitivityWarning = true;
            WindowManager.Instance.DisplayPhotosensitivityWarning(SaveLoadManager.Instance.ShouldShowPhotosensitivityWarning);
        }

        //b_ToDefault: if true, revert to original setting, if false, turn off
        private void UpdateTutorialRaycastTarget(bool b_ToDefault)
        {
            for (int i = 0; i < m_NonTutorialImages.Count; i++)
            {
                //only set to true if both are true
                m_NonTutorialImages[i].raycastTarget = m_NonTutorialImagesWereRaycast[i] && b_ToDefault;
            }
            for (int i = 0; i < m_NonTutorialRawImages.Count; i++)
            {
                //only set to true if both are true
                m_NonTutorialRawImages[i].raycastTarget = m_NonTutorialRawImagesWereRaycast[i] && b_ToDefault;
            }
            for (int i = 0; i < m_NonTutorialTexts.Count; i++)
            {
                //only set to true if both are true
                m_NonTutorialTexts[i].raycastTarget = m_NonTutorialTextsWereRaycast[i] && b_ToDefault;
            }
        }

        //set alpha values of all images, rawimages, and textmeshprouguis
        public void SetNonTutorialAlpha(float a)
        {
            for (int i = 0; i < m_NonTutorialImages.Count; i++)
            {
                m_NonTutorialImages[i].color = new Color(
                    m_NonTutorialImages[i].color.r, m_NonTutorialImages[i].color.g, m_NonTutorialImages[i].color.b, a
                );
            }
            for (int i = 0; i < m_NonTutorialRawImages.Count; i++)
            {
                m_NonTutorialRawImages[i].color = new Color(
                    m_NonTutorialRawImages[i].color.r, m_NonTutorialRawImages[i].color.g, m_NonTutorialRawImages[i].color.b, a
                );
            }

            for (int i = 0; i < m_NonTutorialTexts.Count; i++)
            {
                m_NonTutorialTexts[i].color = new Color(
                    m_NonTutorialTexts[i].color.r, m_NonTutorialTexts[i].color.g, m_NonTutorialTexts[i].color.b, a
                );
            }
        }
    }
}