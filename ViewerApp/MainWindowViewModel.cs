using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Microsoft.Practices;
using Microsoft.Practices.Unity;
using Prism.Commands;
using Prism.Events;
using Prism.Mvvm;
using Prism.Regions;
using Viewer.Infrastructure;
using ViewerApp.Views;
using ViewerCommon;
using Viewer.Interfaces;

namespace ViewerApp
{
    public class MainWindowViewModel : BindableBase
    {

        IRegionManager _regionManager;

        private IUnityContainer _container;
        public DelegateCommand<String> NavigateCommand { get; set; }
        public DelegateCommand ExitCommand { get; private set; }

        public IEventAggregator EventAggregator
        {
            get
            {
                return _eventAggregator;
            }

            set
            {
                _eventAggregator = value;
            }
        }

        IApplicationData _data;

        IEventAggregator _eventAggregator;

        public MainWindowViewModel(IUnityContainer container,  IRegionManager regionManager, IEventAggregator eventAggregator, IApplicationData data)
        {
            _data = data;
            _eventAggregator = eventAggregator;
            _regionManager = regionManager;
            _container = container;
            NavigateCommand = new DelegateCommand<String>(Navigate);
            ExitCommand = new DelegateCommand(Exit);

            //container.RegisterType<IEditorInterop, ViewerInterop.ViewerEditor>(new ContainerControlledLifetimeManager());

            

            Commands.NavigateCommand.RegisterCommand(NavigateCommand);

            //eventAggregator.GetEvent<ViewActivateEvent>().Subscribe(ViewActivated);
           // eventAggregator.GetEvent<ViewItemsCountChangedEvent>().Subscribe(ViewItemsCountChanged);

            regionManager.RegisterViewWithRegion(RegionNames.MainRibbonRegion, typeof (RibbonPanel));
            regionManager.RegisterViewWithRegion(RegionNames.StatusBarRegion, typeof(StatusBar));
            regionManager.RegisterViewWithRegion(RegionNames.ModuleRegion, typeof(DockablePanel));
            regionManager.RegisterViewWithRegion(RegionNames.ToolBarRegion, typeof(ToolBarPanel));




        }


        public void OnKeyDown(int k)
        {
            _eventAggregator.GetEvent<OnKeyPress>().Publish(k);
        }

        public void OnKeyUp(int k)
        {
            _eventAggregator.GetEvent<OnKeyRelease>().Publish(k);
        }

        private void Exit()
        {
            System.Windows.Application.Current.Shutdown();
        }

        private void Navigate(string navigationPath)
        {
            if (string.IsNullOrWhiteSpace(navigationPath))
                return;

            if (!String.IsNullOrWhiteSpace(navigationPath))
                _regionManager.RequestNavigate(RegionNames.DocumentRegion , navigationPath);
        }

    }
}
