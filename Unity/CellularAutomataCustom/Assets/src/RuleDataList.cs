//RuleDataList.cs
//Serializable List<RuleDatas>
using System.Collections.Generic;

namespace ca
{
    [System.Serializable]
    public class RuleDataList
    {
        public RuleDataList() { list = new List<RuleData>(); }
        public RuleDataList(List<RuleData> l) { list = new List<RuleData>(l); }

        public List<RuleData> list;
    }
}