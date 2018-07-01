using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces;

namespace R3D.IOTools
{
    public class R3DIOToolsPlugin : IRPlugin
    {
        public const string DocumentType = "ViewerCommon.RBuilderDocument";

        public void Connect(IApplicationData data)
        {
            data.RegisterTool("Intelligent_objects", DocumentType , data.GetIconPath("IntelligentObject.png"),
                typeof(ViewModels.IObjectBrowserViewModel), RDocumentData.ToolsDockPosition.Left);
        }

        public List<RDocumentData> GetDocumentData()
        {
            return null;
        }

        public string GetXAMLResourcePath()
        {
            return "pack://application:,,,/R3D.IOTools;component/Resources/ToolsResourceDictionary.xaml";
        }
    }
}
