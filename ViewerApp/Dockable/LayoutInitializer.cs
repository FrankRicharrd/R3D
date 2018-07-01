using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xceed.Wpf.AvalonDock.Layout;
using ViewerApp.ViewModels;
using Viewer.Interfaces;

namespace ViewerApp.Dockable
{
    public class LayoutInitializer : ILayoutUpdateStrategy
    {

        DockablePanelViewModel _dockViewmodel;

        public LayoutInitializer(DockablePanelViewModel vm)
        {
            _dockViewmodel = vm;
        }

        private LayoutAnchorSide GetAnchorSide(RDocumentData.ToolsDockPosition pos, LayoutRoot root)
        {
            switch (pos)
            {
                case RDocumentData.ToolsDockPosition.Top:
                    return root.TopSide;
                case RDocumentData.ToolsDockPosition.Bottom:
                    return root.BottomSide;
                case RDocumentData.ToolsDockPosition.Left:
                    return root.LeftSide;
                case RDocumentData.ToolsDockPosition.Right:
                    return root.RightSide;
                default:
                    return null;
            }
        }


        public bool BeforeInsertAnchorable(LayoutRoot layout, LayoutAnchorable anchorableToShow, ILayoutContainer destinationContainer)
        {
            //AD wants to add the anchorable into destinationContainer
            //just for test provide a new anchorablepane 
            //if the pane is floating let the manager go ahead

            

            LayoutAnchorablePane destPane = destinationContainer as LayoutAnchorablePane;
            if (destinationContainer != null &&
                destinationContainer.FindParent<LayoutFloatingWindow>() != null)
                return false;

            //var toolsPane = layout.Descendents().OfType<LayoutAnchorablePane>().FirstOrDefault(d => d.Name == "ToolsPane");

            Type toolType = anchorableToShow.Content.GetType();

            var docData = _dockViewmodel.Data.DocumentTypeData.FirstOrDefault(a => a.DocType == _dockViewmodel.Data.ActiveDocument.GetType());

            var pos = docData.DicToolDockPosition[toolType];

            LayoutAnchorSide side = GetAnchorSide(pos, layout);


            if (side.Children.Count == 0)
            {
                var group = new LayoutAnchorGroup();



                group.Children.Add(anchorableToShow);


                side.Children.Add(group);
                SetToolPaneProperties(anchorableToShow);

                return true;
            }
            else
            {

                side.Children[0].Children.Add(anchorableToShow);
              
                SetToolPaneProperties(anchorableToShow);
                return true;
            }





            return false;

            //return true;
        }

        private static void SetToolPaneProperties(LayoutAnchorable anchorableToShow)
        {
            anchorableToShow.Show();

            anchorableToShow.ToggleAutoHide();


            var toolpanel = anchorableToShow.Content as Viewer.AdvancedTypes.DockingToolHelper.ToolViewModel;

            var layoutPane = anchorableToShow.Parent as LayoutAnchorablePane;

            layoutPane.DockMinWidth = toolpanel.MinWidth;
            layoutPane.DockMinHeight = toolpanel.MinHeight;
        }

        public void AfterInsertAnchorable(LayoutRoot layout, LayoutAnchorable anchorableShown)
        {
        }


        public bool BeforeInsertDocument(LayoutRoot layout, LayoutDocument anchorableToShow, ILayoutContainer destinationContainer)
        {
            return false;
        }

        public void AfterInsertDocument(LayoutRoot layout, LayoutDocument anchorableShown)
        {

        }
    }
}
