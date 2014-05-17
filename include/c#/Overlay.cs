using System;
using System.Runtime.InteropServices;

namespace DX9OverlayAPI
{
    class DX9Overlay
    {
        [DllImport("dx9_overlay.dll")]
        public static extern int TextCreate(string font, int fontSize, bool bBold, bool bItalic, int x, int y, uint color, string text, bool bShadow, bool bShow);
        [DllImport("dx9_overlay.dll")]
        public static extern int TextDestroy(int id);
        [DllImport("dx9_overlay.dll")]
        public static extern int TextSetShadow(int id, bool b);
        [DllImport("dx9_overlay.dll")]
        public static extern int TextSetShown(int id, bool b);
        [DllImport("dx9_overlay.dll")]
        public static extern int TextSetColor(int id, uint color);
        [DllImport("dx9_overlay.dll")]
        public static extern int TextSetPos(int id, int x, int y);
        [DllImport("dx9_overlay.dll")]
        public static extern int TextSetString(int id, string str);
        [DllImport("dx9_overlay.dll")]
        public static extern int TextUpdate(int id, string font, int fontSize, bool bBold, bool bItalic);

        [DllImport("dx9_overlay.dll")]
        public static extern int BoxCreate(int x, int y, int w, int h, uint dwColor, bool bShow);
        [DllImport("dx9_overlay.dll")]
        public static extern int BoxDestroy(int id);
        [DllImport("dx9_overlay.dll")]
        public static extern int BoxSetShown(int id, bool bShown);
        [DllImport("dx9_overlay.dll")]
        public static extern int BoxSetBorder(int id, int height, bool bShown);
        [DllImport("dx9_overlay.dll")]
        public static extern int BoxSetBorderColor(int id, uint dwColor);
        [DllImport("dx9_overlay.dll")]
        public static extern int BoxSetColor(int id, uint dwColor);
        [DllImport("dx9_overlay.dll")]
        public static extern int BoxSetHeight(int id, int height);
        [DllImport("dx9_overlay.dll")]
        public static extern int BoxSetPos(int id, int x, int y);
        [DllImport("dx9_overlay.dll")]
        public static extern int BoxSetWidth(int id, int width);

        [DllImport("dx9_overlay.dll")]
	    public static extern int LineCreate(int x1, int y1, int x2, int y2, int width, uint color, bool bShow);
        [DllImport("dx9_overlay.dll")]
		public static extern int LineDestroy(int id);
        [DllImport("dx9_overlay.dll")]
		public static extern int LineSetShown(int id, bool bShown);
        [DllImport("dx9_overlay.dll")]
		public static extern int LineSetColor(int id, uint color);
        [DllImport("dx9_overlay.dll")]
		public static extern int LineSetWidth(int id, int width);
        [DllImport("dx9_overlay.dll")]
		public static extern int LineSetPos(int id, int x1, int y1, int x2, int y2);

        [DllImport("dx9_overlay.dll")]
		public static extern int ImageCreate(string path, int x, int y, int rotation, int align, bool bShow);
        [DllImport("dx9_overlay.dll")]
		public static extern int ImageDestroy(int id);
        [DllImport("dx9_overlay.dll")]
		public static extern int ImageSetShown(int id, bool bShown);
        [DllImport("dx9_overlay.dll")]
		public static extern int ImageSetAlign(int id, int align);
        [DllImport("dx9_overlay.dll")]
		public static extern int ImageSetPos(int id, int x, int y);
        [DllImport("dx9_overlay.dll")]
		public static extern int ImageSetRotation(int id, int rotation);

        [DllImport("dx9_overlay.dll")]
		public static extern int DestroyAllVisual();
        [DllImport("dx9_overlay.dll")]
		public static extern int ShowAllVisual();
        [DllImport("dx9_overlay.dll")]
		public static extern int HideAllVisual();

        [DllImport("dx9_overlay.dll")]
		public static extern int Init();
        [DllImport("dx9_overlay.dll")]
		public static extern void SetParam(string _szParamName, string _szParamValue);
    }
}
