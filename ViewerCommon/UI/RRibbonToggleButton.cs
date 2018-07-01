using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Viewer.Interfaces.UI;
using Fluent;
namespace ViewerCommon.UI
{
    public class RRibbonToggleButton : ToggleButton, IRibbonToggleButton
    {
        // public ICommand Command { get; set; }

        public RRibbonToggleButton(string name, string iconpath, ICommand executingCommand)
        {
            CommandParameter = this;

            Header = name;
            Name = name;
            IconPath = iconpath;
            Command = executingCommand;
        }

        public string Description { get; set; }
        

        public string IconPath
        {
            get
            {
                return Icon as string;
            }

            set
            {
                Icon = value;
                LargeIcon = value;
            }
        }

        string IRibbonButton.ToolTip
        {
            get
            {

                if (ToolTip is string)
                    return (string)ToolTip;
                else
                    return string.Empty;
            }

            set
            {
                if (ToolTip is string)
                    ToolTip = value;
            }
        }
    }
}
