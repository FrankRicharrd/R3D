using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces;
using Viewer.Interfaces.UI;

namespace ViewerCommon.UI
{
    public class RUIManager : IUIManager
    {
        private IApplicationData _data;
        private Dictionary<string, List<ITab>> _ribbonTabs = new Dictionary<string, List<ITab>>();
        private Dictionary<string, List<IDockableDocument>> _dockableDocuments = new Dictionary<string, List<IDockableDocument>>();
        private Dictionary<string, List<IDockableTool>> _dockableTools = new Dictionary<string, List<IDockableTool>>();

        public RUIManager(IApplicationData _data)
        {
            this.Data = _data;
        }

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




        public ITab CreateNewTab(string typename, string name)
        {
            var tab = new RTab(name);

            List<ITab> tabs;

            if (RibbonTabs.TryGetValue(typename, out tabs))
            {
                tabs.Add(tab);
            }
            else
            {
                RibbonTabs.Add(typename, new List<ITab>() { tab });
            }
            return tab;
        }

        public ITab GetTab(string typeName, string tabName)
        {
            List<ITab> tabs;

            if (RibbonTabs.TryGetValue(typeName, out tabs))
            {
                return tabs?.Where(a => a.TabName == tabName).Select(a => a).FirstOrDefault();
            }
            return null;
        }


        /// <summary>
        /// type of document as key and its respective ribbon tabs as values
        /// For key use syntax: obj.GetType().ToString();
        /// To Add a tool use AddRibbonTabs
        /// </summary>
        public Dictionary<string, List<ITab>> RibbonTabs
        {
            get
            {
                return _ribbonTabs;
            }

            set
            {
                _ribbonTabs = value;
            }
        }

        /// <summary>
        /// type of document as key and their respective dockable documents as values
        /// For key use syntax: obj.GetType().ToString();
        /// To Add a tool use AddDockableDocument
        /// </summary>
        public Dictionary<string, List<IDockableDocument>> DockableDocuments
        {
            get
            {
                return _dockableDocuments;
            }

            set
            {
                _dockableDocuments = value;
            }
        }

        /// <summary>
        /// type of document as key ans their respective dockable tools as values.
        /// For key use syntax: obj.GetType().ToString();
        /// To Add a tool use AddDockableTool
        /// </summary>
        public Dictionary<string, List<IDockableTool>> DockableTools
        {
            get
            {
                return _dockableTools;
            }

            set
            {
                _dockableTools = value;
            }
        }






    }
}
