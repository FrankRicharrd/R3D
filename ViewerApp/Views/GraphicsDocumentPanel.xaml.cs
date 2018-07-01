using Prism.Events;
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
    /// Interaction logic for MainDocumentPanel.xaml
    /// </summary>
    public partial class GraphicsDocumentPanel : UserControl
    {

        public GraphicsDocumentPanelViewModel ViewModel { get; set; }
        public GraphicsDocumentPanel()
        {
            InitializeComponent();





        }



        private void Documentpanel_OnPreviewMouseDown(object sender, MouseButtonEventArgs e)
        {

            //if (e.LeftButton != MouseButtonState.Pressed)
            //    return;
            Point pt = e.GetPosition((IInputElement)this);

            if (e.ChangedButton == MouseButton.Left)
                ViewModel.EditorInterop.MousePressEvent(Viewer.MouseButton.Left, pt.X, pt.Y);
            else if(e.ChangedButton == MouseButton.Middle)
                ViewModel.EditorInterop.MousePressEvent(Viewer.MouseButton.Middle, pt.X, pt.Y);
            else if(e.ChangedButton == MouseButton.Right)
                ViewModel.EditorInterop.MousePressEvent(Viewer.MouseButton.Right, pt.X, pt.Y);
        }

        private void Documentpanel_OnPreviewMouseMove(object sender, MouseEventArgs e)
        {
            //if (e.LeftButton != MouseButtonState.Pressed)
            //    return;
            Point pt = e.GetPosition((IInputElement)this);

           // if (e.LeftButton == MouseButtonState.Pressed)
                ViewModel.EditorInterop.MouseMoveEvent(Viewer.MouseButton.Left, pt.X, pt.Y);
            ////else if (e.MiddleButton == MouseButtonState.Pressed)
            //    ViewModel.EditorInterop.MouseMoveEvent(Viewer.MouseButton.Middle, pt.X, pt.Y);
            ////else if (e.RightButton == MouseButtonState.Pressed)
            //    ViewModel.EditorInterop.MouseMoveEvent(Viewer.MouseButton.Right, pt.X, pt.Y);
        }

        private void Documentpanel_OnPreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            //if (e.LeftButton != MouseButtonState.Released)
            //    return;
            Point pt = e.GetPosition((IInputElement)this);

            if (e.ChangedButton == MouseButton.Left)
                ViewModel.EditorInterop.MouseReleaseEvent(Viewer.MouseButton.Left, pt.X, pt.Y);
            else if (e.ChangedButton == MouseButton.Middle)
                ViewModel.EditorInterop.MouseReleaseEvent(Viewer.MouseButton.Middle, pt.X, pt.Y);
            else if (e.ChangedButton == MouseButton.Right)
                ViewModel.EditorInterop.MouseReleaseEvent(Viewer.MouseButton.Right, pt.X, pt.Y);
        }

        private void Documentpanel_OnPreviewMouseWheel(object sender, MouseWheelEventArgs e)
        {
            Point pt = e.GetPosition((IInputElement)this);

            ViewModel.EditorInterop.MouseWheelEvent(e.Delta, pt.X, pt.Y);
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            ViewModel = this.DataContext as GraphicsDocumentPanelViewModel;
        }


    }
}
