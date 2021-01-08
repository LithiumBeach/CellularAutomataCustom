//RuleDataList.cs
//Serializable List<RuleDatas> for single ruleset
using System.Collections.Generic;

namespace ca
{
    [System.Serializable]
    public class RuleDataList
    {
        public RuleDataList(string n = "") { list = new List<RuleData>(); name = n; }
        public RuleDataList(List<RuleData> l, string n = "") { list = new List<RuleData>(l); name = n; }

        public string name;
        public List<RuleData> list;
    }
}