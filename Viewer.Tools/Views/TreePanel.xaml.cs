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
using Viewer.Tools.ViewModels;
using Xceed;
namespace Viewer.Tools.Views
{
    /// <summary>
    /// Interaction logic for TreeView.xaml
    /// </summary>
    public partial class TreePanel : UserControl
    {
        public TreePanel()
        {
            InitializeComponent();
        }

        private void TreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            var vm = this.DataContext as ViewModels.TreepanelViewModel;

            var control = sender as TreeView;

            var sel = control.SelectedItem as UI_RNode;

            vm.TreeViewSelectionChanged(sel.ThisItem);

        }
    }
}
