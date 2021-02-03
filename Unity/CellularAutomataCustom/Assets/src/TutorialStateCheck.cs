using UnityEngine;

namespace ca
{
    public class TutorialStateCheck
    {
        public virtual bool CanAdvanceStage(TutorialStageBehavior tb=null)
        {
            return true;
        }
    }
    public class TutorialSC1 : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            return WindowManager.Instance.m_CellGrid.m_CellGrid.GetAntiColorCount(WindowManager.Instance.GetClearToColor()) > 8;
        }
    }
    public class TutorialSC2 : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            return WindowManager.Instance.m_CellGrid.m_CellGrid.GetAntiColorCount(WindowManager.Instance.GetClearToColor()) > 36;
        }
    }
    public class TutorialSC3 : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            //if clearto color has been changed AND
            //once clearto-any has been executed once
            if (tb != null && tb.m_ButtonPressCounts != null && tb.m_ButtonPressCounts.ContainsKey(0) && tb.m_ButtonPressCounts.ContainsKey(1))
            {
                return tb.m_ButtonPressCounts[0] >= 1 && tb.m_ButtonPressCounts[1] >= 1 && TutorialManager.Instance.b_HasEverClearedToAny;
            }
            return false;
        }
    }
    public class TutorialSC4 : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            //if clearto color has been changed AND
            //once clearto-any has been executed once
            if (tb != null && tb.m_ButtonPressCounts != null && tb.m_ButtonPressCounts.ContainsKey(0))
            {
                return tb.m_ButtonPressCounts[0] >= 7;
            }
            return false;
        }
    }
    public class TutorialSC5 : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            //if SIMULATE has been pressed and FPS+ has been changed and FPS- has been changed and issimulating
            if (tb != null && tb.m_ButtonPressCounts != null &&
                tb.m_ButtonPressCounts.ContainsKey(0) &&
                tb.m_ButtonPressCounts.ContainsKey(1) &&
                tb.m_ButtonPressCounts.ContainsKey(2))
            {
                return (tb.m_ButtonPressCounts[0] > 0) && (tb.m_ButtonPressCounts[1] + tb.m_ButtonPressCounts[2]) >= 6 && WindowManager.Instance.IsSimulating;
            }
            return false;
        }
    }
    public class TutorialSC6 : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            //if has zoomed in and out a few times
            //and we're simulating
            if (tb != null && tb.m_ButtonPressCounts != null &&
                tb.m_ButtonPressCounts.ContainsKey(0) &&
                tb.m_ButtonPressCounts.ContainsKey(1))
            {
                return tb.m_ButtonPressCounts[0] >= 3 &&
                    tb.m_ButtonPressCounts[1] >= 3 &&
                    WindowManager.Instance.IsSimulating;
            }
            return false;
        }
    }
}