using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ViewerApp.ViewModels;

namespace ViewerApp.Views
{
    /// <summary>
    /// Interaction logic for RibbonPanel.xaml
    /// </summary>
    public partial class RibbonPanel : UserControl
    {
        public RibbonPanel()
        {
            InitializeComponent();
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            var vm = this.DataContext as RibbonPanelViewModel;
            if(vm!=null)
            {
                vm.RibbonControl = this.ribbon;
                
            }
        }
    }
}
