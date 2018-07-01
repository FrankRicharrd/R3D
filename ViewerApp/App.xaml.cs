using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace ViewerApp
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>


    public partial class App : Application
    {
        //[STAThread]
        //public static void Main(string[] args)
        //{
        //    MainWindow mainWindow = new MainWindow();



        //   // Application.Current.MainWindow = (Window)mainWindow;

        //    mainWindow.ShowDialog();

        //}

        protected override void OnStartup(StartupEventArgs e)
        {
            try
            {
                base.OnStartup(e);

                var bs = new MyBootStrapper();
                bs.Run();
            }
            catch (Exception ex)
            {
                Logging.WriteErrorLog(ex);
            }
        }
    }

}
