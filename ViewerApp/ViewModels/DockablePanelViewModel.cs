using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Prism.Mvvm;
using Viewer.Interfaces;
using ViewerApp.Dockable;
using System.Collections.ObjectModel;
using System.Windows.Input;
using System.Windows;
using System.Windows.Threading;
using Viewer.AdvancedTypes.DockingToolHelper;
using Viewer.Tools.ViewModels;
using ViewerCommon;
using Prism.Events;
using Viewer.Infrastructure;
using Viewer.Interfaces.UI;
using Viewer.AdvancedTypes.Document;
using ViewerApp.UIInterfaces;
using Xceed.Wpf.AvalonDock.Layout.Serialization;
using ViewerApp.CustomEvents;
using System.IO;
using System.ComponentModel;

namespace ViewerApp.ViewModels
{
    public class DockablePanelViewModel : BindableBase, ILayoutViewModelParent, IDockpanelContentViewmodelResolver
    {
        IApplicationData _data;

        Dictionary<string, List<ToolViewModel>> dicToolsLoadedforDocumentTypes = new Dictionary<string, List<ToolViewModel>>();
        ObservableCollection<DocumentViewModel> _documentCollection = new ObservableCollection<DocumentViewModel>();
        ObservableCollection<ToolViewModel> _dockableTools = new ObservableCollection<ToolViewModel>();

        /// <summary>
        /// Expose command to load/save AvalonDock layout on application startup and shut-down.
        /// </summary>
        public AvalonDockLayoutViewModel ADLayout { get; private set; }
        private bool mIsBusy;
        IEventAggregator _events;
        public DockablePanelViewModel(IApplicationData data, IEventAggregator events)
        {
            _events = events;
            Data = data;

            this.ADLayout = new AvalonDockLayoutViewModel(this, events);
            _events.GetEvent<ApplicationLoadedEvent>().Subscribe(OnApplicationLoaded);
            _events.GetEvent<NewDocumentAddedEvent>().Subscribe(OnNewDocumentCreated);
            _events.GetEvent<DockableToolToggleButtonClickedEvent>().Subscribe(OnDockableToolsToggleButtonClicked);




        }


        private void OnDockableToolsToggleButtonClicked(KeyValuePair<IRibbonToggleButton, Type> obj)
        {
            if (obj.Key.IsChecked == true)
            {
                ToolViewModel toolInstance = (ToolViewModel)Activator.CreateInstance(obj.Value);
                toolInstance.Connect(Data, _events);
                _dockableTools.Add(toolInstance);
                AddDocumentTools(toolInstance);

            }
            else if (obj.Key.IsChecked == false)
            {
                ToolViewModel tool = _dockableTools.Where(a => a.GetType() == obj.Value).Select(a => a).FirstOrDefault();
                if (tool != null)
                {
                    tool.Disconnect();
                    _dockableTools.Remove(tool);
                    RemoveDocumentTool(tool);
                }
            }
        }

        private void OnNewDocumentCreated(IRDocument obj)
        {
            //if (obj is RBuilderDocument)
            //{
            //    DocumentViewModel doc = new GraphicsDocumentPanelViewModel(_data, _events, (RBuilderDocument)obj);
            //    ActiveDocumentViewModel = doc;
            //    _documentCollection.Add(doc);
            //}


            CreateDocument(obj, true);

        }

        private void OnApplicationLoaded()
        {
            CreateAppropriateViewModels();
        }

        public void CreateDocument(IRDocument obj, bool setActive)
        {
            Type doctype = obj.GetType();

            RDocumentData docData = Data.DocumentTypeData.Where(a => a.DocType == doctype).Select(a => a).FirstOrDefault();

            if (docData != null)
            {
                object[] parms = new object[1];
                parms[0] = obj;
                DocumentViewModel doc = (DocumentViewModel)Activator.CreateInstance(docData.ViewModel, parms);
                _documentCollection.Add(doc);
                if (setActive)
                {
                    ActiveDocumentViewModel = doc;
                }
            }
        }

        public void AddDocumentTools(ToolViewModel tool)
        {
            string docType = Data.ActiveDocument.GetType().ToString();

            List<ToolViewModel> tools;

            if (dicToolsLoadedforDocumentTypes.TryGetValue(docType, out tools))
            {
                tools.Add(tool);
            }
            else
            {
                dicToolsLoadedforDocumentTypes.Add(docType, new List<ToolViewModel>() { tool });
            }

        }

