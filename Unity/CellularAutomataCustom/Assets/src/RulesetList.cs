//RulesetList.cs
//Serializable List<List<RuleData>
using System.Collections.Generic;

namespace ca
{
    [System.Serializable]
    public class RulesetList
    {
        public RulesetList() { list = new List<RuleDataList>(); }
        public RulesetList(List<RulesetSO> rso)
        {
            list = new List<RuleDataList>();
            foreach (var ruleset in rso)
            {
                list.Add(new RuleDataList(ruleset.m_Rules));
            }
        }
        public List<RuleDataList> list;
    }
}