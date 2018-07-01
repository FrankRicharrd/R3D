using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces.UI;

namespace Viewer.Interfaces
{
    public interface IUIManager
    {
        ITab CreateNewTab(string typeName,string name);

        ITab GetTab(string typeName, string tabName);

        IApplicationData Data { get; set; }

        /// <summary>
        /// type of document as key and its respective ribbon tabs as values
        /// For key use syntax: obj.GetType().ToString();
        /// 
        /// </summary>
        Dictionary<string, List<ITab>> RibbonTabs { get; set; }

        /// <summary>
        /// type of document as key and their respective dockable documents as values
        /// For key use syntax: obj.GetType().ToString();
        /// </summary>
        Dictionary<string, List<IDockableDocument>> DockableDocuments { get; set; }

        /// <summary>
        /// type of document as key and their respective dockable tools as values.
        /// For key use syntax: obj.GetType().ToString();
        /// </summary>
        //Dictionary<string, List<IDockableTool>> DockableTools { get; set; }
    }
}
