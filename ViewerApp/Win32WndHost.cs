using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Interop;
using System;
using System.Runtime.InteropServices;
using System.Windows.Interop;
using ViewerCommon;
using ViewerGraphicsEditor;

namespace ViewerApp
{
    public static class NeverTranslateClass
    {
        public static string NeverTranslate(string s)
        {
            return s;
        }
    }
    public class Win32WndHost : HwndHost
    {
        private IntPtr hwndControl = IntPtr.Zero;
        private int hostHeight;
        private int hostWidth;
        internal const int WS_CHILD = 1073741824;
        internal const int WS_VISIBLE = 268435456;
        internal const int LBS_NOTIFY = 1;
        internal const int HOST_ID = 2;
        internal const int LISTBOX_ID = 1;
        internal const int WS_VSCROLL = 2097152;
        internal const int BS_DEFPUSHBUTTON = 1;
        internal const int WS_BORDER = 8388608;
        internal const int WS_CLIPCHILDREN = 33554432;

        public IntPtr hwndHost { get; set; }

        public IntPtr HwndControl
        {
            get
            {
                return this.hwndControl;
            }
            set
            {
                this.hwndControl = value;
            }
        }

        public IEditorInterop EditorInterop;

        public Win32WndHost(double height, double width, IEditorInterop editorInterop)
        {
            EditorInterop = editorInterop;
            this.hostHeight = (int)height;
            this.hostWidth = (int)width;
        }

        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        internal static extern IntPtr CreateWindowEx(int dwExStyle, string lpszClassName, string lpszWindowName, int style, int x, int y, int width, int height, IntPtr hwndParent, IntPtr hMenu, IntPtr hInst, [MarshalAs(UnmanagedType.AsAny)] object pvParam);

      
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            int overlapedWindow = 13565952;
            this.hwndHost = Win32WndHost.CreateWindowEx(0, NeverTranslateClass.NeverTranslate("static"), 
                "", 1375731712, 0, 0, this.hostWidth, this.hostHeight, hwndParent.Handle, 
                (IntPtr)2, IntPtr.Zero, (object)0);


         

            return new HandleRef((object)this, this.hwndHost);
        }

        protected override IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
 
                handled = false;
            return IntPtr.Zero;
        }

        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        internal static extern bool DestroyWindow(IntPtr hwnd);

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            Win32WndHost.DestroyWindow(hwnd.Handle);
        }
    }
}
