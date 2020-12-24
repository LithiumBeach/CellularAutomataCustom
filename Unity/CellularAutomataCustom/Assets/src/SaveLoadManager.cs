using System.Collections.Generic;
using UnityEngine;

namespace ca
{
    //Manages PlayerPrefs dictionary for saving/loading
    //["rulesets"] = <json serialized rulesets array> 
    //["current_ruleset"] = <integer index>
    public class SaveLoadManager : patterns.SingletonBehavior<SaveLoadManager>
    {
        //lists should be contained in a class to serialize
        private class RulesetSOList
        {
            public List<RulesetSO> list;
        }
        private RulesetSOList m_Rulesets = null;


        private int m_CurrentRuleset;
        public int CurrentRuleset { get { return m_CurrentRuleset; } }
        //adds or subtracts one from direction
        public void ChangeRuleset(int direction)
        {
            m_CurrentRuleset += direction;
        }

        public void Initialize(List<RulesetSO> defaultRulesets)
        {
            //if ["current_ruleset"] doesn't exist, 0
            m_CurrentRuleset = PlayerPrefs.GetInt("current_ruleset", 0);

            //load list of rulesets from json string entry in PlayerPrefs
            m_Rulesets = JsonUtility.FromJson<RulesetSOList>(PlayerPrefs.GetString("rulesets"));
            if (m_Rulesets == null)
            {
                //create stock ruleset (conway's game of life?)

            }
        }
    }
}