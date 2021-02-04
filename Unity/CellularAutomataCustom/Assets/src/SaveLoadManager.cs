using Sirenix.OdinInspector;
using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace ca
{
    //Manages PlayerPrefs dictionary for saving/loading
    //["rulesets"] = <json serialized rulesets array> 
    //["current_ruleset"] = <integer index>
    //Accessors: getters always load, setters always save to PlayerPrefs
    //Serialized values saved in PlayerPrefs should never be duplicated as member variables
    public class SaveLoadManager : patterns.SingletonBehavior<SaveLoadManager>, patterns.IStart
    {
        //PlayerPrefs accessors
        private const string c_RulesetsKey = "rulesets";
        private const string c_CurrentRulesetKey = "current_ruleset";
        private const string c_ShouldShowPhotosensitivityWarning = "should_show_photosens_warning";
        private const string c_GlobalColors = "global_colors";

        private const string c_ShouldShowTutorial = "should_show_tutorial";
        //TODO:
        private const string c_TutorialStage = "tutorial_stage";
        //TODO:
        private const string c_TutorialBoards = "tutorial_boards";

        //TODO: pull from a fun list of string pairings or something, 1hr max
        public const string c_NewRulesetName = "New Ruleset";

        private RulesetList Rulesets
        {
            get { return JsonUtility.FromJson<RulesetList>(PlayerPrefs.GetString(c_RulesetsKey)); }
        }

        private int CurrentRulesetIndex
        { get { return PlayerPrefs.GetInt(c_CurrentRulesetKey); } }


        #region Public Accessors

        public int NumRulesets
        {
            get { return Rulesets.list.Count; }
        }

        //returns copy of current ruleset
        public List<RuleData> CurrentRuleset { get { return Rulesets.list[PlayerPrefs.GetInt(c_CurrentRulesetKey)].list; } }

        public string CurrentRulesetName { get { return Rulesets.list[PlayerPrefs.GetInt(c_CurrentRulesetKey)].name; } }

        //we want to lock all default rulesets.
        public bool IsCurrentRulesetLocked { get { return CurrentRulesetIndex < m_DefaultRulesets.Count; } }

        //directly sets integer index of rulesets
        public void SetCurrentRuleset(int i)
        {
            int newIndex = CAMath.Mod(i, Rulesets.list.Count);
            PlayerPrefs.SetInt(c_CurrentRulesetKey, newIndex);
            PlayerPrefs.Save();
        }
        //adds or subtracts one from direction
        public void ChangeCurrentRuleset(int direction)
        {
            int newIndex = CAMath.Mod(CurrentRulesetIndex + direction, Rulesets.list.Count);
            PlayerPrefs.SetInt(c_CurrentRulesetKey, newIndex);
            PlayerPrefs.Save();
        }

        public int GetCurrentThisColor(int rIndex)
        {
            //convert from json to object
            return
                JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            )
            .list[CurrentRulesetIndex]
            .list[rIndex].m_ThisColor;
        }
        public int GetCurrentIfColor(int rIndex)
        {
            //convert from json to object
            return
                JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            )
            .list[CurrentRulesetIndex]
            .list[rIndex].m_IfColor;
        }
        public int GetCurrentThenColor(int rIndex)
        {
            //convert from json to object
            return
                JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            )
            .list[CurrentRulesetIndex]
            .list[rIndex].m_ThenColor;
        }
        public int GetCurrentMinNeighbors(int rIndex)
        {
            //convert from json to object
            return
                JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            )
            .list[CurrentRulesetIndex]
            .list[rIndex].m_MinNumNeighbors;
        }
        public int GetCurrentMaxNeighbors(int rIndex)
        {
            //convert from json to object
            return
                JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            )
            .list[CurrentRulesetIndex]
            .list[rIndex].m_MaxNumNeighbors;
        }

        //b_ignore0 to ignore ANY colors in returned list
        public List<int> GetCurrentRulesetColors(bool b_ignore0=true)
        {
            //use the keys property of this dictionary to keep track of all used colors
            Dictionary<int, bool> areColorsInRules = new Dictionary<int, bool>();
            List<RuleData> rules = CurrentRuleset;

            foreach (RuleData rule in rules)
            {
                //instead of looking for duplicates in a list, simply
                //overwrite dictionary entries with true, so that
                //any colors never used are never added to the dictionary
                areColorsInRules[rule.m_ThisColor] =
                    areColorsInRules[rule.m_IfColor] =
                    areColorsInRules[rule.m_ThenColor] = true;
            }

            //delete dict[0] if we should ignore 0
            if (b_ignore0)
            {
                areColorsInRules.Remove(0);
            }

            //return only created keys
            return areColorsInRules.Keys.ToList();
        }


        public void AddNewRuleset(List<RuleData> defaultRules = null)
        {
            //convert from json to object
            RulesetList rsets = JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );

            //copy default rules if not null
            if (defaultRules != null)
            {
                rsets.list.Add(new RuleDataList(defaultRules, c_NewRulesetName));
            }
            else
            {
                rsets.list.Add(new RuleDataList(c_NewRulesetName));
            }

            //save
            PlayerPrefs.SetString(c_RulesetsKey,
                //convert from object to json
                JsonUtility.ToJson(rsets)
            );

            PlayerPrefs.Save();
        }

        public void AddNewRule() { AddNewRule(new RuleData()); }
        public void AddNewRule(RuleData rd)
        {
            //convert from json to object
            RulesetList rsets = JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].list.Add(rd);

            //save
            PlayerPrefs.SetString(c_RulesetsKey,
                //convert from object to json
                JsonUtility.ToJson(rsets)
            );

            PlayerPrefs.Save();
        }

        public void SetThisColor(int ruleIndex, int newValue)
        {
            //convert from json to object
            RulesetList rsets = JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].list[ruleIndex].m_ThisColor = newValue;

            //save
            PlayerPrefs.SetString(c_RulesetsKey,
                //convert from object to json
                JsonUtility.ToJson(rsets)
            );

            PlayerPrefs.Save();
        }

        public void SetIfColor(int ruleIndex, int newValue)
        {
            //convert from json to object
            RulesetList rsets = JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].list[ruleIndex].m_IfColor = newValue;

            //save
            PlayerPrefs.SetString(c_RulesetsKey,
                //convert from object to json
                JsonUtility.ToJson(rsets)
            );

            PlayerPrefs.Save();
        }
        public void SetThenColor(int ruleIndex, int newValue)
        {
            //convert from json to object
            RulesetList rsets = JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].list[ruleIndex].m_ThenColor = newValue;

            //save
            PlayerPrefs.SetString(c_RulesetsKey,
                //convert from object to json
                JsonUtility.ToJson(rsets)
            );

            PlayerPrefs.Save();
        }
        public void SetMinNeighbors(int ruleIndex, int newValue)
        {
            //convert from json to object
            RulesetList rsets = JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].list[ruleIndex].m_MinNumNeighbors = newValue;

            //save
            PlayerPrefs.SetString(c_RulesetsKey,
                //convert from object to json
                JsonUtility.ToJson(rsets)
            );

            PlayerPrefs.Save();
        }

        internal void DeleteRule(int ruleIndex)
        {
            //convert from json to object
            RulesetList rsets = JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].list.RemoveAt(ruleIndex);

            //save
            PlayerPrefs.SetString(c_RulesetsKey,
                //convert from object to json
                JsonUtility.ToJson(rsets)
            );

            PlayerPrefs.Save();
        }

        public void SetMaxNeighbors(int ruleIndex, int newValue)
        {
            //convert from json to object
            RulesetList rsets = JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].list[ruleIndex].m_MaxNumNeighbors = newValue;

            //save
            PlayerPrefs.SetString(c_RulesetsKey,
                //convert from object to json
                JsonUtility.ToJson(rsets)
            );

            PlayerPrefs.Save();
        }

        public void ChangeCurrentRulesetName(string str)
        {//convert from json to object
            RulesetList rsets = JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );

            rsets.list[CurrentRulesetIndex].name = str;

            //save
            PlayerPrefs.SetString(c_RulesetsKey,
                //convert from object to json
                JsonUtility.ToJson(rsets)
            );

            PlayerPrefs.Save();
        }

        public void DeleteCurrentRuleset()
        {
            DeleteRuleset(CurrentRulesetIndex);
        }
        private void DeleteRuleset(int index)
        {
            Debug.Assert(index > 0 && index < NumRulesets);

            //convert json to object
            RulesetList rsets = JsonUtility.FromJson<RulesetList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );

            //delete ruleset
            rsets.list.RemoveAt(index);

            //save
            PlayerPrefs.SetString(c_RulesetsKey,
                //convert from object to json
                JsonUtility.ToJson(rsets)
            );

            PlayerPrefs.Save();
        }
        #endregion

        #region PlayerPrefs getters & setters

        public bool ShouldShowPhotosensitivityWarning
        {//cannot serialize to bool, so int
            get
            {
                return PlayerPrefs.GetInt(c_ShouldShowPhotosensitivityWarning) == 1;
            }
            set
            {
                PlayerPrefs.SetInt(c_ShouldShowPhotosensitivityWarning, value ? 1 : 0);
                PlayerPrefs.Save();
            }
        }

        public List<Color> Colors
        {
            get
            {
                return JsonUtility.FromJson<ColorList>(PlayerPrefs.GetString(c_GlobalColors)).list;
            }
            set
            {
                CAColor.colors = new List<Color>(value);
                PlayerPrefs.SetString(c_GlobalColors, JsonUtility.ToJson(new ColorList(value)));
            }
        }
        public void AddColor(Color c)
        {
            List<Color> newColors = Colors;
            newColors.Add(c);
            CAColor.colors.Add(c);
            Colors = newColors;
        }
        public void RemoveColor(int index)
        {
            List<Color> newColors = Colors;
            newColors.RemoveAt(index);
            CAColor.colors.RemoveAt(index);
            Colors = newColors;
        }
        public void ChangeColor(int v, Color color)
        {
            List<Color> newColors = Colors;
            newColors[v] = color;
            CAColor.colors[v] = color;
            Colors = newColors;
        }

        //0 dont show, else show tutorial
        public bool ShouldShowTutorial
        {
            get
            {
                return PlayerPrefs.GetInt(c_ShouldShowTutorial) != 0;
            }
            set
            {
                PlayerPrefs.SetInt(c_ShouldShowTutorial, value ? 1 : 0);
            }
        }


        #endregion





        //search in all this, if, to colors in all rulesets and find the maximum state value
        private int MaximumColorStateConsideredGlobally()
        {
            //aggregate all rules into one list
            List<RuleDataList> rulesets = Rulesets.list;
            List<RuleData> allrules = (from ruleset in rulesets
                                       from item in ruleset.list
                                       select item).ToList();

            int maxColor = 0;
            foreach(RuleData r in allrules)
            {
                maxColor = new List<int>() { maxColor, r.m_ThisColor, r.m_IfColor, r.m_ThenColor }.Max();
            }

            //finally, edge case, compare the user's selected clearto color, in case it's the max color
            return Math.Max(maxColor, WindowManager.Instance.GetClearToColor());
        }

        //Given the current number of color states and the current max color state considered globally,
        // can we afford to delete a single color state?
        public bool CanDeleteOneColor()
        {
            return (Colors.Count-1) > MaximumColorStateConsideredGlobally();
        }

        #region public functions
        public List<RulesetSO> m_DefaultRulesets;
        [Sirenix.OdinInspector.Button(Name = "Reset PlayerPrefs")]
        public void ResetPlayerPrefs()
        {
            PlayerPrefs.DeleteAll();

            //current ruleset starts at 0
            PlayerPrefs.SetInt(c_CurrentRulesetKey, 0);

            //initialize rulesets to the default (ie: conway's ruleset)
            PlayerPrefs.SetString(c_RulesetsKey, JsonUtility.ToJson(new RulesetList(m_DefaultRulesets)));

            //show photosensitivity warning
            ShouldShowPhotosensitivityWarning = true;

            ShouldShowTutorial = true;

            Colors = CAColor.defaultColors;

            SetCurrentRuleset(0);

            PlayerPrefs.Save();
        }


        #endregion

        public void IStart()
        {
            //if the user has never saved data before into PlayerPrefs
            //(or has only partially completed the tutorial)
            if (Rulesets == null || ShouldShowTutorial)
            {
                //initialize like new
                ResetPlayerPrefs();
            }
            //load anything from PlayerPrefs that has an alias somehwere else in the code
            else
            {
                CAColor.colors = Colors;
            }
        }
    }
}