        public void RemoveDocumentTool(ToolViewModel tool)
        {

            string docType = Data.ActiveDocument.GetType().ToString();

            List<ToolViewModel> tools;

            if (dicToolsLoadedforDocumentTypes.TryGetValue(docType, out tools))
            {
                tools.Remove(tool);
            }
        }

        public void CreateAppropriateViewModels()
        {
            CreateDocument(Data.ActiveDocument, true);


            //if (Data.ActiveDocument is RBuilderDocument)
            //{
            //    ActiveDocumentViewModel = new GraphicsDocumentPanelViewModel(_data, _events, (RBuilderDocument)Data.ActiveDocument);

            //    //List<IDockableTool> doc;

            //    //if (Data.UIManager.DockableTools.TryGetValue(Data.ActiveDocument.GetType().ToString(), out doc))
            //    //{

            //    //    DockableTools = new ObservableCollection<ToolViewModel>(Array.ConvertAll<IDockableTool, ToolViewModel>(doc.ToArray(),
            //    //    (input) =>
            //    //    {
            //    //        return input as ToolViewModel;
            //    //    }));

            //    //}
            //    _documentCollection.Add(ActiveDocumentViewModel);
            //}
        }

        /// <summary>
        /// Method is called via interface from <seealso cref="AvalonDockLayoutViewModel"/>
        /// when the application loads layout.
        /// </summary>
        /// <param name="args"></param>
        public void ReloadContentOnStartUp(LayoutSerializationCallbackEventArgs args)
        {
            //string sId = args.Model.ContentId;

            //// Empty Ids are invalid but possible if aaplication is closed with File>New without edits.
            //if (string.IsNullOrWhiteSpace(sId) == true)
            //{
            //    args.Cancel = true;
            //    return;
            //}

            //if (args.Model.ContentId == FileStatsViewModel.ToolContentId)
            //    args.Content = this.FileStats;
            //else
            //{
            //    args.Content = this.ReloadDocument(args.Model.ContentId);

            //    if (args.Content == null)
            //        args.Cancel = true;
            //}
        }

        public INotifyPropertyChanged ContentViewModelFromID(string content_id)
        {
            var anchorable_vm = this._dockableTools.FirstOrDefault(d => d.ContentId == content_id);
            if (anchorable_vm != null)
                return anchorable_vm;
            else
                return null;
        }


        /// <summary>
        /// Get/set property to signal whether application is busy or not.
        /// 
        /// The property set method invokes a CommandManager.InvalidateRequerySuggested()
        /// if the new value is different from the old one. The mouse cursor is set to Wait
        /// if the application is busy.
        /// </summary>
        public bool IsBusy
        {
            get
            {
                return this.mIsBusy;
            }

            set
            {
                if (this.mIsBusy != value)
                {
                    this.mIsBusy = value;
                    this.RaisePropertyChanged();

                    CommandManager.InvalidateRequerySuggested();

                    Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        if (value == false)
                            Mouse.OverrideCursor = null;
                        else
                            Mouse.OverrideCursor = Cursors.Wait;
                    }),
                      DispatcherPriority.Background);
                }
            }
        }

        #region ActiveDocument

        private DocumentViewModel _activeDocumentViewModel = null;
        public DocumentViewModel ActiveDocumentViewModel
        {
            get { return _activeDocumentViewModel; }
            set
            {
                if (_activeDocumentViewModel != value)
                {
                    _activeDocumentViewModel = value;
                    RaisePropertyChanged();
                }
            }
        }

        #endregion
        public IApplicationData Data
        {
            get
            {
                return _data;
            }

            set
            {
                _data = value;

                RaisePropertyChanged();
            }
        }



        public ObservableCollection<DocumentViewModel> DocumentCollection
        {
            get
            {


                return _documentCollection;
            }


        }


        public ObservableCollection<ToolViewModel> DockableTools
        {
            get
            {

                return _dockableTools;

            }

            set
            {
                _dockableTools = value;

                RaisePropertyChanged();
            }
        }

        public string DirAppData
        {
            get
            {
                string dirPath = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) +
                                                 System.IO.Path.DirectorySeparatorChar + ConstantNames.ApplicationName;

                try
                {
                    if (System.IO.Directory.Exists(dirPath) == false)
                        System.IO.Directory.CreateDirectory(dirPath);
                }
                catch
                {
                }

                return dirPath;
            }
        }
    }
}
