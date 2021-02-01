using UnityEngine;

namespace ca
{
    public class TutorialStateCheck
    {
        public virtual bool CanAdvanceStage()
        {
            return true;
        }
    }
    public class TutorialSC1 : TutorialStateCheck
    {
        public override bool CanAdvanceStage()
        {
            return WindowManager.Instance.m_CellGrid.m_CellGrid.GetAntiColorCount(WindowManager.Instance.GetClearToColor()) > 8;
        }
    }
    public class TutorialSC2 : TutorialStateCheck
    {
        public override bool CanAdvanceStage()
        {
            return WindowManager.Instance.m_CellGrid.m_CellGrid.GetAntiColorCount(WindowManager.Instance.GetClearToColor()) > 36;
        }
    }

}