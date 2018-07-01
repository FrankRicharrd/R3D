using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Viewer.Interfaces.UI
{
    public interface IButtonGroup
    {

        List<IRibbonButton> Buttons { get; set; }


        string GroupBoxName { get; set;}

        IRibbonButton CreateButton(string name, string iconpath, ICommand executionCommand);

        IRibbonToggleButton CreateToggleButton(string name, string iconPath, ICommand executionCommand);
    }
}
