using System.Collections.Generic;
using UnityEngine;

namespace ca
{
    [CreateAssetMenu(fileName = "Ruleset", menuName = "ScriptableObjects/Ruleset", order = 1)]
    public class RulesetSO : ScriptableObject
    {
        [Sirenix.Serialization.OdinSerialize]
        public List<Rule> m_Rules;
    }
}