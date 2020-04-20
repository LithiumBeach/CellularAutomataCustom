using UnityEngine;

namespace ca
{
    static class EditorUtils
    {

        public static void DestroyAllChildren(this Transform t)
        {
            GameObject[] objsToDestroy = new GameObject[t.childCount];
            int idx = 0;
            foreach (Transform child in t)
            {
                objsToDestroy[idx++] = child.gameObject;
            }
            for (int i = 0; i < objsToDestroy.Length; ++i)
            {
#if UNITY_EDITOR
                GameObject.DestroyImmediate(objsToDestroy[i]);
#else
                GameObject.Destroy(objsToDestroy[i]);
#endif
            }
        }
    }
}