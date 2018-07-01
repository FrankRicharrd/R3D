using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Microsoft.Practices.Unity;
using Prism.Modularity;
using Prism.Mvvm;
using Prism.Unity;
using Viewer.Interfaces;
using ViewerCommon;

namespace ViewerApp
{
    public class MyBootStrapper : UnityBootstrapper
    {
        protected override DependencyObject CreateShell()
        {
            try
            {
               

                Container.RegisterType<IApplicationData, AppData>(new ContainerControlledLifetimeManager());

                return Container.Resolve<MainWindow>();
            }
            catch (Exception ex)
            {

                Logging.WriteErrorLog(ex);

                return null;
            }
        }

        protected override void InitializeShell()
        {
            try
            {
                base.InitializeShell();

                Application.Current.MainWindow = (Window)Shell;
                Application.Current.MainWindow.ContentRendered += (s, a) => Application.Current.MainWindow.WindowState = WindowState.Maximized;
                Application.Current.MainWindow.Show();


            }
            catch (Exception ex)
            {

                Logging.WriteErrorLog(ex);

               
            }

        }

        protected override IModuleCatalog CreateModuleCatalog()
        {
            try
            {
                var catalog = new ModuleCatalog();
                catalog.AddModule(typeof(MainModule));

                return catalog;
            }
            catch (Exception ex)
            {

                Logging.WriteErrorLog(ex);

                return null;
            }
        }

        protected override void ConfigureViewModelLocator()
        {
            try
            {
                base.ConfigureViewModelLocator();

                ViewModelLocationProvider.SetDefaultViewTypeToViewModelTypeResolver(viewType =>
                {
                    var viewName = viewType.FullName;
                    viewName = viewName.Replace(".Views.", ".ViewModels.");
                    var viewAssemblyName = viewType.GetTypeInfo().Assembly.FullName;
                    var suffix = viewName.EndsWith("View") ? "Model" : "ViewModel";
                    var viewModelName = String.Format(CultureInfo.InvariantCulture, "{0}{1}", viewName, suffix);

                    var assembly = viewType.GetTypeInfo().Assembly;
                    var type = assembly.GetType(viewModelName, true);

                    return type;
                });
            }
            catch (Exception ex)
            {

                Logging.WriteErrorLog(ex);

               
            }
        }

        //protected override void ConfigureModuleCatalog()
        //{
        //    Type ModuleItemsType = typeof(MainModule);
        //    ModuleCatalog.AddModule(new ModuleInfo()
        //    {
        //        ModuleName = ModuleItemsType.Name,
        //        ModuleType = ModuleItemsType.AssemblyQualifiedName,
        //        InitializationMode = InitializationMode.OnDemand
        //    });

        //}
    }
}
