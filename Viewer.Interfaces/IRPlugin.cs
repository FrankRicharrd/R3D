using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viewer.Interfaces
{
    public class RDocumentData
    {

        public enum ToolsDockPosition
        {
            Top,
            Bottom,
            Left,
            Right
        }

        public RDocumentData(string typeName, Type docType, string iconPath, Type viewModel)
        {
            TypeName = typeName;
            DocType = docType;
            IconPath = iconPath;
            ViewModel = viewModel;
            DicToolDockPosition = new Dictionary<Type, ToolsDockPosition>();
        }

        /// <summary>
        /// Tool ViewModel Type vs their position
        /// </summary>
        public Dictionary<Type, ToolsDockPosition> DicToolDockPosition {get;set;}

        public string TypeName { get; set; }
        public Type DocType { get; set; }
        public string IconPath { get; set; }

        public Type ViewModel { get; set; }
    }



    public class RPluginAttribute : Attribute
    {


    }

    public interface IRPlugin
    {
        void Connect(IApplicationData data);

        string GetXAMLResourcePath();

        List<RDocumentData> GetDocumentData();


    }


}
