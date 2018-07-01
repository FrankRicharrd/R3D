using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interactivity;
using Viewer.AdvancedTypes.Document;
using ViewerApp.ViewModels;
using ViewerCommon;

namespace ViewerApp.Behaviours
{
    public class AttachWin32CanvasBehavior : Behavior<Border>
    {
        public static readonly DependencyProperty FromMainWindowProperty = DependencyProperty.Register("FromMainWindow", typeof(bool), typeof(AttachWin32CanvasBehavior), new PropertyMetadata((object)false));
        private Border _attachedElement;
        private IntPtr hwndCanvas;
        private Win32WndHost canvasHostObject;
        public bool IsHostLoaded = false;

        private GraphicsDocumentPanelViewModel MainWindowVM
        {
            get
            {
                return this.AssociatedObject.DataContext as GraphicsDocumentPanelViewModel;
            }
        }

        public bool FromMainWindow
        {
            get
            {
                return (bool)this.GetValue(AttachWin32CanvasBehavior.FromMainWindowProperty);
            }
            set
            {
                if (value)

                    this.SetValue(AttachWin32CanvasBehavior.FromMainWindowProperty,(object)1);
                else
                    this.SetValue(AttachWin32CanvasBehavior.FromMainWindowProperty, (object)0);

            }
        }

        protected override void OnAttached()
        {
            this._attachedElement = this.AssociatedObject;
            this._attachedElement.Loaded += new RoutedEventHandler(this.CreateAndAttachWin32CanvasHost);
            this._attachedElement.SizeChanged += new SizeChangedEventHandler(this.AttachedElementSizeChanged);
        }

        private void AttachedElementSizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (!this._attachedElement.IsLoaded || !this.MainWindowVM.MainWindowHwndSet)
                return;
            Size newSize = e.NewSize;
            int num1 = (int)Math.Ceiling(newSize.Width);
            newSize = e.NewSize;
            int num2 = (int)Math.Ceiling(newSize.Height);
            if (num1 <= 0 || num2 <= 0)
                return;
            this.MainWindowVM.ResizeCanvasObject(num1, num2);
            if (!this.FromMainWindow)
                return;
            this.MainWindowVM.SetMainWindowCanvasHostSize(num1, num2);
        }

        private void CreateAndAttachWin32CanvasHost(object sender, RoutedEventArgs args)
        {
            if(IsHostLoaded)
                return;
            int width = (int)Math.Ceiling(this._attachedElement.ActualWidth);
            int height = (int)Math.Ceiling(this._attachedElement.ActualHeight);

            var activedoc = MainWindowVM.Rdoc as RDocument;

            this.canvasHostObject = new Win32WndHost((double)height, (double)width, activedoc.Editor);
            this._attachedElement.Child = (UIElement)this.canvasHostObject;
            this.MainWindowVM.CreateCanvasObject(this.canvasHostObject.hwndHost, width, height);
            this.hwndCanvas = this.MainWindowVM.GetCanvasObject();
            this.canvasHostObject.HwndControl = this.hwndCanvas;
            if (!this.FromMainWindow)
                return;
            this.MainWindowVM.SetMainWindowCanvasHost(this.canvasHostObject.hwndHost);
            this.MainWindowVM.SetMainWindowCanvasHostSize(width, height);
            this.IsHostLoaded = true;
        }
    }
}
