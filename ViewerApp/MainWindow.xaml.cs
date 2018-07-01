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
using Fluent;
using MahApps.Metro.Controls;
using Viewer.Infrastructure;
using Prism.Events;

namespace ViewerApp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    [CLSCompliant(false)]
    public partial class MainWindow : IRibbonWindow
    {

        public MainWindowViewModel ViewModel
        {
            get
            {
                return this.DataContext as MainWindowViewModel;
            }
            set
            {
                this.DataContext = (object)value;
            }
        }

        #region TitelBar

        /// <summary>
        /// Gets ribbon titlebar
        /// </summary>
        public RibbonTitleBar TitleBar
        {
            get { return (RibbonTitleBar)this.GetValue(TitleBarProperty); }
            private set { this.SetValue(titleBarPropertyKey, value); }
        }

        private static readonly DependencyPropertyKey titleBarPropertyKey = DependencyProperty.RegisterReadOnly(nameof(TitleBar), typeof(RibbonTitleBar), typeof(MainWindow), new PropertyMetadata());

        /// <summary>
        /// <see cref="DependencyProperty"/> for <see cref="TitleBar"/>.
        /// </summary>
        public static readonly DependencyProperty TitleBarProperty = titleBarPropertyKey.DependencyProperty;

        #endregion

        

        public MainWindow()
        {

            InitializeComponent();

        }

        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {

            var mainVm = this.DataContext as MainWindowViewModel;

            mainVm.EventAggregator.GetEvent<ApplicationLoadedEvent>().Publish();

            //this.TitleBar = this.FindChild<RibbonTitleBar>("RibbonTitleBar");
            //this.TitleBar.InvalidateArrange();
            //this.TitleBar.UpdateLayout();
        }

        private void MetroWindow_KeyDown(object sender, KeyEventArgs e)
        {
            ViewModel.OnKeyDown((int)e.Key);
        }

        private void MetroWindow_KeyUp(object sender, KeyEventArgs e)
        {
            ViewModel.OnKeyUp((int)e.Key);
        }

        //private void UIPanel_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        //{

        //    if (e.LeftButton != MouseButtonState.Pressed)
        //        return;
        //    Point pt = e.GetPosition((IInputElement)this);

        //    ViewModel.EditorInterop.MousePressEvent(MouseButton.Left, pt.X, pt.Y);
        //}

        //private void UIPanel_PreviewMouseMove(object sender, MouseEventArgs e)
        //{
        //    if (e.LeftButton != MouseButtonState.Pressed)
        //        return;
        //    Point pt = e.GetPosition((IInputElement)this);

        //    ViewModel.EditorInterop.MouseMoveEvent(MouseButton.Left, pt.X, pt.Y);
        //}

        //private void UIPanel_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        //{
        //    if (e.LeftButton != MouseButtonState.Pressed)
        //        return;
        //    Point pt = e.GetPosition((IInputElement)this);

        //    ViewModel.EditorInterop.MouseReleaseEvent(MouseButton.Left, pt.X, pt.Y);
        //}

        //private void UIPanel_PreviewMouseWheel(object sender, MouseWheelEventArgs e)
        //{
        //    ViewModel.EditorInterop.MouseWheelEvent(e.Delta);


        //}

    }
}
