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

        public override string ToString()
        {
            string _str = "";
            foreach (var rule in m_Rules)
            {
                _str += rule.m_IfColor.ToString() + " " + rule.m_ThenColor.ToString() + "\n";
            }
            return _str;
        }
    }
}