using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Viewer.Interfaces.UI
{
    public interface IRibbonButton
    {

        string IconPath { get; set; }

        string Name { get; set; }

        string Description { get; set; }

        string ToolTip { get; set; }

        ICommand Command { get; set; }
    }
}
