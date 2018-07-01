using Prism.Events;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Viewer.AdvancedTypes.Document;
using Viewer.Infrastructure;
using Viewer.Interfaces;
using Viewer.Interfaces.UI;
using Viewer.Tools;
using Viewer.Tools.ViewModels;
using ViewerApp.ViewModels;
using ViewerCommon;
using ViewerCommon.UI;

namespace ViewerApp
{
    public class AppData : IApplicationData
    {
        List<RDocumentData> _documentTypeData = new List<RDocumentData>();

        List<IRPlugin> _plugins = new List<IRPlugin>();

        IUIManager _uiManager;

        RPluginManager _pluginManager;

        List<IRDocument> _documents = new List<IRDocument>();

        IRDocument _activeDocument;

        List<string> _importSupportedFileFormats;

        IEventAggregator _eventaggregator;

        Dictionary<string, Type> _documentTypesLoaded = new Dictionary<string, Type>();

        Dictionary<string, List<Tuple<string, string, string, Type, RDocumentData.ToolsDockPosition>>> _dicDocSpecificToolFromPluginData = new Dictionary<string, List<Tuple<string, string, string, Type, RDocumentData.ToolsDockPosition>>>();


        public AppData(IEventAggregator eventaggregator)
        {
            Eventaggregator = eventaggregator;
            PluginManager = new RPluginManager(this);


            _importSupportedFileFormats = new List<string>();
            _importSupportedFileFormats.Add("osg");
            _importSupportedFileFormats.Add("obj");
            _importSupportedFileFormats.Add("3ds");
            _importSupportedFileFormats.Add("dae");



            _uiManager = new RUIManager(this);


            _documentTypeData.Add(new RBuilderDocumentData("RBuilderDocument", typeof(RBuilderDocument), GetIconPath(ConstantNames.RBuilderDocIconName), typeof(GraphicsDocumentPanelViewModel)));

            _pluginManager.LoadExternalPlugins();

            AddandActiveDocument(new RBuilderDocument(this), true);
        }



        public void AddandActiveDocument(IRDocument doc, bool setactive)
        {
            string typeName = doc.GetType().ToString();
            Type docTy;
            if (!_documentTypesLoaded.TryGetValue(typeName, out docTy))
            {

                _documentTypesLoaded.Add(typeName, doc.GetType());
                doc.CreateDocumentSpecificRibbonTabs();
                doc.CreateDocumentSpecificTools();
                LoadExternalToolsFromPlugin(typeName);
            }

            _documents.Add(doc);

            Eventaggregator.GetEvent<NewDocumentAddedEvent>().Publish(doc);

            if (setactive)
            {
                ActiveDocument = doc;
            }
        }


        public void AddImportFileType(string fileformat)
        {
            if (string.IsNullOrEmpty(fileformat))
                return;

            _importSupportedFileFormats.Add(fileformat);
        }

        public string GetImportSupportFileTypeFilter()
        {
            string outstr = string.Empty;

            foreach (var item in _importSupportedFileFormats)
            {
                string tempstr = "*." + item + ";";

                outstr = outstr + tempstr;
            }

            return outstr;
        }

        public List<string> ImportSupportedFileFormats
        {
            get
            {
                return _importSupportedFileFormats;
            }

            set
            {
                _importSupportedFileFormats = value;
            }
        }

        public IRDocument ActiveDocument
        {
            get
            {
                return _activeDocument;
            }

            set
            {
                _activeDocument = value;

                Eventaggregator.GetEvent<ActiveIDocumentChangedEvent>().Publish(_activeDocument);
            }
        }


        public string GetIconPath(string iconName)
        {
            string iconpath = Path.Combine(AssemblyDirectory, ConstantNames.IconFolderName, iconName);

            if (File.Exists(iconpath))
            {
                return iconpath;
            }
            else
            {
                return Path.Combine(AssemblyDirectory, ConstantNames.IconFolderName, "home.png");
            }
        }

