using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Prism.Mvvm;
using System.Windows.Input;
using ViewerCommon;
using ViewerApp.ApplicationCommands.InsertMenu;
using Prism.Commands;
using Prism.Events;
using Viewer.Interfaces;
using Fluent;
using Viewer.Infrastructure;
using Viewer.Interfaces.UI;
using System.Windows.Controls;
using Viewer.Commands;

namespace ViewerApp.ViewModels
{
    public class RibbonPanelViewModel : BindableBase
    {

        Fluent.Ribbon _ribbonControl;

        public ICommand GeometryImportCommand { get; set; }

        public DelegateCommand TranslateCommand { get; set; }

        public DelegateCommand RotateCommand { get; set; }

        public DelegateCommand ScaleCommand { get; set; }

        public bool IsTranslateChecked
        {
            get
            {
                return _isTranslateChecked;
            }

            set
            {
                if (value == _isTranslateChecked)
                    return;


                _isTranslateChecked = value;
                RaisePropertyChanged();

                if (value)
                {
                    IsScaleChecked = false;
                    IsRotateChecked = false;
                }

            }
        }

        public bool IsRotateChecked
        {
            get
            {
                return _isRotateChecked;
            }

            set
            {
                if (value == _isRotateChecked)
                    return;

                _isRotateChecked = value;
                RaisePropertyChanged();

                if (value)
                {
                    IsScaleChecked = false;
                    IsTranslateChecked = false;
                }
            }
        }

        public bool IsScaleChecked
        {
            get
            {
                return _isScaleChecked;
            }

            set
            {
                if (value == _isScaleChecked)
                    return;

                _isScaleChecked = value;
                RaisePropertyChanged();

                if (value)
                {
                    IsTranslateChecked = false;
                    IsRotateChecked = false;
                }
            }
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

        public Ribbon RibbonControl
        {
            get
            {
                return _ribbonControl;
            }

            set
            {
                _ribbonControl = value;

                if (Data.ActiveDocument != null)
                {
                    AddUITabsFromDoc(Data.ActiveDocument);
                    _currentDoctype = Data.ActiveDocument.GetType().ToString();

                }
                AddApplicationMenuitems();
            }
        }

        bool _isTranslateChecked;
        bool _isRotateChecked;
        bool _isScaleChecked;

        string _currentDoctype;

        IApplicationData _data;
        // IEditorInterop _editor;
        IEventAggregator _eventAggregator;
        public RibbonPanelViewModel(IEventAggregator eventAggregator, IApplicationData data)
        {
            Data = data;
            _eventAggregator = eventAggregator;
            //_editor = editor;
            //GeometryImportCommand = new GeometryImportCommandHandler(_editor, _eventAggregator);

            TranslateCommand = new DelegateCommand(TranslateCommandhandler);
            RotateCommand = new DelegateCommand(RotateCommandhandler);
            ScaleCommand = new DelegateCommand(ScaleCommandhandler);

            //RibbonControl.Background = 

            //if (data.ActiveDocument != null)
            //{
            //    AddUITabsFromDoc(data.ActiveDocument);
            //}


            eventAggregator.GetEvent<ActiveIDocumentChangedEvent>().Subscribe(OnActiveDocumentChanged);
            eventAggregator.GetEvent<ApplicationLoadedEvent>().Subscribe(OnApplicationLoaded);
            if (data.ActiveDocument == null)
                return;


        }

        private void OnApplicationLoaded()
        {
            if (Data.ActiveDocument != null)
                AddUITabsFromDoc(Data.ActiveDocument);
        }

        private void OnActiveDocumentChanged(IRDocument obj)
        {
            AddUITabsFromDoc(obj);
        }

        private void AddApplicationMenuitems()
        {

            if (RibbonControl == null)
                return;

            AppData apData = _data as AppData;


            Grid menu = RibbonControl.Menu as Grid;

            var applicationMenu = menu.Children[0] as Fluent.ApplicationMenu;

            foreach (var item in applicationMenu.Items)
            {
                if (item is Fluent.MenuItem)
                {
                    var newdoc = item as Fluent.MenuItem;

                    if ("New" == (newdoc?.Header as string) )
                    {
                        foreach (var docdata in apData.DocumentTypeData)
                        {
                            var newMenuItem = new Fluent.MenuItem();

                            newMenuItem.Header = docdata.TypeName;

                            var command = new NewDocumentButtonClick(docdata.DocType, _data);

                            newMenuItem.Command = command;

                            newMenuItem.Icon = docdata.IconPath;

                            newdoc.Items.Add(newMenuItem);
                        }
                    }

                }
            }



        }

        private void OnNewMenuButtonClicked(Type obj)
        {
            throw new NotImplementedException();
        }

        private void AddUITabsFromDoc(IRDocument doc)
        {
            if (RibbonControl == null)
                return;

            if (string.IsNullOrEmpty(_currentDoctype) || _currentDoctype != doc.GetType().ToString())
            {
                RemoveAllExistingTabs();
                List<ITab> lstTabs;

                if (Data.UIManager.RibbonTabs.TryGetValue(doc.GetType().ToString(), out lstTabs))
                {
                    foreach (var item in lstTabs)
                    {
                        _ribbonControl.Tabs.Add(item as RibbonTabItem);
                    }
                    _currentDoctype = doc.GetType().ToString();
                }
            }
        }

        private void RemoveAllExistingTabs()
        {
            if (RibbonControl == null)
                return;

            for (int i = 0; i < RibbonControl.Tabs.Count; i++)
            {
                _ribbonControl.Tabs.RemoveAt(i);
            }
        }


        private void ScaleCommandhandler()
        {
            //_editor.SetDraggerState(DraggerType.TabBoxDragger, IsScaleChecked);

        }

        private void RotateCommandhandler()
        {
            //_editor.SetDraggerState(DraggerType.TrackballDragger, IsRotateChecked);

        }

        private void TranslateCommandhandler()
        {
            //_editor.SetDraggerState(DraggerType.TranslateAxisDragger, IsTranslateChecked);
        }
    }
}
