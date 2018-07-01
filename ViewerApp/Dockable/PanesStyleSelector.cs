using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using Viewer.AdvancedTypes.DockingToolHelper;

namespace ViewerApp.Dockable
{
    public class PanesStyleSelector : StyleSelector
    {

        public PanesStyleSelector()
        {

        }

        public Style ToolStyle
        {
            get;
            set;
        }

        public Style DocumentStyle
        {
            get;
            set;
        }

        public override System.Windows.Style SelectStyle(object item, System.Windows.DependencyObject container)
        {
            if (item is ToolViewModel)
                return ToolStyle;

            if (item is DocumentViewModel)
                return DocumentStyle;

            return base.SelectStyle(item, container);
        }
    }
}
