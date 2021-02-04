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
        [Serializable]
        public class TutorialStageReferences
        {
            public GameObject prefab;
            public List<GameObject> m_SceneFocusObjects;
            public List<GameObject> m_SceneRaycastableObjects;
            //TODO: m_SceneFocusObjectParents; //set alpha of self and all children OR just do this for all scenefocusobjects!
        }
        private List<GameObject> m_AllSceneFocusObjectsSoFar;
        private List<GameObject> m_AllSceneRaycastableObjectsSoFar;

        [PropertyTooltip("in-order prefabs to spawn at each tutorial stage.")]
        public List<TutorialStageReferences> m_TutorialStages;

        //cache all Images, RawImages, and TextMeshProUGUIS
        private List<Image> m_NonTutorialImages;
        private List<bool> m_NonTutorialImagesWereRaycast;
        private List<RawImage> m_NonTutorialRawImages;
        private List<bool> m_NonTutorialRawImagesWereRaycast;
        private List<TextMeshProUGUI> m_NonTutorialTexts;
        private List<bool> m_NonTutorialTextsWereRaycast;

        //this should reset every time the game is run. only completing the tutorial will set it to not repeat on startup
        private int m_CurrentStage = 0;
        [HideInInspector]
        public bool b_HasEverClearedToAny = false;

        private TutorialStageBehavior m_CurrentTutorial = null;

        public const float MIN_ALPHA = 0.012f;

        public void IStart()
        {
            m_CurrentStage = 0;
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
            m_CurrentTutorial = Instantiate(m_TutorialStages[0].prefab, gameObject.transform).GetComponent<TutorialStageBehavior>();
        }

        //call this from tutorial event buttons -- the ones in the main scene
        public void OnAdvanceTutorialButtonPressed(int buttonNumber)
        {
            if (m_CurrentTutorial != null && SaveLoadManager.Instance.ShouldShowTutorial)
            {
                m_CurrentTutorial.HandleButtonPress(buttonNumber);
            }
        }

        public void AdvanceTutorialStage()
        {
            StartCoroutine("AdvanceTutorialStageAfterFrame");
        }

        private System.Collections.IEnumerator AdvanceTutorialStageAfterFrame()
        {
            yield return null;

            m_CurrentStage += 1;

            if (m_CurrentStage < m_TutorialStages.Count)
            {
                //instance current tutorial stage
                m_CurrentTutorial = Instantiate(m_TutorialStages[m_CurrentStage].prefab, gameObject.transform).GetComponent<TutorialStageBehavior>();
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

            //re-enable all main window elements
            UpdateTutorialRaycastTarget(true);
            UpdateTutorialAlpha(1.0f);

            //always allow input when tutorial is over
            WindowManager.Instance.m_CellGrid.b_IsInputActive = true;
        }

        //b_ToDefault: if true, revert to original setting, if false, turn off
        private void UpdateTutorialRaycastTarget(bool b_ToDefault)
        {
            for (int i = 0; i < m_NonTutorialImages.Count; i++)
            {
                if (m_NonTutorialImages[i] != null)
                {
                    //only set to true if both are true
                    m_NonTutorialImages[i].raycastTarget = m_NonTutorialImagesWereRaycast[i] && b_ToDefault;
                }
            }
            for (int i = 0; i < m_NonTutorialRawImages.Count; i++)
            {
                if (m_NonTutorialRawImages[i] != null)
                {
                    //only set to true if both are true
                    m_NonTutorialRawImages[i].raycastTarget = m_NonTutorialRawImagesWereRaycast[i] && b_ToDefault;
                }
            }
            for (int i = 0; i < m_NonTutorialTexts.Count; i++)
            {
                if (m_NonTutorialTexts[i] != null)
                {
                    //only set to true if both are true
                    m_NonTutorialTexts[i].raycastTarget = m_NonTutorialTextsWereRaycast[i] && b_ToDefault;
                }
            }
        }
        private void UpdateTutorialAlpha(float a)
        {
            for (int i = 0; i < m_NonTutorialImages.Count; i++)
            {
                if (m_NonTutorialImages[i] != null)
                {
                    m_NonTutorialImages[i].color = new Color(m_NonTutorialImages[i].color.r, m_NonTutorialImages[i].color.g, m_NonTutorialImages[i].color.b, a); 
                }
            }
            for (int i = 0; i < m_NonTutorialRawImages.Count; i++)
            {
                if (m_NonTutorialRawImages[i] != null)
                {
                    m_NonTutorialRawImages[i].color = new Color(m_NonTutorialRawImages[i].color.r, m_NonTutorialRawImages[i].color.g, m_NonTutorialRawImages[i].color.b, a);
                }
            }
            for (int i = 0; i < m_NonTutorialTexts.Count; i++)
            {
                if (m_NonTutorialTexts[i] != null)
                {
                    m_NonTutorialTexts[i].color = new Color(m_NonTutorialTexts[i].color.r, m_NonTutorialTexts[i].color.g, m_NonTutorialTexts[i].color.b, a); 
                }
            }
        }
        public void UpdateFocusObjects(float a, bool b_raycastable)
        {
            if(m_AllSceneFocusObjectsSoFar == null) { m_AllSceneFocusObjectsSoFar = new List<GameObject>(); }
            if (m_AllSceneRaycastableObjectsSoFar == null) { m_AllSceneRaycastableObjectsSoFar = new List<GameObject>(); }

            //we want all "unlocked" buttons to just stay unlocked. append this stage's
            m_AllSceneFocusObjectsSoFar.AddRange(m_TutorialStages[m_CurrentStage].m_SceneFocusObjects);

            //for all scene objects so far
            foreach (GameObject obj in m_AllSceneFocusObjectsSoFar)
            {
                ChangeAlphaOnCompatibleComponents(obj, a);

                //for each child of this root obj
                Transform[] ts = obj.GetComponentsInChildren<Transform>();
                foreach (Transform childT in ts)
                {
                    ChangeAlphaOnCompatibleComponents(childT.gameObject, a);
                }
            }

            m_AllSceneRaycastableObjectsSoFar.AddRange(m_TutorialStages[m_CurrentStage].m_SceneRaycastableObjects);
            foreach (GameObject obj in m_AllSceneRaycastableObjectsSoFar)
            {
                Image im = obj.GetComponent<Image>();
                if (im != null)
                {
                    im.raycastTarget = b_raycastable;
                }
                RawImage rim = obj.GetComponent<RawImage>();
                if (rim != null)
                {
                    rim.raycastTarget = b_raycastable;
                }
                TextMeshProUGUI txt = obj.GetComponent<TextMeshProUGUI>();
                if (txt != null)
                {
                    txt.raycastTarget = b_raycastable;
                }
                TMP_InputField tmpi = obj.GetComponent<TMP_InputField>();
                if (tmpi != null)
                {
                    tmpi.interactable = b_raycastable;
                }
            }
        }

        private void ChangeAlphaOnCompatibleComponents(GameObject obj, float a)
        {
            Image im = obj.GetComponent<Image>();
            if (im != null)
            {
                im.color = new Color(im.color.r, im.color.g, im.color.b, a);
            }
            RawImage rim = obj.GetComponent<RawImage>();
            if (rim != null)
            {
                rim.color = new Color(rim.color.r, rim.color.g, rim.color.b, a);
            }
            TextMeshProUGUI txt = obj.GetComponent<TextMeshProUGUI>();
            if (txt != null)
            {
                txt.color = new Color(txt.color.r, txt.color.g, txt.color.b, a);
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