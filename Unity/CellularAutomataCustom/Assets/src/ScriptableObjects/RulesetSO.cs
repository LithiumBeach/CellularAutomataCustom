using System.Collections.Generic;
using UnityEngine;

namespace ca
{
    [System.Serializable]
    [CreateAssetMenu(fileName = "Ruleset", menuName = "ScriptableObjects/Ruleset", order = 1)]
    public class RulesetSO : ScriptableObject
    {
        [Sirenix.Serialization.OdinSerialize]
        public List<RuleData> m_Rules;

        public string m_Title = "";

        public override string ToString()
        {
            string _str = "";
            foreach (var rule in m_Rules)
            {
                _str += rule.m_IfColor.ToString() + " " + rule.m_ThenColor.ToString() + "\n";
            }
            return _str;
        }

        //optional link, when clicked, will open browser at this url
        public string m_Hyperlink = "";
    }
}