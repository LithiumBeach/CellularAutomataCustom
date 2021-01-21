//WindowManager.cs
//Keep this as lean as possible: manages all buttons, rulesets, and a cell grid
//Buttons trigger functions here
//Manages ONLY displayed portion of rulesets (current ruleset). Call into SaveLoadManager.

using Sirenix.OdinInspector;
using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace ca
{
    public class WindowManager : patterns.SingletonBehavior<WindowManager>, patterns.IStart
    {
        [Required]
        public CellGridBehavior m_CellGrid;
        [Required]
        public GlobalColorBehavior m_GlobalColorBehavior;
        [Required]
        public TextMeshProUGUI m_FPSText;
        [Required]
        public TextMeshProUGUI m_SimulateButtonText;
        [Required]
        public TextMeshProUGUI m_ZoomButtonText;
        [Required]
        public Canvas m_PhotosensitivityWarningCanvas;

        [Required]
        public Transform m_RulesetParent;
        [Required]
        public RuleBehavior m_RuleBehaviorPrefab;
        [Required]
        public TMP_InputField m_RulesetTitle;

        //rainbow background for colorbuttons referencing ANY color
        //not required: null will be a color bg
        public Texture m_AnyColorBgTex;

        public List<GameObject> m_LockButtonGOs;

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

            //display photosensitivity warning if we should
            DisplayPhotosensitivityWarning(SaveLoadManager.Instance.ShouldShowPhotosensitivityWarning);

            //turn off global color canvas
            DisplayGlobalColorCanvas(false);
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

        //turn off & on the photosensitivity warning canvas
        private void DisplayPhotosensitivityWarning(bool canvasActive)
        {
            m_PhotosensitivityWarningCanvas.gameObject.SetActive(canvasActive);
        }
        public void OnPhotosensitivityWarningConfirm(bool b_donotshowagain=false)
        {
            DisplayPhotosensitivityWarning(false);

            if (b_donotshowagain)
            {
                SaveLoadManager.Instance.ShouldShowPhotosensitivityWarning = false;
            }
        }
        public void OnPhotosensitivityWarningExit()
        {
            //ensure warning will reappear if user returns after pressing exit
            SaveLoadManager.Instance.ShouldShowPhotosensitivityWarning = true;
            Application.Quit();
        }

        //turn off & on the global color canvas
        private void DisplayGlobalColorCanvas(bool canvasActive)
        {
            m_GlobalColorBehavior.gameObject.SetActive(canvasActive);

            //if we are turning it on
            if (canvasActive)
            {
                //re-initialize to what's in SaveLoadManager
                m_GlobalColorBehavior.ResetTo(SaveLoadManager.Instance.Colors);
            }
            else
            {
                //re-initialize main window in case any colors have changed
                ChangeRuleset(0);
            }
            m_CellGrid.ChangeClearToColor(0); //reset clearto color box, too
            m_CellGrid.SyncZoomTexture(); //reset cellgrid, too
        }

        //call to turn on/off, opposite of current on/off
        public void OnDisplayGlobalColorCanvasButtonPressed(bool canvasActive)
        {
            DisplayGlobalColorCanvas(canvasActive);
        }

        #region Rulesets

        private void LoadCurrentRulesetUI()
        {
            if (m_RulesetParent.childCount > 0)
            {
                //destroy any leftover children from the previous ruleset
                DestroyAllRuleUIs();
            }
            m_RulesetTitle.text = SaveLoadManager.Instance.CurrentRulesetName;

            // wait 1 frame before updating UI
            // will otherwise cause issues using a transform's sibling index as an id
            StartCoroutine(OnAfterLoadCurrentRulesetUI());
        }

        public bool IsMainCanvasActive()
        {
            return
                !m_PhotosensitivityWarningCanvas.isActiveAndEnabled &&
                !m_GlobalColorBehavior.isActiveAndEnabled;
        }

        public System.Collections.IEnumerator OnAfterLoadCurrentRulesetUI()
        {
            yield return null;
            Debug.Assert(m_RulesetParent.childCount == 0);
            //instantiate all rules
            foreach (RuleData rd in SaveLoadManager.Instance.CurrentRuleset)
            {
                //do not add data, only add rule ui's.
                AddRuleUI(rd, (SaveLoadManager.Instance.IsCurrentRulesetLocked));
            }
            SetLocked(SaveLoadManager.Instance.IsCurrentRulesetLocked);

            StopCoroutine("OnAfterLoadCurrentRulesetUI");
        }

        private void SetLocked(bool isCurrentRulesetLocked)
        {
            m_RulesetTitle.interactable = !isCurrentRulesetLocked;
            foreach(GameObject go in m_LockButtonGOs)
            {
                //it is enabled if it is unlocked
                go.SetActive(!isCurrentRulesetLocked);
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

        private void AddRuleUI(RuleData rd, bool b_locked=false)
        {
            //create new rule UI
            RuleBehavior rb = Instantiate(m_RuleBehaviorPrefab);
            rb.transform.SetParent(m_RulesetParent);
            rb.txtRuleX.text = String.Format(RuleBehavior.c_RuleXString, (rb.transform.GetSiblingIndex()+1).ToString());
            rb.SetLocked(b_locked);

            //Initialize all ui elements in from ruledata
            rb.UpdateAllUI(rd);
        }

        public void DeleteRule(RuleBehavior ruleBehavior)
        {
            int ruleIndex = ruleBehavior.transform.GetSiblingIndex();
            SaveLoadManager.Instance.DeleteRule(ruleIndex);
            Destroy(ruleBehavior.gameObject);

            StartCoroutine(OnAfterDeleteRule());
        }

        public System.Collections.IEnumerator OnAfterDeleteRule()
        {
            yield return null;

            UpdateAllRuleUINames();

            StopCoroutine("OnAfterDeleteRule");
        }

        private void UpdateAllRuleUINames()
        {
            foreach (Transform t in m_RulesetParent)
            {
                if (t.GetComponent<RuleBehavior>())
                {
                    t.GetComponent<RuleBehavior>().txtRuleX.text = String.Format(RuleBehavior.c_RuleXString, (t.GetSiblingIndex() + 1).ToString());
                }
            }
        }

        public void ChangeRuleset(int dir)
        {
            SaveLoadManager.Instance.ChangeCurrentRuleset(dir);
            if (m_RulesetParent.childCount > 0)
            {
                DestroyAllRuleUIs();
            }

            // wait 1 frame before updating UI
            // will otherwise cause issues using a transform's sibling index as an id
            StartCoroutine(OnAfterChangeRuleset());
        }

        public System.Collections.IEnumerator OnAfterChangeRuleset()
        {
            yield return null;

            LoadCurrentRulesetUI();

            StopCoroutine("OnAfterChangeRuleset");
        }

        private void DestroyAllRuleUIs()
        {
            if (m_RulesetParent.childCount > 0)
            {
                foreach (Transform t in m_RulesetParent)
                {
                    Destroy(t.gameObject);
                }
            }
        }

        public void OnEndEditRulesetName()
        {
            SaveLoadManager.Instance.ChangeCurrentRulesetName(m_RulesetTitle.text);
        }

        public void OnDeleteRulesetPressed()
        {
            SaveLoadManager.Instance.DeleteCurrentRuleset();

            ChangeRuleset(0);
        }

        #endregion

        #region Rule Button Press Callbacks
        //returns new color index
        public int OnThisColorChange(RuleBehavior ruleBehavior, int direction)
        {
            //GetSiblingIndex
            int rbIndex = ruleBehavior.transform.GetSiblingIndex();

            //concerning cells of color 'any' is fine here
            SaveLoadManager.Instance.SetThisColor(rbIndex, 
                CAColor.ChangeColorInt(SaveLoadManager.Instance.CurrentRuleset[rbIndex].m_ThisColor, direction, b_includeClear:true));

            return SaveLoadManager.Instance.CurrentRuleset[rbIndex].m_ThisColor;
        }
        public int OnIfColorChange(RuleBehavior ruleBehavior, int direction)
        {
            int rbIndex = ruleBehavior.transform.GetSiblingIndex();

            //concerning cells of color 'any' is fine here
            SaveLoadManager.Instance.SetIfColor(rbIndex, 
                CAColor.ChangeColorInt(SaveLoadManager.Instance.CurrentRuleset[rbIndex].m_IfColor, direction, b_includeClear:true));

            return SaveLoadManager.Instance.CurrentRuleset[rbIndex].m_IfColor;
        }
        public int OnThenColorChange(RuleBehavior ruleBehavior, int direction)
        {
            int rbIndex = ruleBehavior.transform.GetSiblingIndex();
            //then color cannot be transparent
            SaveLoadManager.Instance.SetThenColor(rbIndex,
                CAColor.ChangeColorInt(SaveLoadManager.Instance.CurrentRuleset[rbIndex].m_ThenColor, direction));
            return SaveLoadManager.Instance.CurrentRuleset[rbIndex].m_ThenColor;
        }
        public int OnMinNeighborsChange(RuleBehavior ruleBehavior, int direction)
        {
            int rbIndex = ruleBehavior.transform.GetSiblingIndex();
            SaveLoadManager.Instance.SetMinNeighbors(rbIndex,
                CAMath.Mod(SaveLoadManager.Instance.CurrentRuleset[rbIndex].m_MinNumNeighbors + direction, 9)); //9 directions
            return SaveLoadManager.Instance.CurrentRuleset[rbIndex].m_MinNumNeighbors;
        }
        public int OnMaxNeighborsChange(RuleBehavior ruleBehavior, int direction)
        {
            int rbIndex = ruleBehavior.transform.GetSiblingIndex();
            SaveLoadManager.Instance.SetMaxNeighbors(rbIndex, 
               CAMath.Mod(SaveLoadManager.Instance.CurrentRuleset[rbIndex].m_MaxNumNeighbors + direction, 9)); //9 directions
            return SaveLoadManager.Instance.CurrentRuleset[rbIndex].m_MaxNumNeighbors;
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
            m_CellGrid.EvaluateNextState(SaveLoadManager.Instance.CurrentRuleset);
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