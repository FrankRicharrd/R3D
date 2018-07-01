using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer;
using Viewer.Interfaces;

namespace ViewerGraphicsEditor
{
    
   
    public interface IEditorInterop
    {
        IApplicationData data { get; set; }

        //AppData data { get; set; }

        /// <summary>
        /// Creates the canvas on the hwnd host and returns the parent transform node
        /// </summary>
        /// <param name="hwndHost"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        IRTransformNode CreateCanvasObject(IntPtr hwndHost, int width, int height);

        IntPtr GetCanvasObject();

        void ResizeCanvasObject(int newWidth, int newHeight);

        string TestMessage();

        void DestroyCanvas();

        void MouseMoveEvent(MouseButton btnType, double x, double y);
        void MousePressEvent(MouseButton btnType, double x, double y);
        void MouseReleaseEvent(MouseButton btnType, double x, double y);

        void MouseWheelEvent(int delta, double x, double y);

        void KeyPress(int key);

        void KeyRelease(int key);

        void SetView(FViewType view);

        IRNode ImportGeometry(string filePath);

        IRTransformNode CreateTransformNode(string name);

        void SetDraggerState(DraggerType dragger, bool isChecked);

        void SetGridVisibility(bool ischecked);

        void SetFocus(IRNode node);

        void DeleteNode(IRNode node);


        IRModelingManager GetModelingManager();

        IRMaterialManager GetMaterialManager();

        void SetSelectionType(SelectionType type);

        IRLightManager GetLightsManager();

      

    }
}
