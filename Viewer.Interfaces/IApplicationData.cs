using Prism.Events;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viewer.Interfaces
{
    public interface IApplicationData
    {
        IRPluginManager PluginManager { get; set; }

        List<IRPlugin> Plugins { get; set; }


        IRDocument ActiveDocument { get; set; }

        string GetImportSupportFileTypeFilter();

        string GetIconPath(string iconName);

        void AddandActiveDocument(IRDocument doc, bool setactive);

        IUIManager UIManager { get; set; }

        Dictionary<string, Type> DocumentTypesLoaded { get; set; }

        IEventAggregator Eventaggregator { get; set; }

        bool RegisterTool(string ToolName, string documentType, string iconPath, Type ToolType, RDocumentData.ToolsDockPosition toolDockPosition);

        List<RDocumentData> DocumentTypeData { get; set; }


    }
}
