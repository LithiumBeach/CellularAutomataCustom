using UnityEngine;

namespace ca
{
    public class TutorialStateCheck
    {
        public virtual bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            return true;
        }
    }
    public class TutorialSC1 : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            return WindowManager.Instance.m_CellGrid.m_CellGrid.GetAntiColorCount(WindowManager.Instance.GetClearToColor()) >= 6;
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
            // 0 = clearboardcolor button, 1 = clearboard button
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
            //2 = nextframe button
            if (tb != null && tb.m_ButtonPressCounts != null && tb.m_ButtonPressCounts.ContainsKey(2))
            {
                return tb.m_ButtonPressCounts[2] >= 7;
            }
            return false;
        }
    }
    public class TutorialSC5 : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            //if SIMULATE has been pressed and FPS+ has been changed and FPS- has been changed and issimulating
            // 3 = simulate, 4 = {FPS+ & FPS-}
            if (tb != null && tb.m_ButtonPressCounts != null &&
                tb.m_ButtonPressCounts.ContainsKey(3) &&
                tb.m_ButtonPressCounts.ContainsKey(4))
            {
                return (tb.m_ButtonPressCounts[3] > 0) && tb.m_ButtonPressCounts[4] >= 2 && WindowManager.Instance.IsSimulating;
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
            //6 = {zoom in & zoom out}
            if (tb != null && tb.m_ButtonPressCounts != null &&
                tb.m_ButtonPressCounts.ContainsKey(6))
            {
                return tb.m_ButtonPressCounts[6] >= 3;
            }
            return false;
        }
    }
    public class TutorialSC7 : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            //8 = {ruleset> & ruleset<}
            if (tb != null && tb.m_ButtonPressCounts != null &&
                tb.m_ButtonPressCounts.ContainsKey(8))
            {
                return tb.m_ButtonPressCounts[8] >= 2;
            }
            return false;
        }
    }
    public class TutorialSC8 : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            //10 = new ruleset
            if (tb != null && tb.m_ButtonPressCounts != null && tb.m_ButtonPressCounts.ContainsKey(10))
            {
                return tb.m_ButtonPressCounts[10] >= 1;
            }
            return false;
        }
    }
    public class TutorialSC9 : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            //12 = add rule
            if (tb != null && tb.m_ButtonPressCounts != null && tb.m_ButtonPressCounts.ContainsKey(12))
            {
                return tb.m_ButtonPressCounts[12] >= 1;
            }
            return false;
        }
    }
    public class TutorialSCCustomizeColor : TutorialStateCheck
    {
        public override bool CanAdvanceStage(TutorialStageBehavior tb = null)
        {
            //11 = customize color
            if (tb != null && tb.m_ButtonPressCounts != null && tb.m_ButtonPressCounts.ContainsKey(11))
            {
                return tb.m_ButtonPressCounts[11] >= 1;
            }
            return false;
        }
    }
}