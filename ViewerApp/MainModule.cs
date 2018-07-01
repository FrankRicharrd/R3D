using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Practices.Unity;
using Prism.Regions;
using Viewer.Infrastructure;
//using Viewer.Infrastructure.Prism;
using ViewerApp.Views;
using ViewerCommon;
using Prism.Unity;
using Viewer.Infrastructure.Prism;

namespace ViewerApp
{
    public class MainModule : ModuleBase
    {
        public MainModule(IUnityContainer container, IRegionManager regionManager)
            : base(container, regionManager)
        {


        }

        protected override void RegisterTypes()
        {
            //Register all type views
          

            //Container.RegisterTypeForNavigation<DockablePanel>(NavigationPaths.MainModuleNavigationpath);
        }

        protected override void ResolveRegions()
        {
            // register all regions associated with the main module here
           // Container.RegisterType<IEditorInterop, ViewerInterop.ViewerEditor>(new ContainerControlledLifetimeManager());
            //RegionManager.Regions[RegionNames.ModuleRegion].Add(Container.Resolve<DockablePanel>());


           // RegionManager.RegisterViewWithRegion(RegionNames.DocumentRegion, typeof(GraphicsDocumentPanel));
           // RegionManager.RegisterViewWithRegion(RegionNames.TreeRegion, typeof(TreePanel));
           // RegionManager.RegisterViewWithRegion(RegionNames.PropertiesRegion, typeof(PropertiesPanel));
        }
    }
}
