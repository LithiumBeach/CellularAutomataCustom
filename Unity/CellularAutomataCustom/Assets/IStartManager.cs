using UnityEngine;

namespace ca
{
    //class IStartManager
    //controls load order
    //explicitly calls Start() on objects registered to IStartManager (usually SingletonBehaviors)

    public class IStartManager : patterns.SingletonBehavior<IStartManager>
    {
        private void Start()
        {
            SaveLoadManager.Instance.IStart();
            WindowManager.Instance.IStart();
        }
    }
}