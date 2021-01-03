﻿using Sirenix.OdinInspector;
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

        //lists should be contained in a class to serialize
        [Serializable]
        private class RulesetSOList
        {
            public RulesetSOList() { list = new List<RulesetSO>(); }
            public RulesetSOList(List<RulesetSO> l) { list = l; }
            public List<RulesetSO> list;
        }

        private RulesetSOList Rulesets
        {
            get { return JsonUtility.FromJson<RulesetSOList>(PlayerPrefs.GetString(c_RulesetsKey)); }
        }

        private int CurrentRulesetIndex
        { get { return PlayerPrefs.GetInt(c_CurrentRulesetKey); } }


        #region Public Accessors

        public int NumRulesets
        {
            get { return Rulesets.list.Count; }
        }

        //returns copy of current ruleset
        public RulesetSO CurrentRuleset { get { return Rulesets.list[PlayerPrefs.GetInt(c_CurrentRulesetKey)]; } }
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


        public void AddNewRuleset(List<RuleData> defaultRules=null)
        {
            //convert from json to object
            RulesetSOList rsets = JsonUtility.FromJson<RulesetSOList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );

            //copy default rules if not null
            if (defaultRules != null)
            {
                rsets.list[NumRulesets].m_Rules = new List<RuleData>(defaultRules);
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
            RulesetSOList rsets = JsonUtility.FromJson<RulesetSOList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].m_Rules.Add(rd);

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
            RulesetSOList rsets = JsonUtility.FromJson<RulesetSOList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].m_Rules[ruleIndex].m_ThisColor = newValue;

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
            RulesetSOList rsets = JsonUtility.FromJson<RulesetSOList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].m_Rules[ruleIndex].m_IfColor = newValue;

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
            RulesetSOList rsets = JsonUtility.FromJson<RulesetSOList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].m_Rules[ruleIndex].m_ThenColor = newValue;

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
            RulesetSOList rsets = JsonUtility.FromJson<RulesetSOList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].m_Rules[ruleIndex].m_MinNumNeighbors = newValue;

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
            RulesetSOList rsets = JsonUtility.FromJson<RulesetSOList>(
                PlayerPrefs.GetString(c_RulesetsKey)
            );
            rsets.list[CurrentRulesetIndex].m_Rules[ruleIndex].m_MaxNumNeighbors = newValue;

            //save
            PlayerPrefs.SetString(c_RulesetsKey,
                //convert from object to json
                JsonUtility.ToJson(rsets)
            );

            PlayerPrefs.Save();
        }

        #endregion

        #region public functions
        public List<RulesetSO> m_DefaultRulesets;
        [Sirenix.OdinInspector.Button(Name = "Reset PlayerPrefs")]
        public void ResetPlayerPrefs()
        {
            PlayerPrefs.DeleteAll();

            //current ruleset starts at 0
            PlayerPrefs.SetInt(c_CurrentRulesetKey, 0);

            //initialize rulesets to the default (ie: conway's ruleset)
            PlayerPrefs.SetString(c_RulesetsKey, JsonUtility.ToJson(new RulesetSOList(m_DefaultRulesets)));

            PlayerPrefs.Save();
        }


        #endregion

        public void IStart()
        {
            //if the user has never saved data before into PlayerPrefs
            if (Rulesets == null || Rulesets.list.Count <= m_DefaultRulesets.Count)
            {
                //initialize like new
                ResetPlayerPrefs();
            }
        }
    }
}