using UnityEngine;
using UnityEngine.Events;
using UnityEngine.EventSystems;

namespace ca
{
    public class CAButton : MonoBehaviour, IPointerClickHandler
    {
        public UnityEvent OnLeftClick;
        public UnityEvent OnRightClick;
        public void OnPointerClick(PointerEventData e)
        {
            if (e.button == PointerEventData.InputButton.Left)
            {
                OnLeftClick.Invoke();
            }
            else if (e.button == PointerEventData.InputButton.Right)
            {
                OnRightClick.Invoke();
            }
        }
    }
}