        private void LoadExternalToolsFromPlugin(string documentType)
        {
            List<Tuple<string, string, string, Type, RDocumentData.ToolsDockPosition>> val;

            if (_dicDocSpecificToolFromPluginData.TryGetValue(documentType, out val))
            {
                foreach (var item in val)
                {
                    RegisterTool(item.Item1, item.Item2, item.Item3, item.Item4, item.Item5);
                }
            }
            else
            {
                return;
            }
        }
        
        public bool RegisterTool(string ToolName, string documentType, string iconPath, Type ToolType, RDocumentData.ToolsDockPosition toolDockPosition)
        {
            try
            {
                Type ty;

                if(!DocumentTypesLoaded.TryGetValue(documentType, out ty))
                {
                    List<Tuple<string, string, string, Type, RDocumentData.ToolsDockPosition>> val;

                    if(_dicDocSpecificToolFromPluginData.TryGetValue(documentType, out val))
                    {
                        val.Add(new Tuple<string, string, string, Type, RDocumentData.ToolsDockPosition>(ToolName, documentType, iconPath, ToolType, toolDockPosition));
                    }
                    else
                    {
                        _dicDocSpecificToolFromPluginData.Add(documentType, new List<Tuple<string, string, string, Type, RDocumentData.ToolsDockPosition>>()
                        {
                            new Tuple<string, string, string, Type, RDocumentData.ToolsDockPosition>(ToolName, documentType, iconPath, ToolType, toolDockPosition),
                        });
                    }
                }
                else
                {

                    ITab tab = UIManager.GetTab(documentType, "Tools");
                    IButtonGroup groupbox = tab?.GetButtonGroup("DockingTools");

                    if (tab == null)
                    {
                        tab = UIManager.CreateNewTab(documentType, "Tools");
                        groupbox = tab.CreateButtonGroup("DockingTools");
                    }
                    if (groupbox == null)
                    {
                        groupbox = tab.CreateButtonGroup("DockingTools");
                    }

                    groupbox.CreateToggleButton(ToolName, iconPath, new RelayCommand<RRibbonToggleButton>(
                        (togglebtn) =>
                        {

                            try
                            {
                                _eventaggregator.GetEvent<DockableToolToggleButtonClickedEvent>().Publish(new KeyValuePair<IRibbonToggleButton, Type>(togglebtn, ToolType));


                            }
                            catch (Exception ex)
                            {

                                
                            }

                        }));


                    Type doc = this.DocumentTypesLoaded[documentType];
                    //if (doc == null)
                    //    return false;
                    var docdata = this.DocumentTypeData.FirstOrDefault(a => a.DocType == doc);

                    docdata.DicToolDockPosition.Add(ToolType, toolDockPosition);
                }

                return true;
            }
            catch (Exception ex)
            {

                return false;
            }
        }

        public string AssemblyDirectory
        {
            get
            {
                string codeBase = Assembly.GetExecutingAssembly().CodeBase;
                UriBuilder uri = new UriBuilder(codeBase);
                string path = Uri.UnescapeDataString(uri.Path);
                return Path.GetDirectoryName(path);
            }
        }

        public IEnumerable<IRDocument> Documents
        {
            get
            {
                return _documents;
            }
        }

        public Dictionary<string, Type> DocumentTypesLoaded
        {
            get
            {
                return _documentTypesLoaded;
            }

            set
            {
                _documentTypesLoaded = value;
            }
        }

        public IUIManager UIManager
        {
            get
            {
                return _uiManager;
            }

            set
            {
                _uiManager = value;
            }
        }

        public List<IRPlugin> Plugins
        {
            get
            {
                return _plugins;
            }

            set
            {

                _plugins = value;
            }
        }

        public IEventAggregator Eventaggregator
        {
            get
            {
                return _eventaggregator;
            }

            set
            {
                _eventaggregator = value;
            }
        }

        public List<RDocumentData> DocumentTypeData
        {
            get
            {
                return _documentTypeData;
            }

            set
            {
                _documentTypeData = value;
            }
        }

        public IRPluginManager PluginManager
        {
            get
            {
                return _pluginManager;
            }

            set
            {
                _pluginManager = (RPluginManager)value;
            }
        }
    }
}
