using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
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
using ViewerApp.CustomEvents;
using ViewerApp.Dockable;
using ViewerApp.ViewModels;
using Xceed.Wpf.AvalonDock.Layout.Serialization;

namespace ViewerApp.Views
{
    /// <summary>
    /// Interaction logic for DockPanel.xaml
    /// </summary>
    public partial class DockablePanel : UserControl
    {
        public DockablePanel()
        {
            InitializeComponent();


            //LoadLayoutEvent.Instance.Subscribe(OnLoadLayout);

            //SynchronousEvent<SaveLayoutEventArgs>.Instance.Subscribe(OnSaveLayout);

            this.dockManager.LayoutUpdateStrategy = new LayoutInitializer(this.DataContext as DockablePanelViewModel);

            this.dockManager.Layout.ElementAdded += Layout_ElementAdded;

            this.dockManager.Layout.PropertyChanged += Layout_PropertyChanged;

        }

        #region Workspace Layout Management
        /// <summary>
        /// Is executed when PRISM sends a <seealso cref="SynchronousEvent"/> notification
        /// that was initiallized by a third party (viewmodel).
        /// </summary>
        /// <param name="param">Can be used to return a result of this event</param>
        private void OnSaveLayout(SaveLayoutEventArgs param)
        {
            string xmlLayoutString = string.Empty;

            using (StringWriter fs = new StringWriter())
            {
                XmlLayoutSerializer xmlLayout = new XmlLayoutSerializer(this.dockManager);

                xmlLayout.Serialize(fs);

                xmlLayoutString = fs.ToString();
            }

            param.XmlLayout = xmlLayoutString;
        }

        /// <summary>
        /// Is executed when PRISM sends a Xml layout string notification
        /// via a sender which could be a viewmodel that wants to receive
        /// the load the <seealso cref="LoadLayoutEvent"/>.
        /// </summary>
        /// <param name="message"></param>
        private void OnLoadLayout(string message)
        {
            StringReader sr = new StringReader(message);

            var layoutSerializer = new XmlLayoutSerializer(dockManager);
            layoutSerializer.LayoutSerializationCallback += UpdateLayout;
            layoutSerializer.Deserialize(sr);
        }

        /// <summary>
        /// Convert a Avalondock ContentId into a viewmodel instance
        /// that represents a document or tool window. The re-load of
        /// this component is cancelled if the Id cannot be resolved.
        /// 
        /// The result is (viewmodel Id or Cancel) is returned in <paramref name="args"/>.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        private void UpdateLayout(object sender, LayoutSerializationCallbackEventArgs args)
        {

            var resolver = this.DataContext as IDockpanelContentViewmodelResolver;

            if (resolver == null)
                return;

            // Get a matching viewmodel for that view via DataContext property of this view
            INotifyPropertyChanged content_view_model = resolver.ContentViewModelFromID(args.Model.ContentId);

            if (content_view_model == null)
                args.Cancel = true;

            // found a match - return it
            args.Content = content_view_model;
        }

       
        #endregion Workspace Layout Management



        private void Layout_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            
        }

        private void Layout_ElementAdded(object sender, Xceed.Wpf.AvalonDock.Layout.LayoutElementEventArgs e)
        {
            
        }

        private void dockManager_LayoutChanged(object sender, EventArgs e)
        {

        }

        private void ToolsPane_ChildrenCollectionChanged(object sender, EventArgs e)
        {

        }

        private void ToolsPane_ChildrenTreeChanged(object sender, Xceed.Wpf.AvalonDock.Layout.ChildrenTreeChangedEventArgs e)
        {

        }
    }
}
