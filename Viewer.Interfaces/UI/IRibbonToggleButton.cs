using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viewer.Interfaces.UI
{
    public interface IRibbonToggleButton : IRibbonButton
    {
        bool? IsChecked { get; set; }

    }
}
