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

namespace Viewer.Tools.Views
{
    /// <summary>
    /// Interaction logic for ShaderEditorPanel.xaml
    /// </summary>
    public partial class ShaderEditorPanel : UserControl
    {
        public ShaderEditorPanel()
        {
            InitializeComponent();

            this.Loaded += ShaderEditorPanel_Loaded;
        }

        private void ShaderEditorPanel_Loaded(object sender, RoutedEventArgs e)
        {
            var ss = (this.VisualParent as ContentPresenter);
        }
    }
}
