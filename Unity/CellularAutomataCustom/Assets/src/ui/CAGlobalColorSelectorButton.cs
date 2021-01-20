using Sirenix.OdinInspector;
using UnityEngine.UI;

namespace ca
{
    public class CAGlobalColorSelectorButton : CAButton
    {
        [Required]
        public RawImage m_Background;

        public void HandleLeftClick()
        {
            GlobalColorBehavior.Instance.OnLeftMouseClickColorSquare(this);
        }
        public void HandleRightClick()
        {
            GlobalColorBehavior.Instance.OnRightMouseClickColorSquare(this);
        }
    }
}