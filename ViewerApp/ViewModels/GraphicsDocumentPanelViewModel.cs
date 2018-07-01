using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Prism.Mvvm;
using ViewerCommon;
using Prism.Events;
using Viewer.Infrastructure;
using Viewer.Interfaces;
using Viewer.AdvancedTypes.DockingToolHelper;
using Viewer.AdvancedTypes.Document;
using ViewerGraphicsEditor;

namespace ViewerApp.ViewModels
{
    public class GraphicsDocumentPanelViewModel : DocumentViewModel
    {

        public bool MainWindowHwndSet { get; internal set; }

        public IApplicationData Data
        {
            get
            {
                return _data;
            }

            set
            {
                _data = value;
            }
        }

        public RBuilderDocument Rdoc
        {
            get
            {
                return _rdoc;
            }

            set
            {
                _rdoc = value;
            }
        }

        public IEditorInterop EditorInterop
        {
            get
            {
                return _editorInterop;
            }

            set
            {
                _editorInterop = value;
            }
        }

        private IntPtr _MainWindowCanvasHost;

        private int _MainWindowCanvasHostWidth;
        private int _MainWindowCanvasHostHeight;

        IApplicationData _data;

        IEventAggregator _eventAggregator;

        RBuilderDocument _rdoc;

        IEditorInterop _editorInterop;

        IEventAggregator _events;

        public GraphicsDocumentPanelViewModel(RBuilderDocument rdoc) : base(rdoc)
        {
            _events = rdoc.Data.Eventaggregator;
            Rdoc = rdoc;
            EditorInterop = Rdoc.Editor;

            //string message = this.EditorInterop.TestMessage();

            _events.GetEvent<OnKeyPress>().Subscribe(OnKeyPressCallBack);
            _events.GetEvent<OnKeyRelease>().Subscribe(OnKeyReleaseCallBack);

        }

        private void OnKeyReleaseCallBack(int obj)
        {
            EditorInterop.KeyRelease(obj);
        }

        private void OnKeyPressCallBack(int obj)
        {
            EditorInterop.KeyPress(obj);
        }

        public override bool Connect(IApplicationData data, IEventAggregator events)
        {
            Data = data;
            _eventAggregator = events;

            return true;
        }


        public void ResizeCanvasObject(int newWidth, int newHeight)
        {
            if (this.EditorInterop == null)
                return;
            this.EditorInterop.ResizeCanvasObject(newWidth, newHeight);
        }


        public void SetMainWindowCanvasHostSize(int width, int height)
        {
            this._MainWindowCanvasHostWidth = width;
            this._MainWindowCanvasHostHeight = height;
        }

        public void CreateCanvasObject(IntPtr hwndHost, int width, int height)
        {
           var parentnode = this.EditorInterop.CreateCanvasObject(hwndHost, width, height);

            Rdoc.Tree.GeometryParentNode = parentnode;

            Rdoc.Data.Eventaggregator.GetEvent<OnContextCreatedEvent>().Publish(parentnode);
        }

        public IntPtr GetCanvasObject()
        {
            if (this.EditorInterop == null)
                return (IntPtr)0;
            return this.EditorInterop.GetCanvasObject();
        }

        public void SetMainWindowCanvasHost(IntPtr hwndHostMainWindow)
        {
            this._MainWindowCanvasHost = hwndHostMainWindow;
        }

    }
